#pragma once

#include "tsc/Common.h"
#include "tsc/codegen/LLVMCodeGen.h"

// LLVM includes
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"

namespace tsc {

// Forward declarations
class CodeGenContext;

/**
 * Handles LLVM memory management and allocation
 */
class LLVMMemoryGen {
public:
    explicit LLVMMemoryGen(CodeGenContext& context);
    
    // Memory allocation
    llvm::Value* allocateMemory(llvm::Type* type, const String& name = "");
    llvm::Value* allocateArray(llvm::Type* elementType, llvm::Value* size, const String& name = "");
    llvm::Value* allocateObject(llvm::StructType* objectType, const String& name = "");
    
    // Memory deallocation
    void deallocateMemory(llvm::Value* pointer);
    void deallocateArray(llvm::Value* arrayPointer);
    void deallocateObject(llvm::Value* objectPointer);
    
    // Memory operations
    llvm::Value* loadValue(llvm::Value* pointer, llvm::Type* type);
    void storeValue(llvm::Value* pointer, llvm::Value* value);
    llvm::Value* getElementPointer(llvm::Value* pointer, llvm::Value* index);
    llvm::Value* getStructElementPointer(llvm::Value* structPointer, unsigned fieldIndex);
    
    // Stack allocation
    llvm::Value* allocateStack(llvm::Type* type, const String& name = "");
    void deallocateStack(llvm::Value* stackPointer);
    
    // Heap allocation
    llvm::Value* allocateHeap(llvm::Type* type, const String& name = "");
    void deallocateHeap(llvm::Value* heapPointer);
    
    // Memory utilities
    llvm::Value* getMemorySize(llvm::Type* type);
    llvm::Value* copyMemory(llvm::Value* dest, llvm::Value* src, llvm::Value* size);
    llvm::Value* compareMemory(llvm::Value* ptr1, llvm::Value* ptr2, llvm::Value* size);
    
    // Memory initialization
    void initializeMemory(llvm::Value* pointer, llvm::Type* type, llvm::Value* initialValue = nullptr);
    void initializeArray(llvm::Value* arrayPointer, llvm::Type* elementType, const std::vector<llvm::Value*>& initialValues);
    void initializeObject(llvm::Value* objectPointer, llvm::StructType* objectType, const std::vector<llvm::Value*>& fieldValues);

private:
    CodeGenContext& context_;
    
    // Memory management functions
    llvm::Function* getMallocFunction();
    llvm::Function* getFreeFunction();
    llvm::Function* getMemcpyFunction();
    llvm::Function* getMemcmpFunction();
    llvm::Function* getMemsetFunction();
    
    // Helper methods
    llvm::Value* createMallocCall(llvm::Type* type, llvm::Value* size = nullptr);
    void createFreeCall(llvm::Value* pointer);
    llvm::Value* createMemcpyCall(llvm::Value* dest, llvm::Value* src, llvm::Value* size);
    llvm::Value* createMemcmpCall(llvm::Value* ptr1, llvm::Value* ptr2, llvm::Value* size);
    void createMemsetCall(llvm::Value* pointer, llvm::Value* value, llvm::Value* size);
    
    // Memory layout helpers
    llvm::Value* calculateStructSize(llvm::StructType* structType);
    llvm::Value* calculateArraySize(llvm::Type* elementType, llvm::Value* count);
    llvm::Value* alignMemory(llvm::Value* pointer, llvm::Value* alignment);
    
    // Error handling
    void reportMemoryError(const String& message, const SourceLocation& location);
    void validateMemoryOperation(llvm::Value* pointer, llvm::Type* type);
};

} // namespace tsc