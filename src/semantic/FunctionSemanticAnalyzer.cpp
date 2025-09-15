#include "tsc/semantic/FunctionSemanticAnalyzer.h"
#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace tsc {
namespace semantic {

// =============================================================================
// FUNCTION SEMANTIC ANALYZER IMPLEMENTATION
// =============================================================================

FunctionSemanticAnalyzer::FunctionSemanticAnalyzer(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool FunctionSemanticAnalyzer::analyzeFunctionDeclaration(FunctionDeclaration* functionDecl) {
    if (!functionDecl) {
        std::cerr << "Function declaration is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing function declaration: " << functionDecl->getName() << std::endl;
    
    // Analyze function name
    if (!analyzeFunctionName(functionDecl->getName())) {
        std::cerr << "Failed to analyze function name" << std::endl;
        return false;
    }
    
    // Analyze function overloads if present
    if (functionDecl->hasOverloads()) {
        if (!analyzeFunctionOverloads(functionDecl->getOverloads())) {
            std::cerr << "Failed to analyze function overloads" << std::endl;
            return false;
        }
    }
    
    // Analyze function parameters
    if (!analyzeFunctionParameters(functionDecl->getParameters())) {
        std::cerr << "Failed to analyze function parameters" << std::endl;
        return false;
    }
    
    // Analyze function body
    if (functionDecl->getBody()) {
        if (!analyzeFunctionBody(functionDecl->getBody())) {
            std::cerr << "Failed to analyze function body" << std::endl;
            return false;
        }
    }
    
    // Perform semantic checks
    if (!performSemanticChecks(functionDecl)) {
        std::cerr << "Semantic checks failed" << std::endl;
        return false;
    }
    
    // Add function to symbol table
    if (!addFunctionToSymbolTable(functionDecl)) {
        std::cerr << "Failed to add function to symbol table" << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeFunctionName(const std::string& name) {
    if (name.empty()) {
        std::cerr << "Function name is empty" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing function name: " << name << std::endl;
    
    // Check if name is valid identifier
    if (!isValidIdentifierName(name)) {
        std::cerr << "Invalid function name: " << name << std::endl;
        return false;
    }
    
    // Check if name is reserved keyword
    if (isReservedKeyword(name)) {
        std::cerr << "Function name is a reserved keyword: " << name << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeFunctionOverloads(const std::vector<FunctionOverloadDeclaration*>& overloads) {
    if (overloads.empty()) {
        return true;
    }
    
    std::cout << "Analyzing function overloads" << std::endl;
    
    // Analyze each overload
    for (size_t i = 0; i < overloads.size(); ++i) {
        if (!analyzeFunctionOverload(overloads[i])) {
            std::cerr << "Failed to analyze overload " << i << std::endl;
            return false;
        }
    }
    
    // Check for overload conflicts
    if (!checkOverloadConflicts(overloads)) {
        std::cerr << "Overload conflicts detected" << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeFunctionOverload(FunctionOverloadDeclaration* overload) {
    if (!overload) {
        std::cerr << "Function overload is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing function overload: " << overload->getName() << std::endl;
    
    // Analyze overload parameters
    if (!analyzeFunctionParameters(overload->getParameters())) {
        std::cerr << "Failed to analyze overload parameters" << std::endl;
        return false;
    }
    
    // Analyze overload return type
    if (overload->getReturnType()) {
        if (!analyzeType(overload->getReturnType())) {
            std::cerr << "Failed to analyze overload return type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeFunctionParameters(const std::vector<ASTNode*>& parameters) {
    std::cout << "Analyzing function parameters" << std::endl;
    
    std::set<std::string> parameterNames;
    bool hasRestParameter = false;
    
    for (size_t i = 0; i < parameters.size(); ++i) {
        ASTNode* param = parameters[i];
        if (!param) {
            std::cerr << "Parameter " << i << " is null" << std::endl;
            return false;
        }
        
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            
            // Check parameter
            if (!analyzeParameter(parameter)) {
                std::cerr << "Failed to analyze parameter " << i << std::endl;
                return false;
            }
            
            // Check for duplicate parameter names
            if (parameterNames.find(parameter->getName()) != parameterNames.end()) {
                std::cerr << "Duplicate parameter name: " << parameter->getName() << std::endl;
                return false;
            }
            parameterNames.insert(parameter->getName());
            
            // Check rest parameter position
            if (parameter->isRest()) {
                if (hasRestParameter) {
                    std::cerr << "Multiple rest parameters not allowed" << std::endl;
                    return false;
                }
                if (i != parameters.size() - 1) {
                    std::cerr << "Rest parameter must be last" << std::endl;
                    return false;
                }
                hasRestParameter = true;
            }
        } else {
            std::cerr << "Invalid parameter type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeParameter(Parameter* parameter) {
    if (!parameter) {
        std::cerr << "Parameter is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing parameter: " << parameter->getName() << std::endl;
    
    // Analyze parameter name
    if (!isValidIdentifierName(parameter->getName())) {
        std::cerr << "Invalid parameter name: " << parameter->getName() << std::endl;
        return false;
    }
    
    // Analyze parameter type
    if (parameter->getType()) {
        if (!analyzeType(parameter->getType())) {
            std::cerr << "Failed to analyze parameter type" << std::endl;
            return false;
        }
    }
    
    // Analyze default value
    if (parameter->hasDefaultValue()) {
        if (!analyzeParameterDefaultValue(parameter)) {
            std::cerr << "Failed to analyze parameter default value" << std::endl;
            return false;
        }
    }
    
    // Analyze rest parameter
    if (parameter->isRest()) {
        if (!analyzeRestParameter(parameter)) {
            std::cerr << "Failed to analyze rest parameter" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeParameterDefaultValue(Parameter* parameter) {
    if (!parameter || !parameter->hasDefaultValue()) {
        return true;
    }
    
    std::cout << "Analyzing parameter default value: " << parameter->getName() << std::endl;
    
    ASTNode* defaultValue = parameter->getDefaultValue();
    if (!defaultValue) {
        std::cerr << "Default value is null" << std::endl;
        return false;
    }
    
    // Analyze default value expression
    if (defaultValue->getType() == ASTNodeType::Expression) {
        auto expression = static_cast<Expression*>(defaultValue);
        if (!analyzer_->checkExpression(expression)) {
            std::cerr << "Failed to analyze default value expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeRestParameter(Parameter* parameter) {
    if (!parameter || !parameter->isRest()) {
        return true;
    }
    
    std::cout << "Analyzing rest parameter: " << parameter->getName() << std::endl;
    
    // Rest parameter must have a type
    if (!parameter->getType()) {
        std::cerr << "Rest parameter must have a type" << std::endl;
        return false;
    }
    
    // Rest parameter type should be array-like
    if (!isArrayLikeType(parameter->getType())) {
        std::cerr << "Rest parameter type must be array-like" << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeFunctionBody(ASTNode* body) {
    if (!body) {
        return true;
    }
    
    std::cout << "Analyzing function body" << std::endl;
    
    // Analyze body based on type
    switch (body->getType()) {
        case ASTNodeType::BlockStatement:
            return analyzeBlockStatement(static_cast<BlockStatement*>(body));
        case ASTNodeType::Expression:
            return analyzeExpression(static_cast<Expression*>(body));
        default:
            std::cerr << "Invalid function body type" << std::endl;
            return false;
    }
}

bool FunctionSemanticAnalyzer::analyzeBlockStatement(BlockStatement* block) {
    if (!block) {
        return true;
    }
    
    std::cout << "Analyzing block statement" << std::endl;
    
    // Create new scope for function body
    auto symbolTable = analyzer_->getSymbolTable();
    symbolTable->enterScope();
    
    // Analyze each statement
    auto statements = block->getStatements();
    for (auto& statement : statements) {
        if (!analyzer_->checkStatement(statement)) {
            std::cerr << "Failed to analyze statement in function body" << std::endl;
            symbolTable->exitScope();
            return false;
        }
    }
    
    // Exit function body scope
    symbolTable->exitScope();
    
    return true;
}

bool FunctionSemanticAnalyzer::analyzeExpression(Expression* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Analyzing expression" << std::endl;
    
    return analyzer_->checkExpression(expression);
}

bool FunctionSemanticAnalyzer::analyzeType(Type* type) {
    if (!type) {
        return true;
    }
    
    std::cout << "Analyzing type: " << type->getName() << std::endl;
    
    // Check if type is valid
    if (!isValidType(type)) {
        std::cerr << "Invalid type: " << type->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::performSemanticChecks(FunctionDeclaration* functionDecl) {
    std::cout << "Performing semantic checks" << std::endl;
    
    // Check for potential issues
    if (!checkForPotentialIssues(functionDecl)) {
        std::cerr << "Potential issues check failed" << std::endl;
        return false;
    }
    
    // Check for performance implications
    if (!checkPerformanceImplications(functionDecl)) {
        std::cerr << "Performance implications check failed" << std::endl;
        return false;
    }
    
    // Check for best practices
    if (!checkBestPractices(functionDecl)) {
        std::cerr << "Best practices check failed" << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::checkForPotentialIssues(FunctionDeclaration* functionDecl) {
    std::cout << "Checking for potential issues" << std::endl;
    
    // Check for unused parameters
    if (hasUnusedParameters(functionDecl)) {
        std::cout << "Warning: Some parameters might be unused" << std::endl;
    }
    
    // Check for missing return statements
    if (hasMissingReturnStatements(functionDecl)) {
        std::cout << "Warning: Function might be missing return statements" << std::endl;
    }
    
    // Check for infinite recursion
    if (hasPotentialInfiniteRecursion(functionDecl)) {
        std::cout << "Warning: Function might have infinite recursion" << std::endl;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::checkPerformanceImplications(FunctionDeclaration* functionDecl) {
    std::cout << "Checking performance implications" << std::endl;
    
    // Check for expensive operations
    if (hasExpensiveOperations(functionDecl)) {
        std::cout << "Warning: Function might have expensive operations" << std::endl;
    }
    
    // Check for large parameter lists
    if (hasLargeParameterList(functionDecl)) {
        std::cout << "Warning: Function has many parameters" << std::endl;
    }
    
    // Check for complex overloads
    if (hasComplexOverloads(functionDecl)) {
        std::cout << "Warning: Function has complex overloads" << std::endl;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::checkBestPractices(FunctionDeclaration* functionDecl) {
    std::cout << "Checking best practices" << std::endl;
    
    // Check for consistent naming
    if (!hasConsistentNaming(functionDecl)) {
        std::cout << "Warning: Function naming might not be consistent" << std::endl;
    }
    
    // Check for appropriate parameter order
    if (!hasAppropriateParameterOrder(functionDecl)) {
        std::cout << "Warning: Parameter order might not be appropriate" << std::endl;
    }
    
    // Check for proper use of rest parameters
    if (!hasProperRestParameterUsage(functionDecl)) {
        std::cout << "Warning: Rest parameter usage might not be appropriate" << std::endl;
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::checkOverloadConflicts(const std::vector<FunctionOverloadDeclaration*>& overloads) {
    std::cout << "Checking overload conflicts" << std::endl;
    
    // Check for duplicate signatures
    for (size_t i = 0; i < overloads.size(); ++i) {
        for (size_t j = i + 1; j < overloads.size(); ++j) {
            if (areOverloadSignaturesIdentical(overloads[i], overloads[j])) {
                std::cerr << "Duplicate overload signatures found" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::areOverloadSignaturesIdentical(FunctionOverloadDeclaration* overload1, 
                                                             FunctionOverloadDeclaration* overload2) {
    if (!overload1 || !overload2) {
        return false;
    }
    
    // Check parameter count
    if (overload1->getParameters().size() != overload2->getParameters().size()) {
        return false;
    }
    
    // Check parameter types
    auto params1 = overload1->getParameters();
    auto params2 = overload2->getParameters();
    
    for (size_t i = 0; i < params1.size(); ++i) {
        if (!areParameterTypesIdentical(params1[i], params2[i])) {
            return false;
        }
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::areParameterTypesIdentical(ASTNode* param1, ASTNode* param2) {
    if (!param1 || !param2) {
        return false;
    }
    
    if (param1->getType() != param2->getType()) {
        return false;
    }
    
    if (param1->getType() == ASTNodeType::Parameter) {
        auto p1 = static_cast<Parameter*>(param1);
        auto p2 = static_cast<Parameter*>(param2);
        
        // Check parameter types
        Type* type1 = p1->getType();
        Type* type2 = p2->getType();
        
        if (type1 && type2) {
            return areTypesIdentical(type1, type2);
        } else if (!type1 && !type2) {
            return true;
        }
    }
    
    return false;
}

bool FunctionSemanticAnalyzer::addFunctionToSymbolTable(FunctionDeclaration* functionDecl) {
    if (!functionDecl) {
        return false;
    }
    
    std::cout << "Adding function to symbol table: " << functionDecl->getName() << std::endl;
    
    auto symbolTable = analyzer_->getSymbolTable();
    
    // Create function type
    std::vector<Type*> parameterTypes;
    for (auto& param : functionDecl->getParameters()) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            parameterTypes.push_back(parameter->getType());
        }
    }
    
    Type* functionType = new FunctionType(parameterTypes, functionDecl->getReturnType(), 
                                         functionDecl->isAsync(), functionDecl->isGenerator());
    
    // Create symbol for function
    auto symbol = new Symbol(functionDecl->getName(), SymbolType::Function);
    symbol->setType(functionType);
    symbol->setNode(functionDecl);
    
    // Add to symbol table
    return symbolTable->addSymbol(symbol);
}

// =============================================================================
// HELPER METHODS
// =============================================================================

bool FunctionSemanticAnalyzer::isValidIdentifierName(const std::string& name) {
    if (name.empty()) return false;
    
    // Check first character
    if (!std::isalpha(name[0]) && name[0] != '_' && name[0] != '$') {
        return false;
    }
    
    // Check remaining characters
    for (size_t i = 1; i < name.length(); ++i) {
        char c = name[i];
        if (!std::isalnum(c) && c != '_' && c != '$') {
            return false;
        }
    }
    
    return true;
}

bool FunctionSemanticAnalyzer::isReservedKeyword(const std::string& name) {
    static const std::vector<std::string> reservedKeywords = {
        "break", "case", "catch", "class", "const", "continue", "debugger", "default",
        "delete", "do", "else", "enum", "export", "extends", "finally", "for", "function",
        "if", "import", "in", "instanceof", "let", "new", "return", "super", "switch",
        "this", "throw", "try", "typeof", "var", "void", "while", "with", "yield",
        "async", "await", "static", "private", "protected", "public", "readonly",
        "abstract", "interface", "namespace", "module", "declare", "type", "as"
    };
    
    return std::find(reservedKeywords.begin(), reservedKeywords.end(), name) != reservedKeywords.end();
}

bool FunctionSemanticAnalyzer::isValidType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName != "void" && typeName != "never";
}

bool FunctionSemanticAnalyzer::isArrayLikeType(Type* type) {
    if (!type) return false;
    
    switch (type->getType()) {
        case TypeType::ArrayType:
            return true;
        case TypeType::GenericType:
            return isGenericArrayType(static_cast<GenericType*>(type));
        default:
            return isKnownArrayType(type);
    }
}

bool FunctionSemanticAnalyzer::isGenericArrayType(GenericType* genericType) {
    if (!genericType) return false;
    
    std::string typeName = genericType->getName();
    return typeName == "Array" || typeName == "ReadonlyArray" || typeName == "Iterable";
}

bool FunctionSemanticAnalyzer::isKnownArrayType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName == "string" || typeName == "arguments" || typeName == "NodeList";
}

bool FunctionSemanticAnalyzer::areTypesIdentical(Type* type1, Type* type2) {
    if (!type1 || !type2) return false;
    
    return type1->getName() == type2->getName();
}

// Issue detection methods
bool FunctionSemanticAnalyzer::hasUnusedParameters(FunctionDeclaration* functionDecl) {
    // This is a simplified check - in a real implementation, this would analyze
    // the function body to determine if parameters are used
    
    return false; // Assume all parameters are used for now
}

bool FunctionSemanticAnalyzer::hasMissingReturnStatements(FunctionDeclaration* functionDecl) {
    // This is a simplified check - in a real implementation, this would analyze
    // the function body to determine if return statements are present
    
    return false; // Assume return statements are present for now
}

bool FunctionSemanticAnalyzer::hasPotentialInfiniteRecursion(FunctionDeclaration* functionDecl) {
    // This is a simplified check - in a real implementation, this would analyze
    // the function body for recursive calls
    
    return false; // Assume no infinite recursion for now
}

bool FunctionSemanticAnalyzer::hasExpensiveOperations(FunctionDeclaration* functionDecl) {
    // This is a simplified check - in a real implementation, this would analyze
    // the function body for expensive operations
    
    return false; // Assume no expensive operations for now
}

bool FunctionSemanticAnalyzer::hasLargeParameterList(FunctionDeclaration* functionDecl) {
    if (!functionDecl) return false;
    
    return functionDecl->getParameters().size() > 10;
}

bool FunctionSemanticAnalyzer::hasComplexOverloads(FunctionDeclaration* functionDecl) {
    if (!functionDecl) return false;
    
    return functionDecl->getOverloads().size() > 5;
}

bool FunctionSemanticAnalyzer::hasConsistentNaming(FunctionDeclaration* functionDecl) {
    // This is a simplified check - in a real implementation, this would analyze
    // the naming conventions used
    
    return true; // Assume consistent naming for now
}

bool FunctionSemanticAnalyzer::hasAppropriateParameterOrder(FunctionDeclaration* functionDecl) {
    // This is a simplified check - in a real implementation, this would analyze
    // the parameter order for best practices
    
    return true; // Assume appropriate parameter order for now
}

bool FunctionSemanticAnalyzer::hasProperRestParameterUsage(FunctionDeclaration* functionDecl) {
    // This is a simplified check - in a real implementation, this would analyze
    // the usage of rest parameters
    
    return true; // Assume proper rest parameter usage for now
}

} // namespace semantic
} // namespace tsc