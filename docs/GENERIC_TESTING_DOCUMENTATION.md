# Generic LLVM Code Generation Testing Documentation

## Overview

This document describes the comprehensive testing suite for generic class functionality in the TypeScript compiler. The
testing suite covers unit tests, integration tests, regression tests, and performance tests to ensure the reliability
and correctness of generic class compilation to LLVM IR.

## Test Structure

### 1. Unit Tests (`test_generic_basic_unit.cpp`)

**Purpose**: Test individual components of the generic LLVM code generation system in isolation.

**Test Cases**:

- `BasicGenericClassDeclaration`: Tests basic generic class declaration and monomorphization
- `GenericTypeInstantiation`: Tests generic type instantiation with type arguments
- `MonomorphizedMethodGeneration`: Tests generation of monomorphized methods
- `TypeParameterSubstitution`: Tests type parameter substitution in generic types
- `LLVMIRValidation`: Tests LLVM IR validation using `llvm-as`
- `MultipleTypeParameters`: Tests generic classes with multiple type parameters
- `NestedGenericTypes`: Tests nested generic types (e.g., `Container<Container<number>>`)
- `ErrorHandlingInvalidTypeArguments`: Tests error handling for invalid type arguments
- `PerformanceComparison`: Compares performance between generic and non-generic classes
- `RegressionTestLLVMErrors`: Regression test for previously encountered LLVM errors

### 2. Integration Tests (`test_generic_integration.cpp`)

**Purpose**: Test the complete compilation pipeline from TypeScript source to LLVM IR.

**Test Cases**:

- `SimpleGenericClassWithConstructor`: Tests basic generic class with constructor
- `GenericClassWithMethods`: Tests generic class with getter/setter methods
- `MultipleTypeParameters`: Tests generic class with multiple type parameters
- `NestedGenericTypes`: Tests nested generic type instantiation
- `GenericClassInheritance`: Tests generic class inheritance
- `ErrorHandlingIncorrectTypeArguments`: Tests error handling for incorrect type arguments
- `PerformanceTest`: Tests compilation performance with 1000 generic instantiations
- `ComplexGenericScenario`: Tests complex generic scenario with linked list
- `RegressionTestLLVMErrors`: Regression test for LLVM verification errors
- `SystemTestCompleteCompilation`: Complete system test with function calls

### 3. Regression Tests (`test_generic_regression.cpp`)

**Purpose**: Prevent regression of previously fixed LLVM verification errors.

**Test Cases**:

- `TerminatorIssues`: Tests for "Terminator found in the middle of a basic block!" error
- `CrossFunctionReferences`: Tests for "Referring to an instruction in another function!" error
- `ReturnTypeMismatches`: Tests for "Found return instr that returns non-void in Function of void return type!" error
- `MainFunctionTerminatorIssues`: Tests for "Basic Block in function 'main' does not have terminator!" error
- `ConstructorFunctionTerminatorIssues`: Tests for constructor function terminator issues
- `ComprehensiveErrorPrevention`: Comprehensive test combining all error scenarios
- `PerformanceRegressionPrevention`: Tests for performance regressions
- `MemoryLeakPrevention`: Tests for memory leak prevention

### 4. Test Runner Script (`run_generic_tests.sh`)

**Purpose**: Automated execution of all tests with validation and reporting.

**Features**:

- LLVM tools availability checking
- Test executable building
- Unit test execution
- Generic scenario testing
- Error handling testing
- Performance testing
- Regression testing
- LLVM IR validation using `llvm-as`
- Colored output for better readability
- Cleanup of temporary files

## Test Execution

### Prerequisites

1. **LLVM 20**: Required for LLVM IR validation
   ```bash
   # Check if LLVM tools are available
   /usr/lib/llvm-20/bin/llvm-as --version
   ```

2. **CMake and Make**: Required for building tests
   ```bash
   # Build the project
   mkdir build && cd build
   cmake .. && make
   ```

3. **Google Test**: Required for unit testing
   ```bash
   # Install Google Test (if not already installed)
   sudo apt-get install libgtest-dev
   ```

### Running Tests

#### Option 1: Run All Tests (Recommended)

```bash
./run_generic_tests.sh
```

#### Option 2: Run Individual Test Categories

```bash
# Build tests
cd build && make generic_llvm_tests

# Run unit tests
./build/generic_llvm_tests

# Run specific TypeScript tests
./build/tsc test_generic_basic.ts --emit-llvm
/usr/lib/llvm-20/bin/llvm-as test_generic_basic.ll -o /dev/null
```

#### Option 3: Manual Testing

```bash
# Test basic generic class
./build/tsc test_generic_basic.ts --emit-llvm
/usr/lib/llvm-20/bin/llvm-as test_generic_basic.ll -o /dev/null

# Test generic instantiation
./build/tsc test_generic_instantiation.ts --emit-llvm
/usr/lib/llvm-20/bin/llvm-as test_generic_instantiation.ll -o /dev/null
```

## Test Coverage

### Generic Class Features Tested

1. **Basic Generic Classes**
    - Single type parameter (`Container<T>`)
    - Multiple type parameters (`Pair<T, U>`)
    - Nested generic types (`Container<Container<number>>`)

2. **Generic Methods**
    - Constructor methods with type parameters
    - Getter/setter methods with type parameters
    - Methods returning generic types

3. **Type Parameter Substitution**
    - Substitution in method parameters
    - Substitution in return types
    - Substitution in property types

4. **Monomorphization**
    - Monomorphized method generation
    - Monomorphized type creation
    - Mangled name generation

5. **LLVM IR Generation**
    - Valid LLVM IR generation
    - Proper function signatures
    - Correct basic block structure
    - Proper terminator placement

### Error Scenarios Tested

1. **LLVM Verification Errors**
    - Cross-function references
    - Terminator issues
    - Return type mismatches
    - Basic block structure issues

2. **Type System Errors**
    - Incorrect number of type arguments
    - Invalid type parameter usage
    - Type mismatch errors

3. **Parser Errors**
    - Malformed generic syntax
    - Missing type arguments
    - Invalid type annotations

## Expected Results

### Successful Test Execution

All tests should pass with the following indicators:

```
🚀 Starting Generic LLVM Code Generation Tests
==============================================

[INFO] Checking LLVM tools availability...
[SUCCESS] LLVM tools are available
[INFO] Building test executable...
[SUCCESS] Test executable built successfully
[INFO] Running unit tests...
[SUCCESS] Unit tests passed
[INFO] Testing specific generic scenarios...
[SUCCESS] Basic generic class declaration test passed
[SUCCESS] Generic class instantiation test passed
[SUCCESS] Multiple type parameters test passed
[SUCCESS] Nested generic types test passed
[INFO] Testing error handling...
[SUCCESS] Error handling test correctly failed
[INFO] Running performance test...
[SUCCESS] Performance test passed (1234ms)
[INFO] Running regression test for LLVM errors...
[SUCCESS] Regression test passed

[SUCCESS] All generic tests passed! 🎉
==============================================
✅ Unit tests: PASSED
✅ Generic scenarios: PASSED
✅ Error handling: PASSED
✅ Performance: PASSED
✅ Regression: PASSED
✅ LLVM IR validation: PASSED
```

### LLVM IR Validation

All generated LLVM IR should pass validation with `llvm-as`:

```bash
/usr/lib/llvm-20/bin/llvm-as test_file.ll -o /dev/null
# Should return exit code 0 (success)
```

## Troubleshooting

### Common Issues

1. **LLVM Tools Not Found**
   ```
   [ERROR] llvm-as not found at /usr/lib/llvm-20/bin/llvm-as
   ```
   **Solution**: Install LLVM 20 or update the path in the test script.

2. **Build Failures**
   ```
   [ERROR] Failed to build test executable
   ```
   **Solution**: Ensure CMake and Make are installed, and run `mkdir build && cd build && cmake .. && make`.

3. **LLVM IR Validation Failures**
   ```
   [ERROR] Generated LLVM IR is invalid
   ```
   **Solution**: Check the generated LLVM IR for syntax errors or structural issues.

4. **Performance Test Failures**
   ```
   [WARNING] Performance test took longer than expected
   ```
   **Solution**: This is a warning, not an error. Consider optimizing the code generation if performance is critical.

### Debug Information

To get more detailed debug information:

1. **Enable Debug Output**: Add debug flags to the compiler
2. **Check Generated IR**: Examine the `.ll` files for issues
3. **Run Individual Tests**: Isolate specific test cases
4. **Check Logs**: Review compiler output for error messages

## Maintenance

### Adding New Tests

1. **Unit Tests**: Add new test cases to `test_generic_basic_unit.cpp`
2. **Integration Tests**: Add new test cases to `test_generic_integration.cpp`
3. **Regression Tests**: Add new test cases to `test_generic_regression.cpp`
4. **Test Runner**: Update `run_generic_tests.sh` for new test scenarios

### Updating Tests

1. **When Adding New Features**: Add corresponding test cases
2. **When Fixing Bugs**: Add regression tests to prevent reoccurrence
3. **When Optimizing**: Update performance tests with new benchmarks

### Test Data

- **Test Files**: Store in the project root with `test_` prefix
- **Generated IR**: Temporary `.ll` files are cleaned up automatically
- **Logs**: Test output is displayed in the console

## Conclusion

This comprehensive testing suite ensures the reliability and correctness of generic class functionality in the
TypeScript compiler. The tests cover all aspects of generic class compilation, from basic functionality to complex
scenarios, and include robust error handling and performance validation.

The testing suite is designed to be:

- **Comprehensive**: Covers all major generic class features
- **Reliable**: Includes regression tests to prevent bugs
- **Maintainable**: Easy to add new tests and update existing ones
- **Automated**: Can be run with a single command
- **Validated**: Uses LLVM tools to verify generated IR correctness