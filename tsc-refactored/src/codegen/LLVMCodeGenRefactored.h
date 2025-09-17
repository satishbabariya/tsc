#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/EnhancedErrorReporting.h"

// LLVM includes
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"

#include <unordered_map>
#include <stack>
#include <iostream>

namespace tsc {
namespace codegen {

// Forward declarations
class CodeGenContext;
class TypeGenerator;
class ExpressionGenerator;
class StatementGenerator;
class FunctionGenerator;
class ClassGenerator;
class BuiltinGenerator;
class OptimizationEngine;
class MemoryManager;

// Forward declarations
class CompilerOptions;

// Enhanced IR generation infrastructure classes
class ExceptionContext {
private:
    llvm::BasicBlock* tryBlock_;
    llvm::BasicBlock* catchBlock_;
    llvm::BasicBlock* finallyBlock_;
    llvm::Value* exceptionVar_;
    
public:
    ExceptionContext(llvm::BasicBlock* tryBlock, llvm::BasicBlock* catchBlock, 
                    llvm::BasicBlock* finallyBlock = nullptr, llvm::Value* exceptionVar = nullptr)
        : tryBlock_(tryBlock), catchBlock_(catchBlock), 
          finallyBlock_(finallyBlock), exceptionVar_(exceptionVar) {}
    
    llvm::BasicBlock* getTryBlock() const { return tryBlock_; }
    llvm::BasicBlock* getCatchBlock() const { return catchBlock_; }
    llvm::BasicBlock* getFinallyBlock() const { return finallyBlock_; }
    llvm::Value* getExceptionVar() const { return exceptionVar_; }
    
    bool hasFinally() const { return finallyBlock_ != nullptr; }
    bool hasExceptionVar() const { return exceptionVar_ != nullptr; }
    
    // Exception handling utilities
    void setExceptionVar(llvm::Value* var) { exceptionVar_ = var; }
    void setFinallyBlock(llvm::BasicBlock* block) { finallyBlock_ = block; }
};

class FunctionContext {
private:
    llvm::Function* function_;
    std::unordered_map<String, std::pair<llvm::BasicBlock*, llvm::BasicBlock*>> labels_;
    std::stack<ExceptionContext> exceptionStack_;
    
public:
    FunctionContext(llvm::Function* func) : function_(func) {}
    
    llvm::Function* getFunction() const { return function_; }
    
    // Label management for complex control flow
    void addLabel(const String& name, llvm::BasicBlock* start, llvm::BasicBlock* end);
    std::pair<llvm::BasicBlock*, llvm::BasicBlock*> getLabel(const String& name) const;
    bool hasLabel(const String& name) const;
    void removeLabel(const String& name);
    
    // Exception handling context
    void pushExceptionContext(const ExceptionContext& ctx);
    void popExceptionContext();
    ExceptionContext& getCurrentExceptionContext();
    bool hasExceptionContext() const;
    
    // Cleanup and resource management
    void cleanup();
};

class ScopeContext {
private:
    std::vector<std::unordered_map<String, llvm::Value*>> symbolStack_;
    std::vector<std::unordered_map<String, llvm::Type*>> typeStack_;
    
public:
    ScopeContext() {
        // Start with global scope
        enterScope();
    }
    
    void enterScope() {
        symbolStack_.push_back({});
        typeStack_.push_back({});
    }
    
    void exitScope() {
        if (symbolStack_.size() > 1) {
            symbolStack_.pop_back();
            typeStack_.pop_back();
        }
    }
    
    void declareSymbol(const String& name, llvm::Value* value) {
        symbolStack_.back()[name] = value;
    }
    
    void declareType(const String& name, llvm::Type* type) {
        typeStack_.back()[name] = type;
    }
    
    llvm::Value* getSymbol(const String& name) const {
        // Search from innermost to outermost scope
        for (auto it = symbolStack_.rbegin(); it != symbolStack_.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second;
            }
        }
        return nullptr;
    }
    
    llvm::Type* getType(const String& name) const {
        // Search from innermost to outermost scope
        for (auto it = typeStack_.rbegin(); it != symbolStack_.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second;
            }
        }
        return nullptr;
    }
    
    bool isSymbolDeclared(const String& name) const {
        return getSymbol(name) != nullptr;
    }
    
    bool isTypeDeclared(const String& name) const {
        return getType(name) != nullptr;
    }
    
    // Get current scope depth
    size_t getScopeDepth() const {
        return symbolStack_.size();
    }
};

// Main LLVM code generator class - orchestrates all modules
class LLVMCodeGen : public ASTVisitor {
public:
    explicit LLVMCodeGen(DiagnosticEngine& diagnostics, const CompilerOptions& options);
    ~LLVMCodeGen();
    
    // Main code generation interface
    bool generateCode(Module& module, SymbolTable& symbolTable, 
                     const TypeSystem& typeSystem);
    
    // Output generation
    bool emitLLVMIR(const String& filename) const;
    bool emitObjectFile(const String& filename) const;
    bool emitAssembly(const String& filename) const;
    
    // Access to generated code
    llvm::Module* getLLVMModule() const { return module_.get(); }
    String getLLVMIRString() const;
    
    // Multi-module compilation control
    void setGenerateMainFunction(bool generate) { generateMainFunction_ = generate; }
    
    // Target setup
    bool setupTargetMachine();
    String getTargetTriple() const;
    
    // Utility methods
    void setCurrentValue(llvm::Value* value) { currentValue_ = value; }
    llvm::Value* getCurrentValue() const { return currentValue_; }
    bool hasErrors() const { return codeGenContext_->getErrorCount() > 0; }
    
    // Error handling
    void reportError(const String& message, const SourceLocation& location);
    void reportWarning(const String& message, const SourceLocation& location);
    
    // Access to modules (for internal use)
    TypeGenerator* getTypeGenerator() const { return typeGenerator_.get(); }
    ExpressionGenerator* getExpressionGenerator() const { return expressionGenerator_.get(); }
    StatementGenerator* getStatementGenerator() const { return statementGenerator_.get(); }
    FunctionGenerator* getFunctionGenerator() const { return functionGenerator_.get(); }
    ClassGenerator* getClassGenerator() const { return classGenerator_.get(); }
    BuiltinGenerator* getBuiltinGenerator() const { return builtinGenerator_.get(); }
    OptimizationEngine* getOptimizationEngine() const { return optimizationEngine_.get(); }
    MemoryManager* getMemoryManager() const { return memoryManager_.get(); }
    
    // Access to LLVM infrastructure
    llvm::LLVMContext* getLLVMContext() const { return context_.get(); }
    llvm::Module* getModule() const { return module_.get(); }
    llvm::IRBuilder<>* getBuilder() const { return builder_.get(); }
    CodeGenContext* getCodeGenContext() const { return codeGenContext_.get(); }
    
    // Enhanced IR generation infrastructure access
    std::stack<FunctionContext>& getFunctionContexts() { return functionContexts_; }
    FunctionContext& getCurrentFunctionContext();
    
    // Switch context for break statements
    void enterSwitch(llvm::BasicBlock* exitBlock);
    void exitSwitch();
    llvm::BasicBlock* getCurrentSwitchExitBlock() const;
    
    // Visitor interface implementation
    void visit(NumericLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(TemplateLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(NullLiteral& node) override;
    void visit(Identifier& node) override;
    void visit(ThisExpression& node) override;
    void visit(SuperExpression& node) override;
    void visit(NewExpression& node) override;
    void visit(BinaryExpression& node) override;
    void visit(UnaryExpression& node) override;
    void visit(AssignmentExpression& node) override;
    void visit(ConditionalExpression& node) override;
    void visit(CallExpression& node) override;
    void visit(ArrayLiteral& node) override;
    void visit(IndexExpression& node) override;
    void visit(ObjectLiteral& node) override;
    void visit(PropertyAccess& node) override;
    void visit(ArrowFunction& node) override;
    void visit(FunctionExpression& node) override;
    void visit(MoveExpression& node) override;
    void visit(OptionalPropertyAccess& node) override;
    void visit(OptionalIndexAccess& node) override;
    void visit(OptionalCallExpr& node) override;
    void visit(SpreadElement& node) override;
    void visit(ForOfStatement& node) override;
    
    void visit(ExpressionStatement& node) override;
    void visit(BlockStatement& node) override;
    void visit(ReturnStatement& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(DoWhileStatement& node) override;
    void visit(ForStatement& node) override;
    void visit(SwitchStatement& node) override;
    void visit(CaseClause& node) override;
    void visit(BreakStatement& node) override;
    void visit(ContinueStatement& node) override;
    void visit(TryStatement& node) override;
    void visit(CatchClause& node) override;
    void visit(ThrowStatement& node) override;
    void visit(VariableDeclaration& node) override;
    void visit(FunctionDeclaration& node) override;
    void visit(TypeParameter& node) override;
    
    // Class-related declarations
    void visit(PropertyDeclaration& node) override;
    void visit(MethodDeclaration& node) override;
    void visit(DestructorDeclaration& node) override;
    void visit(ClassDeclaration& node) override;
    void visit(InterfaceDeclaration& node) override;
    void visit(EnumMember& node) override;
    void visit(EnumDeclaration& node) override;
    void visit(TypeAliasDeclaration& node) override;
    void visit(ImportDeclaration& node) override;
    void visit(ExportDeclaration& node) override;
    
    void visit(Module& module) override;
    
    // Destructuring visitor methods
    void visit(DestructuringPattern& node) override;
    void visit(ArrayDestructuringPattern& node) override;
    void visit(ObjectDestructuringPattern& node) override;
    void visit(IdentifierPattern& node) override;
    void visit(DestructuringAssignment& node) override;

private:
    DiagnosticEngine& diagnostics_;
    const CompilerOptions& options_;
    
    // LLVM infrastructure
    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<llvm::Module> module_;
    std::unique_ptr<llvm::IRBuilder<>> builder_;
    std::unique_ptr<CodeGenContext> codeGenContext_;
    
    // Target machine for code generation
    std::unique_ptr<llvm::TargetMachine> targetMachine_;
    
    // Enhanced IR generation infrastructure
    std::stack<FunctionContext> functionContexts_;
    
    // Multi-module compilation control
    bool generateMainFunction_ = true;
    
    // Type system integration
    SymbolTable* symbolTable_;
    const TypeSystem* typeSystem_;
    
    // Current expression value (for visitor pattern)
    llvm::Value* currentValue_;
    
    // Closure struct type cache to ensure consistent types
    std::unordered_map<String, llvm::StructType*> closureTypeCache_;
    
    // Deferred global variable initializations (for non-constant values)
    std::vector<std::pair<llvm::GlobalVariable*, llvm::Value*>> deferredGlobalInitializations_;
    
    // Deferred constructor calls (for global objects that need constructor initialization)
    struct DeferredConstructorCall {
        llvm::GlobalVariable* globalVar;
        String className;
        std::vector<std::shared_ptr<Type>> typeArguments;
        std::vector<llvm::Value*> constructorArgs;
    };
    std::vector<DeferredConstructorCall> deferredConstructorCalls_;
    
    // Deferred method calls (for method calls on global variables)
    std::vector<std::pair<CallExpression*, llvm::GlobalVariable*>> deferredMethodCalls_;
    
    // Module components
    std::unique_ptr<TypeGenerator> typeGenerator_;
    std::unique_ptr<ExpressionGenerator> expressionGenerator_;
    std::unique_ptr<StatementGenerator> statementGenerator_;
    std::unique_ptr<FunctionGenerator> functionGenerator_;
    std::unique_ptr<ClassGenerator> classGenerator_;
    std::unique_ptr<BuiltinGenerator> builtinGenerator_;
    std::unique_ptr<OptimizationEngine> optimizationEngine_;
    std::unique_ptr<MemoryManager> memoryManager_;
    
    // Error reporting
    unique_ptr<EnhancedErrorReporting> errorReporter_;
};

// Code generation result
struct CodeGenResult {
    bool success = false;
    String errorMessage;
    String llvmIR;
    String objectFile;
    String assemblyFile;
    size_t errorCount = 0;
    size_t warningCount = 0;
};

// Factory function
unique_ptr<LLVMCodeGen> createLLVMCodeGen(DiagnosticEngine& diagnostics, const CompilerOptions& options);

} // namespace codegen
} // namespace tsc