#include "tsc/codegen/expressions/ExpressionGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/ast/ASTRefactored.h"

#include <iostream>
#include <memory>
#include <cassert>

namespace tsc {
namespace testing {

class BooleanExpressionUnitTests {
public:
    BooleanExpressionUnitTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== Boolean Expression Unit Tests ===" << std::endl;
        
        testBooleanLiterals();
        testComparisonOperators();
        testLogicalOperators();
        testUnaryLogicalNot();
        testTypeConversions();
        testShortCircuiting();
        testComplexBooleanExpressions();
        
        std::cout << "=== All Boolean Expression Unit Tests Completed ===" << std::endl;
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
    
    void testBooleanLiterals() {
        std::cout << "\n--- Testing Boolean Literals ---" << std::endl;
        
        try {
            // Test true literal
            auto trueLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            llvm::Value* trueValue = exprGen_->generateBooleanLiteral(*trueLit);
            assert(trueValue != nullptr);
            assert(trueValue->getType()->isIntegerTy(1));
            
            // Test false literal
            auto falseLit = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            llvm::Value* falseValue = exprGen_->generateBooleanLiteral(*falseLit);
            assert(falseValue != nullptr);
            assert(falseValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Boolean literals: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Boolean literals: Exception: " << e.what() << std::endl;
        }
    }
    
    void testComparisonOperators() {
        std::cout << "\n--- Testing Comparison Operators ---" << std::endl;
        
        try {
            // Test equality (==)
            auto left = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto eqExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left),
                ast::expressions::BinaryExpression::Operator::Equal,
                std::move(right),
                SourceLocation()
            );
            
            llvm::Value* eqResult = exprGen_->generateBinaryExpression(*eqExpr);
            assert(eqResult != nullptr);
            assert(eqResult->getType()->isIntegerTy(1)); // Should be boolean
            
            // Test inequality (!=)
            auto left2 = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right2 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto neExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left2),
                ast::expressions::BinaryExpression::Operator::NotEqual,
                std::move(right2),
                SourceLocation()
            );
            
            llvm::Value* neResult = exprGen_->generateBinaryExpression(*neExpr);
            assert(neResult != nullptr);
            assert(neResult->getType()->isIntegerTy(1));
            
            // Test less than (<)
            auto left3 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto right3 = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto ltExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left3),
                ast::expressions::BinaryExpression::Operator::Less,
                std::move(right3),
                SourceLocation()
            );
            
            llvm::Value* ltResult = exprGen_->generateBinaryExpression(*ltExpr);
            assert(ltResult != nullptr);
            assert(ltResult->getType()->isIntegerTy(1));
            
            // Test greater than (>)
            auto left4 = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right4 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto gtExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left4),
                ast::expressions::BinaryExpression::Operator::Greater,
                std::move(right4),
                SourceLocation()
            );
            
            llvm::Value* gtResult = exprGen_->generateBinaryExpression(*gtExpr);
            assert(gtResult != nullptr);
            assert(gtResult->getType()->isIntegerTy(1));
            
            // Test less than or equal (<=)
            auto left5 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto right5 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto leExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left5),
                ast::expressions::BinaryExpression::Operator::LessEqual,
                std::move(right5),
                SourceLocation()
            );
            
            llvm::Value* leResult = exprGen_->generateBinaryExpression(*leExpr);
            assert(leResult != nullptr);
            assert(leResult->getType()->isIntegerTy(1));
            
            // Test greater than or equal (>=)
            auto left6 = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right6 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto geExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left6),
                ast::expressions::BinaryExpression::Operator::GreaterEqual,
                std::move(right6),
                SourceLocation()
            );
            
            llvm::Value* geResult = exprGen_->generateBinaryExpression(*geExpr);
            assert(geResult != nullptr);
            assert(geResult->getType()->isIntegerTy(1));
            
            std::cout << "✓ Comparison operators: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Comparison operators: Exception: " << e.what() << std::endl;
        }
    }
    
    void testLogicalOperators() {
        std::cout << "\n--- Testing Logical Operators ---" << std::endl;
        
        try {
            // Test logical AND (&&)
            auto left = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto right = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            auto andExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(right),
                SourceLocation()
            );
            
            llvm::Value* andResult = exprGen_->generateBinaryExpression(*andExpr);
            assert(andResult != nullptr);
            assert(andResult->getType()->isIntegerTy(1));
            
            // Test logical OR (||)
            auto left2 = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            auto right2 = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto orExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left2),
                ast::expressions::BinaryExpression::Operator::LogicalOr,
                std::move(right2),
                SourceLocation()
            );
            
            llvm::Value* orResult = exprGen_->generateBinaryExpression(*orExpr);
            assert(orResult != nullptr);
            assert(orResult->getType()->isIntegerTy(1));
            
            // Test nullish coalescing (??)
            auto left3 = ast::ASTFactory::createNullLiteral(SourceLocation());
            auto right3 = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto nullishExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left3),
                ast::expressions::BinaryExpression::Operator::NullishCoalescing,
                std::move(right3),
                SourceLocation()
            );
            
            llvm::Value* nullishResult = exprGen_->generateBinaryExpression(*nullishExpr);
            assert(nullishResult != nullptr);
            
            std::cout << "✓ Logical operators: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Logical operators: Exception: " << e.what() << std::endl;
        }
    }
    
    void testUnaryLogicalNot() {
        std::cout << "\n--- Testing Unary Logical NOT ---" << std::endl;
        
        try {
            // Test logical NOT (!)
            auto operand = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto notExpr = ast::ASTFactory::createUnaryExpression(
                std::move(operand),
                ast::expressions::UnaryExpression::Operator::LogicalNot,
                SourceLocation()
            );
            
            llvm::Value* notResult = exprGen_->generateUnaryExpression(*notExpr);
            assert(notResult != nullptr);
            assert(notResult->getType()->isIntegerTy(1));
            
            // Test NOT on numeric value
            auto numericOperand = ast::ASTFactory::createNumericLiteral(0.0, SourceLocation());
            auto numericNotExpr = ast::ASTFactory::createUnaryExpression(
                std::move(numericOperand),
                ast::expressions::UnaryExpression::Operator::LogicalNot,
                SourceLocation()
            );
            
            llvm::Value* numericNotResult = exprGen_->generateUnaryExpression(*numericNotExpr);
            assert(numericNotResult != nullptr);
            assert(numericNotResult->getType()->isIntegerTy(1));
            
            std::cout << "✓ Unary logical NOT: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Unary logical NOT: Exception: " << e.what() << std::endl;
        }
    }
    
    void testTypeConversions() {
        std::cout << "\n--- Testing Type Conversions ---" << std::endl;
        
        try {
            // Test numeric to boolean conversion
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            llvm::Value* numericValue = exprGen_->generateNumericLiteral(*numericLit);
            llvm::Value* boolValue = exprGen_->convertToBoolean(numericValue, numericValue->getType());
            assert(boolValue != nullptr);
            assert(boolValue->getType()->isIntegerTy(1));
            
            // Test string to boolean conversion
            auto stringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            llvm::Value* stringValue = exprGen_->generateStringLiteral(*stringLit);
            llvm::Value* stringBoolValue = exprGen_->convertToBoolean(stringValue, stringValue->getType());
            assert(stringBoolValue != nullptr);
            assert(stringBoolValue->getType()->isIntegerTy(1));
            
            // Test null to boolean conversion
            auto nullLit = ast::ASTFactory::createNullLiteral(SourceLocation());
            llvm::Value* nullValue = exprGen_->generateNullLiteral(*nullLit);
            llvm::Value* nullBoolValue = exprGen_->convertToBoolean(nullValue, nullValue->getType());
            assert(nullBoolValue != nullptr);
            assert(nullBoolValue->getType()->isIntegerTy(1));
            
            std::cout << "✓ Type conversions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Type conversions: Exception: " << e.what() << std::endl;
        }
    }
    
    void testShortCircuiting() {
        std::cout << "\n--- Testing Short-Circuiting ---" << std::endl;
        
        try {
            // Test short-circuit AND with false left operand
            auto falseLit = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            auto trueLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto shortAndExpr = ast::ASTFactory::createBinaryExpression(
                std::move(falseLit),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(trueLit),
                SourceLocation()
            );
            
            llvm::Value* shortAndResult = exprGen_->generateBinaryExpression(*shortAndExpr);
            assert(shortAndResult != nullptr);
            assert(shortAndResult->getType()->isIntegerTy(1));
            
            // Test short-circuit OR with true left operand
            auto trueLit2 = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto falseLit2 = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            auto shortOrExpr = ast::ASTFactory::createBinaryExpression(
                std::move(trueLit2),
                ast::expressions::BinaryExpression::Operator::LogicalOr,
                std::move(falseLit2),
                SourceLocation()
            );
            
            llvm::Value* shortOrResult = exprGen_->generateBinaryExpression(*shortOrExpr);
            assert(shortOrResult != nullptr);
            assert(shortOrResult->getType()->isIntegerTy(1));
            
            std::cout << "✓ Short-circuiting: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Short-circuiting: Exception: " << e.what() << std::endl;
        }
    }
    
    void testComplexBooleanExpressions() {
        std::cout << "\n--- Testing Complex Boolean Expressions ---" << std::endl;
        
        try {
            // Test nested logical operations: (true && false) || (true && true)
            auto trueLit1 = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto falseLit1 = ast::ASTFactory::createBooleanLiteral(false, SourceLocation());
            auto innerAnd1 = ast::ASTFactory::createBinaryExpression(
                std::move(trueLit1),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(falseLit1),
                SourceLocation()
            );
            
            auto trueLit2 = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto trueLit3 = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto innerAnd2 = ast::ASTFactory::createBinaryExpression(
                std::move(trueLit2),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(trueLit3),
                SourceLocation()
            );
            
            auto outerOr = ast::ASTFactory::createBinaryExpression(
                std::move(innerAnd1),
                ast::expressions::BinaryExpression::Operator::LogicalOr,
                std::move(innerAnd2),
                SourceLocation()
            );
            
            llvm::Value* complexResult = exprGen_->generateBinaryExpression(*outerOr);
            assert(complexResult != nullptr);
            assert(complexResult->getType()->isIntegerTy(1));
            
            // Test comparison with logical operations: (5 < 10) && (10 > 5)
            auto left1 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto right1 = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto comp1 = ast::ASTFactory::createBinaryExpression(
                std::move(left1),
                ast::expressions::BinaryExpression::Operator::Less,
                std::move(right1),
                SourceLocation()
            );
            
            auto left2 = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right2 = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto comp2 = ast::ASTFactory::createBinaryExpression(
                std::move(left2),
                ast::expressions::BinaryExpression::Operator::Greater,
                std::move(right2),
                SourceLocation()
            );
            
            auto complexAnd = ast::ASTFactory::createBinaryExpression(
                std::move(comp1),
                ast::expressions::BinaryExpression::Operator::LogicalAnd,
                std::move(comp2),
                SourceLocation()
            );
            
            llvm::Value* complexCompResult = exprGen_->generateBinaryExpression(*complexAnd);
            assert(complexCompResult != nullptr);
            assert(complexCompResult->getType()->isIntegerTy(1));
            
            std::cout << "✓ Complex boolean expressions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Complex boolean expressions: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::BooleanExpressionUnitTests test;
    test.runAllTests();
    return 0;
}