#!/bin/bash

# Module Resolution Fix Test Runner
# This script validates the fix for the import/export module resolution issue

set -e

echo "🧪 Running Module Resolution Fix Tests"
echo "======================================"

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ Error: Must be run from the project root directory"
    exit 1
fi

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir -p build
fi

# Check if LLVM_ROOT is set
if [ -z "$LLVM_ROOT" ]; then
    echo "⚠️  Warning: LLVM_ROOT is not set. Tests may not compile."
    echo "   Set LLVM_ROOT to the path of your LLVM installation"
    echo "   Example: export LLVM_ROOT=/usr/lib/llvm-20"
fi

# Build the tests
echo "🔨 Building tests..."
cd build

if [ -n "$LLVM_ROOT" ]; then
    cmake -DLLVM_ROOT=$LLVM_ROOT ..
else
    echo "⚠️  Attempting to build without LLVM_ROOT..."
    cmake ..
fi

make -j$(nproc) tsc_tests

if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo "✅ Build successful!"

# Run the specific module resolution tests
echo "🧪 Running Module Resolution Tests..."
echo "====================================="

# Run semantic analyzer tests
echo "📋 Running Semantic Analyzer Tests..."
./tsc_tests --gtest_filter="SemanticAnalyzerTest.*"

if [ $? -eq 0 ]; then
    echo "✅ Semantic Analyzer Tests PASSED"
else
    echo "❌ Semantic Analyzer Tests FAILED"
    exit 1
fi

# Run module resolution fix tests
echo "📋 Running Module Resolution Fix Tests..."
./tsc_tests --gtest_filter="ModuleResolutionFixTest.*"

if [ $? -eq 0 ]; then
    echo "✅ Module Resolution Fix Tests PASSED"
else
    echo "❌ Module Resolution Fix Tests FAILED"
    exit 1
fi

# Run integration tests
echo "📋 Running Module Resolution Integration Tests..."
./tsc_tests --gtest_filter="ModuleResolutionIntegrationTest.*"

if [ $? -eq 0 ]; then
    echo "✅ Module Resolution Integration Tests PASSED"
else
    echo "❌ Module Resolution Integration Tests FAILED"
    exit 1
fi

# Run all tests to check for regressions
echo "📋 Running All Tests (Regression Check)..."
./tsc_tests

if [ $? -eq 0 ]; then
    echo "✅ All Tests PASSED - No Regressions!"
else
    echo "❌ Some Tests FAILED - Possible Regressions!"
    exit 1
fi

echo ""
echo "🎉 Module Resolution Fix Validation Complete!"
echo "=============================================="
echo "✅ All tests passed successfully"
echo "✅ No regressions detected"
echo "✅ The fix is working correctly"
echo ""
echo "📊 Test Summary:"
echo "  - Semantic Analyzer Tests: PASSED"
echo "  - Module Resolution Fix Tests: PASSED"
echo "  - Integration Tests: PASSED"
echo "  - Regression Tests: PASSED"
echo ""
echo "🚀 The import/export module resolution fix is validated!"