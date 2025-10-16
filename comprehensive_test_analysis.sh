#!/bin/bash

echo "=== COMPREHENSIVE TEST SUITE ANALYSIS ==="
echo "Date: $(date)"
echo ""

# Function to test a directory
test_directory() {
    local dir=$1
    local dir_name=$(basename "$dir")
    local total=0
    local passed=0
    local failed=0
    
    echo "Testing directory: $dir_name"
    echo "----------------------------------------"
    
    if [ -d "$dir" ]; then
        for test_file in "$dir"/*.ts; do
            if [ -f "$test_file" ]; then
                total=$((total + 1))
                test_name=$(basename "$test_file")
                echo -n "  $test_name: "
                
                # Run the test and capture output
                output=$(./build/tsc "$test_file" 2>&1)
                
                if echo "$output" | grep -q "Compilation failed"; then
                    failed=$((failed + 1))
                    echo "FAILED"
                else
                    passed=$((passed + 1))
                    echo "PASSED"
                fi
            fi
        done
        
        echo "  Summary: $passed/$total passed, $failed/$total failed"
    else
        echo "  Directory not found"
    fi
    echo ""
}

# Test all directories
for dir in tests/*/; do
    test_directory "$dir"
done

# Test subdirectories
for subdir in tests/*/*/; do
    if [ -d "$subdir" ]; then
        test_directory "$subdir"
    fi
done

echo "=== ANALYSIS COMPLETE ==="
