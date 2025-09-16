#pragma once

#include "tsc/Common.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/AST.h"

// LLVM includes
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"

namespace tsc {

// Forward declarations
class CodeGenContext;

/**
 * Handles LLVM type system mapping and generation
 */
class LLVMTypeGen {
public:
    explicit LLVMTypeGen(CodeGenContext& context);
    
    // Type mapping
    llvm::Type* mapTypeScriptTypeToLLVM(const Type& type);
    llvm::Type* convertTypeToLLVM(shared_ptr<Type> type);
    llvm::FunctionType* convertFunctionTypeToLLVM(const FunctionType& functionType);
    
    // Primitive types
    llvm::Type* getNumberType() const;
    llvm::Type* getStringType() const;
    llvm::Type* getBooleanType() const;
    llvm::Type* getVoidType() const;
    llvm::Type* getAnyType() const;
    
    // Generic types
    llvm::Type* createMonomorphizedType(const GenericType& genericType);
    llvm::StructType* createMonomorphizedStruct(const GenericType& genericType);
    
    // Array types
    llvm::Type* createArrayType(llvm::Type* elementType);
    llvm::Type* createFixedArrayType(llvm::Type* elementType, size_t size);
    
    // Object types
    llvm::StructType* createObjectType(const String& name, const std::vector<std::pair<String, llvm::Type*>>& fields);
    llvm::StructType* createClassType(const String& name, const std::vector<PropertyDeclaration>& members);
    
    // Function types
    llvm::FunctionType* createFunctionType(llvm::Type* returnType, const std::vector<llvm::Type*>& parameterTypes, bool isVarArg = false);
    
    // Type conversion utilities
    llvm::Value* convertValueToType(llvm::Value* value, llvm::Type* targetType);
    bool isTypeCompatible(llvm::Type* from, llvm::Type* to) const;
    
    // Type caching
    void cacheStructType(const String& name, llvm::StructType* type);
    llvm::StructType* getCachedStructType(const String& name) const;

private:
    CodeGenContext& context_;
    
    // Type cache
    std::unordered_map<String, llvm::StructType*> structTypeCache_;
    
    // Helper methods
    llvm::Type* mapPrimitiveType(const PrimitiveType& type);
    llvm::Type* mapObjectType(const ObjectType& type);
    llvm::Type* mapArrayType(const ArrayType& type);
    llvm::Type* mapFunctionType(const FunctionType& type);
    llvm::Type* mapGenericType(const GenericType& type);
    llvm::Type* mapUnionType(const UnionType& type);
    llvm::Type* mapIntersectionType(const IntersectionType& type);
    
    // Type creation helpers
    llvm::StructType* createStructType(const String& name, const std::vector<llvm::Type*>& fieldTypes);
    llvm::PointerType* createPointerType(llvm::Type* pointeeType);
    
    // Type validation
    bool validateType(llvm::Type* type) const;
    void reportTypeError(const String& message, const SourceLocation& location);
};

} // namespace tsc