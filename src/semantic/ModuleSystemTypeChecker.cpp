#include "tsc/semantic/ModuleSystemTypeChecker.h"
#include "tsc/ast/ModuleSystem.h"
#include "tsc/ast/AdvancedTypes.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeChecker.h"
#include <iostream>
#include <algorithm>

namespace tsc {
namespace semantic {

ModuleSystemTypeChecker::ModuleSystemTypeChecker(SymbolTable& symbolTable, TypeChecker& typeChecker)
    : symbolTable_(symbolTable), typeChecker_(typeChecker) {
}

bool ModuleSystemTypeChecker::checkImportDeclaration(ast::ImportDeclaration* importDecl) {
    if (!importDecl) {
        std::cerr << "Error: Import declaration is null" << std::endl;
        return false;
    }
    
    try {
        // Check module path
        if (importDecl->getModulePath().empty()) {
            std::cerr << "Error: Import declaration has empty module path" << std::endl;
            return false;
        }
        
        // Validate module path format
        if (!isValidModulePath(importDecl->getModulePath())) {
            std::cerr << "Error: Invalid module path format: " << importDecl->getModulePath() << std::endl;
            return false;
        }
        
        // Check default import
        if (importDecl->hasDefaultImport()) {
            if (!isValidIdentifier(importDecl->getDefaultImport())) {
                std::cerr << "Error: Invalid default import identifier: " << importDecl->getDefaultImport() << std::endl;
                return false;
            }
        }
        
        // Check named imports
        for (const auto& namedImport : importDecl->getNamedImports()) {
            if (!isValidIdentifier(namedImport)) {
                std::cerr << "Error: Invalid named import identifier: " << namedImport << std::endl;
                return false;
            }
        }
        
        // Check namespace import
        if (importDecl->hasNamespaceImport()) {
            if (!isValidIdentifier(importDecl->getNamespaceImport())) {
                std::cerr << "Error: Invalid namespace import identifier: " << importDecl->getNamespaceImport() << std::endl;
                return false;
            }
        }
        
        // Check for conflicts
        if (hasImportConflicts(importDecl)) {
            std::cerr << "Error: Import conflicts detected" << std::endl;
            return false;
        }
        
        // Validate static compilation constraints
        if (!validateStaticCompilationConstraints(importDecl)) {
            std::cerr << "Error: Static compilation constraints violated" << std::endl;
            return false;
        }
        
        // Register imports in symbol table
        registerImports(importDecl);
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error checking import declaration: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkExportDeclaration(ast::ExportDeclaration* exportDecl) {
    if (!exportDecl) {
        std::cerr << "Error: Export declaration is null" << std::endl;
        return false;
    }
    
    try {
        // Check export type
        switch (exportDecl->getExportType()) {
            case ast::ExportDeclaration::ExportType::Named:
                return checkNamedExport(exportDecl);
            case ast::ExportDeclaration::ExportType::Default:
                return checkDefaultExport(exportDecl);
            case ast::ExportDeclaration::ExportType::All:
                return checkReExport(exportDecl);
            default:
                std::cerr << "Error: Unknown export type" << std::endl;
                return false;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error checking export declaration: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkNamespaceDeclaration(ast::NamespaceDeclaration* namespaceDecl) {
    if (!namespaceDecl) {
        std::cerr << "Error: Namespace declaration is null" << std::endl;
        return false;
    }
    
    try {
        // Check namespace name
        if (namespaceDecl->getName().empty()) {
            std::cerr << "Error: Namespace declaration has empty name" << std::endl;
            return false;
        }
        
        if (!isValidIdentifier(namespaceDecl->getName())) {
            std::cerr << "Error: Invalid namespace name: " << namespaceDecl->getName() << std::endl;
            return false;
        }
        
        // Check for namespace conflicts
        if (hasNamespaceConflicts(namespaceDecl)) {
            std::cerr << "Error: Namespace conflicts detected: " << namespaceDecl->getName() << std::endl;
            return false;
        }
        
        // Check namespace body
        for (const auto& statement : namespaceDecl->getBody()) {
            if (!checkNamespaceStatement(statement.get())) {
                std::cerr << "Error: Invalid statement in namespace: " << namespaceDecl->getName() << std::endl;
                return false;
            }
        }
        
        // Register namespace in symbol table
        registerNamespace(namespaceDecl);
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error checking namespace declaration: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkModuleResolution(ast::ModuleResolution* moduleResolution) {
    if (!moduleResolution) {
        std::cerr << "Error: Module resolution is null" << std::endl;
        return false;
    }
    
    try {
        // Check module path
        if (moduleResolution->getModulePath().empty()) {
            std::cerr << "Error: Module resolution has empty module path" << std::endl;
            return false;
        }
        
        // Validate module path format
        if (!isValidModulePath(moduleResolution->getModulePath())) {
            std::cerr << "Error: Invalid module path format: " << moduleResolution->getModulePath() << std::endl;
            return false;
        }
        
        // Check if module is resolved
        if (!moduleResolution->isModuleResolved()) {
            std::cerr << "Warning: Module not resolved: " << moduleResolution->getModulePath() << std::endl;
        }
        
        // Check dependencies
        for (const auto& dependency : moduleResolution->getDependencies()) {
            if (!isValidModulePath(dependency)) {
                std::cerr << "Error: Invalid dependency path: " << dependency << std::endl;
                return false;
            }
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error checking module resolution: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkCircularDependency(ast::CircularDependency* circularDep) {
    if (!circularDependency) {
        std::cerr << "Error: Circular dependency is null" << std::endl;
        return false;
    }
    
    try {
        // Check module names
        if (circularDep->getModuleA().empty() || circularDep->getModuleB().empty()) {
            std::cerr << "Error: Circular dependency has empty module names" << std::endl;
            return false;
        }
        
        // Check if modules are the same
        if (circularDep->getModuleA() == circularDep->getModuleB()) {
            std::cerr << "Error: Circular dependency with same module: " << circularDep->getModuleA() << std::endl;
            return false;
        }
        
        // Check dependency chain
        if (circularDep->hasChain()) {
            for (const auto& module : circularDep->getDependencyChain()) {
                if (module.empty()) {
                    std::cerr << "Error: Empty module in dependency chain" << std::endl;
                    return false;
                }
                
                if (!isValidModulePath(module)) {
                    std::cerr << "Error: Invalid module path in dependency chain: " << module << std::endl;
                    return false;
                }
            }
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error checking circular dependency: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkUnionType(ast::UnionType* unionType) {
    if (!unionType) {
        std::cerr << "Error: Union type is null" << std::endl;
        return false;
    }
    
    try {
        // Check if union type is empty
        if (unionType->isEmpty()) {
            std::cerr << "Error: Union type is empty" << std::endl;
            return false;
        }
        
        // Check each type in the union
        for (const auto& type : unionType->getTypes()) {
            if (!type) {
                std::cerr << "Error: Union type contains null type" << std::endl;
                return false;
            }
            
            if (!typeChecker_.checkType(type.get())) {
                std::cerr << "Error: Invalid type in union" << std::endl;
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
        std::cerr << "Error checking union type: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkIntersectionType(ast::IntersectionType* intersectionType) {
    if (!intersectionType) {
        std::cerr << "Error: Intersection type is null" << std::endl;
        return false;
    }
    
    try {
        // Check if intersection type is empty
        if (intersectionType->isEmpty()) {
            std::cerr << "Error: Intersection type is empty" << std::endl;
            return false;
        }
        
        // Check each type in the intersection
        for (const auto& type : intersectionType->getTypes()) {
            if (!type) {
                std::cerr << "Error: Intersection type contains null type" << std::endl;
                return false;
            }
            
            if (!typeChecker_.checkType(type.get())) {
                std::cerr << "Error: Invalid type in intersection" << std::endl;
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
        std::cerr << "Error checking intersection type: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkConditionalType(ast::ConditionalType* conditionalType) {
    if (!conditionalType) {
        std::cerr << "Error: Conditional type is null" << std::endl;
        return false;
    }
    
    try {
        // Check check type
        if (!conditionalType->hasCheckType()) {
            std::cerr << "Error: Conditional type missing check type" << std::endl;
            return false;
        }
        
        if (!typeChecker_.checkType(conditionalType->getCheckType())) {
            std::cerr << "Error: Invalid check type in conditional type" << std::endl;
            return false;
        }
        
        // Check extends type
        if (!conditionalType->hasExtendsType()) {
            std::cerr << "Error: Conditional type missing extends type" << std::endl;
            return false;
        }
        
        if (!typeChecker_.checkType(conditionalType->getExtendsType())) {
            std::cerr << "Error: Invalid extends type in conditional type" << std::endl;
            return false;
        }
        
        // Check true type
        if (!conditionalType->hasTrueType()) {
            std::cerr << "Error: Conditional type missing true type" << std::endl;
            return false;
        }
        
        if (!typeChecker_.checkType(conditionalType->getTrueType())) {
            std::cerr << "Error: Invalid true type in conditional type" << std::endl;
            return false;
        }
        
        // Check false type
        if (!conditionalType->hasFalseType()) {
            std::cerr << "Error: Conditional type missing false type" << std::endl;
            return false;
        }
        
        if (!typeChecker_.checkType(conditionalType->getFalseType())) {
            std::cerr << "Error: Invalid false type in conditional type" << std::endl;
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error checking conditional type: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleSystemTypeChecker::checkMappedType(ast::MappedType* mappedType) {
    if (!mappedType) {
        std::cerr << "Error: Mapped type is null" << std::endl;
        return false;
    }
    
    try {
        // Check key name
        if (mappedType->getKeyName().empty()) {
            std::cerr << "Error: Mapped type has empty key name" << std::endl;
            return false;
        }
        
        if (!isValidIdentifier(mappedType->getKeyName())) {
            std::cerr << "Error: Invalid key name in mapped type: " << mappedType->getKeyName() << std::endl;
            return false;
        }
        
        // Check key type
        if (!mappedType->hasKeyType()) {
            std::cerr << "Error: Mapped type missing key type" << std::endl;
            return false;
        }
        
        if (!typeChecker_.checkType(mappedType->getKeyType())) {
            std::cerr << "Error: Invalid key type in mapped type" << std::endl;
            return false;
        }
        
        // Check value type
        if (!mappedType->hasValueType()) {
            std::cerr << "Error: Mapped type missing value type" << std::endl;
            return false;
        }
        
        if (!typeChecker_.checkType(mappedType->getValueType())) {
            std::cerr << "Error: Invalid value type in mapped type" << std::endl;
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error checking mapped type: " << e.what() << std::endl;
        return false;
    }
}

// Private helper methods

bool ModuleSystemTypeChecker::isValidModulePath(const std::string& path) {
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

bool ModuleSystemTypeChecker::isValidIdentifier(const std::string& identifier) {
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

bool ModuleSystemTypeChecker::hasImportConflicts(ast::ImportDeclaration* importDecl) {
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

bool ModuleSystemTypeChecker::hasNamespaceConflicts(ast::NamespaceDeclaration* namespaceDecl) {
    // Check if namespace name conflicts with existing symbols
    return symbolTable_.hasSymbol(namespaceDecl->getName());
}

bool ModuleSystemTypeChecker::hasDuplicateTypes(ast::UnionType* unionType) {
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

bool ModuleSystemTypeChecker::hasDuplicateTypes(ast::IntersectionType* intersectionType) {
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

bool ModuleSystemTypeChecker::checkNamedExport(ast::ExportDeclaration* exportDecl) {
    // Check named exports
    for (const auto& namedExport : exportDecl->getNamedExports()) {
        if (!isValidIdentifier(namedExport)) {
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

bool ModuleSystemTypeChecker::checkDefaultExport(ast::ExportDeclaration* exportDecl) {
    // Check default export
    if (!exportDecl->hasDefaultExport()) {
        std::cerr << "Error: Default export declaration missing default export" << std::endl;
        return false;
    }
    
    if (!typeChecker_.checkExpression(exportDecl->getDefaultExport())) {
        std::cerr << "Error: Invalid default export expression" << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemTypeChecker::checkReExport(ast::ExportDeclaration* exportDecl) {
    // Check re-export module
    if (exportDecl->getReExportModule().empty()) {
        std::cerr << "Error: Re-export declaration missing module path" << std::endl;
        return false;
    }
    
    if (!isValidModulePath(exportDecl->getReExportModule())) {
        std::cerr << "Error: Invalid re-export module path: " << exportDecl->getReExportModule() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemTypeChecker::checkNamespaceStatement(ast::Statement* statement) {
    if (!statement) {
        std::cerr << "Error: Namespace statement is null" << std::endl;
        return false;
    }
    
    // Check statement type
    switch (statement->getType()) {
        case ast::ASTNodeType::VariableDeclaration:
        case ast::ASTNodeType::FunctionDeclaration:
        case ast::ASTNodeType::ClassDeclaration:
        case ast::ASTNodeType::InterfaceDeclaration:
        case ast::ASTNodeType::TypeAliasDeclaration:
        case ast::ASTNodeType::NamespaceDeclaration:
            return true;
        default:
            std::cerr << "Error: Invalid statement type in namespace" << std::endl;
            return false;
    }
}

void ModuleSystemTypeChecker::registerImports(ast::ImportDeclaration* importDecl) {
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

void ModuleSystemTypeChecker::registerNamespace(ast::NamespaceDeclaration* namespaceDecl) {
    // Register namespace in symbol table
    symbolTable_.addSymbol(namespaceDecl->getName(), namespaceDecl);
}

bool ModuleSystemTypeChecker::validateStaticCompilationConstraints(ast::ImportDeclaration* importDecl) {
    // Validate that all imports are statically resolvable
    if (importDecl->getModulePath().empty()) {
        return false;
    }
    
    // Check for dynamic import patterns (not allowed in static compilation)
    if (importDecl->getModulePath().find("import(") != std::string::npos) {
        std::cerr << "Error: Dynamic imports not supported in static compilation" << std::endl;
        return false;
    }
    
    // Validate module path is statically resolvable
    if (!isStaticallyResolvable(importDecl->getModulePath())) {
        std::cerr << "Error: Module path not statically resolvable: " << importDecl->getModulePath() << std::endl;
        return false;
    }
    
    return true;
}

bool ModuleSystemTypeChecker::isStaticallyResolvable(const std::string& modulePath) {
    // Check if module path can be resolved at compile time
    if (modulePath.empty()) return false;
    
    // Relative paths are statically resolvable
    if (modulePath.starts_with("./") || modulePath.starts_with("../")) {
        return true;
    }
    
    // Absolute paths are statically resolvable
    if (modulePath.starts_with("/")) {
        return true;
    }
    
    // Package names are statically resolvable
    if (modulePath.find('/') == std::string::npos) {
        return true;
    }
    
    // Scoped packages are statically resolvable
    if (modulePath.starts_with("@") && modulePath.find('/') != std::string::npos) {
        return true;
    }
    
    return false;
}

} // namespace semantic
} // namespace tsc