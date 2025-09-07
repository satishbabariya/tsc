#pragma once

#include "tsc/AST.h"
#include <iostream>

namespace tsc {

// Simple AST printer for debugging and verification
class ASTPrinter : public ASTVisitor {
public:
    explicit ASTPrinter(std::ostream& output = std::cout, int indent = 0);
    
    void print(const ASTNode& node);
    
    // Expressions
    void visit(NumericLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(BooleanLiteral& node) override;
    void visit(NullLiteral& node) override;
    void visit(Identifier& node) override;
    void visit(BinaryExpression& node) override;
    void visit(UnaryExpression& node) override;
    void visit(AssignmentExpression& node) override;
    void visit(CallExpression& node) override;
    void visit(ArrayLiteral& node) override;
    void visit(IndexExpression& node) override;
    
    // Statements
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
    void visit(VariableDeclaration& node) override;
    void visit(FunctionDeclaration& node) override;
    
    // Module
    void visit(Module& node) override;

private:
    std::ostream& output_;
    int indent_;
    
    void printIndent();
    void increaseIndent();
    void decreaseIndent();
    
    String getOperatorString(BinaryExpression::Operator op);
    String getOperatorString(UnaryExpression::Operator op);
    String getOperatorString(AssignmentExpression::Operator op);
};

} // namespace tsc