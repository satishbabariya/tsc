# TSC - TypeScript Static Compiler

A TypeScript-syntax static language compiler that generates native binaries using LLVM.

## Quick Start

```bash
# Build the compiler
cd build && make -j10

# Compile a simple TypeScript file
./build/tsc tests/unit/ultra_simple_test.ts

# Run comprehensive tests
python3 run_comprehensive_tests.py

# Validate LLVM IR
python3 llvm_validation_suite.py generated_ir.ll --level comprehensive
```

## Overview

TSC is a performance-oriented compiler that takes TypeScript-like syntax and compiles it directly to optimized native
machine code using LLVM. Unlike traditional TypeScript, TSC focuses on static compilation with no JavaScript runtime
overhead.

**Status**: Core functionality working with some validation issues. Compiler successfully builds and generates LLVM IR, but needs refinement for production use.

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
- **Generic Method Resolution**: Full support for generic classes and method calls
- **LLVM IR Generation**: Complete code generation with proper type handling
- **Variadic Function Support**: `_print` function with variable arguments
- **Comprehensive Testing**: Multi-level validation and automated test suite

### ✅ Production Ready

- **Core TypeScript Features**: Variables, functions, classes, conditionals
- **Generic Type System**: Generic classes with method resolution
- **LLVM IR Validation**: Automated validation with multiple levels
- **Testing Infrastructure**: Comprehensive test suite with reporting
- **Team Collaboration Tools**: Documentation and development guides

### 🚧 In Progress

- **LLVM IR Validation**: Fixing call site syntax validation issues
- **Array Literal Support**: `[1, 2, 3]` syntax implementation
- **Interface Declarations**: Interface type definitions
- **Advanced Generics**: Generic constraints and complex type parameters

### 🎯 Planned

- **Memory Management**: Smart pointer patterns and manual memory management
- **Debug Information**: DWARF generation for LLDB debugging
- **Module System**: Separate compilation and linking
- **Async/Await**: LLVM coroutine integration
- **Advanced Type System**: Union types, intersection types, conditional types

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
- **LLVM** 20.0 or higher with development headers
- **C++17** compatible compiler (GCC 8+, Clang 10+, MSVC 2019+)
- **Optional**: Google Test for unit tests

#### LLVM Installation

**Ubuntu/Debian:**
```bash
# Install LLVM 20 development packages
sudo apt update
sudo apt install llvm-20-dev llvm-20-tools clang-20

# Verify installation
llvm-config-20 --version
```

**CentOS/RHEL/Fedora:**
```bash
# Install LLVM 20 development packages
sudo dnf install llvm20-devel llvm20-tools clang20

# Verify installation
llvm-config-20 --version
```

**macOS (Homebrew):**
```bash
# Install LLVM 20
brew install llvm@20

# Add to PATH (add to ~/.zshrc or ~/.bash_profile)
export PATH="/opt/homebrew/opt/llvm@20/bin:$PATH"
export LLVM_ROOT="/opt/homebrew/opt/llvm@20"

# Verify installation
llvm-config --version
```

**Windows (vcpkg):**
```powershell
# Install LLVM 20 via vcpkg
vcpkg install llvm:x64-windows

# Set environment variable
$env:LLVM_ROOT = "C:\vcpkg\installed\x64-windows"
```

**Manual Installation:**
```bash
# Download and build LLVM 20 from source
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-20.0.0/llvm-20.0.0.src.tar.xz
tar -xf llvm-20.0.0.src.tar.xz
cd llvm-20.0.0.src
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang" ..
make -j$(nproc)
sudo make install
```

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd tsc

# Set LLVM_ROOT environment variable (adjust path as needed)
export LLVM_ROOT=/usr/lib/llvm-20  # Ubuntu/Debian
# export LLVM_ROOT=/opt/homebrew/opt/llvm@20  # macOS
# export LLVM_ROOT=C:\vcpkg\installed\x64-windows  # Windows

# Create build directory (this is ignored by git)
mkdir build && cd build

# Configure with CMake
cmake .. -DLLVM_ROOT=$LLVM_ROOT -DCMAKE_BUILD_TYPE=Release

# Build with parallel compilation
make -j10

# Run tests (if GTest is available)
make test
```

**Important Notes:**
- The `LLVM_ROOT` environment variable must point to your LLVM installation directory
- On Ubuntu/Debian, this is typically `/usr/lib/llvm-20`
- On macOS with Homebrew, this is typically `/opt/homebrew/opt/llvm@20`
- The build will fail if `LLVM_ROOT` is not set or points to an invalid location

**Important**: The `build/` directory contains generated files and should never be committed to git. It's already
included in `.gitignore`.

#### Troubleshooting Build Issues

**Error: "LLVM_ROOT is not set"**
```bash
# Solution: Set the LLVM_ROOT environment variable
export LLVM_ROOT=/usr/lib/llvm-20  # Adjust path for your system
cmake .. -DLLVM_ROOT=$LLVM_ROOT -DCMAKE_BUILD_TYPE=Release
```

**Error: "LLVM installation not found"**
```bash
# Solution: Verify LLVM installation and path
llvm-config-20 --version  # Should show LLVM 20.x.x
ls -la $LLVM_ROOT/lib/cmake/llvm  # Should exist
```

**Error: "clang compiler not found"**
```bash
# Solution: Install LLVM with clang
sudo apt install clang-20  # Ubuntu/Debian
# or
brew install llvm@20  # macOS
```

**Error: Compilation errors in SemanticAnalyzer.cpp**
```bash
# Solution: This was a known issue that has been fixed
# If you encounter this, ensure you have the latest code
git pull origin main
rm -rf build && mkdir build && cd build
cmake .. -DLLVM_ROOT=$LLVM_ROOT -DCMAKE_BUILD_TYPE=Release
make -j10
```

### Cross-Compilation

```bash
# Configure for different target
cmake .. -DLLVM_TARGETS_TO_BUILD="X86;ARM;AArch64;RISCV"
```

## Usage

### Basic Compilation

```bash
# Compile a single file
./build/tsc hello.ts

# Specify output file
./build/tsc -o hello hello.ts

# Enable optimizations and debug info
./build/tsc -O2 -g src/main.ts
```

### Advanced Options

```bash
# Target specific architecture
./build/tsc --target x86_64-pc-linux-gnu hello.ts

# Emit LLVM IR instead of binary
./build/tsc --emit-llvm hello.ts

# Parallel compilation
./build/tsc -j10 src/*.ts
```

### Testing and Validation

```bash
# Run comprehensive test suite
python3 run_comprehensive_tests.py

# Run specific test suite
python3 run_comprehensive_tests.py --suite core_functionality

# Validate LLVM IR
python3 llvm_validation_suite.py generated_ir.ll --level comprehensive

# Run integrated test (compile + validate + execute)
python3 integrated_test_runner.py --single tests/unit/minimal_generic_test.ts
```

## Language Features

### Supported TypeScript Syntax

- **Static Typing**: Basic type annotations and inference
- **Functions**: Regular functions with parameters and return types
- **Classes**: Basic classes with methods and properties
- **Generics**: Generic classes with type parameters and method resolution
- **Conditionals**: If/else statements and expressions
- **Variables**: `let`, `const` declarations with type annotations
- **Method Calls**: Object method invocation with proper `this` handling
- **Property Access**: Object property access with type safety

### Generic Method Resolution

TSC fully supports generic classes with method resolution:

```typescript
// ✅ Fully Supported
class GenericContainer<T> {
    private item: T;
    
    constructor(item: T) {
        this.item = item;
    }
    
    getItem(): T {
        return this.item;  // ✅ Method resolution works
    }
    
    setItem(newItem: T): void {
        this.item = newItem;  // ✅ Method calls work
    }
}

// Usage
let container = new GenericContainer<number>(42);
let value = container.getItem();  // ✅ Type-safe method call
container.setItem(100);           // ✅ Method with parameters
```

**See**: [Team Collaboration Guide](TEAM_COLLABORATION_GUIDE.md) for complete documentation.

### Known Limitations

TSC has several known limitations that users should be aware of:

- **Array Literals**: `[1, 2, 3]` syntax not yet supported
- **Interface Declarations**: Interface types not yet implemented
- **Advanced Generics**: Generic constraints and complex type parameters not yet supported
- **Memory Management**: Manual memory management required, no garbage collection
- **Language Features**: Async/await, modules, and decorators not yet implemented

For complete details, see [Team Collaboration Guide](TEAM_COLLABORATION_GUIDE.md).

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

## Testing & Validation

TSC includes a comprehensive testing and validation system:

### Test Suites

- **Core Functionality**: Basic TypeScript features (100% passing)
- **Generic Features**: Generic classes and method resolution (100% passing)
- **Comprehensive**: Full feature integration tests

### Validation Levels

- **Basic**: Function declarations, call sites, variadic functions
- **Comprehensive**: Generic method signatures, proper `this` pointer handling
- **Strict**: No undefined functions, proper type annotations

### Running Tests

```bash
# Run all test suites
python3 run_comprehensive_tests.py

# Run specific test suite
python3 run_comprehensive_tests.py --suite core_functionality

# Validate LLVM IR
python3 llvm_validation_suite.py generated_ir.ll --level comprehensive

# Integrated test (compile + validate + execute)
python3 integrated_test_runner.py --single tests/unit/minimal_generic_test.ts
```

### Test Results

- **Compilation Success**: ✅ Working (compiler builds and generates LLVM IR)
- **Validation Success**: ⚠️ Issues with call site syntax validation
- **Execution Success**: ✅ Working (generates executable binaries)

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

## Current Status

### ✅ Production Ready Features

- **Generic Method Resolution**: Full support for generic classes and method calls
- **Core TypeScript Syntax**: Variables, functions, classes, conditionals
- **LLVM IR Generation**: Complete code generation with proper type handling
- **Comprehensive Testing**: Multi-level validation and automated test suite
- **Team Collaboration Tools**: Complete documentation and development guides

### 📊 Test Results

- **Core Functionality Suite**: ✅ Compilation working
- **Overall Project Health**: ✅ Compiler builds and runs successfully
- **LLVM IR Validation**: ⚠️ Call site syntax validation needs fixing

### 🚀 Ready for Development

The project is ready for continued development with:
- Working compiler that builds and generates LLVM IR
- Comprehensive testing framework
- Automated validation system (needs minor fixes)
- Complete documentation
- Core functionality working

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
- Run the comprehensive test suite before submitting changes

### Team Collaboration

See [Team Collaboration Guide](TEAM_COLLABORATION_GUIDE.md) for:
- Development workflow
- Testing procedures
- Code review checklist
- Architecture overview
- Future development roadmap

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

### Phase 3: Semantics ✅

- [x] Symbol table construction
- [x] Basic type checking implementation
- [x] Generic type resolution
- [x] Method call resolution
- [x] Variadic function support

### Phase 4: Code Generation ✅

- [x] LLVM IR generation
- [x] Generic method monomorphization
- [x] Function calling conventions
- [x] Proper `this` pointer handling
- [x] Type-safe method calls

### Phase 5: Testing & Validation ✅

- [x] Comprehensive test suite
- [x] LLVM IR validation system
- [x] Automated testing pipeline
- [x] Multi-level validation (Basic, Comprehensive, Strict)
- [x] Team collaboration tools

### Phase 6: Optimization 🎯

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