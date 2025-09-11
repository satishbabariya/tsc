#include "tsc/codegen/EnhancedIRGen.h"
#include "tsc/Compiler.h"
#include "tsc/TargetRegistry.h"

// LLVM includes
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils.h>

#include <iostream>
#include <sstream>

namespace tsc {

// EnhancedIRGen implementation
EnhancedIRGen::EnhancedIRGen(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem)
    : diagnostics_(diagnostics), typeSystem_(typeSystem) {
    
    // Initialize LLVM
    auto& registry = TargetRegistry::getInstance();
    registry.initializeAllTargets();
    
    // Create LLVM context and module
    context_ = std::make_unique<llvm::LLVMContext>();
    module_ = std::make_unique<llvm::Module>("tsc_enhanced_module", *context_);
    builder_ = std::make_unique<llvm::IRBuilder<>>(*context_);
    
    // Initialize exception context
    exceptionContext_ = std::make_unique<ExceptionContext>(this);
    
    // Declare built-in functions
    declareBuiltinFunctions();
}

bool EnhancedIRGen::generateCode(const std::vector<unique_ptr<Statement>>& statements) {
    try {
        // Create main function
        auto mainFuncType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(*context_), // Return type
            {}, // No parameters
            false // Not variadic
        );
        
        auto mainFunc = llvm::Function::Create(
            mainFuncType,
            llvm::Function::ExternalLinkage,
            "main",
            module_.get()
        );
        
        // Create entry block
        auto entryBlock = llvm::BasicBlock::Create(*context_, "entry", mainFunc);
        builder_->SetInsertPoint(entryBlock);
        
        // Enter function context
        FunctionContext funcContext(this, mainFunc);
        
        // Generate code for each statement
        for (const auto& stmt : statements) {
            generateStatement(stmt);
        }
        
        // Return 0
        builder_->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0));
        
        // Optimize the module
        optimizeMemoryUsage();
        
        return true;
    } catch (const std::exception& e) {
        reportError("Code generation failed: " + String(e.what()), SourceLocation());
        return false;
    }
}

bool EnhancedIRGen::generateCode(const unique_ptr<Module>& module) {
    try {
        generateModule(module);
        optimizeMemoryUsage();
        return true;
    } catch (const std::exception& e) {
        reportError("Module code generation failed: " + String(e.what()), SourceLocation());
        return false;
    }
}

void EnhancedIRGen::generateModule(const unique_ptr<Module>& module) {
    // Generate all declarations first
    for (const auto& decl : module->getDeclarations()) {
        generateDeclaration(decl);
    }
    
    // Generate all statements
    for (const auto& stmt : module->getStatements()) {
        generateStatement(stmt);
    }
}

void EnhancedIRGen::generateStatement(const unique_ptr<Statement>& statement) {
    if (!statement) return;
    
    switch (statement->getKind()) {
        case ASTNode::Kind::BlockStatement:
            generateBlockStatement(static_cast<BlockStatement*>(statement.get()));
            break;
        case ASTNode::Kind::IfStatement:
            generateIfStatement(static_cast<IfStatement*>(statement.get()));
            break;
        case ASTNode::Kind::WhileStatement:
            generateWhileStatement(static_cast<WhileStatement*>(statement.get()));
            break;
        case ASTNode::Kind::ForStatement:
            generateForStatement(static_cast<ForStatement*>(statement.get()));
            break;
        case ASTNode::Kind::ReturnStatement:
            generateReturnStatement(static_cast<ReturnStatement*>(statement.get()));
            break;
        case ASTNode::Kind::VariableDeclaration:
            generateVariableDeclaration(static_cast<VariableDeclaration*>(statement.get()));
            break;
        case ASTNode::Kind::ExpressionStatement: {
            auto exprStmt = static_cast<ExpressionStatement*>(statement.get());
            if (exprStmt->getExpression()) {
                generateExpression(exprStmt->getExpression());
            }
            break;
        }
        default:
            reportError("Unsupported statement type in code generation", statement->getLocation());
            break;
    }
}

void EnhancedIRGen::generateExpression(const unique_ptr<Expression>& expression) {
    if (!expression) return;
    
    switch (expression->getKind()) {
        case ASTNode::Kind::BinaryExpression:
            currentValue_ = generateBinaryExpression(static_cast<BinaryExpression*>(expression.get()));
            break;
        case ASTNode::Kind::UnaryExpression:
            currentValue_ = generateUnaryExpression(static_cast<UnaryExpression*>(expression.get()));
            break;
        case ASTNode::Kind::CallExpression:
            currentValue_ = generateCallExpression(static_cast<CallExpression*>(expression.get()));
            break;
        case ASTNode::Kind::MemberAccessExpression:
            currentValue_ = generateMemberAccess(static_cast<MemberAccessExpression*>(expression.get()));
            break;
        case ASTNode::Kind::NewExpression:
            currentValue_ = generateNewExpression(static_cast<NewExpression*>(expression.get()));
            break;
        case ASTNode::Kind::ConditionalExpression:
            currentValue_ = generateConditionalExpression(static_cast<ConditionalExpression*>(expression.get()));
            break;
        case ASTNode::Kind::NumericLiteral:
            currentValue_ = generateNumericLiteral(static_cast<NumericLiteral*>(expression.get()));
            break;
        case ASTNode::Kind::StringLiteral:
            currentValue_ = generateStringLiteral(static_cast<StringLiteral*>(expression.get()));
            break;
        case ASTNode::Kind::BooleanLiteral:
            currentValue_ = generateBooleanLiteral(static_cast<BooleanLiteral*>(expression.get()));
            break;
        case ASTNode::Kind::NullLiteral:
            currentValue_ = generateNullLiteral(static_cast<NullLiteral*>(expression.get()));
            break;
        case ASTNode::Kind::ArrayLiteral:
            currentValue_ = generateArrayLiteral(static_cast<ArrayLiteral*>(expression.get()));
            break;
        case ASTNode::Kind::ObjectLiteral:
            currentValue_ = generateObjectLiteral(static_cast<ObjectLiteral*>(expression.get()));
            break;
        case ASTNode::Kind::Identifier: {
            auto ident = static_cast<Identifier*>(expression.get());
            if (functionContexts_.empty()) {
                reportError("Cannot access identifier outside function context", ident->getLocation());
                return;
            }
            currentValue_ = functionContexts_.top().getScope().getSymbol(ident->getName());
            if (!currentValue_) {
                reportError("Undefined identifier: " + ident->getName(), ident->getLocation());
            }
            break;
        }
        default:
            reportError("Unsupported expression type in code generation", expression->getLocation());
            break;
    }
}

// Binary expression generation with proper type handling
llvm::Value* EnhancedIRGen::generateBinaryExpression(const unique_ptr<BinaryExpression>& expr) {
    auto left = generateExpression(expr->getLeft());
    auto right = generateExpression(expr->getRight());
    
    if (!left || !right) {
        return nullptr;
    }
    
    switch (expr->getOperator()) {
        case BinaryExpression::Operator::Add:
            return builder_->CreateAdd(left, right, "add");
        case BinaryExpression::Operator::Subtract:
            return builder_->CreateSub(left, right, "sub");
        case BinaryExpression::Operator::Multiply:
            return builder_->CreateMul(left, right, "mul");
        case BinaryExpression::Operator::Divide:
            return builder_->CreateSDiv(left, right, "div");
        case BinaryExpression::Operator::Equal:
            return builder_->CreateICmpEQ(left, right, "eq");
        case BinaryExpression::Operator::NotEqual:
            return builder_->CreateICmpNE(left, right, "ne");
        case BinaryExpression::Operator::Less:
            return builder_->CreateICmpSLT(left, right, "lt");
        case BinaryExpression::Operator::Greater:
            return builder_->CreateICmpSGT(left, right, "gt");
        case BinaryExpression::Operator::LessEqual:
            return builder_->CreateICmpSLE(left, right, "le");
        case BinaryExpression::Operator::GreaterEqual:
            return builder_->CreateICmpSGE(left, right, "ge");
        case BinaryExpression::Operator::LogicalAnd:
            return builder_->CreateAnd(left, right, "and");
        case BinaryExpression::Operator::LogicalOr:
            return builder_->CreateOr(left, right, "or");
        default:
            reportError("Unsupported binary operator", expr->getLocation());
            return nullptr;
    }
}

// Unary expression generation
llvm::Value* EnhancedIRGen::generateUnaryExpression(const unique_ptr<UnaryExpression>& expr) {
    auto operand = generateExpression(expr->getOperand());
    
    if (!operand) {
        return nullptr;
    }
    
    switch (expr->getOperator()) {
        case UnaryExpression::Operator::Plus:
            return operand; // No-op for plus
        case UnaryExpression::Operator::Minus:
            return builder_->CreateNeg(operand, "neg");
        case UnaryExpression::Operator::LogicalNot:
            return builder_->CreateNot(operand, "not");
        default:
            reportError("Unsupported unary operator", expr->getLocation());
            return nullptr;
    }
}

// Literal generation
llvm::Value* EnhancedIRGen::generateNumericLiteral(const unique_ptr<NumericLiteral>& literal) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), literal->getValue());
}

llvm::Value* EnhancedIRGen::generateStringLiteral(const unique_ptr<StringLiteral>& literal) {
    return builder_->CreateGlobalStringPtr(literal->getValue().c_str());
}

llvm::Value* EnhancedIRGen::generateBooleanLiteral(const unique_ptr<BooleanLiteral>& literal) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context_), literal->getValue() ? 1 : 0);
}

llvm::Value* EnhancedIRGen::generateNullLiteral(const unique_ptr<NullLiteral>& literal) {
    return llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(*context_));
}

// Block statement generation with scope management
void EnhancedIRGen::generateBlockStatement(const unique_ptr<BlockStatement>& stmt) {
    if (functionContexts_.empty()) {
        reportError("Cannot generate block statement outside function context", stmt->getLocation());
        return;
    }
    
    auto& scope = functionContexts_.top().getScope();
    scope.enterScope();
    
    for (const auto& statement : stmt->getStatements()) {
        generateStatement(statement);
    }
    
    scope.exitScope();
}

// If statement generation with proper control flow
void EnhancedIRGen::generateIfStatement(const unique_ptr<IfStatement>& stmt) {
    auto condition = generateExpression(stmt->getCondition());
    if (!condition) return;
    
    auto currentFunc = functionContexts_.top().getFunction();
    auto thenBlock = llvm::BasicBlock::Create(*context_, "if.then", currentFunc);
    auto elseBlock = llvm::BasicBlock::Create(*context_, "if.else", currentFunc);
    auto mergeBlock = llvm::BasicBlock::Create(*context_, "if.merge", currentFunc);
    
    // Branch based on condition
    builder_->CreateCondBr(condition, thenBlock, elseBlock);
    
    // Generate then block
    builder_->SetInsertPoint(thenBlock);
    generateStatement(stmt->getThenStatement());
    if (!builder_->GetInsertBlock()->getTerminator()) {
        builder_->CreateBr(mergeBlock);
    }
    
    // Generate else block if it exists
    if (stmt->getElseStatement()) {
        builder_->SetInsertPoint(elseBlock);
        generateStatement(stmt->getElseStatement());
        if (!builder_->GetInsertBlock()->getTerminator()) {
            builder_->CreateBr(mergeBlock);
        }
    } else {
        builder_->SetInsertPoint(elseBlock);
        builder_->CreateBr(mergeBlock);
    }
    
    // Continue from merge block
    builder_->SetInsertPoint(mergeBlock);
}

// Variable declaration generation
void EnhancedIRGen::generateVariableDeclaration(const unique_ptr<VariableDeclaration>& stmt) {
    if (functionContexts_.empty()) {
        reportError("Cannot declare variables outside function context", stmt->getLocation());
        return;
    }
    
    auto& scope = functionContexts_.top().getScope();
    
    for (const auto& declarator : stmt->getDeclarators()) {
        auto name = declarator.getName();
        llvm::Value* value = nullptr;
        
        if (declarator.getInitializer()) {
            generateExpression(declarator.getInitializer());
            value = currentValue_;
        } else {
            // Initialize with default value based on type
            if (declarator.getType()) {
                auto llvmType = generateType(declarator.getType());
                value = llvm::Constant::getNullValue(llvmType);
            } else {
                value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context_), 0);
            }
        }
        
        if (value) {
            // Create alloca for the variable
            auto alloca = createAlloca(value->getType(), name);
            builder_->CreateStore(value, alloca);
            scope.declareSymbol(name, alloca);
        }
    }
}

// Type generation
llvm::Type* EnhancedIRGen::generateType(const shared_ptr<Type>& type) {
    if (!type) {
        return llvm::Type::getInt32Ty(*context_); // Default to int32
    }
    
    // Check cache first
    auto typeName = type->toString();
    auto cached = typeCache_.find(typeName);
    if (cached != typeCache_.end()) {
        return cached->second;
    }
    
    llvm::Type* llvmType = nullptr;
    
    switch (type->getKind()) {
        case Type::Kind::Primitive:
            llvmType = generatePrimitiveType(type);
            break;
        case Type::Kind::Array:
            llvmType = generateArrayType(type);
            break;
        case Type::Kind::Object:
            llvmType = generateObjectType(type);
            break;
        case Type::Kind::Function:
            llvmType = generateFunctionType(type);
            break;
        case Type::Kind::Generic:
            llvmType = generateGenericType(type);
            break;
        default:
            llvmType = llvm::Type::getInt32Ty(*context_);
            break;
    }
    
    // Cache the type
    typeCache_[typeName] = llvmType;
    return llvmType;
}

llvm::Type* EnhancedIRGen::generatePrimitiveType(const shared_ptr<Type>& type) {
    auto primitiveType = std::static_pointer_cast<PrimitiveType>(type);
    
    switch (primitiveType->getPrimitiveKind()) {
        case PrimitiveType::PrimitiveKind::Number:
            return llvm::Type::getDoubleTy(*context_);
        case PrimitiveType::PrimitiveKind::String:
            return llvm::Type::getInt8PtrTy(*context_);
        case PrimitiveType::PrimitiveKind::Boolean:
            return llvm::Type::getInt1Ty(*context_);
        case PrimitiveType::PrimitiveKind::Void:
            return llvm::Type::getVoidTy(*context_);
        default:
            return llvm::Type::getInt32Ty(*context_);
    }
}

// Built-in function declarations
void EnhancedIRGen::declareBuiltinFunctions() {
    // Console.log
    auto consoleLogType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context_),
        {llvm::Type::getInt8PtrTy(*context_)},
        true // Variadic
    );
    builtinFunctions_["console.log"] = llvm::Function::Create(
        consoleLogType,
        llvm::Function::ExternalLinkage,
        "console_log",
        module_.get()
    );
    
    // Add more built-in functions as needed
}

llvm::Function* EnhancedIRGen::getBuiltinFunction(const String& name) {
    auto it = builtinFunctions_.find(name);
    return it != builtinFunctions_.end() ? it->second : nullptr;
}

// Helper methods
llvm::Value* EnhancedIRGen::createAlloca(llvm::Type* type, const String& name) {
    auto currentFunc = functionContexts_.empty() ? nullptr : functionContexts_.top().getFunction();
    if (!currentFunc) {
        reportError("Cannot create alloca outside function context", SourceLocation());
        return nullptr;
    }
    
    auto entryBlock = &currentFunc->getEntryBlock();
    auto insertPoint = builder_->saveIP();
    builder_->SetInsertPoint(entryBlock, entryBlock->begin());
    auto alloca = builder_->CreateAlloca(type, nullptr, name);
    builder_->restoreIP(insertPoint);
    return alloca;
}

void EnhancedIRGen::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
}

void EnhancedIRGen::reportWarning(const String& message, const SourceLocation& location) {
    diagnostics_.warning(message, location);
}

// Optimization methods
void EnhancedIRGen::optimizeMemoryUsage() {
    // Run basic optimizations
    llvm::legacy::FunctionPassManager fpm(module_.get());
    
    // Add optimization passes
    fpm.add(llvm::createPromoteMemoryToRegisterPass());
    fpm.add(llvm::createInstructionCombiningPass());
    fpm.add(llvm::createReassociatePass());
    fpm.add(llvm::createGVNPass());
    fpm.add(llvm::createCFGSimplificationPass());
    
    fpm.doInitialization();
    for (auto& func : *module_) {
        fpm.run(func);
    }
    fpm.doFinalization();
}

// ScopeContext implementation
EnhancedIRGen::ScopeContext::ScopeContext(EnhancedIRGen* irgen) : irgen_(irgen) {
    enterScope();
}

EnhancedIRGen::ScopeContext::~ScopeContext() {
    while (!symbolStack_.empty()) {
        exitScope();
    }
}

void EnhancedIRGen::ScopeContext::enterScope() {
    symbolStack_.push_back(std::unordered_map<String, llvm::Value*>());
}

void EnhancedIRGen::ScopeContext::exitScope() {
    if (symbolStack_.size() > 1) {
        symbolStack_.pop_back();
    }
}

void EnhancedIRGen::ScopeContext::declareSymbol(const String& name, llvm::Value* value) {
    if (!symbolStack_.empty()) {
        symbolStack_.back()[name] = value;
    }
}

llvm::Value* EnhancedIRGen::ScopeContext::getSymbol(const String& name) const {
    for (auto it = symbolStack_.rbegin(); it != symbolStack_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }
    return nullptr;
}

bool EnhancedIRGen::ScopeContext::hasSymbol(const String& name) const {
    return getSymbol(name) != nullptr;
}

// FunctionContext implementation
EnhancedIRGen::FunctionContext::FunctionContext(EnhancedIRGen* irgen, llvm::Function* function)
    : irgen_(irgen), function_(function), scope_(irgen) {
}

EnhancedIRGen::FunctionContext::~FunctionContext() {
}

void EnhancedIRGen::FunctionContext::createLabel(const String& name, llvm::BasicBlock* breakTarget, llvm::BasicBlock* continueTarget) {
    labels_[name] = std::make_pair(breakTarget, continueTarget);
}

llvm::BasicBlock* EnhancedIRGen::FunctionContext::getBreakTarget(const String& name) const {
    auto it = labels_.find(name);
    return it != labels_.end() ? it->second.first : nullptr;
}

llvm::BasicBlock* EnhancedIRGen::FunctionContext::getContinueTarget(const String& name) const {
    auto it = labels_.find(name);
    return it != labels_.end() ? it->second.second : nullptr;
}

// ExceptionContext implementation
EnhancedIRGen::ExceptionContext::ExceptionContext(EnhancedIRGen* irgen) : irgen_(irgen) {
}

EnhancedIRGen::ExceptionContext::~ExceptionContext() {
}

void EnhancedIRGen::ExceptionContext::enterTryBlock(llvm::BasicBlock* tryBlock, llvm::BasicBlock* catchBlock) {
    tryStack_.push(std::make_tuple(tryBlock, catchBlock, nullptr));
}

void EnhancedIRGen::ExceptionContext::enterCatchBlock(llvm::BasicBlock* catchBlock) {
    if (!tryStack_.empty()) {
        auto& top = tryStack_.top();
        std::get<1>(top) = catchBlock;
    }
}

void EnhancedIRGen::ExceptionContext::enterFinallyBlock(llvm::BasicBlock* finallyBlock) {
    if (!tryStack_.empty()) {
        auto& top = tryStack_.top();
        std::get<2>(top) = finallyBlock;
    }
}

// Factory function
std::unique_ptr<EnhancedIRGen> createEnhancedIRGen(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem) {
    return std::make_unique<EnhancedIRGen>(diagnostics, typeSystem);
}

} // namespace tsc