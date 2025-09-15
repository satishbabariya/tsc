#!/bin/bash

# Simple Test Runner for RAII and ARC
# This script runs basic functional tests

echo "🧪 TSC RAII & ARC Test Suite"
echo "=============================="

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_file="$2"
    local test_type="$3"
    
    echo ""
    echo "🔍 Running $test_name..."
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ "$test_type" = "tsc" ]; then
        # Compile and run TypeScript test
        if ../build/tsc "$test_file" > /dev/null 2>&1; then
            local executable="${test_file%.ts}"
            if [ -f "$executable" ]; then
                if ./"$executable" > /dev/null 2>&1; then
                    echo "   ✅ PASSED: $test_name"
                    PASSED_TESTS=$((PASSED_TESTS + 1))
                else
                    echo "   ❌ FAILED: $test_name (runtime error)"
                    FAILED_TESTS=$((FAILED_TESTS + 1))
                fi
            else
                echo "   ❌ FAILED: $test_name (no executable generated)"
                FAILED_TESTS=$((FAILED_TESTS + 1))
            fi
        else
            echo "   ❌ FAILED: $test_name (compilation error)"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    elif [ "$test_type" = "c" ]; then
        # Compile and run C test
        local executable="${test_file%.c}"
        if gcc -I../runtime -o "$executable" "$test_file" ../runtime/arc/arc_runtime.c > /dev/null 2>&1; then
            if ./"$executable" > /dev/null 2>&1; then
                echo "   ✅ PASSED: $test_name"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            else
                echo "   ❌ FAILED: $test_name (runtime error)"
                FAILED_TESTS=$((FAILED_TESTS + 1))
            fi
        else
            echo "   ❌ FAILED: $test_name (compilation error)"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
}

# Run tests
run_test "Minimal RAII Test" "minimal_raii_test.ts" "tsc"
run_test "Simple RAII Test" "simple_raii_test.ts" "tsc"
run_test "Minimal ARC Test" "minimal_arc_test.c" "c"

# Print results
echo ""
echo "📊 Test Results"
echo "==============="
echo "Total Tests: $TOTAL_TESTS"
echo "Passed: $PASSED_TESTS"
echo "Failed: $FAILED_TESTS"

if [ $TOTAL_TESTS -gt 0 ]; then
    success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo "Success Rate: $success_rate%"
fi

echo ""
if [ $FAILED_TESTS -eq 0 ]; then
    echo "🎉 All tests passed!"
else
    echo "⚠️  Some tests failed. Check the output above for details."
fi