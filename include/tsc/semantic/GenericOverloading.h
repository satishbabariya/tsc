#pragma once

#include "tsc/ast/AST.h"
#include <vector>
#include <string>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// GENERIC OVERLOADING ANALYZER
// =============================================================================

class GenericOverloadingAnalyzer {
private:
    SemanticAnalyzer* analyzer_;
    
    // Overload retrieval methods
    std::vector<ASTNode*> getMethodOverloads(const std::string& methodName);
    std::vector<ASTNode*> getFunctionOverloads(const std::string& functionName);
    std::vector<ASTNode*> getAllOverloads(const std::string& name);
    
    // Overload validation methods
    bool validateOverloadSignatures(const std::vector<ASTNode*>& overloads);
    bool areSignaturesIdentical(ASTNode* overload1, ASTNode* overload2);
    bool areParameterTypesIdentical(Parameter* param1, Parameter* param2);
    bool areTypesEqual(Type* type1, Type* type2);
    
    // Ambiguity checking methods
    bool checkOverloadAmbiguity(const std::vector<ASTNode*>& overloads);
    bool areOverloadsAmbiguous(ASTNode* overload1, ASTNode* overload2);
    bool isMoreSpecific(ASTNode* overload1, ASTNode* overload2);
    bool isParameterMoreSpecific(Parameter* param1, Parameter* param2);
    bool isTypeMoreSpecific(Type* type1, Type* type2);
    bool haveSameSpecificity(ASTNode* overload1, ASTNode* overload2);
    bool haveSameParameterSpecificity(Parameter* param1, Parameter* param2);
    
    // Overload resolution methods
    ASTNode* resolveBestOverload(const std::vector<ASTNode*>& overloads, CallExpression* callExpr);
    int calculateOverloadScore(ASTNode* overload, CallExpression* callExpr);
    bool isTypeCompatible(ASTNode* arg, Type* paramType);
    int countOptionalParameters(const std::vector<Parameter*>& params);
    
    // Utility methods
    std::vector<Parameter*> getParameters(ASTNode* node);
    std::string getCalleeName(ASTNode* callee);
    std::string getMemberExpressionName(MemberExpression* memberExpr);
    
    // Type argument validation
    bool validateTypeArguments(ASTNode* overload, const std::vector<Type*>& typeArguments);
    bool isGeneric(ASTNode* node);
    std::vector<TypeParameter*> getTypeParameters(ASTNode* node);
    bool validateTypeArgument(Type* typeArg, TypeParameter* typeParam);
    
public:
    GenericOverloadingAnalyzer(SemanticAnalyzer* analyzer);
    
    // Main analysis methods
    bool analyzeGenericOverloading(ASTNode* node);
    bool analyzeMethodOverloading(MethodDeclaration* method);
    bool analyzeFunctionOverloading(FunctionDeclaration* function);
    bool analyzeGenericCallResolution(CallExpression* callExpr);
};

} // namespace semantic
} // namespace tsc