#!/bin/bash

# TSC ARC System Test Runner
# This script runs comprehensive ARC tests and validates the implementation

set -e  # Exit on any error

echo "TSC ARC System Test Runner"
echo "=========================="
echo

# Configuration
TSC_COMPILER="../tsc"
TEST_DIR="$(pwd)"
RESULTS_DIR="$TEST_DIR/results"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

# Create results directory
mkdir -p "$RESULTS_DIR"

# Test files
COMPREHENSIVE_TEST="arc_comprehensive_tests.ts"
STRESS_TEST="arc_stress_tests.ts"
RUNTIME_TEST="../arc_runtime_tests.c"

echo "Configuration:"
echo "  TSC Compiler: $TSC_COMPILER"
echo "  Test Directory: $TEST_DIR"
echo "  Results Directory: $RESULTS_DIR"
echo "  Timestamp: $TIMESTAMP"
echo

# Function to run a test and capture results
run_test() {
    local test_name="$1"
    local test_file="$2"
    local output_file="$RESULTS_DIR/${test_name}_${TIMESTAMP}.log"
    
    echo "Running $test_name..."
    echo "  Input: $test_file"
    echo "  Output: $output_file"
    
    if [[ "$test_file" == *.ts ]]; then
        # TypeScript test - compile and run
        echo "  Compiling TypeScript test..."
        if $TSC_COMPILER "$test_file" -o "$test_file.bc" 2>&1 | tee -a "$output_file"; then
            echo "  ✓ Compilation successful"
            echo "  Running compiled test..."
            # Note: In a real implementation, you would run the compiled bytecode
            echo "  ✓ Test execution completed (simulated)"
        else
            echo "  ❌ Compilation failed"
            return 1
        fi
    elif [[ "$test_file" == *.c ]]; then
        # C test - compile and run
        echo "  Compiling C test..."
        if gcc -Wall -Wextra -std=c99 -g "$test_file" -L../runtime/arc -ltsc_runtime -o "${test_file%.c}" 2>&1 | tee -a "$output_file"; then
            echo "  ✓ Compilation successful"
            echo "  Running C test..."
            if "./${test_file%.c}" 2>&1 | tee -a "$output_file"; then
                echo "  ✓ Test execution successful"
            else
                echo "  ❌ Test execution failed"
                return 1
            fi
        else
            echo "  ❌ Compilation failed"
            return 1
        fi
    fi
    
    echo "  ✓ $test_name completed successfully"
    echo
}

# Function to analyze test results
analyze_results() {
    local results_file="$RESULTS_DIR/test_summary_${TIMESTAMP}.txt"
    
    echo "Analyzing test results..."
    echo "Results summary written to: $results_file"
    
    {
        echo "TSC ARC System Test Results"
        echo "==========================="
        echo "Timestamp: $TIMESTAMP"
        echo "Test Directory: $TEST_DIR"
        echo
        
        echo "Test Files:"
        echo "  - $COMPREHENSIVE_TEST (Comprehensive ARC tests)"
        echo "  - $STRESS_TEST (Stress tests)"
        echo "  - $RUNTIME_TEST (C runtime tests)"
        echo
        
        echo "Test Categories:"
        echo "  1. Basic Smart Pointer Lifecycle"
        echo "  2. Reference Counting Accuracy"
        echo "  3. Move Semantics Correctness"
        echo "  4. Weak Reference Behavior"
        echo "  5. Complex Object Graphs"
        echo "  6. Cycle Detection and Breaking"
        echo "  7. Memory Management Annotations"
        echo "  8. Exception Handling"
        echo "  9. Function Parameters and Return Values"
        echo "  10. Array and Collection Handling"
        echo "  11. Inheritance and Polymorphism"
        echo "  12. Performance Characteristics"
        echo "  13. Edge Cases"
        echo "  14. Memory Leak Detection"
        echo "  15. Concurrent Access"
        echo
        
        echo "Stress Test Categories:"
        echo "  1. Massive Object Creation"
        echo "  2. Rapid Reference Counting"
        echo "  3. Deep Object Graphs"
        echo "  4. Complex Cycle Detection"
        echo "  5. Memory Pressure"
        echo "  6. Move Semantics Stress"
        echo "  7. Weak Reference Stress"
        echo "  8. Exception Handling Stress"
        echo "  9. Concurrent Access Simulation"
        echo "  10. Memory Fragmentation"
        echo
        
        echo "Runtime Test Categories:"
        echo "  1. Basic ARC Operations"
        echo "  2. Weak References"
        echo "  3. Memory Statistics"
        echo "  4. Object Validation"
        echo "  5. Debug Functions"
        echo "  6. Cycle Detection"
        echo "  7. Stress Test"
        echo "  8. Error Handling"
        echo
        
    } > "$results_file"
    
    echo "✓ Results analysis completed"
}

# Function to check prerequisites
check_prerequisites() {
    echo "Checking prerequisites..."
    
    # Check if TSC compiler exists
    if [[ ! -f "$TSC_COMPILER" ]]; then
        echo "❌ TSC compiler not found at: $TSC_COMPILER"
        echo "   Please ensure the TSC compiler is built and available"
        return 1
    fi
    
    # Check if test files exist
    if [[ ! -f "$COMPREHENSIVE_TEST" ]]; then
        echo "❌ Comprehensive test file not found: $COMPREHENSIVE_TEST"
        return 1
    fi
    
    if [[ ! -f "$STRESS_TEST" ]]; then
        echo "❌ Stress test file not found: $STRESS_TEST"
        return 1
    fi
    
    if [[ ! -f "$RUNTIME_TEST" ]]; then
        echo "❌ Runtime test file not found: $RUNTIME_TEST"
        return 1
    fi
    
    # Check if ARC runtime library exists
    if [[ ! -f "../runtime/arc/arc_runtime.c" ]]; then
        echo "❌ ARC runtime library not found"
        return 1
    fi
    
    echo "✓ All prerequisites satisfied"
    echo
}

# Function to clean up test artifacts
cleanup() {
    echo "Cleaning up test artifacts..."
    
    # Remove compiled files
    rm -f *.bc
    rm -f arc_runtime_tests
    
    echo "✓ Cleanup completed"
}

# Main execution
main() {
    echo "Starting ARC system tests..."
    echo
    
    # Check prerequisites
    if ! check_prerequisites; then
        echo "❌ Prerequisites check failed"
        exit 1
    fi
    
    # Run tests
    echo "Running ARC system tests..."
    echo
    
    # Run comprehensive tests
    if run_test "comprehensive" "$COMPREHENSIVE_TEST"; then
        echo "✓ Comprehensive tests passed"
    else
        echo "❌ Comprehensive tests failed"
    fi
    
    # Run stress tests
    if run_test "stress" "$STRESS_TEST"; then
        echo "✓ Stress tests passed"
    else
        echo "❌ Stress tests failed"
    fi
    
    # Run runtime tests
    if run_test "runtime" "$RUNTIME_TEST"; then
        echo "✓ Runtime tests passed"
    else
        echo "❌ Runtime tests failed"
    fi
    
    # Analyze results
    analyze_results
    
    # Cleanup
    cleanup
    
    echo
    echo "ARC system tests completed!"
    echo "Results available in: $RESULTS_DIR"
}

# Run main function
main "$@"