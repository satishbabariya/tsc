#pragma once

#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include <string>
#include <vector>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// FOR...OF SEMANTIC ANALYZER
// =============================================================================

class ForOfSemanticAnalyzer {
private:
    SemanticAnalyzer* analyzer_;
    
    // Main analysis methods
    bool analyzeVariableDeclaration(VariableDeclaration* variable);
    bool analyzeVariableType(Type* type);
    bool analyzeIterableExpression(Expression* iterable);
    bool analyzeBodyStatement(Statement* body);
    bool performSemanticChecks(ForOfStatement* forOfStmt);
    
    // Semantic checking methods
    bool checkVariableUsageInBody(ForOfStatement* forOfStmt);
    bool checkForPotentialIssues(ForOfStatement* forOfStmt);
    bool checkPerformanceImplications(ForOfStatement* forOfStmt);
    
    // Variable validation methods
    bool isValidVariableName(const std::string& name);
    bool isReservedKeyword(const std::string& name);
    bool isValidIterationType(Type* type);
    bool addVariableToSymbolTable(VariableDeclaration* variable);
    
    // Iterable analysis methods
    bool isExpressionIterable(Expression* expr);
    bool isExpressionAsyncIterable(Expression* expr);
    bool isIterableType(Type* type);
    bool isGenericIterableType(GenericType* genericType);
    bool isKnownIterableType(Type* type);
    bool isAsyncIterableType(Type* type);
    
    // Issue detection methods
    bool isVariableUsedInStatement(Statement* stmt, const std::string& variableName);
    bool isVariableReassignedInStatement(Statement* stmt, const std::string& variableName);
    bool isPotentiallyInfiniteLoop(Expression* iterable);
    bool isPotentiallyEmptyIterable(Expression* iterable);
    bool hasSideEffects(Expression* expr);
    bool isExpensiveIterableExpression(Expression* expr);
    bool isPotentiallyLargeIterable(Expression* expr);
    
    // Type inference methods
    Type* getExpressionType(Expression* expr);
    Type* getIdentifierType(Identifier* identifier);
    Type* getArrayExpressionType(ArrayExpression* arrayExpr);
    Type* getCallExpressionType(CallExpression* callExpr);
    Type* getMemberExpressionType(MemberExpression* memberExpr);
    Type* getPropertyType(Type* objectType, const std::string& propertyName);
    
    // Utility methods
    std::string getCalleeName(ASTNode* callee);
    std::string getMemberExpressionName(MemberExpression* memberExpr);
    Type* getFunctionReturnType(Type* functionType);
    
    // Type creation helper methods
    Type* createType(const std::string& name);
    ArrayType* createArrayType(Type* elementType);
    Type* createIteratorType(Type* elementType);
    
public:
    ForOfSemanticAnalyzer(SemanticAnalyzer* analyzer);
    
    // Main analysis method
    bool analyzeForOfStatement(ForOfStatement* forOfStmt);
};

} // namespace semantic
} // namespace tsc