#include "tsc/codegen/statements/StatementGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/ast/ASTRefactored.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cassert>

namespace tsc {
namespace testing {

class StatementGeneratorUnitTests {
public:
    StatementGeneratorUnitTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== StatementGenerator Unit Tests ===" << std::endl;
        
        testExpressionStatement();
        testBlockStatement();
        testReturnStatement();
        testIfStatement();
        testWhileStatement();
        testBasicBlockCreation();
        testBranchGeneration();
        
        std::cout << "=== All StatementGenerator Unit Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    std::unique_ptr<LLVMCodeGen> codeGen_;
    std::unique_ptr<StatementGenerator> statementGen_;
    std::unique_ptr<SymbolTable> symbolTable_;
    std::unique_ptr<TypeSystem> typeSystem_;
    
    void setupTestEnvironment() {
        // Create test environment
        symbolTable_ = std::make_unique<SymbolTable>();
        typeSystem_ = std::make_unique<TypeSystem>();
        
        // Create code generator with test options
        CompilerOptions options;
        codeGen_ = std::make_unique<LLVMCodeGen>(*diagnostics_, options);
        
        // Create statement generator
        statementGen_ = std::make_unique<StatementGenerator>(codeGen_.get());
    }
    
    void testExpressionStatement() {
        std::cout << "\n--- Testing Expression Statement ---" << std::endl;
        
        try {
            // Create a simple numeric literal expression
            auto numericExpr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(numericExpr), SourceLocation());
            
            // Test that the statement can be generated without errors
            statementGen_->generateExpressionStatement(*exprStmt);
            
            std::cout << "✓ Expression statement generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Expression statement generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testBlockStatement() {
        std::cout << "\n--- Testing Block Statement ---" << std::endl;
        
        try {
            // Create a block with multiple statements
            std::vector<std::unique_ptr<ast::Statement>> statements;
            
            // Add expression statement
            auto expr1 = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            statements.push_back(ast::ASTFactory::createExpressionStatement(std::move(expr1), SourceLocation()));
            
            // Add another expression statement
            auto expr2 = ast::ASTFactory::createStringLiteral("test", SourceLocation());
            statements.push_back(ast::ASTFactory::createExpressionStatement(std::move(expr2), SourceLocation()));
            
            auto blockStmt = ast::ASTFactory::createBlockStatement(std::move(statements), SourceLocation());
            
            // Test that the block can be generated without errors
            statementGen_->generateBlockStatement(*blockStmt);
            
            std::cout << "✓ Block statement generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Block statement generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testReturnStatement() {
        std::cout << "\n--- Testing Return Statement ---" << std::endl;
        
        try {
            // Create a return statement with value
            auto returnExpr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto returnStmt = ast::ASTFactory::createReturnStatement(std::move(returnExpr), SourceLocation());
            
            // Test that the return statement can be generated without errors
            statementGen_->generateReturnStatement(*returnStmt);
            
            std::cout << "✓ Return statement with value: PASSED" << std::endl;
            
            // Test return statement without value
            auto voidReturnStmt = ast::ASTFactory::createReturnStatement(nullptr, SourceLocation());
            statementGen_->generateReturnStatement(*voidReturnStmt);
            
            std::cout << "✓ Return statement without value: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Return statement generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testIfStatement() {
        std::cout << "\n--- Testing If Statement ---" << std::endl;
        
        try {
            // Create if statement without else
            auto condition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto thenStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(condition),
                std::move(thenStmt),
                nullptr, // no else
                SourceLocation()
            );
            
            // Test that the if statement can be generated without errors
            statementGen_->generateIfStatement(*ifStmt);
            
            std::cout << "✓ If statement without else: PASSED" << std::endl;
            
            // Create if statement with else
            auto condition2 = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            auto thenStmt2 = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            auto elseStmt2 = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()),
                SourceLocation()
            );
            auto ifStmt2 = ast::ASTFactory::createIfStatement(
                std::move(condition2),
                std::move(thenStmt2),
                std::move(elseStmt2),
                SourceLocation()
            );
            
            // Test that the if-else statement can be generated without errors
            statementGen_->generateIfStatement(*ifStmt2);
            
            std::cout << "✓ If statement with else: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ If statement generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testWhileStatement() {
        std::cout << "\n--- Testing While Statement ---" << std::endl;
        
        try {
            // Create while statement
            auto condition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto bodyStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            auto whileStmt = ast::ASTFactory::createWhileStatement(
                std::move(condition),
                std::move(bodyStmt),
                SourceLocation()
            );
            
            // Test that the while statement can be generated without errors
            statementGen_->generateWhileStatement(*whileStmt);
            
            std::cout << "✓ While statement generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ While statement generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testBasicBlockCreation() {
        std::cout << "\n--- Testing Basic Block Creation ---" << std::endl;
        
        try {
            // Test basic block creation
            llvm::BasicBlock* block = statementGen_->createBasicBlock("test_block");
            
            assert(block != nullptr);
            assert(block->getName() == "test_block");
            
            std::cout << "✓ Basic block creation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Basic block creation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testBranchGeneration() {
        std::cout << "\n--- Testing Branch Generation ---" << std::endl;
        
        try {
            // Create test basic blocks
            llvm::BasicBlock* block1 = statementGen_->createBasicBlock("block1");
            llvm::BasicBlock* block2 = statementGen_->createBasicBlock("block2");
            
            // Test unconditional branch
            statementGen_->generateBranch(block1);
            
            // Test conditional branch
            llvm::Value* condition = llvm::ConstantInt::get(llvm::Type::getInt1Ty(*codeGen_->getLLVMContext()), 1);
            statementGen_->generateConditionalBranch(condition, block1, block2);
            
            std::cout << "✓ Branch generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Branch generation: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::StatementGeneratorUnitTests test;
    test.runAllTests();
    return 0;
}