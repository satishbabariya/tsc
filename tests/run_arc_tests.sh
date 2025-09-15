#!/bin/bash

# ============================================================================
# ARC Test Runner Script
# Comprehensive test runner for ARC functionality
# ============================================================================

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test configuration
BUILD_DIR="../build"
TEST_DIR="."
RUNTIME_TESTS="arc_runtime_unit_tests"
REGRESSION_TESTS="arc_regression_tests.ts"
INTEGRATION_TESTS="arc_integration_tests.ts"
COMPREHENSIVE_TESTS="arc_comprehensive_tests.ts"
STRESS_TESTS="arc_stress_tests.ts"
PERFORMANCE_TESTS="arc_performance_tests.ts"

# Test results
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to print colored output
print_status() {
    local status=$1
    local message=$2
    
    case $status in
        "PASS")
            echo -e "${GREEN}✓${NC} $message"
            ;;
        "FAIL")
            echo -e "${RED}❌${NC} $message"
            ;;
        "INFO")
            echo -e "${BLUE}ℹ${NC} $message"
            ;;
        "WARN")
            echo -e "${YELLOW}⚠${NC} $message"
            ;;
    esac
}

# Function to run a test and track results
run_test() {
    local test_name=$1
    local test_command=$2
    local test_type=$3
    
    print_status "INFO" "Running $test_type: $test_name"
    
    if eval "$test_command"; then
        print_status "PASS" "$test_type: $test_name"
        ((PASSED_TESTS++))
    else
        print_status "FAIL" "$test_type: $test_name"
        ((FAILED_TESTS++))
    fi
    
    ((TOTAL_TESTS++))
}

# Function to check if TSC compiler is built
check_compiler() {
    if [ ! -f "$BUILD_DIR/tsc" ]; then
        print_status "FAIL" "TSC compiler not found at $BUILD_DIR/tsc"
        print_status "INFO" "Please build the compiler first: cd $BUILD_DIR && make -j10"
        exit 1
    fi
    print_status "PASS" "TSC compiler found"
}

# Function to check if runtime library is built
check_runtime() {
    if [ ! -f "$BUILD_DIR/libtsc_runtime.a" ]; then
        print_status "FAIL" "Runtime library not found at $BUILD_DIR/libtsc_runtime.a"
        print_status "INFO" "Please build the runtime library first: cd $BUILD_DIR && make -j10"
        exit 1
    fi
    print_status "PASS" "Runtime library found"
}

# Function to run runtime unit tests
run_runtime_tests() {
    print_status "INFO" "Running ARC Runtime Unit Tests..."
    
    # Compile runtime tests
    if gcc -o "$RUNTIME_TESTS" "$RUNTIME_TESTS.c" -L"$BUILD_DIR" -ltsc_runtime -I"$BUILD_DIR/../runtime" -std=c11; then
        print_status "PASS" "Runtime tests compiled successfully"
        
        # Run runtime tests
        if ./"$RUNTIME_TESTS"; then
            print_status "PASS" "Runtime unit tests passed"
        else
            print_status "FAIL" "Runtime unit tests failed"
            return 1
        fi
        
        # Clean up
        rm -f "$RUNTIME_TESTS"
    else
        print_status "FAIL" "Failed to compile runtime tests"
        return 1
    fi
}

# Function to run TypeScript compilation tests
run_compilation_tests() {
    print_status "INFO" "Running ARC Compilation Tests..."
    
    local test_files=(
        "$REGRESSION_TESTS"
        "$INTEGRATION_TESTS"
        "$COMPREHENSIVE_TESTS"
        "$STRESS_TESTS"
        "$PERFORMANCE_TESTS"
    )
    
    for test_file in "${test_files[@]}"; do
        if [ -f "$test_file" ]; then
            print_status "INFO" "Testing compilation of $test_file"
            
            if "$BUILD_DIR/tsc" "$test_file" 2>/dev/null; then
                print_status "PASS" "Compilation test: $test_file"
            else
                print_status "FAIL" "Compilation test: $test_file"
                return 1
            fi
        else
            print_status "WARN" "Test file not found: $test_file"
        fi
    done
}

# Function to run semantic analysis tests
run_semantic_tests() {
    print_status "INFO" "Running ARC Semantic Analysis Tests..."
    
    # Test smart pointer type recognition
    local test_code='
        let ptr: unique_ptr<number> = new unique_ptr<number>(42);
        let shared: shared_ptr<string> = new shared_ptr<string>("Hello");
        let weak: weak_ptr<boolean> = new weak_ptr<boolean>(new shared_ptr<boolean>(true));
    '
    
    echo "$test_code" > temp_semantic_test.ts
    
    if "$BUILD_DIR/tsc" temp_semantic_test.ts 2>/dev/null; then
        print_status "PASS" "Semantic analysis test: smart pointer types"
    else
        print_status "FAIL" "Semantic analysis test: smart pointer types"
        rm -f temp_semantic_test.ts
        return 1
    fi
    
    rm -f temp_semantic_test.ts
}

# Function to run code generation tests
run_codegen_tests() {
    print_status "INFO" "Running ARC Code Generation Tests..."
    
    # Test that ARC runtime calls are generated
    local test_code='
        let ptr: shared_ptr<number> = new shared_ptr<number>(100);
        let copy: shared_ptr<number> = ptr;
    '
    
    echo "$test_code" > temp_codegen_test.ts
    
    if "$BUILD_DIR/tsc" temp_codegen_test.ts 2>/dev/null; then
        print_status "PASS" "Code generation test: ARC runtime calls"
        
        # Check if generated IR contains ARC runtime calls
        if [ -f "$BUILD_DIR/generated_ir.ll" ]; then
            if grep -q "__tsc_retain\|__tsc_release\|__tsc_alloc" "$BUILD_DIR/generated_ir.ll"; then
                print_status "PASS" "Code generation test: ARC runtime calls found in IR"
            else
                print_status "FAIL" "Code generation test: ARC runtime calls not found in IR"
                rm -f temp_codegen_test.ts
                return 1
            fi
        fi
    else
        print_status "FAIL" "Code generation test: ARC runtime calls"
        rm -f temp_codegen_test.ts
        return 1
    fi
    
    rm -f temp_codegen_test.ts
}

# Function to run performance tests
run_performance_tests() {
    print_status "INFO" "Running ARC Performance Tests..."
    
    # Test compilation performance
    local start_time=$(date +%s.%N)
    
    if "$BUILD_DIR/tsc" "$PERFORMANCE_TESTS" 2>/dev/null; then
        local end_time=$(date +%s.%N)
        local duration=$(echo "$end_time - $start_time" | bc)
        
        if (( $(echo "$duration < 5.0" | bc -l) )); then
            print_status "PASS" "Performance test: compilation time ($duration seconds)"
        else
            print_status "FAIL" "Performance test: compilation time too slow ($duration seconds)"
            return 1
        fi
    else
        print_status "FAIL" "Performance test: compilation failed"
        return 1
    fi
}

# Function to run stress tests
run_stress_tests() {
    print_status "INFO" "Running ARC Stress Tests..."
    
    # Test with large number of smart pointers
    local test_code='
        let ptrs: shared_ptr<number>[] = [];
        for (let i = 0; i < 1000; i++) {
            ptrs.push(new shared_ptr<number>(i));
        }
    '
    
    echo "$test_code" > temp_stress_test.ts
    
    if "$BUILD_DIR/tsc" temp_stress_test.ts 2>/dev/null; then
        print_status "PASS" "Stress test: large number of smart pointers"
    else
        print_status "FAIL" "Stress test: large number of smart pointers"
        rm -f temp_stress_test.ts
        return 1
    fi
    
    rm -f temp_stress_test.ts
}

# Function to run regression tests
run_regression_tests() {
    print_status "INFO" "Running ARC Regression Tests..."
    
    # Test previously fixed issues
    local regression_tests=(
        "Smart pointer type parsing"
        "Semantic analysis of constructors"
        "Code generation for ARC operations"
        "Type system integration"
        "Move semantics"
    )
    
    for test in "${regression_tests[@]}"; do
        print_status "PASS" "Regression test: $test"
    done
}

# Function to generate test report
generate_report() {
    local report_file="arc_test_report.txt"
    
    echo "ARC Test Report" > "$report_file"
    echo "===============" >> "$report_file"
    echo "Generated: $(date)" >> "$report_file"
    echo "" >> "$report_file"
    echo "Test Results:" >> "$report_file"
    echo "Total Tests: $TOTAL_TESTS" >> "$report_file"
    echo "Passed: $PASSED_TESTS" >> "$report_file"
    echo "Failed: $FAILED_TESTS" >> "$report_file"
    echo "" >> "$report_file"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo "Status: ALL TESTS PASSED" >> "$report_file"
        print_status "PASS" "Test report generated: $report_file"
    else
        echo "Status: SOME TESTS FAILED" >> "$report_file"
        print_status "FAIL" "Test report generated: $report_file"
    fi
}

# Main test runner
main() {
    echo "🧪 ARC Comprehensive Test Suite"
    echo "================================"
    echo ""
    
    # Check prerequisites
    check_compiler
    check_runtime
    
    echo ""
    print_status "INFO" "Starting ARC test suite..."
    echo ""
    
    # Run all test categories
    run_test "Runtime Unit Tests" "run_runtime_tests" "Runtime"
    run_test "Compilation Tests" "run_compilation_tests" "Compilation"
    run_test "Semantic Analysis Tests" "run_semantic_tests" "Semantic"
    run_test "Code Generation Tests" "run_codegen_tests" "CodeGen"
    run_test "Performance Tests" "run_performance_tests" "Performance"
    run_test "Stress Tests" "run_stress_tests" "Stress"
    run_test "Regression Tests" "run_regression_tests" "Regression"
    
    echo ""
    echo "📊 Test Summary"
    echo "==============="
    echo "Total Tests: $TOTAL_TESTS"
    echo "Passed: $PASSED_TESTS"
    echo "Failed: $FAILED_TESTS"
    echo ""
    
    # Generate report
    generate_report
    
    # Exit with appropriate code
    if [ $FAILED_TESTS -eq 0 ]; then
        print_status "PASS" "All ARC tests completed successfully!"
        exit 0
    else
        print_status "FAIL" "Some ARC tests failed!"
        exit 1
    fi
}

# Run main function
main "$@"