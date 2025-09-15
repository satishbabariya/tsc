#pragma once

#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// ENHANCED GENERICS TYPE CHECKER
// =============================================================================

class EnhancedGenericsTypeChecker {
private:
    SemanticAnalyzer* analyzer_;
    
    // Main checking methods
    bool checkGenericConstraints(const std::vector<TypeParameter*>& typeParams);
    bool checkTypeParameterConstraints(TypeParameter* typeParam);
    bool checkConstraintValidity(Type* constraint);
    bool checkComplexConstraint(Type* constraint);
    bool checkDefaultTypeCompatibility(TypeParameter* typeParam);
    
    // Constraint checking methods
    bool checkIntersectionConstraint(IntersectionType* intersectionType);
    bool checkUnionConstraint(UnionType* unionType);
    bool checkGenericConstraint(GenericType* genericType);
    
    // Inference checking methods
    bool checkGenericInference(GenericDeclaration* genericDecl);
    bool checkTypeParameterInference(const std::vector<TypeParameter*>& typeParams);
    bool checkSingleTypeParameterInference(TypeParameter* typeParam);
    bool checkComplexInferenceScenario(TypeParameter* typeParam);
    bool checkConstraintBasedInference(TypeParameter* typeParam);
    bool checkGenericMethodInference(MethodDeclaration* methodDecl);
    bool checkParameterInference(Parameter* parameter);
    bool checkReturnTypeInference(MethodDeclaration* methodDecl);
    
    // Overload checking methods
    bool checkGenericOverloads(GenericDeclaration* genericDecl);
    bool checkMethodOverloads(const std::vector<MethodOverloadDeclaration*>& overloads);
    bool checkMethodOverload(MethodOverloadDeclaration* overload);
    bool checkOverloadConflicts(const std::vector<MethodOverloadDeclaration*>& overloads);
    bool areOverloadSignaturesIdentical(MethodOverloadDeclaration* overload1, 
                                       MethodOverloadDeclaration* overload2);
    bool areParameterTypesIdentical(ASTNode* param1, ASTNode* param2);
    
    // Helper methods
    bool isValidConstraintType(Type* type);
    bool isComplexConstraint(Type* type);
    bool canInferTypeParameter(TypeParameter* typeParam);
    bool isComplexInferenceScenario(TypeParameter* typeParam);
    bool constraintProvidesInferenceInfo(Type* constraint);
    bool canInferParameterType(Parameter* parameter);
    bool canInferReturnType(MethodDeclaration* methodDecl);
    bool areTypesCompatible(Type* type1, Type* type2);
    bool areTypesIdentical(Type* type1, Type* type2);
    bool isStructurallyCompatible(Type* type1, Type* type2);
    
public:
    EnhancedGenericsTypeChecker(SemanticAnalyzer* analyzer);
    
    // Main type checking method
    bool checkGenericDeclaration(GenericDeclaration* genericDecl);
};

} // namespace semantic
} // namespace tsc