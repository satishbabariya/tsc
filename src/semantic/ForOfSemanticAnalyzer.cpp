#include "tsc/semantic/ForOfSemanticAnalyzer.h"
#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <vector>
#include <string>

namespace tsc {
namespace semantic {

// =============================================================================
// FOR...OF SEMANTIC ANALYZER IMPLEMENTATION
// =============================================================================

ForOfSemanticAnalyzer::ForOfSemanticAnalyzer(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool ForOfSemanticAnalyzer::analyzeForOfStatement(ForOfStatement* forOfStmt) {
    if (!forOfStmt) {
        std::cerr << "ForOfStatement is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing for...of statement" << std::endl;
    
    // Create new scope for the loop variable
    auto symbolTable = analyzer_->getSymbolTable();
    symbolTable->enterScope();
    
    try {
        // Analyze variable declaration
        if (!analyzeVariableDeclaration(forOfStmt->getVariable())) {
            std::cerr << "Failed to analyze variable declaration" << std::endl;
            symbolTable->exitScope();
            return false;
        }
        
        // Analyze iterable expression
        if (!analyzeIterableExpression(forOfStmt->getIterable())) {
            std::cerr << "Failed to analyze iterable expression" << std::endl;
            symbolTable->exitScope();
            return false;
        }
        
        // Analyze body statement
        if (!analyzeBodyStatement(forOfStmt->getBody())) {
            std::cerr << "Failed to analyze body statement" << std::endl;
            symbolTable->exitScope();
            return false;
        }
        
        // Perform semantic checks
        if (!performSemanticChecks(forOfStmt)) {
            std::cerr << "Semantic checks failed" << std::endl;
            symbolTable->exitScope();
            return false;
        }
        
        symbolTable->exitScope();
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception during for...of analysis: " << e.what() << std::endl;
        symbolTable->exitScope();
        return false;
    }
}

bool ForOfSemanticAnalyzer::analyzeVariableDeclaration(VariableDeclaration* variable) {
    if (!variable) {
        std::cerr << "Variable declaration is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing variable declaration: " << variable->getName() << std::endl;
    
    // Check variable name validity
    if (!isValidVariableName(variable->getName())) {
        std::cerr << "Invalid variable name: " << variable->getName() << std::endl;
        return false;
    }
    
    // Check for reserved keywords
    if (isReservedKeyword(variable->getName())) {
        std::cerr << "Variable name is a reserved keyword: " << variable->getName() << std::endl;
        return false;
    }
    
    // Analyze variable type if specified
    if (variable->getType()) {
        if (!analyzeVariableType(variable->getType())) {
            std::cerr << "Failed to analyze variable type" << std::endl;
            return false;
        }
    }
    
    // Add variable to symbol table
    if (!addVariableToSymbolTable(variable)) {
        std::cerr << "Failed to add variable to symbol table" << std::endl;
        return false;
    }
    
    return true;
}

bool ForOfSemanticAnalyzer::analyzeVariableType(Type* type) {
    if (!type) return true; // No type specified, will be inferred
    
    std::cout << "Analyzing variable type: " << type->getName() << std::endl;
    
    // Check if type exists
    auto symbolTable = analyzer_->getSymbolTable();
    auto typeInfo = symbolTable->lookupType(type->getName());
    
    if (!typeInfo) {
        std::cerr << "Type not found: " << type->getName() << std::endl;
        return false;
    }
    
    // Check if type is valid for iteration
    if (!isValidIterationType(type)) {
        std::cerr << "Type not suitable for iteration: " << type->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ForOfSemanticAnalyzer::analyzeIterableExpression(Expression* iterable) {
    if (!iterable) {
        std::cerr << "Iterable expression is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing iterable expression" << std::endl;
    
    // Analyze the expression itself
    if (!analyzer_->checkExpression(iterable)) {
        std::cerr << "Failed to analyze iterable expression" << std::endl;
        return false;
    }
    
    // Check if expression is iterable
    if (!isExpressionIterable(iterable)) {
        std::cerr << "Expression is not iterable" << std::endl;
        return false;
    }
    
    // Check for async iteration if needed
    if (analyzer_->isInAsyncContext()) {
        if (!isExpressionAsyncIterable(iterable)) {
            std::cerr << "Expression is not async iterable" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ForOfSemanticAnalyzer::analyzeBodyStatement(Statement* body) {
    if (!body) {
        std::cerr << "Body statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing body statement" << std::endl;
    
    // Analyze the body statement
    return analyzer_->checkStatement(body);
}

bool ForOfSemanticAnalyzer::performSemanticChecks(ForOfStatement* forOfStmt) {
    std::cout << "Performing semantic checks" << std::endl;
    
    // Check variable usage in body
    if (!checkVariableUsageInBody(forOfStmt)) {
        std::cerr << "Variable usage check failed" << std::endl;
        return false;
    }
    
    // Check for potential issues
    if (!checkForPotentialIssues(forOfStmt)) {
        std::cerr << "Potential issues check failed" << std::endl;
        return false;
    }
    
    // Check performance implications
    if (!checkPerformanceImplications(forOfStmt)) {
        std::cerr << "Performance implications check failed" << std::endl;
        return false;
    }
    
    return true;
}

bool ForOfSemanticAnalyzer::checkVariableUsageInBody(ForOfStatement* forOfStmt) {
    std::cout << "Checking variable usage in body" << std::endl;
    
    auto variable = forOfStmt->getVariable();
    auto body = forOfStmt->getBody();
    
    if (!variable || !body) {
        return false;
    }
    
    std::string variableName = variable->getName();
    
    // Check if variable is used in body
    if (!isVariableUsedInStatement(body, variableName)) {
        std::cout << "Warning: Variable " << variableName << " is not used in loop body" << std::endl;
    }
    
    // Check for variable reassignment (should be avoided)
    if (isVariableReassignedInStatement(body, variableName)) {
        std::cout << "Warning: Variable " << variableName << " is reassigned in loop body" << std::endl;
    }
    
    return true;
}

bool ForOfSemanticAnalyzer::checkForPotentialIssues(ForOfStatement* forOfStmt) {
    std::cout << "Checking for potential issues" << std::endl;
    
    auto iterable = forOfStmt->getIterable();
    auto body = forOfStmt->getBody();
    
    if (!iterable || !body) {
        return false;
    }
    
    // Check for infinite loops
    if (isPotentiallyInfiniteLoop(iterable)) {
        std::cout << "Warning: Potential infinite loop detected" << std::endl;
    }
    
    // Check for empty iterables
    if (isPotentiallyEmptyIterable(iterable)) {
        std::cout << "Warning: Iterable might be empty" << std::endl;
    }
    
    // Check for side effects in iterable expression
    if (hasSideEffects(iterable)) {
        std::cout << "Warning: Iterable expression has side effects" << std::endl;
    }
    
    return true;
}

bool ForOfSemanticAnalyzer::checkPerformanceImplications(ForOfStatement* forOfStmt) {
    std::cout << "Checking performance implications" << std::endl;
    
    auto iterable = forOfStmt->getIterable();
    
    if (!iterable) {
        return false;
    }
    
    // Check for expensive iterable expressions
    if (isExpensiveIterableExpression(iterable)) {
        std::cout << "Warning: Iterable expression might be expensive" << std::endl;
    }
    
    // Check for large iterables
    if (isPotentiallyLargeIterable(iterable)) {
        std::cout << "Warning: Iterable might be large" << std::endl;
    }
    
    return true;
}

bool ForOfSemanticAnalyzer::isValidVariableName(const std::string& name) {
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

bool ForOfSemanticAnalyzer::isReservedKeyword(const std::string& name) {
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

bool ForOfSemanticAnalyzer::isValidIterationType(Type* type) {
    if (!type) return true;
    
    // Check if type is not void or never
    std::string typeName = type->getName();
    return typeName != "void" && typeName != "never";
}

bool ForOfSemanticAnalyzer::addVariableToSymbolTable(VariableDeclaration* variable) {
    if (!variable) return false;
    
    auto symbolTable = analyzer_->getSymbolTable();
    
    // Create symbol for the variable
    auto symbol = new Symbol(variable->getName(), SymbolType::Variable);
    symbol->setType(variable->getType());
    symbol->setNode(variable);
    
    // Add to symbol table
    return symbolTable->addSymbol(symbol);
}

bool ForOfSemanticAnalyzer::isExpressionIterable(Expression* expr) {
    if (!expr) return false;
    
    // Get expression type
    Type* exprType = getExpressionType(expr);
    if (!exprType) return false;
    
    // Check if type is iterable
    return isIterableType(exprType);
}

bool ForOfSemanticAnalyzer::isExpressionAsyncIterable(Expression* expr) {
    if (!expr) return false;
    
    // Get expression type
    Type* exprType = getExpressionType(expr);
    if (!exprType) return false;
    
    // Check if type is async iterable
    return isAsyncIterableType(exprType);
}

bool ForOfSemanticAnalyzer::isIterableType(Type* type) {
    if (!type) return false;
    
    switch (type->getType()) {
        case TypeType::ArrayType:
            return true;
        case TypeType::GenericType:
            return isGenericIterableType(static_cast<GenericType*>(type));
        default:
            return isKnownIterableType(type);
    }
}

bool ForOfSemanticAnalyzer::isGenericIterableType(GenericType* genericType) {
    if (!genericType) return false;
    
    std::string typeName = genericType->getName();
    
    return typeName == "Array" || typeName == "Set" || typeName == "Map" ||
           typeName == "ReadonlyArray" || typeName == "Iterable" || typeName == "Iterator";
}

bool ForOfSemanticAnalyzer::isKnownIterableType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    
    return typeName == "string" || typeName == "arguments" || typeName == "NodeList" ||
           typeName == "HTMLCollection" || typeName == "FileList" || typeName == "FormData";
}

bool ForOfSemanticAnalyzer::isAsyncIterableType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName == "AsyncIterable" || typeName == "ReadableStream" ||
           typeName == "AsyncGenerator" || typeName == "AsyncIterator";
}

bool ForOfSemanticAnalyzer::isVariableUsedInStatement(Statement* stmt, const std::string& variableName) {
    if (!stmt) return false;
    
    // This is a simplified check - in a real implementation, this would traverse
    // the AST to find variable references
    
    return true; // Assume variable is used
}

bool ForOfSemanticAnalyzer::isVariableReassignedInStatement(Statement* stmt, const std::string& variableName) {
    if (!stmt) return false;
    
    // This is a simplified check - in a real implementation, this would traverse
    // the AST to find assignment expressions
    
    return false; // Assume no reassignment
}

bool ForOfSemanticAnalyzer::isPotentiallyInfiniteLoop(Expression* iterable) {
    if (!iterable) return false;
    
    // Check for expressions that might create infinite iterables
    // This is simplified - in a real implementation, this would analyze the expression
    
    return false; // Assume not infinite
}

bool ForOfSemanticAnalyzer::isPotentiallyEmptyIterable(Expression* iterable) {
    if (!iterable) return false;
    
    // Check for expressions that might be empty
    // This is simplified - in a real implementation, this would analyze the expression
    
    return false; // Assume not empty
}

bool ForOfSemanticAnalyzer::hasSideEffects(Expression* expr) {
    if (!expr) return false;
    
    // Check for expressions with side effects
    // This is simplified - in a real implementation, this would analyze the expression
    
    return false; // Assume no side effects
}

bool ForOfSemanticAnalyzer::isExpensiveIterableExpression(Expression* expr) {
    if (!expr) return false;
    
    // Check for expensive expressions
    // This is simplified - in a real implementation, this would analyze the expression
    
    return false; // Assume not expensive
}

bool ForOfSemanticAnalyzer::isPotentiallyLargeIterable(Expression* expr) {
    if (!expr) return false;
    
    // Check for potentially large iterables
    // This is simplified - in a real implementation, this would analyze the expression
    
    return false; // Assume not large
}

Type* ForOfSemanticAnalyzer::getExpressionType(Expression* expr) {
    if (!expr) return nullptr;
    
    // This is a simplified type inference
    // In a real implementation, this would use the semantic analyzer's type inference
    
    switch (expr->getType()) {
        case ASTNodeType::Identifier:
            return getIdentifierType(static_cast<Identifier*>(expr));
        case ASTNodeType::ArrayExpression:
            return getArrayExpressionType(static_cast<ArrayExpression*>(expr));
        case ASTNodeType::CallExpression:
            return getCallExpressionType(static_cast<CallExpression*>(expr));
        case ASTNodeType::MemberExpression:
            return getMemberExpressionType(static_cast<MemberExpression*>(expr));
        default:
            return nullptr;
    }
}

Type* ForOfSemanticAnalyzer::getIdentifierType(Identifier* identifier) {
    if (!identifier) return nullptr;
    
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(identifier->getName());
    
    if (symbol && symbol->getType()) {
        return symbol->getType();
    }
    
    return nullptr;
}

Type* ForOfSemanticAnalyzer::getArrayExpressionType(ArrayExpression* arrayExpr) {
    if (!arrayExpr) return nullptr;
    
    auto elements = arrayExpr->getElements();
    
    if (elements.empty()) {
        return createArrayType(createType("any"));
    }
    
    Type* elementType = getExpressionType(elements[0]);
    if (elementType) {
        return createArrayType(elementType);
    }
    
    return createArrayType(createType("any"));
}

Type* ForOfSemanticAnalyzer::getCallExpressionType(CallExpression* callExpr) {
    if (!callExpr) return nullptr;
    
    auto callee = callExpr->getCallee();
    if (!callee) return nullptr;
    
    std::string functionName = getCalleeName(callee);
    if (functionName.empty()) return nullptr;
    
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(functionName);
    
    if (symbol && symbol->getType()) {
        return getFunctionReturnType(symbol->getType());
    }
    
    return nullptr;
}

Type* ForOfSemanticAnalyzer::getMemberExpressionType(MemberExpression* memberExpr) {
    if (!memberExpr) return nullptr;
    
    Type* objectType = getExpressionType(memberExpr->getObject());
    if (!objectType) return nullptr;
    
    auto property = memberExpr->getProperty();
    if (!property) return nullptr;
    
    if (property->getType() == ASTNodeType::Identifier) {
        auto identifier = static_cast<Identifier*>(property);
        return getPropertyType(objectType, identifier->getName());
    }
    
    return nullptr;
}

Type* ForOfSemanticAnalyzer::getPropertyType(Type* objectType, const std::string& propertyName) {
    if (!objectType) return nullptr;
    
    if (objectType->getName() == "Array" || objectType->getType() == TypeType::ArrayType) {
        if (propertyName == "values" || propertyName == "keys" || propertyName == "entries") {
            return createIteratorType(createType("any"));
        }
    }
    
    return nullptr;
}

std::string ForOfSemanticAnalyzer::getCalleeName(ASTNode* callee) {
    if (!callee) return " + ";
    
    switch (callee->getType()) {
        case ASTNodeType::Identifier:
            return static_cast<Identifier*>(callee)->getName();
        case ASTNodeType::MemberExpression:
            return getMemberExpressionName(static_cast<MemberExpression*>(callee));
        default:
            return "";
    }
}

std::string ForOfSemanticAnalyzer::getMemberExpressionName(MemberExpression* memberExpr) {
    if (!memberExpr) return " + ";
    
    auto property = memberExpr->getProperty();
    if (property && property->getType() == ASTNodeType::Identifier) {
        return static_cast<Identifier*>(property)->getName();
    }
    
    return "";
}

Type* ForOfSemanticAnalyzer::getFunctionReturnType(Type* functionType) {
    return createType("any");
}

// Type creation helper methods
Type* ForOfSemanticAnalyzer::createType(const std::string& name) {
    return new Type(name);
}

ArrayType* ForOfSemanticAnalyzer::createArrayType(Type* elementType) {
    return new ArrayType(elementType);
}

Type* ForOfSemanticAnalyzer::createIteratorType(Type* elementType) {
    return new GenericType("Iterator", {elementType});
}

} // namespace semantic
} // namespace tsc