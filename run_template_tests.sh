#!/bin/bash

# Template Literal Test Runner
# This script runs all template literal tests and reports results

echo "🧪 Template Literal Test Suite"
echo "================================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a test and report results
run_test() {
    local test_name="$1"
    local test_file="$2"
    local test_type="$3"
    
    echo -e "${BLUE}Running $test_type: $test_name${NC}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if ./build/tsc "$test_file" > /dev/null 2>&1; then
        echo -e "${GREEN}✅ PASSED: $test_name${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAILED: $test_name${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Function to run unit tests
run_unit_tests() {
    echo -e "${YELLOW}📋 Unit Tests${NC}"
    echo "============="
    
    # Note: Unit tests require Google Test framework
    # For now, we'll run the TypeScript tests that exercise the functionality
    
    echo -e "${BLUE}Note: Unit tests require Google Test framework setup${NC}"
    echo -e "${BLUE}Running functional tests instead...${NC}"
}

# Function to run integration tests
run_integration_tests() {
    echo -e "${YELLOW}🔗 Integration Tests${NC}"
    echo "===================="
    
    # Test basic template literal functionality
    run_test "Basic Template Literal" "test_template_basic.ts" "Integration"
    run_test "Simple Interpolation" "test_template_simple_interpolation.ts" "Integration"
    run_test "Multiple Interpolations" "test_template_multiple_interpolation.ts" "Integration"
    run_test "Complex Expressions" "test_template_complex_expressions.ts" "Integration"
    run_test "Edge Cases" "test_template_edge_cases.ts" "Integration"
    run_test "Type Conversion" "test_type_conversion.ts" "Integration"
    run_test "Arithmetic Expressions" "test_arithmetic_expressions.ts" "Integration"
    run_test "Runtime Arithmetic" "test_runtime_arithmetic.ts" "Integration"
    run_test "Local Arithmetic" "test_local_arithmetic.ts" "Integration"
}

# Function to run system tests
run_system_tests() {
    echo -e "${YELLOW}🖥️  System Tests${NC}"
    echo "==============="
    
    run_test "System Test Suite" "test_template_system.ts" "System"
    run_test "Runtime Functions" "test_runtime_functions.ts" "System"
    run_test "Performance Tests" "test_template_performance.ts" "System"
}

# Function to run regression tests
run_regression_tests() {
    echo -e "${YELLOW}🔄 Regression Tests${NC}"
    echo "=================="
    
    run_test "Regression Test Suite" "test_template_regression.ts" "Regression"
}

# Function to run performance tests
run_performance_tests() {
    echo -e "${YELLOW}⚡ Performance Tests${NC}"
    echo "===================="
    
    echo -e "${BLUE}Running performance benchmarks...${NC}"
    
    # Test compilation time
    echo "Testing compilation time..."
    time_start=$(date +%s%N)
    ./build/tsc test_template_performance.ts > /dev/null 2>&1
    time_end=$(date +%s%N)
    compilation_time=$(( (time_end - time_start) / 1000000 ))
    echo -e "${GREEN}Compilation time: ${compilation_time}ms${NC}"
    
    # Test execution time
    echo "Testing execution time..."
    time_start=$(date +%s%N)
    ./test_template_performance > /dev/null 2>&1
    time_end=$(date +%s%N)
    execution_time=$(( (time_end - time_start) / 1000000 ))
    echo -e "${GREEN}Execution time: ${execution_time}ms${NC}"
}

# Function to run error tests
run_error_tests() {
    echo -e "${YELLOW}🚨 Error Tests${NC}"
    echo "============="
    
    run_test "Error Cases" "test_template_error_cases.ts" "Error"
}

# Function to generate test report
generate_report() {
    echo -e "${YELLOW}📊 Test Report${NC}"
    echo "============="
    
    echo -e "Total Tests: ${TOTAL_TESTS}"
    echo -e "${GREEN}Passed: ${PASSED_TESTS}${NC}"
    echo -e "${RED}Failed: ${FAILED_TESTS}${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}🎉 All tests passed!${NC}"
        exit 0
    else
        echo -e "${RED}❌ Some tests failed!${NC}"
        exit 1
    fi
}

# Main execution
main() {
    echo "Starting template literal test suite..."
    echo ""
    
    # Check if compiler is built
    if [ ! -f "./build/tsc" ]; then
        echo -e "${RED}Error: Compiler not found. Please build the project first.${NC}"
        exit 1
    fi
    
    # Run all test suites
    run_unit_tests
    echo ""
    
    run_integration_tests
    echo ""
    
    run_system_tests
    echo ""
    
    run_regression_tests
    echo ""
    
    run_error_tests
    echo ""
    
    run_performance_tests
    echo ""
    
    generate_report
}

# Run main function
main "$@"