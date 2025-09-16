#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

// LLVM includes
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"

namespace tsc {

// Forward declarations
class CodeGenContext;

/**
 * Handles LLVM code generation for expressions
 */
class LLVMExpressionGen {
public:
    explicit LLVMExpressionGen(CodeGenContext& context);
    
    // Literal expressions
    llvm::Value* visit(NumericLiteral& node);
    llvm::Value* visit(StringLiteral& node);
    llvm::Value* visit(TemplateLiteral& node);
    llvm::Value* visit(BooleanLiteral& node);
    llvm::Value* visit(NullLiteral& node);
    
    // Identifier and property access
    llvm::Value* visit(Identifier& node);
    llvm::Value* visit(ThisExpression& node);
    llvm::Value* visit(SuperExpression& node);
    llvm::Value* visit(PropertyAccess& node);
    llvm::Value* visit(IndexExpression& node);
    
    // Object and array creation
    llvm::Value* visit(NewExpression& node);
    llvm::Value* visit(ObjectLiteral& node);
    llvm::Value* visit(ArrayLiteral& node);
    
    // Binary and unary expressions
    llvm::Value* visit(BinaryExpression& node);
    llvm::Value* visit(UnaryExpression& node);
    llvm::Value* visit(AssignmentExpression& node);
    
    // Function calls
    llvm::Value* visit(CallExpression& node);
    
    // Conditional expressions
    llvm::Value* visit(ConditionalExpression& node);
    
    // Type expressions

private:
    CodeGenContext& context_;
    
    // Helper methods
    llvm::Value* createStringLiteral(const String& value);
    llvm::Value* createNumberLiteral(double value);
    llvm::Value* createBooleanLiteral(bool value);
    llvm::Value* createNullLiteral();
    
    // Binary operation helpers
    llvm::Value* generateBinaryOperation(BinaryExpression& node, llvm::Value* left, llvm::Value* right);
    llvm::Value* generateArithmeticOperation(TokenType op, llvm::Value* left, llvm::Value* right);
    llvm::Value* generateComparisonOperation(TokenType op, llvm::Value* left, llvm::Value* right);
    llvm::Value* generateLogicalOperation(TokenType op, llvm::Value* left, llvm::Value* right);
    
    // Unary operation helpers
    llvm::Value* generateUnaryOperation(UnaryExpression& node, llvm::Value* operand);
    
    // Function call helpers
    llvm::Value* generateFunctionCall(const String& functionName, const std::vector<llvm::Value*>& args);
    llvm::Value* generateMethodCall(llvm::Value* object, const String& methodName, const std::vector<llvm::Value*>& args);
};

} // namespace tsc