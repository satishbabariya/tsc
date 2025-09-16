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
 * Handles Automatic Reference Counting (ARC) for LLVM code generation
 */
class LLVMARCGen {
public:
    explicit LLVMARCGen(CodeGenContext& context);
    
    // ARC object management
    void addARCManagedObject(const String& name, llvm::Value* object, const String& className);
    void generateScopeCleanup();
    void generateObjectCleanup(const String& objectName, const String& className);
    
    // Reference counting operations
    llvm::Value* retainObject(llvm::Value* object, const String& className);
    void releaseObject(llvm::Value* object, const String& className);
    llvm::Value* autoreleaseObject(llvm::Value* object, const String& className);
    
    // ARC function generation
    llvm::Function* createRetainFunction(const String& className);
    llvm::Function* createReleaseFunction(const String& className);
    llvm::Function* createAutoreleaseFunction(const String& className);
    
    // ARC runtime functions
    llvm::Function* getARCAllocFunction();
    llvm::Function* getARCDeallocFunction();
    llvm::Function* getARCRetainFunction();
    llvm::Function* getARCReleaseFunction();
    llvm::Function* getARCAutoreleaseFunction();
    
    // ARC object tracking
    struct ARCManagedObject {
        String name;
        llvm::Value* object;
        String className;
    };
    
    void pushARCScope();
    void popARCScope();
    void addObjectToCurrentScope(const ARCManagedObject& object);
    std::vector<ARCManagedObject> getCurrentScopeObjects() const;
    
    // ARC optimization
    void optimizeRetainReleasePairs();
    void eliminateRedundantRetains();
    void eliminateRedundantReleases();
    
    // ARC debugging
    void generateARCDebugInfo(const String& objectName, const String& operation);
    void validateARCInvariants();

private:
    CodeGenContext& context_;
    
    // ARC object stack
    std::vector<std::vector<ARCManagedObject>> arcObjectStack_;
    
    // ARC function cache
    std::unordered_map<String, llvm::Function*> retainFunctions_;
    std::unordered_map<String, llvm::Function*> releaseFunctions_;
    std::unordered_map<String, llvm::Function*> autoreleaseFunctions_;
    
    // Helper methods
    llvm::Function* createARCFunction(const String& name, llvm::FunctionType* functionType);
    llvm::Value* createARCCall(llvm::Function* function, const std::vector<llvm::Value*>& args);
    
    // ARC runtime setup
    void setupARCRuntime();
    void declareARCFunctions();
    
    // ARC object lifecycle
    void generateObjectInitialization(llvm::Value* object, const String& className);
    void generateObjectDestruction(llvm::Value* object, const String& className);
    
    // ARC error handling
    void reportARCError(const String& message, const SourceLocation& location);
    void validateARCObject(llvm::Value* object, const String& className);
    
    // ARC optimization helpers
    bool canOptimizeRetainRelease(llvm::Value* object);
    void eliminateRetainReleasePair(llvm::Value* object);
};

} // namespace tsc