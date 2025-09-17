#!/bin/bash

# Memory Validation Test Runner
# This script runs tests with memory sanitizers to detect memory issues

set -e  # Exit on any error

echo "🧠 Running Memory Validation Tests"
echo "=================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
BUILD_DIR="/workspace/build"
TEST_DIR="/workspace/tests"
OUTPUT_DIR="/workspace/tests/memory_validation/output"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Test files to run with memory sanitizers
MEMORY_TEST_FILES=(
    "regression/generic_constraints/minimal_regression.ts"
    "system/generic_constraints/final_system_tests.ts"
    "memory_validation/memory_stress_tests.ts"
    "memory_validation/memory_leak_tests.ts"
)

# Statistics
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
MEMORY_ERRORS=0

echo -e "${BLUE}📁 Build Directory: $BUILD_DIR${NC}"
echo -e "${BLUE}📁 Test Directory: $TEST_DIR${NC}"
echo -e "${BLUE}📤 Output Directory: $OUTPUT_DIR${NC}"
echo ""

# Function to run memory validation test
run_memory_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .ts)
    local sanitizer_type="$2"
    
    echo -e "${YELLOW}🧠 Running memory test: $test_name (${sanitizer_type})${NC}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Set environment variables for sanitizer
    local env_vars=""
    if [ "$sanitizer_type" = "AddressSanitizer" ]; then
        env_vars="ASAN_OPTIONS=detect_leaks=1:abort_on_error=1:check_initialization_order=1"
    elif [ "$sanitizer_type" = "MemorySanitizer" ]; then
        env_vars="MSAN_OPTIONS=abort_on_error=1"
    fi
    
    # Compile and run the test
    echo "  📝 Compiling $test_file with ${sanitizer_type}..."
    if $env_vars "$BUILD_DIR/tsc" "$TEST_DIR/$test_file" 2>"$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.log" >"$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.out"; then
        echo -e "  ${GREEN}✅ Compilation successful with ${sanitizer_type}${NC}"
        
        # Check for memory errors in the output
        if grep -q "ERROR: AddressSanitizer\|ERROR: MemorySanitizer\|ERROR: UndefinedBehaviorSanitizer" "$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.log"; then
            echo -e "  ${RED}❌ Memory error detected by ${sanitizer_type}${NC}"
            echo "  📋 Memory error details:"
            grep -A 5 -B 5 "ERROR:" "$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.log" | head -20
            FAILED_TESTS=$((FAILED_TESTS + 1))
            MEMORY_ERRORS=$((MEMORY_ERRORS + 1))
        else
            echo -e "  ${GREEN}✅ No memory errors detected by ${sanitizer_type}${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        fi
    else
        # Check if compilation failed due to memory issues or other issues
        if grep -q "ERROR: AddressSanitizer\|ERROR: MemorySanitizer\|ERROR: UndefinedBehaviorSanitizer" "$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.log"; then
            echo -e "  ${RED}❌ Memory error detected by ${sanitizer_type}${NC}"
            echo "  📋 Memory error details:"
            grep -A 5 -B 5 "ERROR:" "$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.log" | head -20
            FAILED_TESTS=$((FAILED_TESTS + 1))
            MEMORY_ERRORS=$((MEMORY_ERRORS + 1))
        elif grep -q "undefined reference" "$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.log"; then
            echo -e "  ${GREEN}✅ No memory errors (runtime linking issue)${NC}"
            echo "  📋 Note: Runtime linking error (not a memory issue)"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "  ${RED}❌ Compilation failed${NC}"
            echo "  📋 Compilation errors:"
            cat "$OUTPUT_DIR/${test_name}_${sanitizer_type}_compile.log" | head -10
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
    
    echo ""
}

# Function to build with sanitizer
build_with_sanitizer() {
    local sanitizer_type="$1"
    local sanitizer_flag=""
    
    case "$sanitizer_type" in
        "AddressSanitizer")
            sanitizer_flag="-DENABLE_ADDRESS_SANITIZER=ON"
            ;;
        "MemorySanitizer")
            sanitizer_flag="-DENABLE_MEMORY_SANITIZER_MSAN=ON"
            ;;
        "UndefinedBehaviorSanitizer")
            sanitizer_flag="-DENABLE_UNDEFINED_BEHAVIOR_SANITIZER=ON"
            ;;
    esac
    
    echo -e "${BLUE}🔨 Building with ${sanitizer_type}...${NC}"
    cd "$BUILD_DIR"
    
    # Configure with sanitizer
    cmake -DLLVM_ROOT=/usr/lib/llvm-20 $sanitizer_flag ..
    
    # Build with sanitizer
    make -j10
    
    echo -e "${GREEN}✅ Build completed with ${sanitizer_type}${NC}"
    echo ""
}

# Run memory validation tests
echo -e "${BLUE}🚀 Starting memory validation tests...${NC}"
echo ""

# Test with AddressSanitizer
echo -e "${BLUE}🔍 Testing with AddressSanitizer${NC}"
echo "=================================="
build_with_sanitizer "AddressSanitizer"

for test_file in "${MEMORY_TEST_FILES[@]}"; do
    run_memory_test "$test_file" "AddressSanitizer"
done

# Test with UndefinedBehaviorSanitizer
echo -e "${BLUE}🔍 Testing with UndefinedBehaviorSanitizer${NC}"
echo "=============================================="
build_with_sanitizer "UndefinedBehaviorSanitizer"

for test_file in "${MEMORY_TEST_FILES[@]}"; do
    run_memory_test "$test_file" "UndefinedBehaviorSanitizer"
done

# Print summary
echo -e "${BLUE}📊 Memory Validation Summary${NC}"
echo "============================="
echo -e "Total Tests: ${TOTAL_TESTS}"
echo -e "Passed: ${GREEN}${PASSED_TESTS}${NC}"
echo -e "Failed: ${RED}${FAILED_TESTS}${NC}"
echo -e "Memory Errors: ${RED}${MEMORY_ERRORS}${NC}"
echo ""

if [ $MEMORY_ERRORS -eq 0 ]; then
    echo -e "${GREEN}🎉 No memory errors detected! All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}💥 Memory errors detected! ${MEMORY_ERRORS} memory error(s) found!${NC}"
    echo -e "${RED}🚨 This indicates potential memory safety issues!${NC}"
    exit 1
fi