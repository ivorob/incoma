# incoma

## macOS building

mkdir build
cd build
cmake -G "Unix Makefiles" <source dir> -DCMAKE_PREFIX_PATH=<path to Qt clan dir>
cmake --build .

# macOS running
cmake --build . --target run
