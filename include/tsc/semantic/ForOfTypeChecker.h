#pragma once

#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// FOR...OF TYPE CHECKER
// =============================================================================

class ForOfTypeChecker {
private:
    SemanticAnalyzer* analyzer_;
    
    // Main checking methods
    bool checkVariableDeclaration(VariableDeclaration* variable);
    bool checkVariableType(Type* type);
    bool checkIterableExpression(Expression* iterable);
    bool checkBodyStatement(Statement* body);
    bool checkTypeCompatibility(ForOfStatement* forOfStmt);
    
    // Type inference methods
    Type* getExpressionType(Expression* expr);
    Type* getIdentifierType(Identifier* identifier);
    Type* getArrayExpressionType(ArrayExpression* arrayExpr);
    Type* getCallExpressionType(CallExpression* callExpr);
    Type* getMemberExpressionType(MemberExpression* memberExpr);
    Type* getPropertyType(Type* objectType, const std::string& propertyName);
    
    // Iterable type analysis
    Type* getIterableElementType(Expression* iterable);
    Type* getElementTypeFromIterable(Type* iterableType);
    Type* getElementTypeFromGenericIterable(GenericType* genericType);
    Type* getElementTypeFromKnownIterable(Type* iterableType);
    
    // Type checking methods
    bool isIterableType(Type* type);
    bool isGenericIterableType(GenericType* genericType);
    bool isKnownIterableType(Type* type);
    bool isAsyncIterableType(Type* type);
    bool isValidIterationType(Type* type);
    
    // Type compatibility methods
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
    Type* createIteratorType(Type* elementType);
    Type* createTupleType(const std::vector<Type*>& types);
    
public:
    ForOfTypeChecker(SemanticAnalyzer* analyzer);
    
    // Main type checking method
    bool checkForOfStatement(ForOfStatement* forOfStmt);
};

} // namespace semantic
} // namespace tsc