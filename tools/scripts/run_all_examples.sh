#!/bin/bash

# Run All Examples Script
# This script runs all TypeScript example files in the examples directory

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL_FILES=0
PASSED_FILES=0
FAILED_FILES=0
SKIPPED_FILES=0

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

# Function to run a single file
run_file() {
    local file="$1"
    local relative_path="$2"
    
    TOTAL_FILES=$((TOTAL_FILES + 1))
    
    echo
    print_status "Running: $relative_path"
    echo "----------------------------------------"
    
    # Check if file exists
    if [ ! -f "$file" ]; then
        print_error "File not found: $file"
        FAILED_FILES=$((FAILED_FILES + 1))
        return 1
    fi
    
    # Try to compile the file
    if ./build/tsc "$file" --emit-llvm 2>/dev/null; then
        # Check if LLVM IR was generated
        local ll_file="${file%.ts}.ll"
        if [ -f "$ll_file" ]; then
            # Validate LLVM IR
            if /usr/lib/llvm-20/bin/llvm-as "$ll_file" -o /dev/null 2>/dev/null; then
                print_success "PASSED: $relative_path (LLVM IR valid)"
                PASSED_FILES=$((PASSED_FILES + 1))
                
                # Show some info about the generated IR
                local line_count=$(wc -l < "$ll_file")
                echo "  Generated $line_count lines of LLVM IR"
                
                # Clean up the .ll file
                rm -f "$ll_file"
            else
                print_error "FAILED: $relative_path (Invalid LLVM IR)"
                FAILED_FILES=$((FAILED_FILES + 1))
            fi
        else
            print_warning "SKIPPED: $relative_path (No LLVM IR generated)"
            SKIPPED_FILES=$((SKIPPED_FILES + 1))
        fi
    else
        print_error "FAILED: $relative_path (Compilation error)"
        FAILED_FILES=$((FAILED_FILES + 1))
    fi
}

# Main execution
main() {
    echo
    print_status "Starting comprehensive example testing..."
    echo
    
    # Check if compiler exists
    if [ ! -f "./build/tsc" ]; then
        print_error "Compiler not found. Please build the project first:"
        print_error "  mkdir build && cd build && cmake .. && make"
        exit 1
    fi
    
    # Check if LLVM tools are available
    if ! command -v /usr/lib/llvm-20/bin/llvm-as &> /dev/null; then
        print_error "llvm-as not found at /usr/lib/llvm-20/bin/llvm-as"
        exit 1
    fi
    
    print_success "Compiler and LLVM tools are available"
    echo
    
    # Run files from main examples directory
    print_status "Running main examples directory files..."
    echo
    
    for file in examples/*.ts; do
        if [ -f "$file" ]; then
            run_file "$file" "$(basename "$file")"
        fi
    done
    
    echo
    print_status "Running validation directory files..."
    echo
    
    # Run files from validation subdirectory
    for file in examples/validation/*.ts; do
        if [ -f "$file" ]; then
            run_file "$file" "validation/$(basename "$file")"
        fi
    done
    
    # Print summary
    echo
    echo "=============================================="
    print_status "SUMMARY"
    echo "=============================================="
    echo "Total files: $TOTAL_FILES"
    print_success "Passed: $PASSED_FILES"
    if [ $FAILED_FILES -gt 0 ]; then
        print_error "Failed: $FAILED_FILES"
    else
        echo "Failed: $FAILED_FILES"
    fi
    if [ $SKIPPED_FILES -gt 0 ]; then
        print_warning "Skipped: $SKIPPED_FILES"
    else
        echo "Skipped: $SKIPPED_FILES"
    fi
    
    # Calculate success rate
    if [ $TOTAL_FILES -gt 0 ]; then
        local success_rate=$((PASSED_FILES * 100 / TOTAL_FILES))
        echo "Success rate: $success_rate%"
        
        if [ $success_rate -ge 80 ]; then
            print_success "Overall result: EXCELLENT ($success_rate%)"
        elif [ $success_rate -ge 60 ]; then
            print_warning "Overall result: GOOD ($success_rate%)"
        elif [ $success_rate -ge 40 ]; then
            print_warning "Overall result: FAIR ($success_rate%)"
        else
            print_error "Overall result: POOR ($success_rate%)"
        fi
    fi
    
    echo "=============================================="
    
    # Exit with appropriate code
    if [ $FAILED_FILES -eq 0 ]; then
        print_success "All tests completed successfully! 🎉"
        exit 0
    else
        print_error "Some tests failed. Please check the output above."
        exit 1
    fi
}

# Run main function
main "$@"