cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target main -- -j $(nproc)
build/main
