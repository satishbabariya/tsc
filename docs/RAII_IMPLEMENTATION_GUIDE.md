# TSC RAII Implementation Guide - Background Agent Prompt

## Mission Overview

**Objective**: Implement RAII (Resource Acquisition Is Initialization) for the TSC compiler to provide automatic
resource cleanup that works seamlessly with the existing ARC memory management system.

## Current Status

### ✅ **Completed (ARC Memory Management)**

- Smart pointer types (`unique_ptr<T>`, `shared_ptr<T>`, `weak_ptr<T>`)
- Move semantics (`std::move()`)
- ARC runtime library with atomic reference counting
- Parser integration for ARC syntax
- Semantic analysis with ownership tracking
- LLVM code generation with ARC runtime calls
- Comprehensive test suite
- Runtime refactoring and organization

### ❌ **Missing (RAII Resource Management)**

- Destructor syntax (`~ClassName()`)
- Automatic resource cleanup
- RAII patterns for file handles, locks, etc.
- Resource management tied to object scope
- Exception safety during resource cleanup

## Why RAII is Critical

1. **Resource Management**: ARC handles memory, but RAII handles other resources (files, locks, network connections)
2. **Exception Safety**: RAII ensures resources are cleaned up even when exceptions occur
3. **Deterministic Cleanup**: Resources are cleaned up when objects go out of scope
4. **Memory Safety**: Prevents resource leaks and dangling resource handles

## Implementation Phases

---

## Phase 1: Destructor Syntax Implementation

### Task 1.1: Grammar Extensions

**File**: `tsc.tm`
**Action**: Add destructor syntax to the grammar

```text
// Add to ClassMember rule:
ClassMember:
  MethodDeclaration
  | PropertyDeclaration
  | DestructorDeclaration  // NEW: ~ClassName()

// Add new rule:
DestructorDeclaration:
  '~' Identifier '(' ')' Block
```

**Validation**: Ensure grammar parses `~ClassName() { /* cleanup code */ }` syntax correctly.

### Task 1.2: AST Extensions

**File**: `include/tsc/AST.h`
**Action**: Add destructor AST node

```cpp
class DestructorDeclaration : public ClassMember {
public:
    DestructorDeclaration(const std::string& className, 
                         std::unique_ptr<BlockStatement> body,
                         const SourceLocation& loc);
    
    const std::string& getClassName() const { return className_; }
    BlockStatement* getBody() const { return body_.get(); }
    
    void accept(ASTVisitor& visitor) override;
    
private:
    std::string className_;
    std::unique_ptr<BlockStatement> body_;
    SourceLocation location_;
};
```

**File**: `src/AST.cpp`
**Action**: Implement destructor AST node

```cpp
void DestructorDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}
```

### Task 1.3: Parser Integration

**File**: `src/parser/Parser.cpp`
**Action**: Add destructor parsing to `parseClassMember()` method

```cpp
// In parseClassMember() method, add:
if (currentToken_.getType() == TokenType::TILDE) {
    return parseDestructorDeclaration();
}

// Add new method:
std::unique_ptr<DestructorDeclaration> Parser::parseDestructorDeclaration() {
    // Parse ~ClassName() { body }
    // Return DestructorDeclaration AST node
}
```

---

## Phase 2: Semantic Analysis Integration

### Task 2.1: Destructor Validation

**File**: `include/tsc/semantic/SemanticAnalyzer.h`
**Action**: Add RAII analysis methods

```cpp
class RAIIAnalyzer {
public:
    void analyzeDestructor(const DestructorDeclaration& destructor);
    void validateRAIIPatterns(const ClassDeclaration& classDecl);
    void suggestResourceCleanup(const ClassDeclaration& classDecl);
    void detectResourceLeaks(const ClassDeclaration& classDecl);
};
```

**File**: `src/semantic/SemanticAnalyzer.cpp`
**Action**: Implement RAII analysis

```cpp
void SemanticAnalyzer::visit(DestructorDeclaration& node) {
    // Analyze destructor for resource cleanup
    // Validate RAII patterns
    // Check for resource leaks
}
```

### Task 2.2: Symbol Table Integration

**File**: `src/semantic/SymbolTable.cpp`
**Action**: Register destructors in symbol table

```cpp
// In setupBuiltinEnvironment() or similar:
// Register destructor functions for ARC integration
```

---

## Phase 3: Code Generation Integration

### Task 3.1: Destructor Code Generation

**File**: `include/tsc/codegen/LLVMCodeGen.h`
**Action**: Add destructor generation methods

```cpp
class RAIICodeGenerator {
public:
    llvm::Function* generateDestructorFunction(const DestructorDeclaration& node);
    void registerDestructor(const std::string& className, llvm::Function* destructor);
    void generateDestructorCall(llvm::Value* object, const std::string& className);
};
```

**File**: `src/codegen/LLVMCodeGen.cpp`
**Action**: Implement destructor code generation

```cpp
void LLVMCodeGen::visit(DestructorDeclaration& node) {
    // Generate destructor function
    llvm::Function* destructorFunc = generateDestructorFunction(node);
    
    // Register destructor with ARC runtime
    registerDestructor(node.getClassName(), destructorFunc);
}
```

### Task 3.2: ARC Integration

**File**: `src/codegen/LLVMCodeGen.cpp`
**Action**: Integrate destructor calls with ARC deallocation

```cpp
// In ARC deallocation code, add:
// Call destructor before deallocating object
if (hasDestructor(objectType)) {
    generateDestructorCall(object, objectType);
}
```

---

## Phase 4: RAII Runtime Integration

### Task 4.1: Destructor Registration

**File**: `runtime/arc/arc_runtime.h`
**Action**: Add destructor registration to ARC runtime

```c
// Add to ARC runtime API:
typedef void (*DestructorFunction)(void* object);

void __tsc_register_destructor(const char* className, DestructorFunction destructor);
void __tsc_call_destructor(void* object, const char* className);
bool __tsc_has_destructor(const char* className);
```

**File**: `runtime/arc/arc_runtime.c`
**Action**: Implement destructor registration

```c
// Implement destructor registry and calling mechanism
```

### Task 4.2: RAII Examples

**File**: `examples/raii_examples.ts`
**Action**: Create RAII pattern examples

```typescript
// File handle with automatic cleanup
class FileHandle {
    private handle: unique_ptr<File>;
    
    constructor(filename: string) {
        this.handle = new File(filename);
    }
    
    ~FileHandle() {
        if (this.handle) {
            this.handle.close();
        }
        // ARC automatically deallocates File object
    }
}

// Lock with automatic release
class MutexLock {
    private mutex: shared_ptr<Mutex>;
    
    constructor(mutex: shared_ptr<Mutex>) {
        this.mutex = mutex;
        this.mutex.lock();
    }
    
    ~MutexLock() {
        if (this.mutex) {
            this.mutex.unlock();
        }
    }
}

// Network connection with automatic cleanup
class NetworkConnection {
    private socket: unique_ptr<Socket>;
    
    constructor(host: string, port: number) {
        this.socket = new Socket(host, port);
        this.socket.connect();
    }
    
    ~NetworkConnection() {
        if (this.socket) {
            this.socket.disconnect();
        }
    }
}
```

---

## Phase 5: Testing & Validation

### Task 5.1: RAII Test Suite

**File**: `test/raii_tests.ts`
**Action**: Create comprehensive RAII tests

```typescript
// Test destructor syntax parsing
// Test RAII pattern validation
// Test resource cleanup
// Test exception safety
// Test integration with ARC

// Basic destructor test
class TestClass {
    private resource: string = "resource";
    
    ~TestClass() {
        console.log("Cleaning up:", this.resource);
    }
}

// RAII with smart pointers test
class FileManager {
    private file: unique_ptr<File>;
    
    constructor(filename: string) {
        this.file = new File(filename);
    }
    
    ~FileManager() {
        if (this.file) {
            this.file.close();
        }
    }
}

// Exception safety test
class ExceptionSafeResource {
    private resource: string = "resource";
    
    ~ExceptionSafeResource() {
        try {
            console.log("Cleaning up:", this.resource);
        } catch (e) {
            console.log("Cleanup failed:", e);
        }
    }
}
```

### Task 5.2: Integration Tests

**File**: `test/raii_integration_tests.ts`
**Action**: Test RAII + ARC integration

```typescript
// Test RAII with smart pointers
// Test RAII with move semantics
// Test complex RAII patterns
// Test performance impact

// RAII + ARC integration test
function testRAIIWithARC(): void {
    let fileManager: unique_ptr<FileManager> = new FileManager("test.txt");
    // FileManager destructor should be called when unique_ptr goes out of scope
    // ARC should deallocate the FileManager object after destructor
}

// RAII + Move semantics test
function testRAIIWithMove(): void {
    let source: unique_ptr<FileManager> = new FileManager("source.txt");
    let destination: unique_ptr<FileManager> = std::move(source);
    // source should be null, destination should own the FileManager
    // Destructor should be called when destination goes out of scope
}
```

---

## Success Criteria

### Functionality

- [ ] Destructor syntax `~ClassName()` parses correctly
- [ ] Destructors are called automatically when objects are deallocated
- [ ] RAII patterns work with ARC memory management
- [ ] Resource cleanup happens deterministically
- [ ] Exception safety during destruction

### Integration

- [ ] RAII works seamlessly with existing ARC system
- [ ] Destructors are called before ARC deallocation
- [ ] No conflicts with existing smart pointer functionality
- [ ] Performance impact is minimal

### Testing

- [ ] Comprehensive test suite for RAII functionality
- [ ] Integration tests with ARC
- [ ] Edge case testing (exceptions, complex patterns)
- [ ] Performance benchmarking

---

## Implementation Guidelines

### Code Style

- Follow existing TSC code style and patterns
- Use existing ARC infrastructure where possible
- Maintain backward compatibility
- Add comprehensive error handling

### Testing

- Write tests before implementing features
- Test both success and failure cases
- Include performance benchmarks
- Validate integration with existing features

### Documentation

- Update existing ARC documentation to include RAII
- Add RAII examples and best practices
- Document RAII + ARC integration patterns
- Create migration guide for RAII usage

---

## Deliverables

1. **Grammar Extensions**: Destructor syntax in `tsc.tm`
2. **AST Implementation**: `DestructorDeclaration` class
3. **Parser Integration**: Destructor parsing in `Parser.cpp`
4. **Semantic Analysis**: RAII analysis in `SemanticAnalyzer`
5. **Code Generation**: Destructor generation in `LLVMCodeGen`
6. **Runtime Integration**: Destructor registration in ARC runtime
7. **Test Suite**: Comprehensive RAII tests
8. **Examples**: RAII pattern examples
9. **Documentation**: Updated ARC documentation with RAII

---

## Timeline

- **Week 1**: Grammar, AST, and Parser implementation
- **Week 2**: Semantic analysis and validation
- **Week 3**: Code generation and runtime integration
- **Week 4**: Testing, examples, and documentation

---

## Key Files to Modify

### Grammar & Parser

- `tsc.tm` - Add destructor syntax
- `src/parser/Parser.cpp` - Add destructor parsing
- `include/tsc/parser/Parser.h` - Add destructor parsing methods

### AST & Semantic Analysis

- `include/tsc/AST.h` - Add `DestructorDeclaration` class
- `src/AST.cpp` - Implement `DestructorDeclaration`
- `include/tsc/semantic/SemanticAnalyzer.h` - Add RAII analysis
- `src/semantic/SemanticAnalyzer.cpp` - Implement RAII analysis

### Code Generation

- `include/tsc/codegen/LLVMCodeGen.h` - Add destructor generation
- `src/codegen/LLVMCodeGen.cpp` - Implement destructor generation

### Runtime

- `runtime/arc/arc_runtime.h` - Add destructor registration API
- `runtime/arc/arc_runtime.c` - Implement destructor registration

### Testing & Examples

- `test/raii_tests.ts` - RAII test suite
- `test/raii_integration_tests.ts` - Integration tests
- `examples/raii_examples.ts` - RAII pattern examples

---

## Expected Outcome

Upon completion, TSC will have a complete automatic resource management system:

1. **ARC** handles automatic memory management
2. **RAII** handles automatic resource cleanup
3. **Combined** provides comprehensive automatic resource management
4. **TypeScript syntax** with C++-like resource management capabilities
5. **Deterministic performance** with no garbage collection overhead
6. **Memory safety** and resource safety

This implementation will complete the TSC memory management system by adding RAII resource management to complement the
existing ARC memory management, providing a complete automatic resource management solution.

---

**Document Version**: 1.0  
**Created**: [Current Date]  
**Status**: Ready for Implementation  
**Target**: Background Agent Implementation
