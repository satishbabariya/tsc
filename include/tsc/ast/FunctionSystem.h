#pragma once

#include "tsc/ast/AST.h"
#include "tsc/ast/Expression.h"
#include "tsc/ast/Statement.h"
#include <vector>
#include <string>

namespace tsc {
namespace ast {

// =============================================================================
// FUNCTION SYSTEM AST NODES
// =============================================================================

// =============================================================================
// FUNCTION OVERLOAD DECLARATION
// =============================================================================

class FunctionOverloadDeclaration : public ASTNode {
private:
    std::string name_;
    std::vector<ASTNode*> parameters_;
    Type* returnType_;
    bool isAsync_;
    bool isGenerator_;
    
public:
    FunctionOverloadDeclaration(const std::string& name, 
                               const std::vector<ASTNode*>& parameters,
                               Type* returnType = nullptr,
                               bool isAsync = false,
                               bool isGenerator = false)
        : ASTNode(ASTNodeType::FunctionOverloadDeclaration), 
          name_(name), parameters_(parameters), returnType_(returnType),
          isAsync_(isAsync), isGenerator_(isGenerator) {}
    
    ~FunctionOverloadDeclaration() override {
        for (auto& param : parameters_) {
            delete param;
        }
        delete returnType_;
    }
    
    // Getters
    const std::string& getName() const { return name_; }
    const std::vector<ASTNode*>& getParameters() const { return parameters_; }
    Type* getReturnType() const { return returnType_; }
    bool isAsync() const { return isAsync_; }
    bool isGenerator() const { return isGenerator_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setParameters(const std::vector<ASTNode*>& parameters) { parameters_ = parameters; }
    void setReturnType(Type* returnType) { returnType_ = returnType; }
    void setAsync(bool isAsync) { isAsync_ = isAsync; }
    void setGenerator(bool isGenerator) { isGenerator_ = isGenerator; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children = parameters_;
        if (returnType_) children.push_back(returnType_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitFunctionOverloadDeclaration(this);
    }
    
    std::string toString() const override {
        std::string result = "";
        if (isAsync_) result += "async ";
        if (isGenerator_) result += "function* ";
        else result += "function ";
        result += name_ + "(";
        
        for (size_t i = 0; i < parameters_.size(); ++i) {
            if (i > 0) result += ", ";
            if (parameters_[i]) {
                result += parameters_[i]->toString();
            }
        }
        
        result += ")";
        if (returnType_) {
            result += ": " + returnType_->getName();
        }
        return result;
    }
};

// =============================================================================
// FUNCTION DECLARATION (Enhanced)
// =============================================================================

class FunctionDeclaration : public ASTNode {
private:
    std::string name_;
    std::vector<FunctionOverloadDeclaration*> overloads_;
    std::vector<ASTNode*> parameters_;
    Type* returnType_;
    ASTNode* body_;
    bool isAsync_;
    bool isGenerator_;
    bool isArrow_;
    
public:
    FunctionDeclaration(const std::string& name,
                       const std::vector<FunctionOverloadDeclaration*>& overloads,
                       const std::vector<ASTNode*>& parameters,
                       Type* returnType = nullptr,
                       ASTNode* body = nullptr,
                       bool isAsync = false,
                       bool isGenerator = false,
                       bool isArrow = false)
        : ASTNode(ASTNodeType::FunctionDeclaration),
          name_(name), overloads_(overloads), parameters_(parameters),
          returnType_(returnType), body_(body),
          isAsync_(isAsync), isGenerator_(isGenerator), isArrow_(isArrow) {}
    
    ~FunctionDeclaration() override {
        for (auto& overload : overloads_) {
            delete overload;
        }
        for (auto& param : parameters_) {
            delete param;
        }
        delete returnType_;
        delete body_;
    }
    
    // Getters
    const std::string& getName() const { return name_; }
    const std::vector<FunctionOverloadDeclaration*>& getOverloads() const { return overloads_; }
    const std::vector<ASTNode*>& getParameters() const { return parameters_; }
    Type* getReturnType() const { return returnType_; }
    ASTNode* getBody() const { return body_; }
    bool isAsync() const { return isAsync_; }
    bool isGenerator() const { return isGenerator_; }
    bool isArrow() const { return isArrow_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setOverloads(const std::vector<FunctionOverloadDeclaration*>& overloads) { overloads_ = overloads; }
    void setParameters(const std::vector<ASTNode*>& parameters) { parameters_ = parameters; }
    void setReturnType(Type* returnType) { returnType_ = returnType; }
    void setBody(ASTNode* body) { body_ = body; }
    void setAsync(bool isAsync) { isAsync_ = isAsync; }
    void setGenerator(bool isGenerator) { isGenerator_ = isGenerator; }
    void setArrow(bool isArrow) { isArrow_ = isArrow; }
    
    // Helper methods
    void addOverload(FunctionOverloadDeclaration* overload) { overloads_.push_back(overload); }
    bool hasOverloads() const { return !overloads_.empty(); }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        for (auto& overload : overloads_) {
            children.push_back(overload);
        }
        for (auto& param : parameters_) {
            children.push_back(param);
        }
        if (returnType_) children.push_back(returnType_);
        if (body_) children.push_back(body_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitFunctionDeclaration(this);
    }
    
    std::string toString() const override {
        std::string result = "";
        if (isAsync_) result += "async ";
        if (isGenerator_) result += "function* ";
        else if (isArrow_) result += "";
        else result += "function ";
        
        if (!isArrow_) {
            result += name_ + "(";
        } else {
            result += "(";
        }
        
        for (size_t i = 0; i < parameters_.size(); ++i) {
            if (i > 0) result += ", ";
            if (parameters_[i]) {
                result += parameters_[i]->toString();
            }
        }
        
        result += ")";
        if (isArrow_) result += " => ";
        
        if (returnType_) {
            result += ": " + returnType_->getName();
        }
        
        if (body_) {
            if (isArrow_) {
                result += body_->toString();
            } else {
                result += " " + body_->toString();
            }
        }
        
        return result;
    }
};

// =============================================================================
// PARAMETER (Enhanced)
// =============================================================================

class Parameter : public ASTNode {
private:
    std::string name_;
    Type* type_;
    ASTNode* defaultValue_;
    bool isRest_;
    bool isOptional_;
    
public:
    Parameter(const std::string& name,
              Type* type = nullptr,
              ASTNode* defaultValue = nullptr,
              bool isRest = false,
              bool isOptional = false)
        : ASTNode(ASTNodeType::Parameter),
          name_(name), type_(type), defaultValue_(defaultValue),
          isRest_(isRest), isOptional_(isOptional) {}
    
    ~Parameter() override {
        delete type_;
        delete defaultValue_;
    }
    
    // Getters
    const std::string& getName() const { return name_; }
    Type* getType() const { return type_; }
    ASTNode* getDefaultValue() const { return defaultValue_; }
    bool isRest() const { return isRest_; }
    bool isOptional() const { return isOptional_; }
    
    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setType(Type* type) { type_ = type; }
    void setDefaultValue(ASTNode* defaultValue) { defaultValue_ = defaultValue; }
    void setRest(bool isRest) { isRest_ = isRest; }
    void setOptional(bool isOptional) { isOptional_ = isOptional; }
    
    // Helper methods
    bool hasDefaultValue() const { return defaultValue_ != nullptr; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (type_) children.push_back(type_);
        if (defaultValue_) children.push_back(defaultValue_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitParameter(this);
    }
    
    std::string toString() const override {
        std::string result = "";
        
        if (isRest_) result += "...";
        result += name_;
        
        if (isOptional_) result += "?";
        
        if (type_) {
            result += ": " + type_->getName();
        }
        
        if (defaultValue_) {
            result += " = " + defaultValue_->toString();
        }
        
        return result;
    }
};

// =============================================================================
// ARROW FUNCTION EXPRESSION
// =============================================================================

class ArrowFunctionExpression : public Expression {
private:
    std::vector<ASTNode*> parameters_;
    Type* returnType_;
    ASTNode* body_;
    bool isAsync_;
    
public:
    ArrowFunctionExpression(const std::vector<ASTNode*>& parameters,
                           Type* returnType = nullptr,
                           ASTNode* body = nullptr,
                           bool isAsync = false)
        : Expression(ASTNodeType::ArrowFunctionExpression),
          parameters_(parameters), returnType_(returnType), body_(body), isAsync_(isAsync) {}
    
    ~ArrowFunctionExpression() override {
        for (auto& param : parameters_) {
            delete param;
        }
        delete returnType_;
        delete body_;
    }
    
    // Getters
    const std::vector<ASTNode*>& getParameters() const { return parameters_; }
    Type* getReturnType() const { return returnType_; }
    ASTNode* getBody() const { return body_; }
    bool isAsync() const { return isAsync_; }
    
    // Setters
    void setParameters(const std::vector<ASTNode*>& parameters) { parameters_ = parameters; }
    void setReturnType(Type* returnType) { returnType_ = returnType; }
    void setBody(ASTNode* body) { body_ = body; }
    void setAsync(bool isAsync) { isAsync_ = isAsync; }
    
    // Helper methods
    bool hasReturnType() const { return returnType_ != nullptr; }
    bool hasBody() const { return body_ != nullptr; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children = parameters_;
        if (returnType_) children.push_back(returnType_);
        if (body_) children.push_back(body_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitArrowFunctionExpression(this);
    }
    
    std::string toString() const override {
        std::string result = "";
        
        if (isAsync_) result += "async ";
        
        if (parameters_.size() == 1 && parameters_[0]->getType() == ASTNodeType::Identifier) {
            // Single parameter without parentheses
            result += parameters_[0]->toString();
        } else {
            result += "(";
            for (size_t i = 0; i < parameters_.size(); ++i) {
                if (i > 0) result += ", ";
                if (parameters_[i]) {
                    result += parameters_[i]->toString();
                }
            }
            result += ")";
        }
        
        result += " => ";
        
        if (returnType_) {
            result += ": " + returnType_->getName() + " ";
        }
        
        if (body_) {
            result += body_->toString();
        }
        
        return result;
    }
};

// =============================================================================
// FUNCTION TYPE
// =============================================================================

class FunctionType : public Type {
private:
    std::vector<Type*> parameterTypes_;
    Type* returnType_;
    bool isAsync_;
    bool isGenerator_;
    
public:
    FunctionType(const std::vector<Type*>& parameterTypes,
                 Type* returnType,
                 bool isAsync = false,
                 bool isGenerator = false)
        : Type(TypeType::FunctionType),
          parameterTypes_(parameterTypes), returnType_(returnType),
          isAsync_(isAsync), isGenerator_(isGenerator) {}
    
    ~FunctionType() override {
        for (auto& paramType : parameterTypes_) {
            delete paramType;
        }
        delete returnType_;
    }
    
    // Getters
    const std::vector<Type*>& getParameterTypes() const { return parameterTypes_; }
    Type* getReturnType() const { return returnType_; }
    bool isAsync() const { return isAsync_; }
    bool isGenerator() const { return isGenerator_; }
    
    // Setters
    void setParameterTypes(const std::vector<Type*>& parameterTypes) { parameterTypes_ = parameterTypes; }
    void setReturnType(Type* returnType) { returnType_ = returnType; }
    void setAsync(bool isAsync) { isAsync_ = isAsync; }
    void setGenerator(bool isGenerator) { isGenerator_ = isGenerator; }
    
    // Helper methods
    size_t getParameterCount() const { return parameterTypes_.size(); }
    Type* getParameterType(size_t index) const {
        return (index < parameterTypes_.size()) ? parameterTypes_[index] : nullptr;
    }
    
    std::string getName() const override {
        std::string result = "";
        if (isAsync_) result += "async ";
        if (isGenerator_) result += "generator ";
        result += "(";
        
        for (size_t i = 0; i < parameterTypes_.size(); ++i) {
            if (i > 0) result += ", ";
            if (parameterTypes_[i]) {
                result += parameterTypes_[i]->getName();
            }
        }
        
        result += ") => ";
        if (returnType_) {
            result += returnType_->getName();
        } else {
            result += "void";
        }
        
        return result;
    }
};

// =============================================================================
// CALL EXPRESSION (Enhanced)
// =============================================================================

class CallExpression : public Expression {
private:
    ASTNode* callee_;
    std::vector<ASTNode*> arguments_;
    bool isOptional_;
    
public:
    CallExpression(ASTNode* callee,
                   const std::vector<ASTNode*>& arguments,
                   bool isOptional = false)
        : Expression(ASTNodeType::CallExpression),
          callee_(callee), arguments_(arguments), isOptional_(isOptional) {}
    
    ~CallExpression() override {
        delete callee_;
        for (auto& arg : arguments_) {
            delete arg;
        }
    }
    
    // Getters
    ASTNode* getCallee() const { return callee_; }
    const std::vector<ASTNode*>& getArguments() const { return arguments_; }
    bool isOptional() const { return isOptional_; }
    
    // Setters
    void setCallee(ASTNode* callee) { callee_ = callee; }
    void setArguments(const std::vector<ASTNode*>& arguments) { arguments_ = arguments; }
    void setOptional(bool isOptional) { isOptional_ = isOptional; }
    
    // Helper methods
    size_t getArgumentCount() const { return arguments_.size(); }
    ASTNode* getArgument(size_t index) const {
        return (index < arguments_.size()) ? arguments_[index] : nullptr;
    }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (callee_) children.push_back(callee_);
        for (auto& arg : arguments_) {
            children.push_back(arg);
        }
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitCallExpression(this);
    }
    
    std::string toString() const override {
        std::string result = "";
        if (callee_) {
            result += callee_->toString();
        }
        
        if (isOptional_) result += "?";
        
        result += "(";
        for (size_t i = 0; i < arguments_.size(); ++i) {
            if (i > 0) result += ", ";
            if (arguments_[i]) {
                result += arguments_[i]->toString();
            }
        }
        result += ")";
        return result;
    }
};

} // namespace ast
} // namespace tsc