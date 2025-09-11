#!/bin/bash

echo "=== Valgrind Memory Leak Detection Test ==="
echo "==========================================="

# Check if valgrind is available
if ! command -v valgrind &> /dev/null; then
    echo "Valgrind not found. Installing valgrind..."
    if command -v apt-get &> /dev/null; then
        sudo apt-get update && sudo apt-get install -y valgrind
    elif command -v yum &> /dev/null; then
        sudo yum install -y valgrind
    else
        echo "Cannot install valgrind automatically. Please install it manually."
        exit 1
    fi
fi

# Compile the validation tests
echo "Compiling validation tests..."
gcc -g -I. arc_validation_tests.c runtime_arc.c runtime.c -o arc_validation_tests -lpthread

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Run valgrind tests
echo "Running Valgrind memory leak detection..."

echo -e "\n--- Test 1: Basic Functionality ---"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    --error-exitcode=1 ./arc_validation_tests

if [ $? -eq 0 ]; then
    echo "✅ Basic functionality test passed Valgrind"
else
    echo "❌ Basic functionality test failed Valgrind"
fi

echo -e "\n--- Test 2: Regression Test ---"
gcc -g -I. arc_regression_test.c runtime_arc.c runtime.c -o arc_regression_test
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    --error-exitcode=1 ./arc_regression_test

if [ $? -eq 0 ]; then
    echo "✅ Regression test passed Valgrind"
else
    echo "❌ Regression test failed Valgrind"
fi

echo -e "\n--- Test 3: Extended Tests ---"
gcc -g -I. extended_arc_tests.c runtime_arc.c runtime.c -o extended_arc_tests
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    --error-exitcode=1 ./extended_arc_tests

if [ $? -eq 0 ]; then
    echo "✅ Extended tests passed Valgrind"
else
    echo "❌ Extended tests failed Valgrind"
fi

echo -e "\n--- Valgrind Summary ---"
echo "All Valgrind tests completed. Check output above for any memory leaks."