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

class BooleanExpressionIntegrationTests {
public:
    BooleanExpressionIntegrationTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== Boolean Expression Integration Tests ===" << std::endl;
        
        testIfStatementWithBooleanExpressions();
        testWhileStatementWithBooleanExpressions();
        testNestedControlFlowWithBooleanExpressions();
        testComplexBooleanLogicInControlFlow();
        testTypeCoercionInBooleanExpressions();
        testShortCircuitingInControlFlow();
        
        std::cout << "=== All Boolean Expression Integration Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    std::unique_ptr<codegen::LLVMCodeGenRefactored> codeGen_;
    std::unique_ptr<SymbolTable> symbolTable_;
    std::unique_ptr<TypeSystem> typeSystem_;
    
    void setupTestEnvironment() {
        // Create test environment
        symbolTable_ = std::make_unique<SymbolTable>();
        typeSystem_ = std::make_unique<TypeSystem>();
        
        // Create code generator with test options
        CompilerOptions options;
        codeGen_ = std::make_unique<codegen::LLVMCodeGenRefactored>(*diagnostics_, options);
        codeGen_->setSymbolTable(symbolTable_.get());
    }
    
    void testIfStatementWithBooleanExpressions() {
        std::cout << "\n--- Testing If Statement with Boolean Expressions ---" << std::endl;
        
        try {
            // Create a function with if statement using boolean expressions
            auto condition = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(10.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Greater,
                ast::ASTFactory::createNumericLiteral(5.0, SourceLocation()),
                SourceLocation()
            );
            
            auto thenStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            
            auto elseStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()),
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(condition),
                std::move(thenStmt),
                std::move(elseStmt),
                SourceLocation()
            );
            
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    std::move(ifStmt),
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testIfBoolean",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testIfBoolean");
            assert(generatedFunc != nullptr);
            assert(generatedFunc->getReturnType() != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ If statement with boolean expressions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ If statement with boolean expressions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testWhileStatementWithBooleanExpressions() {
        std::cout << "\n--- Testing While Statement with Boolean Expressions ---" << std::endl;
        
        try {
            // Create a function with while statement using boolean expressions
            auto condition = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(10.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Greater,
                ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()),
                SourceLocation()
            );
            
            auto bodyStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            
            auto whileStmt = ast::ASTFactory::createWhileStatement(
                std::move(condition),
                std::move(bodyStmt),
                SourceLocation()
            );
            
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    std::move(whileStmt),
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testWhileBoolean",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testWhileBoolean");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ While statement with boolean expressions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ While statement with boolean expressions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testNestedControlFlowWithBooleanExpressions() {
        std::cout << "\n--- Testing Nested Control Flow with Boolean Expressions ---" << std::endl;
        
        try {
            // Create nested if statements with complex boolean expressions
            auto outerCondition = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(10.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Greater,
                ast::ASTFactory::createNumericLiteral(5.0, SourceLocation()),
                SourceLocation()
            );
            
            auto innerCondition = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(3.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Less,
                ast::ASTFactory::createNumericLiteral(7.0, SourceLocation()),
                SourceLocation()
            );
            
            auto innerThenStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            
            auto innerIfStmt = ast::ASTFactory::createIfStatement(
                std::move(innerCondition),
                std::move(innerThenStmt),
                nullptr,
                SourceLocation()
            );
            
            auto outerThenStmt = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    std::move(innerIfStmt)
                },
                SourceLocation()
            );
            
            auto outerIfStmt = ast::ASTFactory::createIfStatement(
                std::move(outerCondition),
                std::move(outerThenStmt),
                nullptr,
                SourceLocation()
            );
            
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    std::move(outerIfStmt),
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testNestedBoolean",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testNestedBoolean");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ Nested control flow with boolean expressions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Nested control flow with boolean expressions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testComplexBooleanLogicInControlFlow() {
        std::cout << "\n--- Testing Complex Boolean Logic in Control Flow ---" << std::endl;
        
        try {
            // Create complex boolean expression: (a > b) && (c < d) || (e == f)
            auto leftAnd = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(10.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Greater,
                ast::ASTFactory::createNumericLiteral(5.0, SourceLocation()),
                SourceLocation()
            );
            
            auto rightAnd = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(3.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Less,
                ast::ASTFactory::createNumericLiteral(7.0, SourceLocation()),
                SourceLocation()
            );
            
            auto andExpr = ast::ASTFactory::createBinaryExpression(
                std::move(leftAnd),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(rightAnd),
                SourceLocation()
            );
            
            auto rightOr = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(5.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Equal,
                ast::ASTFactory::createNumericLiteral(5.0, SourceLocation()),
                SourceLocation()
            );
            
            auto complexCondition = ast::ASTFactory::createBinaryExpression(
                std::move(andExpr),
                ast::expressions::BinaryExpression::Operator::LogicalOr,
                std::move(rightOr),
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(complexCondition),
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                    SourceLocation()
                ),
                nullptr,
                SourceLocation()
            );
            
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    std::move(ifStmt),
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testComplexBoolean",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testComplexBoolean");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ Complex boolean logic in control flow: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Complex boolean logic in control flow: Exception: " << e.what() << std::endl;
        }
    }
    
    void testTypeCoercionInBooleanExpressions() {
        std::cout << "\n--- Testing Type Coercion in Boolean Expressions ---" << std::endl;
        
        try {
            // Test mixing numeric and boolean types in expressions
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto booleanLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            
            // Test: number && boolean
            auto mixedAnd = ast::ASTFactory::createBinaryExpression(
                std::move(numericLit),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(booleanLit),
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(mixedAnd),
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                    SourceLocation()
                ),
                nullptr,
                SourceLocation()
            );
            
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    std::move(ifStmt),
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testTypeCoercion",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testTypeCoercion");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ Type coercion in boolean expressions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Type coercion in boolean expressions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testShortCircuitingInControlFlow() {
        std::cout << "\n--- Testing Short-Circuiting in Control Flow ---" << std::endl;
        
        try {
            // Test short-circuiting in if condition: false && (expensive operation)
            auto falseLit = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            auto expensiveOp = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(100.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Multiply,
                ast::ASTFactory::createNumericLiteral(200.0, SourceLocation()),
                SourceLocation()
            );
            
            auto shortCircuitCondition = ast::ASTFactory::createBinaryExpression(
                std::move(falseLit),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(expensiveOp),
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(shortCircuitCondition),
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                    SourceLocation()
                ),
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()),
                    SourceLocation()
                ),
                SourceLocation()
            );
            
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    std::move(ifStmt),
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testShortCircuit",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testShortCircuit");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ Short-circuiting in control flow: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Short-circuiting in control flow: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::BooleanExpressionIntegrationTests test;
    test.runAllTests();
    return 0;
}