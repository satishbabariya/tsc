# Known Limitations

This document describes current limitations in the TSC compiler that users should be aware of when developing
applications.

## Parser Limitations

### `<` Operator Ambiguity with Generic Types

**Issue**: The `<` operator cannot be used in expression contexts when `Array<T>` or other generic types are present in
function signatures.

**Root Cause**: The parser has a fundamental ambiguity where `<` can be interpreted as either:

- The start of generic type arguments: `Array<number>`
- A comparison operator: `i < 3`

**Impact**: Functions with generic type parameters cannot use `<` operators in their body expressions.

**Affected Syntax**:

```typescript
// ❌ This will FAIL with parsing errors
function processArray(arr: Array<number>): number {
    let i: number = 0;
    while (i < 3) {  // Error: Expected type name
        i = i + 1;
    }
    return i;
}
```

**Workarounds**:

1. **Use `==` instead of `<`**:
   ```typescript
   // ✅ This works
   function processArray(arr: Array<number>): number {
       let i: number = 0;
       while (i == 0) {  // Use == instead of <
           i = i + 1;
       }
       return i;
   }
   ```

2. **Use helper functions**:
   ```typescript
   // ✅ Helper function without Array<T> parameter
   function isLessThan(a: number, b: number): boolean {
       return a < b;  // This works fine
   }
   
   // ✅ Main function using the helper
   function processArray(arr: Array<number>): number {
       let i: number = 0;
       while (isLessThan(i, 3)) {  // This works
           i = i + 1;
       }
       return i;
   }
   ```

3. **Use parentheses for complex expressions**:
   ```typescript
   // ✅ This works
   function processArray(arr: Array<number>): number {
       let i: number = 0;
       while ((i < 3)) {  // Use parentheses
           i = i + 1;
       }
       return i;
   }
   ```

**Technical Details**:

- The issue stems from precedence conflicts in the grammar
- `TypeArguments` uses `resolveShift` precedence
- Relational operators (`<`, `>`, `<=`, `>=`) have different precedence
- The parser consistently chooses generic type interpretation

**Future Resolution**:
This limitation requires a fundamental grammar redesign to implement context-aware parsing. This is planned for a future
major version of the compiler.

**Related Documentation**:

- [Array Types User Guide](ARRAY_TYPES_USER_GUIDE.md)
- [Array Types Quick Reference](ARRAY_TYPES_QUICK_REFERENCE.md)
- [FAQ](FAQ.md)

## Type System Limitations

### Generic Type Constraints

- `extends` clauses for generic types are not yet implemented
- Type parameter constraints are not enforced

### Union Types

- `string | number | boolean` syntax is not yet implemented
- Union type checking is not available

### Intersection Types

- `A & B` syntax is not yet implemented
- Intersection type checking is not available

## Memory Management Limitations

### Garbage Collection

- No automatic garbage collection
- Manual memory management required
- Smart pointers (`unique_ptr`, `shared_ptr`) not yet implemented

### Memory Leak Detection

- Basic memory leak detection available through AddressSanitizer
- Advanced memory profiling tools not yet integrated

## Performance Limitations

### Compilation Speed

- No incremental compilation
- No parallel parsing
- Large files may compile slowly

### Runtime Performance

- No LLVM optimization passes by default
- Manual optimization required
- No profile-guided optimization

## Language Feature Limitations

### Async/Await

- Not yet implemented
- No coroutine support
- No Promise types

### Modules

- No import/export system
- No separate compilation
- No module resolution

### Decorators

- Not yet implemented
- No metadata support
- No annotation system

## Debugging Limitations

### Source Maps

- Basic DWARF debug information
- No source map generation
- Limited debugging support

### Error Messages

- Basic error reporting
- No suggestion system
- Limited context information

## Platform Limitations

### Cross-Platform Support

- Limited to LLVM-supported targets
- No Windows-specific optimizations
- No embedded system support

### Standard Library

- No standard library implementation
- No runtime library
- No platform-specific APIs

---

**Note**: This document is updated as limitations are resolved and new ones are discovered. For the most current
information, please refer to the latest version of this document.