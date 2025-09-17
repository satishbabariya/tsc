#include "tsc/codegen/types/TypeGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {
namespace codegen {

TypeGenerator::TypeGenerator(LLVMCodeGen* codeGen) 
    : codeGen_(codeGen) {
}

llvm::Type* TypeGenerator::getStringType() const {
    // Use i8* for strings (C-style strings for now)
    llvm::Type *stringType = llvm::PointerType::get(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()), 0);
    std::cout << "DEBUG: getStringType() returning type: " << stringType->getTypeID() << std::endl;
    return stringType;
}

llvm::Type* TypeGenerator::getNumberType() const {
    // Use double for all numeric types (simplified)
    return llvm::Type::getDoubleTy(*codeGen_->getLLVMContext());
}

llvm::Type* TypeGenerator::getBooleanType() const {
    return llvm::Type::getInt1Ty(*codeGen_->getLLVMContext());
}

llvm::Type* TypeGenerator::getVoidType() const {
    return llvm::Type::getVoidTy(*codeGen_->getLLVMContext());
}

llvm::Type* TypeGenerator::getAnyType() const {
    // Use i8* as a generic pointer type for 'any'
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()), 0);
}

llvm::Type* TypeGenerator::getNullType() const {
    // Null is represented as a pointer type
    return getAnyType();
}

llvm::Type* TypeGenerator::getUndefinedType() const {
    // Undefined is represented as a pointer type
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
            
        case TypeKind::Any:
            return getAnyType();
            
        case TypeKind::Null:
            return getNullType();
            
        case TypeKind::Undefined:
            return getUndefinedType();
            
        case TypeKind::Array:
            return mapArrayType(static_cast<const ArrayType&>(type));
            
        case TypeKind::Function:
            return mapFunctionType(static_cast<const FunctionType&>(type));
            
        case TypeKind::Class:
            return mapClassType(static_cast<const ClassType&>(type));
            
        case TypeKind::Interface:
            return mapInterfaceType(static_cast<const InterfaceType&>(type));
            
        case TypeKind::Union:
            return mapUnionType(static_cast<const UnionType&>(type));
            
        case TypeKind::Generic:
            return mapGenericType(static_cast<const GenericType&>(type));
            
        case TypeKind::Object:
            return mapObjectType(static_cast<const ObjectType&>(type));
            
        default:
            std::cout << "DEBUG: Unknown type kind: " << static_cast<int>(type.getKind()) << std::endl;
            return getAnyType(); // Fallback
    }
}

llvm::Type* TypeGenerator::mapArrayType(const ArrayType& arrayType) {
    // Get element type
    llvm::Type* elementType = mapTypeScriptTypeToLLVM(*arrayType.getElementType());
    
    // For now, represent arrays as a simple struct with pointer and length
    std::vector<llvm::Type*> fields = {
        llvm::PointerType::get(elementType, 0), // data pointer
        llvm::Type::getInt64Ty(*codeGen_->getLLVMContext()) // length
    };
    
    return llvm::StructType::get(*codeGen_->getLLVMContext(), fields);
}

llvm::Type* TypeGenerator::mapFunctionType(const FunctionType& functionType) {
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

llvm::Type* TypeGenerator::mapClassType(const ClassType& classType) {
    // Check if we've already created this class type
    auto it = classTypeCache_.find(classType.getName());
    if (it != classTypeCache_.end()) {
        return it->second;
    }
    
    // Create struct type for class
    std::vector<llvm::Type*> fields;
    
    // Add fields from class properties
    for (const auto& property : classType.getProperties()) {
        llvm::Type* fieldType = mapTypeScriptTypeToLLVM(*property.second);
        fields.push_back(fieldType);
    }
    
    // If no fields, add a dummy field to avoid empty struct
    if (fields.empty()) {
        fields.push_back(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()));
    }
    
    // Create the struct type
    llvm::StructType* structType = llvm::StructType::create(*codeGen_->getLLVMContext(), 
                                                            fields, 
                                                            classType.getName());
    
    // Cache the type
    classTypeCache_[classType.getName()] = structType;
    
    return structType;
}

llvm::Type* TypeGenerator::mapInterfaceType(const InterfaceType& interfaceType) {
    // Interfaces are similar to classes but may have different representation
    // For now, treat them similarly to classes
    std::vector<llvm::Type*> fields;
    
    // Add fields from interface properties
    for (const auto& property : interfaceType.getProperties()) {
        llvm::Type* fieldType = mapTypeScriptTypeToLLVM(*property.second);
        fields.push_back(fieldType);
    }
    
    // If no fields, add a dummy field
    if (fields.empty()) {
        fields.push_back(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()));
    }
    
    return llvm::StructType::get(*codeGen_->getLLVMContext(), fields);
}

llvm::Type* TypeGenerator::mapUnionType(const UnionType& unionType) {
    // Union types are complex - for now, use a tagged union approach
    // This is a simplified implementation
    
    // Find the largest type in the union
    size_t maxSize = 0;
    llvm::Type* largestType = getAnyType();
    
    for (const auto& memberType : unionType.getTypes()) {
        llvm::Type* llvmType = mapTypeScriptTypeToLLVM(*memberType);
        // Note: This is a simplified size calculation
        if (llvmType->isPointerTy() || llvmType->isIntegerTy() || llvmType->isFloatingPointTy()) {
            size_t typeSize = 8; // Simplified - assume all basic types are 8 bytes
            if (typeSize > maxSize) {
                maxSize = typeSize;
                largestType = llvmType;
            }
        }
    }
    
    // Create a tagged union with type tag and value
    std::vector<llvm::Type*> fields = {
        llvm::Type::getInt32Ty(*codeGen_->getLLVMContext()), // type tag
        largestType // value (using largest type for simplicity)
    };
    
    return llvm::StructType::get(*codeGen_->getLLVMContext(), fields);
}

llvm::Type* TypeGenerator::mapGenericType(const GenericType& genericType) {
    // For generic types, we need to perform monomorphization
    // This is a simplified implementation
    
    auto baseType = genericType.getBaseType();
    if (baseType->getKind() == TypeKind::Class) {
        return createMonomorphizedStruct(genericType);
    } else if (baseType->getKind() == TypeKind::Array) {
        // Handle generic arrays
        if (!genericType.getTypeArguments().empty()) {
            auto elementType = genericType.getTypeArguments()[0];
            ArrayType arrayType(elementType);
            return mapArrayType(arrayType);
        }
    }
    
    // For other generic types, fall back to any type for now
    return getAnyType();
}

llvm::Type* TypeGenerator::mapObjectType(const ObjectType& objectType) {
    // Object types are similar to interfaces
    std::vector<llvm::Type*> fields;
    
    // Add fields from object properties
    for (const auto& property : objectType.getProperties()) {
        llvm::Type* fieldType = mapTypeScriptTypeToLLVM(*property.second);
        fields.push_back(fieldType);
    }
    
    // If no fields, add a dummy field
    if (fields.empty()) {
        fields.push_back(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()));
    }
    
    return llvm::StructType::get(*codeGen_->getLLVMContext(), fields);
}

llvm::Type* TypeGenerator::createMonomorphizedStruct(const GenericType& genericType) {
    // Create a monomorphized version of a generic struct
    auto baseType = std::static_pointer_cast<ClassType>(genericType.getBaseType());
    
    // Generate a unique name for this monomorphization
    String monomorphizedName = baseType->getName();
    for (const auto& typeArg : genericType.getTypeArguments()) {
        monomorphizedName += "_" + typeArg->toString();
    }
    
    // Check cache first
    auto it = classTypeCache_.find(monomorphizedName);
    if (it != classTypeCache_.end()) {
        return it->second;
    }
    
    // Create the monomorphized struct
    std::vector<llvm::Type*> fields;
    
    // Process base class properties with type substitution
    for (const auto& property : baseType->getProperties()) {
        llvm::Type* fieldType;
        
        // If the property type is a generic parameter, substitute it
        if (property.second->getKind() == TypeKind::Generic) {
            // Simplified type parameter substitution
            if (!genericType.getTypeArguments().empty()) {
                fieldType = mapTypeScriptTypeToLLVM(*genericType.getTypeArguments()[0]);
            } else {
                fieldType = getAnyType();
            }
        } else {
            fieldType = mapTypeScriptTypeToLLVM(*property.second);
        }
        
        fields.push_back(fieldType);
    }
    
    // If no fields, add a dummy field
    if (fields.empty()) {
        fields.push_back(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()));
    }
    
    // Create the struct type
    llvm::StructType* structType = llvm::StructType::create(*codeGen_->getLLVMContext(), 
                                                            fields, 
                                                            monomorphizedName);
    
    // Cache the type
    classTypeCache_[monomorphizedName] = structType;
    
    return structType;
}

llvm::Type* TypeGenerator::convertTypeToLLVM(std::shared_ptr<Type> type) {
    if (!type) {
        return getAnyType();
    }
    return mapTypeScriptTypeToLLVM(*type);
}

llvm::Type* TypeGenerator::convertFunctionTypeToLLVM(const FunctionType& functionType) {
    return mapFunctionType(functionType);
}

bool TypeGenerator::isPointerType(llvm::Type* type) const {
    return type->isPointerTy();
}

bool TypeGenerator::isNumericType(llvm::Type* type) const {
    return type->isFloatingPointTy() || type->isIntegerTy();
}

bool TypeGenerator::isStringType(llvm::Type* type) const {
    return type == getStringType();
}

bool TypeGenerator::isBooleanType(llvm::Type* type) const {
    return type->isIntegerTy(1);
}

bool TypeGenerator::isCompatible(llvm::Type* from, llvm::Type* to) const {
    // Basic type compatibility check
    if (from == to) {
        return true;
    }
    
    // Any type is compatible with everything
    if (from == getAnyType() || to == getAnyType()) {
        return true;
    }
    
    // Numeric types are compatible with each other
    if (isNumericType(from) && isNumericType(to)) {
        return true;
    }
    
    // Pointer types have some compatibility
    if (isPointerType(from) && isPointerType(to)) {
        return true;
    }
    
    return false;
}

llvm::Value* TypeGenerator::convertValue(llvm::Value* value, llvm::Type* fromType, llvm::Type* toType) {
    if (fromType == toType) {
        return value;
    }
    
    // Convert between numeric types
    if (isNumericType(fromType) && isNumericType(toType)) {
        if (fromType->isIntegerTy() && toType->isFloatingPointTy()) {
            return codeGen_->getBuilder()->CreateSIToFP(value, toType, "int_to_float");
        } else if (fromType->isFloatingPointTy() && toType->isIntegerTy()) {
            return codeGen_->getBuilder()->CreateFPToSI(value, toType, "float_to_int");
        }
    }
    
    // Convert to any type (cast to pointer)
    if (toType == getAnyType()) {
        if (isPointerType(fromType)) {
            return codeGen_->getBuilder()->CreateBitCast(value, toType, "to_any");
        } else {
            // For non-pointer types, we need to allocate and store
            llvm::Value* alloca = codeGen_->getBuilder()->CreateAlloca(fromType, nullptr, "temp_any");
            codeGen_->getBuilder()->CreateStore(value, alloca);
            return codeGen_->getBuilder()->CreateBitCast(alloca, toType, "to_any");
        }
    }
    
    // Convert from any type
    if (fromType == getAnyType()) {
        if (isPointerType(toType)) {
            return codeGen_->getBuilder()->CreateBitCast(value, toType, "from_any");
        } else {
            // Load from pointer
            llvm::Value* casted = codeGen_->getBuilder()->CreateBitCast(value, 
                                                                       llvm::PointerType::get(toType, 0), 
                                                                       "cast_any_ptr");
            return codeGen_->getBuilder()->CreateLoad(toType, casted, "from_any");
        }
    }
    
    // Default: bit cast for pointer types
    if (isPointerType(fromType) && isPointerType(toType)) {
        return codeGen_->getBuilder()->CreateBitCast(value, toType, "ptr_cast");
    }
    
    // If no conversion is possible, return the original value
    return value;
}

void TypeGenerator::clearCache() {
    classTypeCache_.clear();
}

} // namespace codegen
} // namespace tsc