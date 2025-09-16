#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"

#include <vector>
#include <unordered_map>

namespace tsc {
namespace semantic {

// Forward declarations
class SemanticContext;

// Inheritance resolution engine for handling class and interface inheritance
class InheritanceResolver {
public:
    explicit InheritanceResolver(SemanticContext& context);
    
    // Main inheritance resolution interface
    void resolveInheritance(Module& module);
    
    // Class inheritance
    void resolveClassInheritance(const ClassDeclaration& classDecl);
    void resolveClassHierarchy(const ClassDeclaration& classDecl);
    void checkInheritanceCycles(const ClassDeclaration& classDecl);
    
    // Interface inheritance
    void resolveInterfaceInheritance(const InterfaceDeclaration& interfaceDecl);
    void resolveInterfaceHierarchy(const InterfaceDeclaration& interfaceDecl);
    void checkInterfaceCycles(const InterfaceDeclaration& interfaceDecl);
    
    // Method resolution
    void resolveMethodOverrides(const ClassDeclaration& classDecl);
    void resolveMethodImplementations(const ClassDeclaration& classDecl);
    void checkOverrideCompatibility(const MethodDeclaration& method, const MethodDeclaration& baseMethod);
    
    // Property resolution
    void resolvePropertyOverrides(const ClassDeclaration& classDecl);
    void resolvePropertyImplementations(const ClassDeclaration& classDecl);
    void checkPropertyCompatibility(const PropertyDeclaration& property, const PropertyDeclaration& baseProperty);
    
    // Generic inheritance
    void resolveGenericInheritance(const ClassDeclaration& classDecl);
    void resolveGenericConstraints(const ClassDeclaration& classDecl);
    void checkGenericCompatibility(const ClassDeclaration& classDecl, const ClassDeclaration& baseClass);
    
    // Helper methods
    Symbol* findInterfaceMember(const InterfaceType& interfaceType, const String& memberName);
    std::vector<Symbol*> findBaseMembers(const ClassDeclaration& classDecl, const String& memberName);
    bool isCompatibleSignature(const FunctionType& derived, const FunctionType& base);

private:
    SemanticContext& context_;
    
    // Inheritance tracking
    std::unordered_map<const ClassDeclaration*, std::vector<const ClassDeclaration*>> classHierarchy_;
    std::unordered_map<const InterfaceDeclaration*, std::vector<const InterfaceDeclaration*>> interfaceHierarchy_;
    
    // Helper methods
    void buildClassHierarchy(const ClassDeclaration& classDecl);
    void buildInterfaceHierarchy(const InterfaceDeclaration& interfaceDecl);
    void validateInheritanceChain(const ClassDeclaration& classDecl);
    void validateInterfaceChain(const InterfaceDeclaration& interfaceDecl);
};

} // namespace semantic
} // namespace tsc