#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {
namespace semantic {

// Forward declarations
class SemanticContext;

// Symbol resolution engine for handling symbol declarations and references
class SymbolResolutionEngine {
public:
    explicit SymbolResolutionEngine(SemanticContext& context);
    
    // Main resolution interface
    void resolveSymbols(Module& module);
    
    // Symbol collection
    void collectFunctionDeclarations(Module& module);
    void collectVariableDeclarations(Module& module);
    void collectClassDeclarations(Module& module);
    void collectInterfaceDeclarations(Module& module);
    void collectEnumDeclarations(Module& module);
    void collectTypeAliasDeclarations(Module& module);
    
    // Symbol resolution
    void resolveSymbolReferences(Module& module);
    void resolveInheritance(Module& module);
    
    // Deferred resolution
    void resolveDeferredSuperExpressions();
    void resolveDeferredSuperPropertyAccesses();
    void resolveDeferredSuperCallExpressions();
    
    // Helper methods
    Symbol* resolveIdentifier(const Identifier& identifier);
    Symbol* resolvePropertyAccess(const PropertyAccess& access);
    Symbol* resolveCallExpression(const CallExpression& call);

private:
    SemanticContext& context_;
    
    // Deferred resolution tracking
    std::vector<SuperExpression*> deferredSuperExpressions_;
    std::vector<PropertyAccess*> deferredSuperPropertyAccesses_;
    std::vector<CallExpression*> deferredSuperCallExpressions_;
    
    // Helper methods
    void collectSymbolsFromNode(ASTNode& node);
    void resolveSymbolsInNode(ASTNode& node);
    Symbol* createSymbolForDeclaration(ASTNode& declaration);
};

} // namespace semantic
} // namespace tsc