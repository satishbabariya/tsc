# Array Types User Guide

## Overview

TSC now supports `Array<T>` type annotations for function parameters and return types. This guide explains how to use
this feature effectively and work around current limitations.

## ✅ What Works

### Function Parameters

```typescript
function processNumbers(arr: Array<number>): number {
    return 42;
}

function processStrings(arr: Array<string>): string {
    return "hello";
}

function processBooleans(arr: Array<boolean>): boolean {
    return true;
}
```

### Return Types

```typescript
function getNumbers(): Array<number> {
    return [];
}

function getStrings(): Array<string> {
    return [];
}
```

### Multiple Array Types

```typescript
function transformArray(input: Array<number>): Array<string> {
    return [];
}

function processMixed(flags: Array<boolean>): Array<number> {
    return [];
}
```

## ❌ Known Limitations

### The `<` Operator Issue

**Problem**: When using `Array<T>` in function signatures, the `<` operator in expressions is misinterpreted as the
start of a generic type argument.

**Affected Code Patterns**:

```typescript
// ❌ This will FAIL with parsing errors
function processArray(arr: Array<number>): number {
    let i: number = 0;
    while (i < 3) {  // Error: Expected type name
        i = i + 1;
    }
    return i;
}

// ❌ This will also FAIL
function checkArray(arr: Array<string>): boolean {
    let x: number = 5;
    if (x < 10) {  // Error: Expected type name
        return true;
    }
    return false;
}
```

**Error Messages**:

```
error: Expected type name
error: Expected '>' after type arguments
error: Unexpected token in expression
```

## 🛠️ Workarounds

### Workaround 1: Use `==` Instead of `<`

Replace comparison operators with equality checks:

```typescript
// ✅ This works
function processArray(arr: Array<number>): number {
    let i: number = 0;
    while (i == 0) {  // Use == instead of <
        i = i + 1;
    }
    return i;
}

function checkArray(arr: Array<string>): boolean {
    let x: number = 5;
    if (x == 5) {  // Use == instead of <
        return true;
    }
    return false;
}
```

### Workaround 2: Separate Comparison Functions

Move comparison logic to functions without `Array<T>` parameters:

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

### Workaround 3: Use Different Control Structures

```typescript
// ✅ Use for loops with ==
function processArray(arr: Array<number>): number {
    let sum: number = 0;
    for (let i: number = 0; i == 0; i = i + 1) {  // Use ==
        sum = sum + i;
    }
    return sum;
}

// ✅ Use if statements with ==
function checkCondition(arr: Array<boolean>): string {
    let x: number = 3;
    return x == 3 ? "small" : "large";  // Use ==
}
```

## 📋 Best Practices

### 1. Design Functions Carefully

When using `Array<T>`, design your functions to avoid complex expressions with `<` operators:

```typescript
// ✅ Good: Simple logic
function sumArray(arr: Array<number>): number {
    let total: number = 0;
    for (let i: number = 0; i == 0; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

// ❌ Avoid: Complex expressions with <
function complexArray(arr: Array<number>): number {
    let i: number = 0;
    while (i < arr.length) {  // This will fail
        // complex logic
    }
}
```

### 2. Use Helper Functions

Extract complex logic into separate functions:

```typescript
// ✅ Helper functions for comparisons
function isIndexValid(index: number, length: number): boolean {
    return index < length;  // This works
}

function isValueSmall(value: number): boolean {
    return value < 10;  // This works
}

// ✅ Main function using helpers
function processArray(arr: Array<number>): number {
    let i: number = 0;
    while (isIndexValid(i, arr.length)) {  // This works
        if (isValueSmall(arr[i])) {  // This works
            // process element
        }
        i = i + 1;
    }
    return 0;
}
```

### 3. Test Your Code

Always test functions with `Array<T>` parameters to ensure they compile correctly:

```typescript
// ✅ Test your Array<T> functions
function testArrayFunction(): void {
    let numbers: Array<number> = [];
    let result: number = processArray(numbers);
    console.log(result);
}
```

## 🔍 Troubleshooting

### Common Error Patterns

1. **"Expected type name"** - Usually means `<` was interpreted as generic type start
2. **"Expected '>' after type arguments"** - Parser is looking for generic type arguments
3. **"Unexpected token in expression"** - Parser is confused by the syntax

### Debugging Steps

1. **Check for `<` operators** in functions with `Array<T>` parameters
2. **Replace with `==`** or move to helper functions
3. **Test compilation** after each change
4. **Use simple expressions** when possible

### Getting Help

If you encounter issues:

1. Check this guide for workarounds
2. Look at the test files for examples:
    - `test_array_simple_parsing.ts` - Basic usage
    - `test_array_comprehensive.ts` - Multiple scenarios
    - `test_workaround_validation.ts` - Workaround examples
3. Refer to `ARRAY_TYPE_SUPPORT_STATUS.md` for technical details

## 🚀 Future Improvements

The parsing ambiguity with the `<` operator is a known limitation that will be addressed in future versions. The current
workarounds allow you to use `Array<T>` functionality effectively while we work on a more comprehensive solution.

## 📚 Related Documentation

- [Array Type Support Status](../ARRAY_TYPE_SUPPORT_STATUS.md) - Technical implementation details
- [Architecture Overview](ARCHITECTURE.md) - Compiler architecture
- [README](../README.md) - Project overview and build instructions