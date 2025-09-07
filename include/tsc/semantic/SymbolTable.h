#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include <unordered_map>
#include <vector>

namespace tsc {

// Forward declarations
class Type;
class Scope;

// Symbol kinds for different declaration types
enum class SymbolKind {
    Variable,
    Function,
    Parameter,
    Class,
    Interface,
    TypeAlias,
    Namespace,
    Module,
    Property,
    Method,
    Type
};

// Symbol information stored in the symbol table
class Symbol {
public:
    Symbol(const String& name, SymbolKind kind, shared_ptr<Type> type, 
           const SourceLocation& location, ASTNode* declaration = nullptr);
    
    const String& getName() const { return name_; }
    SymbolKind getKind() const { return kind_; }
    shared_ptr<Type> getType() const { return type_; }
    const SourceLocation& getLocation() const { return location_; }
    ASTNode* getDeclaration() const { return declaration_; }
    
    // Mutability and access control
    bool isMutable() const { return mutable_; }
    void setMutable(bool mutable_val) { mutable_ = mutable_val; }
    
    bool isUsed() const { return used_; }
    void markUsed() { used_ = true; }
    
    bool isExported() const { return exported_; }
    void setExported(bool exported) { exported_ = exported; }
    
    // Type information
    void setType(shared_ptr<Type> type) { type_ = type; }
    
    String toString() const;

private:
    String name_;
    SymbolKind kind_;
    shared_ptr<Type> type_;
    SourceLocation location_;
    ASTNode* declaration_;
    bool mutable_ = true;
    bool used_ = false;
    bool exported_ = false;
};

// Scope represents a lexical scope with its own symbol table
class Scope {
public:
    enum class ScopeType {
        Global,
        Module,
        Function,
        Block,
        Class,
        Interface,
        Namespace
    };
    
    explicit Scope(ScopeType type, Scope* parent = nullptr, const String& name = "");
    ~Scope();
    
    // Symbol management
    bool addSymbol(unique_ptr<Symbol> symbol);
    Symbol* lookupSymbol(const String& name) const;
    Symbol* lookupSymbolLocal(const String& name) const;
    
    // Scope hierarchy
    Scope* getParent() const { return parent_; }
    const std::vector<unique_ptr<Scope>>& getChildren() const { return children_; }
    unique_ptr<Scope> createChildScope(ScopeType type, const String& name = "");
    
    // Scope properties
    ScopeType getType() const { return type_; }
    const String& getName() const { return name_; }
    
    // Utilities
    bool isGlobalScope() const { return type_ == ScopeType::Global; }
    bool isFunctionScope() const { return type_ == ScopeType::Function; }
    bool isBlockScope() const { return type_ == ScopeType::Block; }
    
    // Symbol iteration
    const std::unordered_map<String, unique_ptr<Symbol>>& getSymbols() const { return symbols_; }
    
    // Debug support
    String toString() const;
    void printScope(int indent = 0) const;

public: // Make children_ accessible to SymbolTable
    std::vector<unique_ptr<Scope>> children_;

private:
    ScopeType type_;
    Scope* parent_;
    String name_;
    std::unordered_map<String, unique_ptr<Symbol>> symbols_;
};

// Symbol table manages the entire symbol hierarchy for a module
class SymbolTable {
public:
    explicit SymbolTable(const String& moduleName = "");
    ~SymbolTable();
    
    // Scope management
    Scope* getCurrentScope() const { return currentScope_; }
    Scope* getGlobalScope() const { return globalScope_.get(); }
    
    void enterScope(Scope::ScopeType type, const String& name = "");
    void exitScope();
    
    // Symbol operations
    bool addSymbol(const String& name, SymbolKind kind, shared_ptr<Type> type, 
                   const SourceLocation& location, ASTNode* declaration = nullptr);
    Symbol* lookupSymbol(const String& name) const;
    Symbol* lookupSymbolInScope(const String& name, Scope* scope) const;
    
    // Built-in symbols
    void addBuiltinSymbols();
    
    // Validation
    bool validateSymbolRedeclaration(const String& name, const SourceLocation& location) const;
    std::vector<Symbol*> getUnusedSymbols() const;
    
    // Debug and introspection
    void printSymbolTable() const;
    String toString() const;
    
    // Statistics
    size_t getSymbolCount() const;
    size_t getScopeCount() const;

private:
    String moduleName_;
    unique_ptr<Scope> globalScope_;
    Scope* currentScope_;
    std::vector<Scope*> scopeStack_;
    
    void collectUnusedSymbols(Scope* scope, std::vector<Symbol*>& unused) const;
    size_t countScopes(Scope* scope) const;
    size_t countSymbols(Scope* scope) const;
};

// Factory functions for common symbol creation
unique_ptr<Symbol> createVariableSymbol(const String& name, shared_ptr<Type> type, 
                                       const SourceLocation& location, bool mutable_ = true);
unique_ptr<Symbol> createFunctionSymbol(const String& name, shared_ptr<Type> type, 
                                       const SourceLocation& location);
unique_ptr<Symbol> createParameterSymbol(const String& name, shared_ptr<Type> type, 
                                        const SourceLocation& location);

} // namespace tsc