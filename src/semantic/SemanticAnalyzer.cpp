#include "tsc/semantic/SemanticAnalyzer.h"
#include <iostream>

namespace tsc {

SemanticAnalyzer::SemanticAnalyzer(DiagnosticEngine& diagnostics)
    : diagnostics_(diagnostics) {
    symbolTable_ = make_unique<SymbolTable>();
    typeSystem_ = make_unique<TypeSystem>();
    context_ = make_unique<SemanticContext>(*symbolTable_, *typeSystem_, diagnostics_);
    
    setupBuiltinEnvironment();
}

SemanticAnalyzer::~SemanticAnalyzer() = default;

bool SemanticAnalyzer::analyze(Module& module) {
    try {
        // Multi-phase semantic analysis
        performSymbolResolution(module);
        performTypeChecking(module);
        performFlowAnalysis(module);
        
        // Check for unused symbols
        auto unusedSymbols = symbolTable_->getUnusedSymbols();
        for (const auto& symbol : unusedSymbols) {
            reportWarning("Unused " + symbol->toString(), symbol->getLocation());
        }
        
        return context_->getErrorCount() == 0;
        
    } catch (const std::exception& e) {
        reportError("Internal semantic analysis error: " + String(e.what()), SourceLocation());
        return false;
    }
}

shared_ptr<Type> SemanticAnalyzer::getExpressionType(const Expression& expr) const {
    auto it = expressionTypes_.find(&expr);
    return it != expressionTypes_.end() ? it->second : typeSystem_->getErrorType();
}

Symbol* SemanticAnalyzer::getSymbolForNode(const ASTNode& node) const {
    auto it = nodeSymbols_.find(&node);
    return it != nodeSymbols_.end() ? it->second : nullptr;
}

// Visitor implementations
void SemanticAnalyzer::visit(NumericLiteral& node) {
    auto type = typeSystem_->getNumberType();
    setExpressionType(node, type);
}

void SemanticAnalyzer::visit(StringLiteral& node) {
    auto type = typeSystem_->getStringType();
    setExpressionType(node, type);
}

void SemanticAnalyzer::visit(BooleanLiteral& node) {
    auto type = typeSystem_->getBooleanType();
    setExpressionType(node, type);
}

void SemanticAnalyzer::visit(NullLiteral& node) {
    auto type = typeSystem_->getNullType();
    setExpressionType(node, type);
}

void SemanticAnalyzer::visit(Identifier& node) {
    Symbol* symbol = resolveSymbol(node.getName(), node.getLocation());
    if (symbol) {
        symbol->markUsed();
        setExpressionType(node, symbol->getType());
        setNodeSymbol(node, symbol);
    } else {
        reportUndefinedSymbol(node.getName(), node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
    }
}

void SemanticAnalyzer::visit(BinaryExpression& node) {
    // Analyze operands first
    node.getLeft()->accept(*this);
    node.getRight()->accept(*this);
    
    checkBinaryOperation(node);
}

void SemanticAnalyzer::visit(UnaryExpression& node) {
    // Analyze operand first
    node.getOperand()->accept(*this);
    
    checkUnaryOperation(node);
}

void SemanticAnalyzer::visit(AssignmentExpression& node) {
    // Analyze both sides
    node.getLeft()->accept(*this);
    node.getRight()->accept(*this);
    
    checkAssignment(*node.getLeft(), *node.getRight(), node.getLocation());
    
    // Assignment expression has the type of the right-hand side
    auto rightType = getExpressionType(*node.getRight());
    setExpressionType(node, rightType);
}

void SemanticAnalyzer::visit(CallExpression& node) {
    // Analyze callee and arguments
    node.getCallee()->accept(*this);
    for (const auto& arg : node.getArguments()) {
        arg->accept(*this);
    }
    
    checkFunctionCall(node);
}

void SemanticAnalyzer::visit(ArrayLiteral& node) {
    // Analyze all elements
    for (const auto& element : node.getElements()) {
        element->accept(*this);
    }
    
    // For now, infer array type as any[] 
    // TODO: Implement proper array type inference based on elements
    auto arrayType = typeSystem_->createArrayType(typeSystem_->getAnyType());
    node.setType(arrayType);
}

void SemanticAnalyzer::visit(IndexExpression& node) {
    // Analyze object and index
    node.getObject()->accept(*this);
    node.getIndex()->accept(*this);
    
    auto objectType = getExpressionType(*node.getObject());
    auto indexType = getExpressionType(*node.getIndex());
    
    // Check if object is indexable (array, string, etc.)
    // For now, assume it returns the element type or any
    // TODO: Implement proper indexing type checking
    if (objectType && typeSystem_->isArrayType(objectType)) {
        auto elementType = typeSystem_->getArrayElementType(objectType);
        node.setType(elementType);
    } else {
        node.setType(typeSystem_->getAnyType());
    }
}

void SemanticAnalyzer::visit(ExpressionStatement& node) {
    node.getExpression()->accept(*this);
}

void SemanticAnalyzer::visit(BlockStatement& node) {
    enterScope(Scope::ScopeType::Block);
    
    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
    }
    
    exitScope();
}

void SemanticAnalyzer::visit(ReturnStatement& node) {
    // Check if we're inside a function
    if (!isInFunctionScope()) {
        reportError("Return statement outside function", node.getLocation());
        return;
    }
    
    // Analyze return value if present
    shared_ptr<Type> returnType = typeSystem_->getVoidType();
    if (node.hasValue()) {
        node.getValue()->accept(*this);
        returnType = node.getValue()->getType();
        if (!returnType) {
            returnType = typeSystem_->getAnyType();
        }
    }
    
    // Set the type of the return statement
    node.setType(returnType);
    
    // TODO: Check return type compatibility with function signature
    // This will be implemented when we add proper function context tracking
}

void SemanticAnalyzer::visit(IfStatement& node) {
    // Analyze condition
    node.getCondition()->accept(*this);
    
    // Check if condition is boolean-compatible
    auto conditionType = node.getCondition()->getType();
    if (conditionType && !typeSystem_->isConvertibleToBoolean(conditionType)) {
        reportWarning("Condition should be boolean-compatible", node.getCondition()->getLocation());
    }
    
    // Analyze then statement
    node.getThenStatement()->accept(*this);
    
    // Analyze else statement if present
    if (node.hasElse()) {
        node.getElseStatement()->accept(*this);
    }
    
    // Set type to void (statements don't have meaningful types)
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(WhileStatement& node) {
    // Analyze condition
    node.getCondition()->accept(*this);
    
    // Check if condition is boolean-compatible
    auto conditionType = node.getCondition()->getType();
    if (conditionType && !typeSystem_->isConvertibleToBoolean(conditionType)) {
        reportWarning("While condition should be boolean-compatible", node.getCondition()->getLocation());
    }
    
    // Analyze body
    node.getBody()->accept(*this);
    
    // Set type to void
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(DoWhileStatement& node) {
    // Analyze body first
    node.getBody()->accept(*this);
    
    // Analyze condition
    node.getCondition()->accept(*this);
    
    // Check if condition is boolean-compatible
    auto conditionType = node.getCondition()->getType();
    if (conditionType && !typeSystem_->isConvertibleToBoolean(conditionType)) {
        reportWarning("Do-while condition should be boolean-compatible", node.getCondition()->getLocation());
    }
    
    // Set type to void
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(ForStatement& node) {
    // Analyze init if present
    if (node.getInit()) {
        node.getInit()->accept(*this);
    }
    
    // Analyze condition if present
    if (node.getCondition()) {
        node.getCondition()->accept(*this);
        
        // Check if condition is boolean-compatible
        auto conditionType = node.getCondition()->getType();
        if (conditionType && !typeSystem_->isConvertibleToBoolean(conditionType)) {
            reportWarning("For condition should be boolean-compatible", node.getCondition()->getLocation());
        }
    }
    
    // Analyze increment if present
    if (node.getIncrement()) {
        node.getIncrement()->accept(*this);
    }
    
    // Analyze body
    node.getBody()->accept(*this);
    
    // Set type to void
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(SwitchStatement& node) {
    // Analyze discriminant
    node.getDiscriminant()->accept(*this);
    
    // Analyze all case clauses
    for (const auto& caseClause : node.getCases()) {
        caseClause->accept(*this);
    }
    
    // Set type to void
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(CaseClause& node) {
    // Analyze test expression if present (not for default case)
    if (!node.isDefault() && node.getTest()) {
        node.getTest()->accept(*this);
    }
    
    // Analyze all statements in this case
    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(BreakStatement& node) {
    // TODO: Check if break is in valid context (loop or switch)
    // For now, just set type to void
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(ContinueStatement& node) {
    // TODO: Check if continue is in valid context (loop)
    // For now, just set type to void
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(VariableDeclaration& node) {
    shared_ptr<Type> varType;
    
    // Analyze initializer if present
    if (node.getInitializer()) {
        node.getInitializer()->accept(*this);
        varType = getExpressionType(*node.getInitializer());
    } else {
        // No initializer - use explicit type or infer as 'any'
        if (node.getTypeAnnotation()) {
            // TODO: Parse type annotation
            varType = typeSystem_->getAnyType();
        } else {
            varType = typeSystem_->getUndefinedType();
        }
    }
    
    // Check for redeclaration
    if (!symbolTable_->validateSymbolRedeclaration(node.getName(), node.getLocation())) {
        Symbol* existing = symbolTable_->lookupSymbol(node.getName());
        if (existing) {
            reportRedefinitionError(node.getName(), node.getLocation(), existing->getLocation());
        }
    }
    
    // Add symbol to current scope
    bool mutable_ = (node.getDeclarationKind() != VariableDeclaration::Kind::Const);
    if (!symbolTable_->addSymbol(node.getName(), SymbolKind::Variable, varType, 
                                 node.getLocation(), &node)) {
        reportError("Failed to declare variable: " + node.getName(), node.getLocation());
    }
}

void SemanticAnalyzer::visit(FunctionDeclaration& node) {
    // Create function type
    std::vector<FunctionType::Parameter> paramTypes;
    for (const auto& param : node.getParameters()) {
        FunctionType::Parameter funcParam;
        funcParam.name = param.name;
        funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
        funcParam.optional = param.optional;
        funcParam.rest = param.rest;
        paramTypes.push_back(funcParam);
    }
    
    auto returnType = node.getReturnType() ? node.getReturnType() : typeSystem_->getVoidType();
    auto functionType = typeSystem_->createFunctionType(std::move(paramTypes), returnType);
    
    // Add function symbol
    if (!symbolTable_->addSymbol(node.getName(), SymbolKind::Function, functionType,
                                 node.getLocation(), &node)) {
        reportError("Failed to declare function: " + node.getName(), node.getLocation());
        return;
    }
    
    // Analyze function body in new scope
    enterScope(Scope::ScopeType::Function, node.getName());
    
    // Add parameters to function scope
    for (const auto& param : node.getParameters()) {
        auto paramType = param.type ? param.type : typeSystem_->getAnyType();
        symbolTable_->addSymbol(param.name, SymbolKind::Parameter, paramType,
                               node.getLocation());
    }
    
    // Set function context
    inFunction_ = true;
    
    // Analyze function body
    if (node.getBody()) {
        node.getBody()->accept(*this);
    }
    
    // Reset function context
    inFunction_ = false;
    
    exitScope();
}

void SemanticAnalyzer::visit(Module& module) {
    // Analyze all statements in the module
    for (const auto& stmt : module.getStatements()) {
        stmt->accept(*this);
    }
}

// Analysis phase implementations
void SemanticAnalyzer::performSymbolResolution(Module& module) {
    // Symbol resolution is performed during the main visitor traversal
    module.accept(*this);
}

void SemanticAnalyzer::performTypeChecking(Module& module) {
    // Type checking is performed during the main visitor traversal
    // This phase could perform additional type-specific validations
}

void SemanticAnalyzer::performFlowAnalysis(Module& module) {
    // Flow analysis for detecting unreachable code, uninitialized variables, etc.
    // This is a placeholder for more sophisticated flow analysis
}

// Helper method implementations
void SemanticAnalyzer::declareSymbol(const String& name, SymbolKind kind, shared_ptr<Type> type, 
                                    const SourceLocation& location, ASTNode* declaration) {
    if (!symbolTable_->addSymbol(name, kind, type, location, declaration)) {
        reportError("Symbol redeclaration: " + name, location);
    }
}

Symbol* SemanticAnalyzer::resolveSymbol(const String& name, const SourceLocation& location) {
    return symbolTable_->lookupSymbol(name);
}

void SemanticAnalyzer::checkAssignment(const Expression& left, const Expression& right, 
                                      const SourceLocation& location) {
    auto leftType = getExpressionType(left);
    auto rightType = getExpressionType(right);
    
    if (!isValidAssignment(*rightType, *leftType)) {
        reportTypeError(leftType->toString(), rightType->toString(), location);
    }
}

void SemanticAnalyzer::checkFunctionCall(const CallExpression& call) {
    auto calleeType = getExpressionType(*call.getCallee());
    
    if (!calleeType->isCallable()) {
        reportError("Expression is not callable", call.getLocation());
        setExpressionType(call, typeSystem_->getErrorType());
        return;
    }
    
    // For now, assume function calls return 'any'
    // In a full implementation, we'd extract the return type from the function type
    setExpressionType(call, typeSystem_->getAnyType());
}

void SemanticAnalyzer::checkBinaryOperation(BinaryExpression& expr) {
    auto leftType = getExpressionType(*expr.getLeft());
    auto rightType = getExpressionType(*expr.getRight());
    
    int opValue = static_cast<int>(expr.getOperator());
    auto resultType = typeSystem_->inferTypeFromBinaryExpression(*leftType, *rightType, opValue);
    
    if (resultType->isError()) {
        reportError("Invalid binary operation between " + leftType->toString() + 
                   " and " + rightType->toString(), expr.getLocation());
    }
    
    setExpressionType(expr, resultType);
}

void SemanticAnalyzer::checkUnaryOperation(UnaryExpression& expr) {
    auto operandType = getExpressionType(*expr.getOperand());
    auto resultType = typeSystem_->inferTypeFromUnaryExpression(*operandType, static_cast<int>(expr.getOperator()));
    
    setExpressionType(expr, resultType);
}

// Error reporting
void SemanticAnalyzer::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
    context_->incrementErrorCount();
}

void SemanticAnalyzer::reportWarning(const String& message, const SourceLocation& location) {
    diagnostics_.warning(message, location);
}

void SemanticAnalyzer::reportTypeError(const String& expected, const String& actual, 
                                      const SourceLocation& location) {
    reportError("Type mismatch: expected " + expected + ", got " + actual, location);
}

void SemanticAnalyzer::reportUndefinedSymbol(const String& name, const SourceLocation& location) {
    reportError("Undefined symbol: " + name, location);
}

void SemanticAnalyzer::reportRedefinitionError(const String& name, const SourceLocation& location, 
                                              const SourceLocation& originalLocation) {
    reportError("Redefinition of symbol: " + name + 
               " (originally defined at " + originalLocation.getFilename() + ":" + 
               std::to_string(originalLocation.getLine()) + ":" + std::to_string(originalLocation.getColumn()) + ")", location);
}

// Utility methods
void SemanticAnalyzer::setExpressionType(const Expression& expr, shared_ptr<Type> type) {
    expressionTypes_[&expr] = type;
}

void SemanticAnalyzer::setNodeSymbol(const ASTNode& node, Symbol* symbol) {
    nodeSymbols_[&node] = symbol;
}

bool SemanticAnalyzer::isValidAssignment(const Type& from, const Type& to) const {
    return typeSystem_->areTypesCompatible(from, to);
}

String SemanticAnalyzer::formatType(const Type& type) const {
    return type.toString();
}

void SemanticAnalyzer::enterScope(Scope::ScopeType type, const String& name) {
    symbolTable_->enterScope(type, name);
}

void SemanticAnalyzer::exitScope() {
    symbolTable_->exitScope();
}

void SemanticAnalyzer::setupBuiltinEnvironment() {
    // Add built-in functions and global objects
    // This is where we'd add console, Math, etc.
    
    // Add console object
    auto consoleType = typeSystem_->createObjectType();
    symbolTable_->addSymbol("console", SymbolKind::Variable, consoleType, SourceLocation());
}

// Type inference helpers
shared_ptr<Type> SemanticAnalyzer::inferExpressionType(Expression& expr) {
    expr.accept(*this);
    return getExpressionType(expr);
}

shared_ptr<Type> SemanticAnalyzer::inferVariableType(const VariableDeclaration& decl) {
    if (decl.getInitializer()) {
        return getExpressionType(*decl.getInitializer());
    }
    return typeSystem_->getUndefinedType();
}

shared_ptr<Type> SemanticAnalyzer::inferFunctionType(const FunctionDeclaration& decl) {
    std::vector<FunctionType::Parameter> params;
    for (const auto& param : decl.getParameters()) {
        FunctionType::Parameter funcParam;
        funcParam.name = param.name;
        funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
        params.push_back(funcParam);
    }
    
    auto returnType = decl.getReturnType() ? decl.getReturnType() : typeSystem_->getVoidType();
    return typeSystem_->createFunctionType(std::move(params), returnType);
}

// Factory function
unique_ptr<SemanticAnalyzer> createSemanticAnalyzer(DiagnosticEngine& diagnostics) {
    return make_unique<SemanticAnalyzer>(diagnostics);
}

} // namespace tsc