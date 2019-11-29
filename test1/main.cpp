#include <iostream>
#include <algorithm>
#include <string.h>

/*class TestClass {
public:
    TestClass() {
        x1 = new char[1024];
        x2 = new char[1024 * 1024];
    }
	
private:
	char *x1;
	char *x2;
};

Проблема данного класса - утечка памяти. Добавление в деструктор
оператора delete[] не решает проблемы.

class TestClass {
public:
    TestClass() {
        x1 = new char[1024];
        x2 = new char[1024 * 1024];
    }

    ~TestClass() {
        delete[] x1;
        delete[] x2;
    }
	
private:
	char *x1;
	char *x2;
};

Сейчас мы имеем удаление памяти в деструкторе, однако, проблема
с утечкой памяти остается, т.к. оператор new может генерировать
исключение std::bad_alloc. Это не страшно, если исключение произошло
при выделение первого участка памяти (x1) - проблему генерирует 
исключение при выделение второго участка памяти (x2), т.к.
первый участок памяти остается не освобожден. Деструктор не вызывается,
т.к. объект еще не сконструирован. Одним из вариантов обработки данной ситуации
- перехват std::bad_alloc (другие варианты - new (std::nothrow) и smart pointers).
В блоке catch мы должны вызвать оператор delete[].  Опять же, простой вызов 
оператора delete[] не пройдет - если исключение произойдет при выделение первого
блока памяти (x1), указатель на второй блок памяти (x2) будет неинициализирован.
Поэтому поместим вначале конструктора инициализацию.

class TestClass {
public:
    TestClass()
      : x1(),
        x2()
    {
        try {
            x1 = new char[1024];
            x2 = new char[1024 * 1024];
        } catch (std::bad_alloc) {
            delete[] x1;
            delete[] x2;

            throw; //пробросим дальше
        }
    }

    ~TestClass() {
        delete[] x1;
        delete[] x2;
    }
	
private:
	char *x1;
	char *x2;
};

Рефакторинг согласно принципу DRY (Don't repeat yourself):

class TestClass {
public:
    TestClass()
      : x1(),
        x2()
    {
        try {
            x1 = new char[1024];
            x2 = new char[1024 * 1024];
        } catch (std::bad_alloc) {
            freeMemory();
            
            throw;
        }
    }

    ~TestClass() {
        freeMemory();
    }
private:
    void freeMemory() {
        delete[] x1;
        delete[] x2;
    }
private:
	char *x1;
	char *x2;
};

Реализация конструктора перемещения и оператора перемещения:

class TestClass {
public:
    TestClass()
      : x1(),
        x2()
    {
        allocBuffer();
    }

    TestClass(TestClass&& other)
        : x1(),
          x2()
    {
        allocBuffer();
        swap(other);
    }

    TestClass& operator=(TestClass&& other) {
        TestClass temp;
        temp.swap(other);
        swap(temp);
        return *this;
    }

    ~TestClass() {
        freeMemory();
    }
private:
    void allocBuffer() {
        try {
            x1 = new char[1024];
            x2 = new char[1024 * 1024];
        } catch (std::bad_alloc) {
            freeMemory();
            
            throw;
        }
    }

    void freeMemory() {
        delete[] x1;
        delete[] x2;
    }

    void swap(TestClass& other) {
        std::swap(x1, other.x1);
        std::swap(x2, other.x2);
    }
private:
	char *x1;
	char *x2;
};

Предполагаем, что x1 и x2 не может быть нулевым. Иначе понадобится
проверка указателей во всех операциях с ними. Оператор перемещения
реализован через временный объект, чтобы в любой момент генерации
исключения наши объекты имели валидные значения. 
Реализация конструктора копирования и оператора копирования:

class TestClass {
public:
    TestClass()
      : x1(),
        x2()
    {
        allocBuffer();
    }

    TestClass(const TestClass& other)
        : x1(),
          x2()
    {
        allocBuffer();
        copy(other);
    }

    TestClass(TestClass&& other)
        : x1(),
          x2()
    {
        allocBuffer();
        swap(other);
    }

    TestClass& operator=(TestClass&& other) {
        TestClass temp;
        temp.swap(other);
        swap(temp);
        return *this;
    }

    TestClass& operator=(const TestClass& other) {
        copy(other);
        return *this;
    }

    ~TestClass() {
        freeMemory();
    }
private:
    void allocBuffer() {
        try {
            x1 = new char[1024];
            x2 = new char[1024 * 1024];
        } catch (std::bad_alloc) {
            freeMemory();
            
            throw;
        }
    }

    void freeMemory() {
        delete[] x1;
        delete[] x2;
    }

    void swap(TestClass& other) {
        std::swap(x1, other.x1);
        std::swap(x2, other.x2);
    }

    void copy(const TestClass& other) {
        memcpy(x1, other.x1, 1024);
        memcpy(x2, other.x2, 1024 * 1024);
    }
private:
	char *x1;
	char *x2;
};

Рефакторинг согласно принципу DRY (Don't repeat yourself):

class TestClass {
public:
    TestClass()
      : x1(),
        x2()
    {
        allocBuffer();
    }

    TestClass(const TestClass& other)
        : x1(),
          x2()
    {
        allocBuffer();
        copy(other);
    }

    TestClass(TestClass&& other)
        : x1(),
          x2()
    {
        allocBuffer();
        swap(other);
    }

    TestClass& operator=(TestClass&& other) {
        TestClass temp;
        temp.swap(other);
        swap(temp);
        return *this;
    }

    TestClass& operator=(const TestClass& other) {
        copy(other);
        return *this;
    }

    ~TestClass() {
        freeMemory();
    }
private:
    const int FIRST_BUFFER_SIZE = 1024;
    const int SECOND_BUFFER_SIZE = 1024 * 1024;
private:
    void allocBuffer() {
        try {
            x1 = new char[FIRST_BUFFER_SIZE];
            x2 = new char[SECOND_BUFFER_SIZE];
        } catch (std::bad_alloc) {
            freeMemory();
            
            throw;
        }
    }

    void freeMemory() {
        delete[] x1;
        delete[] x2;
    }

    void swap(TestClass& other) {
        std::swap(x1, other.x1);
        std::swap(x2, other.x2);
    }

    void copy(const TestClass& other) {
        memcpy(x1, other.x1, FIRST_BUFFER_SIZE);
        memcpy(x2, other.x2, SECOND_BUFFER_SIZE);
    }
private:
	char *x1;
	char *x2;
};

В идеале обнулим память выделяемого буфера 
(memset в функции-члена allocBuffer):

class TestClass {
public:
    TestClass()
      : x1(),
        x2()
    {
        allocBuffer();
    }

    TestClass(const TestClass& other)
        : x1(),
          x2()
    {
        allocBuffer();
        copy(other);
    }

    TestClass(TestClass&& other)
        : x1(),
          x2()
    {
        allocBuffer();
        swap(other);
    }

    TestClass& operator=(TestClass&& other) {
        TestClass temp;
        temp.swap(other);
        swap(temp);
        return *this;
    }

    TestClass& operator=(const TestClass& other) {
        copy(other);
        return *this;
    }

    ~TestClass() {
        freeMemory();
    }
private:
    const int FIRST_BUFFER_SIZE = 1024;
    const int SECOND_BUFFER_SIZE = 1024 * 1024;
private:
    void allocBuffer() {
        try {
            x1 = new char[FIRST_BUFFER_SIZE];
            x2 = new char[SECOND_BUFFER_SIZE];

            memset(x1, 0, FIRST_BUFFER_SIZE);
            memset(x2, 0, SECOND_BUFFER_SIZE);
        } catch (std::bad_alloc) {
            freeMemory();
            
            throw;
        }
    }

    void freeMemory() {
        delete[] x1;
        delete[] x2;
    }

    void swap(TestClass& other) {
        std::swap(x1, other.x1);
        std::swap(x2, other.x2);
    }

    void copy(const TestClass& other) {
        memcpy(x1, other.x1, FIRST_BUFFER_SIZE);
        memcpy(x2, other.x2, SECOND_BUFFER_SIZE);
    }
private:
	char *x1;
	char *x2;
};

Перекладываем управление памятью на smart pointers или std::vector:

class TestClass {
public:
    TestClass()
    {
        allocBuffer();
    }

    TestClass(const TestClass& other)
    {
        allocBuffer();
        copy(other);
    }

    TestClass(TestClass&& other)
    {
        allocBuffer();
        swap(other);
    }

    TestClass& operator=(TestClass&& other) {
        TestClass temp;
        temp.swap(other);
        swap(temp);
        return *this;
    }

    TestClass& operator=(const TestClass& other) {
        copy(other);
        return *this;
    }
private:
    const int FIRST_BUFFER_SIZE = 1024;
    const int SECOND_BUFFER_SIZE = 1024 * 1024;
private:
    void allocBuffer() {
        x1.reset(new char[FIRST_BUFFER_SIZE]);
        x2.reset(new char[SECOND_BUFFER_SIZE]);

        memset(x1.get(), 0, FIRST_BUFFER_SIZE);
        memset(x2.get(), 0, SECOND_BUFFER_SIZE);
    }

    void swap(TestClass& other) {
        std::swap(x1, other.x1);
        std::swap(x2, other.x2);
    }

    void copy(const TestClass& other) {
        memcpy(x1.get(), other.x1.get(), FIRST_BUFFER_SIZE);
        memcpy(x2.get(), other.x2.get(), SECOND_BUFFER_SIZE);
    }
private:
    std::unique_ptr<char[]> x1;
    std::unique_ptr<char[]> x2;
};

*/

class TestClass {
public:
    TestClass()
    {
        allocBuffer();
    }

    TestClass(const TestClass& other)
    {
        allocBuffer();
        copy(other);
    }

    TestClass(TestClass&& other)
    {
        allocBuffer();
        swap(other);
    }

    TestClass& operator=(TestClass&& other) {
        allocBuffer();
        swap(other);
        return *this;
    }

    TestClass& operator=(const TestClass& other) {
        copy(other);
        return *this;
    }

    char *getFirstMemoryBlock() const {
        return x1.get();
    }

    char *getSecondMemoryBlock() const {
        return x2.get();
    }
private:
    const int FIRST_BUFFER_SIZE = 1024;
    const int SECOND_BUFFER_SIZE = 1024 * 1024;
private:
    void allocBuffer() {
        x1.reset(new char[FIRST_BUFFER_SIZE]);
        x2.reset(new char[SECOND_BUFFER_SIZE]);

        memset(x1.get(), 0, FIRST_BUFFER_SIZE);
        memset(x2.get(), 0, SECOND_BUFFER_SIZE);
    }

    void swap(TestClass& other) {
        std::swap(x1, other.x1);
        std::swap(x2, other.x2);
    }

    void copy(const TestClass& other) {
        memcpy(x1.get(), other.x1.get(), FIRST_BUFFER_SIZE);
        memcpy(x2.get(), other.x2.get(), SECOND_BUFFER_SIZE);
    }
private:
    std::unique_ptr<char[]> x1;
    std::unique_ptr<char[]> x2;
};

void dump(const TestClass& test, const std::string& name)
{
    std::cout << name << ":" << std::endl;
    std::cout 
        << "First: "
        << reinterpret_cast<void *>(test.getFirstMemoryBlock())
        << std::endl
        << "Second: "
        << reinterpret_cast<void *>(test.getSecondMemoryBlock())
        << std::endl;
    std::cout << std::endl;
}

int
main()
{
    TestClass test1;
    TestClass test2;
    dump(test1, "Test1");
    dump(test2, "Test2");
    test1 = std::move(test2);
    dump(test1, "Move test2 to test1");
    dump(test2, "New Test2");

    return 0;
}
