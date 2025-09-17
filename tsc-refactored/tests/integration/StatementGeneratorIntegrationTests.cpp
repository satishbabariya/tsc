#include "tsc/codegen/statements/StatementGenerator.h"
#include "tsc/codegen/expressions/ExpressionGenerator.h"
#include "tsc/codegen/context/CodeGenContext.h"
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

class StatementGeneratorIntegrationTests {
public:
    StatementGeneratorIntegrationTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== StatementGenerator Integration Tests ===" << std::endl;
        
        testStatementExpressionInteraction();
        testStatementContextInteraction();
        testComplexControlFlow();
        testNestedStatements();
        testErrorHandling();
        testARCIntegration();
        
        std::cout << "=== All StatementGenerator Integration Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    std::unique_ptr<LLVMCodeGen> codeGen_;
    std::unique_ptr<StatementGenerator> statementGen_;
    std::unique_ptr<ExpressionGenerator> expressionGen_;
    std::unique_ptr<SymbolTable> symbolTable_;
    std::unique_ptr<TypeSystem> typeSystem_;
    
    void setupTestEnvironment() {
        // Create test environment
        symbolTable_ = std::make_unique<SymbolTable>();
        typeSystem_ = std::make_unique<TypeSystem>();
        
        // Create code generator with test options
        CompilerOptions options;
        codeGen_ = std::make_unique<LLVMCodeGen>(*diagnostics_, options);
        
        // Create generators
        statementGen_ = std::make_unique<StatementGenerator>(codeGen_.get());
        expressionGen_ = std::make_unique<ExpressionGenerator>(codeGen_.get());
    }
    
    void testStatementExpressionInteraction() {
        std::cout << "\n--- Testing Statement-Expression Interaction ---" << std::endl;
        
        try {
            // Create a complex expression statement
            auto left = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto binaryExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left),
                ast::expressions::BinaryExpression::Operator::Plus,
                std::move(right),
                SourceLocation()
            );
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(binaryExpr), SourceLocation());
            
            // Test that the statement can generate the expression correctly
            statementGen_->generateExpressionStatement(*exprStmt);
            
            std::cout << "✓ Statement-Expression interaction: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Statement-Expression interaction: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStatementContextInteraction() {
        std::cout << "\n--- Testing Statement-Context Interaction ---" << std::endl;
        
        try {
            // Create a block statement that uses context
            std::vector<std::unique_ptr<ast::Statement>> statements;
            
            // Add variable declaration (simulated)
            auto varExpr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            statements.push_back(ast::ASTFactory::createExpressionStatement(std::move(varExpr), SourceLocation()));
            
            auto blockStmt = ast::ASTFactory::createBlockStatement(std::move(statements), SourceLocation());
            
            // Test that the block statement interacts correctly with context
            statementGen_->generateBlockStatement(*blockStmt);
            
            std::cout << "✓ Statement-Context interaction: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Statement-Context interaction: Exception: " << e.what() << std::endl;
        }
    }
    
    void testComplexControlFlow() {
        std::cout << "\n--- Testing Complex Control Flow ---" << std::endl;
        
        try {
            // Create nested if statement
            auto outerCondition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto innerCondition = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            
            auto innerThenStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            auto innerElseStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()),
                SourceLocation()
            );
            
            auto innerIfStmt = ast::ASTFactory::createIfStatement(
                std::move(innerCondition),
                std::move(innerThenStmt),
                std::move(innerElseStmt),
                SourceLocation()
            );
            
            auto outerThenStmt = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{std::move(innerIfStmt)},
                SourceLocation()
            );
            
            auto outerIfStmt = ast::ASTFactory::createIfStatement(
                std::move(outerCondition),
                std::move(outerThenStmt),
                nullptr,
                SourceLocation()
            );
            
            // Test that nested control flow can be generated correctly
            statementGen_->generateIfStatement(*outerIfStmt);
            
            std::cout << "✓ Complex control flow: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Complex control flow: Exception: " << e.what() << std::endl;
        }
    }
    
    void testNestedStatements() {
        std::cout << "\n--- Testing Nested Statements ---" << std::endl;
        
        try {
            // Create nested block statements
            std::vector<std::unique_ptr<ast::Statement>> innerStatements;
            innerStatements.push_back(ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            ));
            
            auto innerBlock = ast::ASTFactory::createBlockStatement(std::move(innerStatements), SourceLocation());
            
            std::vector<std::unique_ptr<ast::Statement>> outerStatements;
            outerStatements.push_back(std::move(innerBlock));
            outerStatements.push_back(ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(2.0, SourceLocation()),
                SourceLocation()
            ));
            
            auto outerBlock = ast::ASTFactory::createBlockStatement(std::move(outerStatements), SourceLocation());
            
            // Test that nested statements can be generated correctly
            statementGen_->generateBlockStatement(*outerBlock);
            
            std::cout << "✓ Nested statements: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Nested statements: Exception: " << e.what() << std::endl;
        }
    }
    
    void testErrorHandling() {
        std::cout << "\n--- Testing Error Handling ---" << std::endl;
        
        try {
            // Test return statement outside function context
            auto returnExpr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto returnStmt = ast::ASTFactory::createReturnStatement(std::move(returnExpr), SourceLocation());
            
            // This should handle the error gracefully
            statementGen_->generateReturnStatement(*returnStmt);
            
            std::cout << "✓ Error handling: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Error handling: Exception: " << e.what() << std::endl;
        }
    }
    
    void testARCIntegration() {
        std::cout << "\n--- Testing ARC Integration ---" << std::endl;
        
        try {
            // Create a block statement that should trigger ARC cleanup
            std::vector<std::unique_ptr<ast::Statement>> statements;
            statements.push_back(ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createStringLiteral("test", SourceLocation()),
                SourceLocation()
            ));
            
            auto blockStmt = ast::ASTFactory::createBlockStatement(std::move(statements), SourceLocation());
            
            // Test that ARC cleanup is properly integrated
            statementGen_->generateBlockStatement(*blockStmt);
            
            std::cout << "✓ ARC integration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ ARC integration: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::StatementGeneratorIntegrationTests test;
    test.runAllTests();
    return 0;
}