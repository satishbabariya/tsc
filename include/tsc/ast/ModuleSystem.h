#pragma once

#include "ASTNode.h"
#include "Expression.h"
#include "Statement.h"
#include "Type.h"
#include <string>
#include <vector>
#include <memory>

namespace tsc {
namespace ast {

// Forward declarations
class ModuleDeclaration;
class ImportDeclaration;
class ExportDeclaration;
class NamespaceDeclaration;
class ModuleResolution;

// Module System AST Nodes

/**
 * Base class for all module-related AST nodes
 */
class ModuleNode : public ASTNode {
public:
    ModuleNode(ASTNodeType type, SourceLocation location);
    virtual ~ModuleNode() = default;
};

/**
 * Represents an ES6 import declaration
 * import { named1, named2 } from 'module';
 * import defaultExport from 'module';
 * import * as namespace from 'module';
 */
class ImportDeclaration : public ModuleNode {
private:
    std::string modulePath;
    std::string defaultImport;
    std::vector<std::string> namedImports;
    std::string namespaceImport;
    bool isTypeOnly;
    
public:
    ImportDeclaration(SourceLocation location);
    virtual ~ImportDeclaration() = default;
    
    // Getters
    const std::string& getModulePath() const { return modulePath; }
    const std::string& getDefaultImport() const { return defaultImport; }
    const std::vector<std::string>& getNamedImports() const { return namedImports; }
    const std::string& getNamespaceImport() const { return namespaceImport; }
    bool isTypeOnlyImport() const { return isTypeOnly; }
    
    // Setters
    void setModulePath(const std::string& path) { modulePath = path; }
    void setDefaultImport(const std::string& import) { defaultImport = import; }
    void addNamedImport(const std::string& import) { namedImports.push_back(import); }
    void setNamespaceImport(const std::string& import) { namespaceImport = import; }
    void setTypeOnly(bool typeOnly) { isTypeOnly = typeOnly; }
    
    // Utility methods
    bool hasDefaultImport() const { return !defaultImport.empty(); }
    bool hasNamedImports() const { return !namedImports.empty(); }
    bool hasNamespaceImport() const { return !namespaceImport.empty(); }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents an ES6 export declaration
 * export { name1, name2 };
 * export default expression;
 * export * from 'module';
 */
class ExportDeclaration : public ModuleNode {
public:
    enum class ExportType {
        Named,
        Default,
        All
    };
    
private:
    ExportType exportType;
    std::vector<std::string> namedExports;
    std::unique_ptr<Expression> defaultExport;
    std::string reExportModule;
    bool isTypeOnly;
    
public:
    ExportDeclaration(SourceLocation location);
    virtual ~ExportDeclaration() = default;
    
    // Getters
    ExportType getExportType() const { return exportType; }
    const std::vector<std::string>& getNamedExports() const { return namedExports; }
    Expression* getDefaultExport() const { return defaultExport.get(); }
    const std::string& getReExportModule() const { return reExportModule; }
    bool isTypeOnlyExport() const { return isTypeOnly; }
    
    // Setters
    void setExportType(ExportType type) { exportType = type; }
    void addNamedExport(const std::string& export) { namedExports.push_back(export); }
    void setDefaultExport(std::unique_ptr<Expression> export) { defaultExport = std::move(export); }
    void setReExportModule(const std::string& module) { reExportModule = module; }
    void setTypeOnly(bool typeOnly) { isTypeOnly = typeOnly; }
    
    // Utility methods
    bool hasNamedExports() const { return !namedExports.empty(); }
    bool hasDefaultExport() const { return defaultExport != nullptr; }
    bool isReExport() const { return !reExportModule.empty(); }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents a namespace declaration
 * namespace MyNamespace { ... }
 */
class NamespaceDeclaration : public ModuleNode {
private:
    std::string name;
    std::vector<std::unique_ptr<Statement>> body;
    bool isGlobal;
    
public:
    NamespaceDeclaration(SourceLocation location);
    virtual ~NamespaceDeclaration() = default;
    
    // Getters
    const std::string& getName() const { return name; }
    const std::vector<std::unique_ptr<Statement>>& getBody() const { return body; }
    bool isGlobalNamespace() const { return isGlobal; }
    
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void addStatement(std::unique_ptr<Statement> statement) { body.push_back(std::move(statement)); }
    void setGlobal(bool global) { isGlobal = global; }
    
    // Utility methods
    bool isEmpty() const { return body.empty(); }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents module resolution information
 */
class ModuleResolution {
private:
    std::string modulePath;
    std::string resolvedPath;
    bool isResolved;
    std::vector<std::string> dependencies;
    
public:
    ModuleResolution(const std::string& path);
    virtual ~ModuleResolution() = default;
    
    // Getters
    const std::string& getModulePath() const { return modulePath; }
    const std::string& getResolvedPath() const { return resolvedPath; }
    bool isModuleResolved() const { return isResolved; }
    const std::vector<std::string>& getDependencies() const { return dependencies; }
    
    // Setters
    void setResolvedPath(const std::string& path) { resolvedPath = path; isResolved = true; }
    void addDependency(const std::string& dependency) { dependencies.push_back(dependency); }
    
    // Utility methods
    bool hasDependencies() const { return !dependencies.empty(); }
};

/**
 * Represents circular dependency information
 */
class CircularDependency {
private:
    std::string moduleA;
    std::string moduleB;
    std::vector<std::string> dependencyChain;
    
public:
    CircularDependency(const std::string& moduleA, const std::string& moduleB);
    virtual ~CircularDependency() = default;
    
    // Getters
    const std::string& getModuleA() const { return moduleA; }
    const std::string& getModuleB() const { return moduleB; }
    const std::vector<std::string>& getDependencyChain() const { return dependencyChain; }
    
    // Setters
    void setDependencyChain(const std::vector<std::string>& chain) { dependencyChain = chain; }
    
    // Utility methods
    bool hasChain() const { return !dependencyChain.empty(); }
};

} // namespace ast
} // namespace tsc