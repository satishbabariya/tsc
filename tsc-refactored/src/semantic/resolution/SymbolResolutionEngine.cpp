#include "tsc/semantic/resolution/SymbolResolutionEngine.h"
#include "tsc/semantic/SemanticAnalyzerRefactored.h"

namespace tsc {
namespace semantic {

SymbolResolutionEngine::SymbolResolutionEngine(SemanticContext& context) 
    : context_(context) {
}

void SymbolResolutionEngine::resolveSymbols(Module& module) {
    // Phase 1: Collect all symbol declarations
    collectFunctionDeclarations(module);
    collectVariableDeclarations(module);
    collectClassDeclarations(module);
    collectInterfaceDeclarations(module);
    collectEnumDeclarations(module);
    collectTypeAliasDeclarations(module);
    
    // Phase 2: Resolve symbol references
    resolveSymbolReferences(module);
    
    // Phase 3: Resolve inheritance relationships
    resolveInheritance(module);
    
    // Phase 4: Resolve deferred expressions
    resolveDeferredSuperExpressions();
    resolveDeferredSuperPropertyAccesses();
    resolveDeferredSuperCallExpressions();
}

void SymbolResolutionEngine::collectFunctionDeclarations(Module& module) {
    // First pass: collect function signatures and class declarations, don't process bodies
    const auto& statements = module.getStatements();
    std::cout << "DEBUG: collectFunctionDeclarations found " << statements.size() << " statements" << std::endl;

    for (const auto& stmt : statements) {
        std::cout << "DEBUG: Processing statement type: " << typeid(*stmt.get()).name() << std::endl;
        if (auto classDecl = dynamic_cast<ClassDeclaration*>(stmt.get())) {
            // Collect class declarations first so they're available for constructor calls
            // Create class type WITHOUT base class (will be resolved in second pass)
            auto classType = context_.getTypeSystem().createClassType(classDecl->getName(), classDecl, nullptr);
            context_.getSymbolTable().addSymbol(classDecl->getName(), SymbolKind::Class, classType, classDecl->getLocation());
        } else if (auto exportDecl = dynamic_cast<ExportDeclaration*>(stmt.get())) {
            // Handle export declarations that contain function/class declarations
            std::cout << "DEBUG: Found export declaration in collectFunctionDeclarations" << std::endl;
            const ExportClause& clause = exportDecl->getClause();
            std::cout << "DEBUG: Export clause type: " << clause.getType() << std::endl;
            if (clause.getType() == ExportClause::Default && clause.getDefaultExport()) {
                std::cout << "DEBUG: Processing default export" << std::endl;
                if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(clause.getDefaultExport())) {
                    // Collect exported function declarations
                    std::vector<FunctionType::Parameter> paramTypes;
                    for (const auto& param : funcDecl->getParameters()) {
                        FunctionType::Parameter funcParam;
                        funcParam.name = param.name;
                        funcParam.type = param.type ? param.type : context_.getTypeSystem().getAnyType();
                        funcParam.optional = param.optional;
                        funcParam.rest = param.rest;
                        paramTypes.push_back(funcParam);
                    }

                    auto returnType = funcDecl->getReturnType() ? funcDecl->getReturnType() : context_.getTypeSystem().getAnyType();
                    auto functionType = context_.getTypeSystem().createFunctionType(paramTypes, returnType);
                    context_.getSymbolTable().addSymbol(funcDecl->getName(), SymbolKind::Function, functionType, funcDecl->getLocation());
                }
            }
        } else if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(stmt.get())) {
            // Collect function declarations
            std::vector<FunctionType::Parameter> paramTypes;
            for (const auto& param : funcDecl->getParameters()) {
                FunctionType::Parameter funcParam;
                funcParam.name = param.name;
                funcParam.type = param.type ? param.type : context_.getTypeSystem().getAnyType();
                funcParam.optional = param.optional;
                funcParam.rest = param.rest;
                paramTypes.push_back(funcParam);
            }

            auto returnType = funcDecl->getReturnType() ? funcDecl->getReturnType() : context_.getTypeSystem().getAnyType();
            auto functionType = context_.getTypeSystem().createFunctionType(paramTypes, returnType);
            context_.getSymbolTable().addSymbol(funcDecl->getName(), SymbolKind::Function, functionType, funcDecl->getLocation());
        }
    }
}

void SymbolResolutionEngine::collectVariableDeclarations(Module& module) {
    // TODO: Implement variable declaration collection
}

void SymbolResolutionEngine::collectClassDeclarations(Module& module) {
    // TODO: Implement class declaration collection
}

void SymbolResolutionEngine::collectInterfaceDeclarations(Module& module) {
    // TODO: Implement interface declaration collection
}

void SymbolResolutionEngine::collectEnumDeclarations(Module& module) {
    // TODO: Implement enum declaration collection
}

void SymbolResolutionEngine::collectTypeAliasDeclarations(Module& module) {
    // TODO: Implement type alias declaration collection
}

void SymbolResolutionEngine::resolveSymbolReferences(Module& module) {
    // TODO: Implement symbol reference resolution
}

void SymbolResolutionEngine::resolveInheritance(Module& module) {
    // Second pass: resolve inheritance relationships
    const auto& statements = module.getStatements();
    
    for (const auto& stmt : statements) {
        if (auto classDecl = dynamic_cast<ClassDeclaration*>(stmt.get())) {
            if (classDecl->getBaseClass()) {
                // Resolve base class type
                auto baseClassType = context_.getTypeSystem().lookupType(classDecl->getBaseClass()->getName());
                if (baseClassType) {
                    // Update the class type with the resolved base class
                    auto classType = context_.getTypeSystem().createClassType(classDecl->getName(), classDecl, baseClassType);
                    // Update the symbol table entry
                    context_.getSymbolTable().updateSymbolType(classDecl->getName(), classType);
                }
            }
        }
    }
}

void SymbolResolutionEngine::resolveDeferredSuperExpressions() {
    // TODO: Implement deferred super expression resolution
}

void SymbolResolutionEngine::resolveDeferredSuperPropertyAccesses() {
    // TODO: Implement deferred super property access resolution
}

void SymbolResolutionEngine::resolveDeferredSuperCallExpressions() {
    // TODO: Implement deferred super call expression resolution
}

Symbol* SymbolResolutionEngine::resolveIdentifier(const Identifier& identifier) {
    // TODO: Implement identifier resolution
    return nullptr;
}

Symbol* SymbolResolutionEngine::resolvePropertyAccess(const PropertyAccess& access) {
    // TODO: Implement property access resolution
    return nullptr;
}

Symbol* SymbolResolutionEngine::resolveCallExpression(const CallExpression& call) {
    // TODO: Implement call expression resolution
    return nullptr;
}

void SymbolResolutionEngine::collectSymbolsFromNode(ASTNode& node) {
    // TODO: Implement symbol collection from AST node
}

void SymbolResolutionEngine::resolveSymbolsInNode(ASTNode& node) {
    // TODO: Implement symbol resolution in AST node
}

Symbol* SymbolResolutionEngine::createSymbolForDeclaration(ASTNode& declaration) {
    // TODO: Implement symbol creation for declaration
    return nullptr;
}

} // namespace semantic
} // namespace tsc