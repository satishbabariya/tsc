# TSC Architecture Overview

## Project Vision

TSC is a TypeScript-syntax static language compiler that generates native binaries using LLVM. It provides
TypeScript-like syntax with static typing, compiling directly to optimized native code without JavaScript runtime
overhead.

## Design Principles

### 1. **Multi-Platform Support**

- Target all platforms supported by LLVM (x86_64, ARM64, RISC-V, etc.)
- Cross-compilation capabilities
- Platform-agnostic intermediate representation

### 2. **Performance-Oriented**

- **Compilation Speed**: Fast incremental compilation, efficient parsing
- **Runtime Performance**: LLVM optimizations, zero-cost abstractions
- Separate compilation model for large codebases

### 3. **Developer Experience**

- LLDB debugging support with source-level debugging
- Clear error messages with source location information
- Familiar TypeScript syntax

### 4. **Minimalist Runtime**

- No garbage collector
- Manual memory management with smart pointer patterns
- Minimal standard library (basic I/O, memory operations)

## Compiler Pipeline

```
┌─────────────────┐    ┌──────────────┐    ┌─────────────────┐
│ TypeScript      │───▶│    Lexer     │───▶│    Parser       │
│ Source (.ts)    │    │ (Tokenizer)  │    │ (AST Builder)   │
└─────────────────┘    └──────────────┘    └─────────────────┘
                                                     │
                                                     ▼
┌─────────────────┐    ┌──────────────┐    ┌─────────────────┐
│ Native Binary   │◀───│ LLVM Backend │◀───│ Semantic        │
│ (.exe/.so)      │    │ (CodeGen)    │    │ Analysis        │
└─────────────────┘    └──────────────┘    └─────────────────┘
```

### Phase Breakdown

#### 1. **Lexical Analysis (Lexer)**

- **Input**: Raw TypeScript source code
- **Output**: Token stream
- **Responsibilities**:
    - Tokenize source code into meaningful symbols
    - Handle whitespace, comments, string literals
    - Source location tracking for error reporting
    - Unicode support for identifiers and strings

#### 2. **Syntax Analysis (Parser)**

- **Input**: Token stream
- **Output**: Abstract Syntax Tree (AST)
- **Responsibilities**:
    - Build AST using recursive descent parsing
    - Syntax error recovery and reporting
    - Preserve source location information in AST nodes

#### 3. **Semantic Analysis**

- **Input**: AST
- **Output**: Typed AST with symbol tables
- **Responsibilities**:
    - **Symbol Resolution**: Build symbol tables, resolve identifiers
    - **Type Checking**: Verify type compatibility, infer types
    - **Scope Analysis**: Handle variable scoping, shadowing
    - **Module Resolution**: Resolve imports/exports

#### 4. **Code Generation**

- **Input**: Typed AST
- **Output**: LLVM IR
- **Responsibilities**:
    - Generate LLVM IR from typed AST
    - Memory layout decisions
    - Function calling conventions
    - Debug information generation

## Memory Management Strategy

### Stack vs Heap Allocation

- **Stack**: Local variables, function parameters, small objects
- **Heap**: Dynamic arrays, large objects, objects with unclear lifetime

### Manual Memory Management

- Explicit `new`/`delete` operations (syntax TBD)
- RAII patterns encouraged
- Smart pointer types for automatic cleanup:
  ```typescript
  unique_ptr<T>  // Exclusive ownership
  shared_ptr<T>  // Reference counted
  weak_ptr<T>    // Non-owning reference
  ```

### Memory Safety Features

- Bounds checking for arrays (debug builds)
- Use-after-free detection (debug builds)
- Memory leak detection tools integration

## Type System

### Core Types

```typescript
// Primitive types map directly to LLVM types
number    → i64/f64 (configurable)
boolean   → i1
string    → ptr + length (fat pointer)
void      → void

// Composite types
Array<T>  → { ptr: T*, length: i64, capacity: i64 }
Object    → struct types
Function  → function pointers
```

### Type Mapping Strategy

- **Structural to Nominal**: Convert TypeScript's structural typing to LLVM's nominal types
- **Generic Instantiation**: Monomorphization (generate separate code for each type instantiation)
- **Type Erasure**: Remove type information not needed at runtime

## Module System

### Compilation Model

```
Source Files (.ts) → LLVM Modules (.bc) → Object Files (.o) → Executable
```

### Import/Export Mechanism

- Each `.ts` file becomes an LLVM module
- `export` → external linkage in LLVM
- `import` → external symbol references
- Module dependency graph for build ordering

### Separate Compilation

- Compile each module independently
- Generate `.bc` (LLVM bitcode) files
- Link using LLVM linker (`llvm-link` + `llc`)

## Async/Await Implementation

### LLVM Coroutines Integration

- Use LLVM's coroutine intrinsics
- Transform async functions into state machines
- Minimal runtime scheduler for coroutine execution

### Coroutine Lowering

```typescript
async function example(): Promise<number> {
    const result = await fetch();
    return result * 2;
}

// Becomes state machine with suspend/resume points
```

## Debug Information

### LLDB Integration

- Generate DWARF debug information using LLVM's DIBuilder
- Source-level debugging capabilities:
    - Breakpoints on TypeScript source lines
    - Variable inspection with original names
    - Stack traces with TypeScript function names

### Debug Info Strategy

- Preserve original TypeScript identifiers
- Map LLVM IR back to source locations
- Type information for debugger variable display

## Cross-Platform Considerations

### Target Triple Management

- Support LLVM target triples (e.g., `x86_64-pc-linux-gnu`)
- Cross-compilation without target machine
- Platform-specific optimizations

### ABI Compatibility

- Follow platform-specific calling conventions
- Handle structure padding and alignment
- Platform-specific data type sizes

## Performance Optimizations

### Compilation Speed

- Incremental compilation based on dependency analysis
- Parallel compilation of independent modules
- Efficient AST representation and traversal

### Runtime Performance

- LLVM optimization passes (O2/O3)
- Link-time optimization (LTO)
- Profile-guided optimization (PGO) support
- Zero-cost abstractions principle

## Error Handling Strategy

### Compile-Time Errors

- Rich error messages with source context
- Suggestions for common mistakes
- Multiple error reporting (don't stop at first error)

### Runtime Error Handling

- Panic/abort on unrecoverable errors
- Optional error return types for recoverable errors
- Debug builds: additional runtime checks

## Future Extensibility

### Language Feature Additions

- Modular compiler design allows easy feature additions
- Plugin architecture for custom optimizations
- Configurable language feature sets

### Backend Flexibility

- Abstract code generation interface
- Potential for additional backends (WebAssembly, custom architectures)
- Optimization pass customization
