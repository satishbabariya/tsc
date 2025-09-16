# Contributing to TSC

Thank you for your interest in contributing to TSC! This document provides guidelines for contributing to the TypeScript
Static Compiler project.

## Development Setup

### Prerequisites

- **CMake** 3.20 or higher
- **LLVM** 14.0+ with development headers
- **C++17** compatible compiler (GCC 8+, Clang 10+, MSVC 2019+)
- **Git** for version control

### Getting Started

1. **Fork and Clone**
   ```bash
   git clone https://github.com/your-username/tsc.git
   cd tsc
   ```

2. **Create Build Directory**
   ```bash
   # Build directory is gitignored - never commit it!
   mkdir build && cd build
   ```

3. **Configure and Build**
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Debug
   make -j10
   ```

4. **Test Your Build**
   ```bash
   ./tsc --version
   echo "let x: number = 42;" > test.ts
   ./tsc test.ts
   ```

## Project Structure

```
tsc/
├── src/                    # Source code
│   ├── lexer/             # Lexical analysis (Phase 1) ✅
│   ├── parser/            # Syntax analysis (Phase 2) 🚧
│   ├── semantic/          # Semantic analysis (Phase 3) 🚧
│   ├── codegen/           # Code generation (Phase 4) 🚧
│   └── utils/             # Utilities and diagnostics
├── include/tsc/           # Public headers
├── tests/                 # Unit and integration tests
├── examples/              # TypeScript examples
├── docs/                  # Documentation
├── build/                 # ⚠️ BUILD DIRECTORY - NEVER COMMIT!
└── tsc.tm                 # Grammar definition (DO NOT MODIFY)
```

## Important Git Rules

### ❌ Never Commit These Files/Directories

- `build/` - Build artifacts and generated files
- `*.o`, `*.a`, `*.so`, `*.exe` - Compiled binaries
- `*.ll`, `*.bc` - LLVM IR files
- `CMakeCache.txt`, `CMakeFiles/` - CMake generated files
- IDE files (`.vscode/`, `.idea/`)
- Temporary files (`*.tmp`, `*.log`)

### ✅ Always Check Before Committing

```bash
# Check git status
git status

# Verify no build artifacts
git ls-files | grep -E "(build/|\.o$|\.a$|\.exe$|\.ll$)"
# Should return nothing!

# Check what you're about to commit
git diff --cached
```

### Build Directory Management

The `build/` directory:

- Contains **generated files only**
- Is **completely disposable** - can be deleted and recreated
- Is **ignored by git** (in `.gitignore`)
- Should **never be committed** to version control

If you accidentally commit build files:

```bash
# Remove from git (keeps files on disk)
git rm -r build/ --cached

# Commit the removal
git commit -m "Remove build artifacts from git tracking"
```

## Development Workflow

### 1. Create Feature Branch

```bash
git checkout -b feature/your-feature-name
```

### 2. Follow Coding Standards

- **C++17** standard compliance
- **LLVM coding style** for consistency
- **Smart pointers** for memory management (following TSC architecture)
- **Comprehensive error messages** with source locations
- **Unit tests** for new functionality

### 3. Build and Test

```bash
cd build
make -j10

# Run tests
make test

# Test with examples
./tsc ../examples/simple.ts --emit-llvm
```

### 4. Commit Guidelines

- **Clear, descriptive commit messages**
- **Separate logical changes** into different commits
- **Reference issues** if applicable

```bash
# Good commit message format
git commit -m "lexer: Add support for template literal expressions

- Implement template expression parsing in lexer
- Add state management for template/expression contexts  
- Include comprehensive test cases for nested expressions

Fixes #123"
```

### 5. Pull Request Process

1. **Update documentation** if needed
2. **Add tests** for new features
3. **Ensure build passes** on clean checkout
4. **Update STATUS.md** if completing major milestones
5. **Request review** from maintainers

## Architecture Guidelines

### Memory Management

- Use `unique_ptr<T>` for exclusive ownership
- Use `shared_ptr<T>` for shared ownership
- Use `weak_ptr<T>` for non-owning references
- Follow **manual memory management** principles (no GC)

### Error Handling

- Use `DiagnosticEngine` for all error reporting
- Include **source locations** in all errors
- Provide **helpful suggestions** when possible
- Support **multiple error reporting** (don't stop at first error)

### Performance

- Design for **compilation speed** and **runtime performance**
- Use **parallel algorithms** where appropriate
- Follow **zero-cost abstractions** principle
- Profile critical paths

## Testing

### Unit Tests

```bash
# Build with tests (requires GTest)
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j10
make test
```

### Integration Tests

```bash
# Test with real TypeScript files
./tsc examples/demo.ts --emit-llvm
./tsc examples/simple.ts -o simple
```

### Adding Tests

- **Unit tests** in `tests/test_*.cpp`
- **Integration tests** using example files
- **Performance tests** for compilation speed
- **Cross-platform tests** for different targets

## Debugging

### Debug Builds

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address"
make -j10
```

### LLVM Integration

```bash
# Check LLVM configuration
llvm-config-20 --version
llvm-config-20 --libs

# Debug LLVM IR generation
./tsc hello.ts --emit-llvm
cat hello.ll
```

## Common Issues

### Build Fails

1. **Check LLVM installation**: `llvm-config-20 --version`
2. **Clean build**: `rm -rf build && mkdir build && cd build`
3. **Check dependencies**: CMake, C++17 compiler
4. **Verify git status**: No build artifacts committed

### Linker Errors

1. **LLVM library issues**: Check monolithic vs component libraries
2. **Missing dependencies**: Install LLVM development packages
3. **Version conflicts**: Ensure consistent LLVM version

### Git Issues

1. **Build files tracked**: Use `git rm -r build/ --cached`
2. **Large commits**: Check for accidentally committed binaries
3. **Merge conflicts**: Never merge build artifacts

## Getting Help

- **Check existing issues** on GitHub
- **Read architecture docs** in `docs/ARCHITECTURE.md`
- **Review project status** in `STATUS.md`
- **Ask questions** in discussions or issues

## Code of Conduct

- **Be respectful** and constructive
- **Follow project guidelines** and architecture
- **Help others** learn and contribute
- **Focus on code quality** and maintainability

Remember: The goal is to create a **high-performance TypeScript static compiler** that generates **native binaries**
using LLVM. Every contribution should align with this vision while maintaining **code quality** and **architectural
consistency**.