#pragma once

#include "tsc/ast/core/ASTCore.h"
#include "tsc/ast/expressions/ExpressionNodes.h"

namespace tsc {
namespace ast {
namespace statements {

// Expression statement for standalone expressions
class ExpressionStatement : public Statement {
public:
    ExpressionStatement(unique_ptr<Expression> expression, const SourceLocation& loc)
        : expression_(std::move(expression)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Expression; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }

private:
    unique_ptr<Expression> expression_;
    SourceLocation location_;
};

class BlockStatement : public Statement {
public:
    BlockStatement(std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc)
        : statements_(std::move(statements)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Block; }
    String toString() const override;
    
    const std::vector<unique_ptr<Statement>>& getStatements() const { return statements_; }

private:
    std::vector<unique_ptr<Statement>> statements_;
    SourceLocation location_;
};

class ReturnStatement : public Statement {
public:
    ReturnStatement(unique_ptr<Expression> expression, const SourceLocation& loc)
        : expression_(std::move(expression)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Return; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }

private:
    unique_ptr<Expression> expression_;
    SourceLocation location_;
};

class IfStatement : public Statement {
public:
    IfStatement(unique_ptr<Expression> condition, unique_ptr<Statement> thenStmt, unique_ptr<Statement> elseStmt, const SourceLocation& loc)
        : condition_(std::move(condition)), thenStmt_(std::move(thenStmt)), elseStmt_(std::move(elseStmt)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::If; }
    String toString() const override;
    
    Expression* getCondition() const { return condition_.get(); }
    Statement* getThenStatement() const { return thenStmt_.get(); }
    Statement* getElseStatement() const { return elseStmt_.get(); }

private:
    unique_ptr<Expression> condition_;
    unique_ptr<Statement> thenStmt_;
    unique_ptr<Statement> elseStmt_;
    SourceLocation location_;
};

class WhileStatement : public Statement {
public:
    WhileStatement(unique_ptr<Expression> condition, unique_ptr<Statement> body, const SourceLocation& loc)
        : condition_(std::move(condition)), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::While; }
    String toString() const override;
    
    Expression* getCondition() const { return condition_.get(); }
    Statement* getBody() const { return body_.get(); }

private:
    unique_ptr<Expression> condition_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

class DoWhileStatement : public Statement {
public:
    DoWhileStatement(unique_ptr<Statement> body, unique_ptr<Expression> condition, const SourceLocation& loc)
        : body_(std::move(body)), condition_(std::move(condition)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::DoWhile; }
    String toString() const override;
    
    Statement* getBody() const { return body_.get(); }
    Expression* getCondition() const { return condition_.get(); }

private:
    unique_ptr<Statement> body_;
    unique_ptr<Expression> condition_;
    SourceLocation location_;
};

class ForStatement : public Statement {
public:
    ForStatement(unique_ptr<Statement> initializer, unique_ptr<Expression> condition, unique_ptr<Expression> increment, unique_ptr<Statement> body, const SourceLocation& loc)
        : initializer_(std::move(initializer)), condition_(std::move(condition)), increment_(std::move(increment)), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::For; }
    String toString() const override;
    
    Statement* getInitializer() const { return initializer_.get(); }
    Expression* getCondition() const { return condition_.get(); }
    Expression* getIncrement() const { return increment_.get(); }
    Statement* getBody() const { return body_.get(); }

private:
    unique_ptr<Statement> initializer_;
    unique_ptr<Expression> condition_;
    unique_ptr<Expression> increment_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

class ForOfStatement : public Statement {
public:
    ForOfStatement(unique_ptr<Expression> variable, unique_ptr<Expression> iterable, unique_ptr<Statement> body, const SourceLocation& loc)
        : variable_(std::move(variable)), iterable_(std::move(iterable)), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::ForOf; }
    String toString() const override;
    
    Expression* getVariable() const { return variable_.get(); }
    Expression* getIterable() const { return iterable_.get(); }
    Statement* getBody() const { return body_.get(); }

private:
    unique_ptr<Expression> variable_;
    unique_ptr<Expression> iterable_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

class CaseClause : public Statement {
public:
    CaseClause(unique_ptr<Expression> expression, std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc)
        : expression_(std::move(expression)), statements_(std::move(statements)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Expression; } // Case clauses are not top-level statements
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }
    const std::vector<unique_ptr<Statement>>& getStatements() const { return statements_; }

private:
    unique_ptr<Expression> expression_;
    std::vector<unique_ptr<Statement>> statements_;
    SourceLocation location_;
};

class SwitchStatement : public Statement {
public:
    SwitchStatement(unique_ptr<Expression> expression, std::vector<unique_ptr<CaseClause>> cases, unique_ptr<CaseClause> defaultCase, const SourceLocation& loc)
        : expression_(std::move(expression)), cases_(std::move(cases)), defaultCase_(std::move(defaultCase)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Switch; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }
    const std::vector<unique_ptr<CaseClause>>& getCases() const { return cases_; }
    CaseClause* getDefaultCase() const { return defaultCase_.get(); }

private:
    unique_ptr<Expression> expression_;
    std::vector<unique_ptr<CaseClause>> cases_;
    unique_ptr<CaseClause> defaultCase_;
    SourceLocation location_;
};

class BreakStatement : public Statement {
public:
    BreakStatement(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Break; }
    String toString() const override;

private:
    SourceLocation location_;
};

class ContinueStatement : public Statement {
public:
    ContinueStatement(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Continue; }
    String toString() const override;

private:
    SourceLocation location_;
};

class CatchClause : public Statement {
public:
    CatchClause(const String& parameter, unique_ptr<Statement> body, const SourceLocation& loc)
        : parameter_(parameter), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Expression; } // Catch clauses are not top-level statements
    String toString() const override;
    
    const String& getParameter() const { return parameter_; }
    Statement* getBody() const { return body_.get(); }

private:
    String parameter_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

class TryStatement : public Statement {
public:
    TryStatement(unique_ptr<Statement> tryBlock, unique_ptr<CatchClause> catchClause, unique_ptr<Statement> finallyBlock, const SourceLocation& loc)
        : tryBlock_(std::move(tryBlock)), catchClause_(std::move(catchClause)), finallyBlock_(std::move(finallyBlock)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Try; }
    String toString() const override;
    
    Statement* getTryBlock() const { return tryBlock_.get(); }
    CatchClause* getCatchClause() const { return catchClause_.get(); }
    Statement* getFinallyBlock() const { return finallyBlock_.get(); }

private:
    unique_ptr<Statement> tryBlock_;
    unique_ptr<CatchClause> catchClause_;
    unique_ptr<Statement> finallyBlock_;
    SourceLocation location_;
};

class ThrowStatement : public Statement {
public:
    ThrowStatement(unique_ptr<Expression> expression, const SourceLocation& loc)
        : expression_(std::move(expression)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Throw; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }

private:
    unique_ptr<Expression> expression_;
    SourceLocation location_;
};

} // namespace statements
} // namespace ast
} // namespace tsc