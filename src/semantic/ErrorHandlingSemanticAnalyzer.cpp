#include "tsc/semantic/ErrorHandlingSemanticAnalyzer.h"
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
// ERROR HANDLING SEMANTIC ANALYZER IMPLEMENTATION
// =============================================================================

ErrorHandlingSemanticAnalyzer::ErrorHandlingSemanticAnalyzer(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool ErrorHandlingSemanticAnalyzer::analyzeTryStatement(TryStatement* tryStmt) {
    if (!tryStmt) {
        std::cerr << "Try statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing try statement" << std::endl;
    
    // Analyze try block
    if (!analyzeTryBlock(tryStmt->getTryBlock())) {
        std::cerr << "Failed to analyze try block" << std::endl;
        return false;
    }
    
    // Analyze catch clauses
    if (tryStmt->hasCatchClauses()) {
        if (!analyzeCatchClauses(tryStmt->getCatchClauses())) {
            std::cerr << "Failed to analyze catch clauses" << std::endl;
            return false;
        }
    }
    
    // Analyze finally block
    if (tryStmt->hasFinallyBlock()) {
        if (!analyzeFinallyBlock(tryStmt->getFinallyBlock())) {
            std::cerr << "Failed to analyze finally block" << std::endl;
            return false;
        }
    }
    
    // Perform semantic checks
    if (!performSemanticChecks(tryStmt)) {
        std::cerr << "Semantic checks failed" << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeTryBlock(ASTNode* tryBlock) {
    if (!tryBlock) {
        std::cerr << "Try block is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing try block" << std::endl;
    
    // Check if try block is a block statement
    if (tryBlock->getType() == ASTNodeType::BlockStatement) {
        auto block = static_cast<BlockStatement*>(tryBlock);
        return analyzeBlockStatement(block);
    }
    
    std::cerr << "Try block must be a block statement" << std::endl;
    return false;
}

bool ErrorHandlingSemanticAnalyzer::analyzeCatchClauses(const std::vector<CatchClause*>& catchClauses) {
    if (catchClauses.empty()) {
        return true;
    }
    
    std::cout << "Analyzing catch clauses" << std::endl;
    
    // Analyze each catch clause
    for (size_t i = 0; i < catchClauses.size(); ++i) {
        if (!analyzeCatchClause(catchClauses[i])) {
            std::cerr << "Failed to analyze catch clause " << i << std::endl;
            return false;
        }
    }
    
    // Check for catch clause conflicts
    if (!checkCatchClauseConflicts(catchClauses)) {
        std::cerr << "Catch clause conflicts detected" << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeCatchClause(CatchClause* catchClause) {
    if (!catchClause) {
        std::cerr << "Catch clause is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing catch clause: " << catchClause->getParameterName() << std::endl;
    
    // Analyze catch parameter
    if (catchClause->hasParameter()) {
        if (!analyzeCatchParameter(catchClause)) {
            std::cerr << "Failed to analyze catch parameter" << std::endl;
            return false;
        }
    }
    
    // Analyze catch body
    if (catchClause->hasBody()) {
        if (!analyzeCatchBody(catchClause->getBody())) {
            std::cerr << "Failed to analyze catch body" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeCatchParameter(CatchClause* catchClause) {
    if (!catchClause || !catchClause->hasParameter()) {
        return true;
    }
    
    std::cout << "Analyzing catch parameter: " << catchClause->getParameterName() << std::endl;
    
    // Check parameter name
    if (catchClause->getParameterName().empty()) {
        std::cerr << "Catch parameter name is empty" << std::endl;
        return false;
    }
    
    // Check if parameter name is valid identifier
    if (!isValidIdentifierName(catchClause->getParameterName())) {
        std::cerr << "Invalid catch parameter name: " << catchClause->getParameterName() << std::endl;
        return false;
    }
    
    // Check parameter type
    if (catchClause->hasParameterType()) {
        if (!analyzeParameterType(catchClause->getParameterType())) {
            std::cerr << "Failed to analyze catch parameter type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeCatchBody(ASTNode* catchBody) {
    if (!catchBody) {
        return true;
    }
    
    std::cout << "Analyzing catch body" << std::endl;
    
    // Check if catch body is a block statement
    if (catchBody->getType() == ASTNodeType::BlockStatement) {
        auto block = static_cast<BlockStatement*>(catchBody);
        return analyzeBlockStatement(block);
    }
    
    std::cerr << "Catch body must be a block statement" << std::endl;
    return false;
}

bool ErrorHandlingSemanticAnalyzer::analyzeFinallyBlock(ASTNode* finallyBlock) {
    if (!finallyBlock) {
        return true;
    }
    
    std::cout << "Analyzing finally block" << std::endl;
    
    // Check if finally block is a block statement
    if (finallyBlock->getType() == ASTNodeType::BlockStatement) {
        auto block = static_cast<BlockStatement*>(finallyBlock);
        return analyzeBlockStatement(block);
    }
    
    std::cerr << "Finally block must be a block statement" << std::endl;
    return false;
}

bool ErrorHandlingSemanticAnalyzer::analyzeBlockStatement(BlockStatement* block) {
    if (!block) {
        return true;
    }
    
    std::cout << "Analyzing block statement" << std::endl;
    
    // Create new scope for block
    auto symbolTable = analyzer_->getSymbolTable();
    symbolTable->enterScope();
    
    // Analyze each statement in the block
    auto statements = block->getStatements();
    for (const auto& statement : statements) {
        if (!analyzeStatement(statement)) {
            std::cerr << "Failed to analyze statement in block" << std::endl;
            symbolTable->exitScope();
            return false;
        }
    }
    
    // Exit block scope
    symbolTable->exitScope();
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeStatement(ASTNode* statement) {
    if (!statement) {
        return true;
    }
    
    // Analyze statement based on type
    switch (statement->getType()) {
        case ASTNodeType::TryStatement:
            return analyzeTryStatement(static_cast<TryStatement*>(statement));
        case ASTNodeType::ThrowStatement:
            return analyzeThrowStatement(static_cast<ThrowStatement*>(statement));
        case ASTNodeType::PanicStatement:
            return analyzePanicStatement(static_cast<PanicStatement*>(statement));
        case ASTNodeType::AbortStatement:
            return analyzeAbortStatement(static_cast<AbortStatement*>(statement));
        default:
            return analyzer_->checkStatement(statement);
    }
}

bool ErrorHandlingSemanticAnalyzer::analyzeThrowStatement(ThrowStatement* throwStmt) {
    if (!throwStmt) {
        std::cerr << "Throw statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing throw statement" << std::endl;
    
    // Analyze throw expression
    if (throwStmt->hasExpression()) {
        if (!analyzeThrowExpression(throwStmt->getExpression())) {
            std::cerr << "Failed to analyze throw expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeThrowExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Analyzing throw expression" << std::endl;
    
    // Analyze expression
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        return analyzer_->checkExpression(expr);
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeCustomExceptionClass(CustomExceptionClass* exceptionClass) {
    if (!exceptionClass) {
        std::cerr << "Custom exception class is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing custom exception class: " << exceptionClass->getName() << std::endl;
    
    // Analyze exception class name
    if (!analyzeExceptionClassName(exceptionClass->getName())) {
        std::cerr << "Failed to analyze exception class name" << std::endl;
        return false;
    }
    
    // Analyze base exceptions
    if (exceptionClass->hasBaseExceptions()) {
        if (!analyzeBaseExceptions(exceptionClass->getBaseExceptions())) {
            std::cerr << "Failed to analyze base exceptions" << std::endl;
            return false;
        }
    }
    
    // Analyze exception class members
    if (!analyzeExceptionClassMembers(exceptionClass)) {
        std::cerr << "Failed to analyze exception class members" << std::endl;
        return false;
    }
    
    // Add exception class to symbol table
    if (!addExceptionClassToSymbolTable(exceptionClass)) {
        std::cerr << "Failed to add exception class to symbol table" << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeExceptionClassName(const std::string& name) {
    if (name.empty()) {
        std::cerr << "Exception class name is empty" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing exception class name: " << name << std::endl;
    
    // Check if name is valid identifier
    if (!isValidIdentifierName(name)) {
        std::cerr << "Invalid exception class name: " << name << std::endl;
        return false;
    }
    
    // Check if name follows exception naming convention
    if (!followsExceptionNamingConvention(name)) {
        std::cerr << "Exception class name does not follow naming convention: " << name << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeBaseExceptions(const std::vector<ASTNode*>& baseExceptions) {
    std::cout << "Analyzing base exceptions" << std::endl;
    
    for (const auto& baseException : baseExceptions) {
        if (!analyzeBaseException(baseException)) {
            std::cerr << "Failed to analyze base exception" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeBaseException(ASTNode* baseException) {
    if (!baseException) {
        return true;
    }
    
    std::cout << "Analyzing base exception" << std::endl;
    
    // Check if base exception is a valid exception type
    if (baseException->getType() == ASTNodeType::Identifier) {
        auto identifier = static_cast<Identifier*>(baseException);
        return analyzeExceptionType(identifier->getName());
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeExceptionType(const std::string& typeName) {
    if (typeName.empty()) {
        return false;
    }
    
    std::cout << "Analyzing exception type: " << typeName << std::endl;
    
    // Check if type is a valid exception type
    if (!isValidExceptionType(typeName)) {
        std::cerr << "Invalid exception type: " << typeName << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeExceptionClassMembers(CustomExceptionClass* exceptionClass) {
    if (!exceptionClass) {
        return true;
    }
    
    std::cout << "Analyzing exception class members" << std::endl;
    
    // Analyze class members
    auto members = exceptionClass->getMembers();
    for (const auto& member : members) {
        if (!analyzeExceptionClassMember(member)) {
            std::cerr << "Failed to analyze exception class member" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeExceptionClassMember(ASTNode* member) {
    if (!member) {
        return true;
    }
    
    std::cout << "Analyzing exception class member" << std::endl;
    
    // Analyze member based on type
    switch (member->getType()) {
        case ASTNodeType::MethodDeclaration:
            return analyzeExceptionMethod(static_cast<MethodDeclaration*>(member));
        case ASTNodeType::PropertyDeclaration:
            return analyzeExceptionProperty(static_cast<PropertyDeclaration*>(member));
        case ASTNodeType::ConstructorDeclaration:
            return analyzeExceptionConstructor(static_cast<ConstructorDeclaration*>(member));
        default:
            return analyzer_->checkStatement(member);
    }
}

bool ErrorHandlingSemanticAnalyzer::analyzeExceptionMethod(MethodDeclaration* methodDecl) {
    if (!methodDecl) {
        return true;
    }
    
    std::cout << "Analyzing exception method: " << methodDecl->getName() << std::endl;
    
    // Analyze method name
    if (!isValidIdentifierName(methodDecl->getName())) {
        std::cerr << "Invalid method name: " << methodDecl->getName() << std::endl;
        return false;
    }
    
    // Analyze method parameters
    auto parameters = methodDecl->getParameters();
    for (const auto& param : parameters) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            if (!analyzeParameter(parameter)) {
                std::cerr << "Failed to analyze method parameter" << std::endl;
                return false;
            }
        }
    }
    
    // Analyze method return type
    if (methodDecl->getReturnType()) {
        if (!analyzeParameterType(methodDecl->getReturnType())) {
            std::cerr << "Failed to analyze method return type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeExceptionProperty(PropertyDeclaration* propertyDecl) {
    if (!propertyDecl) {
        return true;
    }
    
    std::cout << "Analyzing exception property: " << propertyDecl->getName() << std::endl;
    
    // Analyze property name
    if (!isValidIdentifierName(propertyDecl->getName())) {
        std::cerr << "Invalid property name: " << propertyDecl->getName() << std::endl;
        return false;
    }
    
    // Analyze property type
    if (propertyDecl->getType()) {
        if (!analyzeParameterType(propertyDecl->getType())) {
            std::cerr << "Failed to analyze property type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeExceptionConstructor(ConstructorDeclaration* constructorDecl) {
    if (!constructorDecl) {
        return true;
    }
    
    std::cout << "Analyzing exception constructor" << std::endl;
    
    // Analyze constructor parameters
    auto parameters = constructorDecl->getParameters();
    for (const auto& param : parameters) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            if (!analyzeParameter(parameter)) {
                std::cerr << "Failed to analyze constructor parameter" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzePanicStatement(PanicStatement* panicStmt) {
    if (!panicStmt) {
        std::cerr << "Panic statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing panic statement" << std::endl;
    
    // Analyze panic expression
    if (panicStmt->hasExpression()) {
        if (!analyzePanicExpression(panicStmt->getExpression())) {
            std::cerr << "Failed to analyze panic expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzePanicExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Analyzing panic expression" << std::endl;
    
    // Analyze expression
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        return analyzer_->checkExpression(expr);
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeAbortStatement(AbortStatement* abortStmt) {
    if (!abortStmt) {
        std::cerr << "Abort statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing abort statement" << std::endl;
    
    // Analyze abort expression
    if (abortStmt->hasExpression()) {
        if (!analyzeAbortExpression(abortStmt->getExpression())) {
            std::cerr << "Failed to analyze abort expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeAbortExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Analyzing abort expression" << std::endl;
    
    // Analyze expression
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        return analyzer_->checkExpression(expr);
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeErrorPropagationExpression(ErrorPropagationExpression* errorExpr) {
    if (!errorExpr) {
        std::cerr << "Error propagation expression is null" << std::endl;
        return false;
    }
    
    std::cout << "Analyzing error propagation expression" << std::endl;
    
    // Analyze error expression
    if (errorExpr->hasExpression()) {
        if (!analyzeErrorExpression(errorExpr->getExpression())) {
            std::cerr << "Failed to analyze error expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeErrorExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Analyzing error expression" << std::endl;
    
    // Analyze expression
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        return analyzer_->checkExpression(expr);
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::performSemanticChecks(TryStatement* tryStmt) {
    std::cout << "Performing semantic checks for try statement" << std::endl;
    
    // Check for potential issues
    if (!checkForPotentialIssues(tryStmt)) {
        std::cerr << "Potential issues check failed" << std::endl;
        return false;
    }
    
    // Check for best practices
    if (!checkBestPractices(tryStmt)) {
        std::cerr << "Best practices check failed" << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::checkForPotentialIssues(TryStatement* tryStmt) {
    std::cout << "Checking for potential issues" << std::endl;
    
    // Check for empty catch blocks
    if (hasEmptyCatchBlocks(tryStmt)) {
        std::cout << "Warning: Some catch blocks are empty" << std::endl;
    }
    
    // Check for unreachable code
    if (hasUnreachableCode(tryStmt)) {
        std::cout << "Warning: Unreachable code detected" << std::endl;
    }
    
    // Check for resource leaks
    if (hasResourceLeaks(tryStmt)) {
        std::cout << "Warning: Potential resource leaks detected" << std::endl;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::checkBestPractices(TryStatement* tryStmt) {
    std::cout << "Checking best practices" << std::endl;
    
    // Check for proper exception handling
    if (!hasProperExceptionHandling(tryStmt)) {
        std::cout << "Warning: Exception handling could be improved" << std::endl;
    }
    
    // Check for proper resource management
    if (!hasProperResourceManagement(tryStmt)) {
        std::cout << "Warning: Resource management could be improved" << std::endl;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::checkCatchClauseConflicts(const std::vector<CatchClause*>& catchClauses) {
    std::cout << "Checking catch clause conflicts" << std::endl;
    
    // Check for duplicate catch parameter names
    std::set<std::string> parameterNames;
    for (const auto& catchClause : catchClauses) {
        if (catchClause->hasParameter()) {
            const std::string& paramName = catchClause->getParameterName();
            if (parameterNames.find(paramName) != parameterNames.end()) {
                std::cerr << "Duplicate catch parameter name: " << paramName << std::endl;
                return false;
            }
            parameterNames.insert(paramName);
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::addExceptionClassToSymbolTable(CustomExceptionClass* exceptionClass) {
    if (!exceptionClass) {
        return false;
    }
    
    std::cout << "Adding exception class to symbol table: " << exceptionClass->getName() << std::endl;
    
    auto symbolTable = analyzer_->getSymbolTable();
    
    // Create exception class type
    Type* exceptionType = new ClassType(exceptionClass->getName());
    
    // Create symbol for exception class
    auto symbol = new Symbol(exceptionClass->getName(), SymbolType::Class);
    symbol->setType(exceptionType);
    symbol->setNode(exceptionClass);
    
    // Add to symbol table
    return symbolTable->addSymbol(symbol);
}

// =============================================================================
// HELPER METHODS
// =============================================================================

bool ErrorHandlingSemanticAnalyzer::isValidIdentifierName(const std::string& name) {
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

bool ErrorHandlingSemanticAnalyzer::followsExceptionNamingConvention(const std::string& name) {
    // Check if name ends with "Error" or "Exception"
    return name.find("Error") != std::string::npos || 
           name.find("Exception") != std::string::npos;
}

bool ErrorHandlingSemanticAnalyzer::isValidExceptionType(const std::string& typeName) {
    return typeName == "Error" || typeName == "Exception" || 
           typeName.find("Error") != std::string::npos ||
           typeName.find("Exception") != std::string::npos;
}

bool ErrorHandlingSemanticAnalyzer::analyzeParameter(Parameter* parameter) {
    if (!parameter) {
        return true;
    }
    
    // Analyze parameter name
    if (!isValidIdentifierName(parameter->getName())) {
        std::cerr << "Invalid parameter name: " << parameter->getName() << std::endl;
        return false;
    }
    
    // Analyze parameter type
    if (parameter->getType()) {
        if (!analyzeParameterType(parameter->getType())) {
            std::cerr << "Failed to analyze parameter type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::analyzeParameterType(Type* type) {
    if (!type) {
        return true;
    }
    
    // Check if type is valid
    if (!isValidType(type)) {
        std::cerr << "Invalid type: " << type->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingSemanticAnalyzer::isValidType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName != "void" && typeName != "never";
}

// Issue detection methods
bool ErrorHandlingSemanticAnalyzer::hasEmptyCatchBlocks(TryStatement* tryStmt) {
    // This is a simplified check - in a real implementation, this would analyze
    // the catch block bodies to determine if they are empty
    
    return false; // Assume catch blocks are not empty for now
}

bool ErrorHandlingSemanticAnalyzer::hasUnreachableCode(TryStatement* tryStmt) {
    // This is a simplified check - in a real implementation, this would analyze
    // the control flow to determine if there is unreachable code
    
    return false; // Assume no unreachable code for now
}

bool ErrorHandlingSemanticAnalyzer::hasResourceLeaks(TryStatement* tryStmt) {
    // This is a simplified check - in a real implementation, this would analyze
    // the resource usage to determine if there are potential leaks
    
    return false; // Assume no resource leaks for now
}

bool ErrorHandlingSemanticAnalyzer::hasProperExceptionHandling(TryStatement* tryStmt) {
    // This is a simplified check - in a real implementation, this would analyze
    // the exception handling patterns to determine if they follow best practices
    
    return true; // Assume proper exception handling for now
}

bool ErrorHandlingSemanticAnalyzer::hasProperResourceManagement(TryStatement* tryStmt) {
    // This is a simplified check - in a real implementation, this would analyze
    // the resource management patterns to determine if they follow best practices
    
    return true; // Assume proper resource management for now
}

} // namespace semantic
} // namespace tsc