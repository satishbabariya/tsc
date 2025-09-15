#include "tsc/semantic/EnhancedGenericsTypeChecker.h"
#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

namespace tsc {
namespace semantic {

// =============================================================================
// ENHANCED GENERICS TYPE CHECKER IMPLEMENTATION
// =============================================================================

EnhancedGenericsTypeChecker::EnhancedGenericsTypeChecker(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool EnhancedGenericsTypeChecker::checkGenericDeclaration(GenericDeclaration* genericDecl) {
    if (!genericDecl) {
        std::cerr << "Generic declaration is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced type checking generic declaration: " << genericDecl->getName() << std::endl;
    
    // Enhanced type checking for generics
    if (!checkGenericConstraints(genericDecl->getTypeParameters())) {
        std::cerr << "Failed to check generic constraints" << std::endl;
        return false;
    }
    
    if (!checkGenericInference(genericDecl)) {
        std::cerr << "Failed to check generic inference" << std::endl;
        return false;
    }
    
    if (!checkGenericOverloads(genericDecl)) {
        std::cerr << "Failed to check generic overloads" << std::endl;
        return false;
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkGenericConstraints(const std::vector<TypeParameter*>& typeParams) {
    std::cout << "Enhanced checking generic constraints" << std::endl;
    
    for (const auto& typeParam : typeParams) {
        if (!checkTypeParameterConstraints(typeParam)) {
            std::cerr << "Failed to check type parameter constraints" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkTypeParameterConstraints(TypeParameter* typeParam) {
    if (!typeParam) {
        return true;
    }
    
    std::cout << "Enhanced checking type parameter constraints: " << typeParam->getName() << std::endl;
    
    // Check constraint validity
    if (typeParam->getConstraint()) {
        if (!checkConstraintValidity(typeParam->getConstraint())) {
            std::cerr << "Invalid constraint for type parameter: " << typeParam->getName() << std::endl;
            return false;
        }
    }
    
    // Check default type
    if (typeParam->getDefaultType()) {
        if (!checkDefaultTypeCompatibility(typeParam)) {
            std::cerr << "Incompatible default type for type parameter: " << typeParam->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkConstraintValidity(Type* constraint) {
    if (!constraint) {
        return true;
    }
    
    std::cout << "Enhanced checking constraint validity: " << constraint->getName() << std::endl;
    
    // Check if constraint is a valid type
    if (!isValidConstraintType(constraint)) {
        std::cerr << "Invalid constraint type: " << constraint->getName() << std::endl;
        return false;
    }
    
    // Check constraint complexity
    if (isComplexConstraint(constraint)) {
        if (!checkComplexConstraint(constraint)) {
            std::cerr << "Complex constraint validation failed: " << constraint->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkComplexConstraint(Type* constraint) {
    if (!constraint) {
        return true;
    }
    
    std::cout << "Enhanced checking complex constraint: " << constraint->getName() << std::endl;
    
    // Handle intersection types
    if (constraint->getType() == TypeType::IntersectionType) {
        auto intersectionType = static_cast<IntersectionType*>(constraint);
        return checkIntersectionConstraint(intersectionType);
    }
    
    // Handle union types
    if (constraint->getType() == TypeType::UnionType) {
        auto unionType = static_cast<UnionType*>(constraint);
        return checkUnionConstraint(unionType);
    }
    
    // Handle generic types
    if (constraint->getType() == TypeType::GenericType) {
        auto genericType = static_cast<GenericType*>(constraint);
        return checkGenericConstraint(genericType);
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkIntersectionConstraint(IntersectionType* intersectionType) {
    if (!intersectionType) {
        return true;
    }
    
    std::cout << "Enhanced checking intersection constraint" << std::endl;
    
    auto types = intersectionType->getTypes();
    for (const auto& type : types) {
        if (!checkConstraintValidity(type)) {
            std::cerr << "Invalid type in intersection constraint" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkUnionConstraint(UnionType* unionType) {
    if (!unionType) {
        return true;
    }
    
    std::cout << "Enhanced checking union constraint" << std::endl;
    
    auto types = unionType->getTypes();
    for (const auto& type : types) {
        if (!checkConstraintValidity(type)) {
            std::cerr << "Invalid type in union constraint" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkGenericConstraint(GenericType* genericType) {
    if (!genericType) {
        return true;
    }
    
    std::cout << "Enhanced checking generic constraint: " << genericType->getName() << std::endl;
    
    // Check generic type arguments
    auto typeArguments = genericType->getTypeArguments();
    for (const auto& typeArg : typeArguments) {
        if (!checkConstraintValidity(typeArg)) {
            std::cerr << "Invalid type argument in generic constraint" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkDefaultTypeCompatibility(TypeParameter* typeParam) {
    if (!typeParam || !typeParam->getDefaultType()) {
        return true;
    }
    
    std::cout << "Enhanced checking default type compatibility: " << typeParam->getName() << std::endl;
    
    Type* constraint = typeParam->getConstraint();
    Type* defaultType = typeParam->getDefaultType();
    
    if (constraint) {
        if (!areTypesCompatible(constraint, defaultType)) {
            std::cerr << "Default type is not compatible with constraint" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkGenericInference(GenericDeclaration* genericDecl) {
    if (!genericDecl) {
        return true;
    }
    
    std::cout << "Enhanced checking generic inference: " << genericDecl->getName() << std::endl;
    
    // Check type parameter inference
    if (!checkTypeParameterInference(genericDecl->getTypeParameters())) {
        std::cerr << "Failed to check type parameter inference" << std::endl;
        return false;
    }
    
    // Check generic method inference
    if (genericDecl->getType() == ASTNodeType::MethodDeclaration) {
        auto methodDecl = static_cast<MethodDeclaration*>(genericDecl);
        if (!checkGenericMethodInference(methodDecl)) {
            std::cerr << "Failed to check generic method inference" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkTypeParameterInference(const std::vector<TypeParameter*>& typeParams) {
    std::cout << "Enhanced checking type parameter inference" << std::endl;
    
    for (const auto& typeParam : typeParams) {
        if (!checkSingleTypeParameterInference(typeParam)) {
            std::cerr << "Failed to check type parameter inference: " << typeParam->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkSingleTypeParameterInference(TypeParameter* typeParam) {
    if (!typeParam) {
        return true;
    }
    
    std::cout << "Enhanced checking single type parameter inference: " << typeParam->getName() << std::endl;
    
    // Check if type parameter can be inferred
    if (!canInferTypeParameter(typeParam)) {
        std::cerr << "Cannot infer type parameter: " << typeParam->getName() << std::endl;
        return false;
    }
    
    // Check inference complexity
    if (isComplexInferenceScenario(typeParam)) {
        if (!checkComplexInferenceScenario(typeParam)) {
            std::cerr << "Complex inference scenario failed: " << typeParam->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkComplexInferenceScenario(TypeParameter* typeParam) {
    if (!typeParam) {
        return true;
    }
    
    std::cout << "Enhanced checking complex inference scenario: " << typeParam->getName() << std::endl;
    
    // Handle complex inference scenarios
    if (typeParam->getConstraint()) {
        if (!checkConstraintBasedInference(typeParam)) {
            std::cerr << "Constraint-based inference failed" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkConstraintBasedInference(TypeParameter* typeParam) {
    if (!typeParam || !typeParam->getConstraint()) {
        return true;
    }
    
    std::cout << "Enhanced checking constraint-based inference: " << typeParam->getName() << std::endl;
    
    Type* constraint = typeParam->getConstraint();
    
    // Check if constraint provides enough information for inference
    if (!constraintProvidesInferenceInfo(constraint)) {
        std::cerr << "Constraint does not provide enough information for inference" << std::endl;
        return false;
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkGenericMethodInference(MethodDeclaration* methodDecl) {
    if (!methodDecl) {
        return true;
    }
    
    std::cout << "Enhanced checking generic method inference: " << methodDecl->getName() << std::endl;
    
    // Check method parameter inference
    auto parameters = methodDecl->getParameters();
    for (const auto& param : parameters) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            if (!checkParameterInference(parameter)) {
                std::cerr << "Failed to check parameter inference" << std::endl;
                return false;
            }
        }
    }
    
    // Check return type inference
    if (methodDecl->getReturnType()) {
        if (!checkReturnTypeInference(methodDecl)) {
            std::cerr << "Failed to check return type inference" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkParameterInference(Parameter* parameter) {
    if (!parameter) {
        return true;
    }
    
    std::cout << "Enhanced checking parameter inference: " << parameter->getName() << std::endl;
    
    // Check if parameter type can be inferred
    if (!parameter->getType()) {
        if (!canInferParameterType(parameter)) {
            std::cerr << "Cannot infer parameter type: " << parameter->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkReturnTypeInference(MethodDeclaration* methodDecl) {
    if (!methodDecl) {
        return true;
    }
    
    std::cout << "Enhanced checking return type inference: " << methodDecl->getName() << std::endl;
    
    // Check if return type can be inferred
    if (!methodDecl->getReturnType()) {
        if (!canInferReturnType(methodDecl)) {
            std::cerr << "Cannot infer return type for method: " << methodDecl->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkGenericOverloads(GenericDeclaration* genericDecl) {
    if (!genericDecl) {
        return true;
    }
    
    std::cout << "Enhanced checking generic overloads: " << genericDecl->getName() << std::endl;
    
    // Check if declaration has overloads
    if (genericDecl->getType() == ASTNodeType::MethodDeclaration) {
        auto methodDecl = static_cast<MethodDeclaration*>(genericDecl);
        if (methodDecl->hasOverloads()) {
            if (!checkMethodOverloads(methodDecl->getOverloads())) {
                std::cerr << "Failed to check method overloads" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkMethodOverloads(const std::vector<MethodOverloadDeclaration*>& overloads) {
    std::cout << "Enhanced checking method overloads" << std::endl;
    
    // Check each overload
    for (size_t i = 0; i < overloads.size(); ++i) {
        if (!checkMethodOverload(overloads[i])) {
            std::cerr << "Failed to check method overload " << i << std::endl;
            return false;
        }
    }
    
    // Check for overload conflicts
    if (!checkOverloadConflicts(overloads)) {
        std::cerr << "Overload conflicts detected" << std::endl;
        return false;
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkMethodOverload(MethodOverloadDeclaration* overload) {
    if (!overload) {
        return true;
    }
    
    std::cout << "Enhanced checking method overload: " << overload->getName() << std::endl;
    
    // Check overload parameters
    auto parameters = overload->getParameters();
    for (const auto& param : parameters) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            if (!checkParameterInference(parameter)) {
                std::cerr << "Failed to check overload parameter inference" << std::endl;
                return false;
            }
        }
    }
    
    // Check overload return type
    if (overload->getReturnType()) {
        if (!checkReturnTypeInference(overload)) {
            std::cerr << "Failed to check overload return type inference" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::checkOverloadConflicts(const std::vector<MethodOverloadDeclaration*>& overloads) {
    std::cout << "Enhanced checking overload conflicts" << std::endl;
    
    // Check for duplicate signatures
    for (size_t i = 0; i < overloads.size(); ++i) {
        for (size_t j = i + 1; j < overloads.size(); ++j) {
            if (areOverloadSignaturesIdentical(overloads[i], overloads[j])) {
                std::cerr << "Duplicate overload signatures found" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::areOverloadSignaturesIdentical(MethodOverloadDeclaration* overload1, 
                                                                MethodOverloadDeclaration* overload2) {
    if (!overload1 || !overload2) {
        return false;
    }
    
    // Check parameter count
    if (overload1->getParameters().size() != overload2->getParameters().size()) {
        return false;
    }
    
    // Check parameter types
    auto params1 = overload1->getParameters();
    auto params2 = overload2->getParameters();
    
    for (size_t i = 0; i < params1.size(); ++i) {
        if (!areParameterTypesIdentical(params1[i], params2[i])) {
            return false;
        }
    }
    
    return true;
}

bool EnhancedGenericsTypeChecker::areParameterTypesIdentical(ASTNode* param1, ASTNode* param2) {
    if (!param1 || !param2) {
        return false;
    }
    
    if (param1->getType() != param2->getType()) {
        return false;
    }
    
    if (param1->getType() == ASTNodeType::Parameter) {
        auto p1 = static_cast<Parameter*>(param1);
        auto p2 = static_cast<Parameter*>(param2);
        
        // Check parameter types
        Type* type1 = p1->getType();
        Type* type2 = p2->getType();
        
        if (type1 && type2) {
            return areTypesIdentical(type1, type2);
        } else if (!type1 && !type2) {
            return true;
        }
    }
    
    return false;
}

// =============================================================================
// HELPER METHODS
// =============================================================================

bool EnhancedGenericsTypeChecker::isValidConstraintType(Type* type) {
    if (!type) return false;
    
    // Check if type is a valid constraint type
    switch (type->getType()) {
        case TypeType::InterfaceType:
        case TypeType::ClassType:
        case TypeType::GenericType:
        case TypeType::UnionType:
        case TypeType::IntersectionType:
            return true;
        default:
            return false;
    }
}

bool EnhancedGenericsTypeChecker::isComplexConstraint(Type* type) {
    if (!type) return false;
    
    // Check if constraint is complex
    switch (type->getType()) {
        case TypeType::IntersectionType:
        case TypeType::UnionType:
        case TypeType::GenericType:
            return true;
        default:
            return false;
    }
}

bool EnhancedGenericsTypeChecker::canInferTypeParameter(TypeParameter* typeParam) {
    if (!typeParam) return false;
    
    // Check if type parameter can be inferred
    // This is a simplified check - in a real implementation, this would analyze
    // the usage context to determine if inference is possible
    
    return true; // Assume inference is possible for now
}

bool EnhancedGenericsTypeChecker::isComplexInferenceScenario(TypeParameter* typeParam) {
    if (!typeParam) return false;
    
    // Check if inference scenario is complex
    return typeParam->getConstraint() != nullptr;
}

bool EnhancedGenericsTypeChecker::constraintProvidesInferenceInfo(Type* constraint) {
    if (!constraint) return false;
    
    // Check if constraint provides enough information for inference
    // This is a simplified check - in a real implementation, this would analyze
    // the constraint structure to determine inference capability
    
    return true; // Assume constraint provides inference info for now
}

bool EnhancedGenericsTypeChecker::canInferParameterType(Parameter* parameter) {
    if (!parameter) return false;
    
    // Check if parameter type can be inferred
    // This is a simplified check - in a real implementation, this would analyze
    // the parameter usage context
    
    return true; // Assume inference is possible for now
}

bool EnhancedGenericsTypeChecker::canInferReturnType(MethodDeclaration* methodDecl) {
    if (!methodDecl) return false;
    
    // Check if return type can be inferred
    // This is a simplified check - in a real implementation, this would analyze
    // the method body to determine return type
    
    return true; // Assume inference is possible for now
}

bool EnhancedGenericsTypeChecker::areTypesCompatible(Type* type1, Type* type2) {
    if (!type1 || !type2) return false;
    
    // Check type compatibility
    if (type1->getName() == type2->getName()) {
        return true;
    }
    
    // Check structural compatibility
    if (isStructurallyCompatible(type1, type2)) {
        return true;
    }
    
    return false;
}

bool EnhancedGenericsTypeChecker::areTypesIdentical(Type* type1, Type* type2) {
    if (!type1 || !type2) return false;
    
    return type1->getName() == type2->getName();
}

bool EnhancedGenericsTypeChecker::isStructurallyCompatible(Type* type1, Type* type2) {
    if (!type1 || !type2) return false;
    
    // Check structural compatibility
    // This is a simplified check - in a real implementation, this would check
    // structural typing rules
    
    return false; // Assume no structural compatibility for now
}

} // namespace semantic
} // namespace tsc