# TSC Compiler vs Stdlib Migration Plan

## Overview

This document outlines the current state of builtin functions in the TSC compiler and provides a migration plan to move appropriate functionality to a proper stdlib (standard library) while keeping the compiler focused on language features.

## Current Architecture Issues

### Problems with Current Approach
- **Compiler knows too much**: Builtin functions are hardcoded in the compiler
- **Naming conflicts**: `console.log`, `Math.abs` suggest JavaScript runtime APIs
- **Inflexibility**: Adding new functions requires compiler changes
- **Mixed concerns**: Language features mixed with library functions

### Benefits of Stdlib Approach
- **Separation of concerns**: Compiler focuses on language, stdlib provides utilities
- **Modularity**: Stdlib can be versioned and updated independently
- **Extensibility**: Users can create their own libraries
- **Future-proof**: No naming conflicts with future packages

---

## Current Runtime Implementation Analysis

### ✅ **SHOULD STAY IN RUNTIME** (Core Language Support)

These are **essential runtime functions** that the compiler needs for basic language operations:

#### **Type Conversion Functions** (Core Language)
- ✅ `string_concat(char* str1, char* str2)` - Essential for string concatenation
- ✅ `number_to_string(double value)` - Essential for number-to-string conversion
- ✅ `boolean_to_string(bool value)` - Essential for boolean-to-string conversion
- ✅ `object_to_string(void* obj)` - Essential for object-to-string conversion
- ✅ `pointer_to_string(void* ptr)` - Essential for pointer-to-string conversion
- ✅ Multiple mangled versions (`number_to_string_1` through `number_to_string_9`)

**Reason**: These are **core language features** that the compiler generates calls to. They're not user-facing library functions.

#### **Exception Handling** (Core Language)
- ✅ `__throw_exception(int64_t exception_value)` - Essential for `throw` statements
- ✅ `__rethrow_exception()` - Essential for `catch` blocks
- ✅ `__has_exception()` - Essential for exception checking
- ✅ `__get_exception()` - Essential for exception access
- ✅ `__clear_exception()` - Essential for exception cleanup

**Reason**: These are **core language control flow** features that the compiler generates calls to.

#### **Global Constants** (Core Language)
- ✅ `Infinity` - Essential JavaScript constant
- ✅ `NaN` - Essential JavaScript constant

**Reason**: These are **core language constants** that the compiler needs to reference.

### 🔄 **SHOULD MOVE TO STDLIB** (User-Facing Functions)

These are **user-facing library functions** that should be in stdlib:

#### **Console/IO Functions** (Library Function)
- ⚠️ `_print(void* first_arg, ...)` - Should become `stdlib/io.print()`

**Reason**: This is a **user-facing library function**, not a core language feature.

#### **Array Operations** (Library Functions)
- ⚠️ `array_length(void* array_ptr)` - Should become `stdlib/array.length()`
- ⚠️ `arrayPush(void* array_ptr, void* item)` - Should become `stdlib/array.push()`
- ⚠️ `arrayPop(void* array_ptr)` - Should become `stdlib/array.pop()`

**Reason**: These are **user-facing library functions** that users call directly.

### **Key Architectural Distinction**

The distinction between runtime and stdlib functions is:

- **Runtime functions** = Core language features the compiler generates calls to
- **Stdlib functions** = User-facing library functions users import and call

**Runtime Layer** (Compiler Internal):
```
runtime/
├── type_conversions.c    // Core type conversion functions
├── exception_handling.c  // Core exception handling
├── global_constants.c    // Core language constants
└── arc/                  // Memory management
```

**Stdlib Layer** (User-Facing):
```
stdlib/
├── io.ts                 // _print() → stdlib/io.print()
├── array.ts              // array_length() → stdlib/array.length()
└── math.ts               // Future math functions
```

This makes the architecture much cleaner:
- **Compiler** → Generates calls to runtime functions for core language features
- **Users** → Import and call stdlib functions for library features
- **Runtime** → Provides essential language support functions
- **Stdlib** → Provides user-facing utility functions

---

## Current Implementation Status

### ✅ **SHOULD STAY IN COMPILER** (Core Language Features)

These are fundamental language features that the compiler must understand:

#### **Type Conversions** (Compiler Core)
- `toString()` method on primitive types
- `valueOf()` method on objects
- Type coercion rules
- **Reason**: Essential for type system and code generation

#### **Exception Handling** (Compiler Core)
- `throw` statement handling
- `try/catch/finally` block generation
- Exception propagation
- **Reason**: Core language control flow feature

#### **Array Literals** (Compiler Core)
- Array initialization syntax `[1, 2, 3]`
- Array type inference
- **Reason**: Core language syntax and type system

#### **Template Literals** (Compiler Core)
- String interpolation `${variable}`
- Template string parsing
- **Reason**: Core language syntax feature

#### **Object Literals** (Compiler Core)
- Object initialization syntax `{a: 1, b: 2}`
- Property access `.` and `[]`
- **Reason**: Core language syntax and type system

#### **Function Calls** (Compiler Core)
- Function invocation syntax
- Parameter passing
- Return value handling
- **Reason**: Core language feature

---

### ❌ **SHOULD MOVE TO STDLIB** (Library Functions)

These are utility functions that should be in a separate stdlib:

#### **Math Functions** (Move to `stdlib/math.ts`)
**Current Status**: Referenced but not implemented
- ❌ `Math.abs()` → `stdlib/math.abs()`
- ❌ `Math.sqrt()` → `stdlib/math.sqrt()`
- ❌ `Math.min()` → `stdlib/math.min()`
- ❌ `Math.max()` → `stdlib/math.max()`
- ❌ `Math.floor()` → `stdlib/math.floor()`
- ❌ `Math.ceil()` → `stdlib/math.ceil()`
- ❌ `Math.round()` → `stdlib/math.round()`
- ❌ `Math.sin()` → `stdlib/math.sin()`
- ❌ `Math.cos()` → `stdlib/math.cos()`

#### **String Methods** (Move to `stdlib/string.ts`)
**Current Status**: Referenced but not implemented
- ❌ `String.length` → `stdlib/string.length()`
- ❌ `String.charAt()` → `stdlib/string.charAt()`
- ❌ `String.substring()` → `stdlib/string.substring()`
- ❌ `String.indexOf()` → `stdlib/string.indexOf()`
- ❌ `String.replace()` → `stdlib/string.replace()`
- ❌ `String.split()` → `stdlib/string.split()`
- ❌ `String.trim()` → `stdlib/string.trim()`

#### **Array Methods** (Move to `stdlib/array.ts`)
**Current Status**: Partially implemented in compiler
- ⚠️ `Array.length` → `stdlib/array.length()` (currently in compiler)
- ⚠️ `Array.push()` → `stdlib/array.push()` (currently in compiler)
- ⚠️ `Array.pop()` → `stdlib/array.pop()` (currently in compiler)
- ❌ `Array.shift()` → `stdlib/array.shift()`
- ❌ `Array.unshift()` → `stdlib/array.unshift()`
- ❌ `Array.slice()` → `stdlib/array.slice()`
- ❌ `Array.indexOf()` → `stdlib/array.indexOf()`
- ❌ `Array.forEach()` → `stdlib/array.forEach()`
- ❌ `Array.map()` → `stdlib/array.map()`

#### **Console/IO Functions** (Move to `stdlib/io.ts`)
**Current Status**: Recently renamed to `_print`
- ⚠️ `_print()` → `stdlib/io.print()` (currently in compiler)
- ❌ `stdio.read()` → `stdlib/io.read()`
- ❌ `stdio.write()` → `stdlib/io.write()`
- ❌ `stdio.readline()` → `stdlib/io.readline()`

#### **Type Utilities** (Move to `stdlib/types.ts`)
**Current Status**: Some implemented in compiler
- ⚠️ `string_concat()` → `stdlib/types.concat()` (currently in compiler)
- ⚠️ `number_to_string()` → `stdlib/types.toString()` (currently in compiler)
- ⚠️ `boolean_to_string()` → `stdlib/types.toString()` (currently in compiler)
- ❌ `string_to_number()` → `stdlib/types.parseNumber()`
- ❌ `string_to_boolean()` → `stdlib/types.parseBoolean()`

---

### 🔄 **MIGRATION PRIORITY**

#### **Phase 1: High Priority** (Immediate)
1. **Create basic stdlib structure**
   - `stdlib/math.ts` with basic functions
   - `stdlib/io.ts` with `print()` function
   - `stdlib/types.ts` with type conversion utilities

2. **Remove misleading references**
   - Remove `Math.*` references from compiler
   - Remove `String.*` references from compiler
   - Update examples to use stdlib imports

3. **Implement core stdlib functions**
   - `stdlib/math.abs()`, `stdlib/math.sqrt()`
   - `stdlib/io.print()` (move from `_print()`)
   - `stdlib/types.toString()` (move from runtime functions)

#### **Phase 2: Medium Priority** (Next)
1. **Move array functions to stdlib**
   - Move `array_length()`, `arrayPush()`, `arrayPop()` to stdlib
   - Implement proper array operations
   - Remove array method recognition from compiler

2. **Expand math stdlib**
   - Add `Math.min`, `Math.max`, `Math.floor`, `Math.ceil`
   - Implement runtime math operations
   - Add trigonometric functions

3. **Add string stdlib**
   - Implement `String.length`, `String.charAt`, `String.substring`
   - Move string utilities to stdlib

#### **Phase 3: Low Priority** (Future)
1. **Advanced stdlib features**
   - Functional array methods (`map`, `filter`, `reduce`)
   - Advanced string operations
   - Date/time utilities
   - File I/O operations

2. **Package system**
   - Module import/export system
   - Package manager
   - Version management

---

## Implementation Plan

### **Step 1: Create Stdlib Structure**
```
stdlib/
├── math.ts          // Math functions
├── io.ts           // Input/output
├── types.ts        // Type utilities
├── array.ts        // Array operations
├── string.ts       // String operations
└── index.ts        // Main exports
```

### **Step 2: Update Compiler**
- Remove builtin function recognition for library functions
- Keep only core language features in compiler
- Add import/export support for modules
- Generate function calls to imported stdlib functions

### **Step 3: Update Runtime**
- Move utility functions from compiler-specific runtime to stdlib runtime
- Create stdlib-specific runtime functions
- Link stdlib as separate library

### **Step 4: Update Examples and Tests**
- Replace direct function calls with stdlib imports
- Update all examples to use new stdlib API
- Ensure backward compatibility during transition

---

## Example Migration

### **Before (Current)**
```typescript
// Compiler knows about these
_print("Hello");
let x = Math.abs(-5);  // Compiler error - not implemented
let arr = [1, 2, 3];
let len = arr.length;  // Compiler knows about this
```

### **After (Stdlib)**
```typescript
// Import stdlib functions
import { print } from "stdlib/io";
import { abs } from "stdlib/math";
import { length } from "stdlib/array";

print("Hello");
let x = abs(-5);  // Works via stdlib
let arr = [1, 2, 3];
let len = length(arr);  // Works via stdlib
```

---

## Benefits After Migration

### **For Compiler**
- ✅ Cleaner, more focused code
- ✅ Easier to maintain and extend
- ✅ Better separation of concerns
- ✅ No hardcoded library functions

### **For Users**
- ✅ Can create their own libraries
- ✅ Can choose which stdlib version to use
- ✅ More familiar API (similar to other languages)
- ✅ Better modularity and reusability

### **For Project**
- ✅ More professional architecture
- ✅ Easier to add new features
- ✅ Better testability
- ✅ Clearer development roadmap

---

## Conclusion

Moving from compiler-integrated builtin functions to a proper stdlib will significantly improve the TSC project's architecture, maintainability, and user experience. The migration should be done incrementally, starting with the most problematic areas (Math, String methods) and gradually moving toward a full stdlib implementation.

This approach aligns with the project's goals of being a static compiler that generates native binaries while providing a clean, modular standard library for common operations.
