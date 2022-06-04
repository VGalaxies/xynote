cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target main -- -j $(nproc)
build/main
