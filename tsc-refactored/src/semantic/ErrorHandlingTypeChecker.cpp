#include "tsc/semantic/ErrorHandlingTypeChecker.h"
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
// ERROR HANDLING TYPE CHECKER IMPLEMENTATION
// =============================================================================

ErrorHandlingTypeChecker::ErrorHandlingTypeChecker(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool ErrorHandlingTypeChecker::checkTryStatement(TryStatement* tryStmt) {
    if (!tryStmt) {
        std::cerr << "Try statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking try statement" << std::endl;
    
    // Check try block
    if (!checkTryBlock(tryStmt->getTryBlock())) {
        std::cerr << "Failed to check try block" << std::endl;
        return false;
    }
    
    // Check catch clauses
    if (tryStmt->hasCatchClauses()) {
        if (!checkCatchClauses(tryStmt->getCatchClauses())) {
            std::cerr << "Failed to check catch clauses" << std::endl;
            return false;
        }
    }
    
    // Check finally block
    if (tryStmt->hasFinallyBlock()) {
        if (!checkFinallyBlock(tryStmt->getFinallyBlock())) {
            std::cerr << "Failed to check finally block" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkTryBlock(ASTNode* tryBlock) {
    if (!tryBlock) {
        std::cerr << "Try block is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking try block" << std::endl;
    
    // Check if try block is a block statement
    if (tryBlock->getType() == ASTNodeType::BlockStatement) {
        auto block = static_cast<BlockStatement*>(tryBlock);
        return checkBlockStatement(block);
    }
    
    std::cerr << "Try block must be a block statement" << std::endl;
    return false;
}

bool ErrorHandlingTypeChecker::checkCatchClauses(const std::vector<CatchClause*>& catchClauses) {
    if (catchClauses.empty()) {
        return true;
    }
    
    std::cout << "Type checking catch clauses" << std::endl;
    
    // Check each catch clause
    for (size_t i = 0; i < catchClauses.size(); ++i) {
        if (!checkCatchClause(catchClauses[i])) {
            std::cerr << "Failed to check catch clause " << i << std::endl;
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

bool ErrorHandlingTypeChecker::checkCatchClause(CatchClause* catchClause) {
    if (!catchClause) {
        std::cerr << "Catch clause is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking catch clause: " << catchClause->getParameterName() << std::endl;
    
    // Check catch parameter
    if (catchClause->hasParameter()) {
        if (!checkCatchParameter(catchClause)) {
            std::cerr << "Failed to check catch parameter" << std::endl;
            return false;
        }
    }
    
    // Check catch body
    if (catchClause->hasBody()) {
        if (!checkCatchBody(catchClause->getBody())) {
            std::cerr << "Failed to check catch body" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkCatchParameter(CatchClause* catchClause) {
    if (!catchClause || !catchClause->hasParameter()) {
        return true;
    }
    
    std::cout << "Type checking catch parameter: " << catchClause->getParameterName() << std::endl;
    
    // Check parameter name
    if (catchClause->getParameterName().empty()) {
        std::cerr << "Catch parameter name is empty" << std::endl;
        return false;
    }
    
    // Check parameter type
    if (catchClause->hasParameterType()) {
        if (!checkCatchParameterType(catchClause->getParameterType())) {
            std::cerr << "Failed to check catch parameter type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkCatchParameterType(Type* parameterType) {
    if (!parameterType) {
        return true;
    }
    
    std::cout << "Type checking catch parameter type: " << parameterType->getName() << std::endl;
    
    // Check if parameter type is a valid exception type
    if (!isValidExceptionType(parameterType)) {
        std::cerr << "Invalid exception type: " << parameterType->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkCatchBody(ASTNode* catchBody) {
    if (!catchBody) {
        return true;
    }
    
    std::cout << "Type checking catch body" << std::endl;
    
    // Check if catch body is a block statement
    if (catchBody->getType() == ASTNodeType::BlockStatement) {
        auto block = static_cast<BlockStatement*>(catchBody);
        return checkBlockStatement(block);
    }
    
    std::cerr << "Catch body must be a block statement" << std::endl;
    return false;
}

bool ErrorHandlingTypeChecker::checkFinallyBlock(ASTNode* finallyBlock) {
    if (!finallyBlock) {
        return true;
    }
    
    std::cout << "Type checking finally block" << std::endl;
    
    // Check if finally block is a block statement
    if (finallyBlock->getType() == ASTNodeType::BlockStatement) {
        auto block = static_cast<BlockStatement*>(finallyBlock);
        return checkBlockStatement(block);
    }
    
    std::cerr << "Finally block must be a block statement" << std::endl;
    return false;
}

bool ErrorHandlingTypeChecker::checkCatchClauseConflicts(const std::vector<CatchClause*>& catchClauses) {
    std::cout << "Type checking catch clause conflicts" << std::endl;
    
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
    
    // Check for catch clause type conflicts
    for (size_t i = 0; i < catchClauses.size(); ++i) {
        for (size_t j = i + 1; j < catchClauses.size(); ++j) {
            if (areCatchClausesConflicting(catchClauses[i], catchClauses[j])) {
                std::cerr << "Conflicting catch clauses found" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::areCatchClausesConflicting(CatchClause* catch1, CatchClause* catch2) {
    if (!catch1 || !catch2) {
        return false;
    }
    
    // Check if both catch clauses have the same parameter type
    if (catch1->hasParameterType() && catch2->hasParameterType()) {
        Type* type1 = catch1->getParameterType();
        Type* type2 = catch2->getParameterType();
        
        if (areTypesIdentical(type1, type2)) {
            return true;
        }
    }
    
    return false;
}

bool ErrorHandlingTypeChecker::checkThrowStatement(ThrowStatement* throwStmt) {
    if (!throwStmt) {
        std::cerr << "Throw statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking throw statement" << std::endl;
    
    // Check throw expression
    if (throwStmt->hasExpression()) {
        if (!checkThrowExpression(throwStmt->getExpression())) {
            std::cerr << "Failed to check throw expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkThrowExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Type checking throw expression" << std::endl;
    
    // Check if expression is a valid throwable type
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        Type* expressionType = getExpressionType(expr);
        
        if (expressionType) {
            if (!isValidThrowableType(expressionType)) {
                std::cerr << "Invalid throwable type: " << expressionType->getName() << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkCustomExceptionClass(CustomExceptionClass* exceptionClass) {
    if (!exceptionClass) {
        std::cerr << "Custom exception class is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking custom exception class: " << exceptionClass->getName() << std::endl;
    
    // Check base exceptions
    if (exceptionClass->hasBaseExceptions()) {
        if (!checkBaseExceptions(exceptionClass->getBaseExceptions())) {
            std::cerr << "Failed to check base exceptions" << std::endl;
            return false;
        }
    }
    
    // Check exception class members
    if (!checkExceptionClassMembers(exceptionClass)) {
        std::cerr << "Failed to check exception class members" << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkBaseExceptions(const std::vector<ASTNode*>& baseExceptions) {
    std::cout << "Type checking base exceptions" << std::endl;
    
    for (const auto& baseException : baseExceptions) {
        if (!checkBaseException(baseException)) {
            std::cerr << "Failed to check base exception" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkBaseException(ASTNode* baseException) {
    if (!baseException) {
        return true;
    }
    
    std::cout << "Type checking base exception" << std::endl;
    
    // Check if base exception is a valid exception type
    if (baseException->getType() == ASTNodeType::Identifier) {
        auto identifier = static_cast<Identifier*>(baseException);
        Type* exceptionType = getIdentifierType(identifier);
        
        if (exceptionType) {
            if (!isValidExceptionType(exceptionType)) {
                std::cerr << "Invalid base exception type: " << exceptionType->getName() << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkExceptionClassMembers(CustomExceptionClass* exceptionClass) {
    if (!exceptionClass) {
        return true;
    }
    
    std::cout << "Type checking exception class members" << std::endl;
    
    // Check class members
    auto members = exceptionClass->getMembers();
    for (const auto& member : members) {
        if (!checkExceptionClassMember(member)) {
            std::cerr << "Failed to check exception class member" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkExceptionClassMember(ASTNode* member) {
    if (!member) {
        return true;
    }
    
    std::cout << "Type checking exception class member" << std::endl;
    
    // Check member based on type
    switch (member->getType()) {
        case ASTNodeType::MethodDeclaration:
            return checkExceptionMethod(static_cast<MethodDeclaration*>(member));
        case ASTNodeType::PropertyDeclaration:
            return checkExceptionProperty(static_cast<PropertyDeclaration*>(member));
        case ASTNodeType::ConstructorDeclaration:
            return checkExceptionConstructor(static_cast<ConstructorDeclaration*>(member));
        default:
            return true;
    }
}

bool ErrorHandlingTypeChecker::checkExceptionMethod(MethodDeclaration* methodDecl) {
    if (!methodDecl) {
        return true;
    }
    
    std::cout << "Type checking exception method: " << methodDecl->getName() << std::endl;
    
    // Check method parameters
    auto parameters = methodDecl->getParameters();
    for (const auto& param : parameters) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            if (!checkParameter(parameter)) {
                std::cerr << "Failed to check method parameter" << std::endl;
                return false;
            }
        }
    }
    
    // Check method return type
    if (methodDecl->getReturnType()) {
        if (!checkMethodReturnType(methodDecl->getReturnType())) {
            std::cerr << "Failed to check method return type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkExceptionProperty(PropertyDeclaration* propertyDecl) {
    if (!propertyDecl) {
        return true;
    }
    
    std::cout << "Type checking exception property: " << propertyDecl->getName() << std::endl;
    
    // Check property type
    if (propertyDecl->getType()) {
        if (!checkPropertyType(propertyDecl->getType())) {
            std::cerr << "Failed to check property type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkExceptionConstructor(ConstructorDeclaration* constructorDecl) {
    if (!constructorDecl) {
        return true;
    }
    
    std::cout << "Type checking exception constructor" << std::endl;
    
    // Check constructor parameters
    auto parameters = constructorDecl->getParameters();
    for (const auto& param : parameters) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            if (!checkParameter(parameter)) {
                std::cerr << "Failed to check constructor parameter" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkPanicStatement(PanicStatement* panicStmt) {
    if (!panicStmt) {
        std::cerr << "Panic statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking panic statement" << std::endl;
    
    // Check panic expression
    if (panicStmt->hasExpression()) {
        if (!checkPanicExpression(panicStmt->getExpression())) {
            std::cerr << "Failed to check panic expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkPanicExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Type checking panic expression" << std::endl;
    
    // Check if expression is a valid panic type
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        Type* expressionType = getExpressionType(expr);
        
        if (expressionType) {
            if (!isValidPanicType(expressionType)) {
                std::cerr << "Invalid panic type: " << expressionType->getName() << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkAbortStatement(AbortStatement* abortStmt) {
    if (!abortStmt) {
        std::cerr << "Abort statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking abort statement" << std::endl;
    
    // Check abort expression
    if (abortStmt->hasExpression()) {
        if (!checkAbortExpression(abortStmt->getExpression())) {
            std::cerr << "Failed to check abort expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkAbortExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Type checking abort expression" << std::endl;
    
    // Check if expression is a valid abort type
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        Type* expressionType = getExpressionType(expr);
        
        if (expressionType) {
            if (!isValidAbortType(expressionType)) {
                std::cerr << "Invalid abort type: " << expressionType->getName() << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkErrorPropagationExpression(ErrorPropagationExpression* errorExpr) {
    if (!errorExpr) {
        std::cerr << "Error propagation expression is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking error propagation expression" << std::endl;
    
    // Check error expression
    if (errorExpr->hasExpression()) {
        if (!checkErrorExpression(errorExpr->getExpression())) {
            std::cerr << "Failed to check error expression" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkErrorExpression(ASTNode* expression) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Type checking error expression" << std::endl;
    
    // Check if expression is a valid error type
    if (expression->getType() == ASTNodeType::Expression) {
        auto expr = static_cast<Expression*>(expression);
        Type* expressionType = getExpressionType(expr);
        
        if (expressionType) {
            if (!isValidErrorType(expressionType)) {
                std::cerr << "Invalid error type: " << expressionType->getName() << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

// =============================================================================
// HELPER METHODS
// =============================================================================

bool ErrorHandlingTypeChecker::checkBlockStatement(BlockStatement* block) {
    if (!block) {
        return true;
    }
    
    std::cout << "Type checking block statement" << std::endl;
    
    // Check each statement in the block
    auto statements = block->getStatements();
    for (const auto& statement : statements) {
        if (!checkStatement(statement)) {
            std::cerr << "Failed to check statement in block" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkStatement(ASTNode* statement) {
    if (!statement) {
        return true;
    }
    
    // Check statement based on type
    switch (statement->getType()) {
        case ASTNodeType::TryStatement:
            return checkTryStatement(static_cast<TryStatement*>(statement));
        case ASTNodeType::ThrowStatement:
            return checkThrowStatement(static_cast<ThrowStatement*>(statement));
        case ASTNodeType::PanicStatement:
            return checkPanicStatement(static_cast<PanicStatement*>(statement));
        case ASTNodeType::AbortStatement:
            return checkAbortStatement(static_cast<AbortStatement*>(statement));
        default:
            return true;
    }
}

bool ErrorHandlingTypeChecker::checkParameter(Parameter* parameter) {
    if (!parameter) {
        return true;
    }
    
    // Check parameter type
    if (parameter->getType()) {
        if (!isValidType(parameter->getType())) {
            std::cerr << "Invalid parameter type: " << parameter->getType()->getName() << std::endl;
            return false;
        }
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkMethodReturnType(Type* returnType) {
    if (!returnType) {
        return true;
    }
    
    // Check if return type is valid
    if (!isValidType(returnType)) {
        std::cerr << "Invalid method return type: " << returnType->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ErrorHandlingTypeChecker::checkPropertyType(Type* propertyType) {
    if (!propertyType) {
        return true;
    }
    
    // Check if property type is valid
    if (!isValidType(propertyType)) {
        std::cerr << "Invalid property type: " << propertyType->getName() << std::endl;
        return false;
    }
    
    return true;
}

Type* ErrorHandlingTypeChecker::getExpressionType(Expression* expr) {
    if (!expr) return nullptr;
    
    // Get expression type based on expression type
    switch (expr->getType()) {
        case ASTNodeType::Identifier:
            return getIdentifierType(static_cast<Identifier*>(expr));
        case ASTNodeType::CallExpression:
            return getCallExpressionReturnType(static_cast<CallExpression*>(expr));
        default:
            return nullptr;
    }
}

Type* ErrorHandlingTypeChecker::getIdentifierType(Identifier* identifier) {
    if (!identifier) return nullptr;
    
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(identifier->getName());
    
    if (symbol && symbol->getType()) {
        return symbol->getType();
    }
    
    return nullptr;
}

Type* ErrorHandlingTypeChecker::getCallExpressionReturnType(CallExpression* callExpr) {
    if (!callExpr) return nullptr;
    
    Type* functionType = getExpressionType(callExpr->getCallee());
    if (functionType && functionType->getType() == TypeType::FunctionType) {
        auto funcType = static_cast<FunctionType*>(functionType);
        return funcType->getReturnType();
    }
    
    return nullptr;
}

bool ErrorHandlingTypeChecker::isValidExceptionType(Type* type) {
    if (!type) return false;
    
    // Check if type is a valid exception type
    std::string typeName = type->getName();
    return typeName == "Error" || typeName == "Exception" || 
           typeName.find("Error") != std::string::npos ||
           typeName.find("Exception") != std::string::npos;
}

bool ErrorHandlingTypeChecker::isValidThrowableType(Type* type) {
    if (!type) return false;
    
    // Check if type is throwable
    return isValidExceptionType(type) || isValidErrorType(type);
}

bool ErrorHandlingTypeChecker::isValidPanicType(Type* type) {
    if (!type) return false;
    
    // Check if type is valid for panic
    std::string typeName = type->getName();
    return typeName == "string" || typeName == "Error" || 
           typeName == "Exception" || isValidExceptionType(type);
}

bool ErrorHandlingTypeChecker::isValidAbortType(Type* type) {
    if (!type) return false;
    
    // Check if type is valid for abort
    std::string typeName = type->getName();
    return typeName == "string" || typeName == "Error" || 
           typeName == "Exception" || isValidExceptionType(type);
}

bool ErrorHandlingTypeChecker::isValidErrorType(Type* type) {
    if (!type) return false;
    
    // Check if type is a valid error type
    std::string typeName = type->getName();
    return typeName == "Error" || typeName == "Result" || 
           typeName == "Optional" || isValidExceptionType(type);
}

bool ErrorHandlingTypeChecker::isValidType(Type* type) {
    if (!type) return false;
    
    // Check if type is valid
    std::string typeName = type->getName();
    return typeName != "void" && typeName != "never";
}

bool ErrorHandlingTypeChecker::areTypesIdentical(Type* type1, Type* type2) {
    if (!type1 || !type2) return false;
    
    return type1->getName() == type2->getName();
}

} // namespace semantic
} // namespace tsc