#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <sstream>

namespace tsc {

// Symbol implementation
Symbol::Symbol(const String& name, SymbolKind kind, shared_ptr<Type> type, 
               const SourceLocation& location, ASTNode* declaration)
    : name_(name), kind_(kind), type_(type), location_(location), declaration_(declaration) {
}

String Symbol::toString() const {
    std::stringstream ss;
    ss << name_ << ": ";
    
    switch (kind_) {
        case SymbolKind::Variable: ss << "variable"; break;
        case SymbolKind::Function: ss << "function"; break;
        case SymbolKind::Parameter: ss << "parameter"; break;
        case SymbolKind::Class: ss << "class"; break;
        case SymbolKind::Interface: ss << "interface"; break;
        case SymbolKind::TypeAlias: ss << "type alias"; break;
        case SymbolKind::Namespace: ss << "namespace"; break;
        case SymbolKind::Module: ss << "module"; break;
    }
    
    if (type_) {
        ss << " of type " << type_->toString();
    }
    
    if (!mutable_) {
        ss << " (readonly)";
    }
    
    if (used_) {
        ss << " (used)";
    }
    
    return ss.str();
}

// Scope implementation
Scope::Scope(ScopeType type, Scope* parent, const String& name)
    : type_(type), parent_(parent), name_(name) {
}

Scope::~Scope() = default;

bool Scope::addSymbol(unique_ptr<Symbol> symbol) {
    const String& name = symbol->getName();
    
    // Check for redeclaration in current scope
    if (symbols_.find(name) != symbols_.end()) {
        return false;
    }
    
    symbols_[name] = std::move(symbol);
    return true;
}

Symbol* Scope::lookupSymbol(const String& name) const {
    std::cout << "DEBUG: Scope::lookupSymbol searching for '" << name << "' in scope " << this << " (type: " << static_cast<int>(type_) << ")" << std::endl;
    
    // Look in current scope first
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        std::cout << "DEBUG: Found symbol '" << name << "' in scope " << this << std::endl;
        return it->second.get();
    }
    
    // Look in parent scopes
    if (parent_) {
        std::cout << "DEBUG: Symbol not found in scope " << this << ", searching parent scope " << parent_ << std::endl;
        return parent_->lookupSymbol(name);
    }
    
    std::cout << "DEBUG: Symbol '" << name << "' not found in any scope" << std::endl;
    return nullptr;
}

Symbol* Scope::lookupSymbolLocal(const String& name) const {
    auto it = symbols_.find(name);
    return (it != symbols_.end()) ? it->second.get() : nullptr;
}

Symbol* Scope::lookupSymbolInChildren(const String& name) const {
    // Search in current scope first
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return it->second.get();
    }
    
    // Search in child scopes
    for (const auto& child : children_) {
        Symbol* found = child->lookupSymbolInChildren(name);
        if (found) {
            return found;
        }
    }
    
    return nullptr;
}

Scope* Scope::createChildScope(ScopeType type, const String& name) {
    auto child = make_unique<Scope>(type, this, name);
    Scope* childPtr = child.get();
    children_.push_back(std::move(child));
    return childPtr; // Return the actual child scope that was added
}

String Scope::toString() const {
    std::stringstream ss;
    
    switch (type_) {
        case ScopeType::Global: ss << "Global"; break;
        case ScopeType::Module: ss << "Module"; break;
        case ScopeType::Function: ss << "Function"; break;
        case ScopeType::Block: ss << "Block"; break;
        case ScopeType::Class: ss << "Class"; break;
        case ScopeType::Interface: ss << "Interface"; break;
        case ScopeType::Namespace: ss << "Namespace"; break;
    }
    
    if (!name_.empty()) {
        ss << " '" << name_ << "'";
    }
    
    ss << " (" << symbols_.size() << " symbols)";
    
    return ss.str();
}

void Scope::printScope(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    
    std::cout << toString() << std::endl;
    
    // Print symbols
    for (const auto& [name, symbol] : symbols_) {
        for (int i = 0; i <= indent; ++i) {
            std::cout << "  ";
        }
        std::cout << "- " << symbol->toString() << std::endl;
    }
    
    // Print child scopes
    for (const auto& child : children_) {
        child->printScope(indent + 1);
    }
}

// SymbolTable implementation
SymbolTable::SymbolTable(const String& moduleName) : moduleName_(moduleName) {
    globalScope_ = make_unique<Scope>(Scope::ScopeType::Global, nullptr, "global");
    currentScope_ = globalScope_.get();
    scopeStack_.push_back(currentScope_);
    
    addBuiltinSymbols();
}

SymbolTable::~SymbolTable() = default;

void SymbolTable::enterScope(Scope::ScopeType type, const String& name) {
    auto newScope = make_unique<Scope>(type, currentScope_, name);
    Scope* newScopePtr = newScope.get();
    
    // Add as child to current scope
    currentScope_->children_.push_back(std::move(newScope));
    
    // Update current scope
    currentScope_ = newScopePtr;
    scopeStack_.push_back(currentScope_);
}

void SymbolTable::exitScope() {
    if (scopeStack_.size() > 1) {
        scopeStack_.pop_back();
        currentScope_ = scopeStack_.back();
    }
}

bool SymbolTable::addSymbol(const String& name, SymbolKind kind, shared_ptr<Type> type, 
                           const SourceLocation& location, ASTNode* declaration) {
    auto symbol = make_unique<Symbol>(name, kind, type, location, declaration);
    return currentScope_->addSymbol(std::move(symbol));
}

Symbol* SymbolTable::lookupSymbol(const String& name) const {
    std::cout << "DEBUG: SymbolTable::lookupSymbol searching for '" << name << "' starting from scope " << currentScope_ << std::endl;
    
    // First try the normal lookup (current scope and parents)
    Symbol* symbol = currentScope_->lookupSymbol(name);
    if (symbol) {
        std::cout << "DEBUG: Found '" << name << "' in parent hierarchy" << std::endl;
        return symbol;
    }
    
    std::cout << "DEBUG: Not found in parent hierarchy, searching child scopes..." << std::endl;
    
    // If not found, also search in child scopes
    Symbol* childSymbol = currentScope_->lookupSymbolInChildren(name);
    if (childSymbol) {
        std::cout << "DEBUG: Found '" << name << "' in child scopes" << std::endl;
        return childSymbol;
    }
    
    std::cout << "DEBUG: '" << name << "' not found in any scope (parent or child)" << std::endl;
    return nullptr;
}

Symbol* SymbolTable::lookupSymbolInScope(const String& name, Scope* scope) const {
    return scope ? scope->lookupSymbol(name) : nullptr;
}

void SymbolTable::addBuiltinSymbols() {
    // Add built-in global functions and variables
    auto printType = make_shared<FunctionType>(
        std::vector<std::shared_ptr<Type>>{make_shared<PrimitiveType>(TypeKind::Any)}, // parameter: any
        make_shared<PrimitiveType>(TypeKind::Void) // return: void
    );
    addSymbol("print", SymbolKind::Function, printType, SourceLocation());
    
    // Add built-in number constants
    auto numberType = make_shared<PrimitiveType>(TypeKind::Number);
    addSymbol("Infinity", SymbolKind::Variable, numberType, SourceLocation());
    addSymbol("NaN", SymbolKind::Variable, numberType, SourceLocation());
    
    // Add built-in types (these would be handled by the type system)
    // This is mainly for demonstration - in a full implementation,
    // built-in types would be handled differently
}

bool SymbolTable::validateSymbolRedeclaration(const String& name, const SourceLocation& location) const {
    Symbol* existing = currentScope_->lookupSymbolLocal(name);
    return existing == nullptr;
}

std::vector<Symbol*> SymbolTable::getUnusedSymbols() const {
    std::vector<Symbol*> unused;
    collectUnusedSymbols(globalScope_.get(), unused);
    return unused;
}

void SymbolTable::printSymbolTable() const {
    std::cout << "=== Symbol Table for " << moduleName_ << " ===" << std::endl;
    globalScope_->printScope(0);
    std::cout << "============================================" << std::endl;
}

String SymbolTable::toString() const {
    return "SymbolTable(" + moduleName_ + ", " + std::to_string(getSymbolCount()) + " symbols)";
}

size_t SymbolTable::getSymbolCount() const {
    return countSymbols(globalScope_.get());
}

size_t SymbolTable::getScopeCount() const {
    return countScopes(globalScope_.get());
}

void SymbolTable::collectUnusedSymbols(Scope* scope, std::vector<Symbol*>& unused) const {
    for (const auto& [name, symbol] : scope->getSymbols()) {
        if (!symbol->isUsed() && symbol->getKind() != SymbolKind::Parameter) {
            unused.push_back(symbol.get());
        }
    }
    
    for (const auto& child : scope->getChildren()) {
        collectUnusedSymbols(child.get(), unused);
    }
}

size_t SymbolTable::countScopes(Scope* scope) const {
    size_t count = 1;
    for (const auto& child : scope->getChildren()) {
        count += countScopes(child.get());
    }
    return count;
}

size_t SymbolTable::countSymbols(Scope* scope) const {
    size_t count = scope->getSymbols().size();
    for (const auto& child : scope->getChildren()) {
        count += countSymbols(child.get());
    }
    return count;
}

// Factory functions
unique_ptr<Symbol> createVariableSymbol(const String& name, shared_ptr<Type> type, 
                                       const SourceLocation& location, bool mutable_) {
    auto symbol = make_unique<Symbol>(name, SymbolKind::Variable, type, location);
    symbol->setMutable(mutable_);
    return symbol;
}

unique_ptr<Symbol> createFunctionSymbol(const String& name, shared_ptr<Type> type, 
                                       const SourceLocation& location) {
    return make_unique<Symbol>(name, SymbolKind::Function, type, location);
}

unique_ptr<Symbol> createParameterSymbol(const String& name, shared_ptr<Type> type, 
                                        const SourceLocation& location) {
    return make_unique<Symbol>(name, SymbolKind::Parameter, type, location);
}

// Scope navigation implementation
Scope* SymbolTable::findScopeByName(const String& name) const {
    return findScopeByNameRecursive(globalScope_.get(), name);
}

Scope* SymbolTable::findScopeByNameRecursive(Scope* scope, const String& name) const {
    if (!scope) return nullptr;
    
    // Check if current scope matches
    if (scope->getName() == name) {
        return scope;
    }
    
    // Search in children
    for (const auto& child : scope->getChildren()) {
        Scope* found = findScopeByNameRecursive(child.get(), name);
        if (found) return found;
    }
    
    return nullptr;
}

bool SymbolTable::navigateToScope(Scope* scope) {
    if (!scope) return false;
    
    // Push current scope onto stack
    scopeStack_.push_back(currentScope_);
    
    // Navigate to the new scope
    currentScope_ = scope;
    
    std::cout << "DEBUG: Navigated to scope " << scope << " (type: " << static_cast<int>(scope->getType()) 
              << ", name: " << scope->getName() << ")" << std::endl;
    
    return true;
}

void SymbolTable::pushScope(Scope* scope) {
    if (scope) {
        scopeStack_.push_back(currentScope_);
        currentScope_ = scope;
    }
}

void SymbolTable::popScope() {
    if (!scopeStack_.empty()) {
        currentScope_ = scopeStack_.back();
        scopeStack_.pop_back();
        
        std::cout << "DEBUG: Popped scope, current scope: " << currentScope_ 
                  << " (type: " << static_cast<int>(currentScope_->getType()) 
                  << ", name: " << currentScope_->getName() << ")" << std::endl;
    }
}

} // namespace tsc