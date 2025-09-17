#include "tsc/ast/statements/StatementNodes.h"
#include "tsc/ast/core/ASTCore.h"

namespace tsc {
namespace ast {
namespace statements {

// ExpressionStatement implementation
void ExpressionStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ExpressionStatement::toString() const {
    return expression_->toString() + ";";
}

// BlockStatement implementation
void BlockStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BlockStatement::toString() const {
    String result = "{\n";
    for (const auto& stmt : statements_) {
        result += "  " + stmt->toString() + "\n";
    }
    result += "}";
    return result;
}

// ReturnStatement implementation
void ReturnStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ReturnStatement::toString() const {
    if (expression_) {
        return "return " + expression_->toString() + ";";
    } else {
        return "return;";
    }
}

// IfStatement implementation
void IfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String IfStatement::toString() const {
    String result = "if (" + condition_->toString() + ") " + thenStmt_->toString();
    if (elseStmt_) {
        result += " else " + elseStmt_->toString();
    }
    return result;
}

// WhileStatement implementation
void WhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String WhileStatement::toString() const {
    return "while (" + condition_->toString() + ") " + body_->toString();
}

// DoWhileStatement implementation
void DoWhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String DoWhileStatement::toString() const {
    return "do " + body_->toString() + " while (" + condition_->toString() + ");";
}

// ForStatement implementation
void ForStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ForStatement::toString() const {
    String result = "for (";
    if (initializer_) {
        result += initializer_->toString();
    }
    result += "; ";
    if (condition_) {
        result += condition_->toString();
    }
    result += "; ";
    if (increment_) {
        result += increment_->toString();
    }
    result += ") " + body_->toString();
    return result;
}

// ForOfStatement implementation
void ForOfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ForOfStatement::toString() const {
    return "for (" + variable_->toString() + " of " + iterable_->toString() + ") " + body_->toString();
}

// CaseClause implementation
void CaseClause::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CaseClause::toString() const {
    String result = "case " + expression_->toString() + ":\n";
    for (const auto& stmt : statements_) {
        result += "  " + stmt->toString() + "\n";
    }
    return result;
}

// SwitchStatement implementation
void SwitchStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String SwitchStatement::toString() const {
    String result = "switch (" + expression_->toString() + ") {\n";
    for (const auto& case_ : cases_) {
        result += "  " + case_->toString() + "\n";
    }
    if (defaultCase_) {
        result += "  default:\n";
        for (const auto& stmt : defaultCase_->getStatements()) {
            result += "    " + stmt->toString() + "\n";
        }
    }
    result += "}";
    return result;
}

// BreakStatement implementation
void BreakStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BreakStatement::toString() const {
    return "break;";
}

// ContinueStatement implementation
void ContinueStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ContinueStatement::toString() const {
    return "continue;";
}

// CatchClause implementation
void CatchClause::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CatchClause::toString() const {
    return "catch (" + parameter_ + ") " + body_->toString();
}

// TryStatement implementation
void TryStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TryStatement::toString() const {
    String result = "try " + tryBlock_->toString();
    if (catchClause_) {
        result += " " + catchClause_->toString();
    }
    if (finallyBlock_) {
        result += " finally " + finallyBlock_->toString();
    }
    return result;
}

// ThrowStatement implementation
void ThrowStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ThrowStatement::toString() const {
    return "throw " + expression_->toString() + ";";
}

} // namespace statements
} // namespace ast
} // namespace tsc