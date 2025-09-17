#pragma once

#include "tsc/Common.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {
namespace codegen {

class LLVMCodeGen; // Forward declaration

/**
 * TypeGenerator handles the mapping between TypeScript types and LLVM types.
 * It provides a centralized location for all type-related operations in code generation.
 */
class TypeGenerator {
public:
    explicit TypeGenerator(LLVMCodeGen* codeGen);
    
    // Basic type getters
    llvm::Type* getStringType() const;
    llvm::Type* getNumberType() const;
    llvm::Type* getBooleanType() const;
    llvm::Type* getVoidType() const;
    llvm::Type* getAnyType() const;
    llvm::Type* getNullType() const;
    llvm::Type* getUndefinedType() const;
    
    // Main type mapping function
    llvm::Type* mapTypeScriptTypeToLLVM(const Type& type);
    
    // Specific type mapping functions
    llvm::Type* mapArrayType(const ArrayType& arrayType);
    llvm::Type* mapFunctionType(const FunctionType& functionType);
    llvm::Type* mapClassType(const ClassType& classType);
    llvm::Type* mapInterfaceType(const InterfaceType& interfaceType);
    llvm::Type* mapUnionType(const UnionType& unionType);
    llvm::Type* mapGenericType(const GenericType& genericType);
    llvm::Type* mapObjectType(const ObjectType& objectType);
    
    // Generic type specialization
    llvm::Type* createMonomorphizedStruct(const GenericType& genericType);
    
    // Convenience functions
    llvm::Type* convertTypeToLLVM(std::shared_ptr<Type> type);
    llvm::Type* convertFunctionTypeToLLVM(const FunctionType& functionType);
    
    // Type checking utilities
    bool isPointerType(llvm::Type* type) const;
    bool isNumericType(llvm::Type* type) const;
    bool isStringType(llvm::Type* type) const;
    bool isBooleanType(llvm::Type* type) const;
    bool isCompatible(llvm::Type* from, llvm::Type* to) const;
    
    // Type conversion utilities
    llvm::Value* convertValue(llvm::Value* value, llvm::Type* fromType, llvm::Type* toType);
    
    // Cache management
    void clearCache();
    
private:
    LLVMCodeGen* codeGen_;
    
    // Type cache to avoid recreating the same types
    std::unordered_map<String, llvm::StructType*> classTypeCache_;
};

} // namespace codegen
} // namespace tsc