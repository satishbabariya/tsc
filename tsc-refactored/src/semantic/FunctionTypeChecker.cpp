#include "tsc/semantic/FunctionTypeChecker.h"
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
// FUNCTION TYPE CHECKER IMPLEMENTATION
// =============================================================================

FunctionTypeChecker::FunctionTypeChecker(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool FunctionTypeChecker::checkFunctionDeclaration(FunctionDeclaration* functionDecl) {
    if (!functionDecl) {
        std::cerr << "Function declaration is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking function declaration: " << functionDecl->getName() << std::endl;
    
    // Check function overloads if present
    if (functionDecl->hasOverloads()) {
        if (!checkFunctionOverloads(functionDecl->getOverloads())) {
            std::cerr << "Failed to check function overloads" << std::endl;
            return false;
        }
    }
    
    // Check function parameters
    if (!checkFunctionParameters(functionDecl->getParameters())) {
        std::cerr << "Failed to check function parameters" << std::endl;
        return false;
    }
    
    // Check function body
    if (functionDecl->getBody()) {
        if (!checkFunctionBody(functionDecl->getBody(), functionDecl->getReturnType())) {
            std::cerr << "Failed to check function body" << std::endl;
            return false;
        }
    }
    
    // Check return type consistency
    if (!checkReturnTypeConsistency(functionDecl)) {
        std::cerr << "Failed return type consistency check" << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionTypeChecker::checkFunctionOverloads(const std::vector<FunctionOverloadDeclaration*>& overloads) {
    if (overloads.empty()) {
        return true;
    }
    
    std::cout << "Type checking function overloads" << std::endl;
    
    // Check each overload
    for (size_t i = 0; i < overloads.size(); ++i) {
        if (!checkFunctionOverload(overloads[i])) {
            std::cerr << "Failed to check overload " << i << std::endl;
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

bool FunctionTypeChecker::checkFunctionOverload(FunctionOverloadDeclaration* overload) {
    if (!overload) {
        std::cerr << "Function overload is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking function overload: " << overload->getName() << std::endl;
    
    // Check overload parameters
    if (!checkFunctionParameters(overload->getParameters())) {
        std::cerr << "Failed to check overload parameters" << std::endl;
        return false;
    }
    
    // Check overload return type
    if (overload->getReturnType()) {
        if (!isValidType(overload->getReturnType())) {
            std::cerr << "Invalid overload return type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkFunctionParameters(const std::vector<ASTNode*>& parameters) {
    std::cout << "Type checking function parameters" << std::endl;
    
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
            if (!checkParameter(parameter)) {
                std::cerr << "Failed to check parameter " << i << std::endl;
                return false;
            }
            
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

bool FunctionTypeChecker::checkParameter(Parameter* parameter) {
    if (!parameter) {
        std::cerr << "Parameter is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking parameter: " << parameter->getName() << std::endl;
    
    // Check parameter name
    if (parameter->getName().empty()) {
        std::cerr << "Parameter name is empty" << std::endl;
        return false;
    }
    
    // Check parameter type
    if (parameter->getType()) {
        if (!isValidType(parameter->getType())) {
            std::cerr << "Invalid parameter type" << std::endl;
            return false;
        }
    }
    
    // Check default value
    if (parameter->hasDefaultValue()) {
        if (!checkParameterDefaultValue(parameter)) {
            std::cerr << "Failed to check parameter default value" << std::endl;
            return false;
        }
    }
    
    // Check rest parameter type
    if (parameter->isRest()) {
        if (!checkRestParameterType(parameter)) {
            std::cerr << "Failed to check rest parameter type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkParameterDefaultValue(Parameter* parameter) {
    if (!parameter || !parameter->hasDefaultValue()) {
        return true;
    }
    
    std::cout << "Type checking parameter default value: " << parameter->getName() << std::endl;
    
    ASTNode* defaultValue = parameter->getDefaultValue();
    Type* parameterType = parameter->getType();
    
    if (!defaultValue) {
        std::cerr << "Default value is null" << std::endl;
        return false;
    }
    
    // Get default value type
    Type* defaultType = getExpressionType(defaultValue);
    if (!defaultType) {
        std::cerr << "Could not determine default value type" << std::endl;
        return false;
    }
    
    // Check type compatibility
    if (parameterType) {
        if (!areTypesCompatible(parameterType, defaultType)) {
            std::cerr << "Default value type is not compatible with parameter type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkRestParameterType(Parameter* parameter) {
    if (!parameter || !parameter->isRest()) {
        return true;
    }
    
    std::cout << "Type checking rest parameter: " << parameter->getName() << std::endl;
    
    Type* parameterType = parameter->getType();
    if (!parameterType) {
        std::cerr << "Rest parameter must have a type" << std::endl;
        return false;
    }
    
    // Rest parameter type should be an array type
    if (!isArrayLikeType(parameterType)) {
        std::cerr << "Rest parameter type must be array-like" << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionTypeChecker::checkFunctionBody(ASTNode* body, Type* returnType) {
    if (!body) {
        return true;
    }
    
    std::cout << "Type checking function body" << std::endl;
    
    // Check if body is a block statement
    if (body->getType() == ASTNodeType::BlockStatement) {
        auto block = static_cast<BlockStatement*>(body);
        return checkBlockStatement(block, returnType);
    } else if (body->getType() == ASTNodeType::Expression) {
        // Arrow function with expression body
        auto expression = static_cast<Expression*>(body);
        return checkExpressionReturnType(expression, returnType);
    }
    
    std::cerr << "Invalid function body type" << std::endl;
    return false;
}

bool FunctionTypeChecker::checkBlockStatement(BlockStatement* block, Type* returnType) {
    if (!block) {
        return true;
    }
    
    std::cout << "Type checking block statement" << std::endl;
    
    auto statements = block->getStatements();
    bool hasReturnStatement = false;
    
    for (auto& statement : statements) {
        if (statement->getType() == ASTNodeType::ReturnStatement) {
            hasReturnStatement = true;
            auto returnStmt = static_cast<ReturnStatement*>(statement);
            if (!checkReturnStatement(returnStmt, returnType)) {
                std::cerr << "Failed to check return statement" << std::endl;
                return false;
            }
        }
    }
    
    // Check if function has return type but no return statement
    if (returnType && !hasReturnStatement) {
        if (returnType->getName() != "void") {
            std::cerr << "Function with non-void return type must have return statement" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkReturnStatement(ReturnStatement* returnStmt, Type* returnType) {
    if (!returnStmt) {
        return true;
    }
    
    std::cout << "Type checking return statement" << std::endl;
    
    ASTNode* argument = returnStmt->getArgument();
    
    if (argument) {
        // Function returns a value
        Type* argumentType = getExpressionType(argument);
        if (!argumentType) {
            std::cerr << "Could not determine return value type" << std::endl;
            return false;
        }
        
        if (returnType) {
            if (!areTypesCompatible(returnType, argumentType)) {
                std::cerr << "Return value type is not compatible with function return type" << std::endl;
                return false;
            }
        }
    } else {
        // Function returns void
        if (returnType && returnType->getName() != "void") {
            std::cerr << "Function with non-void return type must return a value" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkExpressionReturnType(Expression* expression, Type* returnType) {
    if (!expression) {
        return true;
    }
    
    std::cout << "Type checking expression return type" << std::endl;
    
    Type* expressionType = getExpressionType(expression);
    if (!expressionType) {
        std::cerr << "Could not determine expression type" << std::endl;
        return false;
    }
    
    if (returnType) {
        if (!areTypesCompatible(returnType, expressionType)) {
            std::cerr << "Expression type is not compatible with function return type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkReturnTypeConsistency(FunctionDeclaration* functionDecl) {
    if (!functionDecl) {
        return true;
    }
    
    std::cout << "Type checking return type consistency" << std::endl;
    
    Type* functionReturnType = functionDecl->getReturnType();
    
    // Check consistency with overloads
    if (functionDecl->hasOverloads()) {
        for (auto& overload : functionDecl->getOverloads()) {
            Type* overloadReturnType = overload->getReturnType();
            
            if (functionReturnType && overloadReturnType) {
                if (!areTypesCompatible(functionReturnType, overloadReturnType)) {
                    std::cerr << "Function return type is not consistent with overload return type" << std::endl;
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkOverloadConflicts(const std::vector<FunctionOverloadDeclaration*>& overloads) {
    std::cout << "Type checking overload conflicts" << std::endl;
    
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

bool FunctionTypeChecker::areOverloadSignaturesIdentical(FunctionOverloadDeclaration* overload1, 
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

bool FunctionTypeChecker::areParameterTypesIdentical(ASTNode* param1, ASTNode* param2) {
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

bool FunctionTypeChecker::checkArrowFunctionExpression(ArrowFunctionExpression* arrowFunc) {
    if (!arrowFunc) {
        std::cerr << "Arrow function expression is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking arrow function expression" << std::endl;
    
    // Check arrow function parameters
    if (!checkFunctionParameters(arrowFunc->getParameters())) {
        std::cerr << "Failed to check arrow function parameters" << std::endl;
        return false;
    }
    
    // Check arrow function body
    if (arrowFunc->hasBody()) {
        if (!checkFunctionBody(arrowFunc->getBody(), arrowFunc->getReturnType())) {
            std::cerr << "Failed to check arrow function body" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool FunctionTypeChecker::checkCallExpression(CallExpression* callExpr) {
    if (!callExpr) {
        std::cerr << "Call expression is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking call expression" << std::endl;
    
    // Check callee
    ASTNode* callee = callExpr->getCallee();
    if (!callee) {
        std::cerr << "Call expression callee is null" << std::endl;
        return false;
    }
    
    // Get function type
    Type* functionType = getExpressionType(callee);
    if (!functionType) {
        std::cerr << "Could not determine function type" << std::endl;
        return false;
    }
    
    if (functionType->getType() != TypeType::FunctionType) {
        std::cerr << "Callee is not a function type" << std::endl;
        return false;
    }
    
    auto funcType = static_cast<FunctionType*>(functionType);
    
    // Check arguments
    if (!checkCallArguments(callExpr->getArguments(), funcType)) {
        std::cerr << "Failed to check call arguments" << std::endl;
        return false;
    }
    
    return true;
}

bool FunctionTypeChecker::checkCallArguments(const std::vector<ASTNode*>& arguments, FunctionType* functionType) {
    if (!functionType) {
        return false;
    }
    
    std::cout << "Type checking call arguments" << std::endl;
    
    auto parameterTypes = functionType->getParameterTypes();
    size_t paramCount = parameterTypes.size();
    size_t argCount = arguments.size();
    
    // Check argument count
    if (argCount < paramCount) {
        std::cerr << "Too few arguments provided" << std::endl;
        return false;
    }
    
    // Check each argument
    for (size_t i = 0; i < argCount; ++i) {
        ASTNode* argument = arguments[i];
        if (!argument) {
            std::cerr << "Argument " << i << " is null" << std::endl;
            return false;
        }
        
        Type* argumentType = getExpressionType(argument);
        if (!argumentType) {
            std::cerr << "Could not determine argument " << i << " type" << std::endl;
            return false;
        }
        
        if (i < paramCount) {
            Type* parameterType = parameterTypes[i];
            if (!areTypesCompatible(parameterType, argumentType)) {
                std::cerr << "Argument " << i << " type is not compatible with parameter type" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

// =============================================================================
// TYPE CHECKING HELPER METHODS
// =============================================================================

Type* FunctionTypeChecker::getExpressionType(Expression* expr) {
    if (!expr) return nullptr;
    
    switch (expr->getType()) {
        case ASTNodeType::Identifier:
            return getIdentifierType(static_cast<Identifier*>(expr));
        case ASTNodeType::CallExpression:
            return getCallExpressionReturnType(static_cast<CallExpression*>(expr));
        case ASTNodeType::ArrowFunctionExpression:
            return getArrowFunctionType(static_cast<ArrowFunctionExpression*>(expr));
        default:
            return nullptr;
    }
}

Type* FunctionTypeChecker::getIdentifierType(Identifier* identifier) {
    if (!identifier) return nullptr;
    
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(identifier->getName());
    
    if (symbol && symbol->getType()) {
        return symbol->getType();
    }
    
    return nullptr;
}

Type* FunctionTypeChecker::getCallExpressionReturnType(CallExpression* callExpr) {
    if (!callExpr) return nullptr;
    
    Type* functionType = getExpressionType(callExpr->getCallee());
    if (functionType && functionType->getType() == TypeType::FunctionType) {
        auto funcType = static_cast<FunctionType*>(functionType);
        return funcType->getReturnType();
    }
    
    return nullptr;
}

Type* FunctionTypeChecker::getArrowFunctionType(ArrowFunctionExpression* arrowFunc) {
    if (!arrowFunc) return nullptr;
    
    std::vector<Type*> parameterTypes;
    for (auto& param : arrowFunc->getParameters()) {
        if (param->getType() == ASTNodeType::Parameter) {
            auto parameter = static_cast<Parameter*>(param);
            parameterTypes.push_back(parameter->getType());
        }
    }
    
    return new FunctionType(parameterTypes, arrowFunc->getReturnType(), arrowFunc->isAsync());
}

bool FunctionTypeChecker::isValidType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName != "void" && typeName != "never";
}

bool FunctionTypeChecker::isArrayLikeType(Type* type) {
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

bool FunctionTypeChecker::isGenericArrayType(GenericType* genericType) {
    if (!genericType) return false;
    
    std::string typeName = genericType->getName();
    return typeName == "Array" || typeName == "ReadonlyArray" || typeName == "Iterable";
}

bool FunctionTypeChecker::isKnownArrayType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName == "string" || typeName == "arguments" || typeName == "NodeList";
}

bool FunctionTypeChecker::areTypesCompatible(Type* type1, Type* type2) {
    if (!type1 || !type2) return false;
    
    // Check exact match
    if (type1->getName() == type2->getName()) {
        return true;
    }
    
    // Check structural compatibility
    if (isStructurallyCompatible(type1, type2)) {
        return true;
    }
    
    // Check inheritance compatibility
    if (isInheritanceCompatible(type1, type2)) {
        return true;
    }
    
    return false;
}

bool FunctionTypeChecker::areTypesIdentical(Type* type1, Type* type2) {
    if (!type1 || !type2) return false;
    
    return type1->getName() == type2->getName();
}

bool FunctionTypeChecker::isStructurallyCompatible(Type* type1, Type* type2) {
    // Check if types have compatible structure
    // This is simplified - in a real implementation, this would check structural typing
    
    if (type1->getType() == TypeType::ArrayType && type2->getType() == TypeType::ArrayType) {
        auto array1 = static_cast<ArrayType*>(type1);
        auto array2 = static_cast<ArrayType*>(type2);
        return areTypesCompatible(array1->getElementType(), array2->getElementType());
    }
    
    return false;
}

bool FunctionTypeChecker::isInheritanceCompatible(Type* type1, Type* type2) {
    // Check if type1 inherits from type2 or vice versa
    // This is simplified - in a real implementation, this would check the inheritance hierarchy
    
    return false;
}

} // namespace semantic
} // namespace tsc