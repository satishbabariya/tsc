#!/bin/bash
set -e

echo "Running test suite with coverage instrumentation..."

# Set environment variables for coverage
export GCOV_PREFIX="$(pwd)/coverage_data"
export GCOV_PREFIX_STRIP=0

# Create coverage data directory
mkdir -p coverage_data

# Run comprehensive error testing
echo "Running comprehensive error testing..."
./comprehensive_error_testing

# Run advanced transformation testing
echo "Running advanced transformation testing..."
./advanced_transformation_testing

# Run individual test cases
echo "Running individual test cases..."
for test_file in *.ts; do
    if [ -f "$test_file" ]; then
        echo "Testing: $test_file"
        ./build-coverage/tsc "$test_file" 2>/dev/null || true
    fi
done

# Run transformation framework tests
echo "Running transformation framework tests..."
for test_file in test_*.ts; do
    if [ -f "$test_file" ]; then
        echo "Testing: $test_file"
        ./build-coverage/tsc "$test_file" 2>/dev/null || true
    fi
done

echo "Test suite execution complete"
