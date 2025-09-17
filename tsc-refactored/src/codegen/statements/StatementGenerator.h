#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

// LLVM includes
#include "llvm/IR/Value.h"
#include "llvm/IR/BasicBlock.h"

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Statement generation utilities for converting AST statements to LLVM IR
class StatementGenerator {
public:
    explicit StatementGenerator(LLVMCodeGen* codeGen);
    
    // Expression statements
    void generateExpressionStatement(const ExpressionStatement& node);
    void generateBlockStatement(const BlockStatement& node);
    void generateReturnStatement(const ReturnStatement& node);
    
    // Control flow statements
    void generateIfStatement(const IfStatement& node);
    void generateWhileStatement(const WhileStatement& node);
    void generateDoWhileStatement(const DoWhileStatement& node);
    void generateForStatement(const ForStatement& node);
    void generateForOfStatement(const ForOfStatement& node);
    void generateSwitchStatement(const SwitchStatement& node);
    void generateCaseClause(const CaseClause& node);
    void generateBreakStatement(const BreakStatement& node);
    void generateContinueStatement(const ContinueStatement& node);
    
    // Exception handling
    void generateTryStatement(const TryStatement& node);
    void generateCatchClause(const CatchClause& node);
    void generateThrowStatement(const ThrowStatement& node);
    
    // Variable declarations
    void generateVariableDeclaration(const VariableDeclaration& node);
    
    // Control flow utilities
    llvm::BasicBlock* createBasicBlock(const String& name = "");
    void generateBranch(llvm::BasicBlock* block);
    void generateConditionalBranch(llvm::Value* condition, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock);
    
    // Enhanced control flow generation
    void generateIfStatement(const unique_ptr<IfStatement>& stmt);
    void generateWhileStatement(const unique_ptr<WhileStatement>& stmt);
    void generateForStatement(const unique_ptr<ForStatement>& stmt);
    void generateTryStatement(const unique_ptr<TryStatement>& stmt);

private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc