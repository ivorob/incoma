#include <QApplication>
#include <QWidget>

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget widget;
    widget.setWindowTitle("Hello, World!");
    widget.show();
    return app.exec();
}
