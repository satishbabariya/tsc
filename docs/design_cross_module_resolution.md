# Cross-Module Symbol Resolution Design

## Current Architecture Issues

The current `Compiler::compileModule` method has a fundamental architectural flaw:

- It compiles files individually using `compile(sourceFile)`
- No cross-module symbol resolution occurs
- Imported symbols remain unresolved
- Export-to-import binding never happens

## Proposed Solution

### Two-Phase Compilation Process

#### Phase 1: Cross-Module Analysis

1. **Dependency Scanning**: Use `SemanticAnalyzer::analyzeProject` to:
    - Scan all modules for import/export dependencies
    - Build dependency graph
    - Detect circular dependencies
    - Determine compilation order (topological sort)

2. **Symbol Resolution**:
    - Create `ModuleSymbolTable` for each module
    - Parse and analyze each module in dependency order
    - Perform export-to-import binding
    - Build unified symbol table for the entire project

#### Phase 2: Code Generation

1. **LLVM IR Generation**: For each module:
    - Use resolved symbols from Phase 1
    - Generate LLVM IR with proper external linkage
    - Create object files (.o) or bitcode files (.bc)

2. **Linking**: Link all generated object files into final executable

## Implementation Plan

### Step 1: Modify `Compiler::compileModule`

```cpp
CompilationResult Compiler::compileModule(const std::vector<String>& sourceFiles) {
    CompilationResult result;
    
    // Phase 1: Cross-module analysis
    SemanticAnalyzer analyzer(*diagnostics_, *typeChecker_->getTypeSystem());
    if (!analyzer.analyzeProject(sourceFiles)) {
        result.errorMessage = "Cross-module analysis failed";
        return result;
    }
    
    // Phase 2: Code generation for each module
    std::vector<String> objectFiles;
    for (const auto& sourceFile : sourceFiles) {
        auto fileResult = compileWithResolvedSymbols(sourceFile, analyzer);
        if (!fileResult.success) {
            return fileResult;
        }
        if (!fileResult.objectFile.empty()) {
            objectFiles.push_back(fileResult.objectFile);
        }
    }
    
    // Phase 3: Linking
    if (!objectFiles.empty() && !options_.outputFile.empty()) {
        if (!linkExecutable(objectFiles, options_.outputFile)) {
            result.errorMessage = "Module linking failed";
            return result;
        }
        result.executable = options_.outputFile;
    }
    
    result.success = true;
    return result;
}
```

### Step 2: Create `compileWithResolvedSymbols` Method

This method will:

- Parse the source file
- Use the resolved symbols from `SemanticAnalyzer`
- Generate LLVM IR with proper external linkage for imported/exported symbols
- Create object file

### Step 3: Update LLVM Code Generation

Modify `LLVMCodeGen` to:

- Use resolved symbols from `ModuleSymbolManager`
- Generate `ExternalLinkage` for exported symbols
- Generate `ExternalLinkage` declarations for imported symbols
- Ensure proper symbol visibility across modules

## Data Flow

```
Source Files → SemanticAnalyzer::analyzeProject → ModuleSymbolManager → LLVMCodeGen → Object Files → Linker → Executable
```

## Benefits

1. **Proper Cross-Module Resolution**: Imported symbols are resolved to exported symbols
2. **Circular Dependency Detection**: Prevents infinite loops during compilation
3. **Unified Symbol Table**: All modules share a consistent symbol table
4. **Better Error Reporting**: Cross-module errors are caught during analysis phase
5. **Performance**: Dependency order ensures efficient compilation

## Risks and Mitigation

### Risk: Performance Impact

- **Mitigation**: `analyzeProject` only runs once per compilation, and dependency scanning is efficient

### Risk: Complexity Increase

- **Mitigation**: Clear separation between analysis and code generation phases

### Risk: Potential Regressions

- **Mitigation**: Comprehensive testing of both individual and multi-module compilation

## Testing Strategy

1. **Unit Tests**: Test `compileWithResolvedSymbols` method
2. **Integration Tests**: Test cross-module symbol resolution
3. **End-to-End Tests**: Test complete multi-module compilation
4. **Regression Tests**: Ensure single-module compilation still works
5. **Performance Tests**: Measure compilation time impact

## Implementation Order

1. Design and implement `compileWithResolvedSymbols`
2. Modify `Compiler::compileModule` to use two-phase approach
3. Update `LLVMCodeGen` for cross-module symbol handling
4. Add comprehensive tests
5. Performance optimization if needed