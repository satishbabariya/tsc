#pragma once

#include "tsc/Common.h"
#include <unordered_set>

namespace tsc {
namespace codegen {

class LLVMCodeGen; // Forward declaration

/**
 * BuiltinGenerator handles the declaration and management of built-in functions and globals.
 * It provides access to runtime functions like print, malloc, string operations, and ARC functions.
 */
class BuiltinGenerator {
public:
    explicit BuiltinGenerator(LLVMCodeGen* codeGen);
    
    // Declaration functions
    void declareBuiltinFunctions();
    void declareBuiltinGlobals();
    
    // Basic runtime functions
    llvm::Function* getOrCreatePrintFunction();
    llvm::Function* getOrCreateMallocFunction();
    llvm::Function* getOrCreateFreeFunction();
    
    // String conversion functions
    llvm::Function* getOrCreateNumberToStringFunction();
    llvm::Function* getOrCreateBooleanToStringFunction();
    llvm::Function* getOrCreateObjectToStringFunction();
    llvm::Function* getOrCreateStringConcatFunction();
    llvm::Function* getOrCreatePointerToStringFunction();
    
    // ARC (Automatic Reference Counting) functions
    llvm::Function* getOrCreateARCAllocFunction();
    llvm::Function* getOrCreateARCRetainFunction();
    llvm::Function* getOrCreateARCReleaseFunction();
    
    // Exception handling functions
    llvm::Function* getOrCreateSetupExceptionHandlerFunction();
    llvm::Function* getOrCreateTryExceptionHandlerFunction();
    llvm::Function* getOrCreateCleanupExceptionHandlerFunction();
    
    // Global objects
    void declareConsoleObject();
    
    // Utility functions
    llvm::Function* getBuiltinFunction(const String& name);
    bool isBuiltinFunction(const String& name) const;
    llvm::Value* generateBuiltinCall(const String& functionName, const std::vector<llvm::Value*>& args);
    
private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc