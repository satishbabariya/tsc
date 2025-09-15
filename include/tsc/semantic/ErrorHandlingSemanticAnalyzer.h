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
// ERROR HANDLING SEMANTIC ANALYZER
// =============================================================================

class ErrorHandlingSemanticAnalyzer {
private:
    SemanticAnalyzer* analyzer_;
    
    // Try-catch-finally analysis methods
    bool analyzeTryStatement(TryStatement* tryStmt);
    bool analyzeTryBlock(ASTNode* tryBlock);
    bool analyzeCatchClauses(const std::vector<CatchClause*>& catchClauses);
    bool analyzeCatchClause(CatchClause* catchClause);
    bool analyzeCatchParameter(CatchClause* catchClause);
    bool analyzeCatchBody(ASTNode* catchBody);
    bool analyzeFinallyBlock(ASTNode* finallyBlock);
    bool analyzeBlockStatement(BlockStatement* block);
    bool analyzeStatement(ASTNode* statement);
    
    // Throw statement analysis methods
    bool analyzeThrowStatement(ThrowStatement* throwStmt);
    bool analyzeThrowExpression(ASTNode* expression);
    
    // Custom exception analysis methods
    bool analyzeCustomExceptionClass(CustomExceptionClass* exceptionClass);
    bool analyzeExceptionClassName(const std::string& name);
    bool analyzeBaseExceptions(const std::vector<ASTNode*>& baseExceptions);
    bool analyzeBaseException(ASTNode* baseException);
    bool analyzeExceptionType(const std::string& typeName);
    bool analyzeExceptionClassMembers(CustomExceptionClass* exceptionClass);
    bool analyzeExceptionClassMember(ASTNode* member);
    bool analyzeExceptionMethod(MethodDeclaration* methodDecl);
    bool analyzeExceptionProperty(PropertyDeclaration* propertyDecl);
    bool analyzeExceptionConstructor(ConstructorDeclaration* constructorDecl);
    
    // Panic/abort analysis methods
    bool analyzePanicStatement(PanicStatement* panicStmt);
    bool analyzePanicExpression(ASTNode* expression);
    bool analyzeAbortStatement(AbortStatement* abortStmt);
    bool analyzeAbortExpression(ASTNode* expression);
    
    // Error propagation analysis methods
    bool analyzeErrorPropagationExpression(ErrorPropagationExpression* errorExpr);
    bool analyzeErrorExpression(ASTNode* expression);
    
    // Semantic checking methods
    bool performSemanticChecks(TryStatement* tryStmt);
    bool checkForPotentialIssues(TryStatement* tryStmt);
    bool checkBestPractices(TryStatement* tryStmt);
    bool checkCatchClauseConflicts(const std::vector<CatchClause*>& catchClauses);
    
    // Symbol table integration
    bool addExceptionClassToSymbolTable(CustomExceptionClass* exceptionClass);
    
    // Helper methods
    bool isValidIdentifierName(const std::string& name);
    bool followsExceptionNamingConvention(const std::string& name);
    bool isValidExceptionType(const std::string& typeName);
    bool analyzeParameter(Parameter* parameter);
    bool analyzeParameterType(Type* type);
    bool isValidType(Type* type);
    
    // Issue detection methods
    bool hasEmptyCatchBlocks(TryStatement* tryStmt);
    bool hasUnreachableCode(TryStatement* tryStmt);
    bool hasResourceLeaks(TryStatement* tryStmt);
    bool hasProperExceptionHandling(TryStatement* tryStmt);
    bool hasProperResourceManagement(TryStatement* tryStmt);
    
public:
    ErrorHandlingSemanticAnalyzer(SemanticAnalyzer* analyzer);
    
    // Main analysis methods
    bool analyzeTryStatement(TryStatement* tryStmt);
    bool analyzeThrowStatement(ThrowStatement* throwStmt);
    bool analyzeCustomExceptionClass(CustomExceptionClass* exceptionClass);
    bool analyzePanicStatement(PanicStatement* panicStmt);
    bool analyzeAbortStatement(AbortStatement* abortStmt);
    bool analyzeErrorPropagationExpression(ErrorPropagationExpression* errorExpr);
};

} // namespace semantic
} // namespace tsc