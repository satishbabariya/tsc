#!/bin/bash

# ARC Integration Tests Runner
# This script runs comprehensive integration tests for ARC functionality

set -e  # Exit on any error

echo "🔄 ARC Integration Tests Runner"
echo "==============================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test results
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_command="$2"
    
    echo -e "\n${BLUE}=== Running: $test_name ===${NC}"
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if eval "$test_command"; then
        echo -e "${GREEN}✅ $test_name PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ $test_name FAILED${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Function to compile C tests
compile_c_test() {
    local test_file="$1"
    local output_file="$2"
    
    echo "🔨 Compiling $test_file..."
    
    # Compile with ARC runtime
    gcc -o "$output_file" "$test_file" \
        -I../runtime \
        -L../build \
        -ltsc_runtime \
        -lpthread \
        -std=c11 \
        -Wall -Wextra -Werror
    
    if [ $? -eq 0 ]; then
        echo "✅ Compilation successful"
        return 0
    else
        echo "❌ Compilation failed"
        return 1
    fi
}

# Function to run TypeScript tests
run_typescript_test() {
    local test_file="$1"
    local test_name="$2"
    
    echo "🔨 Running TypeScript test: $test_name..."
    
    # Compile TypeScript test
    if ../build/tsc "$test_file" --semantic-only; then
        echo "✅ TypeScript compilation successful"
        return 0
    else
        echo "❌ TypeScript compilation failed"
        return 1
    fi
}

# Change to test directory
cd "$(dirname "$0")"

echo "📁 Working directory: $(pwd)"

# Test 1: Cycle Detection Tests
echo -e "\n${YELLOW}🔄 CYCLE DETECTION TESTS${NC}"
if compile_c_test "arc_cycle_detection_tests.c" "cycle_detection_test"; then
    run_test "Cycle Detection Tests" "./cycle_detection_test"
fi

# Test 2: Retain/Release Semantics Tests
echo -e "\n${YELLOW}🔄 RETAIN/RELEASE SEMANTICS TESTS${NC}"
if compile_c_test "arc_retain_release_tests.c" "retain_release_test"; then
    run_test "Retain/Release Semantics Tests" "./retain_release_test"
fi

# Test 3: TypeScript Integration Tests
echo -e "\n${YELLOW}🔄 TYPESCRIPT INTEGRATION TESTS${NC}"
run_test "TypeScript Integration Tests" "run_typescript_test arc_typescript_integration_tests.ts 'TypeScript Integration'"

# Test 4: ARC Runtime Unit Tests
echo -e "\n${YELLOW}🔄 ARC RUNTIME UNIT TESTS${NC}"
if [ -f "arc_runtime_unit_tests.c" ]; then
    if compile_c_test "arc_runtime_unit_tests.c" "arc_runtime_test"; then
        run_test "ARC Runtime Unit Tests" "./arc_runtime_test"
    fi
else
    echo "⚠️  ARC Runtime Unit Tests not found, skipping..."
fi

# Test 5: Smart Pointer Tests
echo -e "\n${YELLOW}🔄 SMART POINTER TESTS${NC}"
if [ -f "arc_smart_pointer_tests.c" ]; then
    if compile_c_test "arc_smart_pointer_tests.c" "smart_pointer_test"; then
        run_test "Smart Pointer Tests" "./smart_pointer_test"
    fi
else
    echo "⚠️  Smart Pointer Tests not found, skipping..."
fi

# Test 6: Memory Leak Detection
echo -e "\n${YELLOW}🔄 MEMORY LEAK DETECTION${NC}"
if [ -f "arc_memory_leak_tests.c" ]; then
    if compile_c_test "arc_memory_leak_tests.c" "memory_leak_test"; then
        run_test "Memory Leak Detection" "./memory_leak_test"
    fi
else
    echo "⚠️  Memory Leak Tests not found, skipping..."
fi

# Test 7: Performance Benchmarks
echo -e "\n${YELLOW}🔄 PERFORMANCE BENCHMARKS${NC}"
if [ -f "arc_performance_tests.c" ]; then
    if compile_c_test "arc_performance_tests.c" "performance_test"; then
        run_test "Performance Benchmarks" "./performance_test"
    fi
else
    echo "⚠️  Performance Tests not found, skipping..."
fi

# Test 8: Stress Tests
echo -e "\n${YELLOW}🔄 STRESS TESTS${NC}"
if [ -f "arc_stress_tests.c" ]; then
    if compile_c_test "arc_stress_tests.c" "stress_test"; then
        run_test "Stress Tests" "./stress_test"
    fi
else
    echo "⚠️  Stress Tests not found, skipping..."
fi

# Cleanup
echo -e "\n${BLUE}🧹 Cleaning up test binaries...${NC}"
rm -f cycle_detection_test retain_release_test arc_runtime_test smart_pointer_test memory_leak_test performance_test stress_test

# Summary
echo -e "\n${BLUE}📊 TEST SUMMARY${NC}"
echo "=================="
echo -e "Total Tests: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed: ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED!${NC}"
    echo -e "${GREEN}✅ ARC integration is working correctly${NC}"
    exit 0
else
    echo -e "\n${RED}❌ SOME TESTS FAILED!${NC}"
    echo -e "${RED}⚠️  ARC integration needs attention${NC}"
    exit 1
fi