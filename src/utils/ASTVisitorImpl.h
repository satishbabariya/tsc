#pragma once

#include "tsc/AST.h"
#include <iostream>
#include <vector>

namespace tsc {

// Concrete implementation of ASTVisitor for testing and utility purposes
class ASTVisitorImpl : public ASTVisitor {
public:
    ASTVisitorImpl() = default;
    virtual ~ASTVisitorImpl() = default;
    
    // Track visited nodes for testing
    std::vector<String> visitedNodes;
    
    // Expressions
    void visit(NumericLiteral& node) override {
        visitedNodes.push_back("NumericLiteral: " + std::to_string(node.getValue()));
    }
    
    void visit(StringLiteral& node) override {
        visitedNodes.push_back("StringLiteral: " + node.getValue());
    }
    
    void visit(TemplateLiteral& node) override {
        visitedNodes.push_back("TemplateLiteral");
    }
    
    void visit(BooleanLiteral& node) override {
        visitedNodes.push_back(std::string("BooleanLiteral: ") + (node.getValue() ? "true" : "false"));
    }
    
    void visit(NullLiteral& node) override {
        visitedNodes.push_back("NullLiteral");
    }
    
    void visit(Identifier& node) override {
        visitedNodes.push_back(std::string("Identifier: ") + node.getName());
    }
    
    void visit(ThisExpression& node) override {
        visitedNodes.push_back("ThisExpression");
    }
    
    void visit(SuperExpression& node) override {
        visitedNodes.push_back("SuperExpression");
    }
    
    void visit(NewExpression& node) override {
        visitedNodes.push_back("NewExpression");
    }
    
    void visit(BinaryExpression& node) override {
        visitedNodes.push_back(std::string("BinaryExpression: ") + node.toString());
    }
    
    void visit(UnaryExpression& node) override {
        visitedNodes.push_back(std::string("UnaryExpression: ") + node.toString());
    }
    
    void visit(AssignmentExpression& node) override {
        visitedNodes.push_back(std::string("AssignmentExpression: ") + node.toString());
    }
    
    void visit(ConditionalExpression& node) override {
        visitedNodes.push_back("ConditionalExpression");
    }
    
    void visit(CallExpression& node) override {
        visitedNodes.push_back("CallExpression");
    }
    
    void visit(ArrayLiteral& node) override {
        visitedNodes.push_back("ArrayLiteral");
    }
    
    void visit(IndexExpression& node) override {
        visitedNodes.push_back("IndexExpression");
    }
    
    void visit(ObjectLiteral& node) override {
        visitedNodes.push_back("ObjectLiteral");
    }
    
    void visit(PropertyAccess& node) override {
        visitedNodes.push_back(std::string("PropertyAccess: ") + node.toString());
    }
    
    void visit(ArrowFunction& node) override {
        visitedNodes.push_back("ArrowFunction");
    }
    
    void visit(FunctionExpression& node) override {
        visitedNodes.push_back("FunctionExpression");
    }
    
    // Statements
    void visit(ExpressionStatement& node) override {
        visitedNodes.push_back("ExpressionStatement");
    }
    
    void visit(BlockStatement& node) override {
        visitedNodes.push_back("BlockStatement");
    }
    
    void visit(ReturnStatement& node) override {
        visitedNodes.push_back("ReturnStatement");
    }
    
    void visit(IfStatement& node) override {
        visitedNodes.push_back("IfStatement");
    }
    
    void visit(WhileStatement& node) override {
        visitedNodes.push_back("WhileStatement");
    }
    
    void visit(DoWhileStatement& node) override {
        visitedNodes.push_back("DoWhileStatement");
    }
    
    void visit(ForStatement& node) override {
        visitedNodes.push_back("ForStatement");
    }
    
    void visit(ForOfStatement& node) override {
        visitedNodes.push_back("ForOfStatement");
    }
    
    void visit(SwitchStatement& node) override {
        visitedNodes.push_back("SwitchStatement");
    }
    
    void visit(CaseClause& node) override {
        visitedNodes.push_back("CaseClause");
    }
    
    void visit(BreakStatement& node) override {
        visitedNodes.push_back("BreakStatement");
    }
    
    void visit(ContinueStatement& node) override {
        visitedNodes.push_back("ContinueStatement");
    }
    
    void visit(TryStatement& node) override {
        visitedNodes.push_back("TryStatement");
    }
    
    void visit(CatchClause& node) override {
        visitedNodes.push_back("CatchClause");
    }
    
    void visit(ThrowStatement& node) override {
        visitedNodes.push_back("ThrowStatement");
    }
    
    void visit(VariableDeclaration& node) override {
        visitedNodes.push_back(std::string("VariableDeclaration: ") + node.getName());
    }
    
    void visit(FunctionDeclaration& node) override {
        visitedNodes.push_back(std::string("FunctionDeclaration: ") + node.getName());
    }
    
    void visit(TypeParameter& node) override {
        visitedNodes.push_back(std::string("TypeParameter: ") + node.getName());
    }
    
    // Class-related declarations
    void visit(PropertyDeclaration& node) override {
        visitedNodes.push_back(std::string("PropertyDeclaration: ") + node.getName());
    }
    
    void visit(MethodDeclaration& node) override {
        visitedNodes.push_back(std::string("MethodDeclaration: ") + node.getName());
    }
    
    void visit(ClassDeclaration& node) override {
        visitedNodes.push_back(std::string("ClassDeclaration: ") + node.getName());
    }
    
    void visit(InterfaceDeclaration& node) override {
        visitedNodes.push_back(std::string("InterfaceDeclaration: ") + node.getName());
    }
    
    void visit(EnumMember& node) override {
        visitedNodes.push_back(std::string("EnumMember: ") + node.getName());
    }
    
    void visit(EnumDeclaration& node) override {
        visitedNodes.push_back(std::string("EnumDeclaration: ") + node.getName());
    }
    
    void visit(TypeAliasDeclaration& node) override {
        visitedNodes.push_back(std::string("TypeAliasDeclaration: ") + node.getName());
    }
    
    // Module
    void visit(Module& node) override {
        visitedNodes.push_back(std::string("Module: ") + node.getFilename());
    }
    
    // Import/Export declarations
    void visit(ImportDeclaration& node) override {
        visitedNodes.push_back(std::string("ImportDeclaration: ") + node.getModuleSpecifier());
    }
    
    void visit(ExportDeclaration& node) override {
        visitedNodes.push_back(std::string("ExportDeclaration: ") + node.getModuleSpecifier());
    }
    
    // Utility methods
    void clear() {
        visitedNodes.clear();
    }
    
    void printVisitedNodes() const {
        std::cout << "Visited nodes:" << std::endl;
        for (const auto& node : visitedNodes) {
            std::cout << "  " << node << std::endl;
        }
    }
    
    size_t getVisitedCount() const {
        return visitedNodes.size();
    }
    
    bool wasVisited(const String& nodeType) const {
        for (const auto& node : visitedNodes) {
            if (node.find(nodeType) == 0) {
                return true;
            }
        }
        return false;
    }
};

} // namespace tsc