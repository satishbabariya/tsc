# TSC Refactored - Quick Start Guide

## 🚀 Getting Started

This guide will help you get up and running with TSC Refactored in just a few minutes!

## 📋 Prerequisites

Before you begin, make sure you have:

- **LLVM 15.0+** installed on your system
- **CMake 3.16+** installed
- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)

### Installing LLVM (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install llvm-15-dev llvm-15-tools
```

### Installing LLVM (macOS)
```bash
brew install llvm@15
```

### Installing LLVM (Windows)
Download from: https://releases.llvm.org/

## 🏗️ Building the Compiler

### Step 1: Set Environment Variables
```bash
# Set LLVM_ROOT to your LLVM installation
export LLVM_ROOT=/usr/lib/llvm-15  # Ubuntu/Debian
# or
export LLVM_ROOT=/opt/homebrew/opt/llvm@15  # macOS
```

### Step 2: Build the Project
```bash
# Clone or navigate to the project directory
cd tsc-refactored

# Run the build script
./scripts/build.sh
```

### Step 3: Verify the Build
```bash
# Check that the compiler was built
ls -la build/tsc

# Run tests to verify everything works
./scripts/test.sh
```

## 🧪 Running Tests

### Run All Tests
```bash
./scripts/test.sh
```

### Run Individual Test Suites
```bash
cd build

# Unit tests
./tests/unit/LexerUnitTests
./tests/unit/ParserUnitTests
./tests/unit/SemanticAnalysisUnitTests
./tests/unit/StatementGeneratorUnitTests

# Integration tests
./tests/integration/LexerIntegrationTests
./tests/integration/ParserIntegrationTests
./tests/integration/SemanticAnalysisIntegrationTests
./tests/integration/ASTModularIntegrationTest
./tests/integration/StatementGeneratorIntegrationTests
./tests/integration/CodeGenVerificationTests
```

## 📝 Compiling TypeScript

### Basic Usage
```bash
# Compile a TypeScript file to LLVM IR
./build/tsc examples/hello.ts hello.ll

# View the generated LLVM IR
cat hello.ll
```

### Example TypeScript File
```typescript
// examples/hello.ts
function greet(name: string): string {
    return `Hello, ${name}!`;
}

function add(a: number, b: number): number {
    return a + b;
}

function main(): void {
    const message = greet("World");
    const result = add(10, 20);
    
    console.log(message);
    console.log(`Result: ${result}`);
}

main();
```

### Compiler Options
```bash
# Debug build
./build/tsc --debug examples/hello.ts hello.ll

# Optimized build
./build/tsc --optimize examples/hello.ts hello.ll

# Verbose output
./build/tsc --verbose examples/hello.ts hello.ll
```

## 🔧 Development

### Project Structure
```
tsc-refactored/
├── src/                    # Source code
│   ├── lexer/             # Lexical analysis
│   ├── parser/            # Syntax analysis
│   ├── semantic/          # Semantic analysis
│   ├── ast/               # Abstract Syntax Tree
│   ├── codegen/           # Code generation
│   └── utils/             # Utilities
├── include/               # Header files
├── tests/                 # Test suite
├── examples/              # Example programs
└── scripts/               # Build scripts
```

### Adding New Features

1. **Create the module** in the appropriate directory
2. **Add tests** in `tests/unit/` or `tests/integration/`
3. **Update CMakeLists.txt** to include new files
4. **Run tests** to verify functionality
5. **Update documentation** as needed

### Code Style Guidelines

- Use **C++17** features
- Follow **RAII** principles
- Use **smart pointers** (`std::unique_ptr`, `std::shared_ptr`)
- Maintain **const correctness**
- Use **camelCase** for functions, **PascalCase** for classes

## 🐛 Troubleshooting

### Common Issues

#### LLVM_ROOT not found
```bash
# Error: LLVM_ROOT is not set
# Solution: Set the environment variable
export LLVM_ROOT=/path/to/llvm
```

#### Build failures
```bash
# Clean build directory and try again
rm -rf build
./scripts/build.sh
```

#### Test failures
```bash
# Run tests with verbose output
cd build
ctest --output-on-failure
```

### Getting Help

- **Check the logs**: Look at build output for error messages
- **Run tests**: Verify that all tests pass
- **Check dependencies**: Ensure LLVM is properly installed
- **Clean build**: Try a fresh build if issues persist

## 📚 Next Steps

### Learn More
- Read `README.md` for detailed documentation
- Check `PROJECT_SUMMARY.md` for architecture overview
- Explore `examples/` for more TypeScript examples

### Contribute
- Fork the repository
- Create a feature branch
- Make your changes
- Add tests for new functionality
- Submit a pull request

### Advanced Usage
- **Custom LLVM targets**: Modify target configuration
- **Optimization passes**: Add custom LLVM optimizations
- **Runtime library**: Extend the runtime functionality
- **Language features**: Add new TypeScript features

## 🎉 You're Ready!

You now have a working TSC Refactored compiler! The modular architecture makes it easy to understand, modify, and extend. Start by exploring the examples and running the tests to get familiar with the codebase.

Happy compiling! 🚀