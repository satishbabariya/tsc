#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

// LLVM includes
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

namespace tsc {
namespace codegen {

// Forward declarations
class LLVMCodeGen;

// Expression generation utilities for converting AST expressions to LLVM IR
class ExpressionGenerator {
public:
    explicit ExpressionGenerator(LLVMCodeGen* codeGen);
    
    // Literal expression generation
    llvm::Value* generateNumericLiteral(const NumericLiteral& node);
    llvm::Value* generateStringLiteral(const StringLiteral& node);
    llvm::Value* generateTemplateLiteral(const TemplateLiteral& node);
    llvm::Value* generateBooleanLiteral(const BooleanLiteral& node);
    llvm::Value* generateNullLiteral(const NullLiteral& node);
    
    // Identifier and property access
    llvm::Value* generateIdentifier(const Identifier& node);
    llvm::Value* generateThisExpression(const ThisExpression& node);
    llvm::Value* generateSuperExpression(const SuperExpression& node);
    llvm::Value* generatePropertyAccess(const PropertyAccess& node);
    llvm::Value* generateOptionalPropertyAccess(const OptionalPropertyAccess& node);
    llvm::Value* generateIndexExpression(const IndexExpression& node);
    llvm::Value* generateOptionalIndexAccess(const OptionalIndexAccess& node);
    
    // Object and array expressions
    llvm::Value* generateObjectLiteral(const ObjectLiteral& node);
    llvm::Value* generateArrayLiteral(const ArrayLiteral& node);
    llvm::Value* generateNewExpression(const NewExpression& node);
    
    // Binary and unary operations
    llvm::Value* generateBinaryExpression(const BinaryExpression& node);
    llvm::Value* generateUnaryExpression(const UnaryExpression& node);
    llvm::Value* generateAssignmentExpression(const AssignmentExpression& node);
    llvm::Value* generateConditionalExpression(const ConditionalExpression& node);
    
    // Function expressions
    llvm::Value* generateCallExpression(const CallExpression& node);
    llvm::Value* generateOptionalCallExpr(const OptionalCallExpr& node);
    llvm::Value* generateArrowFunction(const ArrowFunction& node);
    llvm::Value* generateFunctionExpression(const FunctionExpression& node);
    
    // Other expressions
    llvm::Value* generateMoveExpression(const MoveExpression& node);
    llvm::Value* generateSpreadElement(const SpreadElement& node);
    
    // Value operations
    llvm::Value* createNumberLiteral(double value);
    llvm::Value* createStringLiteral(const String& value);
    llvm::Value* createBooleanLiteral(bool value);
    llvm::Value* createNullValue(llvm::Type* type);
    llvm::Value* createDefaultValue(llvm::Type* type);
    
    // Type conversions
    llvm::Value* convertToNumber(llvm::Value* value, llvm::Type* fromType);
    llvm::Value* convertToString(llvm::Value* value, llvm::Type* fromType);
    llvm::Value* convertToBoolean(llvm::Value* value, llvm::Type* fromType);
    llvm::Value* convertValue(llvm::Value* value, llvm::Type* fromType, llvm::Type* toType);
    
    // Binary operations
    llvm::Value* generateBinaryOp(BinaryExpression::Operator op, llvm::Value* left, 
                                 llvm::Value* right, llvm::Type* leftType, llvm::Type* rightType);
    llvm::Value* generateArithmeticOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right);
    llvm::Value* generateComparisonOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right);
    llvm::Value* generateLogicalOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right);
    llvm::Value* generateStringConcat(llvm::Value* left, llvm::Value* right);
    
    // Unary operations
    llvm::Value* generateUnaryOp(int op, llvm::Value* operand, llvm::Type* operandType);
    
    // Additional helper methods
    llvm::Value* generateBitwiseOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right);
    llvm::Value* convertStringToBoolean(llvm::Value* stringValue);

private:
    LLVMCodeGen* codeGen_;
};

} // namespace codegen
} // namespace tsc