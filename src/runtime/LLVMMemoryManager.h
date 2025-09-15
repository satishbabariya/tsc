#pragma once

#include "TypeScriptObject.h"
#include "GarbageCollector.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <memory>

namespace tsc::runtime {

/**
 * LLVM Memory Manager for TypeScript objects
 * Integrates ARC with LLVM IR generation
 */
class LLVMMemoryManager {
public:
    LLVMMemoryManager(llvm::LLVMContext& context, llvm::Module& module);
    
    // Generate LLVM IR for object creation
    llvm::Value* createObject(llvm::IRBuilder<>& builder, const std::string& typeName);
    
    // Generate LLVM IR for reference counting
    llvm::Value* addRef(llvm::IRBuilder<>& builder, llvm::Value* object);
    llvm::Value* releaseRef(llvm::IRBuilder<>& builder, llvm::Value* object);
    
    // Generate LLVM IR for garbage collection
    void generateGC(llvm::IRBuilder<>& builder);
    
    // Get LLVM types for TypeScript objects
    llvm::StructType* getObjectType() const { return objectType_; }
    llvm::StructType* getArrayType() const { return arrayType_; }
    llvm::StructType* getObjectLiteralType() const { return objectLiteralType_; }
    
    // Runtime function declarations
    void declareRuntimeFunctions();
    
    // Generate cleanup code
    void generateCleanup(llvm::IRBuilder<>& builder);

private:
    llvm::LLVMContext& context_;
    llvm::Module& module_;
    llvm::IRBuilder<>* builder_;
    
    // LLVM types for TypeScript objects
    llvm::StructType* objectType_;
    llvm::StructType* arrayType_;
    llvm::StructType* objectLiteralType_;
    
    // Runtime function declarations
    llvm::Function* mallocFunc_;
    llvm::Function* freeFunc_;
    llvm::Function* addRefFunc_;
    llvm::Function* releaseRefFunc_;
    llvm::Function* gcCollectFunc_;
    
    // Initialize LLVM types
    void initializeTypes();
    
    // Create object structure type
    llvm::StructType* createObjectStruct(const std::string& name, 
                                        const std::vector<llvm::Type*>& fields);
    
    // Generate reference counting functions
    void generateReferenceCountingFunctions();
    
    // Generate garbage collection functions
    void generateGCFunctions();
};

/**
 * Helper class for generating TypeScript object operations in LLVM IR
 */
class TypeScriptObjectBuilder {
public:
    TypeScriptObjectBuilder(LLVMMemoryManager& memoryManager, llvm::IRBuilder<>& builder);
    
    // Object creation
    llvm::Value* createArray();
    llvm::Value* createObjectLiteral();
    
    // Array operations
    llvm::Value* arrayPush(llvm::Value* array, llvm::Value* value);
    llvm::Value* arrayPop(llvm::Value* array);
    llvm::Value* arrayLength(llvm::Value* array);
    llvm::Value* arrayGet(llvm::Value* array, llvm::Value* index);
    void arraySet(llvm::Value* array, llvm::Value* index, llvm::Value* value);
    
    // Object operations
    void objectSetProperty(llvm::Value* obj, const std::string& key, llvm::Value* value);
    llvm::Value* objectGetProperty(llvm::Value* obj, const std::string& key);
    llvm::Value* objectHasProperty(llvm::Value* obj, const std::string& key);
    void objectDeleteProperty(llvm::Value* obj, const std::string& key);
    
    // Reference counting
    llvm::Value* addRef(llvm::Value* obj);
    llvm::Value* releaseRef(llvm::Value* obj);
    
    // Memory management
    llvm::Value* allocateObject(llvm::Type* type);
    void deallocateObject(llvm::Value* obj);

private:
    LLVMMemoryManager& memoryManager_;
    llvm::IRBuilder<>& builder_;
    
    // Helper methods
    llvm::Value* getObjectField(llvm::Value* obj, size_t fieldIndex);
    void setObjectField(llvm::Value* obj, size_t fieldIndex, llvm::Value* value);
    llvm::Value* createStringConstant(const std::string& str);
};

/**
 * RAII wrapper for LLVM function generation with automatic cleanup
 */
class LLVMFunctionBuilder {
public:
    LLVMFunctionBuilder(llvm::Module& module, llvm::LLVMContext& context,
                       const std::string& name, llvm::FunctionType* type);
    
    ~LLVMFunctionBuilder();
    
    llvm::Function* getFunction() const { return function_; }
    llvm::IRBuilder<>& getBuilder() { return builder_; }
    
    // Generate function prologue with memory management setup
    void generatePrologue();
    
    // Generate function epilogue with cleanup
    void generateEpilogue();
    
    // Add automatic cleanup for local objects
    void addLocalObject(llvm::Value* obj);
    
    // Generate cleanup for all local objects
    void cleanupLocalObjects();

private:
    llvm::Module& module_;
    llvm::LLVMContext& context_;
    llvm::Function* function_;
    llvm::IRBuilder<> builder_;
    std::vector<llvm::Value*> localObjects_;
};

} // namespace tsc::runtime