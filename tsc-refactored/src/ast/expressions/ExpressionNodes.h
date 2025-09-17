#pragma once

#include "tsc/ast/core/ASTCore.h"

namespace tsc {
namespace ast {
namespace expressions {

// Literal expressions
class NumericLiteral : public Expression {
public:
    NumericLiteral(double value, const SourceLocation& loc) 
        : value_(value), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    bool isConstant() const override { return true; }
    String toString() const override;
    
    double getValue() const { return value_; }

private:
    double value_;
    SourceLocation location_;
};

class StringLiteral : public Expression {
public:
    StringLiteral(const String& value, const SourceLocation& loc) 
        : value_(value), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    bool isConstant() const override { return true; }
    String toString() const override;
    
    const String& getValue() const { return value_; }

private:
    String value_;
    SourceLocation location_;
};

class TemplateLiteral : public Expression {
public:
    struct Element {
        bool isExpression_;
        String text_;
        unique_ptr<Expression> expression_;
        
        Element(const String& text) : isExpression_(false), text_(text) {}
        Element(unique_ptr<Expression> expr) : isExpression_(true), expression_(std::move(expr)) {}
        
        bool isExpression() const { return isExpression_; }
        const String& getText() const { return text_; }
        Expression* getExpression() const { return expression_.get(); }
    };
    
    TemplateLiteral(std::vector<Element> elements, const SourceLocation& loc) 
        : elements_(std::move(elements)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    bool isConstant() const override { return false; }
    String toString() const override;
    
    const std::vector<Element>& getElements() const { return elements_; }

private:
    std::vector<Element> elements_;
    SourceLocation location_;
};

class BooleanLiteral : public Expression {
public:
    BooleanLiteral(bool value, const SourceLocation& loc) 
        : value_(value), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    bool isConstant() const override { return true; }
    String toString() const override;
    
    bool getValue() const { return value_; }

private:
    bool value_;
    SourceLocation location_;
};

class NullLiteral : public Expression {
public:
    NullLiteral(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    bool isConstant() const override { return true; }
    String toString() const override;

private:
    SourceLocation location_;
};

// Identifier and special expressions
class Identifier : public Expression {
public:
    Identifier(const String& name, const SourceLocation& loc) 
        : name_(name), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override;
    
    const String& getName() const { return name_; }

private:
    String name_;
    SourceLocation location_;
};

class ThisExpression : public Expression {
public:
    ThisExpression(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;

private:
    SourceLocation location_;
};

class SuperExpression : public Expression {
public:
    SuperExpression(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;

private:
    SourceLocation location_;
};

// Complex expressions
class NewExpression : public Expression {
public:
    NewExpression(unique_ptr<Expression> constructor, std::vector<unique_ptr<Expression>> arguments, const SourceLocation& loc)
        : constructor_(std::move(constructor)), arguments_(std::move(arguments)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getConstructor() const { return constructor_.get(); }
    const std::vector<unique_ptr<Expression>>& getArguments() const { return arguments_; }

private:
    unique_ptr<Expression> constructor_;
    std::vector<unique_ptr<Expression>> arguments_;
    SourceLocation location_;
};

class BinaryExpression : public Expression {
public:
    enum class Operator {
        Plus, Minus, Multiply, Divide, Modulo, Exponentiation,
        Equal, NotEqual, Less, Greater, LessEqual, GreaterEqual,
        LogicalAnd, LogicalOr, NullishCoalescing,
        BitwiseAnd, BitwiseOr, BitwiseXor, LeftShift, RightShift, UnsignedRightShift
    };
    
    BinaryExpression(unique_ptr<Expression> left, Operator op, unique_ptr<Expression> right, const SourceLocation& loc)
        : left_(std::move(left)), operator_(op), right_(std::move(right)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getLeft() const { return left_.get(); }
    Operator getOperator() const { return operator_; }
    Expression* getRight() const { return right_.get(); }

private:
    unique_ptr<Expression> left_;
    Operator operator_;
    unique_ptr<Expression> right_;
    SourceLocation location_;
};

class UnaryExpression : public Expression {
public:
    enum class Operator {
        UnaryPlus, UnaryMinus, LogicalNot, BitwiseNot,
        PrefixIncrement, PrefixDecrement, PostfixIncrement, PostfixDecrement,
        TypeOf, Void, Delete
    };
    
    UnaryExpression(Operator op, unique_ptr<Expression> operand, const SourceLocation& loc, bool isPrefix = true)
        : operator_(op), operand_(std::move(operand)), location_(loc), isPrefix_(isPrefix) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Operator getOperator() const { return operator_; }
    Expression* getOperand() const { return operand_.get(); }
    bool isPrefix() const { return isPrefix_; }

private:
    Operator operator_;
    unique_ptr<Expression> operand_;
    SourceLocation location_;
    bool isPrefix_;
};

class AssignmentExpression : public Expression {
public:
    enum class Operator {
        Assign, PlusAssign, MinusAssign, MultiplyAssign, DivideAssign, ModuloAssign,
        ExponentiationAssign, LeftShiftAssign, RightShiftAssign, UnsignedRightShiftAssign,
        BitwiseAndAssign, BitwiseOrAssign, BitwiseXorAssign
    };
    
    AssignmentExpression(unique_ptr<Expression> left, Operator op, unique_ptr<Expression> right, const SourceLocation& loc)
        : left_(std::move(left)), operator_(op), right_(std::move(right)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getLeft() const { return left_.get(); }
    Operator getOperator() const { return operator_; }
    Expression* getRight() const { return right_.get(); }

private:
    unique_ptr<Expression> left_;
    Operator operator_;
    unique_ptr<Expression> right_;
    SourceLocation location_;
};

class ConditionalExpression : public Expression {
public:
    ConditionalExpression(unique_ptr<Expression> condition, unique_ptr<Expression> thenExpr, unique_ptr<Expression> elseExpr, const SourceLocation& loc)
        : condition_(std::move(condition)), thenExpr_(std::move(thenExpr)), elseExpr_(std::move(elseExpr)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getCondition() const { return condition_.get(); }
    Expression* getThenExpression() const { return thenExpr_.get(); }
    Expression* getElseExpression() const { return elseExpr_.get(); }

private:
    unique_ptr<Expression> condition_;
    unique_ptr<Expression> thenExpr_;
    unique_ptr<Expression> elseExpr_;
    SourceLocation location_;
};

class CallExpression : public Expression {
public:
    CallExpression(unique_ptr<Expression> callee, std::vector<unique_ptr<Expression>> arguments, const SourceLocation& loc)
        : callee_(std::move(callee)), arguments_(std::move(arguments)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getCallee() const { return callee_.get(); }
    const std::vector<unique_ptr<Expression>>& getArguments() const { return arguments_; }

private:
    unique_ptr<Expression> callee_;
    std::vector<unique_ptr<Expression>> arguments_;
    SourceLocation location_;
};

class ArrayLiteral : public Expression {
public:
    ArrayLiteral(std::vector<unique_ptr<Expression>> elements, const SourceLocation& loc)
        : elements_(std::move(elements)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    const std::vector<unique_ptr<Expression>>& getElements() const { return elements_; }

private:
    std::vector<unique_ptr<Expression>> elements_;
    SourceLocation location_;
};

class IndexExpression : public Expression {
public:
    IndexExpression(unique_ptr<Expression> object, unique_ptr<Expression> index, const SourceLocation& loc)
        : object_(std::move(object)), index_(std::move(index)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override;
    
    Expression* getObject() const { return object_.get(); }
    Expression* getIndex() const { return index_.get(); }

private:
    unique_ptr<Expression> object_;
    unique_ptr<Expression> index_;
    SourceLocation location_;
};

class ObjectLiteral : public Expression {
public:
    struct Property {
        String name;
        unique_ptr<Expression> value;
        bool computed;
        bool shorthand;
        bool method;
        
        Property(const String& name, unique_ptr<Expression> value, bool computed = false, bool shorthand = false, bool method = false)
            : name(name), value(std::move(value)), computed(computed), shorthand(shorthand), method(method) {}
    };
    
    ObjectLiteral(std::vector<Property> properties, const SourceLocation& loc)
        : properties_(std::move(properties)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    const std::vector<Property>& getProperties() const { return properties_; }

private:
    std::vector<Property> properties_;
    SourceLocation location_;
};

class PropertyAccess : public Expression {
public:
    PropertyAccess(unique_ptr<Expression> object, const String& property, const SourceLocation& loc, bool computed = false)
        : object_(std::move(object)), property_(property), location_(loc), computed_(computed) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override;
    
    Expression* getObject() const { return object_.get(); }
    const String& getProperty() const { return property_; }
    bool isComputed() const { return computed_; }

private:
    unique_ptr<Expression> object_;
    String property_;
    SourceLocation location_;
    bool computed_;
};

class ArrowFunction : public Expression {
public:
    ArrowFunction(std::vector<unique_ptr<Parameter>> parameters, unique_ptr<Expression> body, shared_ptr<Type> returnType, const SourceLocation& loc)
        : parameters_(std::move(parameters)), body_(std::move(body)), returnType_(returnType), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    const std::vector<unique_ptr<Parameter>>& getParameters() const { return parameters_; }
    Expression* getBody() const { return body_.get(); }
    shared_ptr<Type> getReturnType() const { return returnType_; }

private:
    std::vector<unique_ptr<Parameter>> parameters_;
    unique_ptr<Expression> body_;
    shared_ptr<Type> returnType_;
    SourceLocation location_;
};

class FunctionExpression : public Expression {
public:
    FunctionExpression(const String& name, std::vector<unique_ptr<Parameter>> parameters, unique_ptr<BlockStatement> body, shared_ptr<Type> returnType, const SourceLocation& loc)
        : name_(name), parameters_(std::move(parameters)), body_(std::move(body)), returnType_(returnType), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    const String& getName() const { return name_; }
    const std::vector<unique_ptr<Parameter>>& getParameters() const { return parameters_; }
    BlockStatement* getBody() const { return body_.get(); }
    shared_ptr<Type> getReturnType() const { return returnType_; }

private:
    String name_;
    std::vector<unique_ptr<Parameter>> parameters_;
    unique_ptr<BlockStatement> body_;
    shared_ptr<Type> returnType_;
    SourceLocation location_;
};

class MoveExpression : public Expression {
public:
    MoveExpression(unique_ptr<Expression> expression, const SourceLocation& loc)
        : expression_(std::move(expression)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::XValue; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }

private:
    unique_ptr<Expression> expression_;
    SourceLocation location_;
};

} // namespace expressions
} // namespace ast
} // namespace tsc