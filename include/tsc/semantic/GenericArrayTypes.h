#pragma once

#include "tsc/ast/AST.h"
#include <vector>
#include <string>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// GENERIC ARRAY TYPES ANALYZER
// =============================================================================

class GenericArrayTypesAnalyzer {
private:
    SemanticAnalyzer* analyzer_;
    
    // Array expression and type analysis
    bool analyzeArrayExpression(ArrayExpression* arrayExpr);
    bool analyzeArrayType(ArrayType* arrayType);
    bool analyzeGenericType(GenericType* genericType);
    bool analyzeArrayMethodCall(CallExpression* callExpr);
    
    // Array-like generic type analysis
    bool analyzeArrayLikeGenericType(GenericType* genericType);
    bool analyzeArrayGenericType(GenericType* genericType);
    bool analyzePotentialArrayGenericType(GenericType* genericType);
    
    // Array method call analysis
    bool analyzeArrayMethodCallWithType(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeSpecificArrayMethod(CallExpression* callExpr, ArrayType* arrayType, const std::string& methodName);
    
    // Specific array method analysis
    bool analyzeArrayPushMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayPopMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayShiftMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayUnshiftMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArraySliceMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArraySpliceMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayConcatMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayMapMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayFilterMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayReduceMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayForEachMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayFindMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArraySomeMethod(CallExpression* callExpr, ArrayType* arrayType);
    bool analyzeArrayEveryMethod(CallExpression* callExpr, ArrayType* arrayType);
    
    // Type inference methods
    Type* inferArrayElementType(ArrayExpression* arrayExpr);
    Type* inferTypeFromExpression(ASTNode* expr);
    Type* inferTypeFromLiteral(Literal* literal);
    Type* inferTypeFromIdentifier(Identifier* identifier);
    Type* inferTypeFromCallExpression(CallExpression* callExpr);
    
    // Validation methods
    bool validateGenericArrayElementType(GenericType* genericType);
    bool validateArrayElementType(Type* elementType);
    bool isArrayLikeGenericType(GenericType* genericType);
    bool couldBeUsedAsArray(GenericType* genericType);
    bool isArrayMethodCall(CallExpression* callExpr);
    bool isArrayMethod(const std::string& methodName);
    
    // Utility methods
    ArrayType* getArrayType(ASTNode* arrayObject);
    bool isCompatibleWithElementType(ASTNode* arg, Type* elementType);
    bool areTypesCompatible(Type* type1, Type* type2);
    bool isFunctionType(ASTNode* node);
    Type* getFunctionReturnType(ASTNode* function);
    
    // Type creation methods
    Type* createType(const std::string& name);
    ArrayType* createArrayType(Type* elementType);
    UnionType* createUnionType(const std::vector<Type*>& types);
    
public:
    GenericArrayTypesAnalyzer(SemanticAnalyzer* analyzer);
    
    // Main analysis method
    bool analyzeGenericArrayTypes(ASTNode* node);
};

} // namespace semantic
} // namespace tsc