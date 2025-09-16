#include "tsc/codegen/types/TypeGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"

namespace tsc {
namespace codegen {

TypeGenerator::TypeGenerator(LLVMCodeGen* codeGen) 
    : codeGen_(codeGen), context_(codeGen->getLLVMContext()) {
}

llvm::Type* TypeGenerator::generateType(const shared_ptr<Type>& type) {
    if (!type) {
        return getAnyType();
    }
    return mapTypeScriptTypeToLLVM(*type);
}

llvm::Type* TypeGenerator::generatePrimitiveType(const shared_ptr<Type>& type) {
    return mapTypeScriptTypeToLLVM(*type);
}

llvm::Type* TypeGenerator::generateArrayType(const shared_ptr<Type>& type) {
    // TODO: Implement array type generation
    return getAnyType();
}

llvm::Type* TypeGenerator::generateObjectType(const shared_ptr<Type>& type) {
    // TODO: Implement object type generation
    return getAnyType();
}

llvm::Type* TypeGenerator::generateFunctionType(const shared_ptr<Type>& type) {
    if (auto functionType = std::dynamic_pointer_cast<FunctionType>(type)) {
        return convertFunctionTypeToLLVM(*functionType);
    }
    return getAnyType();
}

llvm::Type* TypeGenerator::generateUnionType(const shared_ptr<Type>& type) {
    // TODO: Implement union type generation
    return getAnyType();
}

llvm::Type* TypeGenerator::generateGenericType(const shared_ptr<Type>& type) {
    if (auto genericType = std::dynamic_pointer_cast<GenericType>(type)) {
        return createMonomorphizedType(*genericType);
    }
    return getAnyType();
}

llvm::Type* TypeGenerator::mapTypeScriptTypeToLLVM(const Type& type) {
    switch (type.getKind()) {
        case TypeKind::Number:
            return getNumberType();
        case TypeKind::String:
            return getStringType();
        case TypeKind::Boolean:
            return getBooleanType();
        case TypeKind::Void:
            return getVoidType();
        case TypeKind::Class:
            // For classes, return a pointer to a struct (simplified)
            if (auto classType = dynamic_cast<const ClassType*>(&type)) {
                llvm::StructType* classStruct = 
                    llvm::StructType::create(*context_, "Class_" + classType->getName());
                return llvm::PointerType::get(classStruct, 0);
            }
            return getAnyType();
        case TypeKind::Generic:
            // For generic types, implement basic monomorphization
            if (auto genericType = dynamic_cast<const GenericType*>(&type)) {
                return createMonomorphizedType(*genericType);
            }
            return getAnyType();
        case TypeKind::Union:
            // For now, treat union types as 'any' type (void*)
            return getAnyType();
        case TypeKind::TypeParameter:
            // For now, treat type parameters as 'any' type (void*)
            return getAnyType();
        case TypeKind::Function:
            // Convert function type to LLVM function type
            if (auto functionType = dynamic_cast<const FunctionType*>(&type)) {
                return convertFunctionTypeToLLVM(*functionType);
            }
            return getAnyType();
        case TypeKind::Any:
        default:
            return getAnyType();
    }
}

llvm::Type* TypeGenerator::getNumberType() const {
    return llvm::Type::getDoubleTy(*context_);
}

llvm::Type* TypeGenerator::getStringType() const {
    // Use i8* for strings (C-style strings for now)
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0);
}

llvm::Type* TypeGenerator::getBooleanType() const {
    return llvm::Type::getInt1Ty(*context_);
}

llvm::Type* TypeGenerator::getVoidType() const {
    return llvm::Type::getVoidTy(*context_);
}

llvm::Type* TypeGenerator::getAnyType() const {
    // Use i8* as a generic pointer type for 'any'
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0);
}

llvm::Type* TypeGenerator::convertFunctionTypeToLLVM(const FunctionType& functionType) {
    // Convert parameter types
    std::vector<llvm::Type*> paramTypes;
    for (const auto& param : functionType.getParameters()) {
        paramTypes.push_back(mapTypeScriptTypeToLLVM(*param.type));
    }

    // Convert return type
    llvm::Type* returnType = mapTypeScriptTypeToLLVM(*functionType.getReturnType());

    // Create LLVM function type
    return llvm::FunctionType::get(returnType, paramTypes, false);
}

llvm::Type* TypeGenerator::convertTypeToLLVM(shared_ptr<Type> type) {
    if (!type) {
        return getAnyType();
    }
    return mapTypeScriptTypeToLLVM(*type);
}

llvm::Type* TypeGenerator::generateGenericType(const shared_ptr<GenericType>& genericType, 
                                              const std::vector<llvm::Type*>& typeArguments) {
    // TODO: Implement generic type generation with type arguments
    return getAnyType();
}

llvm::Type* TypeGenerator::generateSpecializedClass(const shared_ptr<GenericType>& genericType,
                                                   const std::vector<llvm::Type*>& typeArguments) {
    // TODO: Implement specialized class generation
    return getAnyType();
}

llvm::Type* TypeGenerator::specializeType(const shared_ptr<Type>& type,
                                        const std::unordered_map<String, llvm::Type*>& typeMap) {
    // TODO: Implement type specialization
    return getAnyType();
}

llvm::Type* TypeGenerator::createMonomorphizedType(const GenericType& genericType) {
    // For now, implement basic monomorphization for generic classes
    auto baseType = genericType.getBaseType();
    if (baseType->getKind() == TypeKind::Class) {
        return createMonomorphizedStruct(genericType);
    }

    // For other generic types, fall back to any type for now
    return getAnyType();
}

String TypeGenerator::generateMangledName(const GenericType& genericType) {
    auto baseType = genericType.getBaseType();
    String mangledName = baseType->toString();

    // Add type arguments to the mangled name
    auto typeArgs = genericType.getTypeArguments();
    if (!typeArgs.empty()) {
        mangledName += "_";
        for (size_t i = 0; i < typeArgs.size(); ++i) {
            if (i > 0) mangledName += "_";

            // Generate a simple mangled name for the type argument
            switch (typeArgs[i]->getKind()) {
                case TypeKind::Number:
                    mangledName += "number";
                    break;
                case TypeKind::String:
                    mangledName += "string";
                    break;
                case TypeKind::Boolean:
                    mangledName += "boolean";
                    break;
                case TypeKind::Class:
                    if (auto classType = dynamic_cast<const ClassType*>(typeArgs[i].get())) {
                        mangledName += classType->getName();
                    } else {
                        mangledName += "class";
                    }
                    break;
                default:
                    mangledName += "unknown";
                    break;
            }
        }
    }

    return mangledName;
}

llvm::StructType* TypeGenerator::createMonomorphizedStruct(const GenericType& genericType) {
    auto baseType = genericType.getBaseType();
    if (baseType->getKind() != TypeKind::Class) {
        return nullptr;
    }

    auto classType = std::static_pointer_cast<ClassType>(baseType);
    String mangledName = generateMangledName(genericType);

    // Create the monomorphized struct type
    std::vector<llvm::Type*> memberTypes;

    // Get the class declaration to access properties
    ClassDeclaration* classDecl = classType->getDeclaration();
    if (!classDecl) {
        // Fallback: create a simple struct
        return llvm::StructType::create(*context_, {getAnyType()}, mangledName);
    }

    // Create type parameter substitution map
    std::unordered_map<String, shared_ptr<Type>> substitutions;
    auto typeArgs = genericType.getTypeArguments();
    const auto& typeParams = classDecl->getTypeParameters();

    for (size_t i = 0; i < typeArgs.size() && i < typeParams.size(); ++i) {
        substitutions[typeParams[i]->getName()] = typeArgs[i];
    }

    // Process properties with type parameter substitution
    for (const auto& property : classDecl->getProperties()) {
        llvm::Type* propertyType = getAnyType(); // Default fallback

        if (property->getType()) {
            // Implement proper type parameter substitution
            shared_ptr<Type> substitutedType = property->getType();

            // Check if this property type contains type parameters
            if (property->getType()->getKind() == TypeKind::TypeParameter) {
                // This is a type parameter, substitute it
                String paramName = property->getType()->toString();
                auto it = substitutions.find(paramName);
                if (it != substitutions.end()) {
                    substitutedType = it->second;
                }
            }

            propertyType = mapTypeScriptTypeToLLVM(*substitutedType);
        }

        memberTypes.push_back(propertyType);
    }

    // Create the struct type
    return llvm::StructType::create(*context_, memberTypes, mangledName);
}

String TypeGenerator::generateMangledMethodName(const GenericType& genericType, const String& methodName) {
    String baseName = generateMangledName(genericType);
    return baseName + "_" + methodName;
}

llvm::Value* TypeGenerator::convertValueToType(llvm::Value* value, llvm::Type* targetType) {
    // TODO: Implement value type conversion
    return value;
}

llvm::StructType* TypeGenerator::getOrCreateStructType(const std::vector<llvm::Type*>& fieldTypes) {
    // TODO: Implement struct type caching
    return llvm::StructType::create(*context_, fieldTypes);
}

} // namespace codegen
} // namespace tsc