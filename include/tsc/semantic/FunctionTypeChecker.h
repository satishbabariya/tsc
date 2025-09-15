#pragma once

#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// FUNCTION TYPE CHECKER
// =============================================================================

class FunctionTypeChecker {
private:
    SemanticAnalyzer* analyzer_;
    
    // Main checking methods
    bool checkFunctionOverloads(const std::vector<FunctionOverloadDeclaration*>& overloads);
    bool checkFunctionOverload(FunctionOverloadDeclaration* overload);
    bool checkFunctionParameters(const std::vector<ASTNode*>& parameters);
    bool checkParameter(Parameter* parameter);
    bool checkParameterDefaultValue(Parameter* parameter);
    bool checkRestParameterType(Parameter* parameter);
    
    // Function body checking methods
    bool checkFunctionBody(ASTNode* body, Type* returnType);
    bool checkBlockStatement(BlockStatement* block, Type* returnType);
    bool checkReturnStatement(ReturnStatement* returnStmt, Type* returnType);
    bool checkExpressionReturnType(Expression* expression, Type* returnType);
    
    // Consistency checking methods
    bool checkReturnTypeConsistency(FunctionDeclaration* functionDecl);
    bool checkOverloadConflicts(const std::vector<FunctionOverloadDeclaration*>& overloads);
    bool areOverloadSignaturesIdentical(FunctionOverloadDeclaration* overload1, 
                                       FunctionOverloadDeclaration* overload2);
    bool areParameterTypesIdentical(ASTNode* param1, ASTNode* param2);
    
    // Arrow function and call expression checking
    bool checkArrowFunctionExpression(ArrowFunctionExpression* arrowFunc);
    bool checkCallExpression(CallExpression* callExpr);
    bool checkCallArguments(const std::vector<ASTNode*>& arguments, FunctionType* functionType);
    
    // Type inference methods
    Type* getExpressionType(Expression* expr);
    Type* getIdentifierType(Identifier* identifier);
    Type* getCallExpressionReturnType(CallExpression* callExpr);
    Type* getArrowFunctionType(ArrowFunctionExpression* arrowFunc);
    
    // Type checking helper methods
    bool isValidType(Type* type);
    bool isArrayLikeType(Type* type);
    bool isGenericArrayType(GenericType* genericType);
    bool isKnownArrayType(Type* type);
    
    // Type compatibility methods
    bool areTypesCompatible(Type* type1, Type* type2);
    bool areTypesIdentical(Type* type1, Type* type2);
    bool isStructurallyCompatible(Type* type1, Type* type2);
    bool isInheritanceCompatible(Type* type1, Type* type2);
    
public:
    FunctionTypeChecker(SemanticAnalyzer* analyzer);
    
    // Main type checking method
    bool checkFunctionDeclaration(FunctionDeclaration* functionDecl);
};

} // namespace semantic
} // namespace tsc