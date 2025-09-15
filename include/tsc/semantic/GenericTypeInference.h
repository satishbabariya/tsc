#pragma once

#include "tsc/ast/AST.h"
#include <vector>
#include <string>
#include <map>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// TYPE INFERENCE CONTEXT
// =============================================================================

class TypeInferenceContext {
private:
    std::map<std::string, Type*> inferredTypes_;
    
public:
    TypeInferenceContext();
    
    void setInferredType(const std::string& typeParamName, Type* type);
    Type* getInferredType(const std::string& typeParamName);
    bool hasInferredType(const std::string& typeParamName);
    void clear();
};

// =============================================================================
// GENERIC TYPE INFERENCE
// =============================================================================

class GenericTypeInference {
private:
    SemanticAnalyzer* analyzer_;
    
    // Type inference from call expressions
    std::vector<Type*> inferTypeArgumentsFromCall(CallExpression* callExpr, const std::vector<TypeParameter*>& typeParams);
    bool inferTypeFromParameterArgument(Parameter* param, ASTNode* arg, TypeInferenceContext& context);
    
    // Type inference from complex types
    bool inferTypeFromComplexType(Type* paramType, ASTNode* arg, TypeInferenceContext& context);
    bool inferTypeFromArrayType(ArrayType* arrayType, ASTNode* arg, TypeInferenceContext& context);
    bool inferTypeFromUnionType(UnionType* unionType, ASTNode* arg, TypeInferenceContext& context);
    bool inferTypeFromIntersectionType(IntersectionType* intersectionType, ASTNode* arg, TypeInferenceContext& context);
    bool inferTypeFromGenericType(GenericType* genericType, ASTNode* arg, TypeInferenceContext& context);
    
    // Type inference from arguments
    Type* inferTypeFromArgument(ASTNode* arg);
    Type* inferTypeFromLiteral(Literal* literal);
    Type* inferTypeFromIdentifier(Identifier* identifier);
    Type* inferTypeFromArrayExpression(ArrayExpression* arrayExpr);
    Type* inferTypeFromObjectExpression(ObjectExpression* objectExpr);
    Type* inferTypeFromCallExpression(CallExpression* callExpr);
    
    // Utility methods
    Type* getReturnType(Type* functionType);
    Type* createType(const std::string& name);
    Type* createArrayType(Type* elementType);
    std::string getCalleeName(ASTNode* callee);
    std::string getMemberExpressionName(MemberExpression* memberExpr);
    ASTNode* getGenericFunction(const std::string& name);
    bool isGeneric(ASTNode* node);
    std::vector<TypeParameter*> getTypeParameters(ASTNode* node);
    std::vector<Parameter*> getParameters(CallExpression* callExpr);
    
public:
    GenericTypeInference(SemanticAnalyzer* analyzer);
    
    // Main inference method
    std::vector<Type*> inferTypeArguments(CallExpression* callExpr);
};

} // namespace semantic
} // namespace tsc