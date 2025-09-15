#pragma once

#include "tsc/ast/AST.h"
#include "tsc/ast/Expression.h"
#include "tsc/ast/VariableDeclaration.h"
#include <vector>
#include <string>

namespace tsc {
namespace ast {

// =============================================================================
// DESTRUCTURING PATTERNS
// =============================================================================

// Base class for all destructuring patterns
class DestructuringPattern : public ASTNode {
protected:
    DestructuringPattern(ASTNodeType type) : ASTNode(type) {}
    
public:
    virtual ~DestructuringPattern() = default;
    
    // Pure virtual methods to be implemented by derived classes
    virtual std::vector<ASTNode*> getPatternElements() const = 0;
    virtual bool hasRestElement() const = 0;
    virtual ASTNode* getRestElement() const = 0;
};

// =============================================================================
// ARRAY DESTRUCTURING PATTERN
// =============================================================================

class ArrayDestructuringPattern : public DestructuringPattern {
private:
    std::vector<ASTNode*> elements_;
    ASTNode* restElement_;
    
public:
    ArrayDestructuringPattern(const std::vector<ASTNode*>& elements, ASTNode* restElement = nullptr)
        : DestructuringPattern(ASTNodeType::ArrayDestructuringPattern), elements_(elements), restElement_(restElement) {}
    
    ~ArrayDestructuringPattern() override {
        for (auto& element : elements_) {
            delete element;
        }
        delete restElement_;
    }
    
    // Getters
    const std::vector<ASTNode*>& getElements() const { return elements_; }
    ASTNode* getRestElement() const override { return restElement_; }
    bool hasRestElement() const override { return restElement_ != nullptr; }
    
    // Setters
    void setElements(const std::vector<ASTNode*>& elements) { elements_ = elements; }
    void setRestElement(ASTNode* restElement) { restElement_ = restElement; }
    
    // DestructuringPattern interface
    std::vector<ASTNode*> getPatternElements() const override { return elements_; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children = elements_;
        if (restElement_) children.push_back(restElement_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitArrayDestructuringPattern(this);
    }
    
    std::string toString() const override {
        std::string result = "[";
        for (size_t i = 0; i < elements_.size(); ++i) {
            if (i > 0) result += ", ";
            if (elements_[i]) {
                result += elements_[i]->toString();
            } else {
                result += "";
            }
        }
        if (restElement_) {
            if (!elements_.empty()) result += ", ";
            result += "..." + restElement_->toString();
        }
        result += "]";
        return result;
    }
};

// =============================================================================
// OBJECT DESTRUCTURING PATTERN
// =============================================================================

class ObjectDestructuringPattern : public DestructuringPattern {
private:
    std::vector<ASTNode*> properties_;
    ASTNode* restElement_;
    
public:
    ObjectDestructuringPattern(const std::vector<ASTNode*>& properties, ASTNode* restElement = nullptr)
        : DestructuringPattern(ASTNodeType::ObjectDestructuringPattern), properties_(properties), restElement_(restElement) {}
    
    ~ObjectDestructuringPattern() override {
        for (auto& property : properties_) {
            delete property;
        }
        delete restElement_;
    }
    
    // Getters
    const std::vector<ASTNode*>& getProperties() const { return properties_; }
    ASTNode* getRestElement() const override { return restElement_; }
    bool hasRestElement() const override { return restElement_ != nullptr; }
    
    // Setters
    void setProperties(const std::vector<ASTNode*>& properties) { properties_ = properties; }
    void setRestElement(ASTNode* restElement) { restElement_ = restElement; }
    
    // DestructuringPattern interface
    std::vector<ASTNode*> getPatternElements() const override { return properties_; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children = properties_;
        if (restElement_) children.push_back(restElement_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitObjectDestructuringPattern(this);
    }
    
    std::string toString() const override {
        std::string result = "{";
        for (size_t i = 0; i < properties_.size(); ++i) {
            if (i > 0) result += ", ";
            if (properties_[i]) {
                result += properties_[i]->toString();
            } else {
                result += "";
            }
        }
        if (restElement_) {
            if (!properties_.empty()) result += ", ";
            result += "..." + restElement_->toString();
        }
        result += "}";
        return result;
    }
};

// =============================================================================
// DESTRUCTURING PROPERTY
// =============================================================================

class DestructuringProperty : public ASTNode {
private:
    ASTNode* key_;
    ASTNode* value_;
    bool shorthand_;
    
public:
    DestructuringProperty(ASTNode* key, ASTNode* value, bool shorthand = false)
        : ASTNode(ASTNodeType::DestructuringProperty), key_(key), value_(value), shorthand_(shorthand) {}
    
    ~DestructuringProperty() override {
        delete key_;
        delete value_;
    }
    
    // Getters
    ASTNode* getKey() const { return key_; }
    ASTNode* getValue() const { return value_; }
    bool isShorthand() const { return shorthand_; }
    
    // Setters
    void setKey(ASTNode* key) { key_ = key; }
    void setValue(ASTNode* value) { value_ = value; }
    void setShorthand(bool shorthand) { shorthand_ = shorthand; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (key_) children.push_back(key_);
        if (value_) children.push_back(value_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitDestructuringProperty(this);
    }
    
    std::string toString() const override {
        if (shorthand_) {
            return value_->toString();
        } else {
            return key_->toString() + ": " + value_->toString();
        }
    }
};

// =============================================================================
// REST ELEMENT
// =============================================================================

class RestElement : public ASTNode {
private:
    ASTNode* argument_;
    
public:
    RestElement(ASTNode* argument)
        : ASTNode(ASTNodeType::RestElement), argument_(argument) {}
    
    ~RestElement() override {
        delete argument_;
    }
    
    // Getters
    ASTNode* getArgument() const { return argument_; }
    
    // Setters
    void setArgument(ASTNode* argument) { argument_ = argument; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (argument_) children.push_back(argument_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitRestElement(this);
    }
    
    std::string toString() const override {
        return "..." + (argument_ ? argument_->toString() : "");
    }
};

// =============================================================================
// OBJECT LITERAL
// =============================================================================

class ObjectLiteral : public Expression {
private:
    std::vector<ASTNode*> properties_;
    
public:
    ObjectLiteral(const std::vector<ASTNode*>& properties)
        : Expression(ASTNodeType::ObjectLiteral), properties_(properties) {}
    
    ~ObjectLiteral() override {
        for (auto& property : properties_) {
            delete property;
        }
    }
    
    // Getters
    const std::vector<ASTNode*>& getProperties() const { return properties_; }
    
    // Setters
    void setProperties(const std::vector<ASTNode*>& properties) { properties_ = properties; }
    void addProperty(ASTNode* property) { properties_.push_back(property); }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        return properties_;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitObjectLiteral(this);
    }
    
    std::string toString() const override {
        std::string result = "{";
        for (size_t i = 0; i < properties_.size(); ++i) {
            if (i > 0) result += ", ";
            if (properties_[i]) {
                result += properties_[i]->toString();
            } else {
                result += "";
            }
        }
        result += "}";
        return result;
    }
};

// =============================================================================
// OBJECT PROPERTY
// =============================================================================

class ObjectProperty : public ASTNode {
private:
    ASTNode* key_;
    ASTNode* value_;
    bool shorthand_;
    bool computed_;
    
public:
    ObjectProperty(ASTNode* key, ASTNode* value, bool shorthand = false, bool computed = false)
        : ASTNode(ASTNodeType::ObjectProperty), key_(key), value_(value), shorthand_(shorthand), computed_(computed) {}
    
    ~ObjectProperty() override {
        delete key_;
        delete value_;
    }
    
    // Getters
    ASTNode* getKey() const { return key_; }
    ASTNode* getValue() const { return value_; }
    bool isShorthand() const { return shorthand_; }
    bool isComputed() const { return computed_; }
    
    // Setters
    void setKey(ASTNode* key) { key_ = key; }
    void setValue(ASTNode* value) { value_ = value; }
    void setShorthand(bool shorthand) { shorthand_ = shorthand; }
    void setComputed(bool computed) { computed_ = computed; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (key_) children.push_back(key_);
        if (value_) children.push_back(value_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitObjectProperty(this);
    }
    
    std::string toString() const override {
        if (shorthand_) {
            return value_->toString();
        } else {
            std::string keyStr = computed_ ? "[" + key_->toString() + "]" : key_->toString();
            return keyStr + ": " + value_->toString();
        }
    }
};

// =============================================================================
// MEMBER EXPRESSION (Enhanced for object access)
// =============================================================================

class MemberExpression : public Expression {
private:
    ASTNode* object_;
    ASTNode* property_;
    bool computed_;
    
public:
    MemberExpression(ASTNode* object, ASTNode* property, bool computed = false)
        : Expression(ASTNodeType::MemberExpression), object_(object), property_(property), computed_(computed) {}
    
    ~MemberExpression() override {
        delete object_;
        delete property_;
    }
    
    // Getters
    ASTNode* getObject() const { return object_; }
    ASTNode* getProperty() const { return property_; }
    bool isComputed() const { return computed_; }
    
    // Setters
    void setObject(ASTNode* object) { object_ = object; }
    void setProperty(ASTNode* property) { property_ = property; }
    void setComputed(bool computed) { computed_ = computed; }
    
    // AST Node interface
    std::vector<ASTNode*> getChildren() const override {
        std::vector<ASTNode*> children;
        if (object_) children.push_back(object_);
        if (property_) children.push_back(property_);
        return children;
    }
    
    void accept(ASTVisitor* visitor) override {
        visitor->visitMemberExpression(this);
    }
    
    std::string toString() const override {
        std::string objStr = object_ ? object_->toString() : "";
        std::string propStr = property_ ? property_->toString() : "";
        
        if (computed_) {
            return objStr + "[" + propStr + "]";
        } else {
            return objStr + "." + propStr;
        }
    }
};

} // namespace ast
} // namespace tsc