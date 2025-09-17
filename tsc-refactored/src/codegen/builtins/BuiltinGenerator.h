#pragma once

#include "tsc/Common.h"

// LLVM includes
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/GlobalVariable.h"

#include <unordered_map>

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Builtin function generation and management
class BuiltinGenerator {
public:
    explicit BuiltinGenerator(LLVMCodeGen* codeGen);
    
    // Built-in function declarations
    void declareBuiltinFunctions();
    void declareBuiltinGlobals();
    
    // Memory management functions
    llvm::Function* getOrCreateMallocFunction();
    llvm::Function* getOrCreateFreeFunction();
    
    // ARC Runtime functions
    llvm::Function* getOrCreateARCRetainFunction();
    llvm::Function* getOrCreateARCReleaseFunction();
    llvm::Function* getOrCreatePointerToStringFunction();
    llvm::Function* getOrCreateARCAllocFunction();
    
    // Console functions
    llvm::Function* getOrCreatePrintFunction();
    
    // String functions
    llvm::Function* getOrCreateStringConcatFunction();
    llvm::Function* getOrCreateNumberToStringFunction();
    llvm::Function* getOrCreateBooleanToStringFunction();
    llvm::Function* getOrCreateObjectToStringFunction();
    
    // Exception handling functions
    llvm::Function* getOrCreateThrowFunction();
    llvm::Function* getOrCreateRethrowFunction();
    llvm::Function* getOrCreateSetupExceptionHandlerFunction();
    llvm::Function* getOrCreateCleanupExceptionHandlerFunction();
    llvm::Function* getOrCreateTryExceptionHandlerFunction();
    llvm::Function* getOrCreateGetExceptionTypeFunction();
    llvm::Function* getOrCreateGetExceptionMessageFunction();
    llvm::Function* getOrCreateClearExceptionFunction();
    llvm::Function* getOrCreateGetExceptionFunction();
    
    // Exception types
    llvm::StructType* getOrCreateExceptionHandlerType();
    llvm::StructType* getOrCreateExceptionType();
    
    // Deferred symbol handling
    llvm::Value* createDeferredExternalSymbolMarker(llvm::GlobalVariable* externalVar, const String& name);
    
    // Builtin function registry
    llvm::Function* getBuiltinFunction(const String& name);
    bool isBuiltinFunction(const String& name) const;

private:
    LLVMCodeGen* codeGen_;
    
    // Built-in function declarations
    std::unordered_map<String, llvm::Function*> builtinFunctions_;
    
    // Deferred external symbol references (for Infinity, NaN, etc.)
    std::unordered_map<String, llvm::GlobalVariable*> deferredExternalSymbols_;
    
    // Builtin function registration
    void registerBuiltinFunctions();
    void registerConsoleFunctions();
    void registerMathFunctions();
    void registerStringFunctions();
    void registerArrayFunctions();
};

} // namespace codegen
} // namespace tsc