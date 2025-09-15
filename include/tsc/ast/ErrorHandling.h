#pragma once

#include "tsc/ast/AST.h"
#include "tsc/ast/Expression.h"
#include "tsc/ast/Statement.h"
#include <vector>
#include <string>

namespace tsc {
namespace ast {

// =============================================================================
// ERROR HANDLING AST NODES
// =============================================================================

// =============================================================================
// TRY STATEMENT
// =============================================================================

class TryStatement : public Statement {
private:
    ASTNode* tryBlock_;
    std::vector<CatchClause*> catchClauses_;
    ASTNode* finallyBlock_;
    
public:
    TryStatement(ASTNode* tryBlock,
                 const std::vector<CatchClause*>& catchClauses,
                 ASTNode* finallyBlock = nullptr)
        : Statement(ASTNodeType::TryStatement),
          tryBlock_(tryBlock), catchClauses_(catchClauses), finallyBlock_(finallyBlock) {}
    
    ~TryStatement() override {
        delete tryBlock_;
        for (auto& catchClause : catchClauses_) {
            delete catchClause;
        }
        delete finallyBlock_;
    }
    
    // Getters
    ASTNode* getTryBlock() const { return tryBlock_; }
    const std::vector<CatchClause*>& getCatchClauses() const { return catchClauses_; }
    ASTNode* getFinallyBlock() const { return finallyBlock_; }
    
    // Setters
    void setTryBlock(ASTNode* tryBlock) { tryBlock_ = tryBlock; }
    void setCatchClauses(const std::vector<CatchClause*>& catchClauses) { catchClauses_ = catchClauses; }
    void setFinallyBlock(ASTNode* finallyBlock) { finallyBlock_ = finallyBlock; }
    
    // Helper methods
    bool hasCatchClauses() const { return !catchClauses_.empty(); }
    bool hasFinallyBlock() const { return finallyBlock_ != nullptr; }
    void addCatchClause(CatchClause* catchClause) { catchClauses_.push_back(catchClause); }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (tryBlock_) children.push_back(tryBlock_);
        for (auto& catchClause : catchClauses_) {
            children.push_back(catchClause);
        }
        if (finallyBlock_) children.push_back(finallyBlock_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitTryStatement(this);
    }
    
    std::string toString() const override {
        std::string result = "try ";
        if (tryBlock_) {
            result += tryBlock_->toString();
        }
        
        for (auto& catchClause : catchClauses_) {
            result += " " + catchClause->toString();
        }
        
        if (finallyBlock_) {
            result += " finally " + finallyBlock_->toString();
        }
        
        return result;
    }
};

// =============================================================================
// CATCH CLAUSE
// =============================================================================

class CatchClause : public ASTNode {
private:
    std::string parameterName_;
    Type* parameterType_;
    ASTNode* body_;
    
public:
    CatchClause(const std::string& parameterName,
                Type* parameterType = nullptr,
                ASTNode* body = nullptr)
        : ASTNode(ASTNodeType::CatchClause),
          parameterName_(parameterName), parameterType_(parameterType), body_(body) {}
    
    ~CatchClause() override {
        delete parameterType_;
        delete body_;
    }
    
    // Getters
    const std::string& getParameterName() const { return parameterName_; }
    Type* getParameterType() const { return parameterType_; }
    ASTNode* getBody() const { return body_; }
    
    // Setters
    void setParameterName(const std::string& parameterName) { parameterName_ = parameterName; }
    void setParameterType(Type* parameterType) { parameterType_ = parameterType; }
    void setBody(ASTNode* body) { body_ = body; }
    
    // Helper methods
    bool hasParameter() const { return !parameterName_.empty(); }
    bool hasParameterType() const { return parameterType_ != nullptr; }
    bool hasBody() const { return body_ != nullptr; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (parameterType_) children.push_back(parameterType_);
        if (body_) children.push_back(body_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitCatchClause(this);
    }
    
    std::string toString() const override {
        std::string result = "catch";
        
        if (hasParameter()) {
            result += " (" + parameterName_;
            if (parameterType_) {
                result += ": " + parameterType_->getName();
            }
            result += ")";
        }
        
        if (body_) {
            result += " " + body_->toString();
        }
        
        return result;
    }
};

// =============================================================================
// THROW STATEMENT
// =============================================================================

class ThrowStatement : public Statement {
private:
    ASTNode* expression_;
    
public:
    ThrowStatement(ASTNode* expression = nullptr)
        : Statement(ASTNodeType::ThrowStatement), expression_(expression) {}
    
    ~ThrowStatement() override {
        delete expression_;
    }
    
    // Getters
    ASTNode* getExpression() const { return expression_; }
    
    // Setters
    void setExpression(ASTNode* expression) { expression_ = expression; }
    
    // Helper methods
    bool hasExpression() const { return expression_ != nullptr; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (expression_) children.push_back(expression_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitThrowStatement(this);
    }
    
    std::string toString() const override {
        std::string result = "throw";
        if (expression_) {
            result += " " + expression_->toString();
        }
        return result;
    }
};

// =============================================================================
// CUSTOM EXCEPTION CLASS
// =============================================================================

class CustomExceptionClass : public ClassDeclaration {
private:
    std::vector<ASTNode*> baseExceptions_;
    bool isAbstract_;
    
public:
    CustomExceptionClass(const std::string& name,
                        const std::vector<ASTNode*>& baseExceptions = {},
                        const std::vector<ASTNode*>& members = {},
                        bool isAbstract = false)
        : ClassDeclaration(name, members),
          baseExceptions_(baseExceptions), isAbstract_(isAbstract) {}
    
    ~CustomExceptionClass() override {
        for (auto& baseException : baseExceptions_) {
            delete baseException;
        }
    }
    
    // Getters
    const std::vector<ASTNode*>& getBaseExceptions() const { return baseExceptions_; }
    bool isAbstract() const { return isAbstract_; }
    
    // Setters
    void setBaseExceptions(const std::vector<ASTNode*>& baseExceptions) { baseExceptions_ = baseExceptions; }
    void setAbstract(bool isAbstract) { isAbstract_ = isAbstract; }
    
    // Helper methods
    bool hasBaseExceptions() const { return !baseExceptions_.empty(); }
    void addBaseException(ASTNode* baseException) { baseExceptions_.push_back(baseException); }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children = baseExceptions_;
        auto classChildren = ClassDeclaration::getChildren();
        children.insert(children.end(), classChildren.begin(), classChildren.end());
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitCustomExceptionClass(this);
    }
    
    std::string toString() const override {
        std::string result = "";
        if (isAbstract_) result += "abstract ";
        result += "class " + getName();
        
        if (hasBaseExceptions()) {
            result += " extends ";
            for (size_t i = 0; i < baseExceptions_.size(); ++i) {
                if (i > 0) result += ", ";
                if (baseExceptions_[i]) {
                    result += baseExceptions_[i]->toString();
                }
            }
        }
        
        result += " { /* exception class */ }";
        return result;
    }
};

// =============================================================================
// PANIC STATEMENT
// =============================================================================

class PanicStatement : public Statement {
private:
    ASTNode* expression_;
    std::string message_;
    
public:
    PanicStatement(ASTNode* expression = nullptr, const std::string& message = "")
        : Statement(ASTNodeType::PanicStatement), expression_(expression), message_(message) {}
    
    ~PanicStatement() override {
        delete expression_;
    }
    
    // Getters
    ASTNode* getExpression() const { return expression_; }
    const std::string& getMessage() const { return message_; }
    
    // Setters
    void setExpression(ASTNode* expression) { expression_ = expression; }
    void setMessage(const std::string& message) { message_ = message; }
    
    // Helper methods
    bool hasExpression() const { return expression_ != nullptr; }
    bool hasMessage() const { return !message_.empty(); }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (expression_) children.push_back(expression_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitPanicStatement(this);
    }
    
    std::string toString() const override {
        std::string result = "panic";
        if (hasMessage()) {
            result += " \"" + message_ + "\"";
        } else if (expression_) {
            result += " " + expression_->toString();
        }
        return result;
    }
};

// =============================================================================
// ABORT STATEMENT
// =============================================================================

class AbortStatement : public Statement {
private:
    ASTNode* expression_;
    std::string reason_;
    
public:
    AbortStatement(ASTNode* expression = nullptr, const std::string& reason = "")
        : Statement(ASTNodeType::AbortStatement), expression_(expression), reason_(reason) {}
    
    ~AbortStatement() override {
        delete expression_;
    }
    
    // Getters
    ASTNode* getExpression() const { return expression_; }
    const std::string& getReason() const { return reason_; }
    
    // Setters
    void setExpression(ASTNode* expression) { expression_ = expression; }
    void setReason(const std::string& reason) { reason_ = reason; }
    
    // Helper methods
    bool hasExpression() const { return expression_ != nullptr; }
    bool hasReason() const { return !reason_.empty(); }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (expression_) children.push_back(expression_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitAbortStatement(this);
    }
    
    std::string toString() const override {
        std::string result = "abort";
        if (hasReason()) {
            result += " \"" + reason_ + "\"";
        } else if (expression_) {
            result += " " + expression_->toString();
        }
        return result;
    }
};

// =============================================================================
// ERROR PROPAGATION EXPRESSION
// =============================================================================

class ErrorPropagationExpression : public Expression {
private:
    ASTNode* expression_;
    bool isOptional_;
    
public:
    ErrorPropagationExpression(ASTNode* expression, bool isOptional = false)
        : Expression(ASTNodeType::ErrorPropagationExpression),
          expression_(expression), isOptional_(isOptional) {}
    
    ~ErrorPropagationExpression() override {
        delete expression_;
    }
    
    // Getters
    ASTNode* getExpression() const { return expression_; }
    bool isOptional() const { return isOptional_; }
    
    // Setters
    void setExpression(ASTNode* expression) { expression_ = expression; }
    void setOptional(bool isOptional) { isOptional_ = isOptional; }
    
    // Helper methods
    bool hasExpression() const { return expression_ != nullptr; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (expression_) children.push_back(expression_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitErrorPropagationExpression(this);
    }
    
    std::string toString() const override {
        std::string result = "";
        if (expression_) {
            result += expression_->toString();
        }
        
        if (isOptional_) {
            result += "?";
        } else {
            result += "!";
        }
        
        return result;
    }
};

// =============================================================================
// RESULT TYPE
// =============================================================================

class ResultType : public Type {
private:
    Type* successType_;
    Type* errorType_;
    
public:
    ResultType(Type* successType, Type* errorType)
        : Type(TypeType::ResultType), successType_(successType), errorType_(errorType) {}
    
    ~ResultType() override {
        delete successType_;
        delete errorType_;
    }
    
    // Getters
    Type* getSuccessType() const { return successType_; }
    Type* getErrorType() const { return errorType_; }
    
    // Setters
    void setSuccessType(Type* successType) { successType_ = successType; }
    void setErrorType(Type* errorType) { errorType_ = errorType; }
    
    // Helper methods
    bool hasSuccessType() const { return successType_ != nullptr; }
    bool hasErrorType() const { return errorType_ != nullptr; }
    
    std::string getName() const override {
        std::string result = "Result<";
        if (successType_) {
            result += successType_->getName();
        } else {
            result += "void";
        }
        result += ", ";
        if (errorType_) {
            result += errorType_->getName();
        } else {
            result += "Error";
        }
        result += ">";
        return result;
    }
};

// =============================================================================
// OPTIONAL TYPE
// =============================================================================

class OptionalType : public Type {
private:
    Type* innerType_;
    
public:
    OptionalType(Type* innerType)
        : Type(TypeType::OptionalType), innerType_(innerType) {}
    
    ~OptionalType() override {
        delete innerType_;
    }
    
    // Getters
    Type* getInnerType() const { return innerType_; }
    
    // Setters
    void setInnerType(Type* innerType) { innerType_ = innerType; }
    
    // Helper methods
    bool hasInnerType() const { return innerType_ != nullptr; }
    
    std::string getName() const override {
        std::string result = "Optional<";
        if (innerType_) {
            result += innerType_->getName();
        } else {
            result += "unknown";
        }
        result += ">";
        return result;
    }
};

} // namespace ast
} // namespace tsc