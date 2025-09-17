#include "tsc/codegen/context/CodeGenContext.h"
#include "tsc/codegen/LLVMCodeGen.h"

#include <iostream>

namespace tsc {
namespace codegen {

CodeGenContext::CodeGenContext(llvm::LLVMContext& llvmContext, llvm::Module& module,
                               llvm::IRBuilder<>& builder, DiagnosticEngine& diagnostics)
    : llvmContext_(llvmContext), module_(module), builder_(builder), diagnostics_(diagnostics) {
    // Initialize enhanced error reporting
    errorReporter_ = make_unique<EnhancedErrorReporting>(diagnostics_);

    // Initialize with global scope
    symbolStack_.push_back(std::unordered_map<String, llvm::Value*>());

    // Initialize ARC object stack with global scope
    arcObjectStack_.push_back(std::vector<ARCManagedObject>());
}

void CodeGenContext::setSymbolValue(const String& name, llvm::Value* value) {
    if (!symbolStack_.empty()) {
        symbolStack_.back()[name] = value;
    }
}

llvm::Value* CodeGenContext::getSymbolValue(const String& name) const {
    // Search from innermost to outermost scope
    for (auto it = symbolStack_.rbegin(); it != symbolStack_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }
    return nullptr;
}

bool CodeGenContext::hasSymbol(const String& name) const {
    return getSymbolValue(name) != nullptr;
}

void CodeGenContext::enterFunction(llvm::Function* function) {
    functionStack_.push(function);
}

void CodeGenContext::exitFunction() {
    if (!functionStack_.empty()) {
        functionStack_.pop();
    }
}

llvm::Function* CodeGenContext::getCurrentFunction() const {
    return functionStack_.empty() ? nullptr : functionStack_.top();
}

void CodeGenContext::enterClass(const String& className) {
    classStack_.push(className);
}

void CodeGenContext::exitClass() {
    if (!classStack_.empty()) {
        classStack_.pop();
    }
}

String CodeGenContext::getCurrentClassName() const {
    return classStack_.empty() ? "" : classStack_.top();
}

void CodeGenContext::enterScope() {
    symbolStack_.push_back(std::unordered_map<String, llvm::Value*>());
}

void CodeGenContext::exitScope() {
    if (symbolStack_.size() > 1) {
        symbolStack_.pop_back();
    }

    // Generate cleanup for ARC-managed objects in this scope
    // Note: We'll call generateScopeCleanup from LLVMCodeGen where we have access to the codeGen instance
}

void CodeGenContext::addARCManagedObject(const String& name, llvm::Value* object, const String& className) {
    // Ensure we have enough scope levels
    while (arcObjectStack_.size() < symbolStack_.size()) {
        arcObjectStack_.push_back(std::vector<ARCManagedObject>());
    }

    // Add the object to the current scope
    if (!arcObjectStack_.empty()) {
        arcObjectStack_.back().push_back({name, object, className});
        std::cout << "DEBUG: Added ARC-managed object '" << name << "' (class: " << className << ")" << std::endl;
    }
}

void CodeGenContext::generateScopeCleanup(LLVMCodeGen* codeGen) {
    if (arcObjectStack_.empty()) return;

    std::vector<ARCManagedObject>& currentScope = arcObjectStack_.back();

    // Generate destructor calls and ARC release calls in reverse order
    for (auto it = currentScope.rbegin(); it != currentScope.rend(); ++it) {
        const ARCManagedObject& obj = *it;
        std::cout << "DEBUG: Generating cleanup for ARC object '" << obj.name << "' (class: " << obj.className << ")" << std::endl;

        // Call __tsc_release to decrement reference count (this will automatically call the destructor when ref count reaches 0)
        llvm::Function* releaseFunc = codeGen->getOrCreateARCReleaseFunction();
        if (releaseFunc) {
            std::cout << "DEBUG: Calling __tsc_release for object " << obj.name << std::endl;
            builder_.CreateCall(releaseFunc, {obj.object});
        } else {
            std::cout << "DEBUG: Failed to create __tsc_release function" << std::endl;
        }
    }

    // Remove the current scope from the stack
    arcObjectStack_.pop_back();
}

void CodeGenContext::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
    errorCount_++;
}

// Switch context management
void CodeGenContext::enterSwitch(llvm::BasicBlock* exitBlock) {
    switchStack_.push({exitBlock});
}

void CodeGenContext::exitSwitch() {
    if (!switchStack_.empty()) {
        switchStack_.pop();
    }
}

llvm::BasicBlock* CodeGenContext::getCurrentSwitchExitBlock() const {
    if (!switchStack_.empty()) {
        return switchStack_.top().exitBlock;
    }
    return nullptr;
}

// Loop context management
void CodeGenContext::enterLoop(llvm::BasicBlock* continueBlock, llvm::BasicBlock* breakBlock) {
    loopStack_.push({continueBlock, breakBlock});
}

void CodeGenContext::exitLoop() {
    if (!loopStack_.empty()) {
        loopStack_.pop();
    }
}

llvm::BasicBlock* CodeGenContext::getCurrentLoopContinueBlock() const {
    if (!loopStack_.empty()) {
        return loopStack_.top().continueBlock;
    }
    return nullptr;
}

llvm::BasicBlock* CodeGenContext::getCurrentLoopBreakBlock() const {
    if (!loopStack_.empty()) {
        return loopStack_.top().breakBlock;
    }
    return nullptr;
}

} // namespace codegen
} // namespace tsc