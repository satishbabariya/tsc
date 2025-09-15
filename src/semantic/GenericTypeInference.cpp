#include "tsc/semantic/GenericTypeInference.h"
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
// GENERIC TYPE INFERENCE IMPLEMENTATION
// =============================================================================

GenericTypeInference::GenericTypeInference(SemanticAnalyzer* analyzer)
    : analyzer_(analyzer) {
}

std::vector<Type*> GenericTypeInference::inferTypeArguments(CallExpression* callExpr) {
    std::cout << "Inferring type arguments for generic call" << std::endl;
    
    // Get the function/method being called
    auto callee = callExpr->getCallee();
    if (!callee) {
        std::cerr << "No callee in call expression" << std::endl;
        return {};
    }
    
    // Get function/method name
    std::string name = getCalleeName(callee);
    if (name.empty()) {
        std::cerr << "Could not determine callee name" << std::endl;
        return {};
    }
    
    // Get the generic function/method
    auto genericFunction = getGenericFunction(name);
    if (!genericFunction) {
        std::cerr << "Generic function not found: " << name << std::endl;
        return {};
    }
    
    // Get type parameters
    auto typeParams = getTypeParameters(genericFunction);
    if (typeParams.empty()) {
        std::cerr << "No type parameters found for generic function: " << name << std::endl;
        return {};
    }
    
    // Infer type arguments from call arguments
    return inferTypeArgumentsFromCall(callExpr, typeParams);
}

std::vector<Type*> GenericTypeInference::inferTypeArgumentsFromCall(CallExpression* callExpr, const std::vector<TypeParameter*>& typeParams) {
    std::vector<Type*> inferredTypes;
    
    // Get call arguments
    auto args = callExpr->getArguments();
    
    // Get function parameters
    auto params = getParameters(callExpr);
    if (params.size() != args.size()) {
        std::cerr << "Parameter count mismatch" << std::endl;
        return {};
    }
    
    // Create type inference context
    TypeInferenceContext context;
    
    // Infer types from parameter-argument pairs
    for (size_t i = 0; i < params.size(); ++i) {
        if (!inferTypeFromParameterArgument(params[i], args[i], context)) {
            std::cerr << "Failed to infer type from parameter-argument pair " << i << std::endl;
            return {};
        }
    }
    
    // Extract inferred types for type parameters
    for (auto& typeParam : typeParams) {
        auto inferredType = context.getInferredType(typeParam->getName());
        if (inferredType) {
            inferredTypes.push_back(inferredType);
        } else {
            std::cerr << "Could not infer type for parameter: " << typeParam->getName() << std::endl;
            return {};
        }
    }
    
    return inferredTypes;
}

bool GenericTypeInference::inferTypeFromParameterArgument(Parameter* param, ASTNode* arg, TypeInferenceContext& context) {
    // Check if parameter has a generic type
    if (param->getType() && param->getType()->getType() == TypeType::GenericType) {
        auto genericType = static_cast<GenericType*>(param->getType());
        auto typeParamName = genericType->getName();
        
        // Infer type from argument
        auto inferredType = inferTypeFromArgument(arg);
        if (inferredType) {
            context.setInferredType(typeParamName, inferredType);
            return true;
        }
    }
    
    // Check if parameter type contains generic types
    if (param->getType()) {
        return inferTypeFromComplexType(param->getType(), arg, context);
    }
    
    return true; // No generic types to infer
}

bool GenericTypeInference::inferTypeFromComplexType(Type* paramType, ASTNode* arg, TypeInferenceContext& context) {
    switch (paramType->getType()) {
        case TypeType::ArrayType:
            return inferTypeFromArrayType(static_cast<ArrayType*>(paramType), arg, context);
        case TypeType::UnionType:
            return inferTypeFromUnionType(static_cast<UnionType*>(paramType), arg, context);
        case TypeType::IntersectionType:
            return inferTypeFromIntersectionType(static_cast<IntersectionType*>(paramType), arg, context);
        case TypeType::GenericType:
            return inferTypeFromGenericType(static_cast<GenericType*>(paramType), arg, context);
        default:
            return true;
    }
}

bool GenericTypeInference::inferTypeFromArrayType(ArrayType* arrayType, ASTNode* arg, TypeInferenceContext& context) {
    // Check if argument is an array
    if (arg->getType() == ASTNodeType::ArrayExpression) {
        auto arrayExpr = static_cast<ArrayExpression*>(arg);
        auto elements = arrayExpr->getElements();
        
        if (!elements.empty()) {
            // Infer type from first element
            auto elementType = arrayType->getElementType();
            if (elementType && elementType->getType() == TypeType::GenericType) {
                auto genericType = static_cast<GenericType*>(elementType);
                auto typeParamName = genericType->getName();
                
                auto inferredType = inferTypeFromArgument(elements[0]);
                if (inferredType) {
                    context.setInferredType(typeParamName, inferredType);
                    return true;
                }
            }
        }
    }
    
    return true;
}

bool GenericTypeInference::inferTypeFromUnionType(UnionType* unionType, ASTNode* arg, TypeInferenceContext& context) {
    // Try to infer from each union member
    for (auto& type : unionType->getTypes()) {
        if (inferTypeFromComplexType(type, arg, context)) {
            return true;
        }
    }
    
    return false;
}

bool GenericTypeInference::inferTypeFromIntersectionType(IntersectionType* intersectionType, ASTNode* arg, TypeInferenceContext& context) {
    // Infer from all intersection members
    bool success = true;
    for (auto& type : intersectionType->getTypes()) {
        if (!inferTypeFromComplexType(type, arg, context)) {
            success = false;
        }
    }
    
    return success;
}

bool GenericTypeInference::inferTypeFromGenericType(GenericType* genericType, ASTNode* arg, TypeInferenceContext& context) {
    auto typeParamName = genericType->getName();
    
    // Infer type from argument
    auto inferredType = inferTypeFromArgument(arg);
    if (inferredType) {
        context.setInferredType(typeParamName, inferredType);
        return true;
    }
    
    return false;
}

Type* GenericTypeInference::inferTypeFromArgument(ASTNode* arg) {
    switch (arg->getType()) {
        case ASTNodeType::Literal:
            return inferTypeFromLiteral(static_cast<Literal*>(arg));
        case ASTNodeType::Identifier:
            return inferTypeFromIdentifier(static_cast<Identifier*>(arg));
        case ASTNodeType::ArrayExpression:
            return inferTypeFromArrayExpression(static_cast<ArrayExpression*>(arg));
        case ASTNodeType::ObjectExpression:
            return inferTypeFromObjectExpression(static_cast<ObjectExpression*>(arg));
        case ASTNodeType::CallExpression:
            return inferTypeFromCallExpression(static_cast<CallExpression*>(arg));
        default:
            return nullptr;
    }
}

Type* GenericTypeInference::inferTypeFromLiteral(Literal* literal) {
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
            return nullptr;
    }
}

Type* GenericTypeInference::inferTypeFromIdentifier(Identifier* identifier) {
    // Look up identifier in symbol table
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(identifier->getName());
    
    if (symbol && symbol->getType()) {
        return symbol->getType();
    }
    
    return nullptr;
}

Type* GenericTypeInference::inferTypeFromArrayExpression(ArrayExpression* arrayExpr) {
    auto elements = arrayExpr->getElements();
    
    if (elements.empty()) {
        // Empty array - infer as any[]
        return createArrayType(createType("any"));
    }
    
    // Infer type from first element
    auto elementType = inferTypeFromArgument(elements[0]);
    if (elementType) {
        return createArrayType(elementType);
    }
    
    return nullptr;
}

Type* GenericTypeInference::inferTypeFromObjectExpression(ObjectExpression* objectExpr) {
    // Create object type from properties
    auto properties = objectExpr->getProperties();
    
    // For now, return a generic object type
    // In a real implementation, this would create a more specific object type
    return createType("object");
}

Type* GenericTypeInference::inferTypeFromCallExpression(CallExpression* callExpr) {
    // Get return type of called function
    auto callee = callExpr->getCallee();
    if (!callee) {
        return nullptr;
    }
    
    std::string name = getCalleeName(callee);
    if (name.empty()) {
        return nullptr;
    }
    
    // Look up function in symbol table
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(name);
    
    if (symbol && symbol->getType()) {
        // Get return type from function type
        return getReturnType(symbol->getType());
    }
    
    return nullptr;
}

Type* GenericTypeInference::getReturnType(Type* functionType) {
    // Simplified return type extraction
    // In a real implementation, this would properly extract the return type
    // from a function type signature
    
    if (functionType->getName() == "function") {
        // For now, return any type
        return createType("any");
    }
    
    return nullptr;
}

Type* GenericTypeInference::createType(const std::string& name) {
    // Create a simple type
    // In a real implementation, this would create proper Type objects
    return new Type(name);
}

Type* GenericTypeInference::createArrayType(Type* elementType) {
    // Create an array type
    // In a real implementation, this would create proper ArrayType objects
    return new ArrayType(elementType);
}

std::string GenericTypeInference::getCalleeName(ASTNode* callee) {
    switch (callee->getType()) {
        case ASTNodeType::Identifier:
            return static_cast<Identifier*>(callee)->getName();
        case ASTNodeType::MemberExpression:
            return getMemberExpressionName(static_cast<MemberExpression*>(callee));
        default:
            return "";
    }
}

std::string GenericTypeInference::getMemberExpressionName(MemberExpression* memberExpr) {
    auto property = memberExpr->getProperty();
    if (property && property->getType() == ASTNodeType::Identifier) {
        return static_cast<Identifier*>(property)->getName();
    }
    return "";
}

ASTNode* GenericTypeInference::getGenericFunction(const std::string& name) {
    // Look up generic function in symbol table
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(name);
    
    if (symbol && symbol->getNode()) {
        auto node = symbol->getNode();
        if (isGeneric(node)) {
            return node;
        }
    }
    
    return nullptr;
}

bool GenericTypeInference::isGeneric(ASTNode* node) {
    switch (node->getType()) {
        case ASTNodeType::MethodDeclaration:
            return static_cast<MethodDeclaration*>(node)->isGeneric();
        case ASTNodeType::FunctionDeclaration:
            return static_cast<FunctionDeclaration*>(node)->isGeneric();
        default:
            return false;
    }
}

std::vector<TypeParameter*> GenericTypeInference::getTypeParameters(ASTNode* node) {
    switch (node->getType()) {
        case ASTNodeType::MethodDeclaration:
            return static_cast<MethodDeclaration*>(node)->getTypeParameters();
        case ASTNodeType::FunctionDeclaration:
            return static_cast<FunctionDeclaration*>(node)->getTypeParameters();
        default:
            return {};
    }
}

std::vector<Parameter*> GenericTypeInference::getParameters(CallExpression* callExpr) {
    // Get parameters from the called function
    auto callee = callExpr->getCallee();
    if (!callee) {
        return {};
    }
    
    std::string name = getCalleeName(callee);
    if (name.empty()) {
        return {};
    }
    
    // Look up function in symbol table
    auto symbolTable = analyzer_->getSymbolTable();
    auto symbol = symbolTable->lookupSymbol(name);
    
    if (symbol && symbol->getNode()) {
        auto node = symbol->getNode();
        switch (node->getType()) {
            case ASTNodeType::MethodDeclaration:
                return static_cast<MethodDeclaration*>(node)->getParameters();
            case ASTNodeType::FunctionDeclaration:
                return static_cast<FunctionDeclaration*>(node)->getParameters();
            default:
                break;
        }
    }
    
    return {};
}

// =============================================================================
// TYPE INFERENCE CONTEXT
// =============================================================================

TypeInferenceContext::TypeInferenceContext() {
}

void TypeInferenceContext::setInferredType(const std::string& typeParamName, Type* type) {
    inferredTypes_[typeParamName] = type;
}

Type* TypeInferenceContext::getInferredType(const std::string& typeParamName) {
    auto it = inferredTypes_.find(typeParamName);
    if (it != inferredTypes_.end()) {
        return it->second;
    }
    return nullptr;
}

bool TypeInferenceContext::hasInferredType(const std::string& typeParamName) {
    return inferredTypes_.find(typeParamName) != inferredTypes_.end();
}

void TypeInferenceContext::clear() {
    inferredTypes_.clear();
}

} // namespace semantic
} // namespace tsc