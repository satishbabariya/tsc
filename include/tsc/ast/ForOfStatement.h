#pragma once

#include "tsc/ast/Statement.h"
#include "tsc/ast/Expression.h"
#include "tsc/ast/VariableDeclaration.h"

namespace tsc {
namespace ast {

// =============================================================================
// FOR...OF STATEMENT
// =============================================================================

class ForOfStatement : public Statement {
private:
    VariableDeclaration* variable_;
    Expression* iterable_;
    Statement* body_;
    bool isAsync_;
    
public:
    ForOfStatement(VariableDeclaration* variable, Expression* iterable, Statement* body, bool isAsync = false)
        : Statement(ASTNodeType::ForOfStatement), variable_(variable), iterable_(iterable), body_(body), isAsync_(isAsync) {}
    
    ~ForOfStatement() override {
        delete variable_;
        delete iterable_;
        delete body_;
    }
    
    // Getters
    VariableDeclaration* getVariable() const { return variable_; }
    Expression* getIterable() const { return iterable_; }
    Statement* getBody() const { return body_; }
    bool isAsync() const { return isAsync_; }
    
    // Setters
    void setVariable(VariableDeclaration* variable) { variable_ = variable; }
    void setIterable(Expression* iterable) { iterable_ = iterable; }
    void setBody(Statement* body) { body_ = body; }
    void setAsync(bool isAsync) { isAsync_ = isAsync; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (variable_) children.push_back(variable_);
        if (iterable_) children.push_back(iterable_);
        if (body_) children.push_back(body_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitForOfStatement(this);
    }
    
    std::string toString() const override {
        std::string result = isAsync_ ? "for await (" : "for (";
        if (variable_) result += variable_->toString();
        result += " of ";
        if (iterable_) result += iterable_->toString();
        result += ")";
        if (body_) result += " " + body_->toString();
        return result;
    }
};

// =============================================================================
// FOR...IN STATEMENT (for comparison and completeness)
// =============================================================================

class ForInStatement : public Statement {
private:
    VariableDeclaration* variable_;
    Expression* object_;
    Statement* body_;
    
public:
    ForInStatement(VariableDeclaration* variable, Expression* object, Statement* body)
        : Statement(ASTNodeType::ForInStatement), variable_(variable), object_(object), body_(body) {}
    
    ~ForInStatement() override {
        delete variable_;
        delete object_;
        delete body_;
    }
    
    // Getters
    VariableDeclaration* getVariable() const { return variable_; }
    Expression* getObject() const { return object_; }
    Statement* getBody() const { return body_; }
    
    // Setters
    void setVariable(VariableDeclaration* variable) { variable_ = variable; }
    void setObject(Expression* object) { object_ = object; }
    void setBody(Statement* body) { body_ = body; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (variable_) children.push_back(variable_);
        if (object_) children.push_back(object_);
        if (body_) children.push_back(body_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitForInStatement(this);
    }
    
    std::string toString() const override {
        std::string result = "for (";
        if (variable_) result += variable_->toString();
        result += " in ";
        if (object_) result += object_->toString();
        result += ")";
        if (body_) result += " " + body_->toString();
        return result;
    }
};

} // namespace ast
} // namespace tsc