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

shared_ptr<Type> SemanticAnalyzer::getDeclarationType(const Declaration& decl) const {
    auto it = declarationTypes_.find(&decl);
    return it != declarationTypes_.end() ? it->second : typeSystem_->getErrorType();
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
    
    // Infer array type based on elements
    shared_ptr<Type> elementType = typeSystem_->getAnyType(); // Default
    
    if (!node.getElements().empty()) {
        // Get type of first element
        elementType = getExpressionType(*node.getElements()[0]);
        
        // Check if all elements have the same type
        bool allSameType = true;
        for (size_t i = 1; i < node.getElements().size(); ++i) {
            auto currentElementType = getExpressionType(*node.getElements()[i]);
            if (!elementType->isEquivalentTo(*currentElementType)) {
                allSameType = false;
                break;
            }
        }
        
        // If elements have different types, use 'any' as element type
        if (!allSameType) {
            elementType = typeSystem_->getAnyType();
        }
    }
    
    auto arrayType = typeSystem_->createArrayType(elementType);
    setExpressionType(node, arrayType);
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
        setExpressionType(node, elementType);
    } else {
        setExpressionType(node, typeSystem_->getAnyType());
    }
}

void SemanticAnalyzer::visit(ObjectLiteral& node) {
    // Analyze all property values
    for (const auto& property : node.getProperties()) {
        property.getValue()->accept(*this);
    }
    
    // For now, infer object type as any (like a generic object)
    // TODO: Implement proper object type inference based on properties
    auto objectType = typeSystem_->getAnyType();
    setExpressionType(node, objectType);
}

void SemanticAnalyzer::visit(PropertyAccess& node) {
    // Analyze the object being accessed
    node.getObject()->accept(*this);
    auto objectType = getExpressionType(*node.getObject());
    
    // For now, assume property access returns any type
    // TODO: Implement proper property type checking based on object type
    setExpressionType(node, typeSystem_->getAnyType());
}

void SemanticAnalyzer::visit(ArrowFunction& node) {
    // Create function type
    std::vector<FunctionType::Parameter> paramTypes;
    
    // Enter new scope for arrow function parameters
    enterScope(Scope::ScopeType::Function, "arrow_function");
    
    for (const auto& param : node.getParameters()) {
        // Add parameter to scope
        auto paramType = param.type ? param.type : typeSystem_->getAnyType();
        symbolTable_->addSymbol(param.name, SymbolKind::Parameter, paramType, param.location);
        
        // Add to function type
        FunctionType::Parameter funcParam;
        funcParam.name = param.name;
        funcParam.type = paramType;
        funcParam.optional = param.optional;
        funcParam.rest = param.rest;
        paramTypes.push_back(funcParam);
    }
    
    // Analyze function body
    functionDepth_++;
    node.getBody()->accept(*this);
    functionDepth_--;
    
    // Determine return type
    auto returnType = node.getReturnType() ? node.getReturnType() : typeSystem_->getVoidType();
    
    // Create function type
    auto functionType = typeSystem_->createFunctionType(std::move(paramTypes), returnType);
    setExpressionType(node, functionType);
    
    exitScope();
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

void SemanticAnalyzer::visit(TryStatement& node) {
    // Analyze try block
    node.getTryBlock()->accept(*this);
    
    // Analyze catch clause if present
    if (node.hasCatch()) {
        enterScope(Scope::ScopeType::Block);
        
        // Add catch parameter to scope if it exists
        if (node.getCatchClause()->hasParameter()) {
            const String& paramName = node.getCatchClause()->getParameter();
            // TODO: Define proper error type for catch parameters
            auto errorType = typeSystem_->getAnyType();
            symbolTable_->addSymbol(paramName, SymbolKind::Variable, errorType, node.getLocation());
        }
        
        node.getCatchClause()->accept(*this);
        exitScope();
    }
    
    // Analyze finally block if present
    if (node.hasFinally()) {
        node.getFinallyBlock()->accept(*this);
    }
    
    node.setType(typeSystem_->getVoidType());
}

void SemanticAnalyzer::visit(CatchClause& node) {
    // Analyze catch body
    node.getBody()->accept(*this);
}

void SemanticAnalyzer::visit(ThrowStatement& node) {
    // Analyze the expression being thrown
    node.getExpression()->accept(*this);
    
    // TODO: Type check that the expression is throwable
    // For now, allow any type to be thrown
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
    // Check if function symbol already exists (from first pass)
    Symbol* existingSymbol = symbolTable_->lookupSymbol(node.getName());
    if (!existingSymbol) {
        // This shouldn't happen if collectFunctionDeclarations worked correctly
        reportError("Function symbol not found in second pass: " + node.getName(), node.getLocation());
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
    functionDepth_++;
    
    // Analyze function body
    if (node.getBody()) {
        node.getBody()->accept(*this);
    }
    
    // Reset function context
    functionDepth_--;
    
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
    // Two-pass symbol resolution:
    // Pass 1: Collect all function declarations
    collectFunctionDeclarations(module);
    
    // Pass 2: Process all statements including function bodies
    module.accept(*this);
}

void SemanticAnalyzer::collectFunctionDeclarations(Module& module) {
    // First pass: only collect function signatures, don't process bodies
    // Debug: Check how many statements we're processing
    const auto& statements = module.getStatements();
    // std::cout << "DEBUG: collectFunctionDeclarations found " << statements.size() << " statements" << std::endl;
    
    for (const auto& stmt : statements) {
        if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(stmt.get())) {
            // std::cout << "DEBUG: Found function declaration: " << funcDecl->getName() << std::endl;
            // Create function type
            std::vector<FunctionType::Parameter> paramTypes;
            for (const auto& param : funcDecl->getParameters()) {
                FunctionType::Parameter funcParam;
                funcParam.name = param.name;
                funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
                funcParam.optional = param.optional;
                funcParam.rest = param.rest;
                paramTypes.push_back(funcParam);
            }
            
            auto returnType = funcDecl->getReturnType() ? funcDecl->getReturnType() : typeSystem_->getVoidType();
            auto functionType = typeSystem_->createFunctionType(std::move(paramTypes), returnType);
            
            // Add function symbol (signature only, no body processing)
            if (!symbolTable_->addSymbol(funcDecl->getName(), SymbolKind::Function, functionType,
                                         funcDecl->getLocation(), funcDecl)) {
                reportError("Failed to declare function: " + funcDecl->getName(), funcDecl->getLocation());
            } else {
                // std::cout << "DEBUG: Successfully added function " << funcDecl->getName() << " to symbol table" << std::endl;
            }
        }
    }
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

void SemanticAnalyzer::setDeclarationType(const Declaration& decl, shared_ptr<Type> type) {
    declarationTypes_[&decl] = type;
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

// Class-related visitor implementations
void SemanticAnalyzer::visit(PropertyDeclaration& node) {
    // Analyze property type
    shared_ptr<Type> propertyType = node.getType();
    if (!propertyType) {
        if (node.getInitializer()) {
            // Infer type from initializer
            node.getInitializer()->accept(*this);
            propertyType = getExpressionType(*node.getInitializer());
        } else {
            // Default to any type if no type annotation or initializer
            propertyType = typeSystem_->getAnyType();
        }
    }
    
    // Analyze initializer if present
    if (node.getInitializer()) {
        node.getInitializer()->accept(*this);
        auto initType = getExpressionType(*node.getInitializer());
        
        // Check type compatibility
        if (!typeSystem_->areTypesCompatible(*initType, *propertyType)) {
            reportError("Property initializer type '" + initType->toString() + 
                       "' is not assignable to property type '" + propertyType->toString() + "'",
                       node.getInitializer()->getLocation());
        }
    }
    
    // Store property type information
    setDeclarationType(node, propertyType);
}

void SemanticAnalyzer::visit(MethodDeclaration& node) {
    // Enter method scope
    enterScope(Scope::ScopeType::Function, node.getName());
    functionDepth_++;
    
    // Add parameters to scope
    std::vector<FunctionType::Parameter> paramTypes;
    for (const auto& param : node.getParameters()) {
        FunctionType::Parameter funcParam;
        funcParam.name = param.name;
        funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
        funcParam.optional = param.optional;
        paramTypes.push_back(funcParam);
        
        // Add parameter to symbol table
        declareSymbol(param.name, SymbolKind::Variable, funcParam.type, node.getLocation());
    }
    
    // Analyze method body
    if (node.getBody()) {
        node.getBody()->accept(*this);
    }
    
    // Create method type
    auto returnType = node.getReturnType() ? node.getReturnType() : typeSystem_->getVoidType();
    auto methodType = typeSystem_->createFunctionType(std::move(paramTypes), returnType);
    setDeclarationType(node, methodType);
    
    // Exit method scope
    functionDepth_--;
    exitScope();
}

void SemanticAnalyzer::visit(ClassDeclaration& node) {
    // Create class type
    auto classType = typeSystem_->createClassType(node.getName(), &node, node.getBaseClass());
    
    // Add class to symbol table
    declareSymbol(node.getName(), SymbolKind::Type, classType, node.getLocation());
    
    // Enter class scope
    enterScope(Scope::ScopeType::Class, node.getName());
    
    // Analyze base class if present
    if (node.getBaseClass()) {
        // Base class validation would go here
        // For now, we assume it's valid
    }
    
    // Analyze interfaces if present
    for (const auto& interface : node.getInterfaces()) {
        // Interface validation would go here
        // For now, we assume they're valid
    }
    
    // Analyze properties
    for (const auto& property : node.getProperties()) {
        property->accept(*this);
        
        // Add property to class scope
        auto propType = getDeclarationType(*property);
        declareSymbol(property->getName(), SymbolKind::Property, propType, property->getLocation());
    }
    
    // Analyze constructor if present
    if (node.getConstructor()) {
        node.getConstructor()->accept(*this);
    }
    
    // Analyze methods
    for (const auto& method : node.getMethods()) {
        method->accept(*this);
        
        // Add method to class scope
        auto methodType = getDeclarationType(*method);
        declareSymbol(method->getName(), SymbolKind::Method, methodType, method->getLocation());
    }
    
    // Exit class scope
    exitScope();
    
    // ClassDeclaration is not an Expression, so we don't call setExpressionType
    // The class type is already stored in the symbol table above
}

// Factory function
unique_ptr<SemanticAnalyzer> createSemanticAnalyzer(DiagnosticEngine& diagnostics) {
    return make_unique<SemanticAnalyzer>(diagnostics);
}

} // namespace tsc