#pragma once

#include "tsc/ast/ModuleSystem.h"
#include "tsc/ast/AdvancedTypes.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/ScopeManager.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

/**
 * Semantic analyzer for module system and advanced TypeScript features
 */
class ModuleSystemSemanticAnalyzer {
private:
    SymbolTable& symbolTable_;
    ScopeManager& scopeManager_;
    
    // Private helper methods
    bool analyzeModulePath(const std::string& path);
    bool analyzeDefaultImport(const std::string& import);
    bool analyzeNamedImport(const std::string& import);
    bool analyzeNamespaceImport(const std::string& import);
    bool analyzeNamespaceName(const std::string& name);
    bool analyzeIdentifier(const std::string& identifier);
    bool analyzeType(ast::Type* type);
    bool analyzeNamedExport(ast::ExportDeclaration* exportDecl);
    bool analyzeDefaultExport(ast::ExportDeclaration* exportDecl);
    bool analyzeReExport(ast::ExportDeclaration* exportDecl);
    bool analyzeNamespaceStatement(ast::Statement* statement);
    bool analyzeExpression(ast::Expression* expression);
    bool analyzeVariableDeclaration(ast::VariableDeclaration* varDecl);
    bool analyzeFunctionDeclaration(ast::FunctionDeclaration* funcDecl);
    bool analyzeClassDeclaration(ast::ClassDeclaration* classDecl);
    bool analyzeInterfaceDeclaration(ast::InterfaceDeclaration* interfaceDecl);
    bool analyzeTypeAliasDeclaration(ast::TypeAliasDeclaration* typeAliasDecl);
    bool hasImportConflicts(ast::ImportDeclaration* importDecl);
    bool hasNamespaceConflicts(ast::NamespaceDeclaration* namespaceDecl);
    bool hasDuplicateTypes(ast::UnionType* unionType);
    bool hasDuplicateTypes(ast::IntersectionType* intersectionType);
    bool isValidModulePath(const std::string& path);
    bool isValidIdentifier(const std::string& identifier);
    void registerImports(ast::ImportDeclaration* importDecl);
    void registerNamespace(ast::NamespaceDeclaration* namespaceDecl);
    
public:
    ModuleSystemSemanticAnalyzer(SymbolTable& symbolTable, ScopeManager& scopeManager);
    virtual ~ModuleSystemSemanticAnalyzer() = default;
    
    // Module system semantic analysis
    bool analyzeImportDeclaration(ast::ImportDeclaration* importDecl);
    bool analyzeExportDeclaration(ast::ExportDeclaration* exportDecl);
    bool analyzeNamespaceDeclaration(ast::NamespaceDeclaration* namespaceDecl);
    bool analyzeModuleResolution(ast::ModuleResolution* moduleResolution);
    bool analyzeCircularDependency(ast::CircularDependency* circularDep);
    
    // Advanced TypeScript features semantic analysis
    bool analyzeUnionType(ast::UnionType* unionType);
    bool analyzeIntersectionType(ast::IntersectionType* intersectionType);
    bool analyzeConditionalType(ast::ConditionalType* conditionalType);
    bool analyzeMappedType(ast::MappedType* mappedType);
    bool analyzeTypeParameter(ast::TypeParameter* typeParam);
    bool analyzeTemplateLiteralType(ast::TemplateLiteralType* templateType);
    bool analyzeKeyofType(ast::KeyofType* keyofType);
    bool analyzeTypeofType(ast::TypeofType* typeofType);
};

} // namespace semantic
} // namespace tsc