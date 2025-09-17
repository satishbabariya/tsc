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

class CompleteCodeGenIntegrationTests {
public:
    CompleteCodeGenIntegrationTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== Complete Code Generation Integration Tests ===" << std::endl;
        
        testTypeGeneratorIntegration();
        testExpressionGeneratorIntegration();
        testStatementGeneratorIntegration();
        testFunctionGeneratorIntegration();
        testBuiltinGeneratorIntegration();
        testCompleteModuleGeneration();
        testLLVMIRVerification();
        testErrorHandling();
        
        std::cout << "=== All Complete Code Generation Integration Tests Completed ===" << std::endl;
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
    
    void testTypeGeneratorIntegration() {
        std::cout << "\n--- Testing TypeGenerator Integration ---" << std::endl;
        
        try {
            auto typeGen = codeGen_->getTypeGenerator();
            
            // Test basic type generation
            llvm::Type* stringType = typeGen->getStringType();
            assert(stringType != nullptr);
            assert(stringType->isPointerTy());
            
            llvm::Type* numberType = typeGen->getNumberType();
            assert(numberType != nullptr);
            assert(numberType->isDoubleTy());
            
            llvm::Type* booleanType = typeGen->getBooleanType();
            assert(booleanType != nullptr);
            assert(booleanType->isIntegerTy(1));
            
            llvm::Type* voidType = typeGen->getVoidType();
            assert(voidType != nullptr);
            assert(voidType->isVoidTy());
            
            llvm::Type* anyType = typeGen->getAnyType();
            assert(anyType != nullptr);
            assert(anyType->isPointerTy());
            
            // Test type compatibility
            assert(typeGen->isCompatible(anyType, stringType));
            assert(typeGen->isCompatible(numberType, numberType));
            assert(!typeGen->isCompatible(stringType, numberType));
            
            std::cout << "✓ TypeGenerator integration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ TypeGenerator integration: Exception: " << e.what() << std::endl;
        }
    }
    
    void testExpressionGeneratorIntegration() {
        std::cout << "\n--- Testing ExpressionGenerator Integration ---" << std::endl;
        
        try {
            // Test numeric literal generation
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            llvm::Value* numericValue = codeGen_->generateExpression(*numericLit);
            assert(numericValue != nullptr);
            assert(numericValue->getType()->isDoubleTy());
            
            // Test string literal generation
            auto stringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            llvm::Value* stringValue = codeGen_->generateExpression(*stringLit);
            assert(stringValue != nullptr);
            assert(stringValue->getType()->isPointerTy());
            
            // Test boolean literal generation
            auto boolLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            llvm::Value* boolValue = codeGen_->generateExpression(*boolLit);
            assert(boolValue != nullptr);
            assert(boolValue->getType()->isIntegerTy(1));
            
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
            
            std::cout << "✓ ExpressionGenerator integration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ ExpressionGenerator integration: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStatementGeneratorIntegration() {
        std::cout << "\n--- Testing StatementGenerator Integration ---" << std::endl;
        
        try {
            // Create a simple function for statement testing
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>(),
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
            
            // Generate the function (which tests statement generation)
            codeGen_->generateDeclaration(*funcDecl);
            
            // Test expression statement generation
            auto expr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(expr), SourceLocation());
            
            codeGen_->generateStatement(*exprStmt);
            
            std::cout << "✓ StatementGenerator integration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ StatementGenerator integration: Exception: " << e.what() << std::endl;
        }
    }
    
    void testFunctionGeneratorIntegration() {
        std::cout << "\n--- Testing FunctionGenerator Integration ---" << std::endl;
        
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
            
            // Verify function was created
            llvm::Function* generatedFunc = codeGen_->getModule()->getFunction("testFunc");
            assert(generatedFunc != nullptr);
            assert(generatedFunc->getReturnType() != nullptr);
            
            std::cout << "✓ FunctionGenerator integration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ FunctionGenerator integration: Exception: " << e.what() << std::endl;
        }
    }
    
    void testBuiltinGeneratorIntegration() {
        std::cout << "\n--- Testing BuiltinGenerator Integration ---" << std::endl;
        
        try {
            auto builtinGen = codeGen_->getBuiltinGenerator();
            
            // Test that built-in functions are available
            llvm::Function* printFunc = builtinGen->getOrCreatePrintFunction();
            assert(printFunc != nullptr);
            assert(printFunc->getName() == "_print");
            
            llvm::Function* mallocFunc = builtinGen->getOrCreateMallocFunction();
            assert(mallocFunc != nullptr);
            assert(mallocFunc->getName() == "malloc");
            
            llvm::Function* stringConcatFunc = builtinGen->getOrCreateStringConcatFunction();
            assert(stringConcatFunc != nullptr);
            assert(stringConcatFunc->getName() == "string_concat");
            
            // Test builtin function detection
            assert(builtinGen->isBuiltinFunction("_print"));
            assert(builtinGen->isBuiltinFunction("malloc"));
            assert(!builtinGen->isBuiltinFunction("userFunction"));
            
            std::cout << "✓ BuiltinGenerator integration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ BuiltinGenerator integration: Exception: " << e.what() << std::endl;
        }
    }
    
    void testCompleteModuleGeneration() {
        std::cout << "\n--- Testing Complete Module Generation ---" << std::endl;
        
        try {
            // Create a complete module with multiple declarations
            std::vector<std::unique_ptr<ast::Declaration>> declarations;
            
            // Add a variable declaration
            declarations.push_back(ast::ASTFactory::createVariableDeclaration(
                ast::declarations::VariableDeclaration::Kind::Let,
                "testVar",
                ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                nullptr,
                SourceLocation()
            ));
            
            // Add a function declaration
            auto funcBody = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{
                    ast::ASTFactory::createReturnStatement(
                        ast::ASTFactory::createNumericLiteral(10.0, SourceLocation()),
                        SourceLocation()
                    )
                },
                SourceLocation()
            );
            
            declarations.push_back(ast::ASTFactory::createFunctionDeclaration(
                "testFunction",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            ));
            
            // Create module
            ast::Module testModule(std::move(declarations));
            
            // Generate code for the entire module
            bool success = codeGen_->generateCode(testModule);
            assert(success);
            
            // Verify module contents
            llvm::Module* generatedModule = codeGen_->getModule();
            assert(generatedModule != nullptr);
            
            llvm::Function* testFunc = generatedModule->getFunction("testFunction");
            assert(testFunc != nullptr);
            
            std::cout << "✓ Complete module generation: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Complete module generation: Exception: " << e.what() << std::endl;
        }
    }
    
    void testLLVMIRVerification() {
        std::cout << "\n--- Testing LLVM IR Verification ---" << std::endl;
        
        try {
            // Generate some code first
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
                "verifyTestFunc",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                std::move(funcBody),
                SourceLocation()
            );
            
            codeGen_->generateDeclaration(*funcDecl);
            
            // Verify the module
            bool verificationResult = codeGen_->verifyModule();
            assert(verificationResult);
            
            std::cout << "✓ LLVM IR verification: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ LLVM IR verification: Exception: " << e.what() << std::endl;
        }
    }
    
    void testErrorHandling() {
        std::cout << "\n--- Testing Error Handling ---" << std::endl;
        
        try {
            // Test error reporting
            codeGen_->reportError("Test error message", SourceLocation());
            assert(codeGen_->hasErrors());
            
            // Test that errors are properly tracked
            int errorCount = diagnostics_->getErrorCount();
            assert(errorCount > 0);
            
            std::cout << "✓ Error handling: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Error handling: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::CompleteCodeGenIntegrationTests test;
    test.runAllTests();
    return 0;
}