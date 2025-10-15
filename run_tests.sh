#!/bin/bash

# Comprehensive Test Execution Script for TSC Compiler
echo "🚀 Starting Comprehensive TSC Compiler Test Suite"
echo "=================================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test results tracking
PASSED=0
FAILED=0
TOTAL=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_file="$2"
    local description="$3"
    
    echo ""
    echo "🧪 Running: $test_name"
    echo "   Description: $description"
    echo "   File: $test_file"
    echo "   ----------------------------------------"
    
    TOTAL=$((TOTAL + 1))
    
    # Compile the test
    if cd /workspace/build && ./tsc "../$test_file" 2>/dev/null; then
        echo "   ✅ Compilation: SUCCESS"
        
        # Run the test
        if cd /workspace && "./${test_file%.ts}" 2>/dev/null; then
            echo "   ✅ Execution: SUCCESS"
            echo -e "   ${GREEN}✅ OVERALL: PASSED${NC}"
            PASSED=$((PASSED + 1))
        else
            echo "   ❌ Execution: FAILED"
            echo -e "   ${RED}❌ OVERALL: FAILED${NC}"
            FAILED=$((FAILED + 1))
        fi
    else
        echo "   ❌ Compilation: FAILED"
        echo -e "   ${RED}❌ OVERALL: FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
}

# Function to run AST test
run_ast_test() {
    local test_name="$1"
    local test_file="$2"
    local description="$3"
    
    echo ""
    echo "🧪 Running: $test_name"
    echo "   Description: $description"
    echo "   File: $test_file"
    echo "   ----------------------------------------"
    
    TOTAL=$((TOTAL + 1))
    
    # Generate AST
    if cd /workspace/build && ./tsc --print-ast "../$test_file" > "../${test_file%.ts}_ast.txt" 2>/dev/null; then
        echo "   ✅ AST Generation: SUCCESS"
        
        # Check if AST file was created and has content
        if [ -f "/workspace/${test_file%.ts}_ast.txt" ] && [ -s "/workspace/${test_file%.ts}_ast.txt" ]; then
            echo "   ✅ AST File: CREATED"
            echo -e "   ${GREEN}✅ OVERALL: PASSED${NC}"
            PASSED=$((PASSED + 1))
        else
            echo "   ❌ AST File: EMPTY OR MISSING"
            echo -e "   ${RED}❌ OVERALL: FAILED${NC}"
            FAILED=$((FAILED + 1))
        fi
    else
        echo "   ❌ AST Generation: FAILED"
        echo -e "   ${RED}❌ OVERALL: FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
}

# Start test execution
echo "📋 Test Suite Overview:"
echo "   - Array Assignment Tests"
echo "   - Function Return Type Tests" 
echo "   - Memory Stress Tests"
echo "   - Integration Tests"
echo "   - AST Generation Tests"
echo ""

# Run all tests
run_test "Basic Array Test" "test_array_bounds.ts" "Basic array assignment and access"
run_test "Simple Comprehensive Array Test" "test_array_simple_comprehensive.ts" "Complex array operations without loops"
run_test "Simple Function Return Types" "test_function_simple.ts" "Function return type inference without loops"
run_test "Simple Memory Test" "test_memory_simple.ts" "Memory allocation without loops"
run_test "Simple Integration Test" "test_integration_simple.ts" "Combined features integration without loops"
run_ast_test "AST Generation Test" "test_ast.ts" "AST generation and structure validation"

# Summary
echo ""
echo "📊 Test Results Summary"
echo "======================="
echo -e "Total Tests: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed. Please review the output above.${NC}"
    exit 1
fi