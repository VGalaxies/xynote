cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target xynote -- -j $(nproc)
build/xynote
