#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/utils/DiagnosticEngine.h"

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
    
    // Block context for break/continue
    void enterLoop(llvm::BasicBlock* continueBlock, llvm::BasicBlock* breakBlock);
    void exitLoop();
    llvm::BasicBlock* getCurrentContinueBlock() const;
    llvm::BasicBlock* getCurrentBreakBlock() const;
    
    // Scope management
    void enterScope();
    void exitScope();
    
    // Error handling
    void reportError(const String& message, const SourceLocation& location);
    size_t getErrorCount() const { return errorCount_; }

private:
    llvm::LLVMContext& llvmContext_;
    llvm::Module& module_;
    llvm::IRBuilder<>& builder_;
    DiagnosticEngine& diagnostics_;
    
    // Symbol table for LLVM values
    std::vector<std::unordered_map<String, llvm::Value*>> symbolStack_;
    
    // Function context stack
    std::stack<llvm::Function*> functionStack_;
    
    // Loop context for break/continue
    struct LoopContext {
        llvm::BasicBlock* continueBlock;
        llvm::BasicBlock* breakBlock;
    };
    std::stack<LoopContext> loopStack_;
    
    size_t errorCount_ = 0;
};

// LLVM code generator
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
    // void visit(TemplateLiteral& node) override;  // TODO: Template literals
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
    void visit(ClassDeclaration& node) override;
    void visit(InterfaceDeclaration& node) override;
    void visit(EnumMember& node) override;
    void visit(EnumDeclaration& node) override;
    void visit(TypeAliasDeclaration& node) override;
    
    void visit(Module& module) override;

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
    
    // Type conversion
    llvm::Type* convertTypeToLLVM(shared_ptr<Type> type);
    
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
    
    // Function generation
    llvm::Function* generateFunctionDeclaration(const FunctionDeclaration& funcDecl);
    void generateFunctionBody(llvm::Function* function, const FunctionDeclaration& funcDecl);
    bool hasReturnStatements(const FunctionDeclaration& funcDecl);
    
    // Unreachable block handling
    void ensureBlockTerminators(llvm::Function* function);
    void addUnreachableTerminator(llvm::BasicBlock* block);
    
    // Built-in functions
    void declareBuiltinFunctions();
    llvm::Function* getOrCreatePrintFunction();
    llvm::Function* getOrCreateStringConcatFunction();
    llvm::Function* getOrCreateNumberToStringFunction();
    llvm::Function* getOrCreateBooleanToStringFunction();
    llvm::Function* getOrCreateThrowFunction();
    llvm::Function* getOrCreateRethrowFunction();
    
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
    void runOptimizationPasses();
    
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