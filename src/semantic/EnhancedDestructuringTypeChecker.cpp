#include "tsc/semantic/EnhancedDestructuringTypeChecker.h"
#include "tsc/ast/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/semantic/SymbolTable.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace tsc {
namespace semantic {

// =============================================================================
// ENHANCED DESTRUCTURING TYPE CHECKER IMPLEMENTATION
// =============================================================================

EnhancedDestructuringTypeChecker::EnhancedDestructuringTypeChecker(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool EnhancedDestructuringTypeChecker::checkDestructuringAssignment(DestructuringPattern* pattern, Expression* rightHandSide) {
    if (!pattern || !rightHandSide) {
        std::cerr << "Enhanced: Destructuring pattern or right-hand side is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Type checking destructuring assignment" << std::endl;
    
    // Get the type of the right-hand side
    Type* rhsType = getExpressionType(rightHandSide);
    if (!rhsType) {
        std::cerr << "Enhanced: Could not determine type of right-hand side" << std::endl;
        return false;
    }
    
    // Check pattern based on its type
    switch (pattern->getType()) {
        case ASTNodeType::ArrayDestructuringPattern:
            return checkArrayDestructuringPattern(static_cast<ArrayDestructuringPattern*>(pattern), rhsType);
        case ASTNodeType::ObjectDestructuringPattern:
            return checkObjectDestructuringPattern(static_cast<ObjectDestructuringPattern*>(pattern), rhsType);
        default:
            std::cerr << "Enhanced: Unknown destructuring pattern type" << std::endl;
            return false;
    }
}

bool EnhancedDestructuringTypeChecker::checkArrayDestructuringPattern(ArrayDestructuringPattern* pattern, Type* rhsType) {
    std::cout << "Enhanced: Type checking array destructuring pattern" << std::endl;
    
    // Check if right-hand side is array-like
    if (!isArrayLikeType(rhsType)) {
        std::cerr << "Enhanced: Right-hand side is not array-like: " << rhsType->getName() << std::endl;
        return false;
    }
    
    // Get element type
    Type* elementType = getArrayElementType(rhsType);
    if (!elementType) {
        std::cerr << "Enhanced: Could not determine array element type" << std::endl;
        return false;
    }
    
    // Check each element in the pattern
    auto elements = pattern->getElements();
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!checkArrayDestructuringElement(elements[i], elementType, i)) {
            std::cerr << "Enhanced: Failed to check array destructuring element at index " << i << std::endl;
            return false;
        }
    }
    
    // Check rest element if present
    if (pattern->hasRestElement()) {
        if (!checkArrayRestElement(pattern->getRestElement(), rhsType)) {
            std::cerr << "Enhanced: Failed to check array rest element" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedDestructuringTypeChecker::checkObjectDestructuringPattern(ObjectDestructuringPattern* pattern, Type* rhsType) {
    std::cout << "Enhanced: Type checking object destructuring pattern" << std::endl;
    
    // Check if right-hand side is object-like
    if (!isObjectLikeType(rhsType)) {
        std::cerr << "Enhanced: Right-hand side is not object-like: " << rhsType->getName() << std::endl;
        return false;
    }
    
    // Check each property in the pattern
    auto properties = pattern->getProperties();
    for (auto& property : properties) {
        if (!checkObjectDestructuringProperty(property, rhsType)) {
            std::cerr << "Enhanced: Failed to check object destructuring property" << std::endl;
            return false;
        }
    }
    
    // Check rest element if present
    if (pattern->hasRestElement()) {
        if (!checkObjectRestElement(pattern->getRestElement(), rhsType)) {
            std::cerr << "Enhanced: Failed to check object rest element" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool EnhancedDestructuringTypeChecker::checkArrayDestructuringElement(ASTNode* element, Type* elementType, size_t index) {
    if (!element) {
        // Empty slot in array destructuring (e.g., [a, , c])
        return true;
    }
    
    std::cout << "Enhanced: Checking array destructuring element at index " << index << std::endl;
    
    switch (element->getType()) {
        case ASTNodeType::Identifier:
            return checkIdentifierDestructuring(static_cast<Identifier*>(element), elementType);
        case ASTNodeType::ArrayDestructuringPattern:
            return checkNestedArrayDestructuring(static_cast<ArrayDestructuringPattern*>(element), elementType);
        case ASTNodeType::ObjectDestructuringPattern:
            return checkNestedObjectDestructuring(static_cast<ObjectDestructuringPattern*>(element), elementType);
        case ASTNodeType::AssignmentPattern:
            return checkAssignmentPattern(static_cast<AssignmentPattern*>(element), elementType);
        default:
            std::cerr << "Enhanced: Unknown array destructuring element type" << std::endl;
            return false;
    }
}

bool EnhancedDestructuringTypeChecker::checkNestedArrayDestructuring(ArrayDestructuringPattern* pattern, Type* elementType) {
    if (!pattern) {
        std::cerr << "Enhanced: Nested array destructuring pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking nested array destructuring" << std::endl;
    
    // Check if element type is array-like
    if (!isArrayLikeType(elementType)) {
        std::cerr << "Enhanced: Element type is not array-like: " << elementType->getName() << std::endl;
        return false;
    }
    
    // Recursively check the nested pattern
    Type* nestedElementType = getArrayElementType(elementType);
    if (!nestedElementType) {
        std::cerr << "Enhanced: Could not determine nested array element type" << std::endl;
        return false;
    }
    
    return checkArrayDestructuringPattern(pattern, elementType);
}

bool EnhancedDestructuringTypeChecker::checkNestedObjectDestructuring(ObjectDestructuringPattern* pattern, Type* objectType) {
    if (!pattern) {
        std::cerr << "Enhanced: Nested object destructuring pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking nested object destructuring" << std::endl;
    
    // Check if object type is object-like
    if (!isObjectLikeType(objectType)) {
        std::cerr << "Enhanced: Object type is not object-like: " << objectType->getName() << std::endl;
        return false;
    }
    
    // Recursively check the nested pattern
    return checkObjectDestructuringPattern(pattern, objectType);
}

bool EnhancedDestructuringTypeChecker::checkObjectDestructuringProperty(ASTNode* property, Type* objectType) {
    if (!property) {
        std::cerr << "Enhanced: Object destructuring property is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking object destructuring property" << std::endl;
    
    if (property->getType() == ASTNodeType::DestructuringProperty) {
        auto destructuringProperty = static_cast<DestructuringProperty*>(property);
        return checkDestructuringProperty(destructuringProperty, objectType);
    }
    
    std::cerr << "Enhanced: Unknown object destructuring property type" << std::endl;
    return false;
}

bool EnhancedDestructuringTypeChecker::checkDestructuringProperty(DestructuringProperty* property, Type* objectType) {
    if (!property) {
        std::cerr << "Enhanced: Destructuring property is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking destructuring property" << std::endl;
    
    // Get property key
    ASTNode* key = property->getKey();
    if (!key) {
        std::cerr << "Enhanced: Destructuring property key is null" << std::endl;
        return false;
    }
    
    // Get property value (the variable being destructured)
    ASTNode* value = property->getValue();
    if (!value) {
        std::cerr << "Enhanced: Destructuring property value is null" << std::endl;
        return false;
    }
    
    // Get property type from object
    Type* propertyType = getObjectPropertyType(objectType, key);
    if (!propertyType) {
        std::cerr << "Enhanced: Could not determine property type" << std::endl;
        return false;
    }
    
    // Check if the destructured variable is compatible with the property type
    return checkDestructuringValue(value, propertyType);
}

bool EnhancedDestructuringTypeChecker::checkDestructuringValue(ASTNode* value, Type* propertyType) {
    if (!value) {
        std::cerr << "Enhanced: Destructuring value is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking destructuring value" << std::endl;
    
    switch (value->getType()) {
        case ASTNodeType::Identifier:
            return checkIdentifierDestructuring(static_cast<Identifier*>(value), propertyType);
        case ASTNodeType::ArrayDestructuringPattern:
            return checkNestedArrayDestructuring(static_cast<ArrayDestructuringPattern*>(value), propertyType);
        case ASTNodeType::ObjectDestructuringPattern:
            return checkNestedObjectDestructuring(static_cast<ObjectDestructuringPattern*>(value), propertyType);
        case ASTNodeType::AssignmentPattern:
            return checkAssignmentPattern(static_cast<AssignmentPattern*>(value), propertyType);
        default:
            std::cerr << "Enhanced: Unknown destructuring value type" << std::endl;
            return false;
    }
}

bool EnhancedDestructuringTypeChecker::checkIdentifierDestructuring(Identifier* identifier, Type* expectedType) {
    if (!identifier) {
        std::cerr << "Enhanced: Identifier is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking identifier destructuring: " << identifier->getName() << std::endl;
    
    // Check if identifier name is valid
    if (identifier->getName().empty()) {
        std::cerr << "Enhanced: Identifier name is empty" << std::endl;
        return false;
    }
    
    // Check if identifier is a reserved keyword
    if (isReservedKeyword(identifier->getName())) {
        std::cerr << "Enhanced: Identifier is a reserved keyword: " << identifier->getName() << std::endl;
        return false;
    }
    
    // Check if expected type is valid
    if (!isValidDestructuringType(expectedType)) {
        std::cerr << "Enhanced: Invalid destructuring type: " << expectedType->getName() << std::endl;
        return false;
    }
    
    return true;
}

bool EnhancedDestructuringTypeChecker::checkAssignmentPattern(AssignmentPattern* pattern, Type* expectedType) {
    if (!pattern) {
        std::cerr << "Enhanced: Assignment pattern is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking assignment pattern" << std::endl;
    
    // Check the left side (destructuring pattern)
    ASTNode* left = pattern->getLeft();
    if (!left) {
        std::cerr << "Enhanced: Assignment pattern left side is null" << std::endl;
        return false;
    }
    
    // Check the right side (default value)
    ASTNode* right = pattern->getRight();
    if (!right) {
        std::cerr << "Enhanced: Assignment pattern right side is null" << std::endl;
        return false;
    }
    
    // Check if default value is compatible with expected type
    Type* defaultType = getExpressionType(right);
    if (!defaultType) {
        std::cerr << "Enhanced: Could not determine default value type" << std::endl;
        return false;
    }
    
    if (!areTypesCompatible(expectedType, defaultType)) {
        std::cerr << "Enhanced: Default value type is not compatible with expected type" << std::endl;
        return false;
    }
    
    // Check the destructuring pattern
    return checkDestructuringValue(left, expectedType);
}

bool EnhancedDestructuringTypeChecker::checkArrayRestElement(ASTNode* restElement, Type* arrayType) {
    if (!restElement) {
        std::cerr << "Enhanced: Array rest element is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking array rest element" << std::endl;
    
    if (restElement->getType() == ASTNodeType::RestElement) {
        auto rest = static_cast<RestElement*>(restElement);
        ASTNode* argument = rest->getArgument();
        
        if (!argument) {
            std::cerr << "Enhanced: Rest element argument is null" << std::endl;
            return false;
        }
        
        // Check if argument is an identifier
        if (argument->getType() == ASTNodeType::Identifier) {
            auto identifier = static_cast<Identifier*>(argument);
            
            // Check if identifier is valid
            if (!checkIdentifierDestructuring(identifier, arrayType)) {
                return false;
            }
            
            // Rest element should be an array of the same element type
            Type* elementType = getArrayElementType(arrayType);
            if (!elementType) {
                std::cerr << "Enhanced: Could not determine array element type for rest element" << std::endl;
                return false;
            }
            
            // The rest element will be an array of the element type
            Type* restType = createArrayType(elementType);
            return checkIdentifierDestructuring(identifier, restType);
        }
    }
    
    std::cerr << "Enhanced: Invalid array rest element" << std::endl;
    return false;
}

bool EnhancedDestructuringTypeChecker::checkObjectRestElement(ASTNode* restElement, Type* objectType) {
    if (!restElement) {
        std::cerr << "Enhanced: Object rest element is null" << std::endl;
        return false;
    }
    
    std::cout << "Enhanced: Checking object rest element" << std::endl;
    
    if (restElement->getType() == ASTNodeType::RestElement) {
        auto rest = static_cast<RestElement*>(restElement);
        ASTNode* argument = rest->getArgument();
        
        if (!argument) {
            std::cerr << "Enhanced: Rest element argument is null" << std::endl;
            return false;
        }
        
        // Check if argument is an identifier
        if (argument->getType() == ASTNodeType::Identifier) {
            auto identifier = static_cast<Identifier*>(argument);
            
            // Check if identifier is valid
            if (!checkIdentifierDestructuring(identifier, objectType)) {
                return false;
            }
            
            // Rest element should be an object with the remaining properties
            // For now, we'll use the same object type
            return checkIdentifierDestructuring(identifier, objectType);
        }
    }
    
    std::cerr << "Enhanced: Invalid object rest element" << std::endl;
    return false;
}

// =============================================================================
// ENHANCED TYPE INFERENCE METHODS
// =============================================================================

Type* EnhancedDestructuringTypeChecker::inferNestedDestructuringType(DestructuringPattern* pattern, Type* sourceType) {
    if (!pattern || !sourceType) {
        return nullptr;
    }
    
    std::cout << "Enhanced: Inferring nested destructuring type" << std::endl;
    
    if (pattern->getType() == ASTNodeType::ArrayDestructuringPattern) {
        auto arrayPattern = static_cast<ArrayDestructuringPattern*>(pattern);
        if (isArrayLikeType(sourceType)) {
            Type* elementType = getArrayElementType(sourceType);
            if (elementType && isArrayLikeType(elementType)) {
                return inferNestedDestructuringType(arrayPattern, elementType);
            }
        }
    }
    
    return sourceType;
}

Type* EnhancedDestructuringTypeChecker::inferUnionDestructuringType(DestructuringPattern* pattern, UnionType* unionType) {
    if (!pattern || !unionType) {
        return nullptr;
    }
    
    std::cout << "Enhanced: Inferring union destructuring type" << std::endl;
    
    std::vector<Type*> compatibleTypes;
    for (auto& memberType : unionType->getMemberTypes()) {
        if (canDestructureFromType(pattern, memberType)) {
            compatibleTypes.push_back(memberType);
        }
    }
    
    if (compatibleTypes.size() == 1) {
        return compatibleTypes[0];
    } else if (compatibleTypes.size() > 1) {
        return createUnionType(compatibleTypes);
    }
    
    return nullptr;
}

Type* EnhancedDestructuringTypeChecker::inferGenericDestructuringType(DestructuringPattern* pattern, GenericType* genericType) {
    if (!pattern || !genericType) {
        return nullptr;
    }
    
    std::cout << "Enhanced: Inferring generic destructuring type" << std::endl;
    
    std::string typeName = genericType->getName();
    auto typeArgs = genericType->getTypeArguments();
    
    if (typeName == "Array" && !typeArgs.empty()) {
        return typeArgs[0];
    } else if (typeName == "ReadonlyArray" && !typeArgs.empty()) {
        return typeArgs[0];
    } else if (typeName == "Iterable" && !typeArgs.empty()) {
        return typeArgs[0];
    }
    
    return createType("any");
}

bool EnhancedDestructuringTypeChecker::canDestructureFromType(DestructuringPattern* pattern, Type* type) {
    if (!pattern || !type) {
        return false;
    }
    
    switch (pattern->getType()) {
        case ASTNodeType::ArrayDestructuringPattern:
            return isArrayLikeType(type);
        case ASTNodeType::ObjectDestructuringPattern:
            return isObjectLikeType(type);
        default:
            return false;
    }
}

// =============================================================================
// TYPE CHECKING HELPER METHODS
// =============================================================================

Type* EnhancedDestructuringTypeChecker::getExpressionType(Expression* expr) {
    if (!expr) return nullptr;
    
    // This is a simplified type inference
    // In a real implementation, this would use the semantic analyzer's type inference
    
    switch (expr->getType()) {
        case ASTNodeType::Identifier:
            return getIdentifierType(static_cast<Identifier*>(expr));
        case ASTNodeType::ArrayExpression:
            return getArrayExpressionType(static_cast<ArrayExpression*>(expr));
        case ASTNodeType::ObjectLiteral:
            return getObjectLiteralType(static_cast<ObjectLiteral*>(expr));
        case ASTNodeType::CallExpression:
            return getCallExpressionType(static_cast<CallExpression*>(expr));
        case ASTNodeType::MemberExpression:
            return getMemberExpressionType(static_cast<MemberExpression*>(expr));
        default:
            return nullptr;
    }
}

Type* EnhancedDestructuringTypeChecker::getIdentifierType(Identifier* identifier) {
    if (!identifier) return nullptr;
    
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(identifier->getName());
    
    if (symbol && symbol->getType()) {
        return symbol->getType();
    }
    
    return nullptr;
}

Type* EnhancedDestructuringTypeChecker::getArrayExpressionType(ArrayExpression* arrayExpr) {
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

Type* EnhancedDestructuringTypeChecker::getObjectLiteralType(ObjectLiteral* objectLiteral) {
    if (!objectLiteral) return nullptr;
    
    // Create object type from properties
    // This is simplified - in a real implementation, this would create a more specific object type
    return createType("object");
}

Type* EnhancedDestructuringTypeChecker::getCallExpressionType(CallExpression* callExpr) {
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

Type* EnhancedDestructuringTypeChecker::getMemberExpressionType(MemberExpression* memberExpr) {
    if (!memberExpr) return nullptr;
    
    Type* objectType = getExpressionType(memberExpr->getObject());
    if (!objectType) return nullptr;
    
    auto property = memberExpr->getProperty();
    if (!property) return nullptr;
    
    if (property->getType() == ASTNodeType::Identifier) {
        auto identifier = static_cast<Identifier*>(property);
        return getObjectPropertyType(objectType, identifier);
    }
    
    return nullptr;
}

Type* EnhancedDestructuringTypeChecker::getObjectPropertyType(Type* objectType, ASTNode* key) {
    if (!objectType || !key) return nullptr;
    
    // This is a simplified property type lookup
    // In a real implementation, this would look up the property in the type definition
    
    if (key->getType() == ASTNodeType::Identifier) {
        auto identifier = static_cast<Identifier*>(key);
        std::string propertyName = identifier->getName();
        
        // Check for common object properties
        if (objectType->getName() == "Array" || objectType->getType() == TypeType::ArrayType) {
            if (propertyName == "length") {
                return createType("number");
            }
        }
        
        // For now, return any type
        return createType("any");
    }
    
    return nullptr;
}

bool EnhancedDestructuringTypeChecker::isArrayLikeType(Type* type) {
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

bool EnhancedDestructuringTypeChecker::isGenericArrayType(GenericType* genericType) {
    if (!genericType) return false;
    
    std::string typeName = genericType->getName();
    return typeName == "Array" || typeName == "ReadonlyArray" || typeName == "Iterable";
}

bool EnhancedDestructuringTypeChecker::isKnownArrayType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName == "string" || typeName == "arguments" || typeName == "NodeList";
}

bool EnhancedDestructuringTypeChecker::isObjectLikeType(Type* type) {
    if (!type) return false;
    
    switch (type->getType()) {
        case TypeType::ObjectType:
            return true;
        case TypeType::GenericType:
            return isGenericObjectType(static_cast<GenericType*>(type));
        default:
            return isKnownObjectType(type);
    }
}

bool EnhancedDestructuringTypeChecker::isGenericObjectType(GenericType* genericType) {
    if (!genericType) return false;
    
    std::string typeName = genericType->getName();
    return typeName == "Object" || typeName == "Record" || typeName == "Map";
}

bool EnhancedDestructuringTypeChecker::isKnownObjectType(Type* type) {
    if (!type) return false;
    
    std::string typeName = type->getName();
    return typeName == "object" || typeName == "Object" || typeName == "any";
}

Type* EnhancedDestructuringTypeChecker::getArrayElementType(Type* arrayType) {
    if (!arrayType) return nullptr;
    
    switch (arrayType->getType()) {
        case TypeType::ArrayType:
            return static_cast<ArrayType*>(arrayType)->getElementType();
        case TypeType::GenericType:
            return getGenericArrayElementType(static_cast<GenericType*>(arrayType));
        default:
            return getKnownArrayElementType(arrayType);
    }
}

Type* EnhancedDestructuringTypeChecker::getGenericArrayElementType(GenericType* genericType) {
    if (!genericType) return nullptr;
    
    std::string typeName = genericType->getName();
    auto typeArgs = genericType->getTypeArguments();
    
    if (typeName == "Array" && !typeArgs.empty()) {
        return typeArgs[0];
    } else if (typeName == "ReadonlyArray" && !typeArgs.empty()) {
        return typeArgs[0];
    } else if (typeName == "Iterable" && !typeArgs.empty()) {
        return typeArgs[0];
    }
    
    return createType("any");
}

Type* EnhancedDestructuringTypeChecker::getKnownArrayElementType(Type* arrayType) {
    if (!arrayType) return nullptr;
    
    std::string typeName = arrayType->getName();
    
    if (typeName == "string") {
        return createType("string");
    } else if (typeName == "arguments") {
        return createType("any");
    } else if (typeName == "NodeList") {
        return createType("Node");
    }
    
    return createType("any");
}

bool EnhancedDestructuringTypeChecker::isReservedKeyword(const std::string& name) {
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

bool EnhancedDestructuringTypeChecker::isValidDestructuringType(Type* type) {
    if (!type) return false;
    
    // Check if type is not void or never
    std::string typeName = type->getName();
    return typeName != "void" && typeName != "never";
}

bool EnhancedDestructuringTypeChecker::areTypesCompatible(Type* type1, Type* type2) {
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

bool EnhancedDestructuringTypeChecker::isStructurallyCompatible(Type* type1, Type* type2) {
    // Check if types have compatible structure
    // This is simplified - in a real implementation, this would check structural typing
    
    if (type1->getType() == TypeType::ArrayType && type2->getType() == TypeType::ArrayType) {
        auto array1 = static_cast<ArrayType*>(type1);
        auto array2 = static_cast<ArrayType*>(type2);
        return areTypesCompatible(array1->getElementType(), array2->getElementType());
    }
    
    return false;
}

bool EnhancedDestructuringTypeChecker::isInheritanceCompatible(Type* type1, Type* type2) {
    // Check if type1 inherits from type2 or vice versa
    // This is simplified - in a real implementation, this would check the inheritance hierarchy
    
    return false;
}

std::string EnhancedDestructuringTypeChecker::getCalleeName(ASTNode* callee) {
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

std::string EnhancedDestructuringTypeChecker::getMemberExpressionName(MemberExpression* memberExpr) {
    if (!memberExpr) return " + ";
    
    auto property = memberExpr->getProperty();
    if (property && property->getType() == ASTNodeType::Identifier) {
        return static_cast<Identifier*>(property)->getName();
    }
    
    return "";
}

Type* EnhancedDestructuringTypeChecker::getFunctionReturnType(Type* functionType) {
    // This is simplified - in a real implementation, this would extract the return type
    // from a function type signature
    
    return createType("any");
}

// Type creation helper methods
Type* EnhancedDestructuringTypeChecker::createType(const std::string& name) {
    return new Type(name);
}

ArrayType* EnhancedDestructuringTypeChecker::createArrayType(Type* elementType) {
    return new ArrayType(elementType);
}

UnionType* EnhancedDestructuringTypeChecker::createUnionType(const std::vector<Type*>& memberTypes) {
    return new UnionType(memberTypes);
}

} // namespace semantic
} // namespace tsc