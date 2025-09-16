#pragma once

#include "tsc/Common.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/EnhancedErrorReporting.h"

// LLVM includes
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"

#include <unordered_map>
#include <stack>
#include <vector>

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Code generation context for tracking state during generation
class CodeGenContext {
public:
    CodeGenContext(llvm::LLVMContext& llvmContext, llvm::Module& module, 
                   llvm::IRBuilder<>& builder, DiagnosticEngine& diagnostics);
    
    // Symbol value tracking
    void setSymbolValue(const String& name, llvm::Value* value);
    llvm::Value* getSymbolValue(const String& name) const;
    bool hasSymbol(const String& name) const;
    
    // Function context
    void enterFunction(llvm::Function* function);
    void exitFunction();
    llvm::Function* getCurrentFunction() const;
    
    // Class context
    void enterClass(const String& className);
    void exitClass();
    String getCurrentClassName() const;
    
    // Block context for break/continue
    void enterLoop(llvm::BasicBlock* continueBlock, llvm::BasicBlock* breakBlock);
    void exitLoop();
    llvm::BasicBlock* getCurrentLoopContinueBlock() const;
    llvm::BasicBlock* getCurrentLoopBreakBlock() const;
    
    // Switch context for break statements
    void enterSwitch(llvm::BasicBlock* exitBlock);
    void exitSwitch();
    llvm::BasicBlock* getCurrentSwitchExitBlock() const;
    
    // Scope management
    void enterScope();
    void exitScope();
    
    // ARC object tracking
    void addARCManagedObject(const String& name, llvm::Value* object, const String& className);
    void generateScopeCleanup(LLVMCodeGen* codeGen);
    
    // Error handling
    void reportError(const String& message, const SourceLocation& location);
    size_t getErrorCount() const { return errorCount_; }

private:
    llvm::LLVMContext& llvmContext_;
    llvm::Module& module_;
    llvm::IRBuilder<>& builder_;
    DiagnosticEngine& diagnostics_;
    unique_ptr<EnhancedErrorReporting> errorReporter_;
    
    // Symbol table for LLVM values
    std::vector<std::unordered_map<String, llvm::Value*>> symbolStack_;
    
    // Function context stack
    std::stack<llvm::Function*> functionStack_;
    
    // Performance optimization: struct type cache
    std::unordered_map<String, llvm::StructType*> structTypeCache_;
    
    // Class context stack
    std::stack<String> classStack_;
    
    // Loop context for break/continue
    struct LoopContext {
        llvm::BasicBlock* continueBlock;
        llvm::BasicBlock* breakBlock;
    };
    std::stack<LoopContext> loopStack_;
    
    // Switch context for break statements
    struct SwitchContext {
        llvm::BasicBlock* exitBlock;
    };
    std::stack<SwitchContext> switchStack_;
    
    // ARC object tracking
    struct ARCManagedObject {
        String name;
        llvm::Value* object;
        String className;
    };
    std::vector<std::vector<ARCManagedObject>> arcObjectStack_;
    
    size_t errorCount_ = 0;
};

} // namespace codegen
} // namespace tsc