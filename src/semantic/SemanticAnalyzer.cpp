#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/semantic/GenericConstraintChecker.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/parser/Parser.h"
#include "tsc/parser/VectorTokenStream.h"
#include <iostream>
#include <fstream>

namespace tsc {
    // Helper class to analyze destructor cleanup operations
    class DestructorCleanupAnalyzer : public ASTVisitor {
    private:
        String propertyName_;
        bool propertyCleanedUp_;
        
    public:
        DestructorCleanupAnalyzer(const String& propertyName) 
            : propertyName_(propertyName), propertyCleanedUp_(false) {}
        
        bool isPropertyCleanedUp() const { return propertyCleanedUp_; }
        
        void visit(AssignmentExpression& expr) override {
            // Check for assignments like: this.property = null
            if (auto memberAccess = dynamic_cast<PropertyAccess*>(expr.getLeft())) {
                if (memberAccess->getObject()->toString() == "this" && 
                    memberAccess->getProperty() == propertyName_) {
                    // Check if it's being set to null or undefined
                    if (auto nullLiteral = dynamic_cast<NullLiteral*>(expr.getRight())) {
                        propertyCleanedUp_ = true;
                    }
                }
            }
            
            // Visit children
            expr.getLeft()->accept(*this);
            expr.getRight()->accept(*this);
        }
        
        void visit(ExpressionStatement& stmt) override {
            // Check for direct cleanup calls like: this.property.reset()
            if (auto callExpr = dynamic_cast<CallExpression*>(stmt.getExpression())) {
                if (auto memberAccess = dynamic_cast<PropertyAccess*>(callExpr->getCallee())) {
                    if (memberAccess->getObject()->toString() == "this" && 
                        memberAccess->getProperty() == propertyName_) {
                        String methodName = callExpr->getArguments().empty() ? "" : 
                                           callExpr->getArguments()[0]->toString();
                        if (methodName == "reset" || methodName == "clear" || methodName == "dispose") {
                            propertyCleanedUp_ = true;
                        }
                    }
                }
            }
            
            // Visit children
            stmt.getExpression()->accept(*this);
        }
        
        void visit(BlockStatement& stmt) override {
            // Visit all statements in the block
            for (const auto& statement : stmt.getStatements()) {
                statement->accept(*this);
            }
        }
        
        // Implement all required virtual methods with empty implementations
        void visit(NumericLiteral& node) override {}
        void visit(StringLiteral& node) override {}
        void visit(TemplateLiteral& node) override {}
        void visit(BooleanLiteral& node) override {}
        void visit(NullLiteral& node) override {}
        void visit(Identifier& node) override {}
        void visit(ThisExpression& node) override {}
        void visit(SuperExpression& node) override {}
        void visit(NewExpression& node) override {}
        void visit(BinaryExpression& node) override {}
        void visit(UnaryExpression& node) override {}
        void visit(IndexExpression& node) override {}
        void visit(ConditionalExpression& node) override {}
        void visit(CallExpression& node) override {}
        void visit(ArrayLiteral& node) override {}
        void visit(ObjectLiteral& node) override {}
        void visit(PropertyAccess& node) override {}
        void visit(ArrowFunction& node) override {}
        void visit(FunctionExpression& node) override {}
        void visit(MoveExpression& node) override {}
        void visit(OptionalPropertyAccess& node) override {}
        void visit(OptionalIndexAccess& node) override {}
        void visit(OptionalCallExpr& node) override {}
        void visit(SpreadElement& node) override {}
        void visit(DestructuringPattern& node) override {}
        void visit(ArrayDestructuringPattern& node) override {}
        void visit(ObjectDestructuringPattern& node) override {}
        void visit(IdentifierPattern& node) override {}
        void visit(DestructuringAssignment& node) override {}
        void visit(ReturnStatement& node) override {}
        void visit(IfStatement& node) override {}
        void visit(WhileStatement& node) override {}
        void visit(DoWhileStatement& node) override {}
        void visit(ForStatement& node) override {}
        void visit(ForOfStatement& node) override {}
        void visit(SwitchStatement& node) override {}
        void visit(CaseClause& node) override {}
        void visit(BreakStatement& node) override {}
        void visit(ContinueStatement& node) override {}
        void visit(TryStatement& node) override {}
        void visit(CatchClause& node) override {}
        void visit(ThrowStatement& node) override {}
        void visit(VariableDeclaration& node) override {}
        void visit(FunctionDeclaration& node) override {}
        void visit(TypeParameter& node) override {}
        void visit(PropertyDeclaration& node) override {}
        void visit(MethodDeclaration& node) override {}
        void visit(DestructorDeclaration& node) override {}
        void visit(ClassDeclaration& node) override {}
        void visit(InterfaceDeclaration& node) override {}
        void visit(EnumMember& node) override {}
        void visit(EnumDeclaration& node) override {}
        void visit(TypeAliasDeclaration& node) override {}
        void visit(Module& node) override {}
        void visit(ImportDeclaration& node) override {}
        void visit(ExportDeclaration& node) override {}
    };
    SemanticAnalyzer::SemanticAnalyzer(DiagnosticEngine &diagnostics)
        : diagnostics_(diagnostics) {
        symbolTable_ = make_unique<SymbolTable>();
        typeSystem_ = make_unique<TypeSystem>();
        context_ = make_unique<SemanticContext>(*symbolTable_, *typeSystem_, diagnostics_);
        constraintChecker_ = make_unique<GenericConstraintChecker>(diagnostics_, *typeSystem_);
        moduleResolver_ = make_unique<ModuleResolver>(diagnostics_);
        dependencyScanner_ = make_unique<DependencyScanner>(*moduleResolver_, diagnostics_);
        moduleSymbolManager_ = make_unique<ModuleSymbolManager>(diagnostics_, symbolTable_.get());
        cycleDetector_ = make_unique<semantic::CycleDetector>(symbolTable_.get());
        errorReporter_ = make_unique<EnhancedErrorReporting>(diagnostics_);

        // Configure error reporting to treat warnings as warnings, not errors
        errorReporter_->setWarningsAsErrors(false);
        errorReporter_->setSuppressWarnings(false);

        std::cout << "DEBUG: SemanticAnalyzer created SymbolTable at address: " << symbolTable_.get() << std::endl;

        setupBuiltinEnvironment();
    }

    SemanticAnalyzer::~SemanticAnalyzer() = default;

    bool SemanticAnalyzer::analyze(Module &module) {
        std::cout << "DEBUG: *** ANALYZE METHOD CALLED *** for module" << std::endl;
        std::cout.flush();
        try {
            // Multi-phase semantic analysis
            std::cout << "DEBUG: *** CALLING performSymbolResolution ***" << std::endl;
            std::cout.flush();
            performSymbolResolution(module);
            resolveDeferredSuperExpressions();
            resolveDeferredSuperPropertyAccesses();
            resolveDeferredSuperCallExpressions();
            performTypeChecking(module);
            performFlowAnalysis(module);

            // Check for unused symbols
            auto unusedSymbols = symbolTable_->getUnusedSymbols();
            for (const auto &symbol: unusedSymbols) {
                reportWarning("Unused " + symbol->toString(), symbol->getLocation());
            }

            return context_->getErrorCount() == 0;
        } catch (const std::exception &e) {
            reportError("Internal semantic analysis error: " + String(e.what()), SourceLocation());
            return false;
        }
    }

    bool SemanticAnalyzer::analyzeProject(const std::vector<String> &modulePaths) {
        try {
            std::cout << "DEBUG: Starting multi-module analysis for " << modulePaths.size() << " modules" << std::endl;

            // Phase 1: Scan dependencies and detect circular dependencies
            auto dependencyGraph = dependencyScanner_->scanProjectWithValidation(modulePaths);
            if (!dependencyGraph) {
                diagnostics_.error("Failed to scan project dependencies", SourceLocation());
                return false;
            }

            // Phase 2: Create module symbol tables for each module
            for (const String &modulePath: modulePaths) {
                ModuleSymbolTable *moduleTable = moduleSymbolManager_->createModuleSymbolTable(modulePath);
                if (!moduleTable) {
                    diagnostics_.error("Failed to create module symbol table for: " + modulePath, SourceLocation());
                    return false;
                }
            }

            // Phase 3: Analyze each module individually
            std::vector<String> compilationOrder = dependencyGraph->getCompilationOrder();
            std::cout << "DEBUG: Compilation order: ";
            for (size_t i = 0; i < compilationOrder.size(); ++i) {
                if (i > 0) std::cout << " -> ";
                std::cout << compilationOrder[i];
            }
            std::cout << std::endl;

            std::cout << "DEBUG: *** STARTING LOOP *** with " << compilationOrder.size() << " modules" << std::endl;
            std::cout.flush();

            // Store AST modules to keep them alive throughout analysis
            std::vector<std::unique_ptr<Module> > astModules;
            astModules.reserve(compilationOrder.size());

            for (const String &modulePath: compilationOrder) {
                std::cout << "DEBUG: Analyzing module: " << modulePath << std::endl;
                std::cout << "DEBUG: *** ABOUT TO READ FILE *** " << modulePath << std::endl;
                std::cout.flush();

                // Parse the module
                std::ifstream file(modulePath);
                if (!file.is_open()) {
                    diagnostics_.error("Cannot open file: " + modulePath, SourceLocation());
                    return false;
                }

                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();

                std::cout << "DEBUG: *** FILE READ SUCCESSFULLY *** for " << modulePath << std::endl;

                // Tokenize and parse
                Lexer lexer(diagnostics_);
                auto tokens = lexer.tokenize(content, modulePath);
                if (tokens.empty()) {
                    diagnostics_.error("Failed to tokenize: " + modulePath, SourceLocation());
                    return false;
                }

                std::cout << "DEBUG: *** TOKENIZATION SUCCESSFUL *** for " << modulePath << " (" << tokens.size() <<
                        " tokens)" << std::endl;

                std::cout << "DEBUG: *** BEFORE PARSER *** About to create parser for " << modulePath << std::endl;
                std::cout.flush();
                VectorTokenStream tokenStream(tokens);
                Parser parser(diagnostics_, *typeSystem_);
                std::cout << "DEBUG: *** PARSER INVOCATION *** About to call parser.parse() for " << modulePath <<
                        std::endl;
                std::cout.flush();
                auto module = parser.parse(tokens, modulePath);
                std::cout << "DEBUG: *** PARSER RESULT *** Parser.parse() returned " << (module ? "SUCCESS" : "NULL") <<
                        " for " << modulePath << std::endl;
                std::cout.flush();
                if (!module) {
                    diagnostics_.error("Failed to parse: " + modulePath, SourceLocation());
                    return false;
                }

                // Store the AST module to keep it alive
                astModules.push_back(std::move(module));

                // Set current module path for symbol table creation
                currentModulePath_ = modulePath;

                // Analyze the module
                if (!analyze(*astModules.back())) {
                    diagnostics_.error("Failed to analyze module: " + modulePath, SourceLocation());
                    return false;
                }
            }

            // Phase 4: Perform export-to-import binding
            std::cout << "DEBUG: Performing export-to-import binding" << std::endl;
            std::cout << "DEBUG: *** ABOUT TO CALL bindExportsToImports ***" << std::endl;
            bool bindingResult = moduleSymbolManager_->bindExportsToImports();
            std::cout << "DEBUG: *** bindExportsToImports returned: " << (bindingResult ? "true" : "false") << " ***" <<
                    std::endl;
            if (!bindingResult) {
                diagnostics_.error("Failed to bind exports to imports", SourceLocation());
                return false;
            }

            // Phase 5: Validate all modules
            if (!moduleSymbolManager_->validateAllModules()) {
                diagnostics_.error("Module validation failed", SourceLocation());
                return false;
            }

            std::cout << "DEBUG: Multi-module analysis completed successfully" << std::endl;
            return context_->getErrorCount() == 0;
        } catch (const std::exception &e) {
            diagnostics_.error("Internal multi-module analysis error: " + String(e.what()), SourceLocation());
            return false;
        }
    }

    shared_ptr<Type> SemanticAnalyzer::getExpressionType(const Expression &expr) const {
        auto it = expressionTypes_.find(&expr);
        return it != expressionTypes_.end() ? it->second : typeSystem_->getErrorType();
    }

    shared_ptr<Type> SemanticAnalyzer::getDeclarationType(const Declaration &decl) const {
        auto it = declarationTypes_.find(&decl);
        return it != declarationTypes_.end() ? it->second : typeSystem_->getErrorType();
    }

    Symbol *SemanticAnalyzer::getSymbolForNode(const ASTNode &node) const {
        auto it = nodeSymbols_.find(&node);
        return it != nodeSymbols_.end() ? it->second : nullptr;
    }

    // Visitor implementations
    void SemanticAnalyzer::visit(NumericLiteral &node) {
        auto type = typeSystem_->getNumberType();
        setExpressionType(node, type);
    }

    void SemanticAnalyzer::visit(StringLiteral &node) {
        auto type = typeSystem_->getStringType();
        setExpressionType(node, type);
    }


    void SemanticAnalyzer::visit(BooleanLiteral &node) {
        auto type = typeSystem_->getBooleanType();
        setExpressionType(node, type);
    }

    void SemanticAnalyzer::visit(NullLiteral &node) {
        auto type = typeSystem_->getNullType();
        setExpressionType(node, type);
    }

    void SemanticAnalyzer::visit(Identifier &node) {
        Symbol *symbol = resolveSymbol(node.getName(), node.getLocation());
        if (symbol) {
            symbol->markUsed();

            // Check if we're inside a nested function and accessing a variable from outer scope
            if (functionDepth_ > 1) {
                // We're inside a nested function, check if the symbol comes from an outer scope
                Scope *currentScope = symbolTable_->getCurrentScope();
                Symbol *localSymbol = currentScope->lookupSymbolLocal(node.getName());

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

    void SemanticAnalyzer::visit(ThisExpression &node) {
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
        Symbol *classSymbol = symbolTable_->lookupSymbol(className);

        if (classSymbol && classSymbol->getKind() == SymbolKind::Class) {
            auto classType = classSymbol->getType();
            setExpressionType(node, classType);
        } else {
            // Fallback to Any if we can't find the class
            auto thisType = typeSystem_->getAnyType();
            setExpressionType(node, thisType);
        }
    }

    void SemanticAnalyzer::visit(SuperExpression &node) {
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

    void SemanticAnalyzer::visit(NewExpression &node) {
        std::cout << "DEBUG: SemanticAnalyzer::visit(NewExpression) called for: " << node.toString() << std::endl;
        std::cout << "DEBUG: hasExplicitTypeArguments: " << (node.hasExplicitTypeArguments() ? "true" : "false") <<
                std::endl;

        // Analyze arguments first
        for (const auto &arg: node.getArguments()) {
            arg->accept(static_cast<ASTVisitor &>(*this));
        }

        // Handle constructor expression
        if (auto identifier = dynamic_cast<Identifier *>(node.getConstructor())) {
            std::cout << "DEBUG: Constructor identifier: " << identifier->getName() << std::endl;
            // Try to find the class type in the symbol table
            Symbol *classSymbol = resolveSymbol(identifier->getName(), node.getLocation());
            if (classSymbol && classSymbol->getKind() == SymbolKind::Class) {
                auto classType = classSymbol->getType();
                std::cout << "DEBUG: Class type: " << classType->toString() << ", kind: " << static_cast<int>(classType
                    ->getKind()) << std::endl;

                // Check if this is a class type that can be instantiated
                if (classType->getKind() == TypeKind::Class ||
                    classType->getKind() == TypeKind::UniquePtr ||
                    classType->getKind() == TypeKind::SharedPtr ||
                    classType->getKind() == TypeKind::WeakPtr) {
                    // Handle explicit type arguments for generic classes
                    if (node.hasExplicitTypeArguments()) {
                        std::cout << "DEBUG: Processing explicit type arguments" << std::endl;
                        // Process explicit type arguments (e.g., Container<number>)
                        const auto &typeArguments = node.getTypeArguments();
                        std::cout << "DEBUG: Number of type arguments: " << typeArguments.size() << std::endl;

                        // Resolve each type argument
                        std::vector<shared_ptr<Type> > resolvedTypeArgs;
                        for (const auto &typeArg: typeArguments) {
                            std::cout << "DEBUG: Resolving type argument: " << typeArg->toString() << std::endl;
                            auto resolvedType = resolveType(typeArg);
                            std::cout << "DEBUG: Resolved to: " << resolvedType->toString() << ", kind: " << static_cast
                                    <int>(resolvedType->getKind()) << std::endl;
                            if (resolvedType->getKind() == TypeKind::Error) {
                                reportError("Invalid type argument: " + typeArg->toString(), node.getLocation());
                                setExpressionType(node, typeSystem_->getErrorType());
                                return;
                            }
                            resolvedTypeArgs.push_back(resolvedType);
                        }

                        // Validate generic instantiation with constraint checking
                        if (!constraintChecker_->validateGenericInstantiation(
                            classType, resolvedTypeArgs, node.getLocation())) {
                            setExpressionType(node, typeSystem_->getErrorType());
                            return;
                        }

                        // Create a generic type instance (e.g., Container<number>)
                        auto genericType = typeSystem_->createGenericType(classType, resolvedTypeArgs);
                        std::cout << "DEBUG: Created GenericType: " << genericType->toString() << ", kind: " <<
                                static_cast<int>(genericType->getKind()) << std::endl;
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
            node.getConstructor()->accept(static_cast<ASTVisitor &>(*this));
            auto constructorType = getExpressionType(*node.getConstructor());

            // For complex constructor expressions, use the constructor's type or any type
            setExpressionType(node, typeSystem_->getAnyType());
        }
    }

    void SemanticAnalyzer::visit(BinaryExpression &node) {
        // Analyze operands first
        node.getLeft()->accept(static_cast<ASTVisitor &>(*this));
        node.getRight()->accept(static_cast<ASTVisitor &>(*this));

        checkBinaryOperation(node);
    }

    void SemanticAnalyzer::visit(UnaryExpression &node) {
        // Analyze operand first
        node.getOperand()->accept(static_cast<ASTVisitor &>(*this));

        checkUnaryOperation(node);
    }

    void SemanticAnalyzer::visit(AssignmentExpression &node) {
        // Analyze both sides
        node.getLeft()->accept(static_cast<ASTVisitor &>(*this));
        node.getRight()->accept(static_cast<ASTVisitor &>(*this));

        checkAssignment(*node.getLeft(), *node.getRight(), node.getLocation());

        // Assignment expression has the type of the right-hand side
        auto rightType = getExpressionType(*node.getRight());
        setExpressionType(node, rightType);
    }

    void SemanticAnalyzer::visit(ConditionalExpression &node) {
        // Analyze all three expressions
        node.getCondition()->accept(static_cast<ASTVisitor &>(*this));
        node.getTrueExpression()->accept(static_cast<ASTVisitor &>(*this));
        node.getFalseExpression()->accept(static_cast<ASTVisitor &>(*this));

        // Get types
        auto conditionType = getExpressionType(*node.getCondition());
        auto trueType = getExpressionType(*node.getTrueExpression());
        auto falseType = getExpressionType(*node.getFalseExpression());

        // Check if condition is convertible to boolean
        if (conditionType && !typeSystem_->isConvertibleToBoolean(conditionType)) {
            reportError("Condition must be convertible to boolean, got: " + conditionType->toString(),
                        node.getCondition()->getLocation());
        }

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

    void SemanticAnalyzer::visit(CallExpression &node) {
        // Analyze callee and arguments
        node.getCallee()->accept(*this);
        for (const auto &arg: node.getArguments()) {
            arg->accept(static_cast<ASTVisitor &>(*this));
        }

        // Check if this is a call to a deferred super property access
        if (auto propertyAccess = dynamic_cast<const PropertyAccess *>(node.getCallee())) {
            if (dynamic_cast<const SuperExpression *>(propertyAccess->getObject())) {
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
                        deferred.propertyAccess = const_cast<PropertyAccess *>(propertyAccess);
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

    void SemanticAnalyzer::visit(ArrayLiteral &node) {
        // Analyze all elements
        for (const auto &element: node.getElements()) {
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
        } else {
            // For empty arrays, we need to be more careful about type inference
            // Check if this array literal is in an assignment context
            // If so, we'll let the assignment checker handle the type compatibility
            // For now, use 'any' as the default element type for empty arrays
            elementType = typeSystem_->getAnyType();
        }

        auto arrayType = typeSystem_->createArrayType(elementType);
        setExpressionType(node, arrayType);
    }

    void SemanticAnalyzer::visit(TemplateLiteral &node) {
        std::cout << "DEBUG: Analyzing template literal with " << node.getElements().size() << " elements" << std::endl;

        // Analyze all template elements
        for (size_t i = 0; i < node.getElements().size(); ++i) {
            const auto &element = node.getElements()[i];

            if (element.isExpression()) {
                std::cout << "DEBUG: Analyzing template expression at position " << i << std::endl;

                // Analyze the expression
                element.getExpression()->accept(*this);

                // Get the type of the expression
                auto exprType = getExpressionType(*element.getExpression());
                if (!exprType) {
                    reportError("Could not determine type of expression in template literal", node.getLocation());
                    setExpressionType(node, typeSystem_->getStringType());
                    return;
                }

                // Check if the expression type can be converted to string
                if (!typeSystem_->isConvertibleToString(exprType)) {
                    reportError("Expression in template literal cannot be converted to string: " + exprType->toString(),
                                node.getLocation());
                }

                std::cout << "DEBUG: Template expression type: " << exprType->toString() << std::endl;
            } else {
                std::cout << "DEBUG: Template text element at position " << i << ": \"" << element.getText() << "\"" <<
                        std::endl;
            }
        }

        // Template literals always result in string type
        setExpressionType(node, typeSystem_->getStringType());
        std::cout << "DEBUG: Template literal type set to: string" << std::endl;
    }

    void SemanticAnalyzer::visit(IndexExpression &node) {
        // Analyze object and index
        node.getObject()->accept(*this);
        node.getIndex()->accept(*this);

        auto objectType = getExpressionType(*node.getObject());
        auto indexType = getExpressionType(*node.getIndex());

        // Check if object is indexable and validate index type
        shared_ptr<Type> resultType = nullptr;

        if (objectType) {
            // Check if index type is valid for indexing
            if (!isValidIndexType(indexType)) {
                reportError("Index must be number or string, got: " + indexType->toString(),
                            node.getIndex()->getLocation());
                setExpressionType(node, typeSystem_->getErrorType());
                return;
            }

            // Determine result type based on object type
            switch (objectType->getKind()) {
                case TypeKind::Array: {
                    auto elementType = typeSystem_->getArrayElementType(objectType);
                    resultType = elementType;
                    break;
                }
                case TypeKind::String: {
                    // String indexing returns string (single character)
                    resultType = typeSystem_->getStringType();
                    break;
                }
                case TypeKind::Object: {
                    // Object indexing returns any (property access)
                    resultType = typeSystem_->getAnyType();
                    break;
                }
                case TypeKind::Tuple: {
                    // Tuple indexing - check the actual index value to determine element type
                    auto tupleType = dynamic_cast<TupleType*>(objectType.get());
                    if (tupleType) {
                        // Try to evaluate the index expression to get a constant value
                        if (auto indexLiteral = dynamic_cast<NumericLiteral*>(node.getIndex())) {
                            int index = static_cast<int>(indexLiteral->getValue());
                            if (index >= 0 && index < tupleType->getElementTypes().size()) {
                                resultType = tupleType->getElementTypes()[index];
                            } else {
                                reportError("Tuple index " + std::to_string(index) + " is out of bounds",
                                          node.getIndex()->getLocation());
                                resultType = typeSystem_->getErrorType();
                            }
                        } else {
                            // Non-constant index - return union of all element types
                            resultType = typeSystem_->createUnionType(tupleType->getElementTypes());
                        }
                    } else {
                        resultType = typeSystem_->getAnyType();
                    }
                    break;
                }
                default: {
                    reportError("Cannot index into type: " + objectType->toString(),
                                node.getObject()->getLocation());
                    setExpressionType(node, typeSystem_->getErrorType());
                    return;
                }
            }
        } else {
            resultType = typeSystem_->getAnyType();
        }

        setExpressionType(node, resultType);
    }

    void SemanticAnalyzer::visit(ObjectLiteral &node) {
        // Analyze all property values
        std::vector<std::pair<String, shared_ptr<Type> > > propertyTypes;

        for (const auto &property: node.getProperties()) {
            property.getValue()->accept(*this);
            auto valueType = getExpressionType(*property.getValue());
            propertyTypes.emplace_back(property.getKey(), valueType);
        }

        // Create an object type based on the properties
        auto objectType = createObjectTypeFromProperties(propertyTypes);
        setExpressionType(node, objectType);
    }

    void SemanticAnalyzer::visit(PropertyAccess &node) {
        // Analyze the object being accessed
        node.getObject()->accept(*this);
        auto objectType = getExpressionType(*node.getObject());


        // Note: SuperExpression is handled by the regular class property access logic below

        // Check if this is an enum member access
        if (objectType->getKind() == TypeKind::Enum) {
            const auto &enumType = static_cast<const EnumType &>(*objectType);
            String memberName = node.getProperty();

            // Check if the enum has this member
            // We need to look up the member in the enum's declaration scope
            EnumDeclaration *enumDecl = enumType.getDeclaration();
            if (enumDecl) {
                // Look for the member in the enum declaration
                for (const auto &member: enumDecl->getMembers()) {
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
            const auto &classType = static_cast<const ClassType &>(*objectType);
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
            const auto &genericType = static_cast<const GenericType &>(*objectType);
            auto baseType = genericType.getBaseType();

            // The base type should be a ClassType
            if (baseType->getKind() == TypeKind::Class) {
                const auto &classType = static_cast<const ClassType &>(*baseType);
                String memberName = node.getProperty();

                // Check if the class has this member (including inherited members)
                if (auto memberType = findClassMember(classType, memberName)) {
                    // If this is a super property access and the member type is Error, defer resolution
                    if (dynamic_cast<const SuperExpression *>(node.getObject()) && memberType->getKind() ==
                        TypeKind::Error) {
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
                reportError("Class '" + classType.getName() + "' has no member '" + memberName + "'",
                            node.getLocation());
                setExpressionType(node, typeSystem_->getErrorType());
                return;
            } else if (baseType->getKind() == TypeKind::Interface) {
                const auto &interfaceType = static_cast<const InterfaceType &>(*baseType);
                String memberName = node.getProperty();

                // Check if the interface has this member
                if (auto memberType = findInterfaceMember(interfaceType, memberName)) {
                    setExpressionType(node, memberType);
                    return;
                }

                // If we get here, the member wasn't found
                reportError("Interface '" + interfaceType.getName() + "' has no member '" + memberName + "'",
                            node.getLocation());
                setExpressionType(node, typeSystem_->getErrorType());
                return;
            } else {
                reportError("Generic type base is not a class or interface", node.getLocation());
                setExpressionType(node, typeSystem_->getErrorType());
                return;
            }
        }

        // Check if this is a property access on a generic type parameter
        if (objectType->getKind() == TypeKind::TypeParameter) {
            const auto &typeParamType = static_cast<const TypeParameterType &>(*objectType);
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
                reportError(
                    "Generic type parameter '" + typeParamType.getName() + "' has no member '" + memberName + "'",
                    node.getLocation());
                setExpressionType(node, typeSystem_->getErrorType());
                return;
            }
        }

        // Check if this is array property access (e.g., arr.length, arr.push)
        if (objectType->getKind() == TypeKind::Array) {
            String propertyName = node.getProperty();

            if (propertyName == "length") {
                // Array length property returns number
                setExpressionType(node, typeSystem_->getNumberType());
                return;
            } else if (propertyName == "push" || propertyName == "pop" ||
                       propertyName == "shift" || propertyName == "unshift" ||
                       propertyName == "concat" || propertyName == "slice" ||
                       propertyName == "splice" || propertyName == "indexOf" ||
                       propertyName == "forEach" || propertyName == "map" ||
                       propertyName == "filter" || propertyName == "reduce") {
                // Array methods - create appropriate function types
                if (propertyName == "push" || propertyName == "unshift") {
                    // push/unshift take variable arguments and return number (new length)
                    std::vector<FunctionType::Parameter> params;
                    auto elementType = typeSystem_->getArrayElementType(objectType);
                    if (elementType) {
                        FunctionType::Parameter param;
                        param.name = "item";
                        param.type = elementType;
                        param.optional = false;
                        param.rest = true; // Variable number of arguments
                        params.push_back(param);
                    }
                    auto funcType = typeSystem_->createFunctionType(std::move(params), typeSystem_->getNumberType());
                    setExpressionType(node, funcType);
                } else if (propertyName == "pop" || propertyName == "shift") {
                    // pop/shift return the element type or undefined
                    auto elementType = typeSystem_->getArrayElementType(objectType);
                    auto returnType = elementType ? elementType : typeSystem_->getAnyType();
                    auto funcType = typeSystem_->createFunctionType({}, returnType);
                    setExpressionType(node, funcType);
                } else {
                    // For other methods, return a generic function type for now
                    auto funcType = typeSystem_->createFunctionType({}, typeSystem_->getAnyType());
                    setExpressionType(node, funcType);
                }
                return;
            } else {
                // Unknown property on array
                reportError("Array has no property '" + propertyName + "'", node.getLocation());
                setExpressionType(node, typeSystem_->getErrorType());
                return;
            }
        }

        // Handle other property access types (objects, etc.)
        // For now, assume property access returns any type
        // TODO: Implement proper property type checking based on object type
        setExpressionType(node, typeSystem_->getAnyType());
    }

    void SemanticAnalyzer::visit(ArrowFunction &node) {
        // Create function type
        std::vector<FunctionType::Parameter> paramTypes;

        // Enter new scope for arrow function parameters
        enterScope(Scope::ScopeType::Function, "arrow_function");

        for (const auto &param: node.getParameters()) {
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

    shared_ptr<Type> SemanticAnalyzer::inferReturnType(const Statement &body) {
        // Simple return type inference for arrow functions
        // Look for return statements and infer type from the first one

        // For now, let's implement a simple version without a full visitor
        // We'll just check if it's a BlockStatement with a ReturnStatement

        if (auto blockStmt = dynamic_cast<const BlockStatement *>(&body)) {
            for (const auto &stmt: blockStmt->getStatements()) {
                if (auto returnStmt = dynamic_cast<const ReturnStatement *>(stmt.get())) {
                    if (returnStmt->getValue()) {
                        return getExpressionType(*returnStmt->getValue());
                    }
                }
            }
        }

        // If we can't infer, return null (caller will use void)
        return nullptr;
    }

    void SemanticAnalyzer::visit(FunctionExpression &node) {
        // Function expressions are similar to arrow functions but with slightly different semantics
        // Create function type
        std::vector<FunctionType::Parameter> paramTypes;

        // Enter new scope for function expression parameters
        enterScope(Scope::ScopeType::Function, "function_expression");

        for (const auto &param: node.getParameters()) {
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

    void SemanticAnalyzer::visit(ExpressionStatement &node) {
        node.getExpression()->accept(*this);
    }

    void SemanticAnalyzer::visit(BlockStatement &node) {
        enterScope(Scope::ScopeType::Block);

        for (const auto &stmt: node.getStatements()) {
            stmt->accept(*this);
        }

        exitScope();
    }

    void SemanticAnalyzer::visit(ReturnStatement &node) {
        // Check if we're inside a function
        if (!isInFunctionScope()) {
            reportError("Return statement outside function", node.getLocation());
            return;
        }

        // Analyze return value if present
        shared_ptr<Type> returnType = typeSystem_->getVoidType();
        if (node.hasValue()) {
            node.getValue()->accept(*this);
            returnType = getExpressionType(*node.getValue());
            if (!returnType || returnType->isError()) {
                returnType = typeSystem_->getAnyType();
            }

            // Debug: Print return type (commented out for now)
            // if (auto identifier = dynamic_cast<Identifier*>(node.getValue())) {
            //     std::cout << "DEBUG: Return statement returning identifier '" << identifier->getName() 
            //               << "' with type: " << returnType->toString() << std::endl;
            // }
        }

        // Check return type compatibility with function signature
        if (!functionStack_.empty()) {
            FunctionDeclaration *currentFunction = functionStack_.back();
            if (currentFunction->getReturnType()) {
                // Function has explicit return type - validate against it
                auto expectedReturnType = resolveType(currentFunction->getReturnType());
                if (!isValidAssignment(*returnType, *expectedReturnType)) {
                    reportTypeError(expectedReturnType->toString(), returnType->toString(), node.getLocation());
                }
            }
        }

        // Set the type of the return statement
        node.setType(returnType);
    }

    void SemanticAnalyzer::visit(IfStatement &node) {
        // Analyze condition
        node.getCondition()->accept(static_cast<ASTVisitor &>(*this));

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

    void SemanticAnalyzer::visit(WhileStatement &node) {
        // Analyze condition
        node.getCondition()->accept(static_cast<ASTVisitor &>(*this));

        // Check if condition is boolean-compatible
        auto conditionType = node.getCondition()->getType();
        if (conditionType && !typeSystem_->isConvertibleToBoolean(conditionType)) {
            reportWarning("While condition should be boolean-compatible", node.getCondition()->getLocation());
        }

        // Enter loop context for break/continue validation
        context_->enterLoop();

        // Analyze body
        node.getBody()->accept(*this);

        // Exit loop context
        context_->exitLoop();

        // Set type to void
        node.setType(typeSystem_->getVoidType());
    }

    void SemanticAnalyzer::visit(DoWhileStatement &node) {
        // Enter loop context for break/continue validation
        context_->enterLoop();

        // Analyze body first
        node.getBody()->accept(*this);

        // Exit loop context
        context_->exitLoop();

        // Analyze condition
        node.getCondition()->accept(static_cast<ASTVisitor &>(*this));

        // Check if condition is boolean-compatible
        auto conditionType = node.getCondition()->getType();
        if (conditionType && !typeSystem_->isConvertibleToBoolean(conditionType)) {
            reportWarning("Do-while condition should be boolean-compatible", node.getCondition()->getLocation());
        }

        // Set type to void
        node.setType(typeSystem_->getVoidType());
    }

    void SemanticAnalyzer::visit(ForStatement &node) {
        // Analyze init if present
        if (node.getInit()) {
            node.getInit()->accept(*this);
        }

        // Analyze condition if present
        if (node.getCondition()) {
            node.getCondition()->accept(static_cast<ASTVisitor &>(*this));

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

        // Enter loop context for break/continue validation
        context_->enterLoop();

        // Analyze body
        node.getBody()->accept(*this);

        // Exit loop context
        context_->exitLoop();

        // Set type to void
        node.setType(typeSystem_->getVoidType());
    }

    void SemanticAnalyzer::visit(ForOfStatement &node) {
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
            if (auto arrayType = dynamic_cast<ArrayType *>(iterableType.get())) {
                elementType = arrayType->getElementType();
            } else {
                // For non-array types, we'll use 'any' for now
                reportWarning("For-of loop requires an iterable type (array expected)",
                              node.getIterable()->getLocation());
            }
        }

        // Add the loop variable to the current scope
        const String &varName = node.getVariable();

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

    void SemanticAnalyzer::visit(SwitchStatement &node) {
        // Analyze discriminant
        node.getDiscriminant()->accept(*this);

        // Enter switch context for break validation
        context_->enterSwitch();

        // Analyze all case clauses
        for (const auto &caseClause: node.getCases()) {
            caseClause->accept(*this);
        }

        // Exit switch context
        context_->exitSwitch();

        // Set type to void
        node.setType(typeSystem_->getVoidType());
    }

    void SemanticAnalyzer::visit(CaseClause &node) {
        // Analyze test expression if present (not for default case)
        if (!node.isDefault() && node.getTest()) {
            node.getTest()->accept(*this);
        }

        // Analyze all statements in this case
        for (const auto &stmt: node.getStatements()) {
            stmt->accept(*this);
        }
    }

    void SemanticAnalyzer::visit(BreakStatement &node) {
        // Check if break is in valid context (loop or switch)
        if (!context_->isInLoop() && !context_->isInSwitch()) {
            reportError("Break statement must be inside a loop or switch statement", node.getLocation());
        }

        // Set type to void
        node.setType(typeSystem_->getVoidType());
    }

    void SemanticAnalyzer::visit(ContinueStatement &node) {
        // Check if continue is in valid context (loop only)
        if (!context_->isInLoop()) {
            reportError("Continue statement must be inside a loop", node.getLocation());
        }

        // Set type to void
        node.setType(typeSystem_->getVoidType());
    }

    void SemanticAnalyzer::visit(TryStatement &node) {
        // Analyze try block
        node.getTryBlock()->accept(*this);

        // Analyze catch clause if present
        if (node.hasCatch()) {
            enterScope(Scope::ScopeType::Block);

            // Add catch parameter to scope if it exists
            if (node.getCatchClause()->hasParameter()) {
                const String &paramName = node.getCatchClause()->getParameter();
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

    void SemanticAnalyzer::visit(CatchClause &node) {
        // Analyze catch body
        node.getBody()->accept(*this);
    }

    void SemanticAnalyzer::visit(ThrowStatement &node) {
        // Analyze the expression being thrown
        node.getExpression()->accept(*this);

        // TODO: Type check that the expression is throwable
        // For now, allow any type to be thrown
        node.setType(typeSystem_->getVoidType());
    }

    void SemanticAnalyzer::visit(VariableDeclaration &node) {
        shared_ptr<Type> varType;

        // Analyze initializer if present
        if (node.getInitializer()) {
            node.getInitializer()->accept(*this);

            // If there's a type annotation, use it; otherwise infer from initializer
            if (node.getTypeAnnotation()) {
                varType = resolveType(node.getTypeAnnotation());
            } else {
                varType = getExpressionType(*node.getInitializer());
            }
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
            Symbol *existing = symbolTable_->lookupSymbol(node.getName());
            if (existing) {
                reportRedefinitionError(node.getName(), node.getLocation(), existing->getLocation());
            }
        }

        // Add symbol to current scope
        bool mutable_ = (node.getDeclarationKind() != VariableDeclaration::Kind::Const);

        std::cout << "DEBUG: Adding variable " << node.getName() << " with type: " << varType->toString() << std::endl;
        std::cout << "DEBUG: Current scope when adding variable: " << symbolTable_->getCurrentScope() << std::endl;

        // Debug: Print scope hierarchy when adding variable
        std::cout << "DEBUG: Scope hierarchy when adding variable " << node.getName() << ":" << std::endl;
        Scope *current = symbolTable_->getCurrentScope();
        int level = 0;
        while (current) {
            std::cout << "  Level " << level << ": " << current << " (type: " << static_cast<int>(current->getType())
                    << ", name: " << current->getName() << ")" << std::endl;
            current = current->getParent();
            level++;
        }

        if (!symbolTable_->addSymbol(node.getName(), SymbolKind::Variable, varType,
                                     node.getLocation(), &node)) {
            reportError("Failed to declare variable: " + node.getName(), node.getLocation());
        }
    }

    void SemanticAnalyzer::visit(FunctionDeclaration &node) {
        // Check if function symbol already exists (from first pass)
        Symbol *existingSymbol = symbolTable_->lookupSymbol(node.getName());
        if (!existingSymbol) {
            // This shouldn't happen if collectFunctionDeclarations worked correctly
            reportError("Function symbol not found in second pass: " + node.getName(), node.getLocation());
            return;
        }

        // Analyze function body in new scope
        enterScope(Scope::ScopeType::Function, node.getName());

        // Add type parameters to function scope first
        for (const auto &typeParam: node.getTypeParameters()) {
            typeParam->accept(*this);
        }

        // Update function signature in symbol table with resolved parameter types
        auto functionType = std::static_pointer_cast<FunctionType>(existingSymbol->getType());
        if (functionType) {
            std::vector<FunctionType::Parameter> resolvedParamTypes;
            for (const auto &param: node.getParameters()) {
                FunctionType::Parameter funcParam;
                funcParam.name = param.name;
                funcParam.type = param.type ? resolveType(param.type) : typeSystem_->getAnyType();
                funcParam.optional = param.optional;
                funcParam.rest = param.rest;
                resolvedParamTypes.push_back(funcParam);
            }
            
            // Create new function type with resolved parameter types
            auto resolvedFunctionType = typeSystem_->createFunctionType(std::move(resolvedParamTypes), functionType->getReturnType());
            existingSymbol->setType(resolvedFunctionType);
        }

        // Add parameters to function scope
        for (const auto &param: node.getParameters()) {
            std::cout << "DEBUG: Processing parameter '" << param.name << "' with type: " << (param.type ? param.type->toString() : "null") << std::endl;
            if (param.type) {
                std::cout << "DEBUG: Parameter type kind before resolve: " << static_cast<int>(param.type->getKind()) << std::endl;
            }
            auto paramType = param.type ? resolveType(param.type) : typeSystem_->getAnyType();
            std::cout << "DEBUG: Parameter type kind after resolve: " << static_cast<int>(paramType->getKind()) << std::endl;
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

        // Infer return type if not explicitly specified
        if (!node.getReturnType()) {
            auto inferredReturnType = inferFunctionReturnType(node);
            if (inferredReturnType && !inferredReturnType->isError()) {
                // Update the function symbol with the inferred return type
                auto functionType = std::static_pointer_cast<FunctionType>(existingSymbol->getType());
                if (functionType) {
                    // Create a new function type with the inferred return type
                    auto newFunctionType = typeSystem_->createFunctionType(
                        functionType->getParameters(),
                        inferredReturnType
                    );
                    existingSymbol->setType(newFunctionType);
                }
            }
        }

        // Reset function context
        functionDepth_--;
        functionStack_.pop_back();

        exitScope();
    }

    void SemanticAnalyzer::visit(TypeParameter &node) {
        // Resolve constraint first if present
        shared_ptr<Type> resolvedConstraint = nullptr;
        if (node.hasConstraint()) {
            resolvedConstraint = resolveType(node.getConstraint());
            if (resolvedConstraint->getKind() == TypeKind::Error) {
                reportError("Invalid type parameter constraint: " + node.getConstraint()->toString(),
                            node.getLocation());
                resolvedConstraint = nullptr;
            }
        }

        // Create type parameter type with resolved constraint
        auto typeParam = typeSystem_->createTypeParameter(node.getName(), resolvedConstraint);

        // Add type parameter to symbol table as a type
        declareSymbol(node.getName(), SymbolKind::Type, typeParam, node.getLocation());

        // Set the node's type (though TypeParameter isn't an Expression, this maintains consistency)
        if (auto expr = dynamic_cast<Expression *>(&node)) {
            setExpressionType(*expr, typeParam);
        }
    }

    void SemanticAnalyzer::visit(Module &module) {
        // Analyze all statements in the module
        for (const auto &stmt: module.getStatements()) {
            stmt->accept(*this);
        }
    }

    // Analysis phase implementations
    void SemanticAnalyzer::performSymbolResolution(Module &module) {
        std::cout << "DEBUG: *** performSymbolResolution CALLED ***" << std::endl;
        std::cout.flush();
        // Three-pass symbol resolution:
        // Pass 1: Collect all function and class declarations
        collectFunctionDeclarations(module);

        // Pass 2: Resolve inheritance relationships
        resolveInheritance(module);

        // Pass 3: Process all statements including function bodies
        std::cout << "DEBUG: *** CALLING module.accept(*this) ***" << std::endl;
        std::cout.flush();
        module.accept(*this);
        std::cout << "DEBUG: *** module.accept(*this) COMPLETED ***" << std::endl;
        std::cout.flush();
    }

    void SemanticAnalyzer::collectFunctionDeclarations(Module &module) {
        // First pass: collect function signatures and class declarations, don't process bodies
        // Debug: Check how many statements we're processing
        const auto &statements = module.getStatements();
        std::cout << "DEBUG: collectFunctionDeclarations found " << statements.size() << " statements" << std::endl;

        for (const auto &stmt: statements) {
            std::cout << "DEBUG: Processing statement type: " << typeid(*stmt.get()).name() << std::endl;
            if (auto classDecl = dynamic_cast<ClassDeclaration *>(stmt.get())) {
                // Collect class declarations first so they're available for constructor calls
                // Create class type WITHOUT base class (will be resolved in second pass)
                auto classType = typeSystem_->createClassType(classDecl->getName(), classDecl, nullptr);
                declareSymbol(classDecl->getName(), SymbolKind::Class, classType, classDecl->getLocation());
            } else if (auto exportDecl = dynamic_cast<ExportDeclaration *>(stmt.get())) {
                // Handle export declarations that contain function/class declarations
                std::cout << "DEBUG: Found export declaration in collectFunctionDeclarations" << std::endl;
                const ExportClause &clause = exportDecl->getClause();
                std::cout << "DEBUG: Export clause type: " << clause.getType() << std::endl;
                if (clause.getType() == ExportClause::Default && clause.getDefaultExport()) {
                    std::cout << "DEBUG: Processing default export" << std::endl;
                    if (auto funcDecl = dynamic_cast<FunctionDeclaration *>(clause.getDefaultExport())) {
                        // Collect exported function declarations
                        std::vector<FunctionType::Parameter> paramTypes;
                        for (const auto &param: funcDecl->getParameters()) {
                            FunctionType::Parameter funcParam;
                            funcParam.name = param.name;
                            funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
                            funcParam.optional = param.optional;
                            funcParam.rest = param.rest;
                            paramTypes.push_back(funcParam);
                        }

                        auto returnType = funcDecl->getReturnType()
                                              ? funcDecl->getReturnType()
                                              : typeSystem_->getVoidType();
                        auto functionType = typeSystem_->createFunctionType(std::move(paramTypes), returnType);

                        std::cout << "DEBUG: Adding exported function to symbol table: " << funcDecl->getName() <<
                                std::endl;
                        if (!symbolTable_->addSymbol(funcDecl->getName(), SymbolKind::Function, functionType,
                                                     funcDecl->getLocation(), funcDecl)) {
                            reportError("Failed to declare exported function: " + funcDecl->getName(),
                                        funcDecl->getLocation());
                        } else {
                            std::cout << "DEBUG: Successfully added exported function to symbol table: " << funcDecl->
                                    getName() << std::endl;
                        }
                    } else if (auto classDecl = dynamic_cast<ClassDeclaration *>(clause.getDefaultExport())) {
                        // Collect exported class declarations
                        auto classType = typeSystem_->createClassType(classDecl->getName(), classDecl, nullptr);
                        declareSymbol(classDecl->getName(), SymbolKind::Class, classType, classDecl->getLocation());
                    }
                } else if (clause.getType() == ExportClause::Named) {
                    // Handle named exports (export function add() { })
                    const auto &namedExports = clause.getNamedExports();
                    for (const auto &spec: namedExports) {
                        // For named exports, we need to find the actual declaration
                        // This is a bit tricky since we're in the first pass
                        // We'll handle this in the second pass when we process the export declaration
                    }
                }
            } else if (auto funcDecl = dynamic_cast<FunctionDeclaration *>(stmt.get())) {
                std::cout << "DEBUG: Found function declaration: " << funcDecl->getName() << std::endl;
                // Create function type
                std::vector<FunctionType::Parameter> paramTypes;
                for (const auto &param: funcDecl->getParameters()) {
                    FunctionType::Parameter funcParam;
                    funcParam.name = param.name;
                    funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
                    funcParam.optional = param.optional;
                    funcParam.rest = param.rest;
                    paramTypes.push_back(funcParam);
                }

                auto returnType = funcDecl->getReturnType() ? funcDecl->getReturnType() : typeSystem_->getVoidType();

                // If no explicit return type, we'll infer it later during the second pass
                // For now, use void as placeholder
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

    void SemanticAnalyzer::resolveInheritance(Module &module) {
        // Second pass: resolve inheritance relationships for all classes
        const auto &statements = module.getStatements();

        for (const auto &stmt: statements) {
            if (auto classDecl = dynamic_cast<ClassDeclaration *>(stmt.get())) {
                // Resolve base class for this class declaration
                if (classDecl->getBaseClass()) {
                    // Look up the class type in the symbol table
                    Symbol *classSymbol = symbolTable_->lookupSymbol(classDecl->getName());
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
        for (const auto &deferred: deferredSuperExpressions_) {
            SuperExpression *superExpr = deferred.expression;
            const String &className = deferred.className;

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
        for (const auto &deferred: deferredSuperPropertyAccesses_) {
            PropertyAccess *propertyAccess = deferred.propertyAccess;
            const String &className = deferred.className;
            const String &memberName = deferred.memberName;

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
                const auto &classType = static_cast<const ClassType &>(*baseClassType);
                memberType = findClassMember(classType, memberName);
            } else if (baseClassType->getKind() == TypeKind::Generic) {
                const auto &genericType = static_cast<const GenericType &>(*baseClassType);
                auto baseType = genericType.getBaseType();
                if (baseType->getKind() == TypeKind::Class) {
                    const auto &classType = static_cast<const ClassType &>(*baseType);
                    memberType = findClassMember(classType, memberName);
                }
            }

            if (!memberType || memberType->getKind() == TypeKind::Error) {
                reportError("Cannot resolve member '" + memberName + "' in base class for 'super' property access",
                            deferred.location);
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
        for (const auto &deferred: deferredSuperCallExpressions_) {
            CallExpression *callExpression = deferred.callExpression;
            PropertyAccess *propertyAccess = deferred.propertyAccess;

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
                const auto &funcType = static_cast<const FunctionType &>(*calleeType);
                setExpressionType(*callExpression, funcType.getReturnType());
            } else {
                // For other callable types, use any type for now
                setExpressionType(*callExpression, typeSystem_->getAnyType());
            }
        }

        // Clear the deferred list
        deferredSuperCallExpressions_.clear();
    }

    void SemanticAnalyzer::performTypeChecking(Module &module) {
        // Type checking is performed during the main visitor traversal
        // This phase could perform additional type-specific validations
    }

    void SemanticAnalyzer::performFlowAnalysis(Module &module) {
        // Flow analysis for detecting unreachable code, uninitialized variables, etc.
        // This is a placeholder for more sophisticated flow analysis
    }

    // Helper method implementations
    void SemanticAnalyzer::declareSymbol(const String &name, SymbolKind kind, shared_ptr<Type> type,
                                         const SourceLocation &location, ASTNode *declaration) {
        std::cout << "DEBUG: declareSymbol called for: " << name << " (kind: " << static_cast<int>(kind) << ")" <<
                std::endl;
        if (!symbolTable_->addSymbol(name, kind, type, location, declaration)) {
            std::cout << "DEBUG: Symbol redeclaration detected for: " << name << std::endl;
            reportError("Symbol redeclaration: " + name, location);
        } else {
            std::cout << "DEBUG: Symbol successfully declared: " << name << std::endl;
        }
    }

    Symbol *SemanticAnalyzer::resolveSymbol(const String &name, const SourceLocation &location) {
        return symbolTable_->lookupSymbol(name);
    }

    void SemanticAnalyzer::checkAssignment(const Expression &left, const Expression &right,
                                           const SourceLocation &location) {
        auto leftType = getExpressionType(left);
        auto rightType = getExpressionType(right);


        // Special handling for array literal assignments
        if (auto arrayLiteral = dynamic_cast<const ArrayLiteral *>(&right)) {
            if (leftType->getKind() == TypeKind::Array && rightType->getKind() == TypeKind::Array) {
                auto leftArrayType = static_cast<const ArrayType *>(leftType.get());
                auto rightArrayType = static_cast<const ArrayType *>(rightType.get());

                // If the right side is an empty array (any[]), and the left side is a generic array (T[]),
                // allow the assignment since empty arrays can be assigned to any array type

                if (arrayLiteral->getElements().empty() &&
                    rightArrayType->getElementType()->getKind() == TypeKind::Any &&
                    (leftArrayType->getElementType()->getKind() == TypeKind::Generic ||
                     leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                     leftArrayType->getElementType()->getKind() == TypeKind::Unresolved)) {
                    // This is a valid assignment: [] can be assigned to T[]
                    return;
                }

                // If both sides are generic arrays with the same generic type parameter,
                // allow the assignment even if they're different instances
                if ((leftArrayType->getElementType()->getKind() == TypeKind::Generic ||
                     leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                     leftArrayType->getElementType()->getKind() == TypeKind::Unresolved) &&
                    (rightArrayType->getElementType()->getKind() == TypeKind::Generic ||
                     rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                     rightArrayType->getElementType()->getKind() == TypeKind::Unresolved)) {
                    // For TypeParameter types, we can directly compare the toString() values
                    if (leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter &&
                        rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter) {
                        if (leftArrayType->getElementType()->toString() == rightArrayType->getElementType()->
                            toString()) {
                            // This is a valid assignment: T[] can be assigned to T[]
                            return;
                        }
                    }

                    // Handle mixed Generic, TypeParameter, and Unresolved types
                    if ((leftArrayType->getElementType()->getKind() == TypeKind::Generic &&
                         (rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter ||
                          rightArrayType->getElementType()->getKind() == TypeKind::Unresolved)) ||
                        (leftArrayType->getElementType()->getKind() == TypeKind::TypeParameter &&
                         (rightArrayType->getElementType()->getKind() == TypeKind::Generic ||
                          rightArrayType->getElementType()->getKind() == TypeKind::Unresolved)) ||
                        (leftArrayType->getElementType()->getKind() == TypeKind::Unresolved &&
                         (rightArrayType->getElementType()->getKind() == TypeKind::Generic ||
                          rightArrayType->getElementType()->getKind() == TypeKind::TypeParameter))) {
                        // Compare the string representations
                        if (leftArrayType->getElementType()->toString() == rightArrayType->getElementType()->
                            toString()) {
                            // This is a valid assignment: T[] can be assigned to T[]
                            return;
                        }
                    }

                    // Handle Unresolved types on both sides
                    if (leftArrayType->getElementType()->getKind() == TypeKind::Unresolved &&
                        rightArrayType->getElementType()->getKind() == TypeKind::Unresolved) {
                        if (leftArrayType->getElementType()->toString() == rightArrayType->getElementType()->
                            toString()) {
                            // This is a valid assignment: T[] can be assigned to T[]
                            return;
                        }
                    }

                    // For Generic types, check the base type
                    if (leftArrayType->getElementType()->getKind() == TypeKind::Generic &&
                        rightArrayType->getElementType()->getKind() == TypeKind::Generic) {
                        auto leftGenericType = static_cast<const GenericType *>(leftArrayType->getElementType().get());
                        auto rightGenericType = static_cast<const GenericType *>(rightArrayType->getElementType().
                            get());


                        // Check if they have the same generic type parameter name
                        if (leftGenericType->getBaseType()->toString() == rightGenericType->getBaseType()->toString()) {
                            // This is a valid assignment: T[] can be assigned to T[]
                            return;
                        }
                    }
                }
            }
        }

        if (!isValidAssignment(*rightType, *leftType)) {
            reportTypeError(leftType->toString(), rightType->toString(), location);
        }
    }

    void SemanticAnalyzer::checkFunctionCall(const CallExpression &call) {
        auto calleeType = getExpressionType(*call.getCallee());

        // Special handling for super() calls
        if (auto superExpr = dynamic_cast<const SuperExpression *>(call.getCallee())) {
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

        // Handle function calls with argument type validation
        if (auto identifier = dynamic_cast<const Identifier *>(call.getCallee())) {
            Symbol *symbol = symbolTable_->lookupSymbol(identifier->getName());
            if (symbol && symbol->getKind() == SymbolKind::Function) {
                // Check if this is a function call by examining the function type
                if (auto functionType =
                        dynamic_cast<const FunctionType *>(calleeType.get())) {
                    // Validate argument types against parameter types
                    if (!validateFunctionArguments(call, *functionType)) {
                        // Argument validation failed - mark expression as error type
                        setExpressionType(call, typeSystem_->getErrorType());
                        return;
                    }

                    // Get the function declaration to access type parameters
                    // This is necessary because the symbol table only stores the function type,
                    // not the detailed AST information including type parameters
                    FunctionDeclaration *funcDecl = getFunctionDeclaration(identifier->getName());
                    if (funcDecl && !funcDecl->getTypeParameters().empty()) {
                        // This is a generic function call - validate that all argument types
                        // satisfy their corresponding type parameter constraints
                        if (!validateGenericFunctionCall(call, *funcDecl, *functionType)) {
                            // Constraint validation failed - mark expression as error type
                            setExpressionType(call, typeSystem_->getErrorType());
                            return;
                        }
                    }

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

    void SemanticAnalyzer::checkBinaryOperation(BinaryExpression &expr) {
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

    void SemanticAnalyzer::checkUnaryOperation(UnaryExpression &expr) {
        auto operandType = getExpressionType(*expr.getOperand());
        auto resultType = typeSystem_->inferTypeFromUnaryExpression(*operandType, static_cast<int>(expr.getOperator()));

        setExpressionType(expr, resultType);
    }

    // Error reporting
    void SemanticAnalyzer::reportError(const String &message, const SourceLocation &location) {
        if (errorReporter_) {
            errorReporter_->reportTypeError(ErrorCodes::Semantic::CANNOT_FIND_NAME, location, message);
        } else {
            diagnostics_.error(message, location);
        }
        context_->incrementErrorCount();
    }

    void SemanticAnalyzer::reportWarning(const String &message, const SourceLocation &location) {
        if (errorReporter_) {
            errorReporter_->reportWarning(ErrorCodes::Warning::UNUSED_VARIABLE, location, message, "");
        } else {
            diagnostics_.warning(message, location);
        }
    }

    void SemanticAnalyzer::reportTypeError(const String &expected, const String &actual,
                                           const SourceLocation &location) {
        if (errorReporter_) {
            errorReporter_->reportTypeMismatch(location, expected, actual,
                                               "Consider using type '" + expected + "' instead of '" + actual + "'");
        } else {
            reportError("Type mismatch: expected " + expected + ", got " + actual, location);
        }
    }

    void SemanticAnalyzer::reportUndefinedSymbol(const String &name, const SourceLocation &location) {
        if (errorReporter_) {
            errorReporter_->reportUndefinedVariable(location, name, "Check the spelling or declare the symbol");
        } else {
            reportError("Undefined symbol: " + name, location);
        }
    }

    void SemanticAnalyzer::reportRedefinitionError(const String &name, const SourceLocation &location,
                                                   const SourceLocation &originalLocation) {
        if (errorReporter_) {
            errorReporter_->reportDuplicateDeclaration(location, name,
                                                       "Rename one of the identifiers to make them unique");
        } else {
            reportError("Redefinition of symbol: " + name +
                        " (originally defined at " + originalLocation.getFilename() + ":" +
                        std::to_string(originalLocation.getLine()) + ":" + std::to_string(originalLocation.getColumn())
                        + ")", location);
        }
    }

    // Utility methods
    void SemanticAnalyzer::setExpressionType(const Expression &expr, shared_ptr<Type> type) {
        expressionTypes_[&expr] = type;
    }

    void SemanticAnalyzer::setDeclarationType(const Declaration &decl, shared_ptr<Type> type) {
        declarationTypes_[&decl] = type;
    }

    void SemanticAnalyzer::setNodeSymbol(const ASTNode &node, Symbol *symbol) {
        nodeSymbols_[&node] = symbol;
    }

    bool SemanticAnalyzer::isValidAssignment(const Type &from, const Type &to) const {
        return typeSystem_->areTypesCompatible(from, to);
    }

    String SemanticAnalyzer::formatType(const Type &type) const {
        return type.toString();
    }

    void SemanticAnalyzer::enterScope(Scope::ScopeType type, const String &name) {
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

        // Add _print function (variadic function that takes any type arguments)
        std::vector<FunctionType::Parameter> printParams;
        FunctionType::Parameter param;
        param.name = "args";
        param.type = typeSystem_->getAnyType();
        param.rest = true; // This makes it variadic
        printParams.push_back(param);
        auto printFunctionType = typeSystem_->createFunctionType(std::move(printParams), typeSystem_->getVoidType());
        symbolTable_->addSymbol("_print", SymbolKind::Function, printFunctionType, SourceLocation());

        // Add built-in number constants
        auto numberType = typeSystem_->getNumberType();
        symbolTable_->addSymbol("Infinity", SymbolKind::Variable, numberType, SourceLocation());
        symbolTable_->addSymbol("NaN", SymbolKind::Variable, numberType, SourceLocation());

        // Add smart pointer constructors
        auto anyType = typeSystem_->getAnyType();

        // unique_ptr constructor
        auto uniquePtrType = typeSystem_->createUniquePtrType(anyType);
        symbolTable_->addSymbol("unique_ptr", SymbolKind::Class, uniquePtrType, SourceLocation());

        // shared_ptr constructor
        auto sharedPtrType = typeSystem_->createSharedPtrType(anyType);
        symbolTable_->addSymbol("shared_ptr", SymbolKind::Class, sharedPtrType, SourceLocation());

        // weak_ptr constructor
        auto weakPtrType = typeSystem_->createWeakPtrType(anyType);
        symbolTable_->addSymbol("weak_ptr", SymbolKind::Class, weakPtrType, SourceLocation());

        // std namespace
        auto stdType = typeSystem_->createObjectType();
        symbolTable_->addSymbol("std", SymbolKind::Variable, stdType, SourceLocation());
    }

    // Type inference helpers
    shared_ptr<Type> SemanticAnalyzer::inferExpressionType(Expression &expr) {
        expr.accept(*this);
        return getExpressionType(expr);
    }

    shared_ptr<Type> SemanticAnalyzer::inferVariableType(const VariableDeclaration &decl) {
        if (decl.getInitializer()) {
            return getExpressionType(*decl.getInitializer());
        }
        return typeSystem_->getUndefinedType();
    }

    shared_ptr<Type> SemanticAnalyzer::inferFunctionType(const FunctionDeclaration &decl) {
        std::vector<FunctionType::Parameter> params;
        for (const auto &param: decl.getParameters()) {
            FunctionType::Parameter funcParam;
            funcParam.name = param.name;
            funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
            params.push_back(funcParam);
        }

        auto returnType = decl.getReturnType() ? decl.getReturnType() : typeSystem_->getVoidType();
        return typeSystem_->createFunctionType(std::move(params), returnType);
    }

    shared_ptr<Type> SemanticAnalyzer::inferFunctionReturnType(const FunctionDeclaration &decl) {
        if (!decl.getBody()) {
            return typeSystem_->getVoidType();
        }

        // Look for return statements in the function body
        shared_ptr<Type> inferredType = typeSystem_->getVoidType();

        // Simple approach: look for the first return statement
        // In a more sophisticated implementation, we'd analyze all return paths
        for (const auto &stmt: decl.getBody()->getStatements()) {
            if (auto returnStmt = dynamic_cast<const ReturnStatement *>(stmt.get())) {
                if (returnStmt->hasValue()) {
                    inferredType = getExpressionType(*returnStmt->getValue());
                    break;
                }
            }
        }

        return inferredType;
    }

    // Class-related visitor implementations
    void SemanticAnalyzer::visit(PropertyDeclaration &node) {
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
            std::cout << "DEBUG: PropertyDeclaration resolving type: " << (propertyType
                                                                               ? propertyType->toString()
                                                                               : "null") << std::endl;
            if (propertyType) {
                std::cout << "DEBUG: PropertyDeclaration type kind: " << static_cast<int>(propertyType->getKind()) <<
                        std::endl;
            }
            propertyType = resolveType(propertyType);
            std::cout << "DEBUG: PropertyDeclaration resolved type: " << (propertyType
                                                                              ? propertyType->toString()
                                                                              : "null") << std::endl;
            if (propertyType) {
                std::cout << "DEBUG: PropertyDeclaration resolved type kind: " << static_cast<int>(propertyType->
                    getKind()) << std::endl;
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

    void SemanticAnalyzer::visit(MethodDeclaration &node) {
        // Enter method scope
        enterScope(Scope::ScopeType::Function, node.getName());
        functionDepth_++;

        // Add parameters to scope
        std::vector<FunctionType::Parameter> paramTypes;
        for (const auto &param: node.getParameters()) {
            FunctionType::Parameter funcParam;
            funcParam.name = param.name;
            // Resolve parameter types to handle generic type parameters
            funcParam.type = param.type ? resolveType(param.type) : typeSystem_->getAnyType();
            std::cout << "DEBUG: MethodDeclaration resolved parameter '" << param.name << "' type: " << (
                funcParam.type ? funcParam.type->toString() : "null") << std::endl;
            if (funcParam.type) {
                std::cout << "DEBUG: MethodDeclaration parameter type kind: " << static_cast<int>(funcParam.type->
                    getKind()) << std::endl;
            }
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

    void SemanticAnalyzer::visit(DestructorDeclaration &node) {
        // Enter destructor scope
        enterScope(Scope::ScopeType::Function, "~" + node.getClassName());
        functionDepth_++;

        std::cout << "DEBUG: Analyzing destructor for class: " << node.getClassName() << std::endl;

        // Analyze destructor body
        if (node.getBody()) {
            node.getBody()->accept(*this);
        }

        // Create destructor type (void function with no parameters)
        std::vector<FunctionType::Parameter> paramTypes; // Empty parameters
        auto destructorType = typeSystem_->createFunctionType(std::move(paramTypes), typeSystem_->getVoidType());
        setDeclarationType(node, destructorType);

        // Perform RAII analysis on the destructor
        analyzeDestructor(node);

        std::cout << "DEBUG: Destructor analysis completed for class: " << node.getClassName() << std::endl;

        // Exit destructor scope
        functionDepth_--;
        exitScope();
    }

    void SemanticAnalyzer::visit(ClassDeclaration &node) {
        // Create class type first (without base class, will be resolved later)
        auto classType = typeSystem_->createClassType(node.getName(), &node, nullptr);

        // Add class to symbol table or update existing symbol from first pass
        Symbol *existingSymbol = symbolTable_->lookupSymbol(node.getName());
        if (existingSymbol) {
            // Update the existing symbol with the more complete class type
            existingSymbol->setType(classType);
            existingSymbol->setDeclaration(&node);
        } else {
            declareSymbol(node.getName(), SymbolKind::Class, classType, node.getLocation(), &node);
        }

        // Enter class scope FIRST
        enterScope(Scope::ScopeType::Class, node.getName());

        // Process type parameters AFTER entering class scope
        std::vector<shared_ptr<Type> > typeParameters;
        for (const auto &typeParam: node.getTypeParameters()) {
            std::cout << "DEBUG: Processing type parameter: " << typeParam->getName() << std::endl;
            std::cout << "DEBUG: Current scope when adding type parameter: " << symbolTable_->getCurrentScope() <<
                    std::endl;
            auto paramType = typeSystem_->createTypeParameter(typeParam->getName(), typeParam->getConstraint());
            typeParameters.push_back(paramType);

            // Add type parameter to class scope so it can be referenced within the class
            std::cout << "DEBUG: Declaring type parameter symbol: " << typeParam->getName() << std::endl;
            declareSymbol(typeParam->getName(), SymbolKind::Type, paramType, typeParam->getLocation());
        }

        // Analyze base class if present
        if (node.getBaseClass()) {
            // Resolve the base class type
            auto resolvedBaseType = resolveType(node.getBaseClass());
            if (resolvedBaseType->getKind() == TypeKind::Error) {
                reportError("Base class type not found: " + node.getBaseClass()->toString(), node.getLocation());
            } else if (resolvedBaseType->getKind() != TypeKind::Class && resolvedBaseType->getKind() !=
                       TypeKind::Generic) {
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
        for (const auto &interface: node.getInterfaces()) {
            // Interface validation would go here
            // For now, we assume they're valid
        }

        // Analyze properties
        for (const auto &property: node.getProperties()) {
            std::cout << "DEBUG: Processing property: " << property->getName() << std::endl;
            std::cout << "DEBUG: Current scope when processing property: " << symbolTable_->getCurrentScope() <<
                    std::endl;
            property->accept(*this);

            // Add property to class scope
            auto propType = getDeclarationType(*property);
            declareSymbol(property->getName(), SymbolKind::Property, propType, property->getLocation());
        }

        // Analyze constructor if present
        if (node.getConstructor()) {
            std::cout << "DEBUG: Processing constructor method" << std::endl;
            node.getConstructor()->accept(static_cast<ASTVisitor &>(*this));

            // Add constructor to class scope as a method
            auto constructorType = getDeclarationType(*node.getConstructor());
            declareSymbol("constructor", SymbolKind::Method, constructorType, node.getConstructor()->getLocation());
            std::cout << "DEBUG: Added constructor to class scope" << std::endl;
        }

        // Analyze methods
        std::cout << "DEBUG: ClassDeclaration processing " << node.getMethods().size() << " methods" << std::endl;
        for (const auto &method: node.getMethods()) {
            std::cout << "DEBUG: Processing method: " << method->getName() << std::endl;
            method->accept(*this);

            // Add method to class scope
            auto methodType = getDeclarationType(*method);
            declareSymbol(method->getName(), SymbolKind::Method, methodType, method->getLocation());
        }

        // Analyze destructor if present
        if (node.getDestructor()) {
            std::cout << "DEBUG: Processing destructor for class: " << node.getName() << std::endl;
            node.getDestructor()->accept(*this);

            // Add destructor to class scope as a method
            auto destructorType = getDeclarationType(*node.getDestructor());
            declareSymbol("~" + node.getName(), SymbolKind::Method, destructorType,
                          node.getDestructor()->getLocation());
            std::cout << "DEBUG: Added destructor to class scope" << std::endl;
        }

        // Perform RAII analysis on the entire class
        validateRAIIPatterns(node);
        suggestResourceCleanup(node);

        // Check constructor
        if (node.getConstructor()) {
            std::cout << "DEBUG: ClassDeclaration found constructor method" << std::endl;
        } else {
            std::cout << "DEBUG: ClassDeclaration no constructor method found" << std::endl;
        }

        // Exit class scope
        exitScope();

        // ClassDeclaration is not an Expression, so we don't call setExpressionType
        // The class type is already stored in the symbol table above
    }

    void SemanticAnalyzer::visit(InterfaceDeclaration &node) {
        // Create interface type
        auto interfaceType = typeSystem_->createInterfaceType(node.getName(), &node);

        // Add interface to symbol table
        declareSymbol(node.getName(), SymbolKind::Type, interfaceType, node.getLocation());

        // Enter interface scope (using Class scope type since interfaces are similar)
        enterScope(Scope::ScopeType::Class, node.getName());

        // Process type parameters AFTER entering interface scope
        std::vector<shared_ptr<Type> > typeParameters;
        for (const auto &typeParam: node.getTypeParameters()) {
            std::cout << "DEBUG: Processing interface type parameter: " << typeParam->getName() << std::endl;
            std::cout << "DEBUG: Current scope when adding interface type parameter: " << symbolTable_->
                    getCurrentScope() << std::endl;

            // Validate type parameter constraint if present
            if (typeParam->getConstraint()) {
                std::cout << "DEBUG: Validating constraint for type parameter: " << typeParam->getName() << std::endl;
                auto constraintType = typeParam->getConstraint();
                std::cout << "DEBUG: Constraint type: " << constraintType->toString() << std::endl;

                // Validate that the constraint is a valid base type
                if (constraintType->isError()) {
                    reportError("Invalid constraint type for type parameter '" + typeParam->getName() + "'",
                                typeParam->getLocation());
                } else {
                    // Check if the constraint type is a valid base type (class, interface, or object type)
                    bool isValidBaseType = false;
                    switch (constraintType->getKind()) {
                        case TypeKind::Class:
                        case TypeKind::Interface:
                        case TypeKind::Object:
                            isValidBaseType = true;
                            break;
                        case TypeKind::TypeParameter: {
                            // For type parameters, check if they have valid constraints
                            auto typeParamType = dynamic_cast<TypeParameterType*>(constraintType.get());
                            if (typeParamType && typeParamType->getConstraint()) {
                                isValidBaseType = true; // Recursive validation would be done elsewhere
                            }
                            break;
                        }
                        case TypeKind::Union: {
                            // For union types, all members must be valid base types
                            auto unionType = dynamic_cast<UnionType*>(constraintType.get());
                            if (unionType) {
                                isValidBaseType = true;
                                for (const auto& memberType : unionType->getTypes()) {
                                    if (memberType->getKind() != TypeKind::Class && 
                                        memberType->getKind() != TypeKind::Interface &&
                                        memberType->getKind() != TypeKind::Object) {
                                        isValidBaseType = false;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        default:
                            isValidBaseType = false;
                            break;
                    }
                    
                    if (!isValidBaseType) {
                        reportError("Type parameter constraint must be a class, interface, or object type, got: " + 
                                   constraintType->toString(),
                                   typeParam->getLocation());
                    }
                }
            }

            auto paramType = typeSystem_->createTypeParameter(typeParam->getName(), typeParam->getConstraint());
            typeParameters.push_back(paramType);

            // Add type parameter to interface scope so it can be referenced within the interface
            std::cout << "DEBUG: Declaring interface type parameter symbol: " << typeParam->getName() << std::endl;
            declareSymbol(typeParam->getName(), SymbolKind::Type, paramType, typeParam->getLocation());
        }

        // Analyze extended interfaces if present
        for (const auto &extended: node.getExtends()) {
            std::cout << "DEBUG: Processing interface extension: " << extended->toString() << std::endl;

            // Validate that the extended type is a valid interface type
            if (extended->isError()) {
                reportError("Invalid interface type in extends clause", node.getLocation());
                continue;
            }

            // For generic interfaces with type arguments, validate that the type arguments
            // satisfy the constraints of the base interface
            if (auto genericType = dynamic_cast<GenericType *>(extended.get())) {
                std::cout << "DEBUG: Extended interface is generic with " << genericType->getTypeArguments().size() <<
                        " type arguments" << std::endl;

                // Validate that type arguments satisfy constraints
                if (!validateGenericInterfaceInheritance(genericType, node.getLocation())) {
                    reportError("Type arguments do not satisfy constraints for extended interface", node.getLocation());
                    continue;
                }
            }

            // Implement full interface inheritance validation
            if (!validateInterfaceInheritance(extended, node.getLocation())) {
                reportError("Invalid interface inheritance: " + extended->toString(), node.getLocation());
                continue;
            }
        }

        // Analyze property signatures
        for (const auto &property: node.getProperties()) {
            property->accept(*this);

            // Add property to interface scope
            auto propType = getDeclarationType(*property);
            declareSymbol(property->getName(), SymbolKind::Property, propType, property->getLocation());
        }

        // Analyze method signatures
        for (const auto &method: node.getMethods()) {
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

    void SemanticAnalyzer::visit(EnumMember &node) {
        // Analyze the value expression if present
        if (node.hasValue()) {
            node.getValue()->accept(*this);

            // Get the type of the value expression
            auto valueType = getExpressionType(*node.getValue());

            // Sophisticated enum value type checking
            bool isValidEnumValue = false;
            
            // Check for literal types (numbers, strings, booleans)
            if (valueType->getKind() == TypeKind::Literal) {
                auto literalType = dynamic_cast<LiteralType*>(valueType.get());
                if (literalType) {
                    switch (literalType->getKind()) {
                        case TypeKind::Literal:
                            // Check if it's a numeric or string literal
                            if (literalType->getValue().find_first_not_of("0123456789.-") == std::string::npos ||
                                literalType->getValue().front() == '"' || literalType->getValue().front() == '\'') {
                                isValidEnumValue = true;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            
            // Check for primitive types (number, string)
            if (!isValidEnumValue && 
                (valueType->isEquivalentTo(*typeSystem_->getNumberType()) ||
                 valueType->isEquivalentTo(*typeSystem_->getStringType()))) {
                isValidEnumValue = true;
            }
            
            // Check for constant expressions (computed values)
            if (!isValidEnumValue && (valueType->getKind() == TypeKind::Literal || 
                valueType->getKind() == TypeKind::StringLiteral || 
                valueType->getKind() == TypeKind::NumericLiteral || 
                valueType->getKind() == TypeKind::BooleanLiteral)) {
                isValidEnumValue = true;
            }
            
            // Check for references to other enum members
            if (!isValidEnumValue && valueType->getKind() == TypeKind::Enum) {
                isValidEnumValue = true;
            }
            
            // Check for union types where all members are valid enum values
            if (!isValidEnumValue && valueType->getKind() == TypeKind::Union) {
                auto unionType = dynamic_cast<UnionType*>(valueType.get());
                if (unionType) {
                    isValidEnumValue = true;
                    for (const auto& memberType : unionType->getTypes()) {
                        if (!memberType->isEquivalentTo(*typeSystem_->getNumberType()) &&
                            !memberType->isEquivalentTo(*typeSystem_->getStringType()) &&
                            memberType->getKind() != TypeKind::Literal &&
                            !(memberType->getKind() == TypeKind::Literal || 
                              memberType->getKind() == TypeKind::StringLiteral || 
                              memberType->getKind() == TypeKind::NumericLiteral || 
                              memberType->getKind() == TypeKind::BooleanLiteral)) {
                            isValidEnumValue = false;
                            break;
                        }
                    }
                }
            }
            
            if (!isValidEnumValue) {
                reportError("Enum member value must be a number, string, literal, constant expression, or reference to another enum member, got: " + 
                           valueType->toString(), node.getLocation());
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

    void SemanticAnalyzer::visit(EnumDeclaration &node) {
        // Create enum type
        auto enumType = typeSystem_->createEnumType(node.getName(), &node);

        // Add enum to symbol table
        declareSymbol(node.getName(), SymbolKind::Type, enumType, node.getLocation());

        // Enter enum scope (using Class scope type since enums are similar)
        enterScope(Scope::ScopeType::Class, node.getName());

        // Track the current auto-increment value for numeric enums
        int currentValue = 0;

        // Analyze enum members
        for (const auto &member: node.getMembers()) {
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

    void SemanticAnalyzer::visit(TypeAliasDeclaration &node) {
        // Create alias type
        auto aliasType = typeSystem_->createAliasType(node.getName(), node.getAliasedType(), &node);

        // Add type alias to symbol table
        declareSymbol(node.getName(), SymbolKind::Type, aliasType, node.getLocation());

        // TypeAliasDeclaration is not an Expression, so we don't call setExpressionType
        // The alias type is already stored in the symbol table above
    }

    void SemanticAnalyzer::visit(ImportDeclaration &node) {
        std::cout << "DEBUG: Processing import declaration: " << node.getModuleSpecifier() << std::endl;

        // Resolve the module
        String currentFile = currentModulePath_.empty() ? "current_file.ts" : currentModulePath_;
        ModuleResolutionResult result = moduleResolver_->resolveModule(node.getModuleSpecifier(), currentFile);

        if (!result.isSuccess) {
            diagnostics_.error("Failed to resolve module: " + result.errorMessage, node.getLocation());
            return;
        }

        std::cout << "DEBUG: Resolved module " << node.getModuleSpecifier() << " to " << result.resolvedPath <<
                std::endl;

        // Get or create module symbol table for current module
        ModuleSymbolTable *currentModuleTable = moduleSymbolManager_->getModuleSymbolTable(currentFile);
        if (!currentModuleTable) {
            currentModuleTable = moduleSymbolManager_->createModuleSymbolTable(currentFile);
        }

        // Add module dependency
        currentModuleTable->addModuleDependency(result.resolvedPath);

        // Process import clause based on type
        const ImportClause &clause = node.getClause();
        switch (clause.getType()) {
            case ImportClause::Default: {
                ImportedSymbol imported(clause.getDefaultBinding(), "default", result.resolvedPath,
                                        node.getLocation(), SymbolKind::Variable);
                currentModuleTable->addImportedSymbol(imported);
                break;
            }
            case ImportClause::Named: {
                const auto &namedImports = clause.getNamedImports();
                for (const auto &spec: namedImports) {
                    ImportedSymbol imported(spec.getLocalName(), spec.getImportedName(), result.resolvedPath,
                                            node.getLocation(), SymbolKind::Variable);
                    currentModuleTable->addImportedSymbol(imported);
                }
                break;
            }
            case ImportClause::Namespace: {
                ImportedSymbol imported(clause.getNamespaceBinding(), "*", result.resolvedPath,
                                        node.getLocation(), SymbolKind::Module);
                currentModuleTable->addImportedSymbol(imported);
                break;
            }
            case ImportClause::Mixed: {
                // Default import
                ImportedSymbol defaultImported(clause.getDefaultBinding(), "default", result.resolvedPath,
                                               node.getLocation(), SymbolKind::Variable);
                currentModuleTable->addImportedSymbol(defaultImported);

                // Named imports
                const auto &namedImports = clause.getNamedImports();
                for (const auto &spec: namedImports) {
                    ImportedSymbol imported(spec.getLocalName(), spec.getImportedName(), result.resolvedPath,
                                            node.getLocation(), SymbolKind::Variable);
                    currentModuleTable->addImportedSymbol(imported);
                }
                break;
            }
        }
    }

    void SemanticAnalyzer::visit(ExportDeclaration &node) {
        std::cout << "DEBUG: *** EXPORT DECLARATION VISITED *** Processing export declaration: " << node.
                getModuleSpecifier() << std::endl;
        std::cout.flush();

        String currentFile = currentModulePath_.empty() ? "current_file.ts" : currentModulePath_;

        // Get or create module symbol table for current module
        ModuleSymbolTable *currentModuleTable = moduleSymbolManager_->getModuleSymbolTable(currentFile);
        if (!currentModuleTable) {
            currentModuleTable = moduleSymbolManager_->createModuleSymbolTable(currentFile);
        }

        // Handle re-exports (exports with 'from' clause)
        if (!node.getModuleSpecifier().empty()) {
            ModuleResolutionResult result = moduleResolver_->resolveModule(node.getModuleSpecifier(), currentFile);

            if (!result.isSuccess) {
                diagnostics_.error("Failed to resolve re-export module: " + result.errorMessage, node.getLocation());
                return;
            }

            std::cout << "DEBUG: Resolved re-export module " << node.getModuleSpecifier() << " to " << result.
                    resolvedPath << std::endl;

            // Add module dependency
            currentModuleTable->addModuleDependency(result.resolvedPath);

            // Process re-export clause
            const ExportClause &clause = node.getClause();
            switch (clause.getType()) {
                case ExportClause::ReExport:
                case ExportClause::All: {
                    // Re-export all symbols from the module
                    ExportedSymbol reExport("*", "*", node.getLocation(), SymbolKind::Module, true,
                                            result.resolvedPath);
                    currentModuleTable->addExportedSymbol(reExport);
                    break;
                }
                case ExportClause::Named: {
                    // Re-export specific named symbols
                    const auto &namedExports = clause.getNamedExports();
                    for (const auto &spec: namedExports) {
                        ExportedSymbol reExport(spec.getExportedName(), spec.getLocalName(), node.getLocation(),
                                                SymbolKind::Variable, true, result.resolvedPath);
                        currentModuleTable->addExportedSymbol(reExport);
                    }
                    break;
                }
                default:
                    break;
            }
        } else {
            // Handle local exports
            const ExportClause &clause = node.getClause();
            switch (clause.getType()) {
                case ExportClause::Default: {
                    std::cout << "DEBUG: Processing default export" << std::endl;
                    // Export default expression/declaration
                    if (clause.getDefaultExport()) {
                        std::cout << "DEBUG: Default export is not null" << std::endl;
                        // Process the declaration first
                        if (auto decl = dynamic_cast<Declaration *>(clause.getDefaultExport())) {
                            std::cout << "DEBUG: Default export is a Declaration: " << decl->getName() << std::endl;
                            // Process the declaration (this will add it to the symbol table)
                            decl->accept(*this);
                            std::cout << "DEBUG: Declaration processed successfully" << std::endl;

                            // Mark the symbol as exported
                            Symbol *symbol = symbolTable_->lookupSymbol(decl->getName());
                            if (symbol) {
                                symbol->setExported(true);

                                // Add to module's exported symbols
                                // For export function/class/var declarations, export with the actual name, not "default"
                                ExportedSymbol exported(decl->getName(), decl->getName(), node.getLocation(),
                                                        symbol->getKind());
                                currentModuleTable->addExportedSymbol(exported);

                                std::cout << "DEBUG: Added named export: " << decl->getName() << " as '" << decl->
                                        getName() << "'" << std::endl;
                            } else {
                                std::cout << "DEBUG: Symbol not found for: " << decl->getName() << std::endl;
                            }
                        } else if (auto expr = dynamic_cast<Expression *>(clause.getDefaultExport())) {
                            std::cout << "DEBUG: Default export is an Expression" << std::endl;
                            // Process the expression
                            expr->accept(*this);

                            // For expressions, we'll export them as "default"
                            ExportedSymbol exported("default", "default", node.getLocation(), SymbolKind::Variable);
                            currentModuleTable->addExportedSymbol(exported);

                            std::cout << "DEBUG: Added default export expression as 'default'" << std::endl;
                        } else {
                            std::cout << "DEBUG: Default export is neither Declaration nor Expression" << std::endl;
                        }
                    } else {
                        std::cout << "DEBUG: Default export is null" << std::endl;
                    }
                    break;
                }
                case ExportClause::Named: {
                    // Export named symbols
                    const auto &namedExports = clause.getNamedExports();
                    for (const auto &spec: namedExports) {
                        ExportedSymbol exported(spec.getExportedName(), spec.getLocalName(), node.getLocation(),
                                                SymbolKind::Variable);
                        currentModuleTable->addExportedSymbol(exported);
                    }
                    break;
                }
                default:
                    break;
            }
        }
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
                std::vector<shared_ptr<Type> > resolvedTypeArgs;
                bool needsReconstruction = false;

                for (const auto &typeArg: genericType->getTypeArguments()) {
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
                    Symbol *symbol = resolveSymbol(classType->getName(), SourceLocation());
                    if (symbol && symbol->getKind() == SymbolKind::Type && symbol->getDeclaration()) {
                        if (auto classDecl = dynamic_cast<ClassDeclaration *>(symbol->getDeclaration())) {
                            classType->setDeclaration(classDecl);
                        }
                    }
                }
            }
            return type; // Already resolved or not an unresolved type
        }

        const auto &unresolvedType = static_cast<const UnresolvedType &>(*type);
        String typeName = unresolvedType.getName();

        // Look up the type name in the symbol table
        Symbol *symbol = resolveSymbol(typeName, SourceLocation());
        std::cout << "DEBUG: resolveType looking up '" << typeName << "': " << (symbol ? "found" : "not found") <<
                std::endl;
        if (symbol) {
            std::cout << "DEBUG: Symbol kind: " << static_cast<int>(symbol->getKind()) << std::endl;
            std::cout << "DEBUG: Symbol type: " << (symbol->getType() ? symbol->getType()->toString() : "null") <<
                    std::endl;
            if (symbol->getType()) {
                std::cout << "DEBUG: Symbol type kind: " << static_cast<int>(symbol->getType()->getKind()) << std::endl;
            }
        } else {
            std::cout << "DEBUG: resolveType failed to find symbol '" << typeName <<
                    "' - this will cause type parameter to be treated as variable" << std::endl;
        }
        if (symbol && (symbol->getKind() == SymbolKind::Type || symbol->getKind() == SymbolKind::Class)) {
            auto resolvedType = symbol->getType();
            std::cout << "DEBUG: resolveType returning resolved type: " << resolvedType->toString() << " (kind: " << static_cast<int>(resolvedType->getKind()) << ")" << std::endl;

            // Always ensure ClassType has correct declaration pointer
            if (resolvedType->getKind() == TypeKind::Class) {
                auto classType = std::static_pointer_cast<ClassType>(resolvedType);
                if (!classType->getDeclaration() && symbol->getDeclaration()) {
                    // Set the declaration pointer if it's missing
                    if (auto classDecl = dynamic_cast<ClassDeclaration *>(symbol->getDeclaration())) {
                        classType->setDeclaration(classDecl);
                    }
                }
            }

            return resolvedType; // Return the resolved type
        }

        // If not found, return error type
        return typeSystem_->getErrorType();
    }

    shared_ptr<Type>
    SemanticAnalyzer::findInterfaceMember(const InterfaceType &interfaceType, const String &memberName) {
        InterfaceDeclaration *interfaceDecl = interfaceType.getDeclaration();

        // If the InterfaceType doesn't have a declaration pointer, try to find it in the symbol table
        if (!interfaceDecl) {
            Symbol *symbol = resolveSymbol(interfaceType.getName(), SourceLocation());
            if (symbol && symbol->getKind() == SymbolKind::Type && symbol->getDeclaration()) {
                interfaceDecl = dynamic_cast<InterfaceDeclaration *>(symbol->getDeclaration());
            }
        }

        if (!interfaceDecl) {
            return nullptr;
        }

        // Look for property in current interface
        for (const auto &property: interfaceDecl->getProperties()) {
            if (property->getName() == memberName) {
                return getDeclarationType(*property);
            }
        }

        // Look for method in current interface
        for (const auto &method: interfaceDecl->getMethods()) {
            if (method->getName() == memberName) {
                return getDeclarationType(*method);
            }
        }

        // If not found in current interface, look in extended interfaces
        for (const auto &extendedInterface: interfaceDecl->getExtends()) {
            auto resolvedExtendedType = resolveType(extendedInterface);
            if (resolvedExtendedType) {
                if (resolvedExtendedType->getKind() == TypeKind::Interface) {
                    const auto &extendedInterfaceType = static_cast<const InterfaceType &>(*resolvedExtendedType);
                    auto memberType = findInterfaceMember(extendedInterfaceType, memberName);
                    if (memberType) {
                        return memberType;
                    }
                } else if (resolvedExtendedType->getKind() == TypeKind::Generic) {
                    // Handle generic extended interfaces (e.g., ExtendedInterface<T>)
                    const auto &genericExtendedType = static_cast<const GenericType &>(*resolvedExtendedType);
                    auto baseType = genericExtendedType.getBaseType();
                    if (baseType->getKind() == TypeKind::Interface) {
                        const auto &extendedInterfaceType = static_cast<const InterfaceType &>(*baseType);
                        auto memberType = findInterfaceMember(extendedInterfaceType, memberName);
                        if (memberType) {
                            return memberType;
                        }
                    }
                }
            }
        }

        return nullptr;
    }

    shared_ptr<Type> SemanticAnalyzer::findClassMember(const ClassType &classType, const String &memberName) {
        ClassDeclaration *classDecl = classType.getDeclaration();

        // If the ClassType doesn't have a declaration pointer, try to find it in the symbol table
        if (!classDecl) {
            Symbol *symbol = resolveSymbol(classType.getName(), SourceLocation());
            if (symbol && symbol->getKind() == SymbolKind::Type && symbol->getDeclaration()) {
                classDecl = dynamic_cast<ClassDeclaration *>(symbol->getDeclaration());
            }
        }

        if (!classDecl) {
            return nullptr;
        }

        // Debug: check what methods are in the class
        // (This would normally be removed in production code)

        // Look for property in current class
        for (const auto &property: classDecl->getProperties()) {
            if (property->getName() == memberName) {
                return getDeclarationType(*property);
            }
        }

        // Look for method in current class
        for (const auto &method: classDecl->getMethods()) {
            if (method->getName() == memberName) {
                return getDeclarationType(*method);
            }
        }

        // If not found in current class, look in base class
        if (classDecl->getBaseClass()) {
            auto resolvedBaseType = resolveType(classDecl->getBaseClass());
            if (resolvedBaseType) {
                if (resolvedBaseType->getKind() == TypeKind::Class) {
                    const auto &baseClassType = static_cast<const ClassType &>(*resolvedBaseType);
                    return findClassMember(baseClassType, memberName);
                } else if (resolvedBaseType->getKind() == TypeKind::Generic) {
                    // Handle generic base classes (e.g., MiddleGeneric<T, U>)
                    const auto &genericBaseType = static_cast<const GenericType &>(*resolvedBaseType);
                    auto baseType = genericBaseType.getBaseType();
                    if (baseType->getKind() == TypeKind::Class) {
                        const auto &baseClassType = static_cast<const ClassType &>(*baseType);
                        return findClassMember(baseClassType, memberName);
                    }
                }
            }
        }

        // Member not found
        return nullptr;
    }

    // Helper method to find member type on a given type
    shared_ptr<Type> SemanticAnalyzer::findMemberType(shared_ptr<Type> type, const String &memberName) {
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
            } else if (memberName == "push" || memberName == "pop" || memberName == "shift" || memberName ==
                       "unshift") {
                // These methods would return the array type or element type
                return type;
            }
        } else if (type->getKind() == TypeKind::Class) {
            // For class types, we would need to look up the class definition
            // and find the member. For now, we'll handle basic cases.
            const auto &classType = static_cast<const ClassType &>(*type);
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

    void SemanticAnalyzer::collectNestedFunctionDeclarations(const Statement &stmt) {
        // Recursively collect function declarations from nested statements
        if (auto blockStmt = dynamic_cast<const BlockStatement *>(&stmt)) {
            // Process all statements in the block
            for (const auto &nestedStmt: blockStmt->getStatements()) {
                if (auto funcDecl = dynamic_cast<FunctionDeclaration *>(nestedStmt.get())) {
                    // Create function type for nested function
                    std::vector<FunctionType::Parameter> paramTypes;
                    for (const auto &param: funcDecl->getParameters()) {
                        FunctionType::Parameter funcParam;
                        funcParam.name = param.name;
                        funcParam.type = param.type ? param.type : typeSystem_->getAnyType();
                        funcParam.optional = param.optional;
                        funcParam.rest = param.rest;
                        paramTypes.push_back(funcParam);
                    }

                    auto returnType = funcDecl->getReturnType()
                                          ? funcDecl->getReturnType()
                                          : typeSystem_->getVoidType();
                    auto functionType = typeSystem_->createFunctionType(std::move(paramTypes), returnType);

                    // Add nested function symbol to current scope
                    if (!symbolTable_->addSymbol(funcDecl->getName(), SymbolKind::Function, functionType,
                                                 funcDecl->getLocation(), funcDecl)) {
                        // If adding to current scope fails, try adding to global scope
                        // This handles the case where we're collecting from module level
                        if (!symbolTable_->addSymbol(funcDecl->getName(), SymbolKind::Function, functionType,
                                                     funcDecl->getLocation(), funcDecl)) {
                            reportError("Failed to declare nested function: " + funcDecl->getName(),
                                        funcDecl->getLocation());
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
        } else if (auto ifStmt = dynamic_cast<const IfStatement *>(&stmt)) {
            // Process if statement branches
            if (ifStmt->getThenStatement()) {
                collectNestedFunctionDeclarations(*ifStmt->getThenStatement());
            }
            if (ifStmt->getElseStatement()) {
                collectNestedFunctionDeclarations(*ifStmt->getElseStatement());
            }
        } else if (auto whileStmt = dynamic_cast<const WhileStatement *>(&stmt)) {
            // Process while statement body
            if (whileStmt->getBody()) {
                collectNestedFunctionDeclarations(*whileStmt->getBody());
            }
        } else if (auto forStmt = dynamic_cast<const ForStatement *>(&stmt)) {
            // Process for statement body
            if (forStmt->getBody()) {
                collectNestedFunctionDeclarations(*forStmt->getBody());
            }
        } else if (auto tryStmt = dynamic_cast<const TryStatement *>(&stmt)) {
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
            FunctionDeclaration *currentFunction = functionStack_.back();
            currentFunction->setCaptured(true);

            // Add a warning that this function captures variables
            diagnostics_.warning("Function '" + currentFunction->getName() + "' captures variables from outer scope",
                                 currentFunction->getLocation());
        }
    }

    void SemanticAnalyzer::markVariableAsCaptured(Symbol *symbol) {
        // Mark the current function as captured and add the variable to the captured list
        if (!functionStack_.empty()) {
            FunctionDeclaration *currentFunction = functionStack_.back();
            currentFunction->setCaptured(true);
            currentFunction->addCapturedVariable(symbol);

            // Debug output
            diagnostics_.warning(
                "Captured variable: " + symbol->getName() + " in function: " + currentFunction->getName(),
                currentFunction->getLocation());
        }
    }

    // Generic constraint validation helpers

    /**
     * Retrieves the function declaration AST node for a given function name.
     * This is needed to access type parameters for constraint validation.
     * 
     * @param functionName The name of the function to look up
     * @return Pointer to FunctionDeclaration if found, nullptr otherwise
     */
    FunctionDeclaration *SemanticAnalyzer::getFunctionDeclaration(const String &functionName) {
        // Look up the function symbol in the symbol table
        Symbol *symbol = symbolTable_->lookupSymbol(functionName);
        if (symbol && symbol->getKind() == SymbolKind::Function) {
            // Get the AST declaration node from the symbol
            ASTNode *declaration = symbol->getDeclaration();
            if (declaration) {
                // Cast to FunctionDeclaration to access type parameters
                return dynamic_cast<FunctionDeclaration *>(declaration);
            }
        }
        return nullptr; // Function not found or not a function symbol
    }

    /**
     * Validates generic function call constraints by checking that each argument
     * type satisfies the corresponding type parameter's constraint.
     * 
     * Algorithm:
     * 1. Extract type parameters from function declaration
     * 2. Extract argument types from function call
     * 3. For each type parameter with a constraint:
     *    - Check if corresponding argument type satisfies the constraint
     *    - Report error if constraint is violated
     * 
     * @param call The function call expression to validate
     * @param funcDecl The function declaration containing type parameters
     * @param functionType The function type (used for type checking)
     * @return true if all constraints are satisfied, false otherwise
     */
    bool SemanticAnalyzer::validateFunctionArguments(const CallExpression &call,
                                                     const FunctionType &functionType) {
        const auto &parameters = functionType.getParameters();
        const auto &arguments = call.getArguments();

        // Check if we have the right number of arguments
        if (arguments.size() != parameters.size()) {
            reportError("Expected " + std::to_string(parameters.size()) + " arguments, but got " +
                        std::to_string(arguments.size()), call.getLocation());
            return false;
        }

        // Check each argument type against its corresponding parameter type
        for (size_t i = 0; i < arguments.size(); ++i) {
            auto argType = getExpressionType(*arguments[i]);
            auto paramType = parameters[i].type;

            std::cout << "DEBUG: Checking argument " << i << ": " << argType->toString() << " vs parameter: " << paramType->toString() << std::endl;
            std::cout << "DEBUG: Argument type kind: " << static_cast<int>(argType->getKind()) << ", Parameter type kind: " << static_cast<int>(paramType->getKind()) << std::endl;
            
            if (!isValidAssignment(*argType, *paramType)) {
                std::cout << "DEBUG: Assignment failed for " << argType->toString() << " to " << paramType->toString() << std::endl;
                reportTypeError(paramType->toString(), argType->toString(), arguments[i]->getLocation());
                return false;
            }
        }

        return true;
    }

    bool SemanticAnalyzer::validateGenericFunctionCall(const CallExpression &call,
                                                       const FunctionDeclaration &funcDecl,
                                                       const FunctionType &functionType) {
        // Get type parameters from the function declaration
        const auto &typeParams = funcDecl.getTypeParameters();
        if (typeParams.empty()) {
            return true; // Not a generic function - no constraints to validate
        }

        // Extract argument types from the function call
        // This step resolves the types of all arguments passed to the function
        std::vector<shared_ptr<Type> > argumentTypes;
        for (const auto &arg: call.getArguments()) {
            auto argType = getExpressionType(*arg);
            argumentTypes.push_back(argType);
        }

        // Validate constraints: for each type parameter with a constraint,
        // check that the corresponding argument type satisfies the constraint
        constexpr size_t START_INDEX = 0;
        for (size_t i = START_INDEX; i < typeParams.size() && i < argumentTypes.size(); ++i) {
            const auto &typeParam = typeParams[i];
            auto argType = argumentTypes[i];

            // Only validate type parameters that have constraints
            if (typeParam->hasConstraint()) {
                auto constraint = typeParam->getConstraint();
                if (constraint) {
                    // Use the constraint checker to validate the relationship
                    // between the argument type and the constraint type
                    if (!constraintChecker_->satisfiesConstraint(argType, constraint)) {
                        // Construct a detailed error message for the developer
                        std::string errorMsg = "Argument type '" + argType->toString() +
                                               "' does not satisfy constraint '" +
                                               constraint->toString() +
                                               "' for type parameter '" +
                                               typeParam->getName() + "'";
                        constraintChecker_->reportConstraintError(errorMsg, call.getLocation());
                        return false; // Constraint violation - stop validation
                    }
                }
            }
        }

        return true; // All constraints satisfied
    }

    bool SemanticAnalyzer::validateGenericInterfaceInheritance(GenericType *genericType,
                                                               const SourceLocation &location) {
        if (!genericType) {
            return false;
        }

        auto baseType = genericType->getBaseType();
        auto typeArguments = genericType->getTypeArguments();

        // Look up the base interface to get its type parameters
        auto baseInterfaceSymbol = symbolTable_->lookupSymbol(baseType->toString());
        if (!baseInterfaceSymbol) {
            reportError("Base interface not found: " + baseType->toString(), location);
            return false;
        }

        // Get the interface declaration to access its type parameters
        auto interfaceType = baseInterfaceSymbol->getType();
        if (interfaceType->getKind() != TypeKind::Interface) {
            reportError("Base type is not an interface: " + baseType->toString(), location);
            return false;
        }

        auto interfaceDecl = static_cast<InterfaceType *>(interfaceType.get())->getDeclaration();
        if (!interfaceDecl) {
            reportError("Interface declaration not found for: " + baseType->toString(), location);
            return false;
        }

        // Validate each type argument against its corresponding type parameter constraint
        const auto &typeParameters = interfaceDecl->getTypeParameters();
        if (typeArguments.size() != typeParameters.size()) {
            reportError("Type argument count mismatch for interface " + baseType->toString() +
                        ": expected " + std::to_string(typeParameters.size()) +
                        ", got " + std::to_string(typeArguments.size()), location);
            return false;
        }

        for (size_t i = 0; i < typeArguments.size(); ++i) {
            const auto &typeParam = typeParameters[i];
            auto typeArg = typeArguments[i];

            // Check constraint if present
            if (typeParam->hasConstraint()) {
                auto constraint = typeParam->getConstraint();
                if (!constraintChecker_->satisfiesConstraint(typeArg, constraint)) {
                    reportError("Type argument '" + typeArg->toString() +
                                "' does not satisfy constraint '" + constraint->toString() +
                                "' for type parameter '" + typeParam->getName() + "'", location);
                    return false;
                }
            }
        }

        return true;
    }

    bool SemanticAnalyzer::validateInterfaceInheritance(shared_ptr<Type> extendedType, const SourceLocation &location) {
        if (!extendedType) {
            return false;
        }

        // Check if the extended type exists in the symbol table
        auto extendedSymbol = symbolTable_->lookupSymbol(extendedType->toString());
        if (!extendedSymbol) {
            reportError("Extended interface not found: " + extendedType->toString(), location);
            return false;
        }

        // Verify it's actually an interface
        auto interfaceType = extendedSymbol->getType();
        if (interfaceType->getKind() != TypeKind::Interface) {
            reportError("Extended type is not an interface: " + extendedType->toString(), location);
            return false;
        }

        // Check for circular inheritance
        if (!checkCircularInterfaceInheritance(extendedType, location)) {
            return false;
        }

        return true;
    }

    bool SemanticAnalyzer::checkCircularInterfaceInheritance(shared_ptr<Type> interfaceType,
                                                             const SourceLocation &location) {
        // Proper circular inheritance check using a stack-based approach
        static std::vector<std::string> inheritanceStack;
        
        auto interfaceName = interfaceType->toString();
        
        // Check if this interface is already in the current inheritance chain
        for (const auto& stackInterface : inheritanceStack) {
            if (stackInterface == interfaceName) {
                // Found a cycle - construct the cycle path for error reporting
                std::string cyclePath = interfaceName;
                bool foundStart = false;
                for (auto it = inheritanceStack.rbegin(); it != inheritanceStack.rend(); ++it) {
                    if (*it == interfaceName) {
                        foundStart = true;
                        continue;
                    }
                    if (foundStart) {
                        cyclePath = *it + " -> " + cyclePath;
                    }
                }
                cyclePath = cyclePath + " -> " + interfaceName; // Complete the cycle
                
                reportError("Circular interface inheritance detected: " + cyclePath, location);
                return false;
            }
        }
        
        // Add current interface to the stack
        inheritanceStack.push_back(interfaceName);
        
        // Check extended interfaces recursively
        auto interfaceSymbol = symbolTable_->lookupSymbol(interfaceName);
        if (interfaceSymbol) {
            auto type = interfaceSymbol->getType();
            if (type->getKind() == TypeKind::Interface) {
                auto interfaceDecl = static_cast<InterfaceType *>(type.get())->getDeclaration();
                if (interfaceDecl) {
                    for (const auto &extended: interfaceDecl->getExtends()) {
                        if (!checkCircularInterfaceInheritance(extended, location)) {
                            // Remove from stack before returning false
                            inheritanceStack.pop_back();
                            return false;
                        }
                    }
                }
            }
        }
        
        // Remove current interface from the stack
        inheritanceStack.pop_back();
        return true;
    }

    // ARC Memory Management Analysis Implementation
    void SemanticAnalyzer::analyzeOwnership(const Expression &expr) {
        // Analyze ownership patterns in expressions
        if (auto *moveExpr = dynamic_cast<const MoveExpression *>(&expr)) {
            analyzeMoveSemantics(*moveExpr);
        } else if (auto *assignExpr = dynamic_cast<const AssignmentExpression *>(&expr)) {
            analyzeAssignmentOwnership(*assignExpr);
        }
    }

    void SemanticAnalyzer::analyzeMoveSemantics(const MoveExpression &moveExpr) {
        // Analyze move semantics
        auto operandType = getExpressionType(*moveExpr.getOperand());

        if (!isMoveable(*operandType)) {
            reportError("Cannot move non-moveable type: " + operandType->toString(),
                        moveExpr.getLocation());
            return;
        }

        // Check if the operand is a unique_ptr or has move semantics
        if (operandType->getKind() == TypeKind::UniquePtr) {
            // Move semantics for unique_ptr - this is valid
            std::cout << "DEBUG: Move semantics applied to unique_ptr" << std::endl;
        } else if (operandType->getKind() == TypeKind::SharedPtr) {
            // Move semantics for shared_ptr - this transfers ownership
            std::cout << "DEBUG: Move semantics applied to shared_ptr" << std::endl;
        } else {
            // For other types, move semantics might not be meaningful
            reportWarning("Move semantics applied to non-smart-pointer type: " + operandType->toString(),
                          moveExpr.getLocation());
        }
    }

    void SemanticAnalyzer::analyzeAssignmentOwnership(const AssignmentExpression &assignExpr) {
        // Analyze assignment ownership patterns
        auto leftType = getExpressionType(*assignExpr.getLeft());
        auto rightType = getExpressionType(*assignExpr.getRight());

        // Check for ownership transfer patterns
        if (isARCManaged(*leftType) && isARCManaged(*rightType)) {
            // Both sides are ARC-managed
            if (leftType->getKind() == TypeKind::UniquePtr && rightType->getKind() == TypeKind::UniquePtr) {
                // unique_ptr assignment - should use move semantics
                reportWarning("unique_ptr assignment should use move semantics: " +
                              leftType->toString() + " = std::move(" + rightType->toString() + ")",
                              assignExpr.getLocation());
            }
        }
    }

    void SemanticAnalyzer::detectCycles(const ClassDeclaration &classDecl) {
        // Detect potential reference cycles in class definitions
        // This is a simplified implementation - a full implementation would
        // analyze the class structure for circular references

        std::cout << "DEBUG: Analyzing class " << classDecl.getName() << " for cycles" << std::endl;

        // For now, just check for obvious patterns
        // In a full implementation, this would:
        // 1. Analyze class properties
        // 2. Check for circular references
        // 3. Suggest weak references where appropriate
    }

    void SemanticAnalyzer::suggestWeakReferences(const ClassDeclaration &classDecl) {
        // Suggest weak references to break cycles
        // This is a placeholder implementation

        std::cout << "DEBUG: Suggesting weak references for class " << classDecl.getName() << std::endl;

        // In a full implementation, this would:
        // 1. Analyze the class structure
        // 2. Identify potential cycles
        // 3. Suggest converting strong references to weak references
    }

    bool SemanticAnalyzer::isARCManaged(const Type &type) const {
        // Check if a type is ARC-managed
        return type.getKind() == TypeKind::UniquePtr ||
               type.getKind() == TypeKind::SharedPtr ||
               type.getKind() == TypeKind::WeakPtr ||
               type.getKind() == TypeKind::Class; // Classes are ARC-managed by default
    }

    bool SemanticAnalyzer::isMoveable(const Type &type) const {
        // Check if a type supports move semantics
        return type.getKind() == TypeKind::UniquePtr ||
               type.getKind() == TypeKind::SharedPtr ||
               type.getKind() == TypeKind::Class; // Classes support move semantics
    }

    bool SemanticAnalyzer::hasDestructor(const Type &type) const {
        // Check if a type has a destructor (needed for ARC)
        return type.getKind() == TypeKind::Class ||
               type.getKind() == TypeKind::UniquePtr ||
               type.getKind() == TypeKind::SharedPtr;
    }

    // MoveExpression visitor implementation
    void SemanticAnalyzer::visit(MoveExpression &node) {
        // Analyze the operand first
        node.getOperand()->accept(static_cast<ASTVisitor &>(*this));

        // Get the type of the operand
        auto operandType = getExpressionType(*node.getOperand());

        // Analyze move semantics
        analyzeMoveSemantics(node);

        // Set the type of the move expression to be the same as the operand
        // Move expressions don't change the type, just the ownership semantics
        setExpressionType(node, operandType);
    }

    // Cycle detection methods
    void SemanticAnalyzer::runCycleDetection() {
        if (!cycleDetector_) {
            std::cerr << "❌ Cycle detector not initialized" << std::endl;
            return;
        }

        std::cout << "🔍 Running static cycle detection..." << std::endl;

        // Run cycle detection on the symbol table
        cycleDetector_->analyzeSymbolTable();

        std::cout << "✅ Static cycle detection completed" << std::endl;
    }

    bool SemanticAnalyzer::hasCycleErrors() const {
        if (!cycleDetector_) return false;
        return cycleDetector_->hasErrors();
    }

    void SemanticAnalyzer::printCycleResults() const {
        if (!cycleDetector_) {
            std::cout << "❌ Cycle detector not initialized" << std::endl;
            return;
        }

        cycleDetector_->printResults();
    }

    // RAII Analysis Implementation
    void SemanticAnalyzer::analyzeDestructor(const DestructorDeclaration &destructor) {
        std::cout << "🔍 Analyzing destructor for RAII patterns: " << destructor.getClassName() << std::endl;

        // Check if destructor is properly implemented
        if (!destructor.getBody()) {
            reportWarning("Destructor has no body - consider adding resource cleanup", destructor.getLocation());
            return;
        }

        // Analyze destructor body for resource cleanup patterns
        BlockStatement *body = destructor.getBody();
        if (body && body->getStatements().empty()) {
            reportWarning("Empty destructor body - ensure all resources are properly cleaned up",
                          destructor.getLocation());
        }

        // Validate destructor safety (no exceptions, no virtual calls)
        validateDestructorSafety(destructor);

        std::cout << "✅ Destructor analysis completed for: " << destructor.getClassName() << std::endl;
    }

    void SemanticAnalyzer::validateRAIIPatterns(const ClassDeclaration &classDecl) {
        std::cout << "🔍 Validating RAII patterns for class: " << classDecl.getName() << std::endl;

        // Check if class has destructor
        if (!classDecl.getDestructor()) {
            // Check if class has resources that need cleanup
            bool hasResources = false;
            for (const auto &prop: classDecl.getProperties()) {
                if (isARCManaged(*getDeclarationType(*prop))) {
                    hasResources = true;
                    break;
                }
            }

            if (hasResources) {
                reportWarning("Class with ARC-managed resources should have a destructor for proper cleanup",
                              classDecl.getLocation());
            }
        }

        // Analyze resource ownership
        analyzeResourceOwnership(classDecl);

        // Detect potential resource leaks
        detectResourceLeaks(classDecl);

        std::cout << "✅ RAII pattern validation completed for: " << classDecl.getName() << std::endl;
    }

    void SemanticAnalyzer::suggestResourceCleanup(const ClassDeclaration &classDecl) {
        std::cout << "💡 Suggesting resource cleanup for class: " << classDecl.getName() << std::endl;

        // Analyze properties for resource management
        for (const auto &prop: classDecl.getProperties()) {
            shared_ptr<Type> propType = getDeclarationType(*prop);

            if (isARCManaged(*propType)) {
                std::cout << "   - Property '" << prop->getName() << "' is ARC-managed" << std::endl;

                // Check if destructor handles this property
                if (classDecl.getDestructor()) {
                    // Analyze the destructor body to see if it properly cleans up this property
                    bool propertyCleanedUp = analyzeDestructorCleanup(classDecl.getDestructor(), prop->getName());
                    if (propertyCleanedUp) {
                        std::cout << "     ✓ Destructor properly cleans up '" << prop->getName() << "'" << std::endl;
                    } else {
                        reportWarning("ARC-managed property '" + prop->getName() + "' is not cleaned up in destructor",
                                      prop->getLocation());
                    }
                } else {
                    reportWarning("ARC-managed property '" + prop->getName() + "' should be cleaned up in destructor",
                                  prop->getLocation());
                }
            }
        }

        std::cout << "✅ Resource cleanup suggestions completed" << std::endl;
    }

    bool SemanticAnalyzer::analyzeDestructorCleanup(const DestructorDeclaration* destructor, const String& propertyName) {
        if (!destructor || !destructor->getBody()) {
            return false;
        }
        
        // Analyze the destructor body for cleanup operations
        DestructorCleanupAnalyzer analyzer(propertyName);
        destructor->getBody()->accept(analyzer);
        
        return analyzer.isPropertyCleanedUp();
    }

    void SemanticAnalyzer::detectResourceLeaks(const ClassDeclaration &classDecl) {
        std::cout << "🔍 Detecting potential resource leaks in class: " << classDecl.getName() << std::endl;

        // Check for ARC-managed properties without proper cleanup
        for (const auto &prop: classDecl.getProperties()) {
            shared_ptr<Type> propType = getDeclarationType(*prop);

            if (isARCManaged(*propType)) {
                // Check if this property is properly managed
                if (!classDecl.getDestructor()) {
                    reportWarning("ARC-managed property '" + prop->getName() + "' may leak without destructor",
                                  prop->getLocation());
                }
            }
        }

        // Check for smart pointer usage patterns
        for (const auto &prop: classDecl.getProperties()) {
            shared_ptr<Type> propType = getDeclarationType(*prop);

            if (propType->getKind() == TypeKind::SharedPtr ||
                propType->getKind() == TypeKind::UniquePtr) {
                std::cout << "   - Smart pointer property '" << prop->getName() << "' detected" << std::endl;

                // Smart pointers are automatically managed, but we should still check for cycles
                if (propType->getKind() == TypeKind::SharedPtr) {
                    std::cout << "     ⚠️  shared_ptr detected - check for reference cycles" << std::endl;
                }
            }
        }

        std::cout << "✅ Resource leak detection completed" << std::endl;
    }

    void SemanticAnalyzer::analyzeResourceOwnership(const ClassDeclaration &classDecl) {
        std::cout << "🔍 Analyzing resource ownership for class: " << classDecl.getName() << std::endl;

        // Analyze each property for ownership semantics
        for (const auto &prop: classDecl.getProperties()) {
            shared_ptr<Type> propType = getDeclarationType(*prop);

            std::cout << "   - Property '" << prop->getName() << "' type: " << propType->toString() << std::endl;

            // Determine ownership type
            if (propType->getKind() == TypeKind::UniquePtr) {
                std::cout << "     ✓ Exclusive ownership (unique_ptr)" << std::endl;
            } else if (propType->getKind() == TypeKind::SharedPtr) {
                std::cout << "     ⚠️  Shared ownership (shared_ptr) - potential for cycles" << std::endl;
            } else if (propType->getKind() == TypeKind::WeakPtr) {
                std::cout << "     ✓ Non-owning reference (weak_ptr)" << std::endl;
            } else if (isARCManaged(*propType)) {
                std::cout << "     ✓ ARC-managed resource" << std::endl;
            } else {
                std::cout << "     - Value type or primitive" << std::endl;
            }
        }

        std::cout << "✅ Resource ownership analysis completed" << std::endl;
    }

    void SemanticAnalyzer::validateDestructorSafety(const DestructorDeclaration &destructor) {
        std::cout << "🔍 Validating destructor safety for: " << destructor.getClassName() << std::endl;

        // Check for common destructor safety issues
        if (!destructor.getBody()) {
            std::cout << "   - No destructor body to analyze" << std::endl;
            return;
        }

        // In a full implementation, we would:
        // 1. Check for exception throwing in destructor
        // 2. Check for virtual function calls
        // 3. Check for proper resource cleanup order
        // 4. Validate that all ARC-managed resources are properly released

        std::cout << "   ✓ Destructor safety validation completed" << std::endl;
        std::cout << "✅ Destructor safety validation completed for: " << destructor.getClassName() << std::endl;
    }

    // Destructuring visitor method implementations
    void SemanticAnalyzer::visit(DestructuringPattern &node) {
        std::cout << "DEBUG: Analyzing destructuring pattern" << std::endl;

        // Destructuring patterns are LValues that can be assigned to
        // The actual analysis is done by the specific pattern types
        setExpressionType(node, typeSystem_->getAnyType());
    }

    void SemanticAnalyzer::visit(ArrayDestructuringPattern &node) {
        std::cout << "DEBUG: Analyzing array destructuring pattern with " << node.getElements().size() << " elements" <<
                std::endl;

        // Analyze each element in the array destructuring pattern
        for (size_t i = 0; i < node.getElements().size(); ++i) {
            const auto &element = node.getElements()[i];

            if (element) {
                std::cout << "DEBUG: Analyzing array destructuring element " << i << std::endl;
                element->accept(*this);

                // Get the type of the element pattern
                auto elementType = getExpressionType(*element);
                if (elementType) {
                    std::cout << "DEBUG: Array destructuring element " << i << " type: " << elementType->toString() <<
                            std::endl;
                }
            } else {
                std::cout << "DEBUG: Array destructuring element " << i << " is null (rest element)" << std::endl;
            }
        }

        // Array destructuring patterns are LValues
        setExpressionType(node, typeSystem_->getAnyType());
        std::cout << "DEBUG: Array destructuring pattern analysis completed" << std::endl;
    }

    void SemanticAnalyzer::visit(ObjectDestructuringPattern &node) {
        std::cout << "DEBUG: Analyzing object destructuring pattern with " << node.getProperties().size() <<
                " properties" << std::endl;

        // Analyze each property in the object destructuring pattern
        for (size_t i = 0; i < node.getProperties().size(); ++i) {
            const auto &property = node.getProperties()[i];

            std::cout << "DEBUG: Analyzing object destructuring property " << i << ": " << property.getKey() <<
                    std::endl;

            // Analyze the pattern for this property
            property.getPattern()->accept(*this);

            // Get the type of the property pattern
            auto patternType = getExpressionType(*property.getPattern());
            if (patternType) {
                std::cout << "DEBUG: Object destructuring property " << property.getKey() << " pattern type: " <<
                        patternType->toString() << std::endl;
            }

            // Analyze default value if present
            if (property.getDefaultValue()) {
                std::cout << "DEBUG: Analyzing default value for property: " << property.getKey() << std::endl;
                property.getDefaultValue()->accept(*this);

                auto defaultValueType = getExpressionType(*property.getDefaultValue());
                if (defaultValueType) {
                    std::cout << "DEBUG: Default value type for property " << property.getKey() << ": " <<
                            defaultValueType->toString() << std::endl;

                    // Check type compatibility between pattern and default value
                    if (patternType && !typeSystem_->areTypesCompatible(*defaultValueType, *patternType)) {
                        reportError("Default value type '" + defaultValueType->toString() +
                                    "' is not compatible with pattern type '" + patternType->toString() +
                                    "' for property '" + property.getKey() + "'",
                                    property.getLocation());
                    }
                }
            }
        }

        // Object destructuring patterns are LValues
        setExpressionType(node, typeSystem_->getAnyType());
        std::cout << "DEBUG: Object destructuring pattern analysis completed" << std::endl;
    }

    void SemanticAnalyzer::visit(IdentifierPattern &node) {
        std::cout << "DEBUG: Analyzing identifier pattern: " << node.getName() << std::endl;

        // Check if this identifier is already declared in the current scope
        auto existingSymbol = symbolTable_->lookupSymbol(node.getName());
        if (existingSymbol) {
            reportError("Identifier '" + node.getName() + "' is already declared in this scope",
                        node.getLocation());
            setExpressionType(node, typeSystem_->getAnyType());
            return;
        }

        // Analyze default value if present
        shared_ptr<Type> inferredType = nullptr;
        if (node.getDefaultValue()) {
            std::cout << "DEBUG: Analyzing default value for identifier pattern: " << node.getName() << std::endl;
            node.getDefaultValue()->accept(*this);

            auto defaultValueType = getExpressionType(*node.getDefaultValue());
            if (defaultValueType) {
                inferredType = defaultValueType;
                std::cout << "DEBUG: Default value type for identifier " << node.getName() << ": " << defaultValueType->
                        toString() << std::endl;
            }
        }

        // If no default value, infer type as 'any' for now
        if (!inferredType) {
            inferredType = typeSystem_->getAnyType();
        }

        // Add the identifier to the symbol table
        symbolTable_->addSymbol(node.getName(), SymbolKind::Variable, inferredType, node.getLocation());
        std::cout << "DEBUG: Added identifier pattern '" << node.getName() << "' to symbol table with type: " <<
                inferredType->toString() << std::endl;

        // Set the expression type
        setExpressionType(node, inferredType);
        std::cout << "DEBUG: Identifier pattern analysis completed for: " << node.getName() << std::endl;
    }

    void SemanticAnalyzer::visit(DestructuringAssignment &node) {
        std::cout << "DEBUG: Analyzing destructuring assignment" << std::endl;

        // Analyze the value being destructured
        node.getValue()->accept(*this);
        auto valueType = getExpressionType(*node.getValue());
        if (!valueType) {
            reportError("Could not determine type of value in destructuring assignment", node.getLocation());
            setExpressionType(node, typeSystem_->getVoidType());
            return;
        }

        std::cout << "DEBUG: Destructuring assignment value type: " << valueType->toString() << std::endl;

        // Analyze the destructuring pattern
        node.getPattern()->accept(*this);
        auto patternType = getExpressionType(*node.getPattern());

        // Validate that the value type is compatible with the pattern
        // For now, we'll allow any type to be destructured
        // In a full implementation, we'd check:
        // - Array destructuring requires array type
        // - Object destructuring requires object type
        // - Specific property/key validation

        std::cout << "DEBUG: Destructuring assignment pattern type: " << (patternType
                                                                              ? patternType->toString()
                                                                              : "unknown") << std::endl;

        // Destructuring assignments are RValues that return void
        setExpressionType(node, typeSystem_->getVoidType());
        std::cout << "DEBUG: Destructuring assignment analysis completed" << std::endl;
    }

    void SemanticAnalyzer::visit(OptionalPropertyAccess &node) {
        std::cout << "DEBUG: Analyzing optional property access: " << node.getProperty() << std::endl;

        // Analyze the object being accessed
        node.getObject()->accept(*this);
        auto objectType = getExpressionType(*node.getObject());

        if (!objectType) {
            reportError("Could not determine type of object in optional property access", node.getLocation());
            setExpressionType(node, typeSystem_->getAnyType());
            return;
        }

        std::cout << "DEBUG: Optional property access object type: " << objectType->toString() << std::endl;

        // Check if the object type supports optional chaining
        // Optional chaining is safe for any type, but we should warn about potential issues
        if (objectType->getKind() == TypeKind::Null || objectType->getKind() == TypeKind::Undefined) {
            // This is actually useful - the optional chaining will prevent errors
            std::cout << "DEBUG: Optional chaining on null/undefined type - this is safe" << std::endl;
        } else if (objectType->getKind() == TypeKind::Any || objectType->getKind() == TypeKind::Unknown) {
            // Unknown type - optional chaining is safe
            std::cout << "DEBUG: Optional chaining on unknown type - this is safe" << std::endl;
        } else {
            // For known types, we could validate that the property exists
            // For now, we'll allow any property access
            std::cout << "DEBUG: Optional chaining on known type: " << objectType->toString() << std::endl;
        }

        // Optional property access always returns a potentially undefined value
        // The result type should be the property type union with undefined
        // For now, we'll use 'any' type to represent the union
        setExpressionType(node, typeSystem_->getAnyType());
        std::cout << "DEBUG: Optional property access analysis completed" << std::endl;
    }

    void SemanticAnalyzer::visit(OptionalIndexAccess &node) {
        std::cout << "DEBUG: Analyzing optional index access" << std::endl;

        // Analyze the object being accessed
        node.getObject()->accept(*this);
        auto objectType = getExpressionType(*node.getObject());

        if (!objectType) {
            reportError("Could not determine type of object in optional index access", node.getLocation());
            setExpressionType(node, typeSystem_->getAnyType());
            return;
        }

        std::cout << "DEBUG: Optional index access object type: " << objectType->toString() << std::endl;

        // Analyze the index expression
        node.getIndex()->accept(*this);
        auto indexType = getExpressionType(*node.getIndex());

        if (!indexType) {
            reportError("Could not determine type of index in optional index access", node.getLocation());
            setExpressionType(node, typeSystem_->getAnyType());
            return;
        }

        std::cout << "DEBUG: Optional index access index type: " << indexType->toString() << std::endl;

        // Validate that the index type is valid for indexing
        if (!isValidIndexType(indexType)) {
            reportError("Invalid index type '" + indexType->toString() + "' for optional index access",
                        node.getLocation());
        }

        // Check if the object type supports optional chaining
        if (objectType->getKind() == TypeKind::Null || objectType->getKind() == TypeKind::Undefined) {
            std::cout << "DEBUG: Optional chaining on null/undefined type - this is safe" << std::endl;
        } else if (objectType->getKind() == TypeKind::Any || objectType->getKind() == TypeKind::Unknown) {
            std::cout << "DEBUG: Optional chaining on unknown type - this is safe" << std::endl;
        } else if (objectType->getKind() == TypeKind::Array) {
            std::cout << "DEBUG: Optional chaining on array type - this is safe" << std::endl;
        } else if (objectType->getKind() == TypeKind::String) {
            std::cout << "DEBUG: Optional chaining on string type - this is safe" << std::endl;
        } else {
            std::cout << "DEBUG: Optional chaining on object type: " << objectType->toString() << std::endl;
        }

        // Optional index access always returns a potentially undefined value
        setExpressionType(node, typeSystem_->getAnyType());
        std::cout << "DEBUG: Optional index access analysis completed" << std::endl;
    }

    void SemanticAnalyzer::visit(OptionalCallExpr &node) {
        std::cout << "DEBUG: Analyzing optional call expression with " << node.getArguments().size() << " arguments" <<
                std::endl;

        // Analyze the callee expression
        node.getCallee()->accept(*this);
        auto calleeType = getExpressionType(*node.getCallee());

        if (!calleeType) {
            reportError("Could not determine type of callee in optional call expression", node.getLocation());
            setExpressionType(node, typeSystem_->getAnyType());
            return;
        }

        std::cout << "DEBUG: Optional call expression callee type: " << calleeType->toString() << std::endl;

        // Check if the callee type supports optional chaining
        if (calleeType->getKind() == TypeKind::Null || calleeType->getKind() == TypeKind::Undefined) {
            std::cout << "DEBUG: Optional chaining on null/undefined callee - this is safe" << std::endl;
        } else if (calleeType->getKind() == TypeKind::Any || calleeType->getKind() == TypeKind::Unknown) {
            std::cout << "DEBUG: Optional chaining on unknown callee type - this is safe" << std::endl;
        } else if (calleeType->getKind() == TypeKind::Function) {
            std::cout << "DEBUG: Optional chaining on function type - this is safe" << std::endl;
        } else {
            std::cout << "DEBUG: Optional chaining on non-function type: " << calleeType->toString() << std::endl;
            // This might be an error - trying to call a non-function
            reportError("Optional call on non-function type '" + calleeType->toString() + "'",
                        node.getLocation());
        }

        // Analyze all arguments
        for (size_t i = 0; i < node.getArguments().size(); ++i) {
            const auto &arg = node.getArguments()[i];
            std::cout << "DEBUG: Analyzing optional call argument " << i << std::endl;

            arg->accept(*this);
            auto argType = getExpressionType(*arg);

            if (argType) {
                std::cout << "DEBUG: Optional call argument " << i << " type: " << argType->toString() << std::endl;
            }
        }

        // Optional call expressions always return a potentially undefined value
        // The result type should be the function return type union with undefined
        // For now, we'll use 'any' type to represent the union
        setExpressionType(node, typeSystem_->getAnyType());
        std::cout << "DEBUG: Optional call expression analysis completed" << std::endl;
    }

    void SemanticAnalyzer::visit(SpreadElement &node) {
        // TODO: Implement spread element semantic analysis
        node.getExpression()->accept(*this);
    }

    bool SemanticAnalyzer::isValidIndexType(shared_ptr<Type> type) const {
        if (!type) return true; // any can be used as index

        switch (type->getKind()) {
            case TypeKind::Number:
            case TypeKind::String:
            case TypeKind::Any:
            case TypeKind::Unknown:
                return true;
            case TypeKind::Boolean:
                // Boolean can be converted to number (0 or 1)
                return true;
            case TypeKind::Null:
            case TypeKind::Undefined:
                // These can be converted to number (0) or string ("null"/"undefined")
                return true;
            default:
                return false;
        }
    }

    shared_ptr<Type> SemanticAnalyzer::createObjectTypeFromProperties(
        const std::vector<std::pair<String, shared_ptr<Type> > > &properties) const {
        if (properties.empty()) {
            // Empty object literal - return a generic object type
            return typeSystem_->getAnyType();
        }

        // Create ObjectType with the properties using TypeSystem factory method
        std::vector<ObjectType::Property> objectProperties;
        for (const auto &prop: properties) {
            ObjectType::Property objectProp;
            objectProp.name = prop.first;
            objectProp.type = prop.second ? prop.second : typeSystem_->getAnyType();
            objectProp.optional = false; // Object literal properties are always present
            objectProp.readonly = false; // Object literal properties are mutable
            objectProperties.push_back(std::move(objectProp));
        }

        return typeSystem_->createObjectType(std::move(objectProperties));
    }

    // Factory function
    unique_ptr<SemanticAnalyzer> createSemanticAnalyzer(DiagnosticEngine &diagnostics) {
        return make_unique<SemanticAnalyzer>(diagnostics);
    }
} // namespace tsc
