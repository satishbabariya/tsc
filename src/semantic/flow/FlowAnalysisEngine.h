#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SymbolTable.h"
#include "tsc/semantic/TypeSystem.h"

#include <unordered_set>
#include <unordered_map>

namespace tsc {
namespace semantic {

// Forward declarations
class SemanticContext;

// Control flow analysis engine for analyzing program flow
class FlowAnalysisEngine {
public:
    explicit FlowAnalysisEngine(SemanticContext& context);
    
    // Main flow analysis interface
    void performFlowAnalysis(Module& module);
    
    // Control flow analysis
    void analyzeControlFlow(ASTNode& node);
    void analyzeFunctionFlow(const FunctionDeclaration& function);
    void analyzeClassFlow(const ClassDeclaration& classDecl);
    
    // Reachability analysis
    void analyzeReachability(ASTNode& node);
    bool isReachable(ASTNode& node);
    void markUnreachable(ASTNode& node);
    
    // Variable usage analysis
    void analyzeVariableUsage(ASTNode& node);
    void checkUnusedVariables();
    void checkUninitializedVariables();
    
    // Return analysis
    void analyzeReturnPaths(const FunctionDeclaration& function);
    bool hasReturnStatement(const FunctionDeclaration& function);
    bool hasReturnValue(const FunctionDeclaration& function);
    bool allPathsReturn(const FunctionDeclaration& function);
    
    // Break/Continue analysis
    void analyzeBreakContinue(ASTNode& node);
    bool isBreakValid(const BreakStatement& breakStmt);
    bool isContinueValid(const ContinueStatement& continueStmt);
    
    // Exception flow analysis
    void analyzeExceptionFlow(ASTNode& node);
    void checkUncaughtExceptions();
    void analyzeTryCatchFlow(const TryStatement& tryStmt);

private:
    SemanticContext& context_;
    
    // Analysis state
    std::unordered_set<ASTNode*> unreachableNodes_;
    std::unordered_map<Symbol*, bool> variableUsed_;
    std::unordered_map<Symbol*, bool> variableInitialized_;
    
    // Helper methods
    void analyzeNodeFlow(ASTNode& node);
    void analyzeExpressionFlow(ASTNode& expression);
    void analyzeStatementFlow(ASTNode& statement);
    void analyzeBlockFlow(const BlockStatement& block);
    
    // Variable tracking
    void trackVariableUsage(Symbol* symbol, bool isWrite);
    void trackVariableInitialization(Symbol* symbol);
    
    // Control flow helpers
    void analyzeConditionalFlow(const IfStatement& ifStmt);
    void analyzeLoopFlow(const WhileStatement& whileStmt);
    void analyzeLoopFlow(const ForStatement& forStmt);
    void analyzeSwitchFlow(const SwitchStatement& switchStmt);
};

} // namespace semantic
} // namespace tsc