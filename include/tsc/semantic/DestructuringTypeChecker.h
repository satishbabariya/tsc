#pragma once

#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// DESTRUCTURING TYPE CHECKER
// =============================================================================

class DestructuringTypeChecker {
private:
    SemanticAnalyzer* analyzer_;
    
    // Main checking methods
    bool checkArrayDestructuringPattern(ArrayDestructuringPattern* pattern, Type* rhsType);
    bool checkObjectDestructuringPattern(ObjectDestructuringPattern* pattern, Type* rhsType);
    
    // Element and property checking methods
    bool checkArrayDestructuringElement(ASTNode* element, Type* elementType, size_t index);
    bool checkObjectDestructuringProperty(ASTNode* property, Type* objectType);
    bool checkDestructuringProperty(DestructuringProperty* property, Type* objectType);
    bool checkDestructuringValue(ASTNode* value, Type* propertyType);
    
    // Specific pattern checking methods
    bool checkIdentifierDestructuring(Identifier* identifier, Type* expectedType);
    bool checkNestedArrayDestructuring(ArrayDestructuringPattern* pattern, Type* elementType);
    bool checkNestedObjectDestructuring(ObjectDestructuringPattern* pattern, Type* objectType);
    bool checkAssignmentPattern(AssignmentPattern* pattern, Type* expectedType);
    
    // Rest element checking methods
    bool checkArrayRestElement(ASTNode* restElement, Type* arrayType);
    bool checkObjectRestElement(ASTNode* restElement, Type* objectType);
    
    // Type inference methods
    Type* getExpressionType(Expression* expr);
    Type* getIdentifierType(Identifier* identifier);
    Type* getArrayExpressionType(ArrayExpression* arrayExpr);
    Type* getObjectLiteralType(ObjectLiteral* objectLiteral);
    Type* getCallExpressionType(CallExpression* callExpr);
    Type* getMemberExpressionType(MemberExpression* memberExpr);
    Type* getObjectPropertyType(Type* objectType, ASTNode* key);
    
    // Type checking helper methods
    bool isArrayLikeType(Type* type);
    bool isGenericArrayType(GenericType* genericType);
    bool isKnownArrayType(Type* type);
    bool isObjectLikeType(Type* type);
    bool isGenericObjectType(GenericType* genericType);
    bool isKnownObjectType(Type* type);
    
    // Type analysis methods
    Type* getArrayElementType(Type* arrayType);
    Type* getGenericArrayElementType(GenericType* genericType);
    Type* getKnownArrayElementType(Type* arrayType);
    
    // Validation methods
    bool isReservedKeyword(const std::string& name);
    bool isValidDestructuringType(Type* type);
    bool areTypesCompatible(Type* type1, Type* type2);
    bool isStructurallyCompatible(Type* type1, Type* type2);
    bool isInheritanceCompatible(Type* type1, Type* type2);
    
    // Utility methods
    std::string getCalleeName(ASTNode* callee);
    std::string getMemberExpressionName(MemberExpression* memberExpr);
    Type* getFunctionReturnType(Type* functionType);
    
    // Type creation helper methods
    Type* createType(const std::string& name);
    ArrayType* createArrayType(Type* elementType);
    
public:
    DestructuringTypeChecker(SemanticAnalyzer* analyzer);
    
    // Main type checking method
    bool checkDestructuringAssignment(DestructuringPattern* pattern, Expression* rightHandSide);
};

} // namespace semantic
} // namespace tsc