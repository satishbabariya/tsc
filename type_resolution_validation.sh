#!/bin/bash

echo "=== TYPE RESOLUTION VALIDATION ==="
echo "Date: $(date)"
echo "Testing type resolution capabilities systematically"
echo ""

# Create results file
RESULTS_FILE="type_resolution_results_$(date +%Y%m%d_%H%M%S).txt"
echo "=== TYPE RESOLUTION VALIDATION RESULTS ===" > $RESULTS_FILE
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
            echo "$output" | grep "error:" | head -5 >> $RESULTS_FILE
        fi
        
        # Capture warnings
        warnings=$(echo "$output" | grep -c "warning:")
        if [ $warnings -gt 0 ]; then
            echo "  Warnings: $warnings" >> $RESULTS_FILE
            echo "$output" | grep "warning:" | head -3 >> $RESULTS_FILE
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

# Test basic type resolution
echo "=== TESTING BASIC TYPE RESOLUTION ==="
test_file "type_resolution_tests/basic/primitive_types.ts"
test_file "type_resolution_tests/basic/function_types.ts"
test_file "type_resolution_tests/basic/class_types.ts"

echo ""
echo "=== TESTING COMPLEX TYPE RESOLUTION ==="
test_file "type_resolution_tests/complex/generic_types.ts"
test_file "type_resolution_tests/complex/interface_types.ts"

echo ""
echo "=== TESTING EDGE CASES ==="
test_file "type_resolution_tests/edge_cases/union_types.ts"
test_file "type_resolution_tests/edge_cases/array_types.ts"

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