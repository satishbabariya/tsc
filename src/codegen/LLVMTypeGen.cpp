#include "tsc/codegen/LLVMTypeGen.h"
#include "tsc/codegen/LLVMCodeGen.h"
#include "tsc/utils/Logger.h"

namespace tsc {

LLVMTypeGen::LLVMTypeGen(CodeGenContext& context) : context_(context) {
}

llvm::Type* LLVMTypeGen::mapTypeScriptTypeToLLVM(const Type& type) {
    TSC_LOG_DEBUG("Mapping TypeScript type to LLVM: " + type.toString(), "TypeGen");
    
    switch (type.getKind()) {
        case TypeKind::Primitive:
            return mapPrimitiveType(static_cast<const PrimitiveType&>(type));
        case TypeKind::Object:
            return mapObjectType(static_cast<const ObjectType&>(type));
        case TypeKind::Array:
            return mapArrayType(static_cast<const ArrayType&>(type));
        case TypeKind::Function:
            return mapFunctionType(static_cast<const FunctionType&>(type));
        case TypeKind::Generic:
            return mapGenericType(static_cast<const GenericType&>(type));
        case TypeKind::Union:
            return mapUnionType(static_cast<const UnionType&>(type));
        case TypeKind::Intersection:
            return mapIntersectionType(static_cast<const IntersectionType&>(type));
        default:
            TSC_LOG_ERROR("Unknown type kind: " + std::to_string(static_cast<int>(type.getKind())), "TypeGen");
            return getAnyType();
    }
}

llvm::Type* LLVMTypeGen::convertTypeToLLVM(shared_ptr<Type> type) {
    if (!type) {
        TSC_LOG_WARN("Converting null type to LLVM", "TypeGen");
        return getAnyType();
    }
    
    return mapTypeScriptTypeToLLVM(*type);
}

llvm::FunctionType* LLVMTypeGen::convertFunctionTypeToLLVM(const FunctionType& functionType) {
    TSC_LOG_DEBUG("Converting function type to LLVM", "TypeGen");
    
    // Convert return type
    llvm::Type* returnType = mapTypeScriptTypeToLLVM(functionType.getReturnType());
    
    // Convert parameter types
    std::vector<llvm::Type*> parameterTypes;
    for (const auto& param : functionType.getParameters()) {
        parameterTypes.push_back(mapTypeScriptTypeToLLVM(param.type));
    }
    
    // Create function type
    return llvm::FunctionType::get(returnType, parameterTypes, functionType.isVarArg());
}

llvm::Type* LLVMTypeGen::getNumberType() const {
    return llvm::Type::getDoubleTy(context_.getLLVMContext());
}

llvm::Type* LLVMTypeGen::getStringType() const {
    return llvm::Type::getInt8PtrTy(context_.getLLVMContext());
}

llvm::Type* LLVMTypeGen::getBooleanType() const {
    return llvm::Type::getInt1Ty(context_.getLLVMContext());
}

llvm::Type* LLVMTypeGen::getVoidType() const {
    return llvm::Type::getVoidTy(context_.getLLVMContext());
}

llvm::Type* LLVMTypeGen::getAnyType() const {
    return llvm::Type::getInt8PtrTy(context_.getLLVMContext());
}

llvm::Type* LLVMTypeGen::createMonomorphizedType(const GenericType& genericType) {
    TSC_LOG_DEBUG("Creating monomorphized type for: " + genericType.toString(), "TypeGen");
    
    // For now, return a generic object type
    // TODO: Implement proper monomorphization
    return getAnyType();
}

llvm::StructType* LLVMTypeGen::createMonomorphizedStruct(const GenericType& genericType) {
    TSC_LOG_DEBUG("Creating monomorphized struct for: " + genericType.toString(), "TypeGen");
    
    // For now, return a generic struct type
    // TODO: Implement proper struct monomorphization
    return llvm::StructType::get(context_.getLLVMContext());
}

llvm::Type* LLVMTypeGen::createArrayType(llvm::Type* elementType) {
    TSC_LOG_DEBUG("Creating array type", "TypeGen");
    
    // Create a struct with count and data pointer
    std::vector<llvm::Type*> fields = {
        llvm::Type::getInt32Ty(context_.getLLVMContext()), // count
        llvm::Type::getInt8PtrTy(context_.getLLVMContext()) // data pointer
    };
    
    return llvm::StructType::get(context_.getLLVMContext(), fields);
}

llvm::Type* LLVMTypeGen::createFixedArrayType(llvm::Type* elementType, size_t size) {
    TSC_LOG_DEBUG("Creating fixed array type with size: " + std::to_string(size), "TypeGen");
    
    return llvm::ArrayType::get(elementType, size);
}

llvm::StructType* LLVMTypeGen::createObjectType(const String& name, const std::vector<std::pair<String, llvm::Type*>>& fields) {
    TSC_LOG_DEBUG("Creating object type: " + name, "TypeGen");
    
    std::vector<llvm::Type*> fieldTypes;
    for (const auto& field : fields) {
        fieldTypes.push_back(field.second);
    }
    
    return llvm::StructType::create(context_.getLLVMContext(), fieldTypes, name);
}

llvm::StructType* LLVMTypeGen::createClassType(const String& name, const std::vector<ClassMember>& members) {
    TSC_LOG_DEBUG("Creating class type: " + name, "TypeGen");
    
    std::vector<llvm::Type*> fieldTypes;
    for (const auto& member : members) {
        if (member.isProperty()) {
            // TODO: Map property type to LLVM type
            fieldTypes.push_back(getAnyType());
        }
    }
    
    return llvm::StructType::create(context_.getLLVMContext(), fieldTypes, name);
}

llvm::FunctionType* LLVMTypeGen::createFunctionType(llvm::Type* returnType, const std::vector<llvm::Type*>& parameterTypes, bool isVarArg) {
    TSC_LOG_DEBUG("Creating function type", "TypeGen");
    
    return llvm::FunctionType::get(returnType, parameterTypes, isVarArg);
}

llvm::Value* LLVMTypeGen::convertValueToType(llvm::Value* value, llvm::Type* targetType) {
    if (!value || !targetType) {
        return nullptr;
    }
    
    if (value->getType() == targetType) {
        return value;
    }
    
    TSC_LOG_DEBUG("Converting value to type", "TypeGen");
    
    // TODO: Implement proper type conversion
    return value;
}

bool LLVMTypeGen::isTypeCompatible(llvm::Type* from, llvm::Type* to) const {
    if (!from || !to) {
        return false;
    }
    
    return from == to;
}

void LLVMTypeGen::cacheStructType(const String& name, llvm::StructType* type) {
    structTypeCache_[name] = type;
}

llvm::StructType* LLVMTypeGen::getCachedStructType(const String& name) const {
    auto it = structTypeCache_.find(name);
    return (it != structTypeCache_.end()) ? it->second : nullptr;
}

// Private helper methods

llvm::Type* LLVMTypeGen::mapPrimitiveType(const PrimitiveType& type) {
    switch (type.getPrimitiveKind()) {
        case PrimitiveKind::Number:
            return getNumberType();
        case PrimitiveKind::String:
            return getStringType();
        case PrimitiveKind::Boolean:
            return getBooleanType();
        case PrimitiveKind::Void:
            return getVoidType();
        default:
            return getAnyType();
    }
}

llvm::Type* LLVMTypeGen::mapObjectType(const ObjectType& type) {
    // TODO: Implement object type mapping
    return getAnyType();
}

llvm::Type* LLVMTypeGen::mapArrayType(const ArrayType& type) {
    llvm::Type* elementType = mapTypeScriptTypeToLLVM(type.getElementType());
    return createArrayType(elementType);
}

llvm::Type* LLVMTypeGen::mapFunctionType(const FunctionType& type) {
    return convertFunctionTypeToLLVM(type);
}

llvm::Type* LLVMTypeGen::mapGenericType(const GenericType& type) {
    return createMonomorphizedType(type);
}

llvm::Type* LLVMTypeGen::mapUnionType(const UnionType& type) {
    // TODO: Implement union type mapping
    return getAnyType();
}

llvm::Type* LLVMTypeGen::mapIntersectionType(const IntersectionType& type) {
    // TODO: Implement intersection type mapping
    return getAnyType();
}

llvm::StructType* LLVMTypeGen::createStructType(const String& name, const std::vector<llvm::Type*>& fieldTypes) {
    return llvm::StructType::create(context_.getLLVMContext(), fieldTypes, name);
}

llvm::PointerType* LLVMTypeGen::createPointerType(llvm::Type* pointeeType) {
    return llvm::PointerType::get(pointeeType, 0);
}

bool LLVMTypeGen::validateType(llvm::Type* type) const {
    return type != nullptr;
}

void LLVMTypeGen::reportTypeError(const String& message, const SourceLocation& location) {
    TSC_LOG_ERROR("Type error: " + message, "TypeGen");
    context_.reportError(message, location);
}

} // namespace tsc