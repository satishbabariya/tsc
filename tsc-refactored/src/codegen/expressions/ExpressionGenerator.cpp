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
    switch (op) {
        // Arithmetic operations
        case BinaryExpression::Operator::Plus:
        case BinaryExpression::Operator::Minus:
        case BinaryExpression::Operator::Multiply:
        case BinaryExpression::Operator::Divide:
        case BinaryExpression::Operator::Modulo:
        case BinaryExpression::Operator::Exponentiation:
            return generateArithmeticOp(op, left, right);
            
        // Comparison operations
        case BinaryExpression::Operator::Equal:
        case BinaryExpression::Operator::NotEqual:
        case BinaryExpression::Operator::Less:
        case BinaryExpression::Operator::Greater:
        case BinaryExpression::Operator::LessEqual:
        case BinaryExpression::Operator::GreaterEqual:
            return generateComparisonOp(op, left, right);
            
        // Logical operations
        case BinaryExpression::Operator::LogicalAnd:
        case BinaryExpression::Operator::LogicalOr:
        case BinaryExpression::Operator::NullishCoalescing:
            return generateLogicalOp(op, left, right);
            
        // Bitwise operations
        case BinaryExpression::Operator::BitwiseAnd:
        case BinaryExpression::Operator::BitwiseOr:
        case BinaryExpression::Operator::BitwiseXor:
        case BinaryExpression::Operator::LeftShift:
        case BinaryExpression::Operator::RightShift:
        case BinaryExpression::Operator::UnsignedRightShift:
            return generateBitwiseOp(op, left, right);
            
        default:
            codeGen_->reportError("Unsupported binary operator", SourceLocation());
            return createNullValue(codeGen_->getTypeGenerator()->getAnyType());
    }
}

llvm::Value* ExpressionGenerator::generateArithmeticOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    // TODO: Implement arithmetic operation generation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateComparisonOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    llvm::IRBuilder<>* builder = codeGen_->getBuilder();
    llvm::Type* booleanType = codeGen_->getTypeGenerator()->getBooleanType();
    
    // Ensure both operands are of compatible types for comparison
    llvm::Type* leftType = left->getType();
    llvm::Type* rightType = right->getType();
    
    // Convert operands to compatible types if necessary
    if (leftType != rightType) {
        if (codeGen_->getTypeGenerator()->isNumericType(leftType) && 
            codeGen_->getTypeGenerator()->isNumericType(rightType)) {
            // Both are numeric - convert to the same type
            if (leftType->isDoubleTy() || rightType->isDoubleTy()) {
                if (!leftType->isDoubleTy()) {
                    left = builder->CreateSIToFP(left, llvm::Type::getDoubleTy(*codeGen_->getLLVMContext()), "to_double");
                }
                if (!rightType->isDoubleTy()) {
                    right = builder->CreateSIToFP(right, llvm::Type::getDoubleTy(*codeGen_->getLLVMContext()), "to_double");
                }
            }
        } else if (leftType->isPointerTy() && rightType->isPointerTy()) {
            // Both are pointers - compare as pointers
        } else {
            // For other type combinations, convert to any type and compare
            llvm::Type* anyType = codeGen_->getTypeGenerator()->getAnyType();
            left = codeGen_->getTypeGenerator()->convertValue(left, leftType, anyType);
            right = codeGen_->getTypeGenerator()->convertValue(right, rightType, anyType);
        }
    }
    
    llvm::Value* result = nullptr;
    
    switch (op) {
        case BinaryExpression::Operator::Equal:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFCmpOEQ(left, right, "eq");
            } else if (left->getType()->isIntegerTy()) {
                result = builder->CreateICmpEQ(left, right, "eq");
            } else if (left->getType()->isPointerTy()) {
                result = builder->CreateICmpEQ(left, right, "eq");
            } else {
                // For other types, use pointer comparison
                result = builder->CreateICmpEQ(left, right, "eq");
            }
            break;
            
        case BinaryExpression::Operator::NotEqual:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFCmpONE(left, right, "ne");
            } else if (left->getType()->isIntegerTy()) {
                result = builder->CreateICmpNE(left, right, "ne");
            } else if (left->getType()->isPointerTy()) {
                result = builder->CreateICmpNE(left, right, "ne");
            } else {
                result = builder->CreateICmpNE(left, right, "ne");
            }
            break;
            
        case BinaryExpression::Operator::Less:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFCmpOLT(left, right, "lt");
            } else if (left->getType()->isIntegerTy()) {
                result = builder->CreateICmpSLT(left, right, "lt");
            } else {
                codeGen_->reportError("Cannot compare non-numeric types with <", SourceLocation());
                return createNullValue(booleanType);
            }
            break;
            
        case BinaryExpression::Operator::Greater:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFCmpOGT(left, right, "gt");
            } else if (left->getType()->isIntegerTy()) {
                result = builder->CreateICmpSGT(left, right, "gt");
            } else {
                codeGen_->reportError("Cannot compare non-numeric types with >", SourceLocation());
                return createNullValue(booleanType);
            }
            break;
            
        case BinaryExpression::Operator::LessEqual:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFCmpOLE(left, right, "le");
            } else if (left->getType()->isIntegerTy()) {
                result = builder->CreateICmpSLE(left, right, "le");
            } else {
                codeGen_->reportError("Cannot compare non-numeric types with <=", SourceLocation());
                return createNullValue(booleanType);
            }
            break;
            
        case BinaryExpression::Operator::GreaterEqual:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFCmpOGE(left, right, "ge");
            } else if (left->getType()->isIntegerTy()) {
                result = builder->CreateICmpSGE(left, right, "ge");
            } else {
                codeGen_->reportError("Cannot compare non-numeric types with >=", SourceLocation());
                return createNullValue(booleanType);
            }
            break;
            
        default:
            codeGen_->reportError("Unsupported comparison operator", SourceLocation());
            return createNullValue(booleanType);
    }
    
    return result;
}

llvm::Value* ExpressionGenerator::generateLogicalOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    llvm::IRBuilder<>* builder = codeGen_->getBuilder();
    llvm::Function* currentFunc = codeGen_->getCodeGenContext()->getCurrentFunction();
    
    if (!currentFunc) {
        codeGen_->reportError("Logical operation outside function context", SourceLocation());
        return createNullValue(codeGen_->getTypeGenerator()->getBooleanType());
    }
    
    llvm::Type* booleanType = codeGen_->getTypeGenerator()->getBooleanType();
    
    // Convert operands to boolean if they aren't already
    llvm::Value* leftBool = convertToBoolean(left);
    llvm::Value* rightBool = convertToBoolean(right);
    
    switch (op) {
        case BinaryExpression::Operator::LogicalAnd: {
            // Short-circuit AND: if left is false, return false; otherwise return right
            llvm::BasicBlock* shortCircuitBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "and.short", currentFunc);
            llvm::BasicBlock* evaluateRightBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "and.right", currentFunc);
            llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "and.merge", currentFunc);
            
            // Branch based on left operand
            builder->CreateCondBr(leftBool, evaluateRightBlock, shortCircuitBlock);
            
            // Short-circuit: left is false, return false
            builder->SetInsertPoint(shortCircuitBlock);
            builder->CreateBr(mergeBlock);
            
            // Evaluate right operand
            builder->SetInsertPoint(evaluateRightBlock);
            builder->CreateBr(mergeBlock);
            
            // Merge: create PHI node
            builder->SetInsertPoint(mergeBlock);
            llvm::PHINode* phi = builder->CreatePHI(booleanType, 2, "and.result");
            phi->addIncoming(createBooleanLiteral(false), shortCircuitBlock);
            phi->addIncoming(rightBool, evaluateRightBlock);
            
            return phi;
        }
        
        case BinaryExpression::Operator::LogicalOr: {
            // Short-circuit OR: if left is true, return true; otherwise return right
            llvm::BasicBlock* shortCircuitBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "or.short", currentFunc);
            llvm::BasicBlock* evaluateRightBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "or.right", currentFunc);
            llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "or.merge", currentFunc);
            
            // Branch based on left operand
            builder->CreateCondBr(leftBool, shortCircuitBlock, evaluateRightBlock);
            
            // Short-circuit: left is true, return true
            builder->SetInsertPoint(shortCircuitBlock);
            builder->CreateBr(mergeBlock);
            
            // Evaluate right operand
            builder->SetInsertPoint(evaluateRightBlock);
            builder->CreateBr(mergeBlock);
            
            // Merge: create PHI node
            builder->SetInsertPoint(mergeBlock);
            llvm::PHINode* phi = builder->CreatePHI(booleanType, 2, "or.result");
            phi->addIncoming(createBooleanLiteral(true), shortCircuitBlock);
            phi->addIncoming(rightBool, evaluateRightBlock);
            
            return phi;
        }
        
        case BinaryExpression::Operator::NullishCoalescing: {
            // Nullish coalescing: if left is null or undefined, return right; otherwise return left
            llvm::BasicBlock* useRightBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "nullish.right", currentFunc);
            llvm::BasicBlock* useLeftBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "nullish.left", currentFunc);
            llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "nullish.merge", currentFunc);
            
            // Check if left is null or undefined
            llvm::Value* isNull = builder->CreateICmpEQ(left, llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(left->getType())), "is_null");
            builder->CreateCondBr(isNull, useRightBlock, useLeftBlock);
            
            // Use right operand
            builder->SetInsertPoint(useRightBlock);
            builder->CreateBr(mergeBlock);
            
            // Use left operand
            builder->SetInsertPoint(useLeftBlock);
            builder->CreateBr(mergeBlock);
            
            // Merge: create PHI node
            builder->SetInsertPoint(mergeBlock);
            llvm::PHINode* phi = builder->CreatePHI(left->getType(), 2, "nullish.result");
            phi->addIncoming(right, useRightBlock);
            phi->addIncoming(left, useLeftBlock);
            
            return phi;
        }
        
        default:
            codeGen_->reportError("Unsupported logical operator", SourceLocation());
            return createNullValue(booleanType);
    }
}

llvm::Value* ExpressionGenerator::generateStringConcat(llvm::Value* left, llvm::Value* right) {
    // TODO: Implement string concatenation
    return nullptr;
}

llvm::Value* ExpressionGenerator::generateUnaryOp(int op, llvm::Value* operand, llvm::Type* operandType) {
    llvm::IRBuilder<>* builder = codeGen_->getBuilder();
    
    switch (static_cast<UnaryExpression::Operator>(op)) {
        case UnaryExpression::Operator::UnaryPlus:
            // Unary plus: convert to number if not already
            if (operandType->isFloatingPointTy()) {
                return operand; // Already a number
            } else if (operandType->isIntegerTy()) {
                return builder->CreateSIToFP(operand, llvm::Type::getDoubleTy(*codeGen_->getLLVMContext()), "to_double");
            } else {
                // For other types, convert to any type and then to number
                llvm::Type* anyType = codeGen_->getTypeGenerator()->getAnyType();
                llvm::Value* anyValue = codeGen_->getTypeGenerator()->convertValue(operand, operandType, anyType);
                // TODO: Add runtime conversion from any to number
                return anyValue;
            }
            
        case UnaryExpression::Operator::UnaryMinus:
            // Unary minus: negate the value
            if (operandType->isFloatingPointTy()) {
                return builder->CreateFNeg(operand, "neg");
            } else if (operandType->isIntegerTy()) {
                return builder->CreateNeg(operand, "neg");
            } else {
                codeGen_->reportError("Cannot apply unary minus to non-numeric type", SourceLocation());
                return createNullValue(codeGen_->getTypeGenerator()->getNumberType());
            }
            
        case UnaryExpression::Operator::LogicalNot:
            // Logical NOT: invert boolean value
            llvm::Value* boolValue = convertToBoolean(operand);
            return builder->CreateNot(boolValue, "not");
            
        case UnaryExpression::Operator::BitwiseNot:
            // Bitwise NOT: invert all bits
            if (operandType->isIntegerTy()) {
                return builder->CreateNot(operand, "bitwise_not");
            } else {
                codeGen_->reportError("Cannot apply bitwise NOT to non-integer type", SourceLocation());
                return createNullValue(operandType);
            }
            
        default:
            codeGen_->reportError("Unsupported unary operator", SourceLocation());
            return createNullValue(operandType);
    }
}

llvm::Value* ExpressionGenerator::convertToBoolean(llvm::Value* value) {
    llvm::IRBuilder<>* builder = codeGen_->getBuilder();
    llvm::Type* valueType = value->getType();
    
    if (valueType->isIntegerTy(1)) {
        // Already a boolean
        return value;
    } else if (valueType->isFloatingPointTy()) {
        // Convert floating point to boolean: compare with 0.0
        llvm::Value* zero = llvm::ConstantFP::get(valueType, 0.0);
        return builder->CreateFCmpONE(value, zero, "to_bool");
    } else if (valueType->isIntegerTy()) {
        // Convert integer to boolean: compare with 0
        llvm::Value* zero = llvm::ConstantInt::get(valueType, 0);
        return builder->CreateICmpNE(value, zero, "to_bool");
    } else if (valueType->isPointerTy()) {
        // Check if this is a string type
        if (codeGen_->getTypeGenerator()->isStringType(valueType)) {
            return convertStringToBoolean(value);
        } else {
            // Convert other pointer types to boolean: compare with null
            llvm::Value* nullPtr = llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(valueType));
            return builder->CreateICmpNE(value, nullPtr, "to_bool");
        }
    } else {
        // For other types, assume they can be converted to boolean
        // This is a simplified implementation
        return builder->CreateICmpNE(value, llvm::ConstantInt::get(valueType, 0), "to_bool");
    }
}

llvm::Value* ExpressionGenerator::generateBitwiseOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    llvm::IRBuilder<>* builder = codeGen_->getBuilder();
    
    // Ensure both operands are integers
    llvm::Type* leftType = left->getType();
    llvm::Type* rightType = right->getType();
    
    if (!leftType->isIntegerTy() || !rightType->isIntegerTy()) {
        codeGen_->reportError("Bitwise operations require integer operands", SourceLocation());
        return createNullValue(codeGen_->getTypeGenerator()->getNumberType());
    }
    
    // Convert to same type if necessary
    if (leftType != rightType) {
        if (leftType->getIntegerBitWidth() < rightType->getIntegerBitWidth()) {
            left = builder->CreateSExt(left, rightType, "extend");
        } else if (rightType->getIntegerBitWidth() < leftType->getIntegerBitWidth()) {
            right = builder->CreateSExt(right, leftType, "extend");
        }
    }
    
    llvm::Value* result = nullptr;
    
    switch (op) {
        case BinaryExpression::Operator::BitwiseAnd:
            result = builder->CreateAnd(left, right, "and");
            break;
            
        case BinaryExpression::Operator::BitwiseOr:
            result = builder->CreateOr(left, right, "or");
            break;
            
        case BinaryExpression::Operator::BitwiseXor:
            result = builder->CreateXor(left, right, "xor");
            break;
            
        case BinaryExpression::Operator::LeftShift:
            result = builder->CreateShl(left, right, "shl");
            break;
            
        case BinaryExpression::Operator::RightShift:
            result = builder->CreateAShr(left, right, "shr");
            break;
            
        case BinaryExpression::Operator::UnsignedRightShift:
            result = builder->CreateLShr(left, right, "lshr");
            break;
            
        default:
            codeGen_->reportError("Unsupported bitwise operator", SourceLocation());
            return createNullValue(leftType);
    }
    
    return result;
}

llvm::Value* ExpressionGenerator::generateArithmeticOp(BinaryExpression::Operator op, llvm::Value* left, llvm::Value* right) {
    llvm::IRBuilder<>* builder = codeGen_->getBuilder();
    
    // Ensure both operands are numeric
    llvm::Type* leftType = left->getType();
    llvm::Type* rightType = right->getType();
    
    if (!codeGen_->getTypeGenerator()->isNumericType(leftType) || 
        !codeGen_->getTypeGenerator()->isNumericType(rightType)) {
        codeGen_->reportError("Arithmetic operations require numeric operands", SourceLocation());
        return createNullValue(codeGen_->getTypeGenerator()->getNumberType());
    }
    
    // Convert to same type if necessary
    if (leftType != rightType) {
        if (leftType->isDoubleTy() || rightType->isDoubleTy()) {
            if (!leftType->isDoubleTy()) {
                left = builder->CreateSIToFP(left, llvm::Type::getDoubleTy(*codeGen_->getLLVMContext()), "to_double");
            }
            if (!rightType->isDoubleTy()) {
                right = builder->CreateSIToFP(right, llvm::Type::getDoubleTy(*codeGen_->getLLVMContext()), "to_double");
            }
        }
    }
    
    llvm::Value* result = nullptr;
    
    switch (op) {
        case BinaryExpression::Operator::Plus:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFAdd(left, right, "add");
            } else {
                result = builder->CreateAdd(left, right, "add");
            }
            break;
            
        case BinaryExpression::Operator::Minus:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFSub(left, right, "sub");
            } else {
                result = builder->CreateSub(left, right, "sub");
            }
            break;
            
        case BinaryExpression::Operator::Multiply:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFMul(left, right, "mul");
            } else {
                result = builder->CreateMul(left, right, "mul");
            }
            break;
            
        case BinaryExpression::Operator::Divide:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFDiv(left, right, "div");
            } else {
                result = builder->CreateSDiv(left, right, "div");
            }
            break;
            
        case BinaryExpression::Operator::Modulo:
            if (left->getType()->isFloatingPointTy()) {
                result = builder->CreateFRem(left, right, "mod");
            } else {
                result = builder->CreateSRem(left, right, "mod");
            }
            break;
            
        case BinaryExpression::Operator::Exponentiation:
            // Exponentiation is more complex - would need runtime function
            codeGen_->reportError("Exponentiation not yet implemented", SourceLocation());
            return createNullValue(left->getType());
            
        default:
            codeGen_->reportError("Unsupported arithmetic operator", SourceLocation());
            return createNullValue(left->getType());
    }
    
    return result;
}

llvm::Value* ExpressionGenerator::convertStringToBoolean(llvm::Value* stringValue) {
    llvm::IRBuilder<>* builder = codeGen_->getBuilder();
    llvm::Function* currentFunc = codeGen_->getCodeGenContext()->getCurrentFunction();
    
    if (!currentFunc) {
        codeGen_->reportError("String to boolean conversion outside function context", SourceLocation());
        return createBooleanLiteral(false);
    }
    
    // For C-style strings (i8*), we need to check if the string is empty
    // Empty string ("") should be false, non-empty strings should be true
    
    // First check if the string pointer is null
    llvm::Value* nullPtr = llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(stringValue->getType()));
    llvm::Value* isNull = builder->CreateICmpEQ(stringValue, nullPtr, "is_null");
    
    // Create basic blocks for the conversion logic
    llvm::BasicBlock* nullBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "string_null", currentFunc);
    llvm::BasicBlock* checkEmptyBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "string_check_empty", currentFunc);
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*codeGen_->getLLVMContext(), "string_merge", currentFunc);
    
    // Branch based on null check
    builder->CreateCondBr(isNull, nullBlock, checkEmptyBlock);
    
    // Handle null string: return false
    builder->SetInsertPoint(nullBlock);
    builder->CreateBr(mergeBlock);
    
    // Check if string is empty: load first character and compare with null terminator
    builder->SetInsertPoint(checkEmptyBlock);
    llvm::Value* firstChar = builder->CreateLoad(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()), stringValue, "first_char");
    llvm::Value* nullChar = llvm::ConstantInt::get(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()), 0);
    llvm::Value* isEmpty = builder->CreateICmpEQ(firstChar, nullChar, "is_empty");
    builder->CreateBr(mergeBlock);
    
    // Merge: create PHI node
    builder->SetInsertPoint(mergeBlock);
    llvm::PHINode* phi = builder->CreatePHI(codeGen_->getTypeGenerator()->getBooleanType(), 2, "string_to_bool");
    phi->addIncoming(createBooleanLiteral(false), nullBlock);  // null string is false
    phi->addIncoming(builder->CreateNot(isEmpty, "not_empty"), checkEmptyBlock);  // empty string is false, non-empty is true
    
    return phi;
}

} // namespace codegen
} // namespace tsc