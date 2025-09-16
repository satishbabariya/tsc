#pragma once

#include "tsc/Common.h"

// LLVM includes
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/CallInst.h"
#include "llvm/IR/StructType.h"

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Optimization engine for LLVM IR optimization
class OptimizationEngine {
public:
    explicit OptimizationEngine(LLVMCodeGen* codeGen);
    
    // Optimization passes
    void runOptimizationPasses();
    void runFunctionOptimizations();
    void runModuleOptimizations();
    void runMemoryOptimizations();
    void runARCOptimizations();
    
    // ARC optimization methods
    void eliminateRedundantRetainRelease();
    void optimizeWeakReferenceAccess();
    void removeRedundantARCCalls();
    void optimizeMemoryLayout();
    bool canEliminateRetainReleasePair(llvm::CallInst* retainCall, llvm::CallInst* releaseCall);
    void optimizeWeakAccessPattern(llvm::CallInst* weakCall);
    bool isRedundantARCCall(llvm::CallInst* callInst, const String& functionName);
    bool isARCManagedStruct(llvm::StructType* structType);
    void optimizeStructLayout(llvm::StructType* structType);
    
    // Module optimization
    void optimizeModule();

private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc