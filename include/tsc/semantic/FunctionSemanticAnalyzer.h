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
// FUNCTION SEMANTIC ANALYZER
// =============================================================================

class FunctionSemanticAnalyzer {
private:
    SemanticAnalyzer* analyzer_;
    
    // Main analysis methods
    bool analyzeFunctionName(const std::string& name);
    bool analyzeFunctionOverloads(const std::vector<FunctionOverloadDeclaration*>& overloads);
    bool analyzeFunctionOverload(FunctionOverloadDeclaration* overload);
    bool analyzeFunctionParameters(const std::vector<ASTNode*>& parameters);
    bool analyzeParameter(Parameter* parameter);
    bool analyzeParameterDefaultValue(Parameter* parameter);
    bool analyzeRestParameter(Parameter* parameter);
    
    // Function body analysis methods
    bool analyzeFunctionBody(ASTNode* body);
    bool analyzeBlockStatement(BlockStatement* block);
    bool analyzeExpression(Expression* expression);
    bool analyzeType(Type* type);
    
    // Semantic checking methods
    bool performSemanticChecks(FunctionDeclaration* functionDecl);
    bool checkForPotentialIssues(FunctionDeclaration* functionDecl);
    bool checkPerformanceImplications(FunctionDeclaration* functionDecl);
    bool checkBestPractices(FunctionDeclaration* functionDecl);
    
    // Overload conflict checking methods
    bool checkOverloadConflicts(const std::vector<FunctionOverloadDeclaration*>& overloads);
    bool areOverloadSignaturesIdentical(FunctionOverloadDeclaration* overload1, 
                                       FunctionOverloadDeclaration* overload2);
    bool areParameterTypesIdentical(ASTNode* param1, ASTNode* param2);
    
    // Symbol table integration
    bool addFunctionToSymbolTable(FunctionDeclaration* functionDecl);
    
    // Helper methods
    bool isValidIdentifierName(const std::string& name);
    bool isReservedKeyword(const std::string& name);
    bool isValidType(Type* type);
    bool isArrayLikeType(Type* type);
    bool isGenericArrayType(GenericType* genericType);
    bool isKnownArrayType(Type* type);
    bool areTypesIdentical(Type* type1, Type* type2);
    
    // Issue detection methods
    bool hasUnusedParameters(FunctionDeclaration* functionDecl);
    bool hasMissingReturnStatements(FunctionDeclaration* functionDecl);
    bool hasPotentialInfiniteRecursion(FunctionDeclaration* functionDecl);
    bool hasExpensiveOperations(FunctionDeclaration* functionDecl);
    bool hasLargeParameterList(FunctionDeclaration* functionDecl);
    bool hasComplexOverloads(FunctionDeclaration* functionDecl);
    bool hasConsistentNaming(FunctionDeclaration* functionDecl);
    bool hasAppropriateParameterOrder(FunctionDeclaration* functionDecl);
    bool hasProperRestParameterUsage(FunctionDeclaration* functionDecl);
    
public:
    FunctionSemanticAnalyzer(SemanticAnalyzer* analyzer);
    
    // Main analysis method
    bool analyzeFunctionDeclaration(FunctionDeclaration* functionDecl);
};

} // namespace semantic
} // namespace tsc