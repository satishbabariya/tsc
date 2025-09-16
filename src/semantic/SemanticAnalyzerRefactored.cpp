#include "tsc/semantic/SemanticAnalyzerRefactored.h"
#include "tsc/semantic/resolution/SymbolResolutionEngine.h"
#include "tsc/semantic/checking/TypeCheckingEngine.h"
#include "tsc/semantic/flow/FlowAnalysisEngine.h"
#include "tsc/semantic/resolution/InheritanceResolver.h"

#include "tsc/semantic/GenericConstraintChecker.h"
#include "tsc/semantic/ModuleResolver.h"
#include "tsc/semantic/DependencyScanner.h"
#include "tsc/semantic/ModuleSymbolTable.h"
#include "tsc/semantic/CycleDetector.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/EnhancedErrorReporting.h"

#include <iostream>
#include <fstream>

namespace tsc {
namespace semantic {

// SemanticContext implementation
SemanticContext::SemanticContext(SymbolTable& symbolTable, TypeSystem& typeSystem, DiagnosticEngine& diagnostics)
    : symbolTable_(symbolTable), typeSystem_(typeSystem), diagnostics_(diagnostics) {
}

void SemanticContext::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
    errorCount_++;
}

void SemanticContext::reportWarning(const String& message, const SourceLocation& location) {
    diagnostics_.warning(message, location);
    warningCount_++;
}

// SemanticAnalyzer implementation
SemanticAnalyzer::SemanticAnalyzer(DiagnosticEngine& diagnostics)
    : diagnostics_(diagnostics) {
    symbolTable_ = make_unique<SymbolTable>();
    typeSystem_ = make_unique<TypeSystem>();
    context_ = make_unique<SemanticContext>(*symbolTable_, *typeSystem_, diagnostics_);
    constraintChecker_ = make_unique<GenericConstraintChecker>(diagnostics_, *typeSystem_);
    moduleResolver_ = make_unique<ModuleResolver>(diagnostics_);
    dependencyScanner_ = make_unique<DependencyScanner>(*moduleResolver_, diagnostics_);
    moduleSymbolManager_ = make_unique<ModuleSymbolManager>(diagnostics_, symbolTable_.get());
    cycleDetector_ = make_unique<CycleDetector>(symbolTable_.get());
    errorReporter_ = make_unique<EnhancedErrorReporting>(diagnostics_);

    // Configure error reporting to treat warnings as warnings, not errors
    errorReporter_->setWarningsAsErrors(false);
    errorReporter_->setSuppressWarnings(false);

    std::cout << "DEBUG: SemanticAnalyzer created SymbolTable at address: " << symbolTable_.get() << std::endl;

    // Initialize analysis engines
    symbolResolutionEngine_ = std::make_unique<SymbolResolutionEngine>(*context_);
    typeCheckingEngine_ = std::make_unique<TypeCheckingEngine>(*context_);
    flowAnalysisEngine_ = std::make_unique<FlowAnalysisEngine>(*context_);
    inheritanceResolver_ = std::make_unique<InheritanceResolver>(*context_);

    setupBuiltinEnvironment();
}

SemanticAnalyzer::~SemanticAnalyzer() = default;

bool SemanticAnalyzer::analyze(Module& module) {
    std::cout << "DEBUG: *** ANALYZE METHOD CALLED *** for module" << std::endl;
    std::cout.flush();
    try {
        // Multi-phase semantic analysis
        std::cout << "DEBUG: *** CALLING performSymbolResolution ***" << std::endl;
        std::cout.flush();
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

bool SemanticAnalyzer::analyzeProject(const std::vector<String>& modulePaths) {
    try {
        std::cout << "DEBUG: Starting multi-module analysis for " << modulePaths.size() << " modules" << std::endl;

        // Phase 1: Scan dependencies and detect circular dependencies
        auto dependencyGraph = dependencyScanner_->scanProjectWithValidation(modulePaths);
        if (!dependencyGraph) {
            diagnostics_.error("Failed to scan project dependencies", SourceLocation());
            return false;
        }

        // Phase 2: Create module symbol tables for each module
        for (const String& modulePath : modulePaths) {
            ModuleSymbolTable* moduleTable = moduleSymbolManager_->createModuleSymbolTable(modulePath);
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
        std::vector<std::unique_ptr<Module>> astModules;
        astModules.reserve(compilationOrder.size());

        for (const String& modulePath : compilationOrder) {
            std::cout << "DEBUG: Analyzing module: " << modulePath << std::endl;
            std::cout << "DEBUG: *** ABOUT TO READ FILE *** " << modulePath << std::endl;
            std::cout.flush();

            // Read and parse the module
            std::ifstream file(modulePath);
            if (!file.is_open()) {
                diagnostics_.error("Failed to open file: " + modulePath, SourceLocation());
                return false;
            }

            String content((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
            file.close();

            // Parse the module
            Lexer lexer(content, diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, diagnostics_);
            auto module = parser.parseModule();

            if (!module) {
                diagnostics_.error("Failed to parse module: " + modulePath, SourceLocation());
                return false;
            }

            // Store the module and analyze it
            astModules.push_back(std::move(module));
            Module* currentModule = astModules.back().get();

            // Set the module path for proper error reporting
            currentModule->setSourcePath(modulePath);

            // Analyze the module
            if (!analyze(*currentModule)) {
                return false;
            }
        }

        return true;
    } catch (const std::exception& e) {
        diagnostics_.error("Project analysis failed: " + String(e.what()), SourceLocation());
        return false;
    }
}

void SemanticAnalyzer::reportError(const String& message, const SourceLocation& location) {
    diagnostics_.error(message, location);
}

void SemanticAnalyzer::reportWarning(const String& message, const SourceLocation& location) {
    diagnostics_.warning(message, location);
}

// Analysis phases
void SemanticAnalyzer::performSymbolResolution(Module& module) {
    symbolResolutionEngine_->resolveSymbols(module);
}

void SemanticAnalyzer::performTypeChecking(Module& module) {
    typeCheckingEngine_->performTypeChecking(module);
}

void SemanticAnalyzer::performFlowAnalysis(Module& module) {
    flowAnalysisEngine_->performFlowAnalysis(module);
}

void SemanticAnalyzer::collectFunctionDeclarations(Module& module) {
    symbolResolutionEngine_->collectFunctionDeclarations(module);
}

void SemanticAnalyzer::resolveInheritance(Module& module) {
    inheritanceResolver_->resolveInheritance(module);
}

void SemanticAnalyzer::resolveDeferredSuperExpressions() {
    symbolResolutionEngine_->resolveDeferredSuperExpressions();
}

void SemanticAnalyzer::resolveDeferredSuperPropertyAccesses() {
    symbolResolutionEngine_->resolveDeferredSuperPropertyAccesses();
}

void SemanticAnalyzer::resolveDeferredSuperCallExpressions() {
    symbolResolutionEngine_->resolveDeferredSuperCallExpressions();
}

void SemanticAnalyzer::setupBuiltinEnvironment() {
    // TODO: Implement builtin environment setup
}

Symbol* SemanticAnalyzer::findInterfaceMember(const InterfaceType& interfaceType, const String& memberName) {
    return inheritanceResolver_->findInterfaceMember(interfaceType, memberName);
}

// Visitor implementations - delegate to appropriate engines
void SemanticAnalyzer::visit(NumericLiteral& node) {
    // Type checking handles literal types
}

void SemanticAnalyzer::visit(StringLiteral& node) {
    // Type checking handles literal types
}

void SemanticAnalyzer::visit(TemplateLiteral& node) {
    // Type checking handles template literals
}

void SemanticAnalyzer::visit(BooleanLiteral& node) {
    // Type checking handles literal types
}

void SemanticAnalyzer::visit(NullLiteral& node) {
    // Type checking handles literal types
}

void SemanticAnalyzer::visit(Identifier& node) {
    // Symbol resolution handles identifier resolution
}

void SemanticAnalyzer::visit(ThisExpression& node) {
    // Symbol resolution handles this expressions
}

void SemanticAnalyzer::visit(SuperExpression& node) {
    // Symbol resolution handles super expressions
}

void SemanticAnalyzer::visit(NewExpression& node) {
    // Type checking handles new expressions
}

void SemanticAnalyzer::visit(BinaryExpression& node) {
    // Type checking handles binary expressions
}

void SemanticAnalyzer::visit(UnaryExpression& node) {
    // Type checking handles unary expressions
}

void SemanticAnalyzer::visit(AssignmentExpression& node) {
    // Type checking handles assignment expressions
}

void SemanticAnalyzer::visit(ConditionalExpression& node) {
    // Type checking handles conditional expressions
}

void SemanticAnalyzer::visit(CallExpression& node) {
    // Type checking handles call expressions
}

void SemanticAnalyzer::visit(ArrayLiteral& node) {
    // Type checking handles array literals
}

void SemanticAnalyzer::visit(IndexExpression& node) {
    // Type checking handles index expressions
}

void SemanticAnalyzer::visit(ObjectLiteral& node) {
    // Type checking handles object literals
}

void SemanticAnalyzer::visit(PropertyAccess& node) {
    // Type checking handles property access
}

void SemanticAnalyzer::visit(ArrowFunction& node) {
    // Type checking handles arrow functions
}

void SemanticAnalyzer::visit(FunctionExpression& node) {
    // Type checking handles function expressions
}

void SemanticAnalyzer::visit(ExpressionStatement& node) {
    // Flow analysis handles expression statements
}

void SemanticAnalyzer::visit(BlockStatement& node) {
    // Flow analysis handles block statements
}

void SemanticAnalyzer::visit(ReturnStatement& node) {
    // Flow analysis handles return statements
}

void SemanticAnalyzer::visit(IfStatement& node) {
    // Flow analysis handles if statements
}

void SemanticAnalyzer::visit(WhileStatement& node) {
    // Flow analysis handles while statements
}

void SemanticAnalyzer::visit(DoWhileStatement& node) {
    // Flow analysis handles do-while statements
}

void SemanticAnalyzer::visit(ForStatement& node) {
    // Flow analysis handles for statements
}

void SemanticAnalyzer::visit(ForOfStatement& node) {
    // Flow analysis handles for-of statements
}

void SemanticAnalyzer::visit(SwitchStatement& node) {
    // Flow analysis handles switch statements
}

void SemanticAnalyzer::visit(CaseClause& node) {
    // Flow analysis handles case clauses
}

void SemanticAnalyzer::visit(BreakStatement& node) {
    // Flow analysis handles break statements
}

void SemanticAnalyzer::visit(ContinueStatement& node) {
    // Flow analysis handles continue statements
}

void SemanticAnalyzer::visit(TryStatement& node) {
    // Flow analysis handles try statements
}

void SemanticAnalyzer::visit(CatchClause& node) {
    // Flow analysis handles catch clauses
}

void SemanticAnalyzer::visit(ThrowStatement& node) {
    // Flow analysis handles throw statements
}

void SemanticAnalyzer::visit(VariableDeclaration& node) {
    // Symbol resolution and type checking handle variable declarations
}

void SemanticAnalyzer::visit(FunctionDeclaration& node) {
    // Symbol resolution and type checking handle function declarations
}

void SemanticAnalyzer::visit(TypeParameter& node) {
    // Type checking handles type parameters
}

void SemanticAnalyzer::visit(Module& module) {
    // Process all declarations in the module
    for (const auto& declaration : module.getDeclarations()) {
        declaration->accept(static_cast<ASTVisitor&>(*this));
    }
}

void SemanticAnalyzer::visit(PropertyDeclaration& node) {
    // Type checking handles property declarations
}

void SemanticAnalyzer::visit(MethodDeclaration& node) {
    // Type checking handles method declarations
}

void SemanticAnalyzer::visit(DestructorDeclaration& node) {
    // Type checking handles destructor declarations
}

void SemanticAnalyzer::visit(ClassDeclaration& node) {
    // Symbol resolution and type checking handle class declarations
}

void SemanticAnalyzer::visit(InterfaceDeclaration& node) {
    // Symbol resolution and type checking handle interface declarations
}

void SemanticAnalyzer::visit(EnumMember& node) {
    // Type checking handles enum members
}

void SemanticAnalyzer::visit(EnumDeclaration& node) {
    // Type checking handles enum declarations
}

void SemanticAnalyzer::visit(TypeAliasDeclaration& node) {
    // Type checking handles type alias declarations
}

void SemanticAnalyzer::visit(ImportDeclaration& node) {
    // Symbol resolution handles import declarations
}

void SemanticAnalyzer::visit(ExportDeclaration& node) {
    // Symbol resolution handles export declarations
}

void SemanticAnalyzer::visit(DestructuringPattern& node) {
    // Type checking handles destructuring patterns
}

void SemanticAnalyzer::visit(ArrayDestructuringPattern& node) {
    // Type checking handles array destructuring patterns
}

void SemanticAnalyzer::visit(ObjectDestructuringPattern& node) {
    // Type checking handles object destructuring patterns
}

void SemanticAnalyzer::visit(IdentifierPattern& node) {
    // Type checking handles identifier patterns
}

void SemanticAnalyzer::visit(DestructuringAssignment& node) {
    // Type checking handles destructuring assignments
}

} // namespace semantic
} // namespace tsc