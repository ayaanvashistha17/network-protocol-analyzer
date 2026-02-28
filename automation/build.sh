
#!/bin/bash
set -e
mkdir -p build
cd build
cmake ..
make
echo "Build complete. Executables are in build/"
