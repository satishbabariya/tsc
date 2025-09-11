#!/bin/bash

# Build script with Enhanced IRGen integration
# This script builds the TSC compiler with enhanced IRGen and runs integration tests

set -e

echo "🔧 Building TSC with Enhanced IRGen..."

# Build the compiler
echo "📦 Building compiler..."
mkdir -p build
cd build

# Configure with CMake
echo "⚙️ Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_ENHANCED_IRGEN=ON

# Build with parallel jobs
echo "🔨 Building with parallel jobs..."
make -j$(nproc)

# Check if build was successful
if [ ! -f "tsc" ]; then
    echo "❌ Build failed - tsc executable not found"
    exit 1
fi

echo "✅ Build completed successfully"

# Run integration tests
echo "🧪 Running integration tests..."
cd ..

# Make test scripts executable
chmod +x tests/integration/test_environment_setup.sh
chmod +x tests/integration/run_integration_tests.py
chmod +x tests/integration/validate_llvm_ir.py
chmod +x tests/integration/validate_memory.py

# Setup test environment
echo "🏗️ Setting up test environment..."
./tests/integration/test_environment_setup.sh

# Run integration tests
echo "🚀 Running comprehensive integration tests..."
python3 tests/integration/run_integration_tests.py

# Check test results
if [ $? -eq 0 ]; then
    echo "🎉 All integration tests passed!"
else
    echo "❌ Some integration tests failed"
    exit 1
fi

echo "✅ Enhanced IRGen integration complete!"