#include "tsc/codegen/statements/StatementGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"

namespace tsc {
namespace codegen {

StatementGenerator::StatementGenerator(LLVMCodeGen* codeGen) 
    : codeGen_(codeGen) {
}

void StatementGenerator::generateExpressionStatement(const ExpressionStatement& node) {
    std::cout << "DEBUG: ExpressionStatement visitor called" << std::endl;
    codeGen_->generateExpression(*node.getExpression());
    // Expression statement doesn't return a value
}

void StatementGenerator::generateBlockStatement(const BlockStatement& node) {
    codeGen_->getCodeGenContext()->enterScope();

    // Note: We don't create new scopes in LLVMCodeGen - we reuse existing ones from semantic analysis
    std::cout << "DEBUG: LLVMCodeGen processing block with current scope: " << codeGen_->getSymbolTable()->getCurrentScope() <<
            std::endl;

    for (const auto &stmt: node.getStatements()) {
        // Skip processing if the current block already has a terminator
        // This prevents double processing of statements that appear in both
        // control flow statements (like if/while) and their parent block
        llvm::BasicBlock *currentBlock = codeGen_->getBuilder()->GetInsertBlock();
        if (currentBlock && currentBlock->getTerminator()) {
            continue;
        }

        codeGen_->generateStatement(*stmt);
        if (codeGen_->hasErrors()) break;
    }

    // Note: We don't exit scopes in LLVMCodeGen - we leave the SymbolTable as-is
    std::cout << "DEBUG: LLVMCodeGen finished processing block with current scope: " << codeGen_->getSymbolTable()->
            getCurrentScope() << std::endl;

    codeGen_->getCodeGenContext()->exitScope();
}

void StatementGenerator::generateReturnStatement(const ReturnStatement& node) {
    llvm::Function *currentFunc = codeGen_->getCodeGenContext()->getCurrentFunction();
    if (!currentFunc) {
        codeGen_->reportError("Return statement outside function", node.getLocation());
        return;
    }

    // Generate cleanup for ARC-managed objects before return
    codeGen_->getCodeGenContext()->generateScopeCleanup(codeGen_);

    if (node.hasValue()) {
        // Generate code for return value
        llvm::Value *returnValue = codeGen_->generateExpression(*node.getValue());

        if (returnValue) {
            // Convert to appropriate return type if needed
            llvm::Type *returnType = currentFunc->getReturnType();

            if (returnValue->getType() != returnType) {
                // Perform type conversion
                returnValue = codeGen_->convertValueToType(returnValue, returnType);
            }
            codeGen_->getBuilder()->CreateRet(returnValue);
        } else {
            codeGen_->reportError("Failed to generate return value", node.getLocation());
            codeGen_->getBuilder()->CreateRetVoid();
        }
    } else {
        // Return void
        codeGen_->getBuilder()->CreateRetVoid();
    }
}

void StatementGenerator::generateIfStatement(const IfStatement& node) {
    llvm::Function *currentFunc = codeGen_->getCodeGenContext()->getCurrentFunction();
    if (!currentFunc) {
        codeGen_->reportError("If statement outside function", node.getLocation());
        return;
    }

    // Generate condition
    llvm::Value *conditionValue = codeGen_->generateExpression(*node.getCondition());

    if (!conditionValue) {
        codeGen_->reportError("Failed to generate condition", node.getLocation());
        return;
    }

    // Convert condition to boolean (simplified for now)
    if (conditionValue->getType()->isDoubleTy()) {
        // Compare double to 0.0
        llvm::Value *zero = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*codeGen_->getLLVMContext()), 0.0);
        conditionValue = codeGen_->getBuilder()->CreateFCmpONE(conditionValue, zero, "tobool");
    } else if (conditionValue->getType()->isIntegerTy(1)) {
        // Already boolean
    } else {
        // For other types, just use as-is (this is a simplification)
        // TODO: Add proper type conversion
    }

    // Create basic blocks
    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "if.then", currentFunc);
    llvm::BasicBlock *elseBlock = node.hasElse()
                                      ? llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "if.else", currentFunc)
                                      : nullptr;
    llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "if.end", currentFunc);

    // Create conditional branch
    if (node.hasElse()) {
        codeGen_->getBuilder()->CreateCondBr(conditionValue, thenBlock, elseBlock);
    } else {
        codeGen_->getBuilder()->CreateCondBr(conditionValue, thenBlock, endBlock);
    }

    // Generate then block
    codeGen_->getBuilder()->SetInsertPoint(thenBlock);
    generateStatement(*node.getThenStatement());

    // Check if then block has terminator after generating its content
    llvm::BasicBlock *currentThenBlock = codeGen_->getBuilder()->GetInsertBlock();
    bool thenHasTerminator = currentThenBlock && currentThenBlock->getTerminator() != nullptr;

    // Generate cleanup for ARC-managed objects if the block doesn't have a terminator
    if (!thenHasTerminator && currentThenBlock) {
        // Generate cleanup before adding the branch
        codeGen_->getCodeGenContext()->generateScopeCleanup(codeGen_);
        codeGen_->getBuilder()->CreateBr(endBlock);
    }

    // Generate else block if present
    bool elseHasTerminator = false;
    if (node.hasElse()) {
        codeGen_->getBuilder()->SetInsertPoint(elseBlock);
        generateStatement(*node.getElseStatement());

        // Check if else block has terminator after generating its content
        llvm::BasicBlock *currentElseBlock = codeGen_->getBuilder()->GetInsertBlock();
        elseHasTerminator = currentElseBlock && currentElseBlock->getTerminator() != nullptr;
        if (!elseHasTerminator && currentElseBlock) {
            // Generate cleanup before adding the branch
            codeGen_->getCodeGenContext()->generateScopeCleanup(codeGen_);
            codeGen_->getBuilder()->CreateBr(endBlock);
        }
    }

    // Handle end block
    bool bothBranchesTerminate = thenHasTerminator && (!node.hasElse() || elseHasTerminator);

    if (!bothBranchesTerminate) {
        // At least one branch doesn't terminate - continue with end block
        codeGen_->getBuilder()->SetInsertPoint(endBlock);
    }
    // For unreachable blocks, the general terminator placement will handle it
}

void StatementGenerator::generateWhileStatement(const WhileStatement& node) {
    llvm::Function *currentFunc = codeGen_->getCodeGenContext()->getCurrentFunction();
    if (!currentFunc) {
        codeGen_->reportError("While statement outside function", node.getLocation());
        return;
    }

    // Create basic blocks
    llvm::BasicBlock *conditionBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "while.cond", currentFunc);
    llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "while.body", currentFunc);
    llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "while.end", currentFunc);

    // Jump to condition block
    codeGen_->getBuilder()->CreateBr(conditionBlock);

    // Generate condition
    codeGen_->getBuilder()->SetInsertPoint(conditionBlock);
    llvm::Value *conditionValue = codeGen_->generateExpression(*node.getCondition());

    if (!conditionValue) {
        codeGen_->reportError("Failed to generate while condition", node.getCondition()->getLocation());
        return;
    }

    // Convert condition to boolean (simplified for now)
    if (conditionValue->getType()->isDoubleTy()) {
        // Compare double to 0.0
        llvm::Value *zero = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*codeGen_->getLLVMContext()), 0.0);
        conditionValue = codeGen_->getBuilder()->CreateFCmpONE(conditionValue, zero, "tobool");
    } else if (conditionValue->getType()->isIntegerTy(1)) {
        // Already boolean
    } else {
        // For other types, just use as-is (this is a simplification)
        // TODO: Add proper type conversion
    }

    // Create conditional branch
    codeGen_->getBuilder()->CreateCondBr(conditionValue, bodyBlock, endBlock);

    // Generate body
    codeGen_->getBuilder()->SetInsertPoint(bodyBlock);
    generateStatement(*node.getBody());
    if (!codeGen_->getBuilder()->GetInsertBlock()->getTerminator()) {
        codeGen_->getBuilder()->CreateBr(conditionBlock);
    }

    // Continue with end block
    codeGen_->getBuilder()->SetInsertPoint(endBlock);
}

void StatementGenerator::generateDoWhileStatement(const DoWhileStatement& node) {
    // TODO: Implement do-while statement generation
}

void StatementGenerator::generateForStatement(const ForStatement& node) {
    // TODO: Implement for statement generation
}

void StatementGenerator::generateForOfStatement(const ForOfStatement& node) {
    // TODO: Implement for-of statement generation
}

void StatementGenerator::generateSwitchStatement(const SwitchStatement& node) {
    // TODO: Implement switch statement generation
}

void StatementGenerator::generateCaseClause(const CaseClause& node) {
    // TODO: Implement case clause generation
}

void StatementGenerator::generateBreakStatement(const BreakStatement& node) {
    // TODO: Implement break statement generation
}

void StatementGenerator::generateContinueStatement(const ContinueStatement& node) {
    // TODO: Implement continue statement generation
}

void StatementGenerator::generateTryStatement(const TryStatement& node) {
    // TODO: Implement try statement generation
}

void StatementGenerator::generateCatchClause(const CatchClause& node) {
    // TODO: Implement catch clause generation
}

void StatementGenerator::generateThrowStatement(const ThrowStatement& node) {
    // TODO: Implement throw statement generation
}

void StatementGenerator::generateVariableDeclaration(const VariableDeclaration& node) {
    // TODO: Implement variable declaration generation
}

llvm::BasicBlock* StatementGenerator::createBasicBlock(const String& name) {
    return llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), name);
}

void StatementGenerator::generateBranch(llvm::BasicBlock* block) {
    codeGen_->getBuilder()->CreateBr(block);
}

void StatementGenerator::generateConditionalBranch(llvm::Value* condition, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) {
    codeGen_->getBuilder()->CreateCondBr(condition, trueBlock, falseBlock);
}

void StatementGenerator::generateIfStatement(const unique_ptr<IfStatement>& stmt) {
    // TODO: Implement enhanced if statement generation
}

void StatementGenerator::generateWhileStatement(const unique_ptr<WhileStatement>& stmt) {
    // TODO: Implement enhanced while statement generation
}

void StatementGenerator::generateForStatement(const unique_ptr<ForStatement>& stmt) {
    // TODO: Implement enhanced for statement generation
}

void StatementGenerator::generateTryStatement(const unique_ptr<TryStatement>& stmt) {
    // TODO: Implement enhanced try statement generation
}

} // namespace codegen
} // namespace tsc