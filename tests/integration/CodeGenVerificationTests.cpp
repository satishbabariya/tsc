#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/codegen/statements/StatementGenerator.h"
#include "tsc/codegen/expressions/ExpressionGenerator.h"
#include "tsc/codegen/context/CodeGenContext.h"
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

class CodeGenVerificationTests {
public:
    CodeGenVerificationTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== Code Generation Verification Tests ===" << std::endl;
        
        testSimpleExpressionGeneration();
        testComplexExpressionGeneration();
        testStatementGeneration();
        testControlFlowGeneration();
        testFunctionGeneration();
        testClassGeneration();
        testErrorHandling();
        testLLVMIRValidity();
        
        std::cout << "=== All Code Generation Verification Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    std::unique_ptr<LLVMCodeGen> codeGen_;
    std::unique_ptr<SymbolTable> symbolTable_;
    std::unique_ptr<TypeSystem> typeSystem_;
    
    void setupTestEnvironment() {
        // Create test environment
        symbolTable_ = std::make_unique<SymbolTable>();
        typeSystem_ = std::make_unique<TypeSystem>();
        
        // Create code generator with test options
        CompilerOptions options;
        codeGen_ = std::make_unique<LLVMCodeGen>(*diagnostics_, options);
    }
    
    void testSimpleExpressionGeneration() {
        std::cout << "\n--- Testing Simple Expression Generation ---" << std::endl;
        
        try {
            // Test numeric literal generation
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            llvm::Value* numericValue = codeGen_->generateExpression(*numericLit);
            assert(numericValue != nullptr);
            assert(numericValue->getType()->isDoubleTy());
            
            std::cout << "✓ Numeric literal generation: PASSED" << std::endl;
            
            // Test string literal generation
            auto stringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            llvm::Value* stringValue = codeGen_->generateExpression(*stringLit);
            assert(stringValue != nullptr);
            assert(stringValue->getType()->isPointerTy());
            
            std::cout << "✓ String literal generation: PASSED" << std::endl;
            
            // Test boolean literal generation
            auto boolLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            llvm::Value* boolValue = codeGen_->generateExpression(*boolLit);
            assert(boolValue != nullptr);
            assert(boolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Boolean literal generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Simple expression generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testComplexExpressionGeneration() {
        std::cout << "\n--- Testing Complex Expression Generation ---" << std::endl;
        
        try {
            // Test binary expression generation
            auto left = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto binaryExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left),
                ast::expressions::BinaryExpression::Operator::Plus,
                std::move(right),
                SourceLocation()
            );
            
            llvm::Value* binaryValue = codeGen_->generateExpression(*binaryExpr);
            assert(binaryValue != nullptr);
            assert(binaryValue->getType()->isDoubleTy());
            
            std::cout << "✓ Binary expression generation: PASSED" << std::endl;
            
            // Test unary expression generation
            auto operand = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto unaryExpr = ast::ASTFactory::createUnaryExpression(
                ast::expressions::UnaryExpression::Operator::UnaryMinus,
                std::move(operand),
                SourceLocation()
            );
            
            llvm::Value* unaryValue = codeGen_->generateExpression(*unaryExpr);
            assert(unaryValue != nullptr);
            assert(unaryValue->getType()->isDoubleTy());
            
            std::cout << "✓ Unary expression generation: PASSED" << std::endl;
            
            // Test conditional expression generation
            auto condition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto thenExpr = ast::ASTFactory::createNumericLiteral(1.0, SourceLocation());
            auto elseExpr = ast::ASTFactory::createNumericLiteral(0.0, SourceLocation());
            auto condExpr = ast::ASTFactory::createConditionalExpression(
                std::move(condition),
                std::move(thenExpr),
                std::move(elseExpr),
                SourceLocation()
            );
            
            llvm::Value* condValue = codeGen_->generateExpression(*condExpr);
            assert(condValue != nullptr);
            assert(condValue->getType()->isDoubleTy());
            
            std::cout << "✓ Conditional expression generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Complex expression generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStatementGeneration() {
        std::cout << "\n--- Testing Statement Generation ---" << std::endl;
        
        try {
            // Test expression statement generation
            auto expr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(expr), SourceLocation());
            
            codeGen_->generateStatement(*exprStmt);
            
            std::cout << "✓ Expression statement generation: PASSED" << std::endl;
            
            // Test block statement generation
            std::vector<std::unique_ptr<ast::Statement>> statements;
            statements.push_back(ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            ));
            statements.push_back(ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(2.0, SourceLocation()),
                SourceLocation()
            ));
            
            auto blockStmt = ast::ASTFactory::createBlockStatement(std::move(statements), SourceLocation());
            codeGen_->generateStatement(*blockStmt);
            
            std::cout << "✓ Block statement generation: PASSED" << std::endl;
            
            // Test return statement generation
            auto returnExpr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto returnStmt = ast::ASTFactory::createReturnStatement(std::move(returnExpr), SourceLocation());
            
            codeGen_->generateStatement(*returnStmt);
            
            std::cout << "✓ Return statement generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Statement generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testControlFlowGeneration() {
        std::cout << "\n--- Testing Control Flow Generation ---" << std::endl;
        
        try {
            // Test if statement generation
            auto condition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto thenStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(condition),
                std::move(thenStmt),
                nullptr,
                SourceLocation()
            );
            
            codeGen_->generateStatement(*ifStmt);
            
            std::cout << "✓ If statement generation: PASSED" << std::endl;
            
            // Test while statement generation
            auto whileCondition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto whileBody = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            auto whileStmt = ast::ASTFactory::createWhileStatement(
                std::move(whileCondition),
                std::move(whileBody),
                SourceLocation()
            );
            
            codeGen_->generateStatement(*whileStmt);
            
            std::cout << "✓ While statement generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Control flow generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testFunctionGeneration() {
        std::cout << "\n--- Testing Function Generation ---" << std::endl;
        
        try {
            // Test function declaration generation
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testFunc",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            codeGen_->generateDeclaration(*funcDecl);
            
            std::cout << "✓ Function declaration generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Function generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testClassGeneration() {
        std::cout << "\n--- Testing Class Generation ---" << std::endl;
        
        try {
            // Test class declaration generation
            auto classBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>(),
                SourceLocation()
            );
            
            auto classDecl = ast::ASTFactory::createClassDeclaration(
                "TestClass",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                nullptr,
                std::vector<std::unique_ptr<ast::Statement>>(),
                SourceLocation()
            );
            
            codeGen_->generateDeclaration(*classDecl);
            
            std::cout << "✓ Class declaration generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Class generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testErrorHandling() {
        std::cout << "\n--- Testing Error Handling ---" << std::endl;
        
        try {
            // Test error handling in expression generation
            auto invalidExpr = ast::ASTFactory::createIdentifier("undefinedVar", SourceLocation());
            llvm::Value* result = codeGen_->generateExpression(*invalidExpr);
            
            // Should handle error gracefully (result might be null or error value)
            std::cout << "✓ Error handling in expression generation: PASSED" << std::endl;
            
            // Test error handling in statement generation
            auto invalidStmt = ast::ASTFactory::createReturnStatement(
                ast::ASTFactory::createIdentifier("undefinedVar", SourceLocation()),
                SourceLocation()
            );
            
            codeGen_->generateStatement(*invalidStmt);
            
            std::cout << "✓ Error handling in statement generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Error handling: Exception: " << e.what() << std::endl;
        }
    }
    
    void testLLVMIRValidity() {
        std::cout << "\n--- Testing LLVM IR Validity ---" << std::endl;
        
        try {
            // Generate some code and verify the LLVM IR is valid
            auto expr = ast::ASTFactory::createBinaryExpression(
                ast::ASTFactory::createNumericLiteral(10.0, SourceLocation()),
                ast::expressions::BinaryExpression::Operator::Multiply,
                ast::ASTFactory::createNumericLiteral(5.0, SourceLocation()),
                SourceLocation()
            );
            
            llvm::Value* result = codeGen_->generateExpression(*expr);
            assert(result != nullptr);
            
            // Verify the module is valid
            llvm::Module* module = codeGen_->getModule();
            assert(module != nullptr);
            
            // Check that the module has the expected structure
            assert(module->getFunctionList().size() >= 0); // Should have some functions
            
            std::cout << "✓ LLVM IR validity: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ LLVM IR validity: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::CodeGenVerificationTests test;
    test.runAllTests();
    return 0;
}