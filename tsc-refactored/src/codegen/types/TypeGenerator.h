#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/TypeSystem.h"

// LLVM includes
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"

#include <unordered_map>

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Type generation utilities for converting TypeScript types to LLVM types
class TypeGenerator {
public:
    explicit TypeGenerator(LLVMCodeGen* codeGen);
    
    // Main type conversion interface
    llvm::Type* generateType(const shared_ptr<Type>& type);
    llvm::Type* generatePrimitiveType(const shared_ptr<Type>& type);
    llvm::Type* generateArrayType(const shared_ptr<Type>& type);
    llvm::Type* generateObjectType(const shared_ptr<Type>& type);
    llvm::Type* generateFunctionType(const shared_ptr<Type>& type);
    llvm::Type* generateUnionType(const shared_ptr<Type>& type);
    llvm::Type* generateGenericType(const shared_ptr<Type>& type);
    
    // Type mapping from TypeScript to LLVM
    llvm::Type* mapTypeScriptTypeToLLVM(const Type& type);
    llvm::Type* getNumberType() const;
    llvm::Type* getStringType() const;
    llvm::Type* getBooleanType() const;
    llvm::Type* getVoidType() const;
    llvm::Type* getAnyType() const;
    llvm::Type* convertFunctionTypeToLLVM(const FunctionType& functionType);
    llvm::Type* convertTypeToLLVM(shared_ptr<Type> type);
    
    // Generic type specialization
    llvm::Type* generateGenericType(const shared_ptr<GenericType>& genericType, 
                                   const std::vector<llvm::Type*>& typeArguments);
    llvm::Type* generateSpecializedClass(const shared_ptr<GenericType>& genericType,
                                        const std::vector<llvm::Type*>& typeArguments);
    llvm::Type* specializeType(const shared_ptr<Type>& type,
                              const std::unordered_map<String, llvm::Type*>& typeMap);
    
    // Generic type monomorphization
    llvm::Type* createMonomorphizedType(const GenericType& genericType);
    String generateMangledName(const GenericType& genericType);
    llvm::StructType* createMonomorphizedStruct(const GenericType& genericType);
    String generateMangledMethodName(const GenericType& genericType, const String& methodName);
    
    // Type conversion utilities
    llvm::Value* convertValueToType(llvm::Value* value, llvm::Type* targetType);
    
    // Performance optimization: struct type caching
    llvm::StructType* getOrCreateStructType(const std::vector<llvm::Type*>& fieldTypes);

private:
    LLVMCodeGen* codeGen_;
    llvm::LLVMContext* context_;
    
    // Type caching system
    std::unordered_map<String, llvm::Type*> typeCache_;
    std::unordered_map<String, llvm::FunctionType*> functionTypeCache_;
    std::unordered_map<String, llvm::StructType*> structTypeCache_;
    
    // Generic type handling
    std::unordered_map<String, llvm::Type*> specializedTypes_;
    std::unordered_map<String, std::vector<llvm::Type*>> genericTypeMap_;
};

} // namespace codegen
} // namespace tsc