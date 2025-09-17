#include "tsc/semantic/ForOfTypeChecker.h"
#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <vector>
#include <string>

namespace tsc {
namespace semantic {

// =============================================================================
// FOR...OF TYPE CHECKER IMPLEMENTATION
// =============================================================================

ForOfTypeChecker::ForOfTypeChecker(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool ForOfTypeChecker::checkForOfStatement(ForOfStatement* forOfStmt) {
    if (!forOfStmt) {
        std::cerr << "ForOfStatement is null" << std::endl;
        return false;
    }
    
    std::cout << "Type checking for...of statement" << std::endl;
    
    // Check variable declaration
    if (!checkVariableDeclaration(forOfStmt->getVariable())) {
        std::cerr << "Invalid variable declaration in for...of statement" << std::endl;
        return false;
    }
    
    // Check iterable expression
    if (!checkIterableExpression(forOfStmt->getIterable())) {
        std::cerr << "Invalid iterable expression in for...of statement" << std::endl;
        return false;
    }
    
    // Check body statement
    if (!checkBodyStatement(forOfStmt->getBody())) {
        std::cerr << "Invalid body statement in for...of statement" << std::endl;
        return false;
    }
    
    // Check type compatibility between variable and iterable element type
    if (!checkTypeCompatibility(forOfStmt)) {
        std::cerr << "Type incompatibility in for...of statement" << std::endl;
        return false;
    }
    
    return true;
}

bool ForOfTypeChecker::checkVariableDeclaration(VariableDeclaration* variable) {
    if (!variable) {
        std::cerr << "Variable declaration is null" << std::endl;
        return false;
    }
    
    std::cout << "Checking variable declaration: " << variable->getName() << std::endl;
    
    // Check if variable declaration is valid
    if (variable->getName().empty()) {
        std::cerr << "Variable name is empty" << std::endl;
        return false;
    }
    
    // Check variable type if specified
    if (variable->getType()) {
        if (!checkVariableType(variable->getType())) {
            std::cerr << "Invalid variable type: " << variable->getType()->getName() << std::endl;
            return false;
        }
    }
    
    // Check if variable is already declared in current scope
    auto symbolTable = analyzer_->getSymbolTable();
    if (symbolTable->lookupSymbol(variable->getName())) {
        std::cerr << "Variable already declared: " << variable->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool ForOfTypeChecker::checkVariableType(Type* type) {
    if (!type) return true; // No type specified, will be inferred
    
    std::cout << "Checking variable type: " << type->getName() << std::endl;
    
    // Check if type exists in symbol table
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

bool ForOfTypeChecker::checkIterableExpression(Expression* iterable) {
    if (!iterable) {
        std::cerr << "Iterable expression is null" << std::endl;
        return false;
    }
    
    std::cout << "Checking iterable expression" << std::endl;
    
    // Get the type of the iterable expression
    Type* iterableType = getExpressionType(iterable);
    if (!iterableType) {
        std::cerr << "Could not determine type of iterable expression" << std::endl;
        return false;
    }
    
    // Check if the type is iterable
    if (!isIterableType(iterableType)) {
        std::cerr << "Expression type is not iterable: " << iterableType->getName() << std::endl;
        return false;
    }
    
    // Check if it's an async iterable for async for...of
    if (analyzer_->isInAsyncContext() && !isAsyncIterableType(iterableType)) {
        std::cerr << "Expression type is not async iterable" << std::endl;
        return false;
    }
    
    return true;
}

bool ForOfTypeChecker::checkBodyStatement(Statement* body) {
    if (!body) {
        std::cerr << "Body statement is null" << std::endl;
        return false;
    }
    
    std::cout << "Checking body statement" << std::endl;
    
    // Check if body statement is valid
    return analyzer_->checkStatement(body);
}

bool ForOfTypeChecker::checkTypeCompatibility(ForOfStatement* forOfStmt) {
    std::cout << "Checking type compatibility" << std::endl;
    
    auto variable = forOfStmt->getVariable();
    auto iterable = forOfStmt->getIterable();
    
    if (!variable || !iterable) {
        return false;
    }
    
    // Get variable type
    Type* variableType = variable->getType();
    if (!variableType) {
        // Type will be inferred from iterable element type
        return true;
    }
    
    // Get iterable element type
    Type* elementType = getIterableElementType(iterable);
    if (!elementType) {
        std::cerr << "Could not determine element type of iterable" << std::endl;
        return false;
    }
    
    // Check type compatibility
    if (!areTypesCompatible(variableType, elementType)) {
        std::cerr << "Type incompatibility: variable type " << variableType->getName() 
                  << " is not compatible with element type " << elementType->getName() << std::endl;
        return false;
    }
    
    return true;
}

Type* ForOfTypeChecker::getExpressionType(Expression* expr) {
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

Type* ForOfTypeChecker::getIdentifierType(Identifier* identifier) {
    if (!identifier) return nullptr;
    
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(identifier->getName());
    
    if (symbol && symbol->getType()) {
        return symbol->getType();
    }
    
    return nullptr;
}

Type* ForOfTypeChecker::getArrayExpressionType(ArrayExpression* arrayExpr) {
    if (!arrayExpr) return nullptr;
    
    // Create array type based on element types
    auto elements = arrayExpr->getElements();
    
    if (elements.empty()) {
        // Empty array - return any[]
        return createArrayType(createType("any"));
    }
    
    // Infer element type from first element
    Type* elementType = getExpressionType(elements[0]);
    if (elementType) {
        return createArrayType(elementType);
    }
    
    return createArrayType(createType("any"));
}

Type* ForOfTypeChecker::getCallExpressionType(CallExpression* callExpr) {
    if (!callExpr) return nullptr;
    
    // Get return type of called function
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

Type* ForOfTypeChecker::getMemberExpressionType(MemberExpression* memberExpr) {
    if (!memberExpr) return nullptr;
    
    // Get type of object
    Type* objectType = getExpressionType(memberExpr->getObject());
    if (!objectType) return nullptr;
    
    // Get property type
    auto property = memberExpr->getProperty();
    if (!property) return nullptr;
    
    if (property->getType() == ASTNodeType::Identifier) {
        auto identifier = static_cast<Identifier*>(property);
        return getPropertyType(objectType, identifier->getName());
    }
    
    return nullptr;
}

Type* ForOfTypeChecker::getPropertyType(Type* objectType, const std::string& propertyName) {
    if (!objectType) return nullptr;
    
    // This is a simplified property type lookup
    // In a real implementation, this would look up the property in the type definition
    
    if (objectType->getName() == "Array" || objectType->getType() == TypeType::ArrayType) {
        if (propertyName == "values" || propertyName == "keys" || propertyName == "entries") {
            return createIteratorType(createType("any"));
        }
    }
    
    return nullptr;
}

Type* ForOfTypeChecker::getIterableElementType(Expression* iterable) {
    Type* iterableType = getExpressionType(iterable);
    if (!iterableType) return nullptr;
    
    return getElementTypeFromIterable(iterableType);
}

Type* ForOfTypeChecker::getElementTypeFromIterable(Type* iterableType) {
    if (!iterableType) return nullptr;
    
    switch (iterableType->getType()) {
        case TypeType::ArrayType:
            return static_cast<ArrayType*>(iterableType)->getElementType();
        case TypeType::GenericType:
            return getElementTypeFromGenericIterable(static_cast<GenericType*>(iterableType));
        default:
            // Check if it's a known iterable type
            if (isKnownIterableType(iterableType)) {
                return getElementTypeFromKnownIterable(iterableType);
            }
            return createType("any");
    }
}

Type* ForOfTypeChecker::getElementTypeFromGenericIterable(GenericType* genericType) {
    if (!genericType) return nullptr;
    
    std::string typeName = genericType->getName();
    auto typeArgs = genericType->getTypeArguments();
    
    if (typeName == "Array" && !typeArgs.empty()) {
        return typeArgs[0];
    } else if (typeName == "Set" && !typeArgs.empty()) {
        return typeArgs[0];
    } else if (typeName == "Map" && typeArgs.size() >= 2) {
        // For Map, return the key-value pair type
        return createTupleType({typeArgs[0], typeArgs[1]});
    }
    
    return createType("any");
}

Type* ForOfTypeChecker::getElementTypeFromKnownIterable(Type* iterableType) {
    if (!iterableType) return nullptr;
    
    std::string typeName = iterableType->getName();
    
    if (typeName == "string") {
        return createType("string");
    } else if (typeName == "arguments") {
        return createType("any");
    } else if (typeName == "NodeList") {
        return createType("Node");
    } else if (typeName == "HTMLCollection") {
        return createType("Element");
    }
    
    return createType("any");
}

bool ForOfTypeChecker::isIterableType(Type* type) {
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

bool ForOfTypeChecker::isGenericIterableType(GenericType* genericType) {
    if (!genericType) return false;
    
    std::string typeName = genericType->getName();
    
    return typeName == "Array" || typeName == "Set" || typeName == "Map" ||
           typeName == "ReadonlyArray" || typeName == "Iterable" || typeName == "Iterator";
}

bool ForOfTypeChecker::isKnownIterableType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    
    return typeName == "string" || typeName == "arguments" || typeName == "NodeList" ||
           typeName == "HTMLCollection" || typeName == "FileList" || typeName == "FormData";
}

bool ForOfTypeChecker::isAsyncIterableType(Type* type) {
    if (!type) return false;
    
    // Check if type implements AsyncIterable interface
    // This is simplified - in a real implementation, this would check the interface hierarchy
    
    std::string typeName = type->getName();
    return typeName == "AsyncIterable" || typeName == "ReadableStream" ||
           typeName == "AsyncGenerator" || typeName == "AsyncIterator";
}

bool ForOfTypeChecker::isValidIterationType(Type* type) {
    if (!type) return true; // Any type can be used for iteration
    
    // Check if type is not void or never
    std::string typeName = type->getName();
    return typeName != "void" && typeName != "never";
}

bool ForOfTypeChecker::areTypesCompatible(Type* type1, Type* type2) {
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

bool ForOfTypeChecker::isStructurallyCompatible(Type* type1, Type* type2) {
    // Check if types have compatible structure
    // This is simplified - in a real implementation, this would check structural typing
    
    if (type1->getType() == TypeType::ArrayType && type2->getType() == TypeType::ArrayType) {
        auto array1 = static_cast<ArrayType*>(type1);
        auto array2 = static_cast<ArrayType*>(type2);
        return areTypesCompatible(array1->getElementType(), array2->getElementType());
    }
    
    return false;
}

bool ForOfTypeChecker::isInheritanceCompatible(Type* type1, Type* type2) {
    // Check if type1 inherits from type2 or vice versa
    // This is simplified - in a real implementation, this would check the inheritance hierarchy
    
    return false;
}

std::string ForOfTypeChecker::getCalleeName(ASTNode* callee) {
    if (!callee) return "";
    
    switch (callee->getType()) {
        case ASTNodeType::Identifier:
            return static_cast<Identifier*>(callee)->getName();
        case ASTNodeType::MemberExpression:
            return getMemberExpressionName(static_cast<MemberExpression*>(callee));
        default:
            return "";
    }
}

std::string ForOfTypeChecker::getMemberExpressionName(MemberExpression* memberExpr) {
    if (!memberExpr) return "";
    
    auto property = memberExpr->getProperty();
    if (property && property->getType() == ASTNodeType::Identifier) {
        return static_cast<Identifier*>(property)->getName();
    }
    
    return "";
}

Type* ForOfTypeChecker::getFunctionReturnType(Type* functionType) {
    // This is simplified - in a real implementation, this would extract the return type
    // from a function type signature
    
    return createType("any");
}

// Type creation helper methods
Type* ForOfTypeChecker::createType(const std::string& name) {
    return new Type(name);
}

ArrayType* ForOfTypeChecker::createArrayType(Type* elementType) {
    return new ArrayType(elementType);
}

Type* ForOfTypeChecker::createIteratorType(Type* elementType) {
    // Create iterator type - simplified
    return new GenericType("Iterator", {elementType});
}

Type* ForOfTypeChecker::createTupleType(const std::vector<Type*>& types) {
    // Create tuple type - simplified
    return new GenericType("Tuple", types);
}

} // namespace semantic
} // namespace tsc