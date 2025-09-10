# Frequently Asked Questions (FAQ)

## Array Types

### Q: Does TSC support `Array<T>` syntax?

**A:** Yes! TSC supports `Array<T>` type annotations for function parameters and return types.

```typescript
// ✅ This works
function processNumbers(arr: Array<number>): number {
    return 42;
}

function getStrings(): Array<string> {
    return [];
}
```

### Q: Why do I get parsing errors when using `<` in while loops?

**A:** There's a parsing ambiguity where `<` is interpreted as the start of generic type arguments instead of a comparison operator. This happens when `Array<T>` is present in the function signature.

```typescript
// ❌ This fails
function process(arr: Array<number>): number {
    while (i < 3) { }  // Error: Expected type name
}

// ✅ Use this workaround
function process(arr: Array<number>): number {
    while (i == 0) { }  // Works
}
```

### Q: What are the workarounds for the `<` operator issue?

**A:** There are several workarounds:

1. **Use `==` instead of `<`**:
   ```typescript
   while (i == 0) { }  // Instead of while (i < 3)
   ```

2. **Use helper functions**:
   ```typescript
   function isLess(a: number, b: number): boolean {
       return a < b;  // This works
   }
   
   function process(arr: Array<number>): number {
       while (isLess(i, 3)) { }  // This works
   }
   ```

3. **Avoid complex expressions** in functions with `Array<T>` parameters.

### Q: Will this limitation be fixed in future versions?

**A:** Yes, this is a known limitation that will be addressed in future versions. The parsing ambiguity requires a fundamental grammar redesign, which is a complex undertaking.

### Q: Can I use `Array<T>` in class methods?

**A:** Yes, `Array<T>` works in class methods:

```typescript
class MyClass {
    processArray(arr: Array<number>): number {
        return 42;
    }
    
    getArray(): Array<string> {
        return [];
    }
}
```

### Q: Are there any other limitations with `Array<T>`?

**A:** The main limitation is the `<` operator parsing ambiguity. Otherwise, `Array<T>` works well for:
- Function parameters
- Return types
- Class methods
- Type aliases (if supported)

## General Compiler Questions

### Q: What TypeScript features are supported?

**A:** See the [README](../README.md) for a complete list. Currently supported:
- Basic types (number, string, boolean)
- Functions and classes
- Interfaces and enums
- Generics (with limitations)
- Array types (with workarounds)

### Q: How do I debug my TSC programs?

**A:** TSC generates DWARF debug information compatible with LLDB:

```bash
# Compile with debug info
./tsc -g myfile.ts

# Debug with LLDB
lldb ./myfile
(lldb) breakpoint set --file myfile.ts --line 5
(lldb) run
```

### Q: What's the difference between TSC and regular TypeScript?

**A:** TSC is a static compiler that generates native binaries, while regular TypeScript compiles to JavaScript. TSC focuses on:
- Native performance
- Manual memory management
- Static compilation (no runtime)
- LLVM optimization

### Q: How do I report bugs or request features?

**A:** Please check the existing documentation first:
- [Array Types User Guide](ARRAY_TYPES_USER_GUIDE.md)
- [Technical Status](../ARRAY_TYPE_SUPPORT_STATUS.md)
- [Test Examples](../test_workaround_validation.ts)

If your issue isn't covered, please provide:
- Minimal reproduction code
- Expected vs actual behavior
- Error messages
- TSC version

## Troubleshooting

### Q: I'm getting "Expected type name" errors. What does this mean?

**A:** This usually indicates the `<` operator parsing ambiguity. Check if you're using `<` in expressions within functions that have `Array<T>` parameters.

### Q: My code compiles but doesn't run. What should I check?

**A:** Check for:
- Missing `main` function
- Correct return types
- Memory management issues
- Runtime errors in the generated code

### Q: How do I optimize my TSC programs?

**A:** Use LLVM optimization flags:

```bash
# Optimize for speed
./tsc -O2 myfile.ts

# Optimize for size
./tsc -Os myfile.ts

# Maximum optimization
./tsc -O3 myfile.ts
```

## Getting Help

### Q: Where can I find more examples?

**A:** Check the test files in the repository:
- `test_array_simple_parsing.ts` - Basic Array<T> usage
- `test_array_comprehensive.ts` - Multiple scenarios
- `test_workaround_validation.ts` - Workaround examples
- `examples/` directory - Various examples

### Q: Is there a community or forum?

**A:** Currently, documentation and examples are the primary resources. Check the repository for the latest information and examples.

---

**Need more help?** Check the [Array Types User Guide](ARRAY_TYPES_USER_GUIDE.md) for detailed documentation and examples.