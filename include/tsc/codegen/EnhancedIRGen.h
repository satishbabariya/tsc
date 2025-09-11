#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/semantic/SymbolTable.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>

#include <unordered_map>
#include <stack>
#include <vector>
#include <memory>

namespace tsc {

// Forward declarations
class DiagnosticEngine;
class TypeSystem;

// Enhanced IRGen inspired by Hermes
class EnhancedIRGen {
public:
    explicit EnhancedIRGen(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem);
    ~EnhancedIRGen() = default;

    // Main code generation entry points
    bool generateCode(const std::vector<unique_ptr<Statement>>& statements);
    bool generateCode(const unique_ptr<Module>& module);
    
    // Get generated LLVM module
    llvm::Module* getModule() const { return module_.get(); }
    llvm::LLVMContext& getContext() const { return *context_; }

private:
    // Core generation methods
    void generateModule(const unique_ptr<Module>& module);
    void generateStatement(const unique_ptr<Statement>& statement);
    void generateExpression(const unique_ptr<Expression>& expression);
    void generateDeclaration(const unique_ptr<Declaration>& declaration);
    
    // Function generation
    llvm::Function* generateFunction(const unique_ptr<FunctionDeclaration>& func);
    llvm::Function* generateMethod(const unique_ptr<MethodDeclaration>& method);
    llvm::Function* generateConstructor(const unique_ptr<ConstructorDeclaration>& ctor);
    
    // Expression generation
    llvm::Value* generateBinaryExpression(const unique_ptr<BinaryExpression>& expr);
    llvm::Value* generateUnaryExpression(const unique_ptr<UnaryExpression>& expr);
    llvm::Value* generateCallExpression(const unique_ptr<CallExpression>& expr);
    llvm::Value* generateMemberAccess(const unique_ptr<MemberAccessExpression>& expr);
    llvm::Value* generateNewExpression(const unique_ptr<NewExpression>& expr);
    llvm::Value* generateConditionalExpression(const unique_ptr<ConditionalExpression>& expr);
    
    // Literal generation
    llvm::Value* generateNumericLiteral(const unique_ptr<NumericLiteral>& literal);
    llvm::Value* generateStringLiteral(const unique_ptr<StringLiteral>& literal);
    llvm::Value* generateBooleanLiteral(const unique_ptr<BooleanLiteral>& literal);
    llvm::Value* generateNullLiteral(const unique_ptr<NullLiteral>& literal);
    llvm::Value* generateArrayLiteral(const unique_ptr<ArrayLiteral>& literal);
    llvm::Value* generateObjectLiteral(const unique_ptr<ObjectLiteral>& literal);
    
    // Statement generation
    void generateBlockStatement(const unique_ptr<BlockStatement>& stmt);
    void generateIfStatement(const unique_ptr<IfStatement>& stmt);
    void generateWhileStatement(const unique_ptr<WhileStatement>& stmt);
    void generateForStatement(const unique_ptr<ForStatement>& stmt);
    void generateReturnStatement(const unique_ptr<ReturnStatement>& stmt);
    void generateVariableDeclaration(const unique_ptr<VariableDeclaration>& stmt);
    
    // Type generation
    llvm::Type* generateType(const shared_ptr<Type>& type);
    llvm::Type* generatePrimitiveType(const shared_ptr<Type>& type);
    llvm::Type* generateArrayType(const shared_ptr<Type>& type);
    llvm::Type* generateObjectType(const shared_ptr<Type>& type);
    llvm::Type* generateFunctionType(const shared_ptr<Type>& type);
    llvm::Type* generateGenericType(const shared_ptr<Type>& type);
    
    // Class generation
    llvm::StructType* generateClassType(const unique_ptr<ClassDeclaration>& classDecl);
    void generateClassMethods(const unique_ptr<ClassDeclaration>& classDecl, llvm::StructType* structType);
    void generateClassConstructor(const unique_ptr<ClassDeclaration>& classDecl, llvm::StructType* structType);
    
    // Generic type handling
    llvm::Type* createMonomorphizedType(const shared_ptr<Type>& genericType, 
                                       const std::vector<shared_ptr<Type>>& typeArgs);
    void generateMonomorphizedMethods(const shared_ptr<Type>& genericType,
                                    const std::vector<shared_ptr<Type>>& typeArgs);
    
    // Scope management (inspired by Hermes)
    class ScopeContext {
    public:
        explicit ScopeContext(EnhancedIRGen* irgen);
        ~ScopeContext();
        
        void enterScope();
        void exitScope();
        void declareSymbol(const String& name, llvm::Value* value);
        llvm::Value* getSymbol(const String& name) const;
        bool hasSymbol(const String& name) const;
        
    private:
        EnhancedIRGen* irgen_;
        std::vector<std::unordered_map<String, llvm::Value*>> symbolStack_;
    };
    
    // Function context (inspired by Hermes FunctionContext)
    class FunctionContext {
    public:
        explicit FunctionContext(EnhancedIRGen* irgen, llvm::Function* function);
        ~FunctionContext();
        
        llvm::Function* getFunction() const { return function_; }
        ScopeContext& getScope() { return scope_; }
        
        // Label management for control flow
        void createLabel(const String& name, llvm::BasicBlock* breakTarget, llvm::BasicBlock* continueTarget);
        llvm::BasicBlock* getBreakTarget(const String& name) const;
        llvm::BasicBlock* getContinueTarget(const String& name) const;
        
    private:
        EnhancedIRGen* irgen_;
        llvm::Function* function_;
        ScopeContext scope_;
        std::unordered_map<String, std::pair<llvm::BasicBlock*, llvm::BasicBlock*>> labels_;
    };
    
    // Exception handling context
    class ExceptionContext {
    public:
        explicit ExceptionContext(EnhancedIRGen* irgen);
        ~ExceptionContext();
        
        void enterTryBlock(llvm::BasicBlock* tryBlock, llvm::BasicBlock* catchBlock);
        void enterCatchBlock(llvm::BasicBlock* catchBlock);
        void enterFinallyBlock(llvm::BasicBlock* finallyBlock);
        
    private:
        EnhancedIRGen* irgen_;
        std::stack<std::tuple<llvm::BasicBlock*, llvm::BasicBlock*, llvm::BasicBlock*>> tryStack_;
    };

private:
    DiagnosticEngine& diagnostics_;
    const TypeSystem& typeSystem_;
    
    // LLVM components
    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<llvm::Module> module_;
    std::unique_ptr<llvm::IRBuilder<>> builder_;
    
    // Context management
    std::stack<FunctionContext> functionContexts_;
    std::unique_ptr<ExceptionContext> exceptionContext_;
    
    // Type cache for performance
    std::unordered_map<String, llvm::Type*> typeCache_;
    
    // Built-in function declarations
    std::unordered_map<String, llvm::Function*> builtinFunctions_;
    
    // Helper methods
    void declareBuiltinFunctions();
    llvm::Function* getBuiltinFunction(const String& name);
    llvm::Type* getLLVMType(const shared_ptr<Type>& type);
    llvm::Value* createAlloca(llvm::Type* type, const String& name = "");
    void reportError(const String& message, const SourceLocation& location);
    void reportWarning(const String& message, const SourceLocation& location);
    
    // Optimization helpers
    bool isConstantExpression(const unique_ptr<Expression>& expr) const;
    llvm::Value* foldConstantExpression(const unique_ptr<Expression>& expr);
    
    // Generic type helpers
    std::vector<shared_ptr<Type>> resolveTypeArguments(const shared_ptr<Type>& genericType,
                                                      const std::vector<shared_ptr<Type>>& providedArgs);
    bool isGenericType(const shared_ptr<Type>& type) const;
    
    // Memory management
    void optimizeMemoryUsage();
    void eliminateDeadCode();
    void inlineSmallFunctions();
};

// Factory function
std::unique_ptr<EnhancedIRGen> createEnhancedIRGen(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem);

} // namespace tsc