#pragma once

#include "tsc/Common.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/ModuleResolver.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace tsc {

// Forward declarations
class Module;
class ImportDeclaration;
class ExportDeclaration;

// Information about an imported symbol
struct ImportedSymbol {
    String localName;           // Name used in the importing module
    String importedName;        // Original name in the source module
    String sourceModulePath;    // Path to the source module
    SourceLocation location;   // Location of the import statement
    SymbolKind kind;           // Kind of the imported symbol
    
    ImportedSymbol(const String& local, const String& imported, const String& source,
                   const SourceLocation& loc, SymbolKind k)
        : localName(local), importedName(imported), sourceModulePath(source), 
          location(loc), kind(k) {}
};

// Information about an exported symbol
struct ExportedSymbol {
    String exportedName;        // Name used when exporting
    String localName;          // Original name in the current module
    SourceLocation location;   // Location of the export statement
    SymbolKind kind;           // Kind of the exported symbol
    bool isReExport;           // Whether this is a re-export from another module
    String sourceModulePath;   // Path to source module (for re-exports)
    
    ExportedSymbol(const String& exported, const String& local, const SourceLocation& loc,
                   SymbolKind k, bool reExport = false, const String& source = "")
        : exportedName(exported), localName(local), location(loc), kind(k),
          isReExport(reExport), sourceModulePath(source) {}
};

// Module symbol table that extends the base SymbolTable with module-specific functionality
class ModuleSymbolTable {
public:
    explicit ModuleSymbolTable(const String& modulePath, DiagnosticEngine& diagnostics, SymbolTable* symbolTable = nullptr);
    ~ModuleSymbolTable();
    
    // Module information
    const String& getModulePath() const { return modulePath_; }
    const String& getModuleName() const { return moduleName_; }
    
    // Access to the underlying symbol table
    SymbolTable& getSymbolTable() { return *symbolTable_; }
    const SymbolTable& getSymbolTable() const { return *symbolTable_; }
    SymbolTable* getSymbolTablePtr() { return symbolTable_.get(); }
    
    // Import management
    void addImportedSymbol(const ImportedSymbol& importedSymbol);
    Symbol* lookupImportedSymbol(const String& name) const;
    const std::vector<ImportedSymbol>& getImportedSymbols() const { return importedSymbols_; }
    
    // Export management
    void addExportedSymbol(const ExportedSymbol& exportedSymbol);
    Symbol* lookupExportedSymbol(const String& name) const;
    const std::vector<ExportedSymbol>& getExportedSymbols() const { return exportedSymbols_; }
    
    // Symbol resolution across module boundaries
    Symbol* resolveSymbol(const String& name) const;
    
    // Module dependency tracking
    void addModuleDependency(const String& modulePath);
    const std::unordered_set<String>& getModuleDependencies() const { return moduleDependencies_; }
    
    // Validation
    bool validateExports() const;
    bool validateImports() const;
    
    // Debug support
    void printModuleSymbolTable() const;
    String toString() const;

private:
    String modulePath_;
    String moduleName_;
    DiagnosticEngine& diagnostics_;
    unique_ptr<SymbolTable> symbolTable_;
    bool ownsSymbolTable_;
    
    std::vector<ImportedSymbol> importedSymbols_;
    std::vector<ExportedSymbol> exportedSymbols_;
    std::unordered_set<String> moduleDependencies_;
    
    // Helper methods
    String extractModuleName(const String& modulePath) const;
    void validateSymbolConflict(const String& name, const SourceLocation& location) const;
};

// Manager for multiple module symbol tables
class ModuleSymbolManager {
public:
    explicit ModuleSymbolManager(DiagnosticEngine& diagnostics, SymbolTable* mainSymbolTable = nullptr);
    ~ModuleSymbolManager();
    
    // Module management
    ModuleSymbolTable* createModuleSymbolTable(const String& modulePath);
    ModuleSymbolTable* getModuleSymbolTable(const String& modulePath) const;
    void removeModuleSymbolTable(const String& modulePath);
    
    // Cross-module symbol resolution
    Symbol* resolveSymbolAcrossModules(const String& symbolName, const String& fromModule) const;
    
    // Export binding - bind exported symbols to imported symbols
    bool bindExportsToImports();
    
    // Validation across all modules
    bool validateAllModules() const;
    
    // Debug support
    void printAllModuleSymbolTables() const;
    
    // Statistics
    size_t getModuleCount() const;
    std::vector<String> getModulePaths() const;

private:
    DiagnosticEngine& diagnostics_;
    SymbolTable* mainSymbolTable_;
    std::unordered_map<String, unique_ptr<ModuleSymbolTable>> moduleTables_;
    
    // Helper methods
    bool validateModuleDependencies() const;
    void reportSymbolResolutionError(const String& symbolName, const String& fromModule, 
                                   const String& error) const;
};

} // namespace tsc