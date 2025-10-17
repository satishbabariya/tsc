#!/bin/bash

echo "=== COMPREHENSIVE CODEBASE VALIDATION ==="
echo "Date: $(date)"
echo "Testing all directories systematically with our method call and console.log fixes"
echo ""

# Create results file
RESULTS_FILE="validation_results_$(date +%Y%m%d_%H%M%S).txt"
echo "=== COMPREHENSIVE VALIDATION RESULTS ===" > $RESULTS_FILE
echo "Date: $(date)" >> $RESULTS_FILE
echo "" >> $RESULTS_FILE

# Function to test a directory
test_directory() {
    local dir=$1
    local dir_name=$(basename "$dir")
    local total=0
    local passed=0
    local failed=0
    local errors=0
    
    echo "Testing directory: $dir_name"
    echo "----------------------------------------"
    echo "Testing directory: $dir_name" >> $RESULTS_FILE
    echo "----------------------------------------" >> $RESULTS_FILE
    
    if [ -d "$dir" ]; then
        for test_file in "$dir"/*.ts; do
            if [ -f "$test_file" ]; then
                total=$((total + 1))
                test_name=$(basename "$test_file")
                echo -n "  $test_name: "
                
                # Run the test and capture output
                output=$(./build/tsc "$test_file" 2>&1)
                error_code=$?
                
                if echo "$output" | grep -q "Compilation failed"; then
                    failed=$((failed + 1))
                    echo "FAILED"
                    echo "  $test_name: FAILED" >> $RESULTS_FILE
                    
                    # Capture specific errors
                    errors=$(echo "$output" | grep -c "error:")
                    if [ $errors -gt 0 ]; then
                        echo "    Errors: $errors" >> $RESULTS_FILE
                        echo "$output" | grep "error:" | head -3 >> $RESULTS_FILE
                    fi
                else
                    passed=$((passed + 1))
                    echo "PASSED"
                    echo "  $test_name: PASSED" >> $RESULTS_FILE
                fi
            fi
        done
        
        echo "  Summary: $passed/$total passed, $failed/$total failed"
        echo "  Summary: $passed/$total passed, $failed/$total failed" >> $RESULTS_FILE
    else
        echo "  Directory not found"
        echo "  Directory not found" >> $RESULTS_FILE
    fi
    echo ""
    echo "" >> $RESULTS_FILE
}

# Test all main directories
echo "=== TESTING MAIN DIRECTORIES ==="
for dir in tests/*/; do
    test_directory "$dir"
done

# Test subdirectories
echo "=== TESTING SUBDIRECTORIES ==="
for subdir in tests/*/*/; do
    if [ -d "$subdir" ]; then
        test_directory "$subdir"
    fi
done

# Calculate overall statistics
echo "=== OVERALL STATISTICS ==="
echo "Calculating overall statistics..."

total_tests=$(grep "Summary:" $RESULTS_FILE | sed 's/.*Summary: \([0-9]*\)\/\([0-9]*\) passed.*/\2/' | awk '{sum += $1} END {print sum}')
total_passed=$(grep "Summary:" $RESULTS_FILE | sed 's/.*Summary: \([0-9]*\)\/\([0-9]*\) passed.*/\1/' | awk '{sum += $1} END {print sum}')
total_failed=$((total_tests - total_passed))
success_rate=$((total_passed * 100 / total_tests))

echo "Total tests: $total_tests"
echo "Passed: $total_passed"
echo "Failed: $total_failed"
echo "Success rate: $success_rate%"

echo "" >> $RESULTS_FILE
echo "=== OVERALL STATISTICS ===" >> $RESULTS_FILE
echo "Total tests: $total_tests" >> $RESULTS_FILE
echo "Passed: $total_passed" >> $RESULTS_FILE
echo "Failed: $total_failed" >> $RESULTS_FILE
echo "Success rate: $success_rate%" >> $RESULTS_FILE

echo ""
echo "=== VALIDATION COMPLETE ==="
echo "Results saved to: $RESULTS_FILE"