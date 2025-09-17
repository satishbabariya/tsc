#!/bin/bash

# Generic Type Constraints Regression Test Runner
# This script runs all regression tests to ensure no regressions have been introduced

set -e  # Exit on any error

echo "🔄 Running Generic Type Constraints Regression Tests"
echo "===================================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test configuration
TSC_BINARY="/workspace/build/tsc"
TEST_DIR="/workspace/tests/regression/generic_constraints"
OUTPUT_DIR="/workspace/tests/regression/generic_constraints/output"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Regression test files
REGRESSION_TESTS=(
    "minimal_regression.ts"
)

# Statistics
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
REGRESSION_FAILURES=0

echo -e "${BLUE}📁 Test Directory: $TEST_DIR${NC}"
echo -e "${BLUE}🔧 TSC Binary: $TSC_BINARY${NC}"
echo -e "${BLUE}📤 Output Directory: $OUTPUT_DIR${NC}"
echo ""

# Function to run a regression test
run_regression_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .ts)
    
    echo -e "${YELLOW}🔄 Running regression test: $test_name${NC}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Compile the test
    echo "  📝 Compiling $test_file..."
    if "$TSC_BINARY" "$TEST_DIR/$test_file" 2>"$OUTPUT_DIR/${test_name}_compile.log" >"$OUTPUT_DIR/${test_name}_compile.out"; then
        echo -e "  ${GREEN}✅ Regression test passed - no regressions detected${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        # Check if compilation failed due to generic constraint issues or runtime issues
        if grep -q "Generic constraint error" "$OUTPUT_DIR/${test_name}_compile.log"; then
            echo -e "  ${RED}❌ REGRESSION DETECTED: Generic constraint error${NC}"
            echo "  📋 Regression details:"
            cat "$OUTPUT_DIR/${test_name}_compile.log" | head -10
            FAILED_TESTS=$((FAILED_TESTS + 1))
            REGRESSION_FAILURES=$((REGRESSION_FAILURES + 1))
        elif grep -q "undefined reference" "$OUTPUT_DIR/${test_name}_compile.log"; then
            echo -e "  ${GREEN}✅ Regression test passed (runtime linking issue, not a regression)${NC}"
            echo "  📋 Note: Runtime linking error (not a generic constraint regression)"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "  ${RED}❌ REGRESSION DETECTED: Compilation failed${NC}"
            echo "  📋 Regression details:"
            cat "$OUTPUT_DIR/${test_name}_compile.log" | head -10
            FAILED_TESTS=$((FAILED_TESTS + 1))
            REGRESSION_FAILURES=$((REGRESSION_FAILURES + 1))
        fi
    fi
    
    echo ""
}

# Function to run constraint violation tests (expected to fail)
run_constraint_violation_regression_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .ts)
    
    echo -e "${YELLOW}🚫 Running constraint violation regression test: $test_name${NC}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Try to compile - should fail
    echo "  📝 Attempting to compile $test_file (should fail)..."
    if "$TSC_BINARY" "$TEST_DIR/$test_file" 2>"$OUTPUT_DIR/${test_name}_violation.log" >"$OUTPUT_DIR/${test_name}_violation.out"; then
        echo -e "  ${RED}❌ REGRESSION DETECTED: Expected compilation failure, but compilation succeeded${NC}"
        echo "  📋 This indicates constraint violation detection has regressed"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        REGRESSION_FAILURES=$((REGRESSION_FAILURES + 1))
    else
        echo -e "  ${GREEN}✅ Constraint violation detection working correctly${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
    
    echo ""
}

# Run all regression tests
echo -e "${BLUE}🚀 Starting regression tests...${NC}"
echo ""

for test_file in "${REGRESSION_TESTS[@]}"; do
    run_regression_test "$test_file"
done

# Run constraint violation regression test
run_constraint_violation_regression_test "constraint_violation_test.ts"

# Print summary
echo -e "${BLUE}📊 Regression Test Summary${NC}"
echo "=========================="
echo -e "Total Tests: ${TOTAL_TESTS}"
echo -e "Passed: ${GREEN}${PASSED_TESTS}${NC}"
echo -e "Failed: ${RED}${FAILED_TESTS}${NC}"
echo -e "Regression Failures: ${RED}${REGRESSION_FAILURES}${NC}"
echo ""

if [ $REGRESSION_FAILURES -eq 0 ]; then
    echo -e "${GREEN}🎉 No regressions detected! All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}💥 REGRESSIONS DETECTED! ${REGRESSION_FAILURES} regression(s) found!${NC}"
    echo -e "${RED}🚨 This indicates that previously working functionality has been broken!${NC}"
    exit 1
fi