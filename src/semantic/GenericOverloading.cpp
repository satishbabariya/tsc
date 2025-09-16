#include "tsc/semantic/GenericOverloading.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/ast/AST.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

namespace tsc {
namespace semantic {

// =============================================================================
// GENERIC OVERLOADING IMPLEMENTATION
// =============================================================================

GenericOverloadingAnalyzer::GenericOverloadingAnalyzer(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool GenericOverloadingAnalyzer::analyzeGenericOverloading(ASTNode* node) {
    if (!node) return true;
    
    switch (node->getType()) {
        case ASTNodeType::MethodDeclaration:
            return analyzeMethodOverloading(static_cast<MethodDeclaration*>(node));
        case ASTNodeType::FunctionDeclaration:
            return analyzeFunctionOverloading(static_cast<FunctionDeclaration*>(node));
        case ASTNodeType::CallExpression:
            return analyzeGenericCallResolution(static_cast<CallExpression*>(node));
        default:
            return true;
    }
}

bool GenericOverloadingAnalyzer::analyzeMethodOverloading(MethodDeclaration* method) {
    std::cout << "Analyzing method overloading for: " << method->getName() << std::endl;
    
    // Check if method is generic
    if (!method->isGeneric()) {
        return true; // Not a generic method, no overloading analysis needed
    }
    
    // Get all overloads for this method
    auto overloads = getMethodOverloads(method->getName());
    
    // Validate overload signatures
    if (!validateOverloadSignatures(overloads)) {
        std::cerr << "Invalid overload signatures for method: " << method->getName() << std::endl;
        return false;
    }
    
    // Check for ambiguous overloads
    if (!checkOverloadAmbiguity(overloads)) {
        std::cerr << "Ambiguous overloads found for method: " << method->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::analyzeFunctionOverloading(FunctionDeclaration* function) {
    std::cout << "Analyzing function overloading for: " << function->getName() << std::endl;
    
    // Check if function is generic
    if (!function->isGeneric()) {
        return true; // Not a generic function, no overloading analysis needed
    }
    
    // Get all overloads for this function
    auto overloads = getFunctionOverloads(function->getName());
    
    // Validate overload signatures
    if (!validateOverloadSignatures(overloads)) {
        std::cerr << "Invalid overload signatures for function: " << function->getName() << std::endl;
        return false;
    }
    
    // Check for ambiguous overloads
    if (!checkOverloadAmbiguity(overloads)) {
        std::cerr << "Ambiguous overloads found for function: " << function->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::analyzeGenericCallResolution(CallExpression* callExpr) {
    std::cout << "Analyzing generic call resolution" << std::endl;
    
    // Get the function/method being called
    auto callee = callExpr->getCallee();
    if (!callee) {
        std::cerr << "No callee in call expression" << std::endl;
        return false;
    }
    
    // Get function/method name
    std::string name = getCalleeName(callee);
    if (name.empty()) {
        std::cerr << "Could not determine callee name" << std::endl;
        return false;
    }
    
    // Get all overloads
    auto overloads = getAllOverloads(name);
    
    // Resolve the best matching overload
    auto bestOverload = resolveBestOverload(overloads, callExpr);
    if (!bestOverload) {
        std::cerr << "No matching overload found for call: " << name << std::endl;
        return false;
    }
    
    // Validate type arguments if provided
    if (callExpr->hasTypeArguments()) {
        if (!validateTypeArguments(bestOverload, callExpr->getTypeArguments())) {
            return false;
        }
    }
    
    return true;
}

std::vector<ASTNode*> GenericOverloadingAnalyzer::getMethodOverloads(const std::string& methodName) {
    std::vector<ASTNode*> overloads;
    
    // Look up method overloads in symbol table
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbols = symbolTable->lookupSymbols(methodName);
    
    for (auto& symbol : symbols) {
        if (symbol->getType() == SymbolType::Method) {
            overloads.push_back(symbol->getNode());
        }
    }
    
    return overloads;
}

std::vector<ASTNode*> GenericOverloadingAnalyzer::getFunctionOverloads(const std::string& functionName) {
    std::vector<ASTNode*> overloads;
    
    // Look up function overloads in symbol table
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbols = symbolTable->lookupSymbols(functionName);
    
    for (auto& symbol : symbols) {
        if (symbol->getType() == SymbolType::Function) {
            overloads.push_back(symbol->getNode());
        }
    }
    
    return overloads;
}

std::vector<ASTNode*> GenericOverloadingAnalyzer::getAllOverloads(const std::string& name) {
    std::vector<ASTNode*> overloads;
    
    // Get both method and function overloads
    auto methodOverloads = getMethodOverloads(name);
    auto functionOverloads = getFunctionOverloads(name);
    
    overloads.insert(overloads.end(), methodOverloads.begin(), methodOverloads.end());
    overloads.insert(overloads.end(), functionOverloads.begin(), functionOverloads.end());
    
    return overloads;
}

bool GenericOverloadingAnalyzer::validateOverloadSignatures(const std::vector<ASTNode*>& overloads) {
    if (overloads.size() <= 1) {
        return true; // No overloading or single overload
    }
    
    // Check that all overloads have different signatures
    for (size_t i = 0; i < overloads.size(); ++i) {
        for (size_t j = i + 1; j < overloads.size(); ++j) {
            if (areSignaturesIdentical(overloads[i], overloads[j])) {
                std::cerr << "Duplicate overload signatures found" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::areSignaturesIdentical(ASTNode* overload1, ASTNode* overload2) {
    // Get parameter lists
    std::vector<Parameter*> params1 = getParameters(overload1);
    std::vector<Parameter*> params2 = getParameters(overload2);
    
    // Check parameter count
    if (params1.size() != params2.size()) {
        return false;
    }
    
    // Check parameter types
    for (size_t i = 0; i < params1.size(); ++i) {
        if (!areParameterTypesIdentical(params1[i], params2[i])) {
            return false;
        }
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::areParameterTypesIdentical(Parameter* param1, Parameter* param2) {
    // Check if both parameters have types
    if (!param1->getType() && !param2->getType()) {
        return true; // Both untyped
    }
    
    if (!param1->getType() || !param2->getType()) {
        return false; // One typed, one untyped
    }
    
    // Check type equality
    return areTypesEqual(param1->getType(), param2->getType());
}

bool GenericOverloadingAnalyzer::areTypesEqual(Type* type1, Type* type2) {
    if (!type1 || !type2) {
        return type1 == type2;
    }
    
    // Check type names
    if (type1->getName() != type2->getName()) {
        return false;
    }
    
    // Check generic type arguments
    if (type1->getType() == TypeType::GenericType && type2->getType() == TypeType::GenericType) {
        auto generic1 = static_cast<GenericType*>(type1);
        auto generic2 = static_cast<GenericType*>(type2);
        
        auto args1 = generic1->getTypeArguments();
        auto args2 = generic2->getTypeArguments();
        
        if (args1.size() != args2.size()) {
            return false;
        }
        
        for (size_t i = 0; i < args1.size(); ++i) {
            if (!areTypesEqual(args1[i], args2[i])) {
                return false;
            }
        }
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::checkOverloadAmbiguity(const std::vector<ASTNode*>& overloads) {
    if (overloads.size() <= 1) {
        return true; // No ambiguity possible
    }
    
    // Check for ambiguous overloads
    for (size_t i = 0; i < overloads.size(); ++i) {
        for (size_t j = i + 1; j < overloads.size(); ++j) {
            if (areOverloadsAmbiguous(overloads[i], overloads[j])) {
                std::cerr << "Ambiguous overloads found" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::areOverloadsAmbiguous(ASTNode* overload1, ASTNode* overload2) {
    // Check if one overload is more specific than the other
    if (isMoreSpecific(overload1, overload2) || isMoreSpecific(overload2, overload1)) {
        return false; // Not ambiguous, one is more specific
    }
    
    // Check if they have the same specificity
    return haveSameSpecificity(overload1, overload2);
}

bool GenericOverloadingAnalyzer::isMoreSpecific(ASTNode* overload1, ASTNode* overload2) {
    auto params1 = getParameters(overload1);
    auto params2 = getParameters(overload2);
    
    if (params1.size() != params2.size()) {
        return false;
    }
    
    bool moreSpecific = false;
    for (size_t i = 0; i < params1.size(); ++i) {
        if (isParameterMoreSpecific(params1[i], params2[i])) {
            moreSpecific = true;
        } else if (isParameterMoreSpecific(params2[i], params1[i])) {
            return false; // params2 is more specific
        }
    }
    
    return moreSpecific;
}

bool GenericOverloadingAnalyzer::isParameterMoreSpecific(Parameter* param1, Parameter* param2) {
    // Check if param1 has a more specific type than param2
    if (!param1->getType() && param2->getType()) {
        return false; // param1 is less specific (no type)
    }
    
    if (param1->getType() && !param2->getType()) {
        return true; // param1 is more specific (has type)
    }
    
    if (!param1->getType() && !param2->getType()) {
        return false; // Both untyped, same specificity
    }
    
    // Check type specificity
    return isTypeMoreSpecific(param1->getType(), param2->getType());
}

bool GenericOverloadingAnalyzer::isTypeMoreSpecific(Type* type1, Type* type2) {
    // Simplified type specificity checking
    // In a real implementation, this would check the type hierarchy
    
    if (type1->getType() == TypeType::GenericType && type2->getType() == TypeType::GenericType) {
        auto generic1 = static_cast<GenericType*>(type1);
        auto generic2 = static_cast<GenericType*>(type2);
        
        // Check if generic1 has more specific type arguments
        auto args1 = generic1->getTypeArguments();
        auto args2 = generic2->getTypeArguments();
        
        if (args1.size() > args2.size()) {
            return true; // More type arguments = more specific
        }
    }
    
    return false;
}

bool GenericOverloadingAnalyzer::haveSameSpecificity(ASTNode* overload1, ASTNode* overload2) {
    auto params1 = getParameters(overload1);
    auto params2 = getParameters(overload2);
    
    if (params1.size() != params2.size()) {
        return false;
    }
    
    for (size_t i = 0; i < params1.size(); ++i) {
        if (!haveSameParameterSpecificity(params1[i], params2[i])) {
            return false;
        }
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::haveSameParameterSpecificity(Parameter* param1, Parameter* param2) {
    // Check if both parameters have the same type specificity
    if (!param1->getType() && !param2->getType()) {
        return true; // Both untyped
    }
    
    if (!param1->getType() || !param2->getType()) {
        return false; // One typed, one untyped
    }
    
    return areTypesEqual(param1->getType(), param2->getType());
}

ASTNode* GenericOverloadingAnalyzer::resolveBestOverload(const std::vector<ASTNode*>& overloads, CallExpression* callExpr) {
    if (overloads.empty()) {
        return nullptr;
    }
    
    if (overloads.size() == 1) {
        return overloads[0];
    }
    
    // Find the best matching overload
    ASTNode* bestOverload = nullptr;
    int bestScore = -1;
    
    for (auto& overload : overloads) {
        int score = calculateOverloadScore(overload, callExpr);
        if (score > bestScore) {
            bestScore = score;
            bestOverload = overload;
        }
    }
    
    return bestOverload;
}

int GenericOverloadingAnalyzer::calculateOverloadScore(ASTNode* overload, CallExpression* callExpr) {
    auto params = getParameters(overload);
    auto args = callExpr->getArguments();
    
    int score = 0;
    
    // Check parameter count match
    if (params.size() == args.size()) {
        score += 100; // Perfect parameter count match
    } else if (params.size() > args.size()) {
        // Check for optional parameters
        int optionalParams = countOptionalParameters(params);
        if (args.size() >= params.size() - optionalParams) {
            score += 50; // Partial match with optional parameters
        }
    } else {
        // Too many arguments
        return -1;
    }
    
    // Check type matches
    for (size_t i = 0; i < std::min(params.size(), args.size()); ++i) {
        if (params[i]->getType()) {
            if (isTypeCompatible(args[i], params[i]->getType())) {
                score += 10; // Type compatibility bonus
            }
        }
    }
    
    return score;
}

bool GenericOverloadingAnalyzer::isTypeCompatible(ASTNode* arg, Type* paramType) {
    // Simplified type compatibility checking
    // In a real implementation, this would check actual type compatibility
    
    if (arg->getType() == ASTNodeType::Literal) {
        auto literal = static_cast<Literal*>(arg);
        if (paramType->getName() == "number" && literal->getValueType() == LiteralType::Number) {
            return true;
        }
        if (paramType->getName() == "string" && literal->getValueType() == LiteralType::String) {
            return true;
        }
        if (paramType->getName() == "boolean" && literal->getValueType() == LiteralType::Boolean) {
            return true;
        }
    }
    
    return false;
}

int GenericOverloadingAnalyzer::countOptionalParameters(const std::vector<Parameter*>& params) {
    int count = 0;
    for (auto& param : params) {
        if (param->isOptional()) {
            count++;
        }
    }
    return count;
}

std::vector<Parameter*> GenericOverloadingAnalyzer::getParameters(ASTNode* node) {
    std::vector<Parameter*> params;
    
    switch (node->getType()) {
        case ASTNodeType::MethodDeclaration:
            params = static_cast<MethodDeclaration*>(node)->getParameters();
            break;
        case ASTNodeType::FunctionDeclaration:
            params = static_cast<FunctionDeclaration*>(node)->getParameters();
            break;
        default:
            break;
    }
    
    return params;
}

std::string GenericOverloadingAnalyzer::getCalleeName(ASTNode* callee) {
    switch (callee->getType()) {
        case ASTNodeType::Identifier:
            return static_cast<Identifier*>(callee)->getName();
        case ASTNodeType::MemberExpression:
            return getMemberExpressionName(static_cast<MemberExpression*>(callee));
        default:
            return " + ";
    }
}

std::string GenericOverloadingAnalyzer::getMemberExpressionName(MemberExpression* memberExpr) {
    auto property = memberExpr->getProperty();
    if (property && property->getType() == ASTNodeType::Identifier) {
        return static_cast<Identifier*>(property)->getName();
    }
    return "";
}

bool GenericOverloadingAnalyzer::validateTypeArguments(ASTNode* overload, const std::vector<Type*>& typeArguments) {
    // Check if overload is generic
    if (!isGeneric(overload)) {
        if (!typeArguments.empty()) {
            std::cerr << "Type arguments provided for non-generic overload" << std::endl;
            return false;
        }
        return true;
    }
    
    // Get type parameters
    auto typeParams = getTypeParameters(overload);
    
    // Check type argument count
    if (typeArguments.size() != typeParams.size()) {
        std::cerr << "Type argument count mismatch" << std::endl;
        return false;
    }
    
    // Validate type arguments
    for (size_t i = 0; i < typeArguments.size(); ++i) {
        if (!validateTypeArgument(typeArguments[i], typeParams[i])) {
            return false;
        }
    }
    
    return true;
}

bool GenericOverloadingAnalyzer::isGeneric(ASTNode* node) {
    switch (node->getType()) {
        case ASTNodeType::MethodDeclaration:
            return static_cast<MethodDeclaration*>(node)->isGeneric();
        case ASTNodeType::FunctionDeclaration:
            return static_cast<FunctionDeclaration*>(node)->isGeneric();
        default:
            return false;
    }
}

std::vector<TypeParameter*> GenericOverloadingAnalyzer::getTypeParameters(ASTNode* node) {
    switch (node->getType()) {
        case ASTNodeType::MethodDeclaration:
            return static_cast<MethodDeclaration*>(node)->getTypeParameters();
        case ASTNodeType::FunctionDeclaration:
            return static_cast<FunctionDeclaration*>(node)->getTypeParameters();
        default:
            return {};
    }
}

bool GenericOverloadingAnalyzer::validateTypeArgument(Type* typeArg, TypeParameter* typeParam) {
    // Validate type argument against type parameter constraints
    // This would integrate with the GenericConstraintsAnalyzer
    return true;
}

} // namespace semantic
} // namespace tsc