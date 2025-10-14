# TSC Development Guidelines

## Memory Management and ARC Rules

### 1. ARC (Automatic Reference Counting) Requirements

#### String Operations
- **ALWAYS** use `string_concat` for string concatenation
- **ALWAYS** use `number_to_string` for number-to-string conversion
- **NEVER** manually manage string memory
- **NEVER** use raw string pointers without proper ARC handling

#### Array Operations
- **ALWAYS** validate array bounds before access
- **ALWAYS** use proper ARC for string array elements
- **NEVER** access arrays without bounds checking
- **NEVER** leak memory in array operations

### 2. Code Quality Standards

#### Function Implementation
- **REQUIRED**: All functions must have proper error handling
- **REQUIRED**: All array accesses must be bounds-checked
- **REQUIRED**: All memory allocations must be properly managed
- **REQUIRED**: All type conversions must be validated

#### Error Handling
- **MANDATORY**: Range checking for all array operations
- **MANDATORY**: Type validation for all assignments
- **MANDATORY**: Memory safety validation
- **MANDATORY**: Clear error messages for debugging

### 3. Testing Requirements

#### Test Coverage
- **REQUIRED**: Every new feature must have comprehensive tests
- **REQUIRED**: All error conditions must be tested
- **REQUIRED**: Memory leak detection for all operations
- **REQUIRED**: Performance validation for critical paths

#### Test Quality
- **MANDATORY**: Tests must verify correct behavior
- **MANDATORY**: Tests must verify error handling
- **MANDATORY**: Tests must validate memory safety
- **MANDATORY**: Tests must check for segfaults

### 4. Code Review Checklist

#### Before Code Review
- [ ] All array accesses are bounds-checked
- [ ] All string operations use ARC functions
- [ ] All memory allocations are properly managed
- [ ] All error conditions are handled
- [ ] All tests pass
- [ ] No memory leaks detected
- [ ] No segfaults in test cases

#### During Code Review
- [ ] Verify ARC compliance
- [ ] Check bounds checking implementation
- [ ] Validate error handling
- [ ] Review memory management
- [ ] Ensure test coverage
- [ ] Verify performance impact

### 5. Memory Safety Rules

#### Array Operations
```typescript
// CORRECT: Bounds checking
if (index >= 0 && index < array.length) {
    array[index] = value;
} else {
    panic("Array index out of bounds");
}

// INCORRECT: No bounds checking
array[index] = value; // DANGEROUS!
```

#### String Operations
```typescript
// CORRECT: Using ARC functions
let result = string_concat(str1, str2);
let numStr = number_to_string(number);

// INCORRECT: Manual string handling
let result = str1 + str2; // May cause memory issues
```

### 6. Performance Guidelines

#### Optimization Rules
- **PRIORITY 1**: Memory safety and correctness
- **PRIORITY 2**: Performance optimization
- **PRIORITY 3**: Code readability
- **PRIORITY 4**: Feature completeness

#### Performance Targets
- **Array Access**: O(1) with minimal overhead
- **Bounds Checking**: < 5% performance impact
- **Memory Usage**: No unnecessary allocations
- **Compilation Speed**: No significant slowdown

### 7. Error Handling Standards

#### Error Types
- **Bounds Errors**: Array index out of range
- **Type Errors**: Incorrect type assignment
- **Memory Errors**: Allocation/deallocation failures
- **Runtime Errors**: Unexpected conditions

#### Error Response
- **Bounds Errors**: Panic with clear message
- **Type Errors**: Compile-time error when possible
- **Memory Errors**: Graceful degradation
- **Runtime Errors**: Panic with context

### 8. Documentation Requirements

#### Code Documentation
- **REQUIRED**: All public functions must be documented
- **REQUIRED**: Complex algorithms must have comments
- **REQUIRED**: Memory management must be explained
- **REQUIRED**: Error conditions must be documented

#### Test Documentation
- **REQUIRED**: Test purpose must be clear
- **REQUIRED**: Expected behavior must be documented
- **REQUIRED**: Error cases must be explained
- **REQUIRED**: Performance expectations must be stated

## Enforcement

### Code Review Process
1. **Automated Checks**: CI/CD pipeline validates ARC compliance
2. **Manual Review**: Human review for complex logic
3. **Testing**: Comprehensive test suite validation
4. **Performance**: Performance regression detection

### Quality Gates
- **MUST PASS**: All tests
- **MUST PASS**: Memory leak detection
- **MUST PASS**: Performance benchmarks
- **MUST PASS**: Code review approval

### Consequences
- **FAILURE**: Code cannot be merged without compliance
- **REGRESSION**: Performance issues must be resolved
- **MEMORY LEAKS**: Critical issues must be fixed immediately
- **SEGFAULTS**: Security issues must be addressed

## Conclusion

These guidelines ensure that the TSC compiler maintains high code quality, memory safety, and performance while implementing new features. All developers must follow these rules to maintain the integrity and reliability of the codebase.