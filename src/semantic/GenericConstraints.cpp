#include "tsc/semantic/GenericConstraints.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/ast/AST.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

namespace tsc {
namespace semantic {

// =============================================================================
// GENERIC CONSTRAINTS IMPLEMENTATION
// =============================================================================

GenericConstraintsAnalyzer::GenericConstraintsAnalyzer(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool GenericConstraintsAnalyzer::analyzeGenericConstraints(ASTNode* node) {
    if (!node) return true;
    
    switch (node->getType()) {
        case ASTNodeType::GenericMethodDeclaration:
            return analyzeGenericMethodConstraints(static_cast<GenericMethodDeclaration*>(node));
        case ASTNodeType::GenericClassDeclaration:
            return analyzeGenericClassConstraints(static_cast<GenericClassDeclaration*>(node));
        case ASTNodeType::GenericInterfaceDeclaration:
            return analyzeGenericInterfaceConstraints(static_cast<GenericInterfaceDeclaration*>(node));
        default:
            return true;
    }
}

bool GenericConstraintsAnalyzer::analyzeGenericMethodConstraints(GenericMethodDeclaration* method) {
    std::cout << "Analyzing generic method constraints for: " << method->getName() << std::endl;
    
    // Analyze type parameters and their constraints
    for (auto& typeParam : method->getTypeParameters()) {
        if (!analyzeTypeParameterConstraints(typeParam)) {
            return false;
        }
    }
    
    // Analyze method body for constraint violations
    if (method->getBody()) {
        if (!analyzeMethodBodyConstraints(method)) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeGenericClassConstraints(GenericClassDeclaration* classDecl) {
    std::cout << "Analyzing generic class constraints for: " << classDecl->getName() << std::endl;
    
    // Analyze type parameters and their constraints
    for (auto& typeParam : classDecl->getTypeParameters()) {
        if (!analyzeTypeParameterConstraints(typeParam)) {
            return false;
        }
    }
    
    // Analyze class members for constraint violations
    for (auto& member : classDecl->getMembers()) {
        if (!analyzeMemberConstraints(member, classDecl->getTypeParameters())) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeGenericInterfaceConstraints(GenericInterfaceDeclaration* interfaceDecl) {
    std::cout << "Analyzing generic interface constraints for: " << interfaceDecl->getName() << std::endl;
    
    // Analyze type parameters and their constraints
    for (auto& typeParam : interfaceDecl->getTypeParameters()) {
        if (!analyzeTypeParameterConstraints(typeParam)) {
            return false;
        }
    }
    
    // Analyze interface members for constraint violations
    for (auto& member : interfaceDecl->getMembers()) {
        if (!analyzeMemberConstraints(member, interfaceDecl->getTypeParameters())) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeTypeParameterConstraints(TypeParameter* typeParam) {
    std::cout << "Analyzing type parameter constraints for: " << typeParam->getName() << std::endl;
    
    // Check constraint validity
    for (auto& constraint : typeParam->getConstraints()) {
        if (!validateConstraint(constraint)) {
            std::cerr << "Invalid constraint for type parameter: " << typeParam->getName() << std::endl;
            return false;
        }
    }
    
    // Check constraint compatibility
    if (!checkConstraintCompatibility(typeParam->getConstraints())) {
        std::cerr << "Incompatible constraints for type parameter: " << typeParam->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::validateConstraint(Constraint* constraint) {
    if (!constraint) return false;
    
    switch (constraint->getType()) {
        case ConstraintType::Extends:
            return validateExtendsConstraint(constraint);
        case ConstraintType::Implements:
            return validateImplementsConstraint(constraint);
        case ConstraintType::Constructor:
            return validateConstructorConstraint(constraint);
        default:
            std::cerr << "Unknown constraint type" << std::endl;
            return false;
    }
}

bool GenericConstraintsAnalyzer::validateExtendsConstraint(Constraint* constraint) {
    auto extendsConstraint = static_cast<ExtendsConstraint*>(constraint);
    auto baseType = extendsConstraint->getBaseType();
    
    // Check if base type exists
    if (!analyzer_->getSymbolTable()->lookupType(baseType->getName())) {
        std::cerr << "Base type not found: " << baseType->getName() << std::endl;
        return false;
    }
    
    // Check if base type is a class or interface
    auto baseTypeInfo = analyzer_->getSymbolTable()->lookupType(baseType->getName());
    if (baseTypeInfo->getType() != TypeInfo::Class && 
        baseTypeInfo->getType() != TypeInfo::Interface) {
        std::cerr << "Base type must be a class or interface: " << baseType->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::validateImplementsConstraint(Constraint* constraint) {
    auto implementsConstraint = static_cast<ImplementsConstraint*>(constraint);
    auto interfaceType = implementsConstraint->getInterfaceType();
    
    // Check if interface type exists
    if (!analyzer_->getSymbolTable()->lookupType(interfaceType->getName())) {
        std::cerr << "Interface type not found: " << interfaceType->getName() << std::endl;
        return false;
    }
    
    // Check if interface type is actually an interface
    auto interfaceTypeInfo = analyzer_->getSymbolTable()->lookupType(interfaceType->getName());
    if (interfaceTypeInfo->getType() != TypeInfo::Interface) {
        std::cerr << "Type must be an interface: " << interfaceType->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::validateConstructorConstraint(Constraint* constraint) {
    auto constructorConstraint = static_cast<ConstructorConstraint*>(constraint);
    
    // Constructor constraint is always valid (new T())
    return true;
}

bool GenericConstraintsAnalyzer::checkConstraintCompatibility(const std::vector<Constraint*>& constraints) {
    // Check for conflicting constraints
    bool hasExtends = false;
    bool hasImplements = false;
    
    for (auto& constraint : constraints) {
        switch (constraint->getType()) {
            case ConstraintType::Extends:
                if (hasExtends) {
                    std::cerr << "Multiple extends constraints not allowed" << std::endl;
                    return false;
                }
                hasExtends = true;
                break;
            case ConstraintType::Implements:
                hasImplements = true;
                break;
            case ConstraintType::Constructor:
                // Constructor constraint is compatible with others
                break;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeMethodBodyConstraints(GenericMethodDeclaration* method) {
    // Analyze method body for constraint violations
    return analyzeNodeConstraints(method->getBody(), method->getTypeParameters());
}

bool GenericConstraintsAnalyzer::analyzeMemberConstraints(ASTNode* member, const std::vector<TypeParameter*>& typeParams) {
    if (!member) return true;
    
    switch (member->getType()) {
        case ASTNodeType::MethodDeclaration:
            return analyzeMethodConstraints(static_cast<MethodDeclaration*>(member), typeParams);
        case ASTNodeType::PropertyDeclaration:
            return analyzePropertyConstraints(static_cast<PropertyDeclaration*>(member), typeParams);
        default:
            return true;
    }
}

bool GenericConstraintsAnalyzer::analyzeMethodConstraints(MethodDeclaration* method, const std::vector<TypeParameter*>& typeParams) {
    // Analyze method parameters
    for (auto& param : method->getParameters()) {
        if (!analyzeParameterConstraints(param, typeParams)) {
            return false;
        }
    }
    
    // Analyze method body
    if (method->getBody()) {
        if (!analyzeNodeConstraints(method->getBody(), typeParams)) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzePropertyConstraints(PropertyDeclaration* property, const std::vector<TypeParameter*>& typeParams) {
    // Analyze property type
    if (property->getType()) {
        if (!analyzeTypeConstraints(property->getType(), typeParams)) {
            return false;
        }
    }
    
    // Analyze property initializer
    if (property->getInitializer()) {
        if (!analyzeNodeConstraints(property->getInitializer(), typeParams)) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeParameterConstraints(Parameter* param, const std::vector<TypeParameter*>& typeParams) {
    if (param->getType()) {
        return analyzeTypeConstraints(param->getType(), typeParams);
    }
    return true;
}

bool GenericConstraintsAnalyzer::analyzeTypeConstraints(Type* type, const std::vector<TypeParameter*>& typeParams) {
    if (!type) return true;
    
    switch (type->getType()) {
        case TypeType::GenericType:
            return analyzeGenericTypeConstraints(static_cast<GenericType*>(type), typeParams);
        case TypeType::ArrayType:
            return analyzeArrayTypeConstraints(static_cast<ArrayType*>(type), typeParams);
        case TypeType::UnionType:
            return analyzeUnionTypeConstraints(static_cast<UnionType*>(type), typeParams);
        case TypeType::IntersectionType:
            return analyzeIntersectionTypeConstraints(static_cast<IntersectionType*>(type), typeParams);
        default:
            return true;
    }
}

bool GenericConstraintsAnalyzer::analyzeGenericTypeConstraints(GenericType* genericType, const std::vector<TypeParameter*>& typeParams) {
    // Check if generic type parameter is in scope
    auto typeParamName = genericType->getName();
    auto typeParam = findTypeParameter(typeParamName, typeParams);
    
    if (!typeParam) {
        std::cerr << "Type parameter not found: " << typeParamName << std::endl;
        return false;
    }
    
    // Analyze type arguments
    for (auto& typeArg : genericType->getTypeArguments()) {
        if (!analyzeTypeConstraints(typeArg, typeParams)) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeArrayTypeConstraints(ArrayType* arrayType, const std::vector<TypeParameter*>& typeParams) {
    return analyzeTypeConstraints(arrayType->getElementType(), typeParams);
}

bool GenericConstraintsAnalyzer::analyzeUnionTypeConstraints(UnionType* unionType, const std::vector<TypeParameter*>& typeParams) {
    for (auto& type : unionType->getTypes()) {
        if (!analyzeTypeConstraints(type, typeParams)) {
            return false;
        }
    }
    return true;
}

bool GenericConstraintsAnalyzer::analyzeIntersectionTypeConstraints(IntersectionType* intersectionType, const std::vector<TypeParameter*>& typeParams) {
    for (auto& type : intersectionType->getTypes()) {
        if (!analyzeTypeConstraints(type, typeParams)) {
            return false;
        }
    }
    return true;
}

bool GenericConstraintsAnalyzer::analyzeNodeConstraints(ASTNode* node, const std::vector<TypeParameter*>& typeParams) {
    if (!node) return true;
    
    // Analyze expressions that use type parameters
    switch (node->getType()) {
        case ASTNodeType::CallExpression:
            return analyzeCallExpressionConstraints(static_cast<CallExpression*>(node), typeParams);
        case ASTNodeType::MemberExpression:
            return analyzeMemberExpressionConstraints(static_cast<MemberExpression*>(node), typeParams);
        case ASTNodeType::NewExpression:
            return analyzeNewExpressionConstraints(static_cast<NewExpression*>(node), typeParams);
        default:
            // Recursively analyze child nodes
            for (auto& child : node->getChildren()) {
                if (!analyzeNodeConstraints(child, typeParams)) {
                    return false;
                }
            }
            return true;
    }
}

bool GenericConstraintsAnalyzer::analyzeCallExpressionConstraints(CallExpression* callExpr, const std::vector<TypeParameter*>& typeParams) {
    // Analyze function call with generic constraints
    if (callExpr->getCallee()) {
        if (!analyzeNodeConstraints(callExpr->getCallee(), typeParams)) {
            return false;
        }
    }
    
    // Analyze arguments
    for (auto& arg : callExpr->getArguments()) {
        if (!analyzeNodeConstraints(arg, typeParams)) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeMemberExpressionConstraints(MemberExpression* memberExpr, const std::vector<TypeParameter*>& typeParams) {
    // Analyze object
    if (memberExpr->getObject()) {
        if (!analyzeNodeConstraints(memberExpr->getObject(), typeParams)) {
            return false;
        }
    }
    
    // Analyze property
    if (memberExpr->getProperty()) {
        if (!analyzeNodeConstraints(memberExpr->getProperty(), typeParams)) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::analyzeNewExpressionConstraints(NewExpression* newExpr, const std::vector<TypeParameter*>& typeParams) {
    // Analyze constructor
    if (newExpr->getCallee()) {
        if (!analyzeNodeConstraints(newExpr->getCallee(), typeParams)) {
            return false;
        }
    }
    
    // Analyze arguments
    for (auto& arg : newExpr->getArguments()) {
        if (!analyzeNodeConstraints(arg, typeParams)) {
            return false;
        }
    }
    
    return true;
}

TypeParameter* GenericConstraintsAnalyzer::findTypeParameter(const std::string& name, const std::vector<TypeParameter*>& typeParams) {
    for (auto& typeParam : typeParams) {
        if (typeParam->getName() == name) {
            return typeParam;
        }
    }
    return nullptr;
}

// =============================================================================
// GENERIC CONSTRAINTS VALIDATION
// =============================================================================

bool GenericConstraintsAnalyzer::validateGenericMethodCall(CallExpression* callExpr, const std::vector<Type*>& typeArguments) {
    std::cout << "Validating generic method call" << std::endl;
    
    // Get the method being called
    auto method = getMethodFromCall(callExpr);
    if (!method) {
        std::cerr << "Method not found in call expression" << std::endl;
        return false;
    }
    
    // Check if method is generic
    if (!method->isGeneric()) {
        std::cerr << "Method is not generic" << std::endl;
        return false;
    }
    
    // Validate type arguments against constraints
    auto typeParams = method->getTypeParameters();
    if (typeArguments.size() != typeParams.size()) {
        std::cerr << "Type argument count mismatch" << std::endl;
        return false;
    }
    
    for (size_t i = 0; i < typeArguments.size(); ++i) {
        if (!validateTypeArgumentAgainstConstraints(typeArguments[i], typeParams[i])) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::validateTypeArgumentAgainstConstraints(Type* typeArg, TypeParameter* typeParam) {
    std::cout << "Validating type argument against constraints for: " << typeParam->getName() << std::endl;
    
    for (auto& constraint : typeParam->getConstraints()) {
        if (!validateTypeArgumentAgainstConstraint(typeArg, constraint)) {
            return false;
        }
    }
    
    return true;
}

bool GenericConstraintsAnalyzer::validateTypeArgumentAgainstConstraint(Type* typeArg, Constraint* constraint) {
    switch (constraint->getType()) {
        case ConstraintType::Extends:
            return validateExtendsConstraintForType(typeArg, static_cast<ExtendsConstraint*>(constraint));
        case ConstraintType::Implements:
            return validateImplementsConstraintForType(typeArg, static_cast<ImplementsConstraint*>(constraint));
        case ConstraintType::Constructor:
            return validateConstructorConstraintForType(typeArg, static_cast<ConstructorConstraint*>(constraint));
        default:
            return false;
    }
}

bool GenericConstraintsAnalyzer::validateExtendsConstraintForType(Type* typeArg, ExtendsConstraint* constraint) {
    // Check if typeArg extends the constraint base type
    auto baseType = constraint->getBaseType();
    
    // This is a simplified check - in a real implementation, you'd need to
    // check the inheritance hierarchy
    if (typeArg->getName() == baseType->getName()) {
        return true;
    }
    
    // Check if typeArg is a subtype of baseType
    return checkSubtypeRelation(typeArg, baseType);
}

bool GenericConstraintsAnalyzer::validateImplementsConstraintForType(Type* typeArg, ImplementsConstraint* constraint) {
    // Check if typeArg implements the constraint interface
    auto interfaceType = constraint->getInterfaceType();
    
    // This is a simplified check - in a real implementation, you'd need to
    // check the interface implementation
    return checkImplementsRelation(typeArg, interfaceType);
}

bool GenericConstraintsAnalyzer::validateConstructorConstraintForType(Type* typeArg, ConstructorConstraint* constraint) {
    // Check if typeArg has a constructor
    // This is always true for most types in TypeScript
    return true;
}

bool GenericConstraintsAnalyzer::checkSubtypeRelation(Type* subtype, Type* supertype) {
    // Simplified subtype checking
    // In a real implementation, this would check the inheritance hierarchy
    return subtype->getName() == supertype->getName();
}

bool GenericConstraintsAnalyzer::checkImplementsRelation(Type* type, Type* interface) {
    // Simplified interface implementation checking
    // In a real implementation, this would check the interface implementation
    return true;
}

MethodDeclaration* GenericConstraintsAnalyzer::getMethodFromCall(CallExpression* callExpr) {
    // Simplified method lookup
    // In a real implementation, this would look up the method in the symbol table
    return nullptr;
}

} // namespace semantic
} // namespace tsc