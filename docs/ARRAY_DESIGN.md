# Array Implementation Design Document

## Overview
This document outlines the design for array assignment, range checking, and memory management in the TSC compiler.

## Memory Layout Design

### Current Array Structure
```llvm
{ i32, [N x T] }  // length + data array
```

### Memory Layout Details
- **Length Field**: First 4 bytes (i32) storing array length
- **Data Array**: Following bytes containing actual array elements
- **Alignment**: 8-byte aligned for optimal performance
- **Storage**: Stack-allocated for small arrays, heap for large arrays (future)

## Array Assignment Design

### Syntax Support
```typescript
// Basic assignment
numbers[0] = 42;
numbers[index] = value;

// String assignment
strings[0] = "hello";
strings[1] = variableString;

// Complex assignment
objects[0] = { name: "test", value: 123 };
```

### Range Checking Strategy
1. **Compile-time bounds checking** for constant indices (when possible)
2. **Runtime bounds checking** for variable indices using LLVM conditional branches
3. **Panic on out-of-bounds** with clear error messages using `panic_bounds_error`
4. **Zero-cost optimization** for known-safe accesses (future optimization)

### LLVM IR Generation Strategy
```llvm
; For array[index] = value
%index_val = load i32, ptr %index_ptr
%length_ptr = getelementptr { i32, [N x T] }, ptr %array, i32 0, i32 0
%length = load i32, ptr %length_ptr
%bounds_check = icmp ult i32 %index_val, %length
br i1 %bounds_check, label %assign, label %panic

assign:
  %element_ptr = getelementptr { i32, [N x T] }, ptr %array, i32 0, i32 1, i32 %index_val
  store T %value, ptr %element_ptr
  br label %end

panic:
  call void @panic_bounds_error(i32 %index_val, i32 %length)
  br label %end

end:
  ; Continue execution
```

### Memory Safety Requirements
1. **ARC Integration**: All array elements must be properly managed
2. **No Memory Leaks**: Proper cleanup when arrays go out of scope
3. **Bounds Validation**: Prevent buffer overflows and underflows
4. **Type Safety**: Ensure correct types are assigned to array elements

## Implementation Plan

### Phase 1: Parser/AST Updates
- Add `ArrayAssignmentExpression` AST node
- Support `array[index] = value` syntax
- Handle both constant and variable indices

### Phase 2: Code Generation
- Implement array assignment in `LLVMCodeGen.cpp`
- Add range checking with conditional branches
- Generate proper store instructions
- Integrate with existing ARC system

### Phase 3: Testing and Validation
- Comprehensive test suite for all array operations
- Memory leak detection
- Performance validation
- Error handling verification

## Error Handling

### Range Check Implementation
```llvm
; Check if index < 0
%index_lt_zero = icmp slt i32 %index, 0
br i1 %index_lt_zero, label %bounds_error, label %check_upper

; Check if index >= length
%index_ge_length = icmp sge i32 %index, %length
br i1 %index_ge_length, label %bounds_error, label %safe_access

bounds_error:
  call void @panic_bounds_error(i32 %index, i32 %length)
  unreachable

safe_access:
  ; Perform array assignment
```

## Memory Management

### ARC Integration
- **String Elements**: Use `string_concat` and proper reference counting
- **Number Elements**: Direct assignment (no ARC needed)
- **Object Elements**: Future implementation for complex objects

### Cleanup Strategy
- **Stack Arrays**: Automatic cleanup when function returns
- **Heap Arrays**: Future implementation with proper deallocation
- **Element Cleanup**: Proper cleanup of individual elements

## Performance Considerations

### Optimization Opportunities
1. **Constant Folding**: Eliminate bounds checks for constant indices
2. **Loop Optimization**: Special handling for array access in loops
3. **Memory Layout**: Optimize for cache locality
4. **Bounds Check Elimination**: Remove redundant checks

### Future Enhancements
1. **Dynamic Arrays**: Resizable arrays with proper memory management
2. **Array Slices**: Efficient sub-array operations
3. **Multidimensional Arrays**: Support for 2D+ arrays
4. **Array Methods**: Built-in array manipulation functions

## Testing Strategy

### Unit Tests
- Basic array assignment
- Range checking validation
- Type safety verification
- Memory leak detection

### Integration Tests
- Array assignment in functions
- Array assignment in loops
- Mixed type arrays
- Error handling scenarios

### Performance Tests
- Large array operations
- Memory usage validation
- Compilation speed verification

## Security Considerations

### Bounds Checking
- **Mandatory**: All array accesses must be bounds-checked
- **Performance**: Minimal overhead for safety
- **Error Messages**: Clear, informative error reporting

### Memory Safety
- **No Buffer Overflows**: Prevented by bounds checking
- **No Use-After-Free**: Managed by ARC system
- **No Double-Free**: Prevented by proper reference counting

## Conclusion

This design provides a robust foundation for array assignment with comprehensive memory safety, range checking, and ARC integration. The implementation will be built incrementally with extensive testing to ensure correctness and performance.