#!/bin/bash
set -e

echo "Building TSC compiler with coverage instrumentation..."

# Clean previous build
rm -rf build-coverage
mkdir -p build-coverage
cd build-coverage

# Configure with coverage flags
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage -g -O0" \
      -DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage" \
      -DLLVM_ROOT=/usr/lib/llvm-18 \
      ..

# Build with coverage
make -j$(nproc)

echo "Coverage build complete"
