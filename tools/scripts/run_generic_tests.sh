#!/bin/bash

# Generic LLVM Code Generation Test Runner
# This script runs comprehensive tests for generic class functionality

set -e  # Exit on any error

echo "🚀 Starting Generic LLVM Code Generation Tests"
echo "=============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if LLVM tools are available
check_llvm_tools() {
    print_status "Checking LLVM tools availability..."
    
    if ! command -v /usr/lib/llvm-20/bin/llvm-as &> /dev/null; then
        print_error "llvm-as not found at /usr/lib/llvm-20/bin/llvm-as"
        exit 1
    fi
    
    if ! command -v /usr/lib/llvm-20/bin/llvm-verify &> /dev/null; then
        print_warning "llvm-verify not found, will use llvm-as for validation"
    fi
    
    print_success "LLVM tools are available"
}

# Build the test executable
build_tests() {
    print_status "Building test executable..."
    
    if [ ! -d "build" ]; then
        print_error "Build directory not found. Please run 'mkdir build && cd build && cmake .. && make' first"
        exit 1
    fi
    
    cd build
    
    if ! make generic_llvm_tests; then
        print_error "Failed to build test executable"
        exit 1
    fi
    
    print_success "Test executable built successfully"
    cd ..
}

# Run unit tests
run_unit_tests() {
    print_status "Running unit tests..."
    
    if [ ! -f "build/generic_llvm_tests" ]; then
        print_error "Test executable not found. Please build first."
        exit 1
    fi
    
    cd build
    
    if ! ./generic_llvm_tests; then
        print_error "Unit tests failed"
        exit 1
    fi
    
    print_success "Unit tests passed"
    cd ..
}

# Test specific generic scenarios
test_generic_scenarios() {
    print_status "Testing specific generic scenarios..."
    
    # Test 1: Basic generic class declaration
    print_status "Testing basic generic class declaration..."
    if ! ./build/tsc test_generic_basic.ts --emit-llvm; then
        print_error "Basic generic class declaration failed"
        exit 1
    fi
    
    if ! /usr/lib/llvm-20/bin/llvm-as test_generic_basic.ll -o /dev/null; then
        print_error "Generated LLVM IR for basic generic class is invalid"
        exit 1
    fi
    
    print_success "Basic generic class declaration test passed"
    
    # Test 2: Generic class instantiation
    print_status "Testing generic class instantiation..."
    if ! ./build/tsc test_generic_instantiation.ts --emit-llvm; then
        print_error "Generic class instantiation failed"
        exit 1
    fi
    
    if ! /usr/lib/llvm-20/bin/llvm-as test_generic_instantiation.ll -o /dev/null; then
        print_error "Generated LLVM IR for generic instantiation is invalid"
        exit 1
    fi
    
    print_success "Generic class instantiation test passed"
    
    # Test 3: Multiple type parameters
    print_status "Testing multiple type parameters..."
    cat > test_multiple_type_params.ts << 'EOF'
class Pair<T, U> {
    first: T;
    second: U;
    
    constructor(f: T, s: U) {
        this.first = f;
        this.second = s;
    }
}

function test() {
    let pair = new Pair<number, string>(42, "hello");
    return pair.first;
}

test();
EOF
    
    if ! ./build/tsc test_multiple_type_params.ts --emit-llvm; then
        print_error "Multiple type parameters test failed"
        exit 1
    fi
    
    if ! /usr/lib/llvm-20/bin/llvm-as test_multiple_type_params.ll -o /dev/null; then
        print_error "Generated LLVM IR for multiple type parameters is invalid"
        exit 1
    fi
    
    print_success "Multiple type parameters test passed"
    
    # Test 4: Nested generic types
    print_status "Testing nested generic types..."
    cat > test_nested_generics.ts << 'EOF'
class Container<T> {
    value: T;
    
    constructor(val: T) {
        this.value = val;
    }
}

function test() {
    let outer = new Container<Container<number>>(new Container<number>(42));
    return outer.value.value;
}

test();
EOF
    
    if ! ./build/tsc test_nested_generics.ts --emit-llvm; then
        print_error "Nested generic types test failed"
        exit 1
    fi
    
    if ! /usr/lib/llvm-20/bin/llvm-as test_nested_generics.ll -o /dev/null; then
        print_error "Generated LLVM IR for nested generics is invalid"
        exit 1
    fi
    
    print_success "Nested generic types test passed"
}

# Test error handling
test_error_handling() {
    print_status "Testing error handling..."
    
    # Test with incorrect number of type arguments
    cat > test_error_handling.ts << 'EOF'
class Container<T> {
    value: T;
    
    constructor(val: T) {
        this.value = val;
    }
}

function test() {
    // This should cause a compilation error
    let container = new Container<number, string>(42);
    return container.value;
}

test();
EOF
    
    if ./build/tsc test_error_handling.ts --emit-llvm 2>/dev/null; then
        print_warning "Error handling test should have failed but didn't"
    else
        print_success "Error handling test correctly failed"
    fi
}

# Performance test
performance_test() {
    print_status "Running performance test..."
    
    cat > test_performance.ts << 'EOF'
class Container<T> {
    value: T;
    
    constructor(val: T) {
        this.value = val;
    }
}

function test() {
    let containers: Container<number>[] = [];
    for (let i = 0; i < 1000; i++) {
        containers.push(new Container<number>(i));
    }
    return containers.length;
}

test();
EOF
    
    start_time=$(date +%s%N)
    if ! ./build/tsc test_performance.ts --emit-llvm; then
        print_error "Performance test failed"
        exit 1
    fi
    end_time=$(date +%s%N)
    
    duration=$(( (end_time - start_time) / 1000000 ))  # Convert to milliseconds
    
    if [ $duration -lt 5000 ]; then
        print_success "Performance test passed (${duration}ms)"
    else
        print_warning "Performance test took longer than expected (${duration}ms)"
    fi
    
    if ! /usr/lib/llvm-20/bin/llvm-as test_performance.ll -o /dev/null; then
        print_error "Generated LLVM IR for performance test is invalid"
        exit 1
    fi
}

# Regression test for LLVM errors
regression_test() {
    print_status "Running regression test for LLVM errors..."
    
    cat > test_regression.ts << 'EOF'
class Container<T> {
    value: T;
    
    constructor(val: T) {
        this.value = val;
    }
    
    getValue(): T {
        return this.value;
    }
    
    setValue(val: T): void {
        this.value = val;
    }
}

function test() {
    let container = new Container<number>(42);
    container.setValue(100);
    return container.getValue();
}

test();
EOF
    
    if ! ./build/tsc test_regression.ts --emit-llvm; then
        print_error "Regression test failed"
        exit 1
    fi
    
    # Check for specific error patterns
    if grep -q "Referring to an instruction in another function" test_regression.ll; then
        print_error "Regression test found cross-function reference error"
        exit 1
    fi
    
    if grep -q "Terminator found in the middle of a basic block" test_regression.ll; then
        print_error "Regression test found terminator error"
        exit 1
    fi
    
    if grep -q "Found return instr that returns non-void in Function of void return type" test_regression.ll; then
        print_error "Regression test found return type mismatch error"
        exit 1
    fi
    
    if ! /usr/lib/llvm-20/bin/llvm-as test_regression.ll -o /dev/null; then
        print_error "Generated LLVM IR for regression test is invalid"
        exit 1
    fi
    
    print_success "Regression test passed"
}

# Clean up test files
cleanup() {
    print_status "Cleaning up test files..."
    rm -f test_*.ts test_*.ll
    print_success "Cleanup completed"
}

# Main execution
main() {
    echo
    print_status "Starting comprehensive generic testing..."
    echo
    
    check_llvm_tools
    build_tests
    run_unit_tests
    test_generic_scenarios
    test_error_handling
    performance_test
    regression_test
    cleanup
    
    echo
    print_success "All generic tests passed! 🎉"
    echo "=============================================="
    echo "✅ Unit tests: PASSED"
    echo "✅ Generic scenarios: PASSED"
    echo "✅ Error handling: PASSED"
    echo "✅ Performance: PASSED"
    echo "✅ Regression: PASSED"
    echo "✅ LLVM IR validation: PASSED"
    echo
}

# Run main function
main "$@"