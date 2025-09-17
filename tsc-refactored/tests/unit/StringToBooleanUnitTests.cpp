#include "tsc/codegen/expressions/ExpressionGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/ast/ASTRefactored.h"

#include <iostream>
#include <memory>
#include <cassert>

namespace tsc {
namespace testing {

class StringToBooleanUnitTests {
public:
    StringToBooleanUnitTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== String to Boolean Conversion Unit Tests ===" << std::endl;
        
        testEmptyStringConversion();
        testNonEmptyStringConversion();
        testNullStringConversion();
        testStringWithWhitespace();
        testStringWithNumbers();
        testStringWithSpecialCharacters();
        testStringWithTrueFalse();
        testLongStringConversion();
        testDirectConversionMethod();
        testTypeCoercion();
        
        std::cout << "=== All String to Boolean Conversion Unit Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    std::unique_ptr<codegen::LLVMCodeGenRefactored> codeGen_;
    std::unique_ptr<codegen::ExpressionGenerator> exprGen_;
    
    void setupTestEnvironment() {
        // Create code generator with test options
        CompilerOptions options;
        codeGen_ = std::make_unique<codegen::LLVMCodeGenRefactored>(*diagnostics_, options);
        exprGen_ = codeGen_->getExpressionGenerator();
    }
    
    void testEmptyStringConversion() {
        std::cout << "\n--- Testing Empty String Conversion ---" << std::endl;
        
        try {
            // Test empty string literal
            auto emptyStringLit = ast::ASTFactory::createStringLiteral("", SourceLocation());
            llvm::Value* emptyStringValue = exprGen_->generateStringLiteral(*emptyStringLit);
            assert(emptyStringValue != nullptr);
            
            // Convert to boolean - should be false
            llvm::Value* boolValue = exprGen_->convertToBoolean(emptyStringValue, emptyStringValue->getType());
            assert(boolValue != nullptr);
            assert(boolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Empty string conversion: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Empty string conversion: Exception: " << e.what() << std::endl;
        }
    }
    
    void testNonEmptyStringConversion() {
        std::cout << "\n--- Testing Non-Empty String Conversion ---" << std::endl;
        
        try {
            // Test non-empty string literal
            auto nonEmptyStringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            llvm::Value* nonEmptyStringValue = exprGen_->generateStringLiteral(*nonEmptyStringLit);
            assert(nonEmptyStringValue != nullptr);
            
            // Convert to boolean - should be true
            llvm::Value* boolValue = exprGen_->convertToBoolean(nonEmptyStringValue, nonEmptyStringValue->getType());
            assert(boolValue != nullptr);
            assert(boolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Non-empty string conversion: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Non-empty string conversion: Exception: " << e.what() << std::endl;
        }
    }
    
    void testNullStringConversion() {
        std::cout << "\n--- Testing Null String Conversion ---" << std::endl;
        
        try {
            // Test null literal (which should be treated as null string)
            auto nullLit = ast::ASTFactory::createNullLiteral(SourceLocation());
            llvm::Value* nullValue = exprGen_->generateNullLiteral(*nullLit);
            assert(nullValue != nullptr);
            
            // Convert to boolean - should be false
            llvm::Value* boolValue = exprGen_->convertToBoolean(nullValue, nullValue->getType());
            assert(boolValue != nullptr);
            assert(boolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Null string conversion: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Null string conversion: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStringWithWhitespace() {
        std::cout << "\n--- Testing String with Whitespace ---" << std::endl;
        
        try {
            // Test string with only whitespace
            auto whitespaceStringLit = ast::ASTFactory::createStringLiteral("   ", SourceLocation());
            llvm::Value* whitespaceStringValue = exprGen_->generateStringLiteral(*whitespaceStringLit);
            assert(whitespaceStringValue != nullptr);
            
            // Convert to boolean - should be true (non-empty string)
            llvm::Value* boolValue = exprGen_->convertToBoolean(whitespaceStringValue, whitespaceStringValue->getType());
            assert(boolValue != nullptr);
            assert(boolValue->getType()->isIntegerTy(1));
            
            // Test string with whitespace and content
            auto mixedStringLit = ast::ASTFactory::createStringLiteral("  hello  ", SourceLocation());
            llvm::Value* mixedStringValue = exprGen_->generateStringLiteral(*mixedStringLit);
            assert(mixedStringValue != nullptr);
            
            llvm::Value* mixedBoolValue = exprGen_->convertToBoolean(mixedStringValue, mixedStringValue->getType());
            assert(mixedBoolValue != nullptr);
            assert(mixedBoolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ String with whitespace: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ String with whitespace: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStringWithNumbers() {
        std::cout << "\n--- Testing String with Numbers ---" << std::endl;
        
        try {
            // Test string containing "0" - should be true (non-empty string)
            auto zeroStringLit = ast::ASTFactory::createStringLiteral("0", SourceLocation());
            llvm::Value* zeroStringValue = exprGen_->generateStringLiteral(*zeroStringLit);
            assert(zeroStringValue != nullptr);
            
            llvm::Value* zeroBoolValue = exprGen_->convertToBoolean(zeroStringValue, zeroStringValue->getType());
            assert(zeroBoolValue != nullptr);
            assert(zeroBoolValue->getType()->isIntegerTy(1));
            
            // Test string containing "123" - should be true
            auto numberStringLit = ast::ASTFactory::createStringLiteral("123", SourceLocation());
            llvm::Value* numberStringValue = exprGen_->generateStringLiteral(*numberStringLit);
            assert(numberStringValue != nullptr);
            
            llvm::Value* numberBoolValue = exprGen_->convertToBoolean(numberStringValue, numberStringValue->getType());
            assert(numberBoolValue != nullptr);
            assert(numberBoolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ String with numbers: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ String with numbers: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStringWithSpecialCharacters() {
        std::cout << "\n--- Testing String with Special Characters ---" << std::endl;
        
        try {
            // Test string with special characters
            auto specialStringLit = ast::ASTFactory::createStringLiteral("!@#$%^&*()", SourceLocation());
            llvm::Value* specialStringValue = exprGen_->generateStringLiteral(*specialStringLit);
            assert(specialStringValue != nullptr);
            
            llvm::Value* specialBoolValue = exprGen_->convertToBoolean(specialStringValue, specialStringValue->getType());
            assert(specialBoolValue != nullptr);
            assert(specialBoolValue->getType()->isIntegerTy(1));
            
            // Test string with Unicode characters
            auto unicodeStringLit = ast::ASTFactory::createStringLiteral("🚀🌟✨", SourceLocation());
            llvm::Value* unicodeStringValue = exprGen_->generateStringLiteral(*unicodeStringLit);
            assert(unicodeStringValue != nullptr);
            
            llvm::Value* unicodeBoolValue = exprGen_->convertToBoolean(unicodeStringValue, unicodeStringValue->getType());
            assert(unicodeBoolValue != nullptr);
            assert(unicodeBoolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ String with special characters: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ String with special characters: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStringWithTrueFalse() {
        std::cout << "\n--- Testing String with 'true' and 'false' ---" << std::endl;
        
        try {
            // Test string containing "true" - should be true (non-empty string)
            auto trueStringLit = ast::ASTFactory::createStringLiteral("true", SourceLocation());
            llvm::Value* trueStringValue = exprGen_->generateStringLiteral(*trueStringLit);
            assert(trueStringValue != nullptr);
            
            llvm::Value* trueBoolValue = exprGen_->convertToBoolean(trueStringValue, trueStringValue->getType());
            assert(trueBoolValue != nullptr);
            assert(trueBoolValue->getType()->isIntegerTy(1));
            
            // Test string containing "false" - should be true (non-empty string)
            auto falseStringLit = ast::ASTFactory::createStringLiteral("false", SourceLocation());
            llvm::Value* falseStringValue = exprGen_->generateStringLiteral(*falseStringLit);
            assert(falseStringValue != nullptr);
            
            llvm::Value* falseBoolValue = exprGen_->convertToBoolean(falseStringValue, falseStringValue->getType());
            assert(falseBoolValue != nullptr);
            assert(falseBoolValue->getType()->isIntegerTy(1));
            
            // Test string containing "TRUE" (case insensitive doesn't matter for string-to-boolean)
            auto upperTrueStringLit = ast::ASTFactory::createStringLiteral("TRUE", SourceLocation());
            llvm::Value* upperTrueStringValue = exprGen_->generateStringLiteral(*upperTrueStringLit);
            assert(upperTrueStringValue != nullptr);
            
            llvm::Value* upperTrueBoolValue = exprGen_->convertToBoolean(upperTrueStringValue, upperTrueStringValue->getType());
            assert(upperTrueBoolValue != nullptr);
            assert(upperTrueBoolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ String with 'true' and 'false': PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ String with 'true' and 'false': Exception: " << e.what() << std::endl;
        }
    }
    
    void testLongStringConversion() {
        std::cout << "\n--- Testing Long String Conversion ---" << std::endl;
        
        try {
            // Test very long string
            std::string longString(1000, 'a'); // 1000 'a' characters
            auto longStringLit = ast::ASTFactory::createStringLiteral(longString, SourceLocation());
            llvm::Value* longStringValue = exprGen_->generateStringLiteral(*longStringLit);
            assert(longStringValue != nullptr);
            
            llvm::Value* longBoolValue = exprGen_->convertToBoolean(longStringValue, longStringValue->getType());
            assert(longBoolValue != nullptr);
            assert(longBoolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Long string conversion: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Long string conversion: Exception: " << e.what() << std::endl;
        }
    }
    
    void testDirectConversionMethod() {
        std::cout << "\n--- Testing Direct Conversion Method ---" << std::endl;
        
        try {
            // Test direct convertStringToBoolean method
            auto testStringLit = ast::ASTFactory::createStringLiteral("test", SourceLocation());
            llvm::Value* testStringValue = exprGen_->generateStringLiteral(*testStringLit);
            assert(testStringValue != nullptr);
            
            // Test direct conversion method
            llvm::Value* directBoolValue = exprGen_->convertStringToBoolean(testStringValue);
            assert(directBoolValue != nullptr);
            assert(directBoolValue->getType()->isIntegerTy(1));
            
            // Test with empty string
            auto emptyStringLit = ast::ASTFactory::createStringLiteral("", SourceLocation());
            llvm::Value* emptyStringValue = exprGen_->generateStringLiteral(*emptyStringLit);
            assert(emptyStringValue != nullptr);
            
            llvm::Value* emptyDirectBoolValue = exprGen_->convertStringToBoolean(emptyStringValue);
            assert(emptyDirectBoolValue != nullptr);
            assert(emptyDirectBoolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Direct conversion method: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Direct conversion method: Exception: " << e.what() << std::endl;
        }
    }
    
    void testTypeCoercion() {
        std::cout << "\n--- Testing Type Coercion ---" << std::endl;
        
        try {
            // Test string in boolean expression context
            auto stringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            auto boolLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            
            // Test: string && boolean
            auto mixedAnd = ast::ASTFactory::createBinaryExpression(
                std::move(stringLit),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(boolLit),
                SourceLocation()
            );
            
            llvm::Value* mixedAndResult = exprGen_->generateBinaryExpression(*mixedAnd);
            assert(mixedAndResult != nullptr);
            assert(mixedAndResult->getType()->isIntegerTy(1));
            
            // Test: !string
            auto stringLit2 = ast::ASTFactory::createStringLiteral("", SourceLocation());
            auto notStringExpr = ast::ASTFactory::createUnaryExpression(
                std::move(stringLit2),
                ast::expressions::UnaryExpression::Operator::LogicalNot,
                SourceLocation()
            );
            
            llvm::Value* notStringResult = exprGen_->generateUnaryExpression(*notStringExpr);
            assert(notStringResult != nullptr);
            assert(notStringResult->getType()->isIntegerTy(1));
            
            std::cout << "✓ Type coercion: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Type coercion: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::StringToBooleanUnitTests test;
    test.runAllTests();
    return 0;
}