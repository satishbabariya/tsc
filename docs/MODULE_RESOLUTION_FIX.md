# Module Resolution Fix

## Problem Identified

The import/export system has a bug in the `collectFunctionDeclarations` method in `src/semantic/SemanticAnalyzer.cpp`.
The issue is:

1. **Export declarations are parsed correctly** as `ExportDeclaration` with `Default` export clause
2. **Symbols are not being added to the symbol table** during the first pass (`collectFunctionDeclarations`)
3. **Module symbol tables can't find the symbols** during the export-to-import binding phase

## Root Cause

In `src/semantic/SemanticAnalyzer.cpp`, lines 1353-1375, the `collectFunctionDeclarations` method handles export
declarations with default exports, but there's a logic issue.

The method checks:

```cpp
if (clause.getType() == ExportClause::Default && clause.getDefaultExport()) {
    if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(clause.getDefaultExport())) {
        // Add function to symbol table
    }
}
```

However, the issue is that `export function add() { }` is parsed as an `ExportDeclaration` with a `Default` export
clause, but the `getDefaultExport()` method might not be returning the function declaration correctly.

## Proposed Fix

The fix involves ensuring that exported function declarations are properly added to the symbol table during the first
pass. Here are the changes needed:

### 1. Fix in `collectFunctionDeclarations` method

The method should handle export declarations more robustly:

```cpp
} else if (auto exportDecl = dynamic_cast<ExportDeclaration*>(stmt.get())) {
    // Handle export declarations that contain function/class declarations
    const ExportClause& clause = exportDecl->getClause();
    
    // Handle both default exports and named exports
    if (clause.getType() == ExportClause::Default && clause.getDefaultExport()) {
        if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(clause.getDefaultExport())) {
            // Add function to symbol table (existing code)
            // ... existing implementation ...
        }
    } else if (clause.getType() == ExportClause::Named) {
        // Handle named exports - this is the missing piece!
        const auto& namedExports = clause.getNamedExports();
        for (const auto& spec : namedExports) {
            // For named exports, we need to find the actual declaration
            // This requires looking at the statement that follows the export
            // or handling it differently
        }
    }
}
```

### 2. Alternative Fix: Handle in `visit(ExportDeclaration& node)`

The issue might be that the symbols are being added during the second pass (`visit(ExportDeclaration& node)`) but the
`ModuleSymbolTable` is trying to look them up before they're added.

The fix would be to ensure that symbols are added to the symbol table during the first pass, not just during the second
pass.

### 3. Debug the Issue

To confirm the exact issue, we need to:

1. Add debug output to see if `collectFunctionDeclarations` is being called
2. Add debug output to see if symbols are being added to the symbol table
3. Add debug output to see what's happening in the `visit(ExportDeclaration& node)` method

## Testing the Fix

Once the fix is implemented, test with:

```bash
./build-asan/tsc --emit-llvm -o test_fixed.ll moduleA.ts test_usage.ts
```

The expected behavior should be:

1. `moduleA.ts` exports `add` and `subtract` functions
2. `test_usage.ts` imports `add` and `subtract` from `moduleA.ts`
3. The symbols should be resolved correctly
4. No "Undefined symbol" errors should occur

## Current Status

The import/export parsing and grammar are working correctly. The issue is in the semantic analysis phase where symbols
are not being properly registered in the symbol table during the first pass, preventing them from being found during the
export-to-import binding phase.

The infrastructure is in place and working - it just needs this bug fix to complete the module resolution system.