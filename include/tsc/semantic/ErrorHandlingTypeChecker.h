#pragma once

#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include <string>
#include <vector>
#include <set>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// ERROR HANDLING TYPE CHECKER
// =============================================================================

class ErrorHandlingTypeChecker {
private:
    SemanticAnalyzer* analyzer_;
    
    // Try-catch-finally checking methods
    bool checkTryStatement(TryStatement* tryStmt);
    bool checkTryBlock(ASTNode* tryBlock);
    bool checkCatchClauses(const std::vector<CatchClause*>& catchClauses);
    bool checkCatchClause(CatchClause* catchClause);
    bool checkCatchParameter(CatchClause* catchClause);
    bool checkCatchParameterType(Type* parameterType);
    bool checkCatchBody(ASTNode* catchBody);
    bool checkFinallyBlock(ASTNode* finallyBlock);
    bool checkCatchClauseConflicts(const std::vector<CatchClause*>& catchClauses);
    bool areCatchClausesConflicting(CatchClause* catch1, CatchClause* catch2);
    
    // Throw statement checking methods
    bool checkThrowStatement(ThrowStatement* throwStmt);
    bool checkThrowExpression(ASTNode* expression);
    
    // Custom exception checking methods
    bool checkCustomExceptionClass(CustomExceptionClass* exceptionClass);
    bool checkBaseExceptions(const std::vector<ASTNode*>& baseExceptions);
    bool checkBaseException(ASTNode* baseException);
    bool checkExceptionClassMembers(CustomExceptionClass* exceptionClass);
    bool checkExceptionClassMember(ASTNode* member);
    bool checkExceptionMethod(MethodDeclaration* methodDecl);
    bool checkExceptionProperty(PropertyDeclaration* propertyDecl);
    bool checkExceptionConstructor(ConstructorDeclaration* constructorDecl);
    
    // Panic/abort checking methods
    bool checkPanicStatement(PanicStatement* panicStmt);
    bool checkPanicExpression(ASTNode* expression);
    bool checkAbortStatement(AbortStatement* abortStmt);
    bool checkAbortExpression(ASTNode* expression);
    
    // Error propagation checking methods
    bool checkErrorPropagationExpression(ErrorPropagationExpression* errorExpr);
    bool checkErrorExpression(ASTNode* expression);
    
    // Helper methods
    bool checkBlockStatement(BlockStatement* block);
    bool checkStatement(ASTNode* statement);
    bool checkParameter(Parameter* parameter);
    bool checkMethodReturnType(Type* returnType);
    bool checkPropertyType(Type* propertyType);
    
    // Type inference methods
    Type* getExpressionType(Expression* expr);
    Type* getIdentifierType(Identifier* identifier);
    Type* getCallExpressionReturnType(CallExpression* callExpr);
    
    // Type validation methods
    bool isValidExceptionType(Type* type);
    bool isValidThrowableType(Type* type);
    bool isValidPanicType(Type* type);
    bool isValidAbortType(Type* type);
    bool isValidErrorType(Type* type);
    bool isValidType(Type* type);
    bool areTypesIdentical(Type* type1, Type* type2);
    
public:
    ErrorHandlingTypeChecker(SemanticAnalyzer* analyzer);
    
    // Main type checking methods
    bool checkTryStatement(TryStatement* tryStmt);
    bool checkThrowStatement(ThrowStatement* throwStmt);
    bool checkCustomExceptionClass(CustomExceptionClass* exceptionClass);
    bool checkPanicStatement(PanicStatement* panicStmt);
    bool checkAbortStatement(AbortStatement* abortStmt);
    bool checkErrorPropagationExpression(ErrorPropagationExpression* errorExpr);
};

} // namespace semantic
} // namespace tsc