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
        resolveDeferredSuperExpressions();
        resolveDeferredSuperPropertyAccesses();
        resolveDeferredSuperCallExpressions();
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

// TODO: Template literals semantic analysis
/*
void SemanticAnalyzer::visit(TemplateLiteral& node) {
    // Analyze all expressions within the template literal
    for (const auto& element : node.getElements()) {
        if (element.isExpression()) {
            element.getExpression()->accept(*this);
        }
    }
    
    // Template literals result in strings
    setExpressionType(node, typeSystem_->getStringType());
}
*/

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
        
        // Check if we're inside a nested function and accessing a variable from outer scope
        if (functionDepth_ > 1) {
            // We're inside a nested function, check if the symbol comes from an outer scope
            Scope* currentScope = symbolTable_->getCurrentScope();
            Symbol* localSymbol = currentScope->lookupSymbolLocal(node.getName());
            
            if (!localSymbol) {
                // Symbol is not in current function scope, it comes from outer scope
                // Mark the current function as captured and add the variable to captured list
                markVariableAsCaptured(symbol);
            }
        }
        
        setExpressionType(node, symbol->getType());
        setNodeSymbol(node, symbol);
    } else {
        reportUndefinedSymbol(node.getName(), node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
    }
}

void SemanticAnalyzer::visit(ThisExpression& node) {
    // 'this' is only valid within a class method or constructor
    auto currentScope = symbolTable_->getCurrentScope();
    
    // Walk up the scope chain to find a class scope
    while (currentScope && currentScope->getType() != Scope::ScopeType::Class) {
        currentScope = currentScope->getParent();
    }
    
    if (!currentScope) {
        reportError("'this' keyword used outside of class context", node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
        return;
    }
    
    // Get the class type from the class scope
    String className = currentScope->getName();
    
    // Look up the class symbol to get its type
    Symbol* classSymbol = symbolTable_->lookupSymbol(className);
    
    if (classSymbol && classSymbol->getKind() == SymbolKind::Class) {
        auto classType = classSymbol->getType();
        setExpressionType(node, classType);
    } else {
        // Fallback to Any if we can't find the class
        auto thisType = typeSystem_->getAnyType();
        setExpressionType(node, thisType);
    }
}

void SemanticAnalyzer::visit(SuperExpression& node) {
    // 'super' is only valid within a class method or constructor that has inheritance
    auto currentScope = symbolTable_->getCurrentScope();
    
    // Walk up the scope chain to find a class scope
    while (currentScope && currentScope->getType() != Scope::ScopeType::Class) {
        currentScope = currentScope->getParent();
    }
    
    if (!currentScope) {
        reportError("'super' keyword used outside of class context", node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
        return;
    }
    
    // Find the current class declaration to check if it has a base class
    // For now, we'll look up the class type in the symbol table
    String className = currentScope->getName();
    auto classSymbol = resolveSymbol(className, node.getLocation());
    
    if (!classSymbol || classSymbol->getKind() != SymbolKind::Class) {
        reportError("Cannot find current class type", node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
        return;
    }
    
    shared_ptr<Type> currentType = classSymbol->getType();
    shared_ptr<Type> baseClass = nullptr;
    
    if (currentType->getKind() == TypeKind::Class) {
        auto classType = std::static_pointer_cast<ClassType>(currentType);
        baseClass = classType->getBaseClass();
    } else if (currentType->getKind() == TypeKind::Generic) {
        auto genericType = std::static_pointer_cast<GenericType>(currentType);
        auto baseType = genericType->getBaseType();
        if (baseType->getKind() == TypeKind::Class) {
            auto classType = std::static_pointer_cast<ClassType>(baseType);
            baseClass = classType->getBaseClass();
        }
    }
    
    if (!baseClass) {
        reportError("'super' used in class that does not extend another class", node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
        return;
    }
    
    // Set the type to the resolved base class type
    auto baseClassType = resolveType(baseClass);
    
    // If resolution failed, defer this super expression for later resolution
    if (baseClassType->getKind() == TypeKind::Error) {
        // Store this super expression for deferred resolution
        DeferredSuperExpression deferred;
        deferred.expression = &node;
        deferred.className = className;
        deferred.location = node.getLocation();
        deferredSuperExpressions_.push_back(deferred);
        
        
        // Set a placeholder type for now
        setExpressionType(node, typeSystem_->getAnyType());
        return;
    }
    
    setExpressionType(node, baseClassType);
}

void SemanticAnalyzer::visit(NewExpression& node) {
    // Analyze arguments first
    for (const auto& arg : node.getArguments()) {
        arg->accept(*this);
    }
    
    // Handle constructor expression
    if (auto identifier = dynamic_cast<Identifier*>(node.getConstructor())) {
        // Try to find the class type in the symbol table
        Symbol* classSymbol = resolveSymbol(identifier->getName(), node.getLocation());
        if (classSymbol && classSymbol->getKind() == SymbolKind::Class) {
            auto classType = classSymbol->getType();
            
            // Check if this is a class type that can be instantiated
            if (classType->getKind() == TypeKind::Class) {
                // Handle explicit type arguments for generic classes
                if (node.hasExplicitTypeArguments()) {
                    // Process explicit type arguments (e.g., Container<number>)
                    const auto& typeArguments = node.getTypeArguments();
                    
                    // Resolve each type argument
                    std::vector<shared_ptr<Type>> resolvedTypeArgs;
                    for (const auto& typeArg : typeArguments) {
                        auto resolvedType = resolveType(typeArg);
                        if (resolvedType->getKind() == TypeKind::Error) {
                            reportError("Invalid type argument: " + typeArg->toString(), node.getLocation());
                            setExpressionType(node, typeSystem_->getErrorType());
                            return;
                        }
                        resolvedTypeArgs.push_back(resolvedType);
                    }
                    
                    // Create a generic type instance (e.g., Container<number>)
                    auto genericType = typeSystem_->createGenericType(classType, resolvedTypeArgs);
                    setExpressionType(node, genericType);
                } else {
                    // No explicit type arguments - use the raw class type
                    // TODO: In the future, we might infer type arguments from constructor arguments
                    setExpressionType(node, classType);
                }
            } else {
                reportError("'" + identifier->getName() + "' is not a class type", node.getLocation());
                setExpressionType(node, typeSystem_->getErrorType());
            }
        } else {
            reportError("Constructor '" + identifier->getName() + "' not found", node.getLocation());
            setExpressionType(node, typeSystem_->getErrorType());
        }
    } else {
        // For complex constructor expressions, analyze them
        node.getConstructor()->accept(*this);
        auto constructorType = getExpressionType(*node.getConstructor());
        
        // For complex constructor expressions, use the constructor's type or any type
        setExpressionType(node, typeSystem_->getAnyType());
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

void SemanticAnalyzer::visit(ConditionalExpression& node) {
    // Analyze all three expressions
    node.getCondition()->accept(*this);
    node.getTrueExpression()->accept(*this);
    node.getFalseExpression()->accept(*this);
    
    // Get types
    auto conditionType = getExpressionType(*node.getCondition());
    auto trueType = getExpressionType(*node.getTrueExpression());
    auto falseType = getExpressionType(*node.getFalseExpression());
    
    // Condition should be boolean (but we allow any type for now)
    // TODO: Add proper boolean conversion checking
    
    // Result type is the union of true and false types
    // For now, use the true type if both are the same, otherwise use any
    shared_ptr<Type> resultType;
    if (trueType == falseType) {
        resultType = trueType;
    } else {
        resultType = typeSystem_->getAnyType();
    }
    
    setExpressionType(node, resultType);
}

void SemanticAnalyzer::visit(CallExpression& node) {
    // Analyze callee and arguments
    node.getCallee()->accept(*this);
    for (const auto& arg : node.getArguments()) {
        arg->accept(*this);
    }
    
    // Check if this is a call to a deferred super property access
    if (auto propertyAccess = dynamic_cast<const PropertyAccess*>(node.getCallee())) {
        if (dynamic_cast<const SuperExpression*>(propertyAccess->getObject())) {
            // Check if this property access was deferred
            auto calleeType = getExpressionType(*propertyAccess);
            if (calleeType->getKind() == TypeKind::Any) {
                // This is likely a deferred super property access, defer the call expression too
                auto currentScope = symbolTable_->getCurrentScope();
                while (currentScope && currentScope->getType() != Scope::ScopeType::Class) {
                    currentScope = currentScope->getParent();
                }
                
                if (currentScope) {
                    DeferredSuperCallExpression deferred;
                    deferred.callExpression = &node;
                    deferred.propertyAccess = const_cast<PropertyAccess*>(propertyAccess);
                    deferred.className = currentScope->getName();
                    deferred.memberName = propertyAccess->getProperty();
                    deferred.location = node.getLocation();
                    deferredSuperCallExpressions_.push_back(deferred);
                    
                    
                    // Set a placeholder type for now
                    setExpressionType(node, typeSystem_->getAnyType());
                    return;
                }
            }
        }
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
    
    
    
    // Note: SuperExpression is handled by the regular class property access logic below
    
    // Check if this is an enum member access
    if (objectType->getKind() == TypeKind::Enum) {
        const auto& enumType = static_cast<const EnumType&>(*objectType);
        String memberName = node.getProperty();
        
        // Check if the enum has this member
        // We need to look up the member in the enum's declaration scope
        EnumDeclaration* enumDecl = enumType.getDeclaration();
        if (enumDecl) {
            // Look for the member in the enum declaration
            for (const auto& member : enumDecl->getMembers()) {
                if (member->getName() == memberName) {
                    // Found the member - return its type (number for now)
                    setExpressionType(node, typeSystem_->getNumberType());
                    return;
                }
            }
        }
        
        // If we get here, the member wasn't found
        reportError("Enum '" + enumType.getName() + "' has no member '" + memberName + "'", node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
        return;
    }
    
    // Check if this is a class property/method access
    if (objectType->getKind() == TypeKind::Class) {
        const auto& classType = static_cast<const ClassType&>(*objectType);
        String memberName = node.getProperty();
        
        // Check if the class has this member (including inherited members)
        if (auto memberType = findClassMember(classType, memberName)) {
            setExpressionType(node, memberType);
            return;
        }
        
        // If we get here, the member wasn't found
        reportError("Class '" + classType.getName() + "' has no member '" + memberName + "'", node.getLocation());
        setExpressionType(node, typeSystem_->getErrorType());
        return;
    }
    
    // Check if this is a generic class property/method access
    if (objectType->getKind() == TypeKind::Generic) {
        const auto& genericType = static_cast<const GenericType&>(*objectType);
        auto baseType = genericType.getBaseType();
        
        // The base type should be a ClassType
        if (baseType->getKind() == TypeKind::Class) {
            const auto& classType = static_cast<const ClassType&>(*baseType);
            String memberName = node.getProperty();
            
            // Check if the class has this member (including inherited members)
            if (auto memberType = findClassMember(classType, memberName)) {
                
                // If this is a super property access and the member type is Error, defer resolution
                if (dynamic_cast<const SuperExpression*>(node.getObject()) && memberType->getKind() == TypeKind::Error) {
                    // Find the current class name for deferred resolution
                    auto currentScope = symbolTable_->getCurrentScope();
                    while (currentScope && currentScope->getType() != Scope::ScopeType::Class) {
                        currentScope = currentScope->getParent();
                    }
                    
                    if (currentScope) {
                        // Store this property access for deferred resolution
                        DeferredSuperPropertyAccess deferred;
                        deferred.propertyAccess = &node;
                        deferred.className = currentScope->getName();
                        deferred.memberName = memberName;
                        deferred.location = node.getLocation();
                        deferredSuperPropertyAccesses_.push_back(deferred);
                        
                        
                        // Set a placeholder type for now
                        setExpressionType(node, typeSystem_->getAnyType());
                        return;
                    }
                }
                
                setExpressionType(node, memberType);
                return;
            }
            
            // If we get here, the member wasn't found
            reportError("Class '" + classType.getName() + "' has no member '" + memberName + "'", node.getLocation());
            setExpressionType(node, typeSystem_->getErrorType());
            return;
        } else {
            reportError("Generic type base is not a class", node.getLocation());
            setExpressionType(node, typeSystem_->getErrorType());
            return;
        }
    }
    
    // Check if this is a property access on a generic type parameter
    if (objectType->getKind() == TypeKind::TypeParameter) {
        const auto& typeParamType = static_cast<const TypeParameterType&>(*objectType);
        String memberName = node.getProperty();
        
        // Check if the type parameter has a constraint
        auto constraint = typeParamType.getConstraint();
        if (constraint) {
            // If there's a constraint, check if the member exists on the constraint type
            auto memberType = findMemberType(constraint, memberName);
            if (memberType) {
                setExpressionType(node, memberType);
                return;
            }
        }
        
        // All types have certain built-in methods
        if (memberName == "toString") {
            // toString() method returns string
            auto stringReturnType = typeSystem_->getStringType();
            auto toStringType = typeSystem_->createFunctionType({}, stringReturnType);
            setExpressionType(node, toStringType);
            return;
        } else if (memberName == "valueOf") {
            // valueOf() method returns the same type as the type parameter
            auto valueOfType = typeSystem_->createFunctionType({}, objectType);
            setExpressionType(node, valueOfType);
            return;
        } else {
            // Unknown method on generic type parameter
            reportError("Generic type parameter '" + typeParamType.getName() + "' has no member '" + memberName + "'", node.getLocation());
            setExpressionType(node, typeSystem_->getErrorType());
            return;
        }
    }
    
    // Handle other property access types (objects, etc.)
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
    auto returnType = node.getReturnType();
    if (!returnType) {
        // For arrow functions, try to infer return type from body
        returnType = inferReturnType(*node.getBody());
        if (!returnType) {
            returnType = typeSystem_->getVoidType();
        }
    }
    
    // Create function type
    auto functionType = typeSystem_->createFunctionType(std::move(paramTypes), returnType);
    setExpressionType(node, functionType);
    
    exitScope();
}

shared_ptr<Type> SemanticAnalyzer::inferReturnType(const Statement& body) {
    // Simple return type inference for arrow functions
    // Look for return statements and infer type from the first one
    
    // For now, let's implement a simple version without a full visitor
    // We'll just check if it's a BlockStatement with a ReturnStatement
    
    if (auto blockStmt = dynamic_cast<const BlockStatement*>(&body)) {
        for (const auto& stmt : blockStmt->getStatements()) {
            if (auto returnStmt = dynamic_cast<const ReturnStatement*>(stmt.get())) {
                if (returnStmt->getValue()) {
                    return getExpressionType(*returnStmt->getValue());
                }
            }
        }
    }
    
    // If we can't infer, return null (caller will use void)
    return nullptr;
}

void SemanticAnalyzer::visit(FunctionExpression& node) {
    // Function expressions are similar to arrow functions but with slightly different semantics
    // Create function type
    std::vector<FunctionType::Parameter> paramTypes;
    
    // Enter new scope for function expression parameters
    enterScope(Scope::ScopeType::Function, "function_expression");
    
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
    auto returnType = node.getReturnType();
    if (!returnType) {
        // For function expressions, try to infer return type from body
        returnType = inferReturnType(*node.getBody());
        if (!returnType) {
            returnType = typeSystem_->getVoidType();
        }
    }
    
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

void SemanticAnalyzer::visit(ForOfStatement& node) {
    // Enter a new scope for the loop
    enterScope(Scope::ScopeType::Block);
    
    // Analyze the iterable expression first
    node.getIterable()->accept(*this);
    auto iterableType = node.getIterable()->getType();
    
    // For now, we'll assume arrays are the primary iterable type
    // TODO: Add proper iterable interface support
    shared_ptr<Type> elementType = typeSystem_->getAnyType();
    
    if (iterableType) {
        // Check if it's an array type - for now we'll infer the element type
        // This is a simplified implementation
        if (auto arrayType = dynamic_cast<ArrayType*>(iterableType.get())) {
            elementType = arrayType->getElementType();
        } else {
            // For non-array types, we'll use 'any' for now
            reportWarning("For-of loop requires an iterable type (array expected)", node.getIterable()->getLocation());
        }
    }
    
    // Add the loop variable to the current scope
    const String& varName = node.getVariable();
    
    // Add variable to symbol table
    if (!symbolTable_->addSymbol(varName, SymbolKind::Variable, elementType, node.getLocation())) {
        reportError("Variable '" + varName + "' already declared in this scope", node.getLocation());
    }
    
    // Analyze the body
    node.getBody()->accept(*this);
    
    // Exit the loop scope
    exitScope();
    
    // Set the for-of statement type to void
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
            // Resolve the type annotation
            varType = resolveType(node.getTypeAnnotation());
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
    
    // Add type parameters to function scope first
    for (const auto& typeParam : node.getTypeParameters()) {
        typeParam->accept(*this);
    }
    
    // Add parameters to function scope
    for (const auto& param : node.getParameters()) {
        auto paramType = param.type ? resolveType(param.type) : typeSystem_->getAnyType();
        symbolTable_->addSymbol(param.name, SymbolKind::Parameter, paramType,
                               node.getLocation());
    }
    
    // Set function context
    functionDepth_++;
    functionStack_.push_back(&node);
    
    // Collect nested function declarations before analyzing the body
    if (node.getBody()) {
        collectNestedFunctionDeclarations(*node.getBody());
    }
    
    // Analyze function body
    if (node.getBody()) {
        node.getBody()->accept(*this);
    }
    
    // Reset function context
    functionDepth_--;
    functionStack_.pop_back();
    
    exitScope();
}

void SemanticAnalyzer::visit(TypeParameter& node) {
    // Create type parameter type
    auto typeParam = typeSystem_->createTypeParameter(node.getName(), node.getConstraint());
    
    // Add type parameter to symbol table as a type
    declareSymbol(node.getName(), SymbolKind::Type, typeParam, node.getLocation());
    
    // Set the node's type (though TypeParameter isn't an Expression, this maintains consistency)
    if (auto expr = dynamic_cast<Expression*>(&node)) {
        setExpressionType(*expr, typeParam);
    }
}

void SemanticAnalyzer::visit(Module& module) {
    // Analyze all statements in the module
    for (const auto& stmt : module.getStatements()) {
        stmt->accept(*this);
    }
}

// Analysis phase implementations
void SemanticAnalyzer::performSymbolResolution(Module& module) {
    // Three-pass symbol resolution:
    // Pass 1: Collect all function and class declarations
    collectFunctionDeclarations(module);
    
    // Pass 2: Resolve inheritance relationships
    resolveInheritance(module);
    
    // Pass 3: Process all statements including function bodies
    module.accept(*this);
}

void SemanticAnalyzer::collectFunctionDeclarations(Module& module) {
    // First pass: collect function signatures and class declarations, don't process bodies
    // Debug: Check how many statements we're processing
    const auto& statements = module.getStatements();
    // std::cout << "DEBUG: collectFunctionDeclarations found " << statements.size() << " statements" << std::endl;
    
    for (const auto& stmt : statements) {
        if (auto classDecl = dynamic_cast<ClassDeclaration*>(stmt.get())) {
            // Collect class declarations first so they're available for constructor calls
            // Create class type WITHOUT base class (will be resolved in second pass)
            auto classType = typeSystem_->createClassType(classDecl->getName(), classDecl, nullptr);
            declareSymbol(classDecl->getName(), SymbolKind::Class, classType, classDecl->getLocation());
        } else if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(stmt.get())) {
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
            
            // Note: Nested functions will be collected during the second pass
            // when we're already in the correct scope
        }
    }
}

void SemanticAnalyzer::resolveInheritance(Module& module) {
    // Second pass: resolve inheritance relationships for all classes
    const auto& statements = module.getStatements();
    
    for (const auto& stmt : statements) {
        if (auto classDecl = dynamic_cast<ClassDeclaration*>(stmt.get())) {
            // Resolve base class for this class declaration
            if (classDecl->getBaseClass()) {
                // Look up the class type in the symbol table
                Symbol* classSymbol = symbolTable_->lookupSymbol(classDecl->getName());
                if (classSymbol && classSymbol->getKind() == SymbolKind::Class) {
                    auto classType = std::static_pointer_cast<ClassType>(classSymbol->getType());
                    
                    // Resolve the base class type
                    auto resolvedBaseType = resolveType(classDecl->getBaseClass());
                    if (resolvedBaseType->getKind() != TypeKind::Error) {
                        // Set the resolved base class on the class type
                        classType->setBaseClass(resolvedBaseType);
                    }
                }
            }
        }
    }
}

void SemanticAnalyzer::resolveDeferredSuperExpressions() {
    
    // Resolve all deferred super expressions now that inheritance is fully resolved
    for (const auto& deferred : deferredSuperExpressions_) {
        SuperExpression* superExpr = deferred.expression;
        const String& className = deferred.className;
        
        // Find the class symbol
        auto classSymbol = resolveSymbol(className, deferred.location);
        
        if (!classSymbol || classSymbol->getKind() != SymbolKind::Class) {
            reportError("Cannot find current class type for deferred super resolution", deferred.location);
            setExpressionType(*superExpr, typeSystem_->getErrorType());
            continue;
        }
        
        shared_ptr<Type> currentType = classSymbol->getType();
        shared_ptr<Type> baseClass = nullptr;
        
        if (currentType->getKind() == TypeKind::Class) {
            auto classType = std::static_pointer_cast<ClassType>(currentType);
            baseClass = classType->getBaseClass();
        } else if (currentType->getKind() == TypeKind::Generic) {
            auto genericType = std::static_pointer_cast<GenericType>(currentType);
            auto baseType = genericType->getBaseType();
            if (baseType->getKind() == TypeKind::Class) {
                auto classType = std::static_pointer_cast<ClassType>(baseType);
                baseClass = classType->getBaseClass();
            }
        }
        
        if (!baseClass) {
            reportError("'super' used in class that does not extend another class", deferred.location);
            setExpressionType(*superExpr, typeSystem_->getErrorType());
            continue;
        }
        
        // Now resolve the base class type - this should work now that inheritance is resolved
        auto baseClassType = resolveType(baseClass);
        if (baseClassType->getKind() == TypeKind::Error) {
            reportError("Cannot resolve base class type for 'super'", deferred.location);
            setExpressionType(*superExpr, typeSystem_->getErrorType());
            continue;
        }
        
        // Set the resolved type
        setExpressionType(*superExpr, baseClassType);
    }
    
    // Clear the deferred list
    deferredSuperExpressions_.clear();
}

void SemanticAnalyzer::resolveDeferredSuperPropertyAccesses() {
    
    // Resolve all deferred super property accesses now that all classes are fully processed
    for (const auto& deferred : deferredSuperPropertyAccesses_) {
        PropertyAccess* propertyAccess = deferred.propertyAccess;
        const String& className = deferred.className;
        const String& memberName = deferred.memberName;
        
        // Find the class symbol
        auto classSymbol = resolveSymbol(className, deferred.location);
        
        if (!classSymbol || classSymbol->getKind() != SymbolKind::Class) {
            reportError("Cannot find current class type for deferred super property access", deferred.location);
            setExpressionType(*propertyAccess, typeSystem_->getErrorType());
            continue;
        }
        
        shared_ptr<Type> currentType = classSymbol->getType();
        shared_ptr<Type> baseClass = nullptr;
        
        if (currentType->getKind() == TypeKind::Class) {
            auto classType = std::static_pointer_cast<ClassType>(currentType);
            baseClass = classType->getBaseClass();
        } else if (currentType->getKind() == TypeKind::Generic) {
            auto genericType = std::static_pointer_cast<GenericType>(currentType);
            auto baseType = genericType->getBaseType();
            if (baseType->getKind() == TypeKind::Class) {
                auto classType = std::static_pointer_cast<ClassType>(baseType);
                baseClass = classType->getBaseClass();
            }
        }
        
        if (!baseClass) {
            reportError("'super' used in class that does not extend another class", deferred.location);
            setExpressionType(*propertyAccess, typeSystem_->getErrorType());
            continue;
        }
        
        // Now resolve the base class type and find the member
        auto baseClassType = resolveType(baseClass);
        if (baseClassType->getKind() == TypeKind::Error) {
            reportError("Cannot resolve base class type for 'super' property access", deferred.location);
            setExpressionType(*propertyAccess, typeSystem_->getErrorType());
            continue;
        }
        
        // Find the member in the base class
        shared_ptr<Type> memberType = nullptr;
        if (baseClassType->getKind() == TypeKind::Class) {
            const auto& classType = static_cast<const ClassType&>(*baseClassType);
            memberType = findClassMember(classType, memberName);
        } else if (baseClassType->getKind() == TypeKind::Generic) {
            const auto& genericType = static_cast<const GenericType&>(*baseClassType);
            auto baseType = genericType.getBaseType();
            if (baseType->getKind() == TypeKind::Class) {
                const auto& classType = static_cast<const ClassType&>(*baseType);
                memberType = findClassMember(classType, memberName);
            }
        }
        
        if (!memberType || memberType->getKind() == TypeKind::Error) {
            reportError("Cannot resolve member '" + memberName + "' in base class for 'super' property access", deferred.location);
            setExpressionType(*propertyAccess, typeSystem_->getErrorType());
            continue;
        }
        
        
        // Set the resolved type
        setExpressionType(*propertyAccess, memberType);
    }
    
    // Clear the deferred list
    deferredSuperPropertyAccesses_.clear();
}

void SemanticAnalyzer::resolveDeferredSuperCallExpressions() {
    
    // Resolve all deferred super call expressions now that property access is resolved
    for (const auto& deferred : deferredSuperCallExpressions_) {
        CallExpression* callExpression = deferred.callExpression;
        PropertyAccess* propertyAccess = deferred.propertyAccess;
        
        // Get the resolved type of the property access (should be resolved by now)
        auto calleeType = getExpressionType(*propertyAccess);
        
        
        // Check if the callee is callable
        if (!calleeType->isCallable()) {
            reportError("Expression is not callable", callExpression->getLocation());
            setExpressionType(*callExpression, typeSystem_->getErrorType());
            continue;
        }
        
        // For function calls, return the function's return type
        if (calleeType->getKind() == TypeKind::Function) {
            const auto& funcType = static_cast<const FunctionType&>(*calleeType);
            setExpressionType(*callExpression, funcType.getReturnType());
        } else {
            // For other callable types, use any type for now
            setExpressionType(*callExpression, typeSystem_->getAnyType());
        }
        
    }
    
    // Clear the deferred list
    deferredSuperCallExpressions_.clear();
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
    
    // Special handling for super() calls
    if (auto superExpr = dynamic_cast<const SuperExpression*>(call.getCallee())) {
        // super() calls are constructor calls to the parent class
        if (calleeType->getKind() == TypeKind::Class || calleeType->getKind() == TypeKind::Generic) {
            // This is a valid super constructor call
            setExpressionType(call, typeSystem_->getVoidType());
            return;
        } else if (calleeType->getKind() != TypeKind::Error) {
            reportError("'super' must be called as a constructor", call.getLocation());
            setExpressionType(call, typeSystem_->getErrorType());
            return;
        } else {
            // Error already reported in SuperExpression analysis
            setExpressionType(call, typeSystem_->getErrorType());
            return;
        }
    }
    
    if (!calleeType->isCallable()) {
        reportError("Expression is not callable", call.getLocation());
        setExpressionType(call, typeSystem_->getErrorType());
        return;
    }
    
    // Handle generic function calls with basic monomorphization
    if (auto identifier = dynamic_cast<const Identifier*>(call.getCallee())) {
        Symbol* symbol = symbolTable_->lookupSymbol(identifier->getName());
        if (symbol && symbol->getKind() == SymbolKind::Function) {
            // Check if this is a generic function call
            if (auto functionType = dynamic_cast<const FunctionType*>(calleeType.get())) {
                // Extract return type from function type
                auto returnType = functionType->getReturnType();
                
                // Basic type inference for generic functions
                if (returnType && returnType->getKind() == TypeKind::TypeParameter) {
                    // If return type is a type parameter, try to infer it from arguments
                    if (!call.getArguments().empty()) {
                        auto firstArgType = getExpressionType(*call.getArguments()[0]);
                        if (firstArgType && firstArgType->getKind() != TypeKind::Error) {
                            // Use the first argument's type as the return type (simple inference)
                            setExpressionType(call, firstArgType);
                            return;
                        }
                    }
                }
                
                // Use the function's declared return type
                setExpressionType(call, returnType ? returnType : typeSystem_->getAnyType());
                return;
            }
        }
    }
    
    // For other function calls, assume they return 'any'
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
    } else {
        // Resolve the property type (important for generic type parameters)
        propertyType = resolveType(propertyType);
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
        // Resolve parameter types to handle generic type parameters
        funcParam.type = param.type ? resolveType(param.type) : typeSystem_->getAnyType();
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
    // Create class type first (without base class, will be resolved later)
    auto classType = typeSystem_->createClassType(node.getName(), &node, nullptr);
    
    // Add class to symbol table or update existing symbol from first pass
    Symbol* existingSymbol = symbolTable_->lookupSymbol(node.getName());
    if (existingSymbol) {
        // Update the existing symbol with the more complete class type
        existingSymbol->setType(classType);
    } else {
        declareSymbol(node.getName(), SymbolKind::Class, classType, node.getLocation());
    }
    
    // Enter class scope FIRST
    enterScope(Scope::ScopeType::Class, node.getName());
    
    // Process type parameters AFTER entering class scope
    std::vector<shared_ptr<Type>> typeParameters;
    for (const auto& typeParam : node.getTypeParameters()) {
        auto paramType = typeSystem_->createTypeParameter(typeParam->getName(), typeParam->getConstraint());
        typeParameters.push_back(paramType);
        
        // Add type parameter to class scope so it can be referenced within the class
        declareSymbol(typeParam->getName(), SymbolKind::Type, paramType, typeParam->getLocation());
    }
    
    // Analyze base class if present
    if (node.getBaseClass()) {
        // Resolve the base class type
        auto resolvedBaseType = resolveType(node.getBaseClass());
        if (resolvedBaseType->getKind() == TypeKind::Error) {
            reportError("Base class type not found: " + node.getBaseClass()->toString(), node.getLocation());
        } else if (resolvedBaseType->getKind() != TypeKind::Class && resolvedBaseType->getKind() != TypeKind::Generic) {
            reportError("Base class must be a class type", node.getLocation());
        } else {
            // Check for circular inheritance (simplified check)
            String baseClassName;
            if (resolvedBaseType->getKind() == TypeKind::Class) {
                auto baseClassType = std::static_pointer_cast<ClassType>(resolvedBaseType);
                baseClassName = baseClassType->getName();
            } else if (resolvedBaseType->getKind() == TypeKind::Generic) {
                auto genericBaseType = std::static_pointer_cast<GenericType>(resolvedBaseType);
                auto baseType = genericBaseType->getBaseType();
                if (baseType->getKind() == TypeKind::Class) {
                    auto baseClassType = std::static_pointer_cast<ClassType>(baseType);
                    baseClassName = baseClassType->getName();
                }
            }
            
            if (baseClassName == node.getName()) {
                reportError("Class cannot extend itself", node.getLocation());
            } else {
                // Update the ClassType with the resolved base class type
                if (classType->getKind() == TypeKind::Class) {
                    auto currentClassType = std::static_pointer_cast<ClassType>(classType);
                    currentClassType->setBaseClass(resolvedBaseType);
                } else if (classType->getKind() == TypeKind::Generic) {
                    auto currentGenericType = std::static_pointer_cast<GenericType>(classType);
                    auto currentBaseType = currentGenericType->getBaseType();
                    if (currentBaseType->getKind() == TypeKind::Class) {
                        auto currentClassType = std::static_pointer_cast<ClassType>(currentBaseType);
                        currentClassType->setBaseClass(resolvedBaseType);
                    }
                }
            }
        }
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

void SemanticAnalyzer::visit(InterfaceDeclaration& node) {
    // Create interface type
    auto interfaceType = typeSystem_->createInterfaceType(node.getName(), &node);
    
    // Add interface to symbol table
    declareSymbol(node.getName(), SymbolKind::Type, interfaceType, node.getLocation());
    
    // Enter interface scope (using Class scope type since interfaces are similar)
    enterScope(Scope::ScopeType::Class, node.getName());
    
    // Analyze extended interfaces if present
    for (const auto& extended : node.getExtends()) {
        // Interface extension validation would go here
        // For now, we assume they're valid
    }
    
    // Analyze property signatures
    for (const auto& property : node.getProperties()) {
        property->accept(*this);
        
        // Add property to interface scope
        auto propType = getDeclarationType(*property);
        declareSymbol(property->getName(), SymbolKind::Property, propType, property->getLocation());
    }
    
    // Analyze method signatures
    for (const auto& method : node.getMethods()) {
        method->accept(*this);
        
        // Add method to interface scope
        auto methodType = getDeclarationType(*method);
        declareSymbol(method->getName(), SymbolKind::Method, methodType, method->getLocation());
    }
    
    // Exit interface scope
    exitScope();
    
    // InterfaceDeclaration is not an Expression, so we don't call setExpressionType
    // The interface type is already stored in the symbol table above
}

void SemanticAnalyzer::visit(EnumMember& node) {
    // Analyze the value expression if present
    if (node.hasValue()) {
        node.getValue()->accept(*this);
        
        // Get the type of the value expression
        auto valueType = getExpressionType(*node.getValue());
        
        // For now, we'll accept number and string types for enum values
        // In a full implementation, we'd do more sophisticated type checking
        if (!valueType->isEquivalentTo(*typeSystem_->getNumberType()) && 
            !valueType->isEquivalentTo(*typeSystem_->getStringType())) {
            reportError("Enum member value must be a number or string", node.getLocation());
        }
    }
    
    // Enum members have number type by default (auto-incremented)
    // or the type of their explicit value
    auto memberType = typeSystem_->getNumberType();
    if (node.hasValue()) {
        memberType = getExpressionType(*node.getValue());
    }
    
    // Note: We don't call setExpressionType because EnumMember is not an Expression
    // The member's type information is handled by the containing EnumDeclaration
}

void SemanticAnalyzer::visit(EnumDeclaration& node) {
    // Create enum type
    auto enumType = typeSystem_->createEnumType(node.getName(), &node);
    
    // Add enum to symbol table
    declareSymbol(node.getName(), SymbolKind::Type, enumType, node.getLocation());
    
    // Enter enum scope (using Class scope type since enums are similar)
    enterScope(Scope::ScopeType::Class, node.getName());
    
    // Track the current auto-increment value for numeric enums
    int currentValue = 0;
    
    // Analyze enum members
    for (const auto& member : node.getMembers()) {
        member->accept(*this);
        
        // Determine member value and type
        shared_ptr<Type> memberType = typeSystem_->getNumberType();
        
        if (member->hasValue()) {
            // Member has explicit value
            auto valueType = getExpressionType(*member->getValue());
            memberType = valueType;
            
            // If it's a numeric literal, update the auto-increment counter
            // This is a simplified implementation
            currentValue++;
        } else {
            // Member uses auto-increment (numeric enum)
            memberType = typeSystem_->getNumberType();
            currentValue++;
        }
        
        // Add member to enum scope as a constant
        declareSymbol(member->getName(), SymbolKind::Variable, memberType, member->getLocation());
    }
    
    // Exit enum scope
    exitScope();
    
    // EnumDeclaration is not an Expression, so we don't call setExpressionType
    // The enum type is already stored in the symbol table above
}

void SemanticAnalyzer::visit(TypeAliasDeclaration& node) {
    // Create alias type
    auto aliasType = typeSystem_->createAliasType(node.getName(), node.getAliasedType(), &node);
    
    // Add type alias to symbol table
    declareSymbol(node.getName(), SymbolKind::Type, aliasType, node.getLocation());
    
    // TypeAliasDeclaration is not an Expression, so we don't call setExpressionType
    // The alias type is already stored in the symbol table above
}

shared_ptr<Type> SemanticAnalyzer::resolveType(shared_ptr<Type> type) {
    if (!type) {
        return type;
    }
    
    if (type->getKind() != TypeKind::Unresolved) {
        // Handle GenericType that might have unresolved components
        if (type->getKind() == TypeKind::Generic) {
            auto genericType = std::static_pointer_cast<GenericType>(type);
            
            // Resolve base type
            auto resolvedBaseType = resolveType(genericType->getBaseType());
            
            // Resolve type arguments
            std::vector<shared_ptr<Type>> resolvedTypeArgs;
            bool needsReconstruction = false;
            
            for (const auto& typeArg : genericType->getTypeArguments()) {
                auto resolvedTypeArg = resolveType(typeArg);
                resolvedTypeArgs.push_back(resolvedTypeArg);
                if (resolvedTypeArg != typeArg) {
                    needsReconstruction = true;
                }
            }
            
            // If base type or any type argument changed, create new GenericType
            if (resolvedBaseType != genericType->getBaseType() || needsReconstruction) {
                return typeSystem_->createGenericType(resolvedBaseType, std::move(resolvedTypeArgs));
            }
            
            return type; // No changes needed
        }
        
        // Handle ClassType without declaration pointer
        if (type->getKind() == TypeKind::Class) {
            auto classType = std::static_pointer_cast<ClassType>(type);
            if (!classType->getDeclaration()) {
                // Try to find the declaration in the symbol table
                Symbol* symbol = resolveSymbol(classType->getName(), SourceLocation());
                if (symbol && symbol->getKind() == SymbolKind::Type && symbol->getDeclaration()) {
                    if (auto classDecl = dynamic_cast<ClassDeclaration*>(symbol->getDeclaration())) {
                        classType->setDeclaration(classDecl);
                    }
                }
            }
        }
        return type; // Already resolved or not an unresolved type
    }
    
    const auto& unresolvedType = static_cast<const UnresolvedType&>(*type);
    String typeName = unresolvedType.getName();
    
    // Look up the type name in the symbol table
    Symbol* symbol = resolveSymbol(typeName, SourceLocation());
    if (symbol && (symbol->getKind() == SymbolKind::Type || symbol->getKind() == SymbolKind::Class)) {
        auto resolvedType = symbol->getType();
        
        // Always ensure ClassType has correct declaration pointer
        if (resolvedType->getKind() == TypeKind::Class) {
            auto classType = std::static_pointer_cast<ClassType>(resolvedType);
            if (!classType->getDeclaration() && symbol->getDeclaration()) {
                // Set the declaration pointer if it's missing
                if (auto classDecl = dynamic_cast<ClassDeclaration*>(symbol->getDeclaration())) {
                    classType->setDeclaration(classDecl);
                }
            }
        }
        
        return resolvedType; // Return the resolved type
    }
    
    // If not found, return error type
    return typeSystem_->getErrorType();
}

shared_ptr<Type> SemanticAnalyzer::findClassMember(const ClassType& classType, const String& memberName) {
    
    ClassDeclaration* classDecl = classType.getDeclaration();
    
    // If the ClassType doesn't have a declaration pointer, try to find it in the symbol table
    if (!classDecl) {
        Symbol* symbol = resolveSymbol(classType.getName(), SourceLocation());
        if (symbol && symbol->getKind() == SymbolKind::Type && symbol->getDeclaration()) {
            classDecl = dynamic_cast<ClassDeclaration*>(symbol->getDeclaration());
        }
    }
    
    if (!classDecl) {
        return nullptr;
    }
    
    // Debug: check what methods are in the class
    // (This would normally be removed in production code)
    
    // Look for property in current class
    for (const auto& property : classDecl->getProperties()) {
        if (property->getName() == memberName) {
            return getDeclarationType(*property);
        }
    }
    
    // Look for method in current class
    for (const auto& method : classDecl->getMethods()) {
        if (method->getName() == memberName) {
            return getDeclarationType(*method);
        }
    }
    
    // If not found in current class, look in base class
    if (classDecl->getBaseClass()) {
        auto resolvedBaseType = resolveType(classDecl->getBaseClass());
        if (resolvedBaseType) {
            if (resolvedBaseType->getKind() == TypeKind::Class) {
                const auto& baseClassType = static_cast<const ClassType&>(*resolvedBaseType);
                return findClassMember(baseClassType, memberName);
            } else if (resolvedBaseType->getKind() == TypeKind::Generic) {
                // Handle generic base classes (e.g., MiddleGeneric<T, U>)
                const auto& genericBaseType = static_cast<const GenericType&>(*resolvedBaseType);
                auto baseType = genericBaseType.getBaseType();
                if (baseType->getKind() == TypeKind::Class) {
                    const auto& baseClassType = static_cast<const ClassType&>(*baseType);
                    return findClassMember(baseClassType, memberName);
                }
            }
        }
    }
    
    // Member not found
    return nullptr;
}

// Helper method to find member type on a given type
shared_ptr<Type> SemanticAnalyzer::findMemberType(shared_ptr<Type> type, const String& memberName) {
    if (!type) {
        return nullptr;
    }
    
    // Handle primitive types
    if (type->getKind() == TypeKind::String) {
        if (memberName == "length") {
            return typeSystem_->getNumberType();
        } else if (memberName == "charAt" || memberName == "substring" || memberName == "indexOf") {
            return typeSystem_->getStringType();
        }
    } else if (type->getKind() == TypeKind::Array) {
        if (memberName == "length") {
            return typeSystem_->getNumberType();
        } else if (memberName == "push" || memberName == "pop" || memberName == "shift" || memberName == "unshift") {
            // These methods would return the array type or element type
            return type;
        }
    } else if (type->getKind() == TypeKind::Class) {
        // For class types, we would need to look up the class definition
        // and find the member. For now, we'll handle basic cases.
        const auto& classType = static_cast<const ClassType&>(*type);
        String className = classType.getName();
        
        // Basic built-in properties for common types
        if (className == "String" && memberName == "length") {
            return typeSystem_->getNumberType();
        } else if (className == "Array" && memberName == "length") {
            return typeSystem_->getNumberType();
        }
    }
    
    return nullptr;
}

void SemanticAnalyzer::collectNestedFunctionDeclarations(const Statement& stmt) {
    // Recursively collect function declarations from nested statements
    if (auto blockStmt = dynamic_cast<const BlockStatement*>(&stmt)) {
        // Process all statements in the block
        for (const auto& nestedStmt : blockStmt->getStatements()) {
            if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(nestedStmt.get())) {
                // Create function type for nested function
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
                
                // Add nested function symbol to current scope
                if (!symbolTable_->addSymbol(funcDecl->getName(), SymbolKind::Function, functionType,
                                             funcDecl->getLocation(), funcDecl)) {
                    // If adding to current scope fails, try adding to global scope
                    // This handles the case where we're collecting from module level
                    if (!symbolTable_->addSymbol(funcDecl->getName(), SymbolKind::Function, functionType,
                                                 funcDecl->getLocation(), funcDecl)) {
                        reportError("Failed to declare nested function: " + funcDecl->getName(), funcDecl->getLocation());
                    }
                }
                
                // Recursively collect nested functions from this function's body
                if (funcDecl->getBody()) {
                    collectNestedFunctionDeclarations(*funcDecl->getBody());
                }
            } else {
                // Recursively process other statements that might contain nested functions
                collectNestedFunctionDeclarations(*nestedStmt);
            }
        }
    } else if (auto ifStmt = dynamic_cast<const IfStatement*>(&stmt)) {
        // Process if statement branches
        if (ifStmt->getThenStatement()) {
            collectNestedFunctionDeclarations(*ifStmt->getThenStatement());
        }
        if (ifStmt->getElseStatement()) {
            collectNestedFunctionDeclarations(*ifStmt->getElseStatement());
        }
    } else if (auto whileStmt = dynamic_cast<const WhileStatement*>(&stmt)) {
        // Process while statement body
        if (whileStmt->getBody()) {
            collectNestedFunctionDeclarations(*whileStmt->getBody());
        }
    } else if (auto forStmt = dynamic_cast<const ForStatement*>(&stmt)) {
        // Process for statement body
        if (forStmt->getBody()) {
            collectNestedFunctionDeclarations(*forStmt->getBody());
        }
    } else if (auto tryStmt = dynamic_cast<const TryStatement*>(&stmt)) {
        // Process try statement blocks
        if (tryStmt->getTryBlock()) {
            collectNestedFunctionDeclarations(*tryStmt->getTryBlock());
        }
        if (tryStmt->getCatchClause()) {
            // CatchClause is not a Statement, so we need to process its body
            if (tryStmt->getCatchClause()->getBody()) {
                collectNestedFunctionDeclarations(*tryStmt->getCatchClause()->getBody());
            }
        }
        if (tryStmt->getFinallyBlock()) {
            collectNestedFunctionDeclarations(*tryStmt->getFinallyBlock());
        }
    }
    // Note: Other statement types (return, break, continue, etc.) don't contain nested functions
}

void SemanticAnalyzer::markCurrentFunctionAsCaptured() {
    // Mark the current function as captured
    if (!functionStack_.empty()) {
        FunctionDeclaration* currentFunction = functionStack_.back();
        currentFunction->setCaptured(true);
        
        // Add a warning that this function captures variables
        diagnostics_.warning("Function '" + currentFunction->getName() + "' captures variables from outer scope", 
                            currentFunction->getLocation());
    }
}

void SemanticAnalyzer::markVariableAsCaptured(Symbol* symbol) {
    // Mark the current function as captured and add the variable to the captured list
    if (!functionStack_.empty()) {
        FunctionDeclaration* currentFunction = functionStack_.back();
        currentFunction->setCaptured(true);
        currentFunction->addCapturedVariable(symbol);
        
        // Debug output
        diagnostics_.warning("Captured variable: " + symbol->getName() + " in function: " + currentFunction->getName(), 
                            currentFunction->getLocation());
    }
}

// Factory function
unique_ptr<SemanticAnalyzer> createSemanticAnalyzer(DiagnosticEngine& diagnostics) {
    return make_unique<SemanticAnalyzer>(diagnostics);
}

} // namespace tsc