#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/semantic/ModuleResolver.h"
#include "tsc/semantic/DependencyScanner.h"
#include "tsc/semantic/ModuleSymbolTable.h"
#include "tsc/semantic/CycleDetector.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/EnhancedErrorReporting.h"

namespace tsc {

// Forward declarations
class GenericConstraintChecker;

// Semantic analysis context for tracking state during analysis
class SemanticContext {
public:
    SemanticContext(SymbolTable& symbolTable, TypeSystem& typeSystem, DiagnosticEngine& diagnostics)
        : symbolTable_(symbolTable), typeSystem_(typeSystem), diagnostics_(diagnostics) {}
    
    SymbolTable& getSymbolTable() { return symbolTable_; }
    TypeSystem& getTypeSystem() { return typeSystem_; }
    DiagnosticEngine& getDiagnostics() { return diagnostics_; }
    
    // Context state
    
    shared_ptr<Type> getCurrentFunctionReturnType() const { return currentFunctionReturnType_; }
    void setCurrentFunctionReturnType(shared_ptr<Type> type) { currentFunctionReturnType_ = type; }
    
    bool isInLoop() const { return loopDepth_ > 0; }
    void enterLoop() { loopDepth_++; }
    void exitLoop() { if (loopDepth_ > 0) loopDepth_--; }
    
    bool isInSwitch() const { return switchDepth_ > 0; }
    void enterSwitch() { switchDepth_++; }
    void exitSwitch() { if (switchDepth_ > 0) switchDepth_--; }
    
    // Error tracking
    size_t getErrorCount() const { return errorCount_; }
    void incrementErrorCount() { errorCount_++; }

private:
    SymbolTable& symbolTable_;
    TypeSystem& typeSystem_;
    DiagnosticEngine& diagnostics_;
    
    shared_ptr<Type> currentFunctionReturnType_;
    int loopDepth_ = 0;
    int switchDepth_ = 0;
    size_t errorCount_ = 0;
};

// Main semantic analyzer class
class SemanticAnalyzer : public ASTVisitor {
public:
    explicit SemanticAnalyzer(DiagnosticEngine& diagnostics);
    ~SemanticAnalyzer();
    
    // Main analysis entry point
    bool analyze(Module& module);
    
    // Multi-module analysis entry point
    bool analyzeProject(const std::vector<String>& modulePaths);
    
    // Type information access
    shared_ptr<Type> getExpressionType(const Expression& expr) const;
    shared_ptr<Type> getDeclarationType(const Declaration& decl) const;
    Symbol* getSymbolForNode(const ASTNode& node) const;
    
    // Analysis results
    SymbolTable& getSymbolTable() { return *symbolTable_; }
    const TypeSystem& getTypeSystem() const { return *typeSystem_; }
    ModuleSymbolManager* getModuleSymbolManager() { return moduleSymbolManager_.get(); }
    
    // Cycle detection
    void runCycleDetection();
    bool hasCycleErrors() const;
    void printCycleResults() const;
    
    // RAII Analysis
    void analyzeDestructor(const DestructorDeclaration& destructor);
    void validateRAIIPatterns(const ClassDeclaration& classDecl);
    void suggestResourceCleanup(const ClassDeclaration& classDecl);
    void detectResourceLeaks(const ClassDeclaration& classDecl);
    void analyzeResourceOwnership(const ClassDeclaration& classDecl);
    void validateDestructorSafety(const DestructorDeclaration& destructor);
    bool analyzeDestructorCleanup(const DestructorDeclaration* destructor, const String& propertyName);
    
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
    void visit(ArrayAssignmentExpression& node) override;
    void visit(ConditionalExpression& node) override;
    void visit(CallExpression& node) override;
    void visit(ArrayLiteral& node) override;
    void visit(TemplateLiteral& node) override;
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
    
    void visit(Module& node) override;
    
    // Destructuring visitor methods
    void visit(DestructuringPattern& node) override;
    void visit(ArrayDestructuringPattern& node) override;
    void visit(ObjectDestructuringPattern& node) override;
    void visit(IdentifierPattern& node) override;
    void visit(DestructuringAssignment& node) override;

private:
    // Helper methods
    bool isInFunctionScope() const { return functionDepth_ > 0; }
    shared_ptr<Type> inferReturnType(const Statement& body);
    shared_ptr<Type> findMemberType(shared_ptr<Type> type, const String& memberName);
    void collectNestedFunctionDeclarations(const Statement& stmt);
    bool isValidIndexType(shared_ptr<Type> type) const;
    bool isArrayType(shared_ptr<Type> type) const;
    bool isNumberType(shared_ptr<Type> type) const;
    shared_ptr<Type> getArrayElementType(shared_ptr<Type> arrayType) const;
    shared_ptr<Type> createObjectTypeFromProperties(const std::vector<std::pair<String, shared_ptr<Type>>>& properties) const;
    void markCurrentFunctionAsCaptured();
    void markVariableAsCaptured(Symbol* symbol);
    DiagnosticEngine& diagnostics_;
    unique_ptr<SymbolTable> symbolTable_;
    unique_ptr<TypeSystem> typeSystem_;
    unique_ptr<SemanticContext> context_;
    unique_ptr<GenericConstraintChecker> constraintChecker_;
    unique_ptr<ModuleResolver> moduleResolver_;
    unique_ptr<DependencyScanner> dependencyScanner_;
    unique_ptr<ModuleSymbolManager> moduleSymbolManager_;
    
    // Module context tracking
    String currentModulePath_;
    unique_ptr<semantic::CycleDetector> cycleDetector_;
    
    // Function context tracking
    int functionDepth_ = 0;
    std::vector<FunctionDeclaration*> functionStack_;
    
    // Type information storage
    std::unordered_map<const Expression*, shared_ptr<Type>> expressionTypes_;
    std::unordered_map<const Declaration*, shared_ptr<Type>> declarationTypes_;
    std::unordered_map<const ASTNode*, Symbol*> nodeSymbols_;
    
    // Deferred super resolution
    struct DeferredSuperExpression {
        SuperExpression* expression;
        String className;
        SourceLocation location;
    };
    std::vector<DeferredSuperExpression> deferredSuperExpressions_;
    
    // Deferred property access on super
    struct DeferredSuperPropertyAccess {
        PropertyAccess* propertyAccess;
        String className;
        String memberName;
        SourceLocation location;
    };
    std::vector<DeferredSuperPropertyAccess> deferredSuperPropertyAccesses_;
    
    // Deferred call expressions on super property access
    struct DeferredSuperCallExpression {
        CallExpression* callExpression;
        PropertyAccess* propertyAccess;
        String className;
        String memberName;
        SourceLocation location;
    };
    std::vector<DeferredSuperCallExpression> deferredSuperCallExpressions_;
    
    // Analysis phases
    void performSymbolResolution(Module& module);
    void collectFunctionDeclarations(Module& module);
    void performTypeChecking(Module& module);
    void performFlowAnalysis(Module& module);
    
    // Symbol resolution helpers
    void declareSymbol(const String& name, SymbolKind kind, shared_ptr<Type> type, 
                      const SourceLocation& location, ASTNode* declaration = nullptr);
    Symbol* resolveSymbol(const String& name, const SourceLocation& location);
    
    // Class member lookup (with inheritance)
    shared_ptr<Type> findInterfaceMember(const InterfaceType& interfaceType, const String& memberName);
    shared_ptr<Type> findClassMember(const ClassType& classType, const String& memberName);
    
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
    shared_ptr<Type> inferFunctionReturnType(const FunctionDeclaration& decl);
    
    // Type resolution
    shared_ptr<Type> resolveType(shared_ptr<Type> type);
    
    // Inheritance resolution
    void resolveInheritance(Module& module);
    void resolveDeferredSuperExpressions();
    void resolveDeferredSuperPropertyAccesses();
    void resolveDeferredSuperCallExpressions();
    
    // Error reporting
    void reportError(const String& message, const SourceLocation& location);
    void reportWarning(const String& message, const SourceLocation& location);
    void reportTypeError(const String& expected, const String& actual, const SourceLocation& location);
    void reportUndefinedSymbol(const String& name, const SourceLocation& location);
    void reportRedefinitionError(const String& name, const SourceLocation& location, 
                                const SourceLocation& originalLocation);
    
    // Utilities
    void setExpressionType(const Expression& expr, shared_ptr<Type> type);
    void setDeclarationType(const Declaration& decl, shared_ptr<Type> type);
    void setNodeSymbol(const ASTNode& node, Symbol* symbol);
    bool isValidAssignment(const Type& from, const Type& to) const;
    String formatType(const Type& type) const;
    
    // Scope management helpers
    void enterScope(Scope::ScopeType type, const String& name = "");
    void exitScope();
    
    // Built-in function and variable setup
    void setupBuiltinEnvironment();
    
    // Generic constraint validation helpers
    FunctionDeclaration* getFunctionDeclaration(const String& functionName);
    bool validateFunctionArguments(const CallExpression& call, const FunctionType& functionType);
    bool validateGenericFunctionCall(const CallExpression& call, const FunctionDeclaration& funcDecl, const FunctionType& functionType);
    bool validateGenericInterfaceInheritance(GenericType* genericType, const SourceLocation& location);
    bool validateInterfaceInheritance(shared_ptr<Type> extendedType, const SourceLocation& location);
    bool checkCircularInterfaceInheritance(shared_ptr<Type> interfaceType, const SourceLocation& location);
    
    // ARC Memory Management Analysis
    void analyzeOwnership(const Expression& expr);
    void analyzeMoveSemantics(const MoveExpression& moveExpr);
    void analyzeAssignmentOwnership(const AssignmentExpression& assignExpr);
    void detectCycles(const ClassDeclaration& classDecl);
    void suggestWeakReferences(const ClassDeclaration& classDecl);
    bool isARCManaged(const Type& type) const;
    bool isMoveable(const Type& type) const;
    bool hasDestructor(const Type& type) const;

private:
    unique_ptr<EnhancedErrorReporting> errorReporter_;
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