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

class StringToBooleanIntegrationTests {
public:
    StringToBooleanIntegrationTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== String to Boolean Integration Tests ===" << std::endl;
        
        testIfStatementWithStringConditions();
        testWhileStatementWithStringConditions();
        testStringComparisonInControlFlow();
        testMixedStringBooleanExpressions();
        testStringCoercionInComplexLogic();
        testEmptyStringHandling();
        testNullStringHandling();
        
        std::cout << "=== All String to Boolean Integration Tests Completed ===" << std::endl;
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
    
    void testIfStatementWithStringConditions() {
        std::cout << "\n--- Testing If Statement with String Conditions ---" << std::endl;
        
        try {
            // Test if statement with non-empty string condition
            auto nonEmptyStringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            auto thenStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            auto elseStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()),
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(nonEmptyStringLit),
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
                "testIfString",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testIfString");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ If statement with string conditions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ If statement with string conditions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testWhileStatementWithStringConditions() {
        std::cout << "\n--- Testing While Statement with String Conditions ---" << std::endl;
        
        try {
            // Test while statement with string condition
            auto stringLit = ast::ASTFactory::createStringLiteral("test", SourceLocation());
            auto bodyStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                SourceLocation()
            );
            
            auto whileStmt = ast::ASTFactory::createWhileStatement(
                std::move(stringLit),
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
                "testWhileString",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testWhileString");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ While statement with string conditions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ While statement with string conditions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStringComparisonInControlFlow() {
        std::cout << "\n--- Testing String Comparison in Control Flow ---" << std::endl;
        
        try {
            // Test string comparison in if condition
            auto leftString = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            auto rightString = ast::ASTFactory::createStringLiteral("world", SourceLocation());
            
            auto stringComparison = ast::ASTFactory::createBinaryExpression(
                std::move(leftString),
                ast::expressions::BinaryExpression::Operator::Equal,
                std::move(rightString),
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(stringComparison),
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
                "testStringComparison",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testStringComparison");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ String comparison in control flow: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ String comparison in control flow: Exception: " << e.what() << std::endl;
        }
    }
    
    void testMixedStringBooleanExpressions() {
        std::cout << "\n--- Testing Mixed String Boolean Expressions ---" << std::endl;
        
        try {
            // Test: string && boolean
            auto stringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            auto boolLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            
            auto mixedAnd = ast::ASTFactory::createBinaryExpression(
                std::move(stringLit),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(boolLit),
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
                "testMixedStringBoolean",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testMixedStringBoolean");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ Mixed string boolean expressions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Mixed string boolean expressions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStringCoercionInComplexLogic() {
        std::cout << "\n--- Testing String Coercion in Complex Logic ---" << std::endl;
        
        try {
            // Test complex expression: (string && number) || (string == "test")
            auto stringLit1 = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            auto numberLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            
            auto leftAnd = ast::ASTFactory::createBinaryExpression(
                std::move(stringLit1),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(numberLit),
                SourceLocation()
            );
            
            auto stringLit2 = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            auto testStringLit = ast::ASTFactory::createStringLiteral("test", SourceLocation());
            
            auto stringComparison = ast::ASTFactory::createBinaryExpression(
                std::move(stringLit2),
                ast::expressions::BinaryExpression::Operator::Equal,
                std::move(testStringLit),
                SourceLocation()
            );
            
            auto complexOr = ast::ASTFactory::createBinaryExpression(
                std::move(leftAnd),
                ast::expressions::BinaryExpression::Operator::LogicalOr,
                std::move(stringComparison),
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(complexOr),
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
                "testStringCoercionComplex",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testStringCoercionComplex");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ String coercion in complex logic: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ String coercion in complex logic: Exception: " << e.what() << std::endl;
        }
    }
    
    void testEmptyStringHandling() {
        std::cout << "\n--- Testing Empty String Handling ---" << std::endl;
        
        try {
            // Test empty string in if condition - should be false
            auto emptyStringLit = ast::ASTFactory::createStringLiteral("", SourceLocation());
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(emptyStringLit),
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
                "testEmptyString",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testEmptyString");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ Empty string handling: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Empty string handling: Exception: " << e.what() << std::endl;
        }
    }
    
    void testNullStringHandling() {
        std::cout << "\n--- Testing Null String Handling ---" << std::endl;
        
        try {
            // Test null string in if condition - should be false
            auto nullLit = ast::ASTFactory::createNullLiteral(SourceLocation());
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(nullLit),
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
                "testNullString",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            // Generate the function
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testNullString");
            assert(generatedFunc != nullptr);
            
            // Verify module is valid
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ Null string handling: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Null string handling: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::StringToBooleanIntegrationTests test;
    test.runAllTests();
    return 0;
}