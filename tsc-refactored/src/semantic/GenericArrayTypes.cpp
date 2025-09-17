#include "tsc/semantic/GenericArrayTypes.h"
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
// GENERIC ARRAY TYPES IMPLEMENTATION
// =============================================================================

GenericArrayTypesAnalyzer::GenericArrayTypesAnalyzer(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

bool GenericArrayTypesAnalyzer::analyzeGenericArrayTypes(ASTNode* node) {
    if (!node) return true;
    
    switch (node->getType()) {
        case ASTNodeType::ArrayExpression:
            return analyzeArrayExpression(static_cast<ArrayExpression*>(node));
        case ASTNodeType::ArrayType:
            return analyzeArrayType(static_cast<ArrayType*>(node));
        case ASTNodeType::GenericType:
            return analyzeGenericType(static_cast<GenericType*>(node));
        case ASTNodeType::CallExpression:
            return analyzeArrayMethodCall(static_cast<CallExpression*>(node));
        default:
            return true;
    }
}

bool GenericArrayTypesAnalyzer::analyzeArrayExpression(ArrayExpression* arrayExpr) {
    std::cout << "Analyzing array expression" << std::endl;
    
    // Infer array element type
    auto elementType = inferArrayElementType(arrayExpr);
    if (!elementType) {
        std::cerr << "Could not infer array element type" << std::endl;
        return false;
    }
    
    // Create array type
    auto arrayType = createArrayType(elementType);
    
    // Store type information
    arrayExpr->setInferredType(arrayType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayType(ArrayType* arrayType) {
    std::cout << "Analyzing array type" << std::endl;
    
    // Analyze element type
    auto elementType = arrayType->getElementType();
    if (!elementType) {
        std::cerr << "Array type missing element type" << std::endl;
        return false;
    }
    
    // Check if element type is generic
    if (elementType->getType() == TypeType::GenericType) {
        auto genericType = static_cast<GenericType*>(elementType);
        if (!validateGenericArrayElementType(genericType)) {
            return false;
        }
    }
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeGenericType(GenericType* genericType) {
    std::cout << "Analyzing generic type: " << genericType->getName() << std::endl;
    
    // Check if this is an array-like generic type
    if (isArrayLikeGenericType(genericType)) {
        return analyzeArrayLikeGenericType(genericType);
    }
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayMethodCall(CallExpression* callExpr) {
    std::cout << "Analyzing array method call" << std::endl;
    
    // Check if this is an array method call
    if (!isArrayMethodCall(callExpr)) {
        return true;
    }
    
    // Get array object
    auto callee = callExpr->getCallee();
    if (!callee || callee->getType() != ASTNodeType::MemberExpression) {
        return true;
    }
    
    auto memberExpr = static_cast<MemberExpression*>(callee);
    auto arrayObject = memberExpr->getObject();
    
    // Get array type
    auto arrayType = getArrayType(arrayObject);
    if (!arrayType) {
        std::cerr << "Could not determine array type" << std::endl;
        return false;
    }
    
    // Analyze method call based on array type
    return analyzeArrayMethodCallWithType(callExpr, arrayType);
}

bool GenericArrayTypesAnalyzer::analyzeArrayLikeGenericType(GenericType* genericType) {
    std::cout << "Analyzing array-like generic type" << std::endl;
    
    // Check if this is Array<T> or similar
    if (genericType->getName() == "Array") {
        return analyzeArrayGenericType(genericType);
    }
    
    // Check if this is a generic type that could be used as an array
    if (couldBeUsedAsArray(genericType)) {
        return analyzePotentialArrayGenericType(genericType);
    }
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayGenericType(GenericType* genericType) {
    std::cout << "Analyzing Array<T> generic type" << std::endl;
    
    // Check type arguments
    auto typeArgs = genericType->getTypeArguments();
    if (typeArgs.size() != 1) {
        std::cerr << "Array<T> must have exactly one type argument" << std::endl;
        return false;
    }
    
    // Validate element type
    auto elementType = typeArgs[0];
    if (!validateArrayElementType(elementType)) {
        return false;
    }
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzePotentialArrayGenericType(GenericType* genericType) {
    std::cout << "Analyzing potential array generic type" << std::endl;
    
    // Check if this generic type could be used as an array
    // This would involve checking if it has array-like methods or properties
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayMethodCallWithType(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array method call with type" << std::endl;
    
    // Get method name
    auto callee = callExpr->getCallee();
    if (callee->getType() != ASTNodeType::MemberExpression) {
        return true;
    }
    
    auto memberExpr = static_cast<MemberExpression*>(callee);
    auto property = memberExpr->getProperty();
    
    if (property->getType() != ASTNodeType::Identifier) {
        return true;
    }
    
    auto methodName = static_cast<Identifier*>(property)->getName();
    
    // Analyze method call based on method name
    return analyzeSpecificArrayMethod(callExpr, arrayType, methodName);
}

bool GenericArrayTypesAnalyzer::analyzeSpecificArrayMethod(CallExpression* callExpr, ArrayType* arrayType, const std::string& methodName) {
    std::cout << "Analyzing array method: " << methodName << std::endl;
    
    if (methodName == "push") {
        return analyzeArrayPushMethod(callExpr, arrayType);
    } else if (methodName == "pop") {
        return analyzeArrayPopMethod(callExpr, arrayType);
    } else if (methodName == "shift") {
        return analyzeArrayShiftMethod(callExpr, arrayType);
    } else if (methodName == "unshift") {
        return analyzeArrayUnshiftMethod(callExpr, arrayType);
    } else if (methodName == "slice") {
        return analyzeArraySliceMethod(callExpr, arrayType);
    } else if (methodName == "splice") {
        return analyzeArraySpliceMethod(callExpr, arrayType);
    } else if (methodName == "concat") {
        return analyzeArrayConcatMethod(callExpr, arrayType);
    } else if (methodName == "map") {
        return analyzeArrayMapMethod(callExpr, arrayType);
    } else if (methodName == "filter") {
        return analyzeArrayFilterMethod(callExpr, arrayType);
    } else if (methodName == "reduce") {
        return analyzeArrayReduceMethod(callExpr, arrayType);
    } else if (methodName == "forEach") {
        return analyzeArrayForEachMethod(callExpr, arrayType);
    } else if (methodName == "find") {
        return analyzeArrayFindMethod(callExpr, arrayType);
    } else if (methodName == "some") {
        return analyzeArraySomeMethod(callExpr, arrayType);
    } else if (methodName == "every") {
        return analyzeArrayEveryMethod(callExpr, arrayType);
    }
    
    return true; // Unknown method, assume valid
}

bool GenericArrayTypesAnalyzer::analyzeArrayPushMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array push method" << std::endl;
    
    auto args = callExpr->getArguments();
    auto elementType = arrayType->getElementType();
    
    // Check if all arguments are compatible with element type
    for (auto& arg : args) {
        if (!isCompatibleWithElementType(arg, elementType)) {
            std::cerr << "Argument not compatible with array element type" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayPopMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array pop method" << std::endl;
    
    // Pop method returns the element type
    auto elementType = arrayType->getElementType();
    callExpr->setInferredType(elementType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayShiftMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array shift method" << std::endl;
    
    // Shift method returns the element type
    auto elementType = arrayType->getElementType();
    callExpr->setInferredType(elementType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayUnshiftMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array unshift method" << std::endl;
    
    auto args = callExpr->getArguments();
    auto elementType = arrayType->getElementType();
    
    // Check if all arguments are compatible with element type
    for (auto& arg : args) {
        if (!isCompatibleWithElementType(arg, elementType)) {
            std::cerr << "Argument not compatible with array element type" << std::endl;
            return false;
        }
    }
    
    // Unshift method returns the new length (number)
    callExpr->setInferredType(createType("number"));
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArraySliceMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array slice method" << std::endl;
    
    // Slice method returns a new array of the same type
    callExpr->setInferredType(arrayType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArraySpliceMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array splice method" << std::endl;
    
    auto args = callExpr->getArguments();
    auto elementType = arrayType->getElementType();
    
    // Check if all arguments after the first two are compatible with element type
    for (size_t i = 2; i < args.size(); ++i) {
        if (!isCompatibleWithElementType(args[i], elementType)) {
            std::cerr << "Argument not compatible with array element type" << std::endl;
            return false;
        }
    }
    
    // Splice method returns an array of removed elements
    callExpr->setInferredType(arrayType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayConcatMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array concat method" << std::endl;
    
    auto args = callExpr->getArguments();
    auto elementType = arrayType->getElementType();
    
    // Check if all arguments are compatible with element type
    for (auto& arg : args) {
        if (!isCompatibleWithElementType(arg, elementType)) {
            std::cerr << "Argument not compatible with array element type" << std::endl;
            return false;
        }
    }
    
    // Concat method returns a new array of the same type
    callExpr->setInferredType(arrayType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayMapMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array map method" << std::endl;
    
    auto args = callExpr->getArguments();
    if (args.empty()) {
        std::cerr << "Map method requires at least one argument" << std::endl;
        return false;
    }
    
    // First argument should be a function
    auto callback = args[0];
    if (!isFunctionType(callback)) {
        std::cerr << "Map callback must be a function" << std::endl;
        return false;
    }
    
    // Map method returns an array of the callback's return type
    auto returnType = getFunctionReturnType(callback);
    if (returnType) {
        auto resultArrayType = createArrayType(returnType);
        callExpr->setInferredType(resultArrayType);
    }
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayFilterMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array filter method" << std::endl;
    
    auto args = callExpr->getArguments();
    if (args.empty()) {
        std::cerr << "Filter method requires at least one argument" << std::endl;
        return false;
    }
    
    // First argument should be a function
    auto callback = args[0];
    if (!isFunctionType(callback)) {
        std::cerr << "Filter callback must be a function" << std::endl;
        return false;
    }
    
    // Filter method returns an array of the same type
    callExpr->setInferredType(arrayType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayReduceMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array reduce method" << std::endl;
    
    auto args = callExpr->getArguments();
    if (args.empty()) {
        std::cerr << "Reduce method requires at least one argument" << std::endl;
        return false;
    }
    
    // First argument should be a function
    auto callback = args[0];
    if (!isFunctionType(callback)) {
        std::cerr << "Reduce callback must be a function" << std::endl;
        return false;
    }
    
    // Reduce method returns the accumulator type
    auto returnType = getFunctionReturnType(callback);
    if (returnType) {
        callExpr->setInferredType(returnType);
    }
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayForEachMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array forEach method" << std::endl;
    
    auto args = callExpr->getArguments();
    if (args.empty()) {
        std::cerr << "ForEach method requires at least one argument" << std::endl;
        return false;
    }
    
    // First argument should be a function
    auto callback = args[0];
    if (!isFunctionType(callback)) {
        std::cerr << "ForEach callback must be a function" << std::endl;
        return false;
    }
    
    // ForEach method returns undefined
    callExpr->setInferredType(createType("undefined"));
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayFindMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array find method" << std::endl;
    
    auto args = callExpr->getArguments();
    if (args.empty()) {
        std::cerr << "Find method requires at least one argument" << std::endl;
        return false;
    }
    
    // First argument should be a function
    auto callback = args[0];
    if (!isFunctionType(callback)) {
        std::cerr << "Find callback must be a function" << std::endl;
        return false;
    }
    
    // Find method returns the element type or undefined
    auto elementType = arrayType->getElementType();
    auto unionType = createUnionType({elementType, createType("undefined")});
    callExpr->setInferredType(unionType);
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArraySomeMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array some method" << std::endl;
    
    auto args = callExpr->getArguments();
    if (args.empty()) {
        std::cerr << "Some method requires at least one argument" << std::endl;
        return false;
    }
    
    // First argument should be a function
    auto callback = args[0];
    if (!isFunctionType(callback)) {
        std::cerr << "Some callback must be a function" << std::endl;
        return false;
    }
    
    // Some method returns boolean
    callExpr->setInferredType(createType("boolean"));
    
    return true;
}

bool GenericArrayTypesAnalyzer::analyzeArrayEveryMethod(CallExpression* callExpr, ArrayType* arrayType) {
    std::cout << "Analyzing array every method" << std::endl;
    
    auto args = callExpr->getArguments();
    if (args.empty()) {
        std::cerr << "Every method requires at least one argument" << std::endl;
        return false;
    }
    
    // First argument should be a function
    auto callback = args[0];
    if (!isFunctionType(callback)) {
        std::cerr << "Every callback must be a function" << std::endl;
        return false;
    }
    
    // Every method returns boolean
    callExpr->setInferredType(createType("boolean"));
    
    return true;
}

Type* GenericArrayTypesAnalyzer::inferArrayElementType(ArrayExpression* arrayExpr) {
    auto elements = arrayExpr->getElements();
    
    if (elements.empty()) {
        // Empty array - return any type
        return createType("any");
    }
    
    // Infer type from first element
    auto firstElement = elements[0];
    return inferTypeFromExpression(firstElement);
}

Type* GenericArrayTypesAnalyzer::inferTypeFromExpression(ASTNode* expr) {
    switch (expr->getType()) {
        case ASTNodeType::Literal:
            return inferTypeFromLiteral(static_cast<Literal*>(expr));
        case ASTNodeType::Identifier:
            return inferTypeFromIdentifier(static_cast<Identifier*>(expr));
        case ASTNodeType::CallExpression:
            return inferTypeFromCallExpression(static_cast<CallExpression*>(expr));
        default:
            return createType("any");
    }
}

Type* GenericArrayTypesAnalyzer::inferTypeFromLiteral(Literal* literal) {
    switch (literal->getValueType()) {
        case LiteralType::Number:
            return createType("number");
        case LiteralType::String:
            return createType("string");
        case LiteralType::Boolean:
            return createType("boolean");
        case LiteralType::Null:
            return createType("null");
        case LiteralType::Undefined:
            return createType("undefined");
        default:
            return createType("any");
    }
}

Type* GenericArrayTypesAnalyzer::inferTypeFromIdentifier(Identifier* identifier) {
    // Look up identifier in symbol table
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(identifier->getName());
    
    if (symbol && symbol->getType()) {
        return symbol->getType();
    }
    
    return createType("any");
}

Type* GenericArrayTypesAnalyzer::inferTypeFromCallExpression(CallExpression* callExpr) {
    // Get return type of called function
    // This is simplified - in a real implementation, this would be more complex
    return createType("any");
}

bool GenericArrayTypesAnalyzer::validateGenericArrayElementType(GenericType* genericType) {
    // Validate that the generic type can be used as an array element
    return true;
}

bool GenericArrayTypesAnalyzer::validateArrayElementType(Type* elementType) {
    // Validate that the element type is valid
    return true;
}

bool GenericArrayTypesAnalyzer::isArrayLikeGenericType(GenericType* genericType) {
    return genericType->getName() == "Array" || 
           genericType->getName() == "ReadonlyArray" ||
           couldBeUsedAsArray(genericType);
}

bool GenericArrayTypesAnalyzer::couldBeUsedAsArray(GenericType* genericType) {
    // Check if this generic type could be used as an array
    // This would involve checking if it has array-like methods or properties
    return false;
}

bool GenericArrayTypesAnalyzer::isArrayMethodCall(CallExpression* callExpr) {
    auto callee = callExpr->getCallee();
    if (!callee || callee->getType() != ASTNodeType::MemberExpression) {
        return false;
    }
    
    auto memberExpr = static_cast<MemberExpression*>(callee);
    auto property = memberExpr->getProperty();
    
    if (property->getType() != ASTNodeType::Identifier) {
        return false;
    }
    
    auto methodName = static_cast<Identifier*>(property)->getName();
    
    // Check if this is a known array method
    return isArrayMethod(methodName);
}

bool GenericArrayTypesAnalyzer::isArrayMethod(const std::string& methodName) {
    static const std::vector<std::string> arrayMethods = {
        "push", "pop", "shift", "unshift", "slice", "splice", "concat",
        "map", "filter", "reduce", "forEach", "find", "some", "every",
        "indexOf", "lastIndexOf", "includes", "join", "reverse", "sort"
    };
    
    return std::find(arrayMethods.begin(), arrayMethods.end(), methodName) != arrayMethods.end();
}

ArrayType* GenericArrayTypesAnalyzer::getArrayType(ASTNode* arrayObject) {
    // Get the type of the array object
    // This is simplified - in a real implementation, this would be more complex
    
    if (arrayObject->getInferredType()) {
        auto type = arrayObject->getInferredType();
        if (type->getType() == TypeType::ArrayType) {
            return static_cast<ArrayType*>(type);
        }
    }
    
    return nullptr;
}

bool GenericArrayTypesAnalyzer::isCompatibleWithElementType(ASTNode* arg, Type* elementType) {
    // Check if argument is compatible with element type
    // This is simplified - in a real implementation, this would be more complex
    
    auto argType = inferTypeFromExpression(arg);
    return areTypesCompatible(argType, elementType);
}

bool GenericArrayTypesAnalyzer::areTypesCompatible(Type* type1, Type* type2) {
    // Simplified type compatibility checking
    return type1->getName() == type2->getName();
}

bool GenericArrayTypesAnalyzer::isFunctionType(ASTNode* node) {
    // Check if node represents a function type
    // This is simplified - in a real implementation, this would be more complex
    return false;
}

Type* GenericArrayTypesAnalyzer::getFunctionReturnType(ASTNode* function) {
    // Get the return type of a function
    // This is simplified - in a real implementation, this would be more complex
    return createType("any");
}

Type* GenericArrayTypesAnalyzer::createType(const std::string& name) {
    // Create a simple type
    // In a real implementation, this would create proper Type objects
    return new Type(name);
}

ArrayType* GenericArrayTypesAnalyzer::createArrayType(Type* elementType) {
    // Create an array type
    // In a real implementation, this would create proper ArrayType objects
    return new ArrayType(elementType);
}

UnionType* GenericArrayTypesAnalyzer::createUnionType(const std::vector<Type*>& types) {
    // Create a union type
    // In a real implementation, this would create proper UnionType objects
    return new UnionType(types);
}

} // namespace semantic
} // namespace tsc