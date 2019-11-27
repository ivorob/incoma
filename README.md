# incoma

## macOS building

mkdir build  
cd build  
cmake -G "Unix Makefiles" &lt;source dir&gt; -DCMAKE_PREFIX_PATH=&lt;path to Qt clan dir&gt;  
cmake --build .  

# macOS running
cmake --build . --target run  
