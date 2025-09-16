#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/semantic/GenericConstraintChecker.h"
#include "tsc/semantic/ModuleResolver.h"
#include "tsc/semantic/DependencyScanner.h"
#include "tsc/semantic/ModuleSymbolTable.h"
#include "tsc/semantic/CycleDetector.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/EnhancedErrorReporting.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace tsc {
namespace semantic {

// Forward declarations
class SymbolResolutionEngine;
class TypeCheckingEngine;
class FlowAnalysisEngine;
class InheritanceResolver;

// Semantic analysis context
class SemanticContext {
public:
    SemanticContext(SymbolTable& symbolTable, TypeSystem& typeSystem, DiagnosticEngine& diagnostics);
    
    SymbolTable& getSymbolTable() { return symbolTable_; }
    TypeSystem& getTypeSystem() { return typeSystem_; }
    DiagnosticEngine& getDiagnostics() { return diagnostics_; }
    
    void reportError(const String& message, const SourceLocation& location);
    void reportWarning(const String& message, const SourceLocation& location);
    
    size_t getErrorCount() const { return errorCount_; }
    size_t getWarningCount() const { return warningCount_; }

private:
    SymbolTable& symbolTable_;
    TypeSystem& typeSystem_;
    DiagnosticEngine& diagnostics_;
    size_t errorCount_ = 0;
    size_t warningCount_ = 0;
};

// Main semantic analyzer - orchestrates all semantic analysis phases
class SemanticAnalyzer : public ASTVisitor {
public:
    explicit SemanticAnalyzer(DiagnosticEngine& diagnostics);
    ~SemanticAnalyzer();
    
    // Main analysis interface
    bool analyze(Module& module);
    bool analyzeProject(const std::vector<String>& modulePaths);
    
    // Access to components
    SymbolTable* getSymbolTable() const { return symbolTable_.get(); }
    TypeSystem* getTypeSystem() const { return typeSystem_.get(); }
    SemanticContext* getContext() const { return context_.get(); }
    
    // Error reporting
    void reportError(const String& message, const SourceLocation& location);
    void reportWarning(const String& message, const SourceLocation& location);
    
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
    void visit(ExpressionStatement& node) override;
    void visit(BlockStatement& node) override;
    void visit(ReturnStatement& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(DoWhileStatement& node) override;
    void visit(ForStatement& node) override;
    void visit(ForOfStatement& node) override;
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
    void visit(Module& module) override;
    
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
    
    // Destructuring visitor methods
    void visit(DestructuringPattern& node) override;
    void visit(ArrayDestructuringPattern& node) override;
    void visit(ObjectDestructuringPattern& node) override;
    void visit(IdentifierPattern& node) override;
    void visit(DestructuringAssignment& node) override;

private:
    DiagnosticEngine& diagnostics_;
    
    // Core components
    std::unique_ptr<SymbolTable> symbolTable_;
    std::unique_ptr<TypeSystem> typeSystem_;
    std::unique_ptr<SemanticContext> context_;
    std::unique_ptr<GenericConstraintChecker> constraintChecker_;
    std::unique_ptr<ModuleResolver> moduleResolver_;
    std::unique_ptr<DependencyScanner> dependencyScanner_;
    std::unique_ptr<ModuleSymbolManager> moduleSymbolManager_;
    std::unique_ptr<CycleDetector> cycleDetector_;
    std::unique_ptr<EnhancedErrorReporting> errorReporter_;
    
    // Analysis engines
    std::unique_ptr<SymbolResolutionEngine> symbolResolutionEngine_;
    std::unique_ptr<TypeCheckingEngine> typeCheckingEngine_;
    std::unique_ptr<FlowAnalysisEngine> flowAnalysisEngine_;
    std::unique_ptr<InheritanceResolver> inheritanceResolver_;
    
    // Analysis phases
    void performSymbolResolution(Module& module);
    void performTypeChecking(Module& module);
    void performFlowAnalysis(Module& module);
    
    // Symbol resolution helpers
    void collectFunctionDeclarations(Module& module);
    void resolveInheritance(Module& module);
    void resolveDeferredSuperExpressions();
    void resolveDeferredSuperPropertyAccesses();
    void resolveDeferredSuperCallExpressions();
    
    // Builtin environment setup
    void setupBuiltinEnvironment();
    
    // Helper methods
    Symbol* findInterfaceMember(const InterfaceType& interfaceType, const String& memberName);
};

} // namespace semantic
} // namespace tsc