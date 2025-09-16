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
// Debug info includes - will be added when needed
// #include "llvm/IR/DIBuilder.h"
// #include "llvm/IR/DebugInfo.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
// #include "llvm/Support/Host.h"  // Not available in this LLVM version
// Advanced optimization includes - will be added later
// #include "llvm/Passes/PassBuilder.h"
// #include "llvm/Passes/StandardInstrumentations.h"
// #include "llvm/Analysis/TargetTransformInfo.h"
// #include "llvm/Transforms/Scalar.h"
// #include "llvm/Transforms/Utils.h"

#include <unordered_map>
#include <stack>
#include <iostream>

namespace tsc {

// Forward declarations
class CompilerOptions;

// Code generation context for tracking state during generation
class CodeGenContext {
public:
    CodeGenContext(llvm::LLVMContext& llvmContext, llvm::Module& module, 
                   llvm::IRBuilder<>& builder, DiagnosticEngine& diagnostics);
    
    // Symbol value tracking
    void setSymbolValue(const String& name, llvm::Value* value);
    llvm::Value* getSymbolValue(const String& name) const;
    bool hasSymbol(const String& name) const;
    
    // Function context
    void enterFunction(llvm::Function* function);
    void exitFunction();
    llvm::Function* getCurrentFunction() const;
    
    // Class context
    void enterClass(const String& className);
    void exitClass();
    String getCurrentClassName() const;
    
    // Block context for break/continue
    void enterLoop(llvm::BasicBlock* continueBlock, llvm::BasicBlock* breakBlock);
    void exitLoop();
    llvm::BasicBlock* getCurrentLoopContinueBlock() const;
    llvm::BasicBlock* getCurrentLoopBreakBlock() const;
    
    // Switch context for break statements
    void enterSwitch(llvm::BasicBlock* exitBlock);
    void exitSwitch();
    llvm::BasicBlock* getCurrentSwitchExitBlock() const;
    
    
    // Scope management
    void enterScope();
    void exitScope();
    
    // ARC object tracking
    void addARCManagedObject(const String& name, llvm::Value* object, const String& className);
    void generateScopeCleanup(class LLVMCodeGen* codeGen);
    
    // Error handling
    void reportError(const String& message, const SourceLocation& location);
    size_t getErrorCount() const { return errorCount_; }

private:
    llvm::LLVMContext& llvmContext_;
    llvm::Module& module_;
    llvm::IRBuilder<>& builder_;
    DiagnosticEngine& diagnostics_;
    unique_ptr<EnhancedErrorReporting> errorReporter_;
    
    // Symbol table for LLVM values
    std::vector<std::unordered_map<String, llvm::Value*>> symbolStack_;
    
    // Function context stack
    std::stack<llvm::Function*> functionStack_;
    
    // Performance optimization: struct type cache
    std::unordered_map<String, llvm::StructType*> structTypeCache_;
    
    // Class context stack
    std::stack<String> classStack_;
    
    // Loop context for break/continue
    struct LoopContext {
        llvm::BasicBlock* continueBlock;
        llvm::BasicBlock* breakBlock;
    };
    std::stack<LoopContext> loopStack_;
    
    // Switch context for break statements
    struct SwitchContext {
        llvm::BasicBlock* exitBlock;
    };
    std::stack<SwitchContext> switchStack_;
    
    // ARC object tracking
    struct ARCManagedObject {
        String name;
        llvm::Value* object;
        String className;
    };
    std::vector<std::vector<ARCManagedObject>> arcObjectStack_;
    
    size_t errorCount_ = 0;
};

// LLVM code generator
class LLVMCodeGen : public ASTVisitor {
public:
    explicit LLVMCodeGen(DiagnosticEngine& diagnostics, const CompilerOptions& options);
    ~LLVMCodeGen();
    
    // Nested helper classes for enhanced IR generation
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
            for (auto it = typeStack_.rbegin(); it != typeStack_.rend(); ++it) {
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
    
    class BuiltinFunctionRegistry {
    private:
        std::unordered_map<String, llvm::Function*> builtinFunctions_;
        llvm::Module* module_;
        llvm::LLVMContext* context_;
        
    public:
        BuiltinFunctionRegistry(llvm::Module* module, llvm::LLVMContext* context)
            : module_(module), context_(context) {
            // NOTE: We don't call registerBuiltinFunctions() to avoid unused external declarations
            // Runtime functions are created on-demand when they're actually used during code generation
        }
        
        llvm::Function* getBuiltinFunction(const String& name) {
            auto it = builtinFunctions_.find(name);
            return it != builtinFunctions_.end() ? it->second : nullptr;
        }
        
        bool isBuiltinFunction(const String& name) const {
            return builtinFunctions_.find(name) != builtinFunctions_.end();
        }
        
    private:
        void registerBuiltinFunctions();
        void registerConsoleFunctions();
        void registerMathFunctions();
        void registerStringFunctions();
        void registerArrayFunctions();
    };
    
    class IRAllocator {
    private:
        llvm::BumpPtrAllocator arena_;
        size_t totalAllocated_ = 0;
        size_t allocationCount_ = 0;
        
    public:
        template<typename T, typename... Args>
        T* allocate(Args&&... args) {
            void* ptr = arena_.Allocate(sizeof(T), alignof(T));
            if (!ptr) {
                throw std::bad_alloc();
            }
            
            totalAllocated_ += sizeof(T);
            allocationCount_++;
            
            return new(ptr) T(std::forward<Args>(args)...);
        }
        
        template<typename T>
        void deallocate(T* ptr) {
            if (ptr) {
                ptr->~T();
                // Note: BumpPtrAllocator doesn't support individual deallocation
                // Memory is freed when the allocator is destroyed
            }
        }
        
        void reset() {
            arena_.Reset();
            totalAllocated_ = 0;
            allocationCount_ = 0;
        }
        
        size_t getTotalAllocated() const { return totalAllocated_; }
        size_t getAllocationCount() const { return allocationCount_; }
        
        // Get memory statistics
        struct MemoryStats {
            size_t totalAllocated;
            size_t allocationCount;
            double averageAllocationSize;
            size_t peakMemoryUsage;
        };
        
        MemoryStats getStats() const {
            MemoryStats stats;
            stats.totalAllocated = totalAllocated_;
            stats.allocationCount = allocationCount_;
            stats.averageAllocationSize = allocationCount_ > 0 ? 
                static_cast<double>(totalAllocated_) / allocationCount_ : 0.0;
            stats.peakMemoryUsage = arena_.getBytesAllocated();
            return stats;
        }
        
        // Memory usage reporting
        void reportMemoryUsage() const;
    };
    
    // Main code generation interface
    bool generateCode(Module& module, SymbolTable& symbolTable, 
                     const TypeSystem& typeSystem);
    
    // Enhanced type generation with caching
    llvm::Type* generateType(const shared_ptr<Type>& type);
    llvm::Type* generatePrimitiveType(const shared_ptr<Type>& type);
    llvm::Type* generateArrayType(const shared_ptr<Type>& type);
    llvm::Type* generateObjectType(const shared_ptr<Type>& type);
    llvm::Type* generateFunctionType(const shared_ptr<Type>& type);
    llvm::Type* generateUnionType(const shared_ptr<Type>& type);
    llvm::Type* generateGenericType(const shared_ptr<Type>& type);
    
    // Enhanced control flow generation
    void generateIfStatement(const unique_ptr<IfStatement>& stmt);
    void generateWhileStatement(const unique_ptr<WhileStatement>& stmt);
    void generateForStatement(const unique_ptr<ForStatement>& stmt);
    void generateTryStatement(const unique_ptr<TryStatement>& stmt);
    
    // Generic type specialization
    llvm::Type* generateGenericType(const shared_ptr<GenericType>& genericType, 
                                   const std::vector<llvm::Type*>& typeArguments);
    llvm::Type* generateSpecializedClass(const shared_ptr<GenericType>& genericType,
                                        const std::vector<llvm::Type*>& typeArguments);
    llvm::Type* specializeType(const shared_ptr<Type>& type,
                              const std::unordered_map<String, llvm::Type*>& typeMap);
    
    // Optimization passes
    void runOptimizationPasses();
    void runFunctionOptimizations();
    void runModuleOptimizations();
    void runMemoryOptimizations();
    void runARCOptimizations();
    
    // ARC optimization methods
    void eliminateRedundantRetainRelease();
    void optimizeWeakReferenceAccess();
    void removeRedundantARCCalls();
    void optimizeMemoryLayout();
    bool canEliminateRetainReleasePair(llvm::CallInst* retainCall, llvm::CallInst* releaseCall);
    void optimizeWeakAccessPattern(llvm::CallInst* weakCall);
    bool isRedundantARCCall(llvm::CallInst* callInst, const String& functionName);
    bool isARCManagedStruct(llvm::StructType* structType);
    void optimizeStructLayout(llvm::StructType* structType);
    
    // Memory management
    void reportMemoryUsage() const;
    IRAllocator::MemoryStats getMemoryStats() const;
    
    // Output generation
    bool emitLLVMIR(const String& filename) const;
    bool emitObjectFile(const String& filename) const;
    bool emitAssembly(const String& filename) const;
    
    // Access to generated code
    llvm::Module* getLLVMModule() const { return module_.get(); }
    String getLLVMIRString() const;
    
    // Multi-module compilation control
    void setGenerateMainFunction(bool generate) { generateMainFunction_ = generate; }
    
    // Nested function generation
    void generateNestedFunction(const FunctionDeclaration& node);
    
    // Closure support
    llvm::StructType* createClosureStructType(const std::vector<Symbol*>& capturedVariables);
    llvm::Value* createClosureEnvironment(const std::vector<Symbol*>& capturedVariables);
    llvm::Value* loadCapturedVariable(const String& varName, llvm::Value* closureEnv);
    void storeCapturedVariable(const String& varName, llvm::Value* value, llvm::Value* closureEnv);
    bool isCapturedVariable(const String& varName, const FunctionDeclaration& currentFunction);
    String generateClosureTypeKey(const std::vector<Symbol*>& capturedVariables) const;
    
    // Scope navigation helpers
    Scope* findDeepestChildScope(Scope* parentScope);
    int getScopeDepth(Scope* scope);
    
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
    std::unique_ptr<BuiltinFunctionRegistry> builtinRegistry_;
    std::unique_ptr<IRAllocator> irAllocator_;
    
    // Multi-module compilation control
    bool generateMainFunction_ = true;
    
    // Type caching system
    std::unordered_map<String, llvm::Type*> typeCache_;
    std::unordered_map<String, llvm::FunctionType*> functionTypeCache_;
    std::unordered_map<String, llvm::StructType*> structTypeCache_;
    
    // Generic type handling
    std::unordered_map<String, llvm::Type*> specializedTypes_;
    std::unordered_map<String, std::vector<llvm::Type*>> genericTypeMap_;
    
    // Debug information builder - disabled for now
    // std::unique_ptr<llvm::DIBuilder> debugBuilder_;
    // llvm::DICompileUnit* debugCompileUnit_;
    // std::stack<llvm::DIScope*> debugScopeStack_;
    
    // Type system integration
    SymbolTable* symbolTable_;
    const TypeSystem* typeSystem_;
    
    // Current expression value (for visitor pattern)
    llvm::Value* currentValue_;
    
    // Built-in function declarations
    std::unordered_map<String, llvm::Function*> builtinFunctions_;
    
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
    
    // Deferred external symbol references (for Infinity, NaN, etc.)
    std::unordered_map<String, llvm::GlobalVariable*> deferredExternalSymbols_;
    
    // Type mapping from TypeScript to LLVM
    llvm::Type* mapTypeScriptTypeToLLVM(const Type& type);
    llvm::Type* getNumberType() const;
    llvm::Type* getStringType() const;
    llvm::Type* getBooleanType() const;
    llvm::Type* getVoidType() const;
    llvm::Type* getAnyType() const;
    llvm::Type* convertFunctionTypeToLLVM(const FunctionType& functionType);
    llvm::Value* convertValueToType(llvm::Value* value, llvm::Type* targetType);
    
    // Memory management functions
    llvm::Function* getOrCreateMallocFunction();
    llvm::Function* getOrCreateFreeFunction();
    
    // ARC Runtime functions
    llvm::Function* getOrCreateARCRetainFunction();
public:
    llvm::Function* getOrCreateARCReleaseFunction();
    llvm::Function* getOrCreatePointerToStringFunction();
    llvm::Function* getOrCreateARCAllocFunction();
    
    // ARC helper functions
    bool isARCManagedType(shared_ptr<Type> type) const;
    void generateAutomaticCleanup(const String& className);
    
    // Type conversion
    llvm::Type* convertTypeToLLVM(shared_ptr<Type> type);
    
    // Performance optimization: struct type caching
    llvm::StructType* getOrCreateStructType(const std::vector<llvm::Type*>& fieldTypes);
    
    // Switch context for break statements
    void enterSwitch(llvm::BasicBlock* exitBlock);
    void exitSwitch();
    llvm::BasicBlock* getCurrentSwitchExitBlock() const;
    
    // Generic type monomorphization
    llvm::Type* createMonomorphizedType(const GenericType& genericType);
    String generateMangledName(const GenericType& genericType);
    llvm::StructType* createMonomorphizedStruct(const GenericType& genericType);
    String generateMangledMethodName(const GenericType& genericType, const String& methodName);
    void generateMonomorphizedMethods(const GenericType& genericType, Symbol* classSymbol);
    void generateMonomorphizedMethod(const MethodDeclaration& method, const GenericType& genericType, const String& mangledName);
    
    // Generic method lookup for constrained types
    llvm::Function* genericMethodLookup(const String& methodName, shared_ptr<Type> objectType, const SourceLocation& location);
    llvm::Function* createBuiltinMethodFunction(const String& methodName, shared_ptr<Type> objectType, const SourceLocation& location);
    
    // Value operations
    llvm::Value* createNumberLiteral(double value);
    llvm::Value* createStringLiteral(const String& value);
    llvm::Value* createBooleanLiteral(bool value);
    llvm::Value* createNullValue(llvm::Type* type);
    llvm::Value* createDefaultValue(llvm::Type* type);
    llvm::Function* getOrCreatePrintFunction();
    
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
    
    // Function generation
    llvm::Function* generateFunctionDeclaration(const FunctionDeclaration& funcDecl);
    void generateFunctionBody(llvm::Function* function, const FunctionDeclaration& funcDecl);
    bool hasReturnStatements(const FunctionDeclaration& funcDecl);
    bool hasReturnStatementsWithValues(const FunctionDeclaration& funcDecl);
    
    // Unreachable block handling
    void ensureBlockTerminators(llvm::Function* function);
    void addUnreachableTerminator(llvm::BasicBlock* block);
    
    // Built-in functions
    void declareBuiltinFunctions();
    void declareBuiltinGlobals();
    llvm::Value* createDeferredExternalSymbolMarker(llvm::GlobalVariable* externalVar, const String& name);
    llvm::Function* getOrCreateStringConcatFunction();
    llvm::Function* getOrCreateNumberToStringFunction();
    llvm::Function* getOrCreateBooleanToStringFunction();
    llvm::Function* getOrCreateObjectToStringFunction();
    llvm::Function* getOrCreateThrowFunction();
    llvm::Function* getOrCreateRethrowFunction();
    
    // Exception handler management
    llvm::Function* getOrCreateSetupExceptionHandlerFunction();
    llvm::Function* getOrCreateCleanupExceptionHandlerFunction();
    llvm::Function* getOrCreateTryExceptionHandlerFunction();
    llvm::Function* getOrCreateGetExceptionTypeFunction();
    llvm::Function* getOrCreateGetExceptionMessageFunction();
    llvm::Function* getOrCreateClearExceptionFunction();
    llvm::Function* getOrCreateGetExceptionFunction();
    
    // Exception types
    llvm::StructType* getOrCreateExceptionHandlerType();
    llvm::StructType* getOrCreateExceptionType();
    
    // Memory management
    llvm::Value* allocateVariable(const String& name, llvm::Type* type, const SourceLocation& location);
    llvm::Value* loadVariable(const String& name, const SourceLocation& location);
    void storeVariable(const String& name, llvm::Value* value, const SourceLocation& location);
    
    // Control flow
    llvm::BasicBlock* createBasicBlock(const String& name = "");
    void generateBranch(llvm::BasicBlock* block);
    void generateConditionalBranch(llvm::Value* condition, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock);
    
    // Debug information - disabled for now
    // void setupDebugInfo(const String& filename);
    // llvm::DIType* createDebugType(const Type& type);
    // void emitDebugLocation(const SourceLocation& location);
    // void enterDebugScope(llvm::DIScope* scope);
    // void exitDebugScope();
    
    // Optimization
    void optimizeModule();
    
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

private:
    unique_ptr<EnhancedErrorReporting> errorReporter_;
    unique_ptr<llvm::LLVMContext> llvmContext_;
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

} // namespace tsc