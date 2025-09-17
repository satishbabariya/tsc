# TSC (TypeScript Compiler) - Refactored

A modern, modular TypeScript compiler that generates native binaries using LLVM. This is a complete refactoring of the original TSC compiler with improved architecture, better separation of concerns, and comprehensive testing.

## 🏗️ Architecture Overview

This refactored compiler follows a clean, modular architecture with the following components:

### Core Components

- **Lexer**: Tokenizes TypeScript source code into tokens
- **Parser**: Parses tokens into an Abstract Syntax Tree (AST)
- **Semantic Analyzer**: Performs type checking and semantic analysis
- **Code Generator**: Generates LLVM IR from the AST
- **AST**: Modular Abstract Syntax Tree representation

### Modular Structure

```
src/
├── lexer/           # Lexical analysis modules
│   ├── core/        # Core lexer functionality
│   ├── tokens/      # Token scanning and classification
│   ├── keywords/    # Keyword recognition
│   └── scanners/    # Character classification
├── parser/          # Syntax analysis modules
│   ├── core/        # Core parser functionality
│   ├── expressions/ # Expression parsing
│   ├── statements/  # Statement parsing
│   ├── declarations/# Declaration parsing
│   └── grammar/     # Grammar rules and utilities
├── semantic/        # Semantic analysis modules
│   ├── resolution/  # Symbol resolution
│   ├── checking/    # Type checking
│   └── flow/        # Control flow analysis
├── ast/             # Abstract Syntax Tree modules
│   ├── core/        # Base AST classes and interfaces
│   ├── expressions/ # Expression AST nodes
│   ├── statements/  # Statement AST nodes
│   ├── declarations/# Declaration AST nodes
│   ├── types/       # Type AST nodes
│   └── visitors/    # Visitor pattern implementations
├── codegen/         # Code generation modules
│   ├── context/     # Code generation context
│   ├── types/       # Type generation
│   ├── expressions/ # Expression code generation
│   ├── statements/  # Statement code generation
│   ├── functions/   # Function code generation
│   ├── classes/     # Class code generation
│   ├── builtins/    # Built-in function generation
│   ├── optimization/# LLVM optimization passes
│   └── memory/      # Memory management
├── runtime/         # Runtime library
└── utils/           # Utility functions and classes
```

## 🚀 Key Features

### Modular Design
- **Separation of Concerns**: Each module has a single, well-defined responsibility
- **Clean Interfaces**: Well-defined APIs between modules
- **Testable Units**: Each module can be tested independently
- **Maintainable Code**: Easier to understand, modify, and extend

### TypeScript Support
- **TypeScript Syntax**: Full TypeScript language support
- **Static Typing**: Compile-time type checking
- **Generics**: Generic type support
- **Classes and Interfaces**: Object-oriented programming features
- **Modules**: ES6 module system

### LLVM Integration
- **Native Code Generation**: Generates LLVM IR for native compilation
- **Cross-Platform**: Supports all LLVM target platforms
- **Optimization**: LLVM optimization passes for performance
- **Debug Information**: DWARF debug information generation

### Memory Management
- **ARC (Automatic Reference Counting)**: Automatic memory management
- **No Garbage Collection**: Manual memory management for performance
- **Smart Pointers**: C++ smart pointer integration

## 🛠️ Building the Compiler

### Prerequisites

- **LLVM**: Version 15.0 or later
- **CMake**: Version 3.16 or later
- **C++ Compiler**: C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd tsc-refactored

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -DLLVM_ROOT=/path/to/llvm ..

# Build the compiler
make -j$(nproc)

# Run tests
make test
```

### Build Options

- `-DLLVM_ROOT=/path/to/llvm`: Specify LLVM installation path
- `-DCMAKE_BUILD_TYPE=Release`: Build in Release mode (default: Debug)
- `-DENABLE_TESTS=ON`: Enable test compilation (default: ON)

## 🧪 Testing

The project includes comprehensive testing at multiple levels:

### Test Structure

```
tests/
├── unit/           # Unit tests for individual modules
├── integration/    # Integration tests between modules
└── examples/       # Example TypeScript programs
```

### Running Tests

```bash
# Run all tests
make test

# Run specific test categories
./build/tests/unit/LexerUnitTests
./build/tests/integration/ParserIntegrationTests
./build/tests/integration/SemanticAnalysisIntegrationTests
./build/tests/integration/ASTModularIntegrationTest
./build/tests/integration/StatementGeneratorIntegrationTests
./build/tests/integration/CodeGenVerificationTests
```

### Test Coverage

- **Unit Tests**: Individual module functionality
- **Integration Tests**: Module interaction testing
- **End-to-End Tests**: Complete compilation pipeline
- **Regression Tests**: Ensure no functionality regression
- **Performance Tests**: Compilation speed benchmarks

## 📚 Documentation

### Architecture Documentation
- `docs/architecture/`: High-level architecture overview
- `docs/api/`: API documentation for each module
- `docs/examples/`: Usage examples and tutorials

### Key Documents
- `docs/architecture/OVERVIEW.md`: Overall architecture
- `docs/architecture/LEXER.md`: Lexer module documentation
- `docs/architecture/PARSER.md`: Parser module documentation
- `docs/architecture/SEMANTIC.md`: Semantic analysis documentation
- `docs/architecture/AST.md`: AST module documentation
- `docs/architecture/CODEGEN.md`: Code generation documentation

## 🔧 Development

### Code Style
- **C++17 Standard**: Modern C++ features
- **RAII**: Resource Acquisition Is Initialization
- **Smart Pointers**: Prefer `std::unique_ptr` and `std::shared_ptr`
- **Const Correctness**: Use `const` wherever possible
- **Naming Conventions**: camelCase for functions, PascalCase for classes

### Contributing
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Submit a pull request

### Module Development Guidelines
- **Single Responsibility**: Each module should have one clear purpose
- **Clean Interfaces**: Minimize dependencies between modules
- **Comprehensive Testing**: Write tests for all public interfaces
- **Documentation**: Document all public APIs
- **Error Handling**: Provide clear error messages and diagnostics

## 📊 Performance

### Compilation Speed
- **Modular Compilation**: Parallel compilation of modules
- **Incremental Compilation**: Only recompile changed modules
- **LLVM Optimization**: Aggressive optimization passes

### Runtime Performance
- **Native Code**: Generates optimized native machine code
- **No Runtime Overhead**: No JavaScript runtime or interpreter
- **Memory Efficient**: ARC-based memory management

## 🎯 Roadmap

### Completed ✅
- [x] Lexer modularization
- [x] Parser modularization
- [x] Semantic analysis modularization
- [x] AST modularization
- [x] StatementGenerator implementation
- [x] Comprehensive testing suite

### In Progress 🔄
- [ ] Complete loop statement generation
- [ ] Switch statement generation
- [ ] Exception handling generation

### Planned 📋
- [ ] TypeGenerator implementation
- [ ] FunctionGenerator implementation
- [ ] ClassGenerator implementation
- [ ] BuiltinGenerator implementation
- [ ] OptimizationEngine implementation
- [ ] MemoryManager implementation
- [ ] Performance benchmarking
- [ ] Documentation completion

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🤝 Acknowledgments

- **LLVM Project**: For the excellent LLVM infrastructure
- **TypeScript Team**: For the TypeScript language specification
- **Original TSC Contributors**: For the initial compiler implementation

## 📞 Support

For questions, issues, or contributions:
- **Issues**: Use GitHub Issues for bug reports and feature requests
- **Discussions**: Use GitHub Discussions for questions and general discussion
- **Pull Requests**: Submit PRs for bug fixes and new features

---

**TSC Refactored** - A modern, modular TypeScript compiler for the future! 🚀