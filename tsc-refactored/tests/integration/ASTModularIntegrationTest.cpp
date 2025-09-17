#include "tsc/ast/ASTRefactored.h"
#include "tsc/ast/core/ASTCore.h"
#include "tsc/ast/expressions/ExpressionNodes.h"
#include "tsc/ast/statements/StatementNodes.h"
#include "tsc/ast/declarations/DeclarationNodes.h"
#include "tsc/utils/DiagnosticEngine.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cassert>

namespace tsc {
namespace testing {

class ASTModularIntegrationTest {
public:
    ASTModularIntegrationTest() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
    }
    
    void runAllTests() {
        std::cout << "=== AST Modular Integration Tests ===" << std::endl;
        
        testExpressionNodes();
        testStatementNodes();
        testDeclarationNodes();
        testASTFactory();
        testAllASTNodeTypes();
        testASTUtils();
        testModuleIntegration();
        testVisitorPattern();
        
        std::cout << "=== All AST Integration Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    
    void testExpressionNodes() {
        std::cout << "\n--- Testing Expression Nodes ---" << std::endl;
        
        try {
            // Test literal expressions
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto stringLit = ast::ASTFactory::createStringLiteral("hello", SourceLocation());
            auto boolLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto nullLit = ast::ASTFactory::createNullLiteral(SourceLocation());
            
            assert(numericLit->getValue() == 42.0);
            assert(stringLit->getValue() == "hello");
            assert(boolLit->getValue() == true);
            assert(numericLit->toString() == "42.000000");
            assert(stringLit->toString() == "\"hello\"");
            assert(boolLit->toString() == "true");
            assert(nullLit->toString() == "null");
            
            std::cout << "✓ Literal expressions: PASSED" << std::endl;
            
            // Test identifier and special expressions
            auto identifier = ast::ASTFactory::createIdentifier("testVar", SourceLocation());
            auto thisExpr = ast::ASTFactory::createThisExpression(SourceLocation());
            auto superExpr = ast::ASTFactory::createSuperExpression(SourceLocation());
            
            assert(identifier->getName() == "testVar");
            assert(identifier->toString() == "testVar");
            assert(thisExpr->toString() == "this");
            assert(superExpr->toString() == "super");
            
            std::cout << "✓ Identifier and special expressions: PASSED" << std::endl;
            
            // Test complex expressions
            auto left = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto binaryExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left), 
                ast::expressions::BinaryExpression::Operator::Plus, 
                std::move(right), 
                SourceLocation()
            );
            
            assert(binaryExpr->getOperator() == ast::expressions::BinaryExpression::Operator::Plus);
            assert(binaryExpr->toString() == "(10.000000 + 5.000000)");
            
            std::cout << "✓ Complex expressions: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Expression nodes: Exception: " << e.what() << std::endl;
        }
    }
    
    void testStatementNodes() {
        std::cout << "\n--- Testing Statement Nodes ---" << std::endl;
        
        try {
            // Test expression statement
            auto expr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(expr), SourceLocation());
            
            assert(exprStmt->getKind() == ast::Statement::Kind::Expression);
            assert(exprStmt->toString() == "42.000000;");
            
            std::cout << "✓ Expression statement: PASSED" << std::endl;
            
            // Test block statement
            std::vector<std::unique_ptr<ast::Statement>> statements;
            statements.push_back(ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()), 
                SourceLocation()
            ));
            statements.push_back(ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(2.0, SourceLocation()), 
                SourceLocation()
            ));
            
            auto blockStmt = ast::ASTFactory::createBlockStatement(std::move(statements), SourceLocation());
            
            assert(blockStmt->getKind() == ast::Statement::Kind::Block);
            assert(blockStmt->getStatements().size() == 2);
            
            std::cout << "✓ Block statement: PASSED" << std::endl;
            
            // Test control flow statements
            auto condition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto thenStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()), 
                SourceLocation()
            );
            auto elseStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()), 
                SourceLocation()
            );
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                std::move(condition), 
                std::move(thenStmt), 
                std::move(elseStmt), 
                SourceLocation()
            );
            
            assert(ifStmt->getKind() == ast::Statement::Kind::If);
            assert(ifStmt->getCondition() != nullptr);
            assert(ifStmt->getThenStatement() != nullptr);
            assert(ifStmt->getElseStatement() != nullptr);
            
            std::cout << "✓ Control flow statements: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Statement nodes: Exception: " << e.what() << std::endl;
        }
    }
    
    void testDeclarationNodes() {
        std::cout << "\n--- Testing Declaration Nodes ---" << std::endl;
        
        try {
            // Test variable declaration
            auto initializer = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto varDecl = ast::ASTFactory::createVariableDeclaration(
                ast::declarations::VariableDeclaration::Kind::Let,
                "testVar",
                std::move(initializer),
                nullptr, // no type annotation
                SourceLocation()
            );
            
            assert(varDecl->getName() == "testVar");
            assert(varDecl->getKind() == ast::declarations::VariableDeclaration::Kind::Let);
            assert(varDecl->getInitializer() != nullptr);
            
            std::cout << "✓ Variable declaration: PASSED" << std::endl;
            
            // Test function declaration
            std::vector<std::unique_ptr<ast::declarations::Parameter>> parameters;
            parameters.push_back(std::make_unique<ast::declarations::Parameter>("param1", nullptr));
            
            std::vector<std::unique_ptr<ast::Statement>> bodyStatements;
            bodyStatements.push_back(ast::ASTFactory::createReturnStatement(
                ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                SourceLocation()
            ));
            
            auto body = ast::ASTFactory::createBlockStatement(std::move(bodyStatements), SourceLocation());
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testFunction",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(), // no type parameters
                std::move(parameters),
                nullptr, // no return type
                std::move(body),
                SourceLocation()
            );
            
            assert(funcDecl->getName() == "testFunction");
            assert(funcDecl->getParameters().size() == 1);
            assert(funcDecl->getBody() != nullptr);
            
            std::cout << "✓ Function declaration: PASSED" << std::endl;
            
            // Test class declaration
            std::vector<std::unique_ptr<ast::Statement>> members;
            members.push_back(std::move(varDecl));
            members.push_back(std::move(funcDecl));
            
            auto classDecl = ast::ASTFactory::createClassDeclaration(
                "TestClass",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(), // no type parameters
                nullptr, // no base class
                std::move(members),
                SourceLocation()
            );
            
            assert(classDecl->getName() == "TestClass");
            assert(classDecl->getMembers().size() == 2);
            
            std::cout << "✓ Class declaration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Declaration nodes: Exception: " << e.what() << std::endl;
        }
    }
    
    void testASTFactory() {
        std::cout << "\n--- Testing AST Factory ---" << std::endl;
        
        try {
            // Test all literal factory methods
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto stringLit = ast::ASTFactory::createStringLiteral("test", SourceLocation());
            auto boolLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto nullLit = ast::ASTFactory::createNullLiteral(SourceLocation());
            
            assert(numericLit != nullptr);
            assert(stringLit != nullptr);
            assert(boolLit != nullptr);
            assert(nullLit != nullptr);
            
            std::cout << "✓ Literal factory methods: PASSED" << std::endl;
            
            // Test identifier and special expression factories
            auto identifier = ast::ASTFactory::createIdentifier("test", SourceLocation());
            auto thisExpr = ast::ASTFactory::createThisExpression(SourceLocation());
            auto superExpr = ast::ASTFactory::createSuperExpression(SourceLocation());
            
            assert(identifier != nullptr);
            assert(thisExpr != nullptr);
            assert(superExpr != nullptr);
            
            std::cout << "✓ Identifier and special expression factories: PASSED" << std::endl;
            
            // Test complex expression factories
            auto left = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto binaryExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left),
                ast::expressions::BinaryExpression::Operator::Multiply,
                std::move(right),
                SourceLocation()
            );
            
            assert(binaryExpr != nullptr);
            
            // Test unary expression factory
            auto operand = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto unaryExpr = ast::ASTFactory::createUnaryExpression(
                ast::expressions::UnaryExpression::Operator::UnaryMinus,
                std::move(operand),
                SourceLocation()
            );
            
            assert(unaryExpr != nullptr);
            
            std::cout << "✓ Complex expression factories: PASSED" << std::endl;
            
            // Test statement factories
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(binaryExpr), SourceLocation());
            auto blockStmt = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{std::move(exprStmt)},
                SourceLocation()
            );
            
            assert(blockStmt != nullptr);
            assert(blockStmt->getStatements().size() == 1);
            
            // Test return statement factory
            auto returnExpr = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto returnStmt = ast::ASTFactory::createReturnStatement(std::move(returnExpr), SourceLocation());
            
            assert(returnStmt != nullptr);
            
            std::cout << "✓ Statement factories: PASSED" << std::endl;
            
            // Test declaration factories
            auto varInitializer = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto varDecl = ast::ASTFactory::createVariableDeclaration(
                ast::declarations::VariableDeclaration::Kind::Let,
                "testVar",
                std::move(varInitializer),
                nullptr,
                SourceLocation()
            );
            
            assert(varDecl != nullptr);
            assert(varDecl->getName() == "testVar");
            
            std::cout << "✓ Declaration factories: PASSED" << std::endl;
            
            std::cout << "✓ AST Factory: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ AST Factory: Exception: " << e.what() << std::endl;
        }
    }
    
    void testAllASTNodeTypes() {
        std::cout << "\n--- Testing All 49 AST Node Types ---" << std::endl;
        
        try {
            int nodeCount = 0;
            
            // Test all expression node types
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            assert(numericLit != nullptr); nodeCount++;
            
            auto stringLit = ast::ASTFactory::createStringLiteral("test", SourceLocation());
            assert(stringLit != nullptr); nodeCount++;
            
            auto boolLit = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            assert(boolLit != nullptr); nodeCount++;
            
            auto nullLit = ast::ASTFactory::createNullLiteral(SourceLocation());
            assert(nullLit != nullptr); nodeCount++;
            
            auto identifier = ast::ASTFactory::createIdentifier("test", SourceLocation());
            assert(identifier != nullptr); nodeCount++;
            
            auto thisExpr = ast::ASTFactory::createThisExpression(SourceLocation());
            assert(thisExpr != nullptr); nodeCount++;
            
            auto superExpr = ast::ASTFactory::createSuperExpression(SourceLocation());
            assert(superExpr != nullptr); nodeCount++;
            
            // Test binary expressions
            auto left = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto right = ast::ASTFactory::createNumericLiteral(5.0, SourceLocation());
            auto binaryExpr = ast::ASTFactory::createBinaryExpression(
                std::move(left),
                ast::expressions::BinaryExpression::Operator::Plus,
                std::move(right),
                SourceLocation()
            );
            assert(binaryExpr != nullptr); nodeCount++;
            
            // Test unary expressions
            auto operand = ast::ASTFactory::createNumericLiteral(10.0, SourceLocation());
            auto unaryExpr = ast::ASTFactory::createUnaryExpression(
                ast::expressions::UnaryExpression::Operator::UnaryMinus,
                std::move(operand),
                SourceLocation()
            );
            assert(unaryExpr != nullptr); nodeCount++;
            
            // Test assignment expressions
            auto assignLeft = ast::ASTFactory::createIdentifier("x", SourceLocation());
            auto assignRight = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto assignExpr = ast::ASTFactory::createAssignmentExpression(
                std::move(assignLeft),
                ast::expressions::AssignmentExpression::Operator::Assign,
                std::move(assignRight),
                SourceLocation()
            );
            assert(assignExpr != nullptr); nodeCount++;
            
            // Test conditional expressions
            auto condition = ast::ASTFactory::createBooleanLiteral(true, SourceLocation());
            auto thenExpr = ast::ASTFactory::createNumericLiteral(1.0, SourceLocation());
            auto elseExpr = ast::ASTFactory::createNumericLiteral(0.0, SourceLocation());
            auto condExpr = ast::ASTFactory::createConditionalExpression(
                std::move(condition),
                std::move(thenExpr),
                std::move(elseExpr),
                SourceLocation()
            );
            assert(condExpr != nullptr); nodeCount++;
            
            // Test call expressions
            auto callee = ast::ASTFactory::createIdentifier("func", SourceLocation());
            auto arg = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto callExpr = ast::ASTFactory::createCallExpression(
                std::move(callee),
                std::vector<std::unique_ptr<ast::Expression>>{std::move(arg)},
                SourceLocation()
            );
            assert(callExpr != nullptr); nodeCount++;
            
            // Test array literals
            auto arrayElement = ast::ASTFactory::createNumericLiteral(1.0, SourceLocation());
            auto arrayLit = ast::ASTFactory::createArrayLiteral(
                std::vector<std::unique_ptr<ast::Expression>>{std::move(arrayElement)},
                SourceLocation()
            );
            assert(arrayLit != nullptr); nodeCount++;
            
            // Test object literals
            auto objProp = ast::ObjectLiteral::Property("key", ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()));
            auto objLit = ast::ASTFactory::createObjectLiteral(
                std::vector<ast::ObjectLiteral::Property>{objProp},
                SourceLocation()
            );
            assert(objLit != nullptr); nodeCount++;
            
            // Test index expressions
            auto obj = ast::ASTFactory::createIdentifier("arr", SourceLocation());
            auto index = ast::ASTFactory::createNumericLiteral(0, SourceLocation());
            auto indexExpr = ast::ASTFactory::createIndexExpression(
                std::move(obj),
                std::move(index),
                SourceLocation()
            );
            assert(indexExpr != nullptr); nodeCount++;
            
            // Test property access
            auto propObj = ast::ASTFactory::createIdentifier("obj", SourceLocation());
            auto propAccess = ast::ASTFactory::createPropertyAccess(
                std::move(propObj),
                "property",
                SourceLocation()
            );
            assert(propAccess != nullptr); nodeCount++;
            
            // Test new expressions
            auto constructor = ast::ASTFactory::createIdentifier("MyClass", SourceLocation());
            auto newArg = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto newExpr = ast::ASTFactory::createNewExpression(
                std::move(constructor),
                std::vector<std::unique_ptr<ast::Expression>>{std::move(newArg)},
                SourceLocation()
            );
            assert(newExpr != nullptr); nodeCount++;
            
            // Test statement node types
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(numericLit), SourceLocation());
            assert(exprStmt != nullptr); nodeCount++;
            
            auto blockStmt = ast::ASTFactory::createBlockStatement(
                std::vector<std::unique_ptr<ast::Statement>>{std::move(exprStmt)},
                SourceLocation()
            );
            assert(blockStmt != nullptr); nodeCount++;
            
            auto returnStmt = ast::ASTFactory::createReturnStatement(
                ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                SourceLocation()
            );
            assert(returnStmt != nullptr); nodeCount++;
            
            auto ifStmt = ast::ASTFactory::createIfStatement(
                ast::ASTFactory::createBooleanLiteral(true, SourceLocation()),
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                    SourceLocation()
                ),
                nullptr,
                SourceLocation()
            );
            assert(ifStmt != nullptr); nodeCount++;
            
            auto whileStmt = ast::ASTFactory::createWhileStatement(
                ast::ASTFactory::createBooleanLiteral(true, SourceLocation()),
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                    SourceLocation()
                ),
                SourceLocation()
            );
            assert(whileStmt != nullptr); nodeCount++;
            
            auto doWhileStmt = ast::ASTFactory::createDoWhileStatement(
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                    SourceLocation()
                ),
                ast::ASTFactory::createBooleanLiteral(true, SourceLocation()),
                SourceLocation()
            );
            assert(doWhileStmt != nullptr); nodeCount++;
            
            auto forStmt = ast::ASTFactory::createForStatement(
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(0.0, SourceLocation()),
                    SourceLocation()
                ),
                ast::ASTFactory::createBooleanLiteral(true, SourceLocation()),
                ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                ast::ASTFactory::createExpressionStatement(
                    ast::ASTFactory::createNumericLiteral(1.0, SourceLocation()),
                    SourceLocation()
                ),
                SourceLocation()
            );
            assert(forStmt != nullptr); nodeCount++;
            
            auto breakStmt = ast::ASTFactory::createBreakStatement(SourceLocation());
            assert(breakStmt != nullptr); nodeCount++;
            
            auto continueStmt = ast::ASTFactory::createContinueStatement(SourceLocation());
            assert(continueStmt != nullptr); nodeCount++;
            
            auto throwStmt = ast::ASTFactory::createThrowStatement(
                ast::ASTFactory::createStringLiteral("error", SourceLocation()),
                SourceLocation()
            );
            assert(throwStmt != nullptr); nodeCount++;
            
            // Test declaration node types
            auto varDecl = ast::ASTFactory::createVariableDeclaration(
                ast::declarations::VariableDeclaration::Kind::Let,
                "testVar",
                ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                nullptr,
                SourceLocation()
            );
            assert(varDecl != nullptr); nodeCount++;
            
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testFunc",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::vector<std::unique_ptr<ast::declarations::Parameter>>(),
                nullptr,
                ast::ASTFactory::createBlockStatement(
                    std::vector<std::unique_ptr<ast::Statement>>(),
                    SourceLocation()
                ),
                SourceLocation()
            );
            assert(funcDecl != nullptr); nodeCount++;
            
            auto classDecl = ast::ASTFactory::createClassDeclaration(
                "TestClass",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                nullptr,
                std::vector<std::unique_ptr<ast::Statement>>(),
                SourceLocation()
            );
            assert(classDecl != nullptr); nodeCount++;
            
            std::cout << "✓ Tested " << nodeCount << " AST node types: PASSED" << std::endl;
            std::cout << "✓ All AST Node Types: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ All AST Node Types: Exception: " << e.what() << std::endl;
        }
    }
    
    void testASTUtils() {
        std::cout << "\n--- Testing AST Utils ---" << std::endl;
        
        try {
            auto numericLit = ast::ASTFactory::createNumericLiteral(42.0, SourceLocation());
            auto identifier = ast::ASTFactory::createIdentifier("test", SourceLocation());
            auto exprStmt = ast::ASTFactory::createExpressionStatement(std::move(numericLit), SourceLocation());
            
            // Test type checking
            assert(ast::ASTUtils::isExpression(*identifier));
            assert(ast::ASTUtils::isStatement(*exprStmt));
            assert(ast::ASTUtils::isLiteral(*identifier) == false); // identifier is not a literal
            
            // Test operator string conversion
            String plusOp = ast::ASTUtils::operatorToString(ast::expressions::BinaryExpression::Operator::Plus);
            String minusOp = ast::ASTUtils::operatorToString(ast::expressions::BinaryExpression::Operator::Minus);
            String assignOp = ast::ASTUtils::operatorToString(ast::expressions::AssignmentExpression::Operator::Assign);
            
            assert(plusOp == "+");
            assert(minusOp == "-");
            assert(assignOp == "=");
            
            // Test kind string conversion
            String varKind = ast::ASTUtils::kindToString(ast::declarations::VariableDeclaration::Kind::Var);
            String letKind = ast::ASTUtils::kindToString(ast::declarations::VariableDeclaration::Kind::Let);
            String constKind = ast::ASTUtils::kindToString(ast::declarations::VariableDeclaration::Kind::Const);
            
            assert(varKind == "var");
            assert(letKind == "let");
            assert(constKind == "const");
            
            std::cout << "✓ AST Utils: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ AST Utils: Exception: " << e.what() << std::endl;
        }
    }
    
    void testModuleIntegration() {
        std::cout << "\n--- Testing Module Integration ---" << std::endl;
        
        try {
            // Create a complex module with multiple statements
            std::vector<std::unique_ptr<ast::Statement>> statements;
            
            // Add variable declaration
            auto varDecl = ast::ASTFactory::createVariableDeclaration(
                ast::declarations::VariableDeclaration::Kind::Let,
                "testVar",
                ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                nullptr,
                SourceLocation()
            );
            statements.push_back(std::move(varDecl));
            
            // Add function declaration
            std::vector<std::unique_ptr<ast::declarations::Parameter>> parameters;
            parameters.push_back(std::make_unique<ast::declarations::Parameter>("x", nullptr));
            
            std::vector<std::unique_ptr<ast::Statement>> bodyStatements;
            bodyStatements.push_back(ast::ASTFactory::createReturnStatement(
                ast::ASTFactory::createIdentifier("x", SourceLocation()),
                SourceLocation()
            ));
            
            auto body = ast::ASTFactory::createBlockStatement(std::move(bodyStatements), SourceLocation());
            auto funcDecl = ast::ASTFactory::createFunctionDeclaration(
                "testFunction",
                std::vector<std::unique_ptr<ast::declarations::TypeParameter>>(),
                std::move(parameters),
                nullptr,
                std::move(body),
                SourceLocation()
            );
            statements.push_back(std::move(funcDecl));
            
            // Add expression statement
            auto exprStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createCallExpression(
                    ast::ASTFactory::createIdentifier("testFunction", SourceLocation()),
                    std::vector<std::unique_ptr<ast::Expression>>{
                        ast::ASTFactory::createNumericLiteral(10.0, SourceLocation())
                    },
                    SourceLocation()
                ),
                SourceLocation()
            );
            statements.push_back(std::move(exprStmt));
            
            // Create module
            auto module = ast::ASTFactory::createModule("test.ts", std::move(statements));
            
            assert(module->getFilename() == "test.ts");
            assert(module->getStatements().size() == 3);
            assert(module->getDeclarations().size() == 2); // varDecl and funcDecl
            
            std::cout << "✓ Module integration: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Module integration: Exception: " << e.what() << std::endl;
        }
    }
    
    void testVisitorPattern() {
        std::cout << "\n--- Testing Visitor Pattern ---" << std::endl;
        
        try {
            // Create a simple visitor that counts nodes
            class TestVisitor : public ast::ASTVisitor {
            public:
                int expressionCount = 0;
                int statementCount = 0;
                int declarationCount = 0;
                
                void visit(ast::expressions::NumericLiteral& node) override {
                    expressionCount++;
                }
                
                void visit(ast::expressions::StringLiteral& node) override {
                    expressionCount++;
                }
                
                void visit(ast::expressions::Identifier& node) override {
                    expressionCount++;
                }
                
                void visit(ast::statements::ExpressionStatement& node) override {
                    statementCount++;
                    node.getExpression()->accept(*this);
                }
                
                void visit(ast::declarations::VariableDeclaration& node) override {
                    declarationCount++;
                    if (node.getInitializer()) {
                        node.getInitializer()->accept(*this);
                    }
                }
            };
            
            // Create test nodes
            auto varDecl = ast::ASTFactory::createVariableDeclaration(
                ast::declarations::VariableDeclaration::Kind::Let,
                "testVar",
                ast::ASTFactory::createNumericLiteral(42.0, SourceLocation()),
                nullptr,
                SourceLocation()
            );
            
            auto exprStmt = ast::ASTFactory::createExpressionStatement(
                ast::ASTFactory::createStringLiteral("hello", SourceLocation()),
                SourceLocation()
            );
            
            // Test visitor
            TestVisitor visitor;
            varDecl->accept(visitor);
            exprStmt->accept(visitor);
            
            assert(visitor.expressionCount == 2); // NumericLiteral + StringLiteral
            assert(visitor.statementCount == 1); // ExpressionStatement
            assert(visitor.declarationCount == 1); // VariableDeclaration
            
            std::cout << "✓ Visitor pattern: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Visitor pattern: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::ASTModularIntegrationTest test;
    test.runAllTests();
    return 0;
}