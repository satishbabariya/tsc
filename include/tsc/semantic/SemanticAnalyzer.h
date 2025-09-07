#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/utils/DiagnosticEngine.h"

namespace tsc {

// Semantic analysis context for tracking state during analysis
class SemanticContext {
public:
    SemanticContext(SymbolTable& symbolTable, TypeSystem& typeSystem, DiagnosticEngine& diagnostics)
        : symbolTable_(symbolTable), typeSystem_(typeSystem), diagnostics_(diagnostics) {}
    
    SymbolTable& getSymbolTable() { return symbolTable_; }
    TypeSystem& getTypeSystem() { return typeSystem_; }
    DiagnosticEngine& getDiagnostics() { return diagnostics_; }
    
    // Context state
    bool isInFunction() const { return inFunction_; }
    void setInFunction(bool inFunction) { inFunction_ = inFunction; }
    
    shared_ptr<Type> getCurrentFunctionReturnType() const { return currentFunctionReturnType_; }
    void setCurrentFunctionReturnType(shared_ptr<Type> type) { currentFunctionReturnType_ = type; }
    
    bool isInLoop() const { return loopDepth_ > 0; }
    void enterLoop() { loopDepth_++; }
    void exitLoop() { if (loopDepth_ > 0) loopDepth_--; }
    
    // Error tracking
    size_t getErrorCount() const { return errorCount_; }
    void incrementErrorCount() { errorCount_++; }

private:
    SymbolTable& symbolTable_;
    TypeSystem& typeSystem_;
    DiagnosticEngine& diagnostics_;
    
    bool inFunction_ = false;
    shared_ptr<Type> currentFunctionReturnType_;
    int loopDepth_ = 0;
    size_t errorCount_ = 0;
};

// Main semantic analyzer class
class SemanticAnalyzer : public ASTVisitor {
public:
    explicit SemanticAnalyzer(DiagnosticEngine& diagnostics);
    ~SemanticAnalyzer();
    
    // Main analysis entry point
    bool analyze(Module& module);
    
    // Type information access
    shared_ptr<Type> getExpressionType(const Expression& expr) const;
    Symbol* getSymbolForNode(const ASTNode& node) const;
    
    // Analysis results
    const SymbolTable& getSymbolTable() const { return *symbolTable_; }
    const TypeSystem& getTypeSystem() const { return *typeSystem_; }
    
    // Visitor interface implementation
    void visit(NumericLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(NullLiteral& node) override;
    void visit(Identifier& node) override;
    void visit(BinaryExpression& node) override;
    void visit(UnaryExpression& node) override;
    void visit(AssignmentExpression& node) override;
    void visit(CallExpression& node) override;
    
    void visit(ExpressionStatement& node) override;
    void visit(BlockStatement& node) override;
    void visit(VariableDeclaration& node) override;
    void visit(FunctionDeclaration& node) override;
    
    void visit(Module& node) override;

private:
    DiagnosticEngine& diagnostics_;
    unique_ptr<SymbolTable> symbolTable_;
    unique_ptr<TypeSystem> typeSystem_;
    unique_ptr<SemanticContext> context_;
    
    // Type information storage
    std::unordered_map<const Expression*, shared_ptr<Type>> expressionTypes_;
    std::unordered_map<const ASTNode*, Symbol*> nodeSymbols_;
    
    // Analysis phases
    void performSymbolResolution(Module& module);
    void performTypeChecking(Module& module);
    void performFlowAnalysis(Module& module);
    
    // Symbol resolution helpers
    void declareSymbol(const String& name, SymbolKind kind, shared_ptr<Type> type, 
                      const SourceLocation& location, ASTNode* declaration = nullptr);
    Symbol* resolveSymbol(const String& name, const SourceLocation& location);
    
    // Type checking helpers
    void checkAssignment(const Expression& left, const Expression& right, 
                        const SourceLocation& location);
    void checkFunctionCall(const CallExpression& call);
    void checkBinaryOperation(BinaryExpression& expr);
    void checkUnaryOperation(UnaryExpression& expr);
    
    // Type inference
    shared_ptr<Type> inferExpressionType(Expression& expr);
    shared_ptr<Type> inferVariableType(const VariableDeclaration& decl);
    shared_ptr<Type> inferFunctionType(const FunctionDeclaration& decl);
    
    // Error reporting
    void reportError(const String& message, const SourceLocation& location);
    void reportWarning(const String& message, const SourceLocation& location);
    void reportTypeError(const String& expected, const String& actual, const SourceLocation& location);
    void reportUndefinedSymbol(const String& name, const SourceLocation& location);
    void reportRedefinitionError(const String& name, const SourceLocation& location, 
                                const SourceLocation& originalLocation);
    
    // Utilities
    void setExpressionType(const Expression& expr, shared_ptr<Type> type);
    void setNodeSymbol(const ASTNode& node, Symbol* symbol);
    bool isValidAssignment(const Type& from, const Type& to) const;
    String formatType(const Type& type) const;
    
    // Scope management helpers
    void enterScope(Scope::ScopeType type, const String& name = "");
    void exitScope();
    
    // Built-in function and variable setup
    void setupBuiltinEnvironment();
};

// Semantic analysis result
struct SemanticAnalysisResult {
    bool success = false;
    size_t errorCount = 0;
    size_t warningCount = 0;
    String errorMessage;
    unique_ptr<SymbolTable> symbolTable;
    unique_ptr<TypeSystem> typeSystem;
};

// Factory function
unique_ptr<SemanticAnalyzer> createSemanticAnalyzer(DiagnosticEngine& diagnostics);

} // namespace tsc