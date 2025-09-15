#include "tsc/semantic/ModuleSymbolTable.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>

namespace tsc {

namespace fs = std::filesystem;

// ModuleSymbolTable implementation
ModuleSymbolTable::ModuleSymbolTable(const String& modulePath, DiagnosticEngine& diagnostics, SymbolTable* symbolTable)
    : modulePath_(modulePath), diagnostics_(diagnostics) {
    moduleName_ = extractModuleName(modulePath);
    if (symbolTable) {
        symbolTable_ = std::unique_ptr<SymbolTable>(symbolTable);
        ownsSymbolTable_ = false;
    } else {
        symbolTable_ = make_unique<SymbolTable>(moduleName_);
        ownsSymbolTable_ = true;
    }
    
    std::cout << "DEBUG: Created ModuleSymbolTable for module: " << moduleName_ 
              << " at path: " << modulePath_ << std::endl;
}

ModuleSymbolTable::~ModuleSymbolTable() {
    if (ownsSymbolTable_) {
        symbolTable_.release(); // Let unique_ptr handle deletion
    } else {
        symbolTable_.release(); // Don't delete, we don't own it
    }
}

void ModuleSymbolTable::addImportedSymbol(const ImportedSymbol& importedSymbol) {
    // Don't check for conflicts with existing symbols for imports
    // Imported symbols can reference existing symbols from other modules
    
    // Add to imported symbols list
    importedSymbols_.push_back(importedSymbol);
    
    // Don't create placeholder symbol immediately - wait for validation
    // The symbol will be created during export binding if it's valid
    
    std::cout << "DEBUG: Added imported symbol: " << importedSymbol.localName 
              << " from " << importedSymbol.sourceModulePath << std::endl;
}

Symbol* ModuleSymbolTable::lookupImportedSymbol(const String& name) const {
    for (const auto& imported : importedSymbols_) {
        if (imported.localName == name) {
            return symbolTable_->lookupSymbol(name);
        }
    }
    return nullptr;
}

void ModuleSymbolTable::addExportedSymbol(const ExportedSymbol& exportedSymbol) {
    // Check for conflicts with existing exports
    for (const auto& existing : exportedSymbols_) {
        if (existing.exportedName == exportedSymbol.exportedName) {
            diagnostics_.error("Duplicate export name: " + exportedSymbol.exportedName,
                              exportedSymbol.location);
            return;
        }
    }
    
    // Add to exported symbols list
    exportedSymbols_.push_back(exportedSymbol);
    
    // Mark the symbol as exported in the symbol table
    Symbol* symbol = symbolTable_->lookupSymbol(exportedSymbol.localName);
    if (symbol) {
        symbol->setExported(true);
    }
    
    std::cout << "DEBUG: Added exported symbol: " << exportedSymbol.exportedName 
              << " (local: " << exportedSymbol.localName << ")" << std::endl;
}

Symbol* ModuleSymbolTable::lookupExportedSymbol(const String& name) const {
    for (const auto& exported : exportedSymbols_) {
        if (exported.exportedName == name) {
            return symbolTable_->lookupSymbol(exported.localName);
        }
    }
    return nullptr;
}

Symbol* ModuleSymbolTable::resolveSymbol(const String& name) const {
    // First check local symbols
    Symbol* symbol = symbolTable_->lookupSymbol(name);
    if (symbol) {
        return symbol;
    }
    
    // Then check imported symbols
    return lookupImportedSymbol(name);
}

void ModuleSymbolTable::addModuleDependency(const String& modulePath) {
    moduleDependencies_.insert(modulePath);
    std::cout << "DEBUG: Added module dependency: " << modulePath << std::endl;
}

bool ModuleSymbolTable::validateExports() const {
    bool isValid = true;
    
    for (const auto& exported : exportedSymbols_) {
        if (!exported.isReExport) {
            // Check that the local symbol exists
            Symbol* symbol = symbolTable_->lookupSymbol(exported.localName);
            if (!symbol) {
                diagnostics_.error("Cannot export undefined symbol: " + exported.localName,
                                  exported.location);
                isValid = false;
            }
        }
    }
    
    return isValid;
}

bool ModuleSymbolTable::validateImports() const {
    bool isValid = true;
    
    for (const auto& imported : importedSymbols_) {
        // Check that the imported symbol name is valid
        if (imported.localName.empty() || imported.importedName.empty()) {
            diagnostics_.error("Invalid import: empty symbol name", imported.location);
            isValid = false;
        }
    }
    
    return isValid;
}

void ModuleSymbolTable::printModuleSymbolTable() const {
    std::cout << "\n=== Module Symbol Table: " << moduleName_ << " ===" << std::endl;
    std::cout << "Module Path: " << modulePath_ << std::endl;
    
    std::cout << "\nImported Symbols (" << importedSymbols_.size() << "):" << std::endl;
    for (const auto& imported : importedSymbols_) {
        std::cout << "  " << imported.localName << " <- " << imported.importedName 
                  << " from " << imported.sourceModulePath << std::endl;
    }
    
    std::cout << "\nExported Symbols (" << exportedSymbols_.size() << "):" << std::endl;
    for (const auto& exported : exportedSymbols_) {
        std::cout << "  " << exported.exportedName << " <- " << exported.localName;
        if (exported.isReExport) {
            std::cout << " (re-export from " << exported.sourceModulePath << ")";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nModule Dependencies (" << moduleDependencies_.size() << "):" << std::endl;
    for (const String& dep : moduleDependencies_) {
        std::cout << "  " << dep << std::endl;
    }
    
    std::cout << "\nSymbol Table:" << std::endl;
    symbolTable_->printSymbolTable();
}

String ModuleSymbolTable::toString() const {
    std::stringstream ss;
    ss << "ModuleSymbolTable(" << moduleName_ << "): ";
    ss << importedSymbols_.size() << " imports, ";
    ss << exportedSymbols_.size() << " exports, ";
    ss << moduleDependencies_.size() << " dependencies";
    return ss.str();
}

String ModuleSymbolTable::extractModuleName(const String& modulePath) const {
    fs::path path(modulePath);
    String filename = path.filename().string();
    
    // Remove extension
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != String::npos) {
        filename = filename.substr(0, dotPos);
    }
    
    return filename;
}

void ModuleSymbolTable::validateSymbolConflict(const String& name, const SourceLocation& location) const {
    // Check for conflicts with existing symbols
    Symbol* existing = symbolTable_->lookupSymbol(name);
    if (existing) {
        diagnostics_.error("Symbol '" + name + "' already declared in this scope", location);
    }
}

// ModuleSymbolManager implementation
ModuleSymbolManager::ModuleSymbolManager(DiagnosticEngine& diagnostics, SymbolTable* mainSymbolTable)
    : diagnostics_(diagnostics), mainSymbolTable_(mainSymbolTable) {
    std::cout << "DEBUG: Created ModuleSymbolManager" << std::endl;
}

ModuleSymbolManager::~ModuleSymbolManager() = default;

ModuleSymbolTable* ModuleSymbolManager::createModuleSymbolTable(const String& modulePath) {
    auto it = moduleTables_.find(modulePath);
    if (it != moduleTables_.end()) {
        return it->second.get();
    }
    
    auto moduleTable = make_unique<ModuleSymbolTable>(modulePath, diagnostics_, mainSymbolTable_);
    ModuleSymbolTable* result = moduleTable.get();
    moduleTables_[modulePath] = std::move(moduleTable);
    
    std::cout << "DEBUG: Created new ModuleSymbolTable for: " << modulePath << std::endl;
    return result;
}

ModuleSymbolTable* ModuleSymbolManager::getModuleSymbolTable(const String& modulePath) const {
    auto it = moduleTables_.find(modulePath);
    return (it != moduleTables_.end()) ? it->second.get() : nullptr;
}

void ModuleSymbolManager::removeModuleSymbolTable(const String& modulePath) {
    moduleTables_.erase(modulePath);
    std::cout << "DEBUG: Removed ModuleSymbolTable for: " << modulePath << std::endl;
}

Symbol* ModuleSymbolManager::resolveSymbolAcrossModules(const String& symbolName, 
                                                       const String& fromModule) const {
    ModuleSymbolTable* fromModuleTable = getModuleSymbolTable(fromModule);
    if (!fromModuleTable) {
        reportSymbolResolutionError(symbolName, fromModule, "Source module not found");
        return nullptr;
    }
    
    // First check if it's an imported symbol in the source module
    Symbol* symbol = fromModuleTable->lookupImportedSymbol(symbolName);
    if (symbol) {
        return symbol;
    }
    
    // Then check if it's a local symbol in the source module
    symbol = fromModuleTable->getSymbolTable().lookupSymbol(symbolName);
    if (symbol) {
        return symbol;
    }
    
    // Finally, check if it's exported from any other module
    for (const auto& pair : moduleTables_) {
        if (pair.first != fromModule) {
            symbol = pair.second->lookupExportedSymbol(symbolName);
            if (symbol) {
                return symbol;
            }
        }
    }
    
    reportSymbolResolutionError(symbolName, fromModule, "Symbol not found");
    return nullptr;
}

bool ModuleSymbolManager::bindExportsToImports() {
    try {
        bool success = true;
        
        std::cout << "DEBUG: *** ENTERING bindExportsToImports ***" << std::endl;
        std::cout << "DEBUG: Starting export-to-import binding" << std::endl;
        std::cout << "DEBUG: Total modules to process: " << moduleTables_.size() << std::endl;
        std::cout << "DEBUG: *** AFTER SIZE CHECK ***" << std::endl;
    
    for (const auto& pair : moduleTables_) {
        ModuleSymbolTable* moduleTable = pair.second.get();
        std::cout << "DEBUG: Processing module: " << pair.first << std::endl;
        std::cout << "DEBUG: Module has " << moduleTable->getImportedSymbols().size() << " imported symbols" << std::endl;
        
        // Process each imported symbol
        for (const auto& imported : moduleTable->getImportedSymbols()) {
            std::cout << "DEBUG: Processing imported symbol: " << imported.localName 
                      << " from " << imported.sourceModulePath << std::endl;
            // Find the source module
            ModuleSymbolTable* sourceModule = getModuleSymbolTable(imported.sourceModulePath);
            if (!sourceModule) {
                diagnostics_.error("Cannot resolve import: source module not found: " + 
                                 imported.sourceModulePath, imported.location);
                success = false;
                continue;
            }
            
            // Look up the exported symbol in the source module
            std::cout << "DEBUG: Looking for exported symbol '" << imported.importedName 
                      << "' in source module " << imported.sourceModulePath << std::endl;
            Symbol* exportedSymbol = sourceModule->lookupExportedSymbol(imported.importedName);
            if (!exportedSymbol) {
                std::cout << "DEBUG: Symbol '" << imported.importedName 
                          << "' not found in exported symbols of " << imported.sourceModulePath << std::endl;
                std::cout << "DEBUG: Source module has " << sourceModule->getExportedSymbols().size() 
                          << " exported symbols" << std::endl;
                diagnostics_.error("Cannot resolve import: symbol '" + imported.importedName + 
                                 "' not exported from " + imported.sourceModulePath, 
                                 imported.location);
                success = false;
                continue;
            }
            std::cout << "DEBUG: Found exported symbol '" << imported.importedName 
                      << "' in source module" << std::endl;
            
            // Create the imported symbol in the symbol table now that we know it's valid
            Symbol* importedSymbol = moduleTable->getSymbolTable().lookupSymbol(imported.localName);
            if (!importedSymbol) {
                // Create the symbol since it doesn't exist yet
                if (!moduleTable->getSymbolTable().addSymbol(imported.localName, imported.kind, 
                                                           exportedSymbol->getType(), imported.location)) {
                    diagnostics_.error("Cannot create import symbol: '" + imported.localName + 
                                     "' already exists", imported.location);
                    success = false;
                    continue;
                }
                importedSymbol = moduleTable->getSymbolTable().lookupSymbol(imported.localName);
            }
            
            if (importedSymbol) {
                // Update the symbol with type and declaration information from the exported symbol
                importedSymbol->setType(exportedSymbol->getType());
                importedSymbol->setDeclaration(exportedSymbol->getDeclaration());
                
                std::cout << "DEBUG: Bound import " << imported.localName 
                          << " to export " << imported.importedName 
                          << " from " << imported.sourceModulePath << std::endl;
            } else {
                std::cout << "DEBUG: Failed to create imported symbol " << imported.localName << std::endl;
                diagnostics_.error("Cannot create import symbol: '" + imported.localName + "'", imported.location);
                success = false;
            }
        }
    }
    
    return success;
    } catch (const std::exception& e) {
        std::cout << "DEBUG: Exception in bindExportsToImports: " << e.what() << std::endl;
        diagnostics_.error("Exception in export-to-import binding: " + String(e.what()), SourceLocation());
        return false;
    }
}

bool ModuleSymbolManager::validateAllModules() const {
    bool isValid = true;
    
    for (const auto& pair : moduleTables_) {
        ModuleSymbolTable* moduleTable = pair.second.get();
        
        if (!moduleTable->validateExports()) {
            isValid = false;
        }
        
        if (!moduleTable->validateImports()) {
            isValid = false;
        }
    }
    
    return isValid;
}

void ModuleSymbolManager::printAllModuleSymbolTables() const {
    std::cout << "\n=== All Module Symbol Tables ===" << std::endl;
    std::cout << "Total modules: " << moduleTables_.size() << std::endl;
    
    for (const auto& pair : moduleTables_) {
        pair.second->printModuleSymbolTable();
    }
}

size_t ModuleSymbolManager::getModuleCount() const {
    return moduleTables_.size();
}

std::vector<String> ModuleSymbolManager::getModulePaths() const {
    std::vector<String> paths;
    for (const auto& pair : moduleTables_) {
        paths.push_back(pair.first);
    }
    return paths;
}

void ModuleSymbolManager::reportSymbolResolutionError(const String& symbolName, 
                                                     const String& fromModule, 
                                                     const String& error) const {
    diagnostics_.error("Symbol resolution error in " + fromModule + ": " + error + 
                      " (symbol: " + symbolName + ")", SourceLocation(fromModule, 0, 0));
}

} // namespace tsc