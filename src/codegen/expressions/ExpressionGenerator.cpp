#include "tsc/codegen/expressions/ExpressionGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"

namespace tsc {
namespace codegen {

ExpressionGenerator::ExpressionGenerator(LLVMCodeGen* codeGen) 
    : codeGen_(codeGen) {
}

llvm::Value* ExpressionGenerator::generateNumericLiteral(const NumericLiteral& node) {
    return createNumberLiteral(node.getValue());
}

llvm::Value* ExpressionGenerator::generateStringLiteral(const StringLiteral& node) {
    return createStringLiteral(node.getValue());
}

llvm::Value* ExpressionGenerator::generateTemplateLiteral(const TemplateLiteral& node) {
    std::cout << "DEBUG: TemplateLiteral visitor called" << std::endl;
    // Build the template literal by concatenating all parts
    llvm::Value *result = nullptr;

    for (const auto &element: node.getElements()) {
        llvm::Value *elementValue = nullptr;

        if (element.isExpression()) {
            // Generate code for the expression
            std::cout << "DEBUG: Processing template expression" << std::endl;
            if (element.getExpression()) {
                elementValue = codeGen_->generateExpression(*element.getExpression());
                std::cout << "DEBUG: Template expression result type: " << (elementValue
                                                                                ? elementValue->getType()->getTypeID()
                                                                                : -1) << std::endl;

                // Convert expression results to string representation
                std::cout << "DEBUG: Converting template expression to string, type: " << elementValue->getType()->
                        getTypeID() << std::endl;
                if (elementValue->getType()->isDoubleTy()) {
                    // Convert number to string using runtime function
                    std::cout << "DEBUG: Converting double to string" << std::endl;
                    llvm::Function *numberToStringFunc = codeGen_->getOrCreateNumberToStringFunction();
                    std::cout << "DEBUG: numberToStringFunc type: " << numberToStringFunc->getReturnType()->getTypeID()
                            << std::endl;
                    elementValue = codeGen_->getBuilder()->CreateCall(numberToStringFunc, {elementValue}, "number_to_string");
                    std::cout << "DEBUG: After CreateCall, elementValue type: " << elementValue->getType()->getTypeID()
                            << std::endl;
                } else if (elementValue->getType()->isIntegerTy(1)) {
                    // Convert boolean to string using runtime function
                    std::cout << "DEBUG: Converting boolean to string" << std::endl;
                    llvm::Function *booleanToStringFunc = codeGen_->getOrCreateBooleanToStringFunction();
                    elementValue = codeGen_->getBuilder()->CreateCall(booleanToStringFunc, {elementValue}, "boolean_to_string");
                } else if (elementValue->getType() != codeGen_->getTypeGenerator()->getStringType()) {
                    // For other types, convert to string representation using runtime function
                    std::cout << "DEBUG: Converting other type to string" << std::endl;
                    llvm::Function *objectToStringFunc = codeGen_->getOrCreateObjectToStringFunction();
                    elementValue = codeGen_->getBuilder()->CreateCall(objectToStringFunc, {elementValue}, "object_to_string");
                }
                std::cout << "DEBUG: After conversion, type: " << elementValue->getType()->getTypeID() << std::endl;
            }
        } else {
            // Text element
            elementValue = createStringLiteral(element.getText());
        }

        // Concatenate with previous result
        if (result == nullptr) {
            result = elementValue;
        } else {
            llvm::Function *concatFunc = codeGen_->getOrCreateStringConcatFunction();
            result = codeGen_->getBuilder()->CreateCall(concatFunc, {result, elementValue}, "template_concat");
        }
    }

    // If no elements, return empty string
    if (result == nullptr) {
        result = createStringLiteral("");
    }

    std::cout << "DEBUG: TemplateLiteral result type: " << (result ? result->getType()->getTypeID() : -1) <<
            std::endl;
    return result;
}

llvm::Value* ExpressionGenerator::generateBooleanLiteral(const BooleanLiteral& node) {
    return createBooleanLiteral(node.getValue());
}

llvm::Value* ExpressionGenerator::generateNullLiteral(const NullLiteral& node) {
    return createNullValue(codeGen_->getTypeGenerator()->getAnyType());
}

llvm::Value* ExpressionGenerator::generateIdentifier(const Identifier& node) {
    std::cout << "DEBUG: Identifier visitor called for: " << node.getName() << std::endl;

    // First, check if this identifier refers to a function
    llvm::Function *function = codeGen_->getModule()->getFunction(node.getName());
    if (function) {
        // This is a function identifier - return the function as a value
        // In LLVM, functions can be treated as values (function pointers)
        return function;
    }

    // Check if this is a captured variable in a closure
    llvm::Value *capturedValue = codeGen_->getCodeGenContext()->getSymbolValue("__closure_env_" + node.getName());
    if (capturedValue) {
        // Use the captured value directly (it's already loaded from the closure environment)
        std::cout << "DEBUG: Using captured variable " << node.getName() << " from closure environment" <<
                std::endl;
        return capturedValue;
    }

    // Check if this is a built-in object like console
    if (node.getName() == "console") {
        // Return a placeholder value for console object
        // The actual methods will be handled in PropertyAccess
        return createNullValue(codeGen_->getTypeGenerator()->getAnyType());
    }

    // First, try to load as a regular variable from code generation context
    llvm::Value *value = codeGen_->loadVariable(node.getName(), node.getLocation());
    if (value) {
        return value;
    }

    // Special handling for built-in functions like _print
    if (node.getName() == "_print") {
        std::cout << "DEBUG: Identifier - Found _print, creating function" << std::endl;
        llvm::Function *printFunc = codeGen_->getOrCreatePrintFunction();
        return printFunc;
    }

    // If not found anywhere, report error
    codeGen_->reportError("Undefined variable: " + node.getName(), node.getLocation());
    return createNullValue(codeGen_->getTypeGenerator()->getAnyType());
}

llvm::Value* ExpressionGenerator::generateThisExpression(const ThisExpression& node) {
    // TODO: Implement this expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateSuperExpression(const SuperExpression& node) {
    // TODO: Implement super expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generatePropertyAccess(const PropertyAccess& node) {
    // TODO: Implement property access generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateOptionalPropertyAccess(const OptionalPropertyAccess& node) {
    // TODO: Implement optional property access generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateIndexExpression(const IndexExpression& node) {
    // TODO: Implement index expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateOptionalIndexAccess(const OptionalIndexAccess& node) {
    // TODO: Implement optional index access generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateObjectLiteral(const ObjectLiteral& node) {
    // TODO: Implement object literal generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateArrayLiteral(const ArrayLiteral& node) {
    // TODO: Implement array literal generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateNewExpression(const NewExpression& node) {
    // TODO: Implement new expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateBinaryExpression(const BinaryExpression& node) {
    // Generate left and right operands
    llvm::Value *left = codeGen_->generateExpression(*node.getLeft());
    llvm::Value *right = codeGen_->generateExpression(*node.getRight());

    if (!left || !right) {
        return createNullValue(codeGen_->getTypeGenerator()->getAnyType());
    }

    // Generate binary operation
    llvm::Value *result = generateBinaryOp(node.getOperator(), left, right,
                                          left->getType(), right->getType());
    return result;
}

llvm::Value* ExpressionGenerator::generateUnaryExpression(const UnaryExpression& node) {
    // Generate operand
    llvm::Value *operand = codeGen_->generateExpression(*node.getOperand());

    if (!operand) {
        return createNullValue(codeGen_->getTypeGenerator()->getAnyType());
    }

    // Generate unary operation
    llvm::Value *result = generateUnaryOp(static_cast<int>(node.getOperator()), operand, operand->getType());
    return result;
}

llvm::Value* ExpressionGenerator::generateAssignmentExpression(const AssignmentExpression& node) {
    // TODO: Implement assignment expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateConditionalExpression(const ConditionalExpression& node) {
    // TODO: Implement conditional expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateCallExpression(const CallExpression& node) {
    // TODO: Implement call expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateOptionalCallExpr(const OptionalCallExpr& node) {
    // TODO: Implement optional call expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateArrowFunction(const ArrowFunction& node) {
    // TODO: Implement arrow function generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateFunctionExpression(const FunctionExpression& node) {
    // TODO: Implement function expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateMoveExpression(const MoveExpression& node) {
    // TODO: Implement move expression generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateSpreadElement(const SpreadElement& node) {
    // TODO: Implement spread element generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::createNumberLiteral(double value) {
    return llvm::ConstantFP::get(codeGen_->getLLVMContext()->getDoubleTy(), value);
}

llvm::Value* ExpressionGenerator::createStringLiteral(const String& value) {
    return codeGen_->getBuilder()->CreateGlobalStringPtr(value);
}

llvm::Value* ExpressionGenerator::createBooleanLiteral(bool value) {
    return llvm::ConstantInt::get(codeGen_->getLLVMContext()->getInt1Ty(), value ? 1 : 0);
}

llvm::Value* ExpressionGenerator::createNullValue(llvm::Type* type) {
    return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type));
}

llvm::Value* ExpressionGenerator::createDefaultValue(llvm::Type* type) {
    if (type->isIntegerTy()) {
        return llvm::ConstantInt::get(type, 0);
    } else if (type->isFloatingPointTy()) {
        return llvm::ConstantFP::get(type, 0.0);
    } else if (type->isPointerTy()) {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type));
    }
    return llvm::UndefValue::get(type);
}

llvm::Value* ExpressionGenerator::convertToNumber(llvm::Value* value, llvm::Type* fromType) {
    // TODO: Implement number conversion
    return value;
}

llvm::Value* ExpressionGenerator::convertToString(llvm::Value* value, llvm::Type* fromType) {
    // TODO: Implement string conversion
    return value;
}

llvm::Value* ExpressionGenerator::convertToBoolean(llvm::Value* value, llvm::Type* fromType) {
    // TODO: Implement boolean conversion
    return value;
}

llvm::Value* ExpressionGenerator::convertValue(llvm::Value* value, llvm::Type* fromType, llvm::Type* toType) {
    // TODO: Implement value conversion
    return value;
}

llvm::Value* ExpressionGenerator::generateBinaryOp(BinaryExpression::Operator op, llvm::Value* left, 
                                                  llvm::Value* right, llvm::Type* leftType, llvm::Type* rightType) {
    // TODO: Implement binary operation generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateArithmeticOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    // TODO: Implement arithmetic operation generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateComparisonOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    // TODO: Implement comparison operation generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateLogicalOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    // TODO: Implement logical operation generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateStringConcat(llvm::Value* left, llvm::Value* right) {
    // TODO: Implement string concatenation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateUnaryOp(int op, llvm::Value* operand, llvm::Type* operandType) {
    // TODO: Implement unary operation generation
    return nullptr;
}

} // namespace codegen
} // namespace tsc