# Array Types Quick Reference

## ✅ Working Examples

```typescript
// Function parameters
function process(arr: Array<number>): number { return 42; }

// Return types  
function getArray(): Array<string> { return []; }

// Multiple types
function transform(input: Array<number>): Array<string> { return []; }
```

## ❌ What Fails

```typescript
// ❌ DON'T: Use < operator in functions with Array<T>
function bad(arr: Array<number>): number {
    while (i < 3) { }  // ERROR: Expected type name
    if (x < 10) { }    // ERROR: Expected type name
}
```

## 🛠️ Quick Fixes

### Fix 1: Use `==` instead of `<`

```typescript
// ✅ DO: Replace < with ==
while (i == 0) { }  // Works
if (x == 5) { }     // Works
```

### Fix 2: Use helper functions

```typescript
// ✅ DO: Move comparisons to separate functions
function isLess(a: number, b: number): boolean {
    return a < b;  // This works
}

function process(arr: Array<number>): number {
    while (isLess(i, 3)) { }  // This works
}
```

## 🚨 Error Messages

| Error                               | Cause                       | Fix                         |
|-------------------------------------|-----------------------------|-----------------------------|
| `Expected type name`                | `<` interpreted as generic  | Use `==` or helper function |
| `Expected '>' after type arguments` | Parser expects generic type | Use `==` or helper function |
| `Unexpected token in expression`    | Parser confusion            | Simplify expression         |

## 📝 Checklist

- [ ] No `<` operators in functions with `Array<T>` parameters
- [ ] Use `==` for simple comparisons
- [ ] Use helper functions for complex logic
- [ ] Test compilation after changes
- [ ] Keep expressions simple when possible

## 🔗 More Info

- [Full User Guide](ARRAY_TYPES_USER_GUIDE.md)
- [Technical Status](../ARRAY_TYPE_SUPPORT_STATUS.md)
- [Test Examples](../test_workaround_validation.ts)