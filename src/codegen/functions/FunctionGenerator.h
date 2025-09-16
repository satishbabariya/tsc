#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

// LLVM includes
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Function generation utilities for converting function declarations to LLVM IR
class FunctionGenerator {
public:
    explicit FunctionGenerator(LLVMCodeGen* codeGen);
    
    // Function declaration generation
    llvm::Function* generateFunctionDeclaration(const FunctionDeclaration& funcDecl);
    void generateFunctionBody(llvm::Function* function, const FunctionDeclaration& funcDecl);
    void generateNestedFunction(const FunctionDeclaration& node);
    
    // Function analysis
    bool hasReturnStatements(const FunctionDeclaration& funcDecl);
    bool hasReturnStatementsWithValues(const FunctionDeclaration& funcDecl);
    
    // Unreachable block handling
    void ensureBlockTerminators(llvm::Function* function);
    void addUnreachableTerminator(llvm::BasicBlock* block);
    
    // Closure support
    llvm::StructType* createClosureStructType(const std::vector<Symbol*>& capturedVariables);
    llvm::Value* createClosureEnvironment(const std::vector<Symbol*>& capturedVariables);
    llvm::Value* loadCapturedVariable(const String& varName, llvm::Value* closureEnv);
    void storeCapturedVariable(const String& varName, llvm::Value* value, llvm::Value* closureEnv);
    bool isCapturedVariable(const String& varName, const FunctionDeclaration& currentFunction);
    String generateClosureTypeKey(const std::vector<Symbol*>& capturedVariables) const;
    
    // Scope navigation helpers
    Scope* findDeepestChildScope(Scope* parentScope);
    int getScopeDepth(Scope* scope);

private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc