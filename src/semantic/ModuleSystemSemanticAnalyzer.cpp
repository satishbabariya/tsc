#include "tsc/semantic/ModuleSystemSemanticAnalyzer.h"
#include "tsc/ast/ModuleSystem.h"
#include "tsc/ast/AdvancedTypes.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/ScopeManager.h"
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

namespace tsc {
namespace semantic {

ModuleSystemSemanticAnalyzer::ModuleSystemSemanticAnalyzer(SymbolTable& symbolTable, ScopeManager& scopeManager)
    : symbolTable_(symbolTable), scopeManager_(scopeManager) {
}

bool ModuleSystemSemanticAnalyzer::analyzeImportDeclaration(ast::ImportDeclaration* importDecl) {
    if (!importDecl) {
        std::cerr << "Error: Import declaration is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze module path
        if (!analyzeModulePath(importDecl->getModulePath())) {
            std::cerr << "Error: Failed to analyze module path: " << importDecl->getModulePath() << std::endl;
            return false;
        }
        
        // Analyze default import
        if (importDecl->hasDefaultImport()) {
            if (!analyzeDefaultImport(importDecl->getDefaultImport())) {
                std::cerr << "Error: Failed to analyze default import: " << importDecl->getDefaultImport() << std::endl;
                return false;
            }
        }
        
        // Analyze named imports
        for (const auto& namedImport : importDecl->getNamedImports()) {
            if (!analyzeNamedImport(namedImport)) {
                std::cerr << "Error: Failed to analyze named import: " << namedImport << std::endl;
                return false;
            }
        }
        
        // Analyze namespace import
        if (importDecl->hasNamespaceImport()) {
            if (!analyzeNamespaceImport(importDecl->getNamespaceImport())) {
                std::cerr << "Error: Failed to analyze namespace import: " << importDecl->getNamespaceImport() << std::endl;
                return false;
            }
        }
        
        // Check for import conflicts
        if (hasImportConflicts(importDecl)) {
            std::cerr << "Error: Import conflicts detected" << std::endl;
            return false;
        }
        
        // Register imports in symbol table
        registerImports(importDecl);
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing import declaration: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeExportDeclaration(ast::ExportDeclaration* exportDecl) {
    if (!exportDecl) {
        std::cerr << "Error: Export declaration is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze export type
        switch (exportDecl->getExportType()) {
            case ast::ExportDeclaration::ExportType::Named:
                return analyzeNamedExport(exportDecl);
            case ast::ExportDeclaration::ExportType::Default:
                return analyzeDefaultExport(exportDecl);
            case ast::ExportDeclaration::ExportType::All:
                return analyzeReExport(exportDecl);
            default:
                std::cerr << "Error: Unknown export type" << std::endl;
                return false;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing export declaration: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeNamespaceDeclaration(ast::NamespaceDeclaration* namespaceDecl) {
    if (!namespaceDecl) {
        std::cerr << "Error: Namespace declaration is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze namespace name
        if (!analyzeNamespaceName(namespaceDecl->getName())) {
            std::cerr << "Error: Failed to analyze namespace name: " << namespaceDecl->getName() << std::endl;
            return false;
        }
        
        // Check for namespace conflicts
        if (hasNamespaceConflicts(namespaceDecl)) {
            std::cerr << "Error: Namespace conflicts detected: " << namespaceDecl->getName() << std::endl;
            return false;
        }
        
        // Enter namespace scope
        scopeManager_.enterScope();
        
        // Analyze namespace body
        for (const auto& statement : namespaceDecl->getBody()) {
            if (!analyzeNamespaceStatement(statement.get())) {
                std::cerr << "Error: Failed to analyze statement in namespace: " << namespaceDecl->getName() << std::endl;
                scopeManager_.exitScope();
                return false;
            }
        }
        
        // Exit namespace scope
        scopeManager_.exitScope();
        
        // Register namespace in symbol table
        registerNamespace(namespaceDecl);
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing namespace declaration: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeModuleResolution(ast::ModuleResolution* moduleResolution) {
    if (!moduleResolution) {
        std::cerr << "Error: Module resolution is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze module path
        if (!analyzeModulePath(moduleResolution->getModulePath())) {
            std::cerr << "Error: Failed to analyze module path: " << moduleResolution->getModulePath() << std::endl;
            return false;
        }
        
        // Analyze resolved path
        if (moduleResolution->isModuleResolved()) {
            if (!analyzeModulePath(moduleResolution->getResolvedPath())) {
                std::cerr << "Error: Failed to analyze resolved path: " << moduleResolution->getResolvedPath() << std::endl;
                return false;
            }
        }
        
        // Analyze dependencies
        for (const auto& dependency : moduleResolution->getDependencies()) {
            if (!analyzeModulePath(dependency)) {
                std::cerr << "Error: Failed to analyze dependency: " << dependency << std::endl;
                return false;
            }
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing module resolution: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeCircularDependency(ast::CircularDependency* circularDep) {
    if (!circularDep) {
        std::cerr << "Error: Circular dependency is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze module names
        if (!analyzeModulePath(circularDep->getModuleA())) {
            std::cerr << "Error: Failed to analyze module A: " << circularDep->getModuleA() << std::endl;
            return false;
        }
        
        if (!analyzeModulePath(circularDep->getModuleB())) {
            std::cerr << "Error: Failed to analyze module B: " << circularDep->getModuleB() << std::endl;
            return false;
        }
        
        // Analyze dependency chain
        if (circularDep->hasChain()) {
            for (const auto& module : circularDep->getDependencyChain()) {
                if (!analyzeModulePath(module)) {
                    std::cerr << "Error: Failed to analyze module in chain: " << module << std::endl;
                    return false;
                }
            }
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing circular dependency: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeUnionType(ast::UnionType* unionType) {
    if (!unionType) {
        std::cerr << "Error: Union type is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze each type in the union
        for (const auto& type : unionType->getTypes()) {
            if (!analyzeType(type.get())) {
                std::cerr << "Error: Failed to analyze type in union" << std::endl;
                return false;
            }
        }
        
        // Check for duplicate types
        if (hasDuplicateTypes(unionType)) {
            std::cerr << "Error: Duplicate types in union" << std::endl;
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing union type: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeIntersectionType(ast::IntersectionType* intersectionType) {
    if (!intersectionType) {
        std::cerr << "Error: Intersection type is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze each type in the intersection
        for (const auto& type : intersectionType->getTypes()) {
            if (!analyzeType(type.get())) {
                std::cerr << "Error: Failed to analyze type in intersection" << std::endl;
                return false;
            }
        }
        
        // Check for duplicate types
        if (hasDuplicateTypes(intersectionType)) {
            std::cerr << "Error: Duplicate types in intersection" << std::endl;
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing intersection type: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeConditionalType(ast::ConditionalType* conditionalType) {
    if (!conditionalType) {
        std::cerr << "Error: Conditional type is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze check type
        if (!analyzeType(conditionalType->getCheckType())) {
            std::cerr << "Error: Failed to analyze check type in conditional type" << std::endl;
            return false;
        }
        
        // Analyze extends type
        if (!analyzeType(conditionalType->getExtendsType())) {
            std::cerr << "Error: Failed to analyze extends type in conditional type" << std::endl;
            return false;
        }
        
        // Analyze true type
        if (!analyzeType(conditionalType->getTrueType())) {
            std::cerr << "Error: Failed to analyze true type in conditional type" << std::endl;
            return false;
        }
        
        // Analyze false type
        if (!analyzeType(conditionalType->getFalseType())) {
            std::cerr << "Error: Failed to analyze false type in conditional type" << std::endl;
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing conditional type: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeMappedType(ast::MappedType* mappedType) {
    if (!mappedType) {
        std::cerr << "Error: Mapped type is null" << std::endl;
        return false;
    }
    
    try {
        // Analyze key name
        if (!analyzeIdentifier(mappedType->getKeyName())) {
            std::cerr << "Error: Failed to analyze key name in mapped type: " << mappedType->getKeyName() << std::endl;
            return false;
        }
        
        // Analyze key type
        if (!analyzeType(mappedType->getKeyType())) {
            std::cerr << "Error: Failed to analyze key type in mapped type" << std::endl;
            return false;
        }
        
        // Analyze value type
        if (!analyzeType(mappedType->getValueType())) {
            std::cerr << "Error: Failed to analyze value type in mapped type" << std::endl;
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error analyzing mapped type: " << e.what() << std::endl;
        return false;
    }
}

// Private helper methods

bool ModuleSystemSemanticAnalyzer::analyzeModulePath(const std::string& path) {
    if (path.empty()) {
        std::cerr << "Error: Module path is empty" << std::endl;
        return false;
    }
    
    // Check for valid module path format
    if (!isValidModulePath(path)) {
        std::cerr << "Error: Invalid module path format: " << path << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeDefaultImport(const std::string& import) {
    if (import.empty()) {
        std::cerr << "Error: Default import is empty" << std::endl;
        return false;
    }
    
    if (!isValidIdentifier(import)) {
        std::cerr << "Error: Invalid default import identifier: " << import << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeNamedImport(const std::string& import) {
    if (import.empty()) {
        std::cerr << "Error: Named import is empty" << std::endl;
        return false;
    }
    
    if (!isValidIdentifier(import)) {
        std::cerr << "Error: Invalid named import identifier: " << import << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeNamespaceImport(const std::string& import) {
    if (import.empty()) {
        std::cerr << "Error: Namespace import is empty" << std::endl;
        return false;
    }
    
    if (!isValidIdentifier(import)) {
        std::cerr << "Error: Invalid namespace import identifier: " << import << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeNamespaceName(const std::string& name) {
    if (name.empty()) {
        std::cerr << "Error: Namespace name is empty" << std::endl;
        return false;
    }
    
    if (!isValidIdentifier(name)) {
        std::cerr << "Error: Invalid namespace name: " << name << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeIdentifier(const std::string& identifier) {
    if (identifier.empty()) {
        std::cerr << "Error: Identifier is empty" << std::endl;
        return false;
    }
    
    if (!isValidIdentifier(identifier)) {
        std::cerr << "Error: Invalid identifier: " << identifier << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeType(ast::Type* type) {
    if (!type) {
        std::cerr << "Error: Type is null" << std::endl;
        return false;
    }
    
    // Analyze type based on its type
    switch (type->getType()) {
        case ast::ASTNodeType::UnionType:
            return analyzeUnionType(static_cast<ast::UnionType*>(type));
        case ast::ASTNodeType::IntersectionType:
            return analyzeIntersectionType(static_cast<ast::IntersectionType*>(type));
        case ast::ASTNodeType::ConditionalType:
            return analyzeConditionalType(static_cast<ast::ConditionalType*>(type));
        case ast::ASTNodeType::MappedType:
            return analyzeMappedType(static_cast<ast::MappedType*>(type));
        default:
            // For other types, just check if they're valid
            return true;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeNamedExport(ast::ExportDeclaration* exportDecl) {
    // Analyze named exports
    for (const auto& namedExport : exportDecl->getNamedExports()) {
        if (!analyzeIdentifier(namedExport)) {
            std::cerr << "Error: Invalid named export identifier: " << namedExport << std::endl;
            return false;
        }
        
        // Check if symbol exists
        if (!symbolTable_.hasSymbol(namedExport)) {
            std::cerr << "Error: Named export symbol not found: " << namedExport << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeDefaultExport(ast::ExportDeclaration* exportDecl) {
    // Analyze default export
    if (!exportDecl->hasDefaultExport()) {
        std::cerr << "Error: Default export declaration missing default export" << std::endl;
        return false;
    }
    
    // Analyze the expression
    if (!analyzeExpression(exportDecl->getDefaultExport())) {
        std::cerr << "Error: Failed to analyze default export expression" << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeReExport(ast::ExportDeclaration* exportDecl) {
    // Analyze re-export module
    if (exportDecl->getReExportModule().empty()) {
        std::cerr << "Error: Re-export declaration missing module path" << std::endl;
        return false;
    }
    
    if (!analyzeModulePath(exportDecl->getReExportModule())) {
        std::cerr << "Error: Failed to analyze re-export module path: " << exportDecl->getReExportModule() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeNamespaceStatement(ast::Statement* statement) {
    if (!statement) {
        std::cerr << "Error: Namespace statement is null" << std::endl;
        return false;
    }
    
    // Analyze statement based on its type
    switch (statement->getType()) {
        case ast::ASTNodeType::VariableDeclaration:
            return analyzeVariableDeclaration(static_cast<ast::VariableDeclaration*>(statement));
        case ast::ASTNodeType::FunctionDeclaration:
            return analyzeFunctionDeclaration(static_cast<ast::FunctionDeclaration*>(statement));
        case ast::ASTNodeType::ClassDeclaration:
            return analyzeClassDeclaration(static_cast<ast::ClassDeclaration*>(statement));
        case ast::ASTNodeType::InterfaceDeclaration:
            return analyzeInterfaceDeclaration(static_cast<ast::InterfaceDeclaration*>(statement));
        case ast::ASTNodeType::TypeAliasDeclaration:
            return analyzeTypeAliasDeclaration(static_cast<ast::TypeAliasDeclaration*>(statement));
        case ast::ASTNodeType::NamespaceDeclaration:
            return analyzeNamespaceDeclaration(static_cast<ast::NamespaceDeclaration*>(statement));
        default:
            std::cerr << "Error: Invalid statement type in namespace" << std::endl;
            return false;
    }
}

bool ModuleSystemSemanticAnalyzer::analyzeExpression(ast::Expression* expression) {
    if (!expression) {
        std::cerr << "Error: Expression is null" << std::endl;
        return false;
    }
    
    // For now, just check if expression is valid
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeVariableDeclaration(ast::VariableDeclaration* varDecl) {
    if (!varDecl) {
        std::cerr << "Error: Variable declaration is null" << std::endl;
        return false;
    }
    
    // Analyze variable name
    if (!analyzeIdentifier(varDecl->getName())) {
        std::cerr << "Error: Invalid variable name: " << varDecl->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeFunctionDeclaration(ast::FunctionDeclaration* funcDecl) {
    if (!funcDecl) {
        std::cerr << "Error: Function declaration is null" << std::endl;
        return false;
    }
    
    // Analyze function name
    if (!analyzeIdentifier(funcDecl->getName())) {
        std::cerr << "Error: Invalid function name: " << funcDecl->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeClassDeclaration(ast::ClassDeclaration* classDecl) {
    if (!classDecl) {
        std::cerr << "Error: Class declaration is null" << std::endl;
        return false;
    }
    
    // Analyze class name
    if (!analyzeIdentifier(classDecl->getName())) {
        std::cerr << "Error: Invalid class name: " << classDecl->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeInterfaceDeclaration(ast::InterfaceDeclaration* interfaceDecl) {
    if (!interfaceDecl) {
        std::cerr << "Error: Interface declaration is null" << std::endl;
        return false;
    }
    
    // Analyze interface name
    if (!analyzeIdentifier(interfaceDecl->getName())) {
        std::cerr << "Error: Invalid interface name: " << interfaceDecl->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::analyzeTypeAliasDeclaration(ast::TypeAliasDeclaration* typeAliasDecl) {
    if (!typeAliasDecl) {
        std::cerr << "Error: Type alias declaration is null" << std::endl;
        return false;
    }
    
    // Analyze type alias name
    if (!analyzeIdentifier(typeAliasDecl->getName())) {
        std::cerr << "Error: Invalid type alias name: " << typeAliasDecl->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemSemanticAnalyzer::hasImportConflicts(ast::ImportDeclaration* importDecl) {
    // Check for conflicts between default and named imports
    if (importDecl->hasDefaultImport() && importDecl->hasNamedImports()) {
        for (const auto& namedImport : importDecl->getNamedImports()) {
            if (namedImport == importDecl->getDefaultImport()) {
                return true;
            }
        }
    }
    
    // Check for conflicts between default and namespace imports
    if (importDecl->hasDefaultImport() && importDecl->hasNamespaceImport()) {
        if (importDecl->getDefaultImport() == importDecl->getNamespaceImport()) {
            return true;
        }
    }
    
    // Check for conflicts between named and namespace imports
    if (importDecl->hasNamedImports() && importDecl->hasNamespaceImport()) {
        for (const auto& namedImport : importDecl->getNamedImports()) {
            if (namedImport == importDecl->getNamespaceImport()) {
                return true;
            }
        }
    }
    
    return false;
}

bool ModuleSystemSemanticAnalyzer::hasNamespaceConflicts(ast::NamespaceDeclaration* namespaceDecl) {
    // Check if namespace name conflicts with existing symbols
    return symbolTable_.hasSymbol(namespaceDecl->getName());
}

bool ModuleSystemSemanticAnalyzer::hasDuplicateTypes(ast::UnionType* unionType) {
    // Check for duplicate types in union
    const auto& types = unionType->getTypes();
    for (size_t i = 0; i < types.size(); ++i) {
        for (size_t j = i + 1; j < types.size(); ++j) {
            if (types[i]->getType() == types[j]->getType()) {
                return true;
            }
        }
    }
    return false;
}

bool ModuleSystemSemanticAnalyzer::hasDuplicateTypes(ast::IntersectionType* intersectionType) {
    // Check for duplicate types in intersection
    const auto& types = intersectionType->getTypes();
    for (size_t i = 0; i < types.size(); ++i) {
        for (size_t j = i + 1; j < types.size(); ++j) {
            if (types[i]->getType() == types[j]->getType()) {
                return true;
            }
        }
    }
    return false;
}

bool ModuleSystemSemanticAnalyzer::isValidModulePath(const std::string& path) {
    if (path.empty()) return false;
    
    // Check for relative paths
    if (path.starts_with("./") || path.starts_with("../")) {
        return true;
    }
    
    // Check for absolute paths
    if (path.starts_with("/")) {
        return true;
    }
    
    // Check for npm package names
    if (path.find('/') == std::string::npos) {
        return true;
    }
    
    // Check for scoped packages
    if (path.starts_with("@") && path.find('/') != std::string::npos) {
        return true;
    }
    
    return false;
}

bool ModuleSystemSemanticAnalyzer::isValidIdentifier(const std::string& identifier) {
    if (identifier.empty()) return false;
    
    // Check first character
    if (!std::isalpha(identifier[0]) && identifier[0] != '_' && identifier[0] != '$') {
        return false;
    }
    
    // Check remaining characters
    for (size_t i = 1; i < identifier.length(); ++i) {
        if (!std::isalnum(identifier[i]) && identifier[i] != '_' && identifier[i] != '$') {
            return false;
        }
    }
    
    return true;
}

void ModuleSystemSemanticAnalyzer::registerImports(ast::ImportDeclaration* importDecl) {
    // Register default import
    if (importDecl->hasDefaultImport()) {
        symbolTable_.addSymbol(importDecl->getDefaultImport(), nullptr);
    }
    
    // Register named imports
    for (const auto& namedImport : importDecl->getNamedImports()) {
        symbolTable_.addSymbol(namedImport, nullptr);
    }
    
    // Register namespace import
    if (importDecl->hasNamespaceImport()) {
        symbolTable_.addSymbol(importDecl->getNamespaceImport(), nullptr);
    }
}

void ModuleSystemSemanticAnalyzer::registerNamespace(ast::NamespaceDeclaration* namespaceDecl) {
    // Register namespace in symbol table
    symbolTable_.addSymbol(namespaceDecl->getName(), namespaceDecl);
}

} // namespace semantic
} // namespace tsc