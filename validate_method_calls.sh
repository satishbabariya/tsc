#!/bin/bash

echo "=== COMPREHENSIVE METHOD CALL VALIDATION ==="
echo "Date: $(date)"
echo "Testing our method call and console.log fixes"
echo ""

# Create results file
RESULTS_FILE="method_call_validation_$(date +%Y%m%d_%H%M%S).txt"
echo "=== METHOD CALL VALIDATION RESULTS ===" > $RESULTS_FILE
echo "Date: $(date)" >> $RESULTS_FILE
echo "" >> $RESULTS_FILE

# Function to test a specific file
test_file() {
    local file=$1
    local test_name=$(basename "$file")
    echo -n "Testing $test_name: "
    
    # Run the test and capture output
    output=$(./build/tsc "$file" 2>&1)
    error_code=$?
    
    if echo "$output" | grep -q "Compilation failed"; then
        echo "FAILED"
        echo "$test_name: FAILED" >> $RESULTS_FILE
        
        # Capture specific errors
        errors=$(echo "$output" | grep -c "error:")
        if [ $errors -gt 0 ]; then
            echo "  Errors: $errors" >> $RESULTS_FILE
            echo "$output" | grep "error:" | head -3 >> $RESULTS_FILE
        fi
    else
        echo "PASSED"
        echo "$test_name: PASSED" >> $RESULTS_FILE
        
        # Try to run the executable
        executable_name=$(basename "$file" .ts)
        if [ -f "./$executable_name" ]; then
            echo -n "  Running executable: "
            run_output=$(./$executable_name 2>&1)
            if [ $? -eq 0 ]; then
                echo "SUCCESS"
                echo "  Output: $run_output" >> $RESULTS_FILE
            else
                echo "RUNTIME ERROR"
                echo "  Runtime Error: $run_output" >> $RESULTS_FILE
            fi
        fi
    fi
}

# Test our custom method call tests
echo "=== TESTING OUR METHOD CALL FIXES ==="

# Test basic method calls
test_file "test_simple_working.ts"
test_file "test_explicit_types_working.ts"
test_file "test_method_2_args.ts"
test_file "test_method_3_args.ts"
test_file "test_method_with_args.ts"

# Test console.log fixes
test_file "test_console_log_simple.ts"
test_file "test_console_log_2_args.ts"
test_file "test_console_log_comprehensive.ts"

# Test comprehensive method calls
test_file "test_comprehensive_method_calls.ts"
test_file "test_method_calls_final.ts"

# Test regression tests
test_file "test_regression_comprehensive.ts"
test_file "test_regression_console_log_variadic.ts"

echo ""
echo "=== TESTING EXISTING UNIT TESTS WITH METHOD CALLS ==="

# Test unit tests that should work with our fixes
test_file "tests/unit/simple_console_test.ts"
test_file "tests/unit/hello_world_test.ts"
test_file "tests/unit/simple_test.ts"
test_file "tests/unit/function_object_test.ts"
test_file "tests/unit/object_literal_test.ts"
test_file "tests/unit/interface_methods.ts"
test_file "tests/unit/interface_inheritance.ts"

echo ""
echo "=== TESTING EXAMPLES WITH METHOD CALLS ==="

# Test example files
test_file "tests/examples/hello.ts"
test_file "tests/examples/simple_test.ts"
test_file "tests/examples/function_simple.ts"
test_file "tests/examples/simple_call.ts"
test_file "tests/examples/simple_call_fixed.ts"

echo ""
echo "=== VALIDATION COMPLETE ==="
echo "Results saved to: $RESULTS_FILE"

# Count results
passed=$(grep ": PASSED" $RESULTS_FILE | wc -l)
failed=$(grep ": FAILED" $RESULTS_FILE | wc -l)
total=$((passed + failed))

echo ""
echo "=== SUMMARY ==="
echo "Total tests: $total"
echo "Passed: $passed"
echo "Failed: $failed"
if [ $total -gt 0 ]; then
    success_rate=$((passed * 100 / total))
    echo "Success rate: $success_rate%"
fi