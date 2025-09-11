# TSC Compiler Examples

This directory contains comprehensive examples demonstrating all the features supported by the TSC (TypeScript Compiler). The TSC compiler is a static TypeScript compiler that generates native binaries using LLVM, providing TypeScript-like syntax with static typing and zero JavaScript runtime overhead.

## 🚀 Quick Start

To compile and run any example:

```bash
# Compile the example
./tsc examples/01_basic_types.ts

# Run the generated executable
./a.out
```

## 📚 Example Categories

### 0. Quick Reference

#### `00_quick_start.ts` - Quick Start Example
- **Features**: Essential TypeScript syntax patterns
- **Demonstrates**: All basic features in one comprehensive example
- **Usage**: Perfect for testing the compiler quickly
- **Status**: ✅ Fully Working

#### `TYPESCRIPT_CHEAT_SHEET.md` - Single-Page Cheat Sheet
- **Features**: Essential TypeScript syntax reference
- **Demonstrates**: 20 core syntax patterns with TSC status
- **Usage**: Quick reference while coding
- **Status**: ✅ Complete

#### `TYPESCRIPT_SYNTAX_BIBLE.md` - Complete Syntax Reference
- **Features**: Beginner to Pro TypeScript syntax
- **Demonstrates**: 30+ features across all levels
- **Usage**: Comprehensive learning and reference guide
- **Status**: ✅ Complete

#### `TYPESCRIPT_COMPLETE_GUIDE.md` - Ultimate TypeScript Reference
- **Features**: Complete guide from Beginner to Expert
- **Demonstrates**: 40+ features with real-world patterns
- **Usage**: The definitive TypeScript reference
- **Status**: ✅ Complete

### 1. Basic Language Features

#### `01_basic_types.ts` - Basic Types and Variables
- **Features**: Primitive types, variable declarations, type annotations
- **Demonstrates**: `number`, `string`, `boolean`, `null`, `undefined`, `void`, `any`
- **Usage**: `let`, `const`, `var` declarations with explicit and inferred types
- **Status**: ✅ Fully Working

#### `02_arithmetic_operations.ts` - Arithmetic Operations
- **Features**: All arithmetic, logical, and bitwise operators
- **Demonstrates**: `+`, `-`, `*`, `/`, `%`, `**`, `++`, `--`, `==`, `===`, `&&`, `||`, `!`, `&`, `|`, `^`
- **Usage**: Assignment operators, comparison operators, ternary operator
- **Status**: ✅ Fully Working

#### `03_control_flow.ts` - Control Flow Statements
- **Features**: Conditional statements, loops, switch statements
- **Demonstrates**: `if/else`, `while`, `do-while`, `for`, `for-in`, `for-of`, `switch/case`
- **Usage**: `break`, `continue`, nested conditions, fall-through cases
- **Status**: ✅ Fully Working

### 2. Functions and Data Structures

#### `04_functions.ts` - Functions
- **Features**: Function declarations, parameters, return types, function calls
- **Demonstrates**: Function expressions, arrow functions, higher-order functions, recursion
- **Usage**: Optional parameters, default parameters, rest parameters, method overloading
- **Status**: ✅ Fully Working

#### `05_arrays.ts` - Arrays
- **Features**: Array literals, indexing, operations
- **Demonstrates**: Array creation, element access, iteration, basic array methods
- **Usage**: Multi-dimensional arrays, array of objects, array processing
- **Status**: ✅ Fully Working

#### `06_objects.ts` - Objects
- **Features**: Object literals, property access, nested objects
- **Demonstrates**: Object creation, property modification, method definitions
- **Usage**: Computed properties, object iteration, object cloning
- **Status**: ✅ Fully Working

### 3. Object-Oriented Programming

#### `07_classes.ts` - Classes
- **Features**: Class declarations, inheritance, methods, properties
- **Demonstrates**: Constructors, inheritance, method overriding, static members
- **Usage**: Public/private members, getters/setters, abstract classes, method overloading
- **Status**: ⚠️ Partial (constructor assignment issues)

### 4. Type System

#### `08_interfaces.ts` - Interfaces
- **Features**: Interface declarations, implementation, usage
- **Demonstrates**: Basic interfaces, optional properties, readonly properties, method signatures
- **Usage**: Interface inheritance, multiple inheritance, generic interfaces
- **Status**: ✅ Fully Working

#### `09_enums.ts` - Enums
- **Features**: Enum declarations, member access, usage
- **Demonstrates**: Numeric enums, string enums, mixed enums, bit flags
- **Usage**: Enum iteration, enum utilities, enum in switch statements
- **Status**: ✅ Fully Working

#### `10_type_aliases.ts` - Type Aliases
- **Features**: Type alias declarations and usage
- **Demonstrates**: Basic type aliases, union types, intersection types, generic types
- **Usage**: Complex object types, function types, utility types, mapped types
- **Status**: ⚠️ Partial (usage in variable declarations has issues)

#### `11_generics.ts` - Generics
- **Features**: Generic functions, classes, and type parameters
- **Demonstrates**: Generic functions, generic classes, generic interfaces, constraints
- **Usage**: Generic inheritance, method overloading, async generics
- **Status**: ✅ Fully Working

### 5. Advanced Features

#### `12_error_handling.ts` - Error Handling
- **Features**: Try-catch-finally, throw statements, error types
- **Demonstrates**: Basic error handling, custom error classes, async error handling
- **Usage**: Error retries, error logging, resource cleanup
- **Status**: ⚠️ Partial (try-catch-finally not fully implemented)

#### `13_advanced_features.ts` - Advanced Features
- **Features**: Complex combinations of TypeScript features
- **Demonstrates**: Complex type definitions, generic services, event systems
- **Usage**: Business logic, data processing, configuration management
- **Status**: ✅ Fully Working (depends on individual feature status)

#### `14_template_literals.ts` - Template Literals
- **Features**: Template literal strings and interpolation
- **Demonstrates**: String interpolation, multi-line strings, expressions
- **Usage**: Dynamic string generation, HTML templates, SQL queries
- **Status**: ✅ Fully Working

#### `15_destructuring.ts` - Destructuring
- **Features**: Object and array destructuring patterns
- **Demonstrates**: Array destructuring, object destructuring, rest/spread
- **Usage**: Function parameters, variable assignment, data extraction
- **Status**: ✅ Fully Working

#### `16_modules.ts` - Modules
- **Features**: Module patterns and namespace concepts
- **Demonstrates**: Namespace patterns, module exports, dependency injection
- **Usage**: Code organization, encapsulation, reusable components
- **Status**: ⚠️ Partial (basic patterns work, full module system pending)

#### `17_intermediate_features.ts` - Intermediate Features
- **Features**: Optional parameters, readonly, utility types, keyof
- **Demonstrates**: Advanced parameter patterns, type utilities, mapped types
- **Usage**: API design, type safety, advanced generic patterns
- **Status**: ⚠️ Partial (some features may not be fully supported)

#### `18_advanced_features.ts` - Advanced Features
- **Features**: Conditional types, infer, recursive types, branded types
- **Demonstrates**: Advanced type manipulation, complex generic patterns
- **Usage**: Type-level programming, advanced type safety
- **Status**: ❌ Not Implemented (shows patterns for reference)

#### `19_expert_patterns.ts` - Expert Patterns
- **Features**: Discriminated unions, template literals, satisfies operator
- **Demonstrates**: Framework-level patterns, real-world TypeScript
- **Usage**: Library development, advanced type safety
- **Status**: ⚠️ Partial (some features not fully supported)

#### `20_meta_programming.ts` - Meta-Programming
- **Features**: Recursive types, variadic tuples, type-level computation
- **Demonstrates**: Compile-time programming, cutting-edge TypeScript
- **Usage**: Library internals, framework development, type-level programming
- **Status**: ❌ Not Implemented (shows patterns for reference)

## 🎯 Feature Status Overview

| Feature Category | Status | Notes |
|------------------|--------|-------|
| **Basic Types** | ✅ Working | All primitive types supported |
| **Variables** | ✅ Working | `let`, `const`, `var` with type annotations |
| **Operators** | ✅ Working | All arithmetic, logical, and bitwise operators |
| **Control Flow** | ✅ Working | `if/else`, loops, `switch` statements |
| **Functions** | ✅ Working | Function declarations, calls, parameters |
| **Arrays** | ✅ Working | Array literals, indexing, operations |
| **Objects** | ✅ Working | Object literals, property access |
| **Classes** | ⚠️ Partial | Basic structure works, method calls have issues |
| **Interfaces** | ✅ Working | Interface declarations and implementation |
| **Enums** | ✅ Working | All enum types and operations |
| **Type Aliases** | ⚠️ Partial | Declaration works, usage in variables has issues |
| **Generics** | ✅ Working | Generic functions, classes, and constraints |
| **Error Handling** | ⚠️ Partial | Basic structure, full implementation pending |

## 🔧 Compilation Instructions

### Prerequisites
- LLVM toolchain installed
- C++ compiler (GCC or Clang)
- CMake

### Building the Compiler
```bash
mkdir build
cd build
cmake ..
make -j10
```

### Compiling Examples
```bash
# Compile a single example
./tsc examples/01_basic_types.ts

# Compile with debug information
./tsc -g examples/01_basic_types.ts

# Compile with optimizations
./tsc -O2 examples/01_basic_types.ts
```

### Running Examples
```bash
# Run the generated executable
./a.out

# Run with arguments (if the example supports it)
./a.out arg1 arg2
```

## 📖 Example Descriptions

### Basic Examples (01-03)
These examples cover the fundamental language features that form the foundation of TypeScript programming. They demonstrate variables, types, operators, and control flow statements.

### Data Structure Examples (04-06)
These examples show how to work with functions, arrays, and objects - the core data structures and abstractions in TypeScript.

### Object-Oriented Examples (07)
This example demonstrates class-based programming with inheritance, encapsulation, and polymorphism.

### Type System Examples (08-11)
These examples showcase TypeScript's powerful type system including interfaces, enums, type aliases, and generics.

### Advanced Examples (12-13)
These examples combine multiple features to create complex, real-world-like applications and demonstrate advanced programming patterns.

## 🐛 Known Issues

1. **Class Method Calls**: Method invocation on class instances may fail with "Expression is not callable" error
2. **Type Alias Usage**: Type aliases can be declared but may not work in variable type annotations
3. **Switch Statements**: Some switch statements may generate LLVM verification errors
4. **Try-Catch-Finally**: Exception handling is not fully implemented in code generation

## 🚀 Getting Started

1. Start with `01_basic_types.ts` to understand basic language features
2. Move to `04_functions.ts` to learn function definitions and calls
3. Try `05_arrays.ts` and `06_objects.ts` for data structures
4. Explore `08_interfaces.ts` and `09_enums.ts` for type system features
5. Use `11_generics.ts` for advanced type programming
6. Study `13_advanced_features.ts` for complex application patterns

## 📝 Contributing

When adding new examples:
1. Follow the existing naming convention (`##_feature_name.ts`)
2. Include comprehensive comments explaining each feature
3. Test the example with the compiler before submitting
4. Update this README with the new example description

## 🔗 Related Documentation

- [Architecture Overview](../docs/ARCHITECTURE.md)
- [Compiler Status](../COMPREHENSIVE_COMPILER_STATUS.md)
- [Test Results](../COMPREHENSIVE_TEST_RESULTS.md)
- [Known Limitations](../docs/KNOWN_LIMITATIONS.md)

---

**Note**: This compiler is in active development. Some features may not be fully implemented or may have known limitations. Check the status indicators above for current feature support.