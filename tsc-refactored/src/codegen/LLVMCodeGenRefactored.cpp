#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/codegen/context/CodeGenContext.h"
#include "tsc/codegen/types/TypeGenerator.h"
#include "tsc/codegen/expressions/ExpressionGenerator.h"
#include "tsc/codegen/statements/StatementGenerator.h"
#include "tsc/codegen/functions/FunctionGenerator.h"
#include "tsc/codegen/builtins/BuiltinGenerator.h"
#include "tsc/ast/ASTRefactored.h"

namespace tsc {
namespace codegen {

LLVMCodeGenRefactored::LLVMCodeGenRefactored(DiagnosticEngine& diagnostics, const CompilerOptions& options)
    : diagnostics_(diagnostics), options_(options), currentValue_(nullptr) {
    
    // Initialize LLVM context and module
    context_ = std::make_unique<llvm::LLVMContext>();
    module_ = std::make_unique<llvm::Module>("tsc_module", *context_);
    builder_ = std::make_unique<llvm::IRBuilder<>>(*context_);
    
    // Create code generation context
    codeGenContext_ = std::make_unique<CodeGenContext>(*context_, *module_, *builder_, diagnostics_);
    
    // Create generators
    typeGenerator_ = std::make_unique<TypeGenerator>(this);
    expressionGenerator_ = std::make_unique<ExpressionGenerator>(this);
    statementGenerator_ = std::make_unique<StatementGenerator>(this);
    functionGenerator_ = std::make_unique<FunctionGenerator>(this);
    builtinGenerator_ = std::make_unique<BuiltinGenerator>(this);
    
    // Declare built-in functions and globals
    builtinGenerator_->declareBuiltinFunctions();
    builtinGenerator_->declareBuiltinGlobals();
}

LLVMCodeGenRefactored::~LLVMCodeGenRefactored() = default;

bool LLVMCodeGenRefactored::generateCode(ast::Module& astModule) {
    try {
        std::cout << "=== LLVM Code Generation Started ===" << std::endl;
        
        // Generate code for all declarations in the module
        for (auto& declaration : astModule.getDeclarations()) {
            generateDeclaration(*declaration);
            
            if (hasErrors()) {
                std::cout << "Code generation failed with errors" << std::endl;
                return false;
            }
        }
        
        // Verify the module
        if (!verifyModule()) {
            std::cout << "Module verification failed" << std::endl;
            return false;
        }
        
        std::cout << "=== LLVM Code Generation Completed Successfully ===" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        reportError("Code generation failed with exception: " + String(e.what()), SourceLocation());
        return false;
    }
}

bool LLVMCodeGenRefactored::writeOutput(const String& filename) {
    try {
        std::error_code errorCode;
        llvm::raw_fd_ostream output(filename, errorCode);
        
        if (errorCode) {
            reportError("Failed to open output file: " + filename, SourceLocation());
            return false;
        }
        
        // Write LLVM IR to file
        module_->print(output, nullptr);
        output.close();
        
        std::cout << "LLVM IR written to: " << filename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        reportError("Failed to write output: " + String(e.what()), SourceLocation());
        return false;
    }
}

void LLVMCodeGenRefactored::generateDeclaration(ast::Declaration& decl) {
    decl.accept(*this);
}

llvm::Value* LLVMCodeGenRefactored::generateExpression(ast::Expression& expr) {
    return expressionGenerator_->generateExpression(expr);
}

void LLVMCodeGenRefactored::generateStatement(ast::Statement& stmt) {
    statementGenerator_->generateStatement(stmt);
}

// Visitor methods for declarations
void LLVMCodeGenRefactored::visit(ast::declarations::FunctionDeclaration& node) {
    std::cout << "DEBUG: Generating function declaration: " << node.getName() << std::endl;
    
    // Generate function declaration
    llvm::Function* function = functionGenerator_->generateFunctionDeclaration(node);
    if (!function) {
        reportError("Failed to generate function declaration: " + node.getName(), node.getLocation());
        return;
    }
    
    // Generate function body
    functionGenerator_->generateFunctionBody(function, node);
    
    std::cout << "DEBUG: Function generation completed: " << node.getName() << std::endl;
}

void LLVMCodeGenRefactored::visit(ast::declarations::VariableDeclaration& node) {
    std::cout << "DEBUG: Generating variable declaration: " << node.getName() << std::endl;
    
    // Determine variable type
    llvm::Type* varType = typeGenerator_->getAnyType(); // Default
    if (node.getType()) {
        varType = typeGenerator_->mapTypeScriptTypeToLLVM(*node.getType());
    }
    
    // Create alloca for the variable
    llvm::AllocaInst* alloca = builder_->CreateAlloca(varType, nullptr, node.getName());
    
    // Initialize with value if provided
    if (node.getInitializer()) {
        llvm::Value* initValue = generateExpression(*node.getInitializer());
        if (initValue) {
            // Convert to variable type if necessary
            if (initValue->getType() != varType) {
                initValue = typeGenerator_->convertValue(initValue, initValue->getType(), varType);
            }
            builder_->CreateStore(initValue, alloca);
        }
    } else {
        // Initialize with default value
        llvm::Value* defaultValue = createDefaultValue(varType);
        builder_->CreateStore(defaultValue, alloca);
    }
    
    // Add to symbol table
    codeGenContext_->setSymbolValue(node.getName(), alloca);
    
    std::cout << "DEBUG: Variable declaration completed: " << node.getName() << std::endl;
}

void LLVMCodeGenRefactored::visit(ast::declarations::ClassDeclaration& node) {
    std::cout << "DEBUG: Generating class declaration: " << node.getName() << std::endl;
    
    // For now, classes are simplified - just process their members
    codeGenContext_->enterClass(node.getName());
    
    // Process class members
    for (auto& member : node.getMembers()) {
        generateStatement(*member);
    }
    
    codeGenContext_->exitClass();
    
    std::cout << "DEBUG: Class declaration completed: " << node.getName() << std::endl;
}

// Visitor methods for expressions (delegate to ExpressionGenerator)
void LLVMCodeGenRefactored::visit(ast::expressions::NumericLiteral& node) {
    setCurrentValue(expressionGenerator_->generateNumericLiteral(node));
}

void LLVMCodeGenRefactored::visit(ast::expressions::StringLiteral& node) {
    setCurrentValue(expressionGenerator_->generateStringLiteral(node));
}

void LLVMCodeGenRefactored::visit(ast::expressions::BooleanLiteral& node) {
    setCurrentValue(expressionGenerator_->generateBooleanLiteral(node));
}

void LLVMCodeGenRefactored::visit(ast::expressions::NullLiteral& node) {
    setCurrentValue(expressionGenerator_->generateNullLiteral(node));
}

void LLVMCodeGenRefactored::visit(ast::expressions::Identifier& node) {
    setCurrentValue(expressionGenerator_->generateIdentifier(node));
}

void LLVMCodeGenRefactored::visit(ast::expressions::BinaryExpression& node) {
    setCurrentValue(expressionGenerator_->generateBinaryExpression(node));
}

void LLVMCodeGenRefactored::visit(ast::expressions::UnaryExpression& node) {
    setCurrentValue(expressionGenerator_->generateUnaryExpression(node));
}

void LLVMCodeGenRefactored::visit(ast::expressions::CallExpression& node) {
    setCurrentValue(functionGenerator_->generateCallExpression(node));
}

// Visitor methods for statements (delegate to StatementGenerator)
void LLVMCodeGenRefactored::visit(ast::statements::ExpressionStatement& node) {
    statementGenerator_->generateExpressionStatement(node);
}

void LLVMCodeGenRefactored::visit(ast::statements::BlockStatement& node) {
    statementGenerator_->generateBlockStatement(node);
}

void LLVMCodeGenRefactored::visit(ast::statements::ReturnStatement& node) {
    statementGenerator_->generateReturnStatement(node);
}

void LLVMCodeGenRefactored::visit(ast::statements::IfStatement& node) {
    statementGenerator_->generateIfStatement(node);
}

void LLVMCodeGenRefactored::visit(ast::statements::WhileStatement& node) {
    statementGenerator_->generateWhileStatement(node);
}

// Utility methods
llvm::Value* LLVMCodeGenRefactored::createDefaultValue(llvm::Type* type) {
    if (type->isIntegerTy()) {
        return llvm::ConstantInt::get(type, 0);
    } else if (type->isFloatingPointTy()) {
        return llvm::ConstantFP::get(type, 0.0);
    } else if (type->isPointerTy()) {
        return llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(type));
    } else {
        return llvm::UndefValue::get(type);
    }
}

bool LLVMCodeGenRefactored::verifyModule() {
    std::string errorString;
    llvm::raw_string_ostream errorStream(errorString);
    
    if (llvm::verifyModule(*module_, &errorStream)) {
        std::cout << "Module verification failed:" << std::endl;
        std::cout << errorStream.str() << std::endl;
        return false;
    }
    
    std::cout << "Module verification passed" << std::endl;
    return true;
}

void LLVMCodeGenRefactored::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
}

bool LLVMCodeGenRefactored::hasErrors() const {
    return diagnostics_.hasErrors();
}

// Accessors
llvm::LLVMContext* LLVMCodeGenRefactored::getLLVMContext() {
    return context_.get();
}

llvm::Module* LLVMCodeGenRefactored::getModule() {
    return module_.get();
}

llvm::IRBuilder<>* LLVMCodeGenRefactored::getBuilder() {
    return builder_.get();
}

CodeGenContext* LLVMCodeGenRefactored::getCodeGenContext() {
    return codeGenContext_.get();
}

TypeGenerator* LLVMCodeGenRefactored::getTypeGenerator() {
    return typeGenerator_.get();
}

ExpressionGenerator* LLVMCodeGenRefactored::getExpressionGenerator() {
    return expressionGenerator_.get();
}

StatementGenerator* LLVMCodeGenRefactored::getStatementGenerator() {
    return statementGenerator_.get();
}

FunctionGenerator* LLVMCodeGenRefactored::getFunctionGenerator() {
    return functionGenerator_.get();
}

BuiltinGenerator* LLVMCodeGenRefactored::getBuiltinGenerator() {
    return builtinGenerator_.get();
}

SymbolTable* LLVMCodeGenRefactored::getSymbolTable() {
    return symbolTable_;
}

void LLVMCodeGenRefactored::setSymbolTable(SymbolTable* symbolTable) {
    symbolTable_ = symbolTable;
}

llvm::Value* LLVMCodeGenRefactored::getCurrentValue() const {
    return currentValue_;
}

void LLVMCodeGenRefactored::setCurrentValue(llvm::Value* value) {
    currentValue_ = value;
}

} // namespace codegen
} // namespace tsc