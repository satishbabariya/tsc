#pragma once

#include "ASTNode.h"
#include "Type.h"
#include "Expression.h"
#include <string>
#include <vector>
#include <memory>

namespace tsc {
namespace ast {

// Forward declarations
class UnionType;
class IntersectionType;
class ConditionalType;
class MappedType;
class TypeParameter;

// Advanced TypeScript Features AST Nodes

/**
 * Base class for all advanced type-related AST nodes
 */
class AdvancedTypeNode : public Type {
public:
    AdvancedTypeNode(ASTNodeType type, SourceLocation location);
    virtual ~AdvancedTypeNode() = default;
};

/**
 * Represents a union type
 * string | number | boolean
 */
class UnionType : public AdvancedTypeNode {
private:
    std::vector<std::unique_ptr<Type>> types;
    
public:
    UnionType(SourceLocation location);
    virtual ~UnionType() = default;
    
    // Getters
    const std::vector<std::unique_ptr<Type>>& getTypes() const { return types; }
    
    // Setters
    void addType(std::unique_ptr<Type> type) { types.push_back(std::move(type)); }
    
    // Utility methods
    bool isEmpty() const { return types.empty(); }
    size_t getTypeCount() const { return types.size(); }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents an intersection type
 * A & B
 */
class IntersectionType : public AdvancedTypeNode {
private:
    std::vector<std::unique_ptr<Type>> types;
    
public:
    IntersectionType(SourceLocation location);
    virtual ~IntersectionType() = default;
    
    // Getters
    const std::vector<std::unique_ptr<Type>>& getTypes() const { return types; }
    
    // Setters
    void addType(std::unique_ptr<Type> type) { types.push_back(std::move(type)); }
    
    // Utility methods
    bool isEmpty() const { return types.empty(); }
    size_t getTypeCount() const { return types.size(); }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents a conditional type
 * T extends U ? X : Y
 */
class ConditionalType : public AdvancedTypeNode {
private:
    std::unique_ptr<Type> checkType;
    std::unique_ptr<Type> extendsType;
    std::unique_ptr<Type> trueType;
    std::unique_ptr<Type> falseType;
    
public:
    ConditionalType(SourceLocation location);
    virtual ~ConditionalType() = default;
    
    // Getters
    Type* getCheckType() const { return checkType.get(); }
    Type* getExtendsType() const { return extendsType.get(); }
    Type* getTrueType() const { return trueType.get(); }
    Type* getFalseType() const { return falseType.get(); }
    
    // Setters
    void setCheckType(std::unique_ptr<Type> type) { checkType = std::move(type); }
    void setExtendsType(std::unique_ptr<Type> type) { extendsType = std::move(type); }
    void setTrueType(std::unique_ptr<Type> type) { trueType = std::move(type); }
    void setFalseType(std::unique_ptr<Type> type) { falseType = std::move(type); }
    
    // Utility methods
    bool hasCheckType() const { return checkType != nullptr; }
    bool hasExtendsType() const { return extendsType != nullptr; }
    bool hasTrueType() const { return trueType != nullptr; }
    bool hasFalseType() const { return falseType != nullptr; }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents a mapped type
 * { [K in keyof T]: T[K] }
 */
class MappedType : public AdvancedTypeNode {
private:
    std::string keyName;
    std::unique_ptr<Type> keyType;
    std::unique_ptr<Type> valueType;
    bool isReadonly;
    bool isOptional;
    
public:
    MappedType(SourceLocation location);
    virtual ~MappedType() = default;
    
    // Getters
    const std::string& getKeyName() const { return keyName; }
    Type* getKeyType() const { return keyType.get(); }
    Type* getValueType() const { return valueType.get(); }
    bool isReadonlyMapped() const { return isReadonly; }
    bool isOptionalMapped() const { return isOptional; }
    
    // Setters
    void setKeyName(const std::string& name) { keyName = name; }
    void setKeyType(std::unique_ptr<Type> type) { keyType = std::move(type); }
    void setValueType(std::unique_ptr<Type> type) { valueType = std::move(type); }
    void setReadonly(bool readonly) { isReadonly = readonly; }
    void setOptional(bool optional) { isOptional = optional; }
    
    // Utility methods
    bool hasKeyType() const { return keyType != nullptr; }
    bool hasValueType() const { return valueType != nullptr; }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents a type parameter for generics
 * T, K extends keyof T
 */
class TypeParameter : public AdvancedTypeNode {
private:
    std::string name;
    std::unique_ptr<Type> constraint;
    std::unique_ptr<Type> defaultType;
    
public:
    TypeParameter(SourceLocation location);
    virtual ~TypeParameter() = default;
    
    // Getters
    const std::string& getName() const { return name; }
    Type* getConstraint() const { return constraint.get(); }
    Type* getDefaultType() const { return defaultType.get(); }
    
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setConstraint(std::unique_ptr<Type> constraint) { this->constraint = std::move(constraint); }
    void setDefaultType(std::unique_ptr<Type> defaultType) { this->defaultType = std::move(defaultType); }
    
    // Utility methods
    bool hasConstraint() const { return constraint != nullptr; }
    bool hasDefaultType() const { return defaultType != nullptr; }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents a template literal type
 * `Hello ${T}`
 */
class TemplateLiteralType : public AdvancedTypeNode {
private:
    std::vector<std::string> parts;
    std::vector<std::unique_ptr<Type>> interpolatedTypes;
    
public:
    TemplateLiteralType(SourceLocation location);
    virtual ~TemplateLiteralType() = default;
    
    // Getters
    const std::vector<std::string>& getParts() const { return parts; }
    const std::vector<std::unique_ptr<Type>>& getInterpolatedTypes() const { return interpolatedTypes; }
    
    // Setters
    void addPart(const std::string& part) { parts.push_back(part); }
    void addInterpolatedType(std::unique_ptr<Type> type) { interpolatedTypes.push_back(std::move(type)); }
    
    // Utility methods
    bool isEmpty() const { return parts.empty(); }
    size_t getPartCount() const { return parts.size(); }
    size_t getInterpolatedTypeCount() const { return interpolatedTypes.size(); }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents a keyof type
 * keyof T
 */
class KeyofType : public AdvancedTypeNode {
private:
    std::unique_ptr<Type> targetType;
    
public:
    KeyofType(SourceLocation location);
    virtual ~KeyofType() = default;
    
    // Getters
    Type* getTargetType() const { return targetType.get(); }
    
    // Setters
    void setTargetType(std::unique_ptr<Type> type) { targetType = std::move(type); }
    
    // Utility methods
    bool hasTargetType() const { return targetType != nullptr; }
    
    void accept(ASTVisitor& visitor) override;
};

/**
 * Represents a typeof type
 * typeof expression
 */
class TypeofType : public AdvancedTypeNode {
private:
    std::unique_ptr<Expression> expression;
    
public:
    TypeofType(SourceLocation location);
    virtual ~TypeofType() = default;
    
    // Getters
    Expression* getExpression() const { return expression.get(); }
    
    // Setters
    void setExpression(std::unique_ptr<Expression> expr) { expression = std::move(expr); }
    
    // Utility methods
    bool hasExpression() const { return expression != nullptr; }
    
    void accept(ASTVisitor& visitor) override;
};

} // namespace ast
} // namespace tsc