#!/bin/bash

# TSC Refactored Build Script
# This script builds the TSC Refactored compiler

set -e  # Exit on any error

echo "=== TSC Refactored Build Script ==="

# Check if LLVM_ROOT is set
if [ -z "$LLVM_ROOT" ]; then
    echo "Error: LLVM_ROOT environment variable is not set"
    echo "Please set it to your LLVM installation directory"
    echo "Example: export LLVM_ROOT=/usr/lib/llvm-15"
    exit 1
fi

echo "Using LLVM_ROOT: $LLVM_ROOT"

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake -DLLVM_ROOT="$LLVM_ROOT" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DENABLE_TESTS=ON \
      ..

# Build the project
echo "Building TSC Refactored..."
make -j$(nproc)

echo "Build completed successfully!"
echo ""
echo "Executables created:"
echo "  - tsc: Main compiler executable"
echo "  - Various test executables in tests/"
echo ""
echo "To run tests:"
echo "  make test"
echo ""
echo "To compile a TypeScript file:"
echo "  ./tsc ../examples/hello.ts hello.ll"