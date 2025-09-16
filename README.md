# TSC - TypeScript Static Compiler

A TypeScript-syntax static language compiler that generates native binaries using LLVM.

## Overview

TSC is a performance-oriented compiler that takes TypeScript-like syntax and compiles it directly to optimized native
machine code using LLVM. Unlike traditional TypeScript, TSC focuses on static compilation with no JavaScript runtime
overhead.

## Features

### ✅ Implemented

- **Project Structure**: Complete foundational architecture
- **Build System**: CMake with LLVM integration and parallel compilation (`-j10`)
- **Lexical Analysis**: Token stream generation from TypeScript source
- **AST Framework**: Complete Abstract Syntax Tree node hierarchy
- **Diagnostic Engine**: Comprehensive error reporting with source context
- **Compiler Driver**: Multi-phase compilation pipeline
- **Cross-Platform**: LLVM target support for multiple architectures

### ✅ Recently Completed

- **Parser Implementation**: Recursive descent parser with AST construction (Phase 2)
- **AST Visualization**: Complete AST printer with `--print-ast` option
- **Expression Parsing**: Binary/unary expressions with correct precedence
- **Function Parsing**: Function declarations with parameters and bodies

### 🚧 In Progress

- **Semantic Analysis**: Type checking and symbol resolution (Phase 3)
- **LLVM Code Generation**: IR generation and optimization (Phase 4)

### 🎯 Planned

- **Memory Management**: Smart pointer patterns and manual memory management
- **Debug Information**: DWARF generation for LLDB debugging
- **Module System**: Separate compilation and linking
- **Async/Await**: LLVM coroutine integration
- **Type System**: Full TypeScript type checking

## Architecture

TSC follows a traditional 4-phase compiler pipeline:

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

## Design Principles

1. **Multi-Platform Support**: Target all LLVM-supported architectures
2. **Performance-Oriented**: Fast compilation and optimized runtime performance
3. **Developer Experience**: LLDB debugging with source-level information
4. **Minimalist Runtime**: Manual memory management, no garbage collection
5. **TypeScript Syntax**: Familiar syntax but compiled to native code

## Building

### Prerequisites

- **CMake** 3.20 or higher
- **LLVM** 14.0 or higher with development headers
- **C++17** compatible compiler (GCC 8+, Clang 10+, MSVC 2019+)
- **Optional**: Google Test for unit tests

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd tsc

# Create build directory (this is ignored by git)
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build with parallel compilation
make -j10

# Run tests (if GTest is available)
make test
```

**Important**: The `build/` directory contains generated files and should never be committed to git. It's already
included in `.gitignore`.

### Cross-Compilation

```bash
# Configure for different target
cmake .. -DLLVM_TARGETS_TO_BUILD="X86;ARM;AArch64;RISCV"
```

## Usage

### Basic Compilation

```bash
# Compile a single file
./tsc hello.ts

# Specify output file
./tsc -o hello hello.ts

# Enable optimizations and debug info
./tsc -O2 -g src/main.ts
```

### Advanced Options

```bash
# Target specific architecture
./tsc --target x86_64-pc-linux-gnu hello.ts

# Emit LLVM IR instead of binary
./tsc --emit-llvm hello.ts

# Parallel compilation
./tsc -j10 src/*.ts
```

## Language Features

### Supported TypeScript Syntax

- **Static Typing**: Full type annotations and inference
- **Functions**: Regular, async, and generator functions
- **Classes**: With inheritance and access modifiers
- **Interfaces**: Type contracts and structural typing
- **Modules**: Import/export with separate compilation
- **Generics**: Type parameters with constraints
- **Enums**: Both numeric and string enums
- **Type Operators**: `keyof`, `typeof`, conditional types
- **Array Types**: `Array<T>` syntax for function parameters and return types

### Array Type Support

TSC supports `Array<T>` type annotations with some limitations:

```typescript
// ✅ Supported
function process(arr: Array<number>): Array<string> {
    return [];
}

// ⚠️ Limitation: < operator in expressions requires workarounds
function example(arr: Array<number>): number {
    // while (i < 3) { }  // ❌ Fails
    while (i == 0) {
    }    // ✅ Works
    return 0;
}
```

**See**: [Array Types User Guide](docs/ARRAY_TYPES_USER_GUIDE.md) for complete documentation and workarounds.

### Known Limitations

TSC has several known limitations that users should be aware of:

- **Parser Limitations**: `<` operator ambiguity with generic types (see [Known Limitations](docs/KNOWN_LIMITATIONS.md))
- **Type System**: Generic constraints, union types, and intersection types not yet implemented
- **Memory Management**: Manual memory management required, no garbage collection
- **Language Features**: Async/await, modules, and decorators not yet implemented

For complete details, see [Known Limitations](docs/KNOWN_LIMITATIONS.md) and [Issue Tracker](ISSUES.md).

### Memory Management

```typescript
// Stack allocation (automatic)
let value: number = 42;

// Heap allocation (manual)
let array = new Array<number>(1000);

// Smart pointers (planned)
let ptr: unique_ptr<MyClass> = new MyClass();
let shared: shared_ptr<MyClass> = makeShared<MyClass>();
```

### Async/Await (Planned)

```typescript
async function fetchData(): Promise<string> {
    const response = await fetch("https://api.example.com");
    return response.text();
}
```

## Debugging

TSC generates DWARF debug information compatible with LLDB:

```bash
# Compile with debug info
./tsc -g hello.ts

# Debug with LLDB
lldb ./hello
(lldb) breakpoint set --file hello.ts --line 5
(lldb) run
```

## Performance

### Compilation Speed

- Parallel compilation with `-j10` flag
- Incremental compilation (planned)
- Efficient AST representation

### Runtime Performance

- LLVM optimization passes (O0/O1/O2/O3/Os/Oz)
- Link-time optimization (LTO)
- Zero-cost abstractions
- Manual memory management

## Contributing

1. **Fork** the repository
2. **Create** a feature branch
3. **Follow** the coding standards in `.cursor/rules/tsc.mdc`
4. **Add tests** for new functionality
5. **Submit** a pull request

### Development Guidelines

- Never modify `tsc.tm` grammar without permission
- Use LLVM best practices for code generation
- Follow manual memory management patterns
- Add comprehensive error messages with source locations
- Write tests for all new features

## Roadmap

### Phase 1: Foundation ✅

- [x] Project structure and build system
- [x] Lexical analysis integration
- [x] AST node definitions
- [x] Diagnostic engine

### Phase 2: Parser ✅

- [x] Recursive descent parser implementation
- [x] Error recovery mechanisms
- [x] AST construction from tokens
- [x] Operator precedence handling
- [x] Expression and statement parsing
- [x] AST visualization tools

### Phase 3: Semantics 🚧

- [ ] Symbol table construction
- [ ] Type checking implementation
- [ ] Module resolution
- [ ] Scope analysis

### Phase 4: Code Generation 🚧

- [ ] LLVM IR generation
- [ ] Memory layout decisions
- [ ] Function calling conventions
- [ ] Debug information generation

### Phase 5: Optimization 🎯

- [ ] LLVM optimization passes
- [ ] Link-time optimization
- [ ] Profile-guided optimization

## License

[License information to be added]

## Acknowledgments

- **LLVM Project**: For the excellent compiler infrastructure
- **TypeScript Team**: For the language design inspiration
- **TextMapper**: For the grammar parsing framework used in `tsc.tm`

---

**TSC**: Bringing TypeScript performance to native code execution.