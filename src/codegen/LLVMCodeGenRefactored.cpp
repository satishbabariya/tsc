#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/codegen/context/CodeGenContext.h"
#include "tsc/codegen/types/TypeGenerator.h"
#include "tsc/codegen/expressions/ExpressionGenerator.h"
#include "tsc/codegen/statements/StatementGenerator.h"
#include "tsc/codegen/functions/FunctionGenerator.h"
#include "tsc/codegen/classes/ClassGenerator.h"
#include "tsc/codegen/builtins/BuiltinGenerator.h"
#include "tsc/codegen/optimization/OptimizationEngine.h"
#include "tsc/codegen/memory/MemoryManager.h"

#include "tsc/Compiler.h"
#include "tsc/TargetRegistry.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/AST.h"

#include <cmath>
#include <limits>

// LLVM includes for implementation
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/TypedPointerType.h"

#include <iostream>
#include <unordered_map>

namespace tsc {
namespace codegen {

// FunctionContext implementation
void FunctionContext::addLabel(const String& name, llvm::BasicBlock* start, llvm::BasicBlock* end) {
    labels_[name] = {start, end};
}

std::pair<llvm::BasicBlock*, llvm::BasicBlock*> FunctionContext::getLabel(const String& name) const {
    auto it = labels_.find(name);
    return it != labels_.end() ? it->second : std::make_pair(nullptr, nullptr);
}

bool FunctionContext::hasLabel(const String& name) const {
    return labels_.find(name) != labels_.end();
}

void FunctionContext::removeLabel(const String& name) {
    labels_.erase(name);
}

void FunctionContext::pushExceptionContext(const ExceptionContext& ctx) {
    exceptionStack_.push(ctx);
}

void FunctionContext::popExceptionContext() {
    if (!exceptionStack_.empty()) {
        exceptionStack_.pop();
    }
}

ExceptionContext& FunctionContext::getCurrentExceptionContext() {
    return exceptionStack_.top();
}

bool FunctionContext::hasExceptionContext() const {
    return !exceptionStack_.empty();
}

void FunctionContext::cleanup() {
    labels_.clear();
    while (!exceptionStack_.empty()) {
        exceptionStack_.pop();
    }
}

// LLVMCodeGen implementation
LLVMCodeGen::LLVMCodeGen(DiagnosticEngine& diagnostics, const CompilerOptions& options)
    : diagnostics_(diagnostics), options_(options), currentValue_(nullptr) {
    // Initialize LLVM - use dynamic target registry
    auto& registry = TargetRegistry::getInstance();
    registry.initializeAllTargets();

    // Create LLVM context and module
    context_ = std::make_unique<llvm::LLVMContext>();
    module_ = std::make_unique<llvm::Module>("tsc_module", *context_);
    builder_ = std::make_unique<llvm::IRBuilder<>>(*context_);

    // Create code generation context
    codeGenContext_ = std::make_unique<CodeGenContext>(*context_, *module_, *builder_, diagnostics_);

    // Initialize enhanced IR generation infrastructure
    functionContexts_ = std::stack<FunctionContext>();

    // Initialize module components
    typeGenerator_ = std::make_unique<TypeGenerator>(this);
    expressionGenerator_ = std::make_unique<ExpressionGenerator>(this);
    statementGenerator_ = std::make_unique<StatementGenerator>(this);
    functionGenerator_ = std::make_unique<FunctionGenerator>(this);
    classGenerator_ = std::make_unique<ClassGenerator>(this);
    builtinGenerator_ = std::make_unique<BuiltinGenerator>(this);
    optimizationEngine_ = std::make_unique<OptimizationEngine>(this);
    memoryManager_ = std::make_unique<MemoryManager>(this);

    // Initialize error reporting
    errorReporter_ = make_unique<EnhancedErrorReporting>(diagnostics_);

    // Setup target machine
    setupTargetMachine();

    // Declare builtin functions
    builtinGenerator_->declareBuiltinFunctions();
    builtinGenerator_->declareBuiltinGlobals();
}

LLVMCodeGen::~LLVMCodeGen() = default;

bool LLVMCodeGen::generateCode(Module& module, SymbolTable& symbolTable, 
                             const TypeSystem& typeSystem) {
    symbolTable_ = &symbolTable;
    typeSystem_ = &typeSystem;

    try {
        // Visit the module to generate code
        module.accept(static_cast<ASTVisitor&>(*this));

        // Process deferred initializations
        for (const auto& deferredInit : deferredGlobalInitializations_) {
            deferredInit.first->setInitializer(
                llvm::cast<llvm::Constant>(deferredInit.second)
            );
        }

        // Process deferred constructor calls
        for (const auto& deferredCall : deferredConstructorCalls_) {
            // TODO: Implement deferred constructor call processing
        }

        // Process deferred method calls
        for (const auto& deferredMethod : deferredMethodCalls_) {
            // TODO: Implement deferred method call processing
        }

        // Run optimization passes
        optimizationEngine_->runOptimizationPasses();

        // Verify the module
        if (llvm::verifyModule(*module_, &llvm::errs())) {
            reportError("Generated LLVM IR is invalid", SourceLocation());
            return false;
        }

        return !hasErrors();
    } catch (const std::exception& e) {
        reportError("Code generation failed: " + String(e.what()), SourceLocation());
        return false;
    }
}

String LLVMCodeGen::getLLVMIRString() const {
    String result;
    llvm::raw_string_ostream stream(result);
    module_->print(stream, nullptr);
    return result;
}

bool LLVMCodeGen::emitLLVMIR(const String& filename) const {
    std::error_code ec;
    llvm::raw_fd_ostream file(filename, ec);
    if (ec) {
        return false;
    }
    module_->print(file, nullptr);
    return true;
}

bool LLVMCodeGen::emitObjectFile(const String& filename) const {
    // TODO: Implement object file emission
    return false;
}

bool LLVMCodeGen::emitAssembly(const String& filename) const {
    // TODO: Implement assembly emission
    return false;
}

bool LLVMCodeGen::setupTargetMachine() {
    // TODO: Implement target machine setup
    return true;
}

String LLVMCodeGen::getTargetTriple() const {
    return module_->getTargetTriple();
}

void LLVMCodeGen::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
}

void LLVMCodeGen::reportWarning(const String& message, const SourceLocation& location) {
    diagnostics_.warning(message, location);
}

FunctionContext& LLVMCodeGen::getCurrentFunctionContext() {
    if (functionContexts_.empty()) {
        throw std::runtime_error("No active function context");
    }
    return functionContexts_.top();
}

void LLVMCodeGen::enterSwitch(llvm::BasicBlock* exitBlock) {
    codeGenContext_->enterSwitch(exitBlock);
}

void LLVMCodeGen::exitSwitch() {
    codeGenContext_->exitSwitch();
}

llvm::BasicBlock* LLVMCodeGen::getCurrentSwitchExitBlock() const {
    return codeGenContext_->getCurrentSwitchExitBlock();
}

// Visitor implementations - delegate to appropriate generators
void LLVMCodeGen::visit(NumericLiteral& node) {
    setCurrentValue(expressionGenerator_->generateNumericLiteral(node));
}

void LLVMCodeGen::visit(StringLiteral& node) {
    setCurrentValue(expressionGenerator_->generateStringLiteral(node));
}

void LLVMCodeGen::visit(TemplateLiteral& node) {
    setCurrentValue(expressionGenerator_->generateTemplateLiteral(node));
}

void LLVMCodeGen::visit(BooleanLiteral& node) {
    setCurrentValue(expressionGenerator_->generateBooleanLiteral(node));
}

void LLVMCodeGen::visit(NullLiteral& node) {
    setCurrentValue(expressionGenerator_->generateNullLiteral(node));
}

void LLVMCodeGen::visit(Identifier& node) {
    setCurrentValue(expressionGenerator_->generateIdentifier(node));
}

void LLVMCodeGen::visit(ThisExpression& node) {
    setCurrentValue(expressionGenerator_->generateThisExpression(node));
}

void LLVMCodeGen::visit(SuperExpression& node) {
    setCurrentValue(expressionGenerator_->generateSuperExpression(node));
}

void LLVMCodeGen::visit(NewExpression& node) {
    setCurrentValue(expressionGenerator_->generateNewExpression(node));
}

void LLVMCodeGen::visit(BinaryExpression& node) {
    setCurrentValue(expressionGenerator_->generateBinaryExpression(node));
}

void LLVMCodeGen::visit(UnaryExpression& node) {
    setCurrentValue(expressionGenerator_->generateUnaryExpression(node));
}

void LLVMCodeGen::visit(AssignmentExpression& node) {
    setCurrentValue(expressionGenerator_->generateAssignmentExpression(node));
}

void LLVMCodeGen::visit(ConditionalExpression& node) {
    setCurrentValue(expressionGenerator_->generateConditionalExpression(node));
}

void LLVMCodeGen::visit(CallExpression& node) {
    setCurrentValue(expressionGenerator_->generateCallExpression(node));
}

void LLVMCodeGen::visit(ArrayLiteral& node) {
    setCurrentValue(expressionGenerator_->generateArrayLiteral(node));
}

void LLVMCodeGen::visit(IndexExpression& node) {
    setCurrentValue(expressionGenerator_->generateIndexExpression(node));
}

void LLVMCodeGen::visit(ObjectLiteral& node) {
    setCurrentValue(expressionGenerator_->generateObjectLiteral(node));
}

void LLVMCodeGen::visit(PropertyAccess& node) {
    setCurrentValue(expressionGenerator_->generatePropertyAccess(node));
}

void LLVMCodeGen::visit(ArrowFunction& node) {
    setCurrentValue(expressionGenerator_->generateArrowFunction(node));
}

void LLVMCodeGen::visit(FunctionExpression& node) {
    setCurrentValue(expressionGenerator_->generateFunctionExpression(node));
}

void LLVMCodeGen::visit(MoveExpression& node) {
    setCurrentValue(expressionGenerator_->generateMoveExpression(node));
}

void LLVMCodeGen::visit(OptionalPropertyAccess& node) {
    setCurrentValue(expressionGenerator_->generateOptionalPropertyAccess(node));
}

void LLVMCodeGen::visit(OptionalIndexAccess& node) {
    setCurrentValue(expressionGenerator_->generateOptionalIndexAccess(node));
}

void LLVMCodeGen::visit(OptionalCallExpr& node) {
    setCurrentValue(expressionGenerator_->generateOptionalCallExpr(node));
}

void LLVMCodeGen::visit(SpreadElement& node) {
    setCurrentValue(expressionGenerator_->generateSpreadElement(node));
}

void LLVMCodeGen::visit(ForOfStatement& node) {
    statementGenerator_->generateForOfStatement(node);
}

void LLVMCodeGen::visit(ExpressionStatement& node) {
    statementGenerator_->generateExpressionStatement(node);
}

void LLVMCodeGen::visit(BlockStatement& node) {
    statementGenerator_->generateBlockStatement(node);
}

void LLVMCodeGen::visit(ReturnStatement& node) {
    statementGenerator_->generateReturnStatement(node);
}

void LLVMCodeGen::visit(IfStatement& node) {
    statementGenerator_->generateIfStatement(node);
}

void LLVMCodeGen::visit(WhileStatement& node) {
    statementGenerator_->generateWhileStatement(node);
}

void LLVMCodeGen::visit(DoWhileStatement& node) {
    statementGenerator_->generateDoWhileStatement(node);
}

void LLVMCodeGen::visit(ForStatement& node) {
    statementGenerator_->generateForStatement(node);
}

void LLVMCodeGen::visit(SwitchStatement& node) {
    statementGenerator_->generateSwitchStatement(node);
}

void LLVMCodeGen::visit(CaseClause& node) {
    statementGenerator_->generateCaseClause(node);
}

void LLVMCodeGen::visit(BreakStatement& node) {
    statementGenerator_->generateBreakStatement(node);
}

void LLVMCodeGen::visit(ContinueStatement& node) {
    statementGenerator_->generateContinueStatement(node);
}

void LLVMCodeGen::visit(TryStatement& node) {
    statementGenerator_->generateTryStatement(node);
}

void LLVMCodeGen::visit(CatchClause& node) {
    statementGenerator_->generateCatchClause(node);
}

void LLVMCodeGen::visit(ThrowStatement& node) {
    statementGenerator_->generateThrowStatement(node);
}

void LLVMCodeGen::visit(VariableDeclaration& node) {
    statementGenerator_->generateVariableDeclaration(node);
}

void LLVMCodeGen::visit(FunctionDeclaration& node) {
    functionGenerator_->generateFunctionDeclaration(node);
}

void LLVMCodeGen::visit(TypeParameter& node) {
    // Type parameters are handled during generic type processing
}

void LLVMCodeGen::visit(PropertyDeclaration& node) {
    classGenerator_->generatePropertyDeclaration(node);
}

void LLVMCodeGen::visit(MethodDeclaration& node) {
    classGenerator_->generateMethodDeclaration(node);
}

void LLVMCodeGen::visit(DestructorDeclaration& node) {
    classGenerator_->generateDestructorDeclaration(node);
}

void LLVMCodeGen::visit(ClassDeclaration& node) {
    classGenerator_->generateClassDeclaration(node);
}

void LLVMCodeGen::visit(InterfaceDeclaration& node) {
    classGenerator_->generateInterfaceDeclaration(node);
}

void LLVMCodeGen::visit(EnumMember& node) {
    classGenerator_->generateEnumMember(node);
}

void LLVMCodeGen::visit(EnumDeclaration& node) {
    classGenerator_->generateEnumDeclaration(node);
}

void LLVMCodeGen::visit(TypeAliasDeclaration& node) {
    classGenerator_->generateTypeAliasDeclaration(node);
}

void LLVMCodeGen::visit(ImportDeclaration& node) {
    // Import declarations are handled during module resolution
}

void LLVMCodeGen::visit(ExportDeclaration& node) {
    // Export declarations are handled during module resolution
}

void LLVMCodeGen::visit(Module& module) {
    // Process all declarations in the module
    for (const auto& declaration : module.getDeclarations()) {
        declaration->accept(static_cast<ASTVisitor&>(*this));
    }
}

void LLVMCodeGen::visit(DestructuringPattern& node) {
    // Destructuring patterns are handled during assignment processing
}

void LLVMCodeGen::visit(ArrayDestructuringPattern& node) {
    // Array destructuring patterns are handled during assignment processing
}

void LLVMCodeGen::visit(ObjectDestructuringPattern& node) {
    // Object destructuring patterns are handled during assignment processing
}

void LLVMCodeGen::visit(IdentifierPattern& node) {
    // Identifier patterns are handled during assignment processing
}

void LLVMCodeGen::visit(DestructuringAssignment& node) {
    // Destructuring assignments are handled during assignment processing
}

// Factory function
unique_ptr<LLVMCodeGen> createLLVMCodeGen(DiagnosticEngine& diagnostics, const CompilerOptions& options) {
    return std::make_unique<LLVMCodeGen>(diagnostics, options);
}

} // namespace codegen
} // namespace tsc