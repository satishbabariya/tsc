#include "tsc/semantic/GenericConstraintChecker.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <iostream>

namespace tsc {

GenericConstraintChecker::GenericConstraintChecker(DiagnosticEngine& diagnostics, TypeSystem& typeSystem)
    : diagnostics_(diagnostics), typeSystem_(typeSystem) {
}

bool GenericConstraintChecker::checkTypeConstraint(const TypeParameter& typeParam, 
                                                  shared_ptr<Type> typeArgument, 
                                                  const SourceLocation& location) {
    if (!typeParam.hasConstraint()) {
        return true; // No constraint to check
    }

    auto constraint = typeParam.getConstraint();
    if (!constraint) {
        return true; // No constraint to check
    }

    // Check the extends constraint
    if (!checkExtendsConstraint(constraint, typeArgument, location)) {
        return false;
    }

    // Check variance constraint
    if (!checkVarianceConstraint(typeParam, typeArgument, location)) {
        return false;
    }

    return true;
}

bool GenericConstraintChecker::checkExtendsConstraint(shared_ptr<Type> constraint, 
                                                    shared_ptr<Type> typeArgument, 
                                                    const SourceLocation& location) {
    if (!constraint || !typeArgument) {
        return true; // No constraint to check
    }

    // Check if typeArgument satisfies the constraint
    if (!satisfiesConstraint(typeArgument, constraint)) {
        reportConstraintError("Type argument '" + typeArgument->toString() + 
                            "' does not satisfy constraint '" + constraint->toString() + "'", 
                            location);
        return false;
    }

    return true;
}

bool GenericConstraintChecker::checkVarianceConstraint(const TypeParameter& typeParam, 
                                                     shared_ptr<Type> typeArgument, 
                                                     const SourceLocation& location) {
    Variance variance = typeParam.getVariance();
    
    if (variance == Variance::Invariant) {
        return true; // No variance constraint to check
    }

    // For now, we'll implement basic variance checking
    // In a full implementation, this would check the variance of the type argument
    // in the context where it's being used
    
    if (variance == Variance::Covariant) {
        // Covariant: typeArgument must be a subtype of the constraint
        if (typeParam.hasConstraint()) {
            if (!isSubtypeOf(typeArgument, typeParam.getConstraint())) {
                reportVarianceError("Covariant type parameter '" + typeParam.getName() + 
                                  "' requires type argument to be a subtype of constraint", 
                                  location);
                return false;
            }
        }
    } else if (variance == Variance::Contravariant) {
        // Contravariant: constraint must be a subtype of typeArgument
        if (typeParam.hasConstraint()) {
            if (!isSubtypeOf(typeParam.getConstraint(), typeArgument)) {
                reportVarianceError("Contravariant type parameter '" + typeParam.getName() + 
                                  "' requires constraint to be a subtype of type argument", 
                                  location);
                return false;
            }
        }
    }

    return true;
}

bool GenericConstraintChecker::checkCovariance(shared_ptr<Type> from, shared_ptr<Type> to, 
                                             const SourceLocation& location) {
    if (!from || !to) {
        return true;
    }

    // Covariant: from must be a subtype of to
    if (!isSubtypeOf(from, to)) {
        reportVarianceError("Covariant type mismatch: '" + from->toString() + 
                          "' is not a subtype of '" + to->toString() + "'", 
                          location);
        return false;
    }

    return true;
}

bool GenericConstraintChecker::checkContravariance(shared_ptr<Type> from, shared_ptr<Type> to, 
                                                 const SourceLocation& location) {
    if (!from || !to) {
        return true;
    }

    // Contravariant: to must be a subtype of from
    if (!isSubtypeOf(to, from)) {
        reportVarianceError("Contravariant type mismatch: '" + to->toString() + 
                          "' is not a subtype of '" + from->toString() + "'", 
                          location);
        return false;
    }

    return true;
}

bool GenericConstraintChecker::checkInvariance(shared_ptr<Type> from, shared_ptr<Type> to, 
                                             const SourceLocation& location) {
    if (!from || !to) {
        return true;
    }

    // Invariant: types must be equivalent
    if (!from->isEquivalentTo(*to)) {
        reportVarianceError("Invariant type mismatch: '" + from->toString() + 
                          "' is not equivalent to '" + to->toString() + "'", 
                          location);
        return false;
    }

    return true;
}

bool GenericConstraintChecker::validateGenericInstantiation(shared_ptr<Type> genericType, 
                                                          const std::vector<shared_ptr<Type>>& typeArguments,
                                                          const SourceLocation& location) {
    if (!genericType) {
        return false;
    }

    // Check if this is a generic type
    if (genericType->getKind() != TypeKind::Generic) {
        return true; // Not a generic type, no validation needed
    }

    auto generic = std::static_pointer_cast<GenericType>(genericType);
    auto baseType = generic->getBaseType();
    auto providedArgs = generic->getTypeArguments();

    // Check if we have the right number of type arguments
    if (typeArguments.size() != providedArgs.size()) {
        reportConstraintError("Generic type requires " + std::to_string(providedArgs.size()) + 
                            " type arguments, but " + std::to_string(typeArguments.size()) + 
                            " were provided", location);
        return false;
    }

    // Validate each type argument against its corresponding type parameter
    // This is a simplified implementation - in practice, we'd need to get the
    // type parameters from the class/function declaration
    for (size_t i = 0; i < typeArguments.size(); ++i) {
        if (!typeArguments[i]) {
            reportConstraintError("Type argument " + std::to_string(i) + " is null", location);
            return false;
        }
    }

    return true;
}

shared_ptr<Type> GenericConstraintChecker::substituteTypeParameters(shared_ptr<Type> type, 
                                                                  const std::unordered_map<String, shared_ptr<Type>>& substitutions) {
    if (!type) {
        return type;
    }

    return substituteInType(type, substitutions);
}

bool GenericConstraintChecker::satisfiesConstraint(shared_ptr<Type> type, shared_ptr<Type> constraint) {
    if (!type || !constraint) {
        return true; // No constraint to satisfy
    }

    // Check if type satisfies the constraint
    return isSubtypeOf(type, constraint);
}

bool GenericConstraintChecker::isSubtypeOf(shared_ptr<Type> subtype, shared_ptr<Type> supertype) {
    if (!subtype || !supertype) {
        return false;
    }

    // If types are equivalent, they are subtypes of each other
    if (subtype->isEquivalentTo(*supertype)) {
        return true;
    }

    // Check primitive type relationships
    if (subtype->getKind() == TypeKind::Number && supertype->getKind() == TypeKind::Number) {
        return true;
    }
    if (subtype->getKind() == TypeKind::String && supertype->getKind() == TypeKind::String) {
        return true;
    }
    if (subtype->getKind() == TypeKind::Boolean && supertype->getKind() == TypeKind::Boolean) {
        return true;
    }

    // Check class inheritance
    if (subtype->getKind() == TypeKind::Class && supertype->getKind() == TypeKind::Class) {
        auto subClass = std::static_pointer_cast<ClassType>(subtype);
        auto superClass = std::static_pointer_cast<ClassType>(supertype);
        
        // Check if subClass extends superClass
        auto currentClass = subClass;
        while (currentClass) {
            if (currentClass->getName() == superClass->getName()) {
                return true;
            }
            currentClass = std::static_pointer_cast<ClassType>(currentClass->getBaseClass());
        }
    }

    // Check generic type relationships
    if (subtype->getKind() == TypeKind::Generic && supertype->getKind() == TypeKind::Generic) {
        auto subGeneric = std::static_pointer_cast<GenericType>(subtype);
        auto superGeneric = std::static_pointer_cast<GenericType>(supertype);
        
        // Check if base types are the same and type arguments are compatible
        if (subGeneric->getBaseType()->isEquivalentTo(*superGeneric->getBaseType())) {
            auto subArgs = subGeneric->getTypeArguments();
            auto superArgs = superGeneric->getTypeArguments();
            
            if (subArgs.size() == superArgs.size()) {
                for (size_t i = 0; i < subArgs.size(); ++i) {
                    if (!isSubtypeOf(subArgs[i], superArgs[i])) {
                        return false;
                    }
                }
                return true;
            }
        }
    }

    // Check union types
    if (supertype->getKind() == TypeKind::Union) {
        auto unionType = std::static_pointer_cast<UnionType>(supertype);
        for (const auto& memberType : unionType->getTypes()) {
            if (isSubtypeOf(subtype, memberType)) {
                return true;
            }
        }
    }

    // Check intersection types
    if (subtype->getKind() == TypeKind::Intersection) {
        auto intersectionType = std::static_pointer_cast<IntersectionType>(subtype);
        for (const auto& memberType : intersectionType->getTypes()) {
            if (!isSubtypeOf(memberType, supertype)) {
                return false;
            }
        }
        return true;
    }

    return false;
}

void GenericConstraintChecker::reportConstraintError(const String& message, const SourceLocation& location) {
    diagnostics_.error("Generic constraint error: " + message, location);
}

void GenericConstraintChecker::reportVarianceError(const String& message, const SourceLocation& location) {
    diagnostics_.error("Generic variance error: " + message, location);
}

// Helper method implementations
bool GenericConstraintChecker::checkPrimitiveConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument) {
    if (!constraint || !typeArgument) {
        return true;
    }

    // Check if typeArgument is a primitive type that matches the constraint
    if (constraint->getKind() == TypeKind::Number && typeArgument->getKind() == TypeKind::Number) {
        return true;
    }
    if (constraint->getKind() == TypeKind::String && typeArgument->getKind() == TypeKind::String) {
        return true;
    }
    if (constraint->getKind() == TypeKind::Boolean && typeArgument->getKind() == TypeKind::Boolean) {
        return true;
    }

    return false;
}

bool GenericConstraintChecker::checkClassConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument) {
    if (!constraint || !typeArgument) {
        return true;
    }

    // Check if typeArgument is a class type that extends or is the constraint
    if (typeArgument->getKind() == TypeKind::Class) {
        auto classType = std::static_pointer_cast<ClassType>(typeArgument);
        auto constraintClass = std::static_pointer_cast<ClassType>(constraint);
        
        // Check if the class is the same or extends the constraint
        auto currentClass = classType;
        while (currentClass) {
            if (currentClass->getName() == constraintClass->getName()) {
                return true;
            }
            currentClass = std::static_pointer_cast<ClassType>(currentClass->getBaseClass());
        }
    }

    return false;
}

bool GenericConstraintChecker::checkInterfaceConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument) {
    if (!constraint || !typeArgument) {
        return true;
    }

    // Check if typeArgument implements the constraint interface
    // This is a simplified implementation
    return typeArgument->isEquivalentTo(*constraint);
}

bool GenericConstraintChecker::checkUnionConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument) {
    if (!constraint || !typeArgument) {
        return true;
    }

    if (constraint->getKind() == TypeKind::Union) {
        auto unionType = std::static_pointer_cast<UnionType>(constraint);
        for (const auto& memberType : unionType->getTypes()) {
            if (isSubtypeOf(typeArgument, memberType)) {
                return true;
            }
        }
    }

    return false;
}

bool GenericConstraintChecker::checkIntersectionConstraint(shared_ptr<Type> constraint, shared_ptr<Type> typeArgument) {
    if (!constraint || !typeArgument) {
        return true;
    }

    if (constraint->getKind() == TypeKind::Intersection) {
        auto intersectionType = std::static_pointer_cast<IntersectionType>(constraint);
        for (const auto& memberType : intersectionType->getTypes()) {
            if (!isSubtypeOf(typeArgument, memberType)) {
                return false;
            }
        }
        return true;
    }

    return false;
}

shared_ptr<Type> GenericConstraintChecker::substituteInType(shared_ptr<Type> type, 
                                                          const std::unordered_map<String, shared_ptr<Type>>& substitutions) {
    if (!type) {
        return type;
    }

    // Handle type parameter substitution
    if (type->getKind() == TypeKind::TypeParameter) {
        auto typeParam = std::static_pointer_cast<TypeParameterType>(type);
        auto it = substitutions.find(typeParam->getName());
        if (it != substitutions.end()) {
            return it->second;
        }
        return type; // No substitution found
    }

    // Handle generic type substitution
    if (type->getKind() == TypeKind::Generic) {
        auto genericType = std::static_pointer_cast<GenericType>(type);
        auto baseType = substituteInType(genericType->getBaseType(), substitutions);
        auto typeArgs = substituteInTypeList(genericType->getTypeArguments(), substitutions);
        
        if (baseType != genericType->getBaseType() || typeArgs != genericType->getTypeArguments()) {
            return typeSystem_.createGenericType(baseType, typeArgs);
        }
        return type;
    }

    // Handle array type substitution
    if (type->getKind() == TypeKind::Array) {
        auto arrayType = std::static_pointer_cast<ArrayType>(type);
        auto elementType = substituteInType(arrayType->getElementType(), substitutions);
        
        if (elementType != arrayType->getElementType()) {
            return typeSystem_.createArrayType(elementType);
        }
        return type;
    }

    // Handle function type substitution
    if (type->getKind() == TypeKind::Function) {
        auto funcType = std::static_pointer_cast<FunctionType>(type);
        auto returnType = substituteInType(funcType->getReturnType(), substitutions);
        
        std::vector<FunctionType::Parameter> newParams;
        bool paramsChanged = false;
        
        for (const auto& param : funcType->getParameters()) {
            auto newParamType = substituteInType(param.type, substitutions);
            if (newParamType != param.type) {
                paramsChanged = true;
            }
            
            FunctionType::Parameter newParam;
            newParam.name = param.name;
            newParam.type = newParamType;
            newParam.optional = param.optional;
            newParam.rest = param.rest;
            newParams.push_back(newParam);
        }
        
        if (returnType != funcType->getReturnType() || paramsChanged) {
            return typeSystem_.createFunctionType(newParams, returnType);
        }
        return type;
    }

    // For other types, no substitution needed
    return type;
}

std::vector<shared_ptr<Type>> GenericConstraintChecker::substituteInTypeList(
    const std::vector<shared_ptr<Type>>& types,
    const std::unordered_map<String, shared_ptr<Type>>& substitutions) {
    
    std::vector<shared_ptr<Type>> result;
    bool changed = false;
    
    for (const auto& type : types) {
        auto substituted = substituteInType(type, substitutions);
        result.push_back(substituted);
        if (substituted != type) {
            changed = true;
        }
    }
    
    return result;
}

} // namespace tsc