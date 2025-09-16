#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

// LLVM includes
#include "llvm/IR/Value.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"

namespace tsc {

// Forward declarations
class CodeGenContext;

/**
 * Handles LLVM code generation for statements
 */
class LLVMStatementGen {
public:
    explicit LLVMStatementGen(CodeGenContext& context);
    
    // Declaration statements
    void visit(VariableDeclaration& node);
    void visit(FunctionDeclaration& node);
    void visit(ClassDeclaration& node);
    void visit(InterfaceDeclaration& node);
    void visit(EnumDeclaration& node);
    void visit(TypeAliasDeclaration& node);
    void visit(ImportDeclaration& node);
    void visit(ExportDeclaration& node);
    
    // Control flow statements
    void visit(IfStatement& node);
    void visit(WhileStatement& node);
    void visit(ForStatement& node);
    void visit(ForOfStatement& node);
    void visit(SwitchStatement& node);
    void visit(DoWhileStatement& node);
    
    // Jump statements
    void visit(BreakStatement& node);
    void visit(ContinueStatement& node);
    void visit(ReturnStatement& node);
    void visit(ThrowStatement& node);
    
    // Expression statements
    void visit(ExpressionStatement& node);
    
    // Block statements
    void visit(BlockStatement& node);
    void visit(TryStatement& node);
    void visit(CatchClause& node);
    
    // Labeled statements

private:
    CodeGenContext& context_;
    
    // Control flow helpers
    llvm::BasicBlock* createBasicBlock(const String& name);
    void generateConditionalBranch(llvm::Value* condition, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock);
    void generateUnconditionalBranch(llvm::BasicBlock* targetBlock);
    
    // Loop helpers
    void generateLoopHeader(llvm::BasicBlock* headerBlock, llvm::BasicBlock* bodyBlock, llvm::BasicBlock* continueBlock);
    void generateLoopFooter(llvm::BasicBlock* continueBlock, llvm::BasicBlock* exitBlock);
    
    // Switch helpers
    void generateSwitch(llvm::Value* condition, const std::vector<std::pair<llvm::Value*, llvm::BasicBlock*>>& cases, llvm::BasicBlock* defaultBlock);
    
    // Exception handling helpers
    void generateTryBlock(llvm::BasicBlock* tryBlock, llvm::BasicBlock* catchBlock, llvm::BasicBlock* finallyBlock);
    void generateCatchBlock(llvm::BasicBlock* catchBlock, const String& exceptionVar);
    void generateFinallyBlock(llvm::BasicBlock* finallyBlock);
    
    // Variable declaration helpers
    void generateVariableDeclaration(const String& name, llvm::Type* type, llvm::Value* initialValue);
    void generateFunctionDeclaration(const String& name, llvm::FunctionType* functionType);
    void generateClassDeclaration(const String& name, const std::vector<PropertyDeclaration>& members);
};

} // namespace tsc