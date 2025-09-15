#pragma once

#include "tsc/ast/ModuleSystem.h"
#include "tsc/ast/AdvancedTypes.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeChecker.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

/**
 * Type checker for module system and advanced TypeScript features
 */
class ModuleSystemTypeChecker {
private:
    SymbolTable& symbolTable_;
    TypeChecker& typeChecker_;
    
    // Private helper methods
    bool isValidModulePath(const std::string& path);
    bool isValidIdentifier(const std::string& identifier);
    bool hasImportConflicts(ast::ImportDeclaration* importDecl);
    bool hasNamespaceConflicts(ast::NamespaceDeclaration* namespaceDecl);
    bool hasDuplicateTypes(ast::UnionType* unionType);
    bool hasDuplicateTypes(ast::IntersectionType* intersectionType);
    bool checkNamedExport(ast::ExportDeclaration* exportDecl);
    bool checkDefaultExport(ast::ExportDeclaration* exportDecl);
    bool checkReExport(ast::ExportDeclaration* exportDecl);
    bool checkNamespaceStatement(ast::Statement* statement);
    void registerImports(ast::ImportDeclaration* importDecl);
    void registerNamespace(ast::NamespaceDeclaration* namespaceDecl);
    
public:
    ModuleSystemTypeChecker(SymbolTable& symbolTable, TypeChecker& typeChecker);
    virtual ~ModuleSystemTypeChecker() = default;
    
    // Module system type checking
    bool checkImportDeclaration(ast::ImportDeclaration* importDecl);
    bool checkExportDeclaration(ast::ExportDeclaration* exportDecl);
    bool checkNamespaceDeclaration(ast::NamespaceDeclaration* namespaceDecl);
    bool checkModuleResolution(ast::ModuleResolution* moduleResolution);
    bool checkCircularDependency(ast::CircularDependency* circularDep);
    
    // Advanced TypeScript features type checking
    bool checkUnionType(ast::UnionType* unionType);
    bool checkIntersectionType(ast::IntersectionType* intersectionType);
    bool checkConditionalType(ast::ConditionalType* conditionalType);
    bool checkMappedType(ast::MappedType* mappedType);
    bool checkTypeParameter(ast::TypeParameter* typeParam);
    bool checkTemplateLiteralType(ast::TemplateLiteralType* templateType);
    bool checkKeyofType(ast::KeyofType* keyofType);
    bool checkTypeofType(ast::TypeofType* typeofType);
    
    // Static compilation validation
    bool validateStaticCompilationConstraints(ast::ImportDeclaration* importDecl);
    bool isStaticallyResolvable(const std::string& modulePath);
};

} // namespace semantic
} // namespace tsc