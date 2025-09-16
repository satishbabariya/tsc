/**
 * @file AST.h
 * @brief Abstract Syntax Tree definitions for the TSC compiler
 * @author TSC Compiler Team
 * @version 1.0
 * 
 * This file defines the complete AST structure for TypeScript syntax,
 * following the visitor pattern for tree traversal and maintaining
 * source location information for error reporting.
 */

#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <optional>

namespace tsc {

// ============================================================================
// ENUMS AND TYPES
// ============================================================================

/**
 * @brief Variance for type parameters
 */
enum class Variance {
    Invariant,
    Covariant,
    Contravariant
};

/**
 * @brief Binary operator types
 */
enum class BinaryOperator {
    Add, Sub, Mul, Div, Mod,
    Equal, NotEqual, Less, LessEqual, Greater, GreaterEqual,
    LogicalAnd, LogicalOr,
    BitwiseAnd, BitwiseOr, BitwiseXor,
    LeftShift, RightShift, UnsignedRightShift
};

/**
 * @brief Unary operator types
 */
enum class UnaryOperator {
    Plus, Minus, LogicalNot, BitwiseNot,
    PreIncrement, PreDecrement, PostIncrement, PostDecrement
};

/**
 * @brief Assignment operator types
 */
enum class AssignmentOperator {
    Assign, AddAssign, SubAssign, MulAssign, DivAssign, ModAssign,
    LeftShiftAssign, RightShiftAssign, UnsignedRightShiftAssign,
    BitwiseAndAssign, BitwiseOrAssign, BitwiseXorAssign
};

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

class ASTVisitor;
class Type;
class Symbol;

// Standalone classes (formerly nested structs)
class CaseClause;
class CatchClause;
class MethodDeclaration;
class EnumMember;

// ============================================================================
// BASE AST NODE CLASS
// ============================================================================

/**
 * @brief Base class for all AST nodes
 * 
 * Provides common functionality for all AST nodes including visitor pattern
 * support, source location tracking, and type information storage.
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    
    // Visitor pattern support
    virtual void accept(ASTVisitor& visitor) = 0;
    
    // Source location information
    virtual SourceLocation getLocation() const = 0;
    
    // Type information (set during semantic analysis)
    virtual shared_ptr<Type> getType() const { return type_; }
    virtual void setType(shared_ptr<Type> type) { type_ = type; }
    
    // Utility methods
    virtual String toString() const = 0;
    
    // Node properties
    virtual bool isConstant() const { return false; }
    virtual bool hasSideEffects() const { return true; }

protected:
    shared_ptr<Type> type_;
};

// ============================================================================
// EXPRESSION BASE CLASS
// ============================================================================

/**
 * @brief Base class for all expressions
 * 
 * Expressions represent values that can be evaluated and have a type.
 * They are categorized by their value category (LValue, RValue, XValue).
 */
class Expression : public ASTNode {
public:
    /// @brief Expression value categories
    enum class Category : std::uint8_t {
        LValue,     ///< Can be assigned to (variables, properties)
        RValue,     ///< Temporary value (literals, function calls)
        XValue      ///< Expiring value (for move semantics)
    };
    
    virtual Category getCategory() const = 0;
    virtual bool isConstant() const override { return false; }
    
    /// @brief Check if expression can be used as statement
    virtual bool canBeStatement() const { return true; }
};

// ============================================================================
// STATEMENT BASE CLASS
// ============================================================================

/**
 * @brief Base class for all statements
 * 
 * Statements represent actions that can be executed and don't produce
 * values (except for expression statements).
 */
class Statement : public ASTNode {
public:
    /// @brief Statement kinds for control flow analysis
    enum class Kind : std::uint8_t {
        Expression,
        Block,
        If,
        While,
        DoWhile,
        For,
        ForOf,
        Return,
        Break,
        Continue,
        Switch,
        Try,
        Throw,
        Declaration,
        Labeled,
        Let
    };
    
    virtual Kind getKind() const = 0;
    virtual bool hasSideEffects() const override { return true; }
};

// ============================================================================
// LITERAL EXPRESSIONS
// ============================================================================

/**
 * @brief Numeric literal expression
 */
class NumericLiteral : public Expression {
public:
    explicit NumericLiteral(double value, const SourceLocation& loc)
        : value_(value), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    bool isConstant() const override { return true; }
    
    double getValue() const { return value_; }

private:
    double value_;
    SourceLocation location_;
};

/**
 * @brief String literal expression
 */
class StringLiteral : public Expression {
public:
    explicit StringLiteral(const String& value, const SourceLocation& loc)
        : value_(value), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    bool isConstant() const override { return true; }
    
    const String& getValue() const { return value_; }

private:
    String value_;
    SourceLocation location_;
};

/**
 * @brief Template literal expression
 */
class TemplateLiteral : public Expression {
public:
    struct Element {
        bool isExpression;
        String text;
        unique_ptr<Expression> expression;
        
        Element(const String& text) : isExpression(false), text(text) {}
        Element(unique_ptr<Expression> expr) : isExpression(true), expression(std::move(expr)) {}
        
        bool isExpressionElement() const { return isExpression; }
        const String& getText() const { return text; }
        Expression* getExpression() const { return expression.get(); }
    };
    
    explicit TemplateLiteral(std::vector<Element> elements, const SourceLocation& loc)
        : elements_(std::move(elements)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    bool isConstant() const override;
    
    const std::vector<Element>& getElements() const { return elements_; }

private:
    std::vector<Element> elements_;
    SourceLocation location_;
};

/**
 * @brief Boolean literal expression
 */
class BooleanLiteral : public Expression {
public:
    explicit BooleanLiteral(bool value, const SourceLocation& loc)
        : value_(value), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    bool isConstant() const override { return true; }
    
    bool getValue() const { return value_; }

private:
    bool value_;
    SourceLocation location_;
};

/**
 * @brief Null literal expression
 */
class NullLiteral : public Expression {
public:
    explicit NullLiteral(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    bool isConstant() const override { return true; }

private:
    SourceLocation location_;
};

// ============================================================================
// IDENTIFIER AND PROPERTY ACCESS
// ============================================================================

/**
 * @brief Identifier expression
 */
class Identifier : public Expression {
public:
    explicit Identifier(const String& name, const SourceLocation& loc)
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

/**
 * @brief This expression
 */
class ThisExpression : public Expression {
public:
    explicit ThisExpression(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override;

private:
    SourceLocation location_;
};

/**
 * @brief Super expression
 */
class SuperExpression : public Expression {
public:
    explicit SuperExpression(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override;

private:
    SourceLocation location_;
};

/**
 * @brief Property access expression
 */
class PropertyAccess : public Expression {
public:
    PropertyAccess(unique_ptr<Expression> object, const String& property, 
                   const SourceLocation& loc)
        : object_(std::move(object)), property_(property), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override;
    
    Expression* getObject() const { return object_.get(); }
    const String& getProperty() const { return property_; }

private:
    unique_ptr<Expression> object_;
    String property_;
    SourceLocation location_;
};

/**
 * @brief Index expression (array/object access)
 */
class IndexExpression : public Expression {
public:
    IndexExpression(unique_ptr<Expression> object, unique_ptr<Expression> index,
                    const SourceLocation& loc)
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

// ============================================================================
// OBJECT AND ARRAY LITERALS
// ============================================================================

/**
 * @brief Object literal expression
 */
class ObjectLiteral : public Expression {
public:
    struct Property {
        String name;
        unique_ptr<Expression> value;
        bool isComputed;
        bool isShorthand;
        
        Property(const String& name, unique_ptr<Expression> value, bool computed = false)
            : name(name), value(std::move(value)), isComputed(computed), isShorthand(false) {}
        
        Property(unique_ptr<Expression> value, bool shorthand = false)
            : value(std::move(value)), isComputed(false), isShorthand(shorthand) {}
    };
    
    explicit ObjectLiteral(std::vector<Property> properties, const SourceLocation& loc)
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

/**
 * @brief Array literal expression
 */
class ArrayLiteral : public Expression {
public:
    explicit ArrayLiteral(std::vector<unique_ptr<Expression>> elements, 
                         const SourceLocation& loc)
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

// ============================================================================
// BINARY AND UNARY EXPRESSIONS
// ============================================================================

/**
 * @brief Binary expression
 */
class BinaryExpression : public Expression {
public:
    using Operator = BinaryOperator;
    
    BinaryExpression(unique_ptr<Expression> left, BinaryOperator operator_, 
                     unique_ptr<Expression> right, const SourceLocation& loc)
        : left_(std::move(left)), operator_(operator_), right_(std::move(right)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getLeft() const { return left_.get(); }
    BinaryOperator getOperator() const { return operator_; }
    Expression* getRight() const { return right_.get(); }

private:
    unique_ptr<Expression> left_;
    BinaryOperator operator_;
    unique_ptr<Expression> right_;
    SourceLocation location_;
};

/**
 * @brief Unary expression
 */
class UnaryExpression : public Expression {
public:
    using Operator = UnaryOperator;
    
    UnaryExpression(UnaryOperator operator_, unique_ptr<Expression> operand, 
                    const SourceLocation& loc)
        : operator_(operator_), operand_(std::move(operand)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    UnaryOperator getOperator() const { return operator_; }
    Expression* getOperand() const { return operand_.get(); }

private:
    UnaryOperator operator_;
    unique_ptr<Expression> operand_;
    SourceLocation location_;
};

/**
 * @brief Assignment expression
 */
class AssignmentExpression : public Expression {
public:
    using Operator = AssignmentOperator;
    
    AssignmentExpression(unique_ptr<Expression> left, AssignmentOperator operator_,
                         unique_ptr<Expression> right, const SourceLocation& loc)
        : left_(std::move(left)), operator_(operator_), right_(std::move(right)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override;
    
    Expression* getLeft() const { return left_.get(); }
    AssignmentOperator getOperator() const { return operator_; }
    Expression* getRight() const { return right_.get(); }

private:
    unique_ptr<Expression> left_;
    AssignmentOperator operator_;
    unique_ptr<Expression> right_;
    SourceLocation location_;
};

// ============================================================================
// FUNCTION CALLS
// ============================================================================

/**
 * @brief Function call expression
 */
class CallExpression : public Expression {
public:
    CallExpression(unique_ptr<Expression> callee, 
                   std::vector<unique_ptr<Expression>> arguments,
                   const SourceLocation& loc)
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

/**
 * @brief New expression (constructor call)
 */
class NewExpression : public Expression {
public:
    NewExpression(unique_ptr<Expression> constructor,
                  std::vector<unique_ptr<Expression>> arguments,
                  const SourceLocation& loc)
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

// ============================================================================
// CONDITIONAL EXPRESSIONS
// ============================================================================

/**
 * @brief Conditional (ternary) expression
 */
class ConditionalExpression : public Expression {
public:
    ConditionalExpression(unique_ptr<Expression> condition,
                          unique_ptr<Expression> trueExpr,
                          unique_ptr<Expression> falseExpr,
                          const SourceLocation& loc)
        : condition_(std::move(condition)), trueExpr_(std::move(trueExpr)), 
          falseExpr_(std::move(falseExpr)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getCondition() const { return condition_.get(); }
    Expression* getTrueExpression() const { return trueExpr_.get(); }
    Expression* getFalseExpression() const { return falseExpr_.get(); }

private:
    unique_ptr<Expression> condition_;
    unique_ptr<Expression> trueExpr_;
    unique_ptr<Expression> falseExpr_;
    SourceLocation location_;
};

// ============================================================================
// FUNCTION EXPRESSIONS
// ============================================================================

/**
 * @brief Arrow function expression
 */
class ArrowFunction : public Expression {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        unique_ptr<Expression> defaultValue;
        
        Parameter(const String& name, shared_ptr<Type> type = nullptr, 
                  unique_ptr<Expression> defaultValue = nullptr)
            : name(name), type(std::move(type)), defaultValue(std::move(defaultValue)) {}
    };
    
    ArrowFunction(std::vector<Parameter> parameters,
                  shared_ptr<Type> returnType,
                  unique_ptr<Statement> body,
                  const SourceLocation& loc)
        : parameters_(std::move(parameters)), returnType_(std::move(returnType)),
          body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    const std::vector<Parameter>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    Statement* getBody() const { return body_.get(); }

private:
    std::vector<Parameter> parameters_;
    shared_ptr<Type> returnType_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

/**
 * @brief Function expression
 */
class FunctionExpression : public Expression {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        unique_ptr<Expression> defaultValue;
        
        Parameter(const String& name, shared_ptr<Type> type = nullptr, 
                  unique_ptr<Expression> defaultValue = nullptr)
            : name(name), type(std::move(type)), defaultValue(std::move(defaultValue)) {}
    };
    
    FunctionExpression(const String& name,
                       std::vector<Parameter> parameters,
                       shared_ptr<Type> returnType,
                       unique_ptr<Statement> body,
                       const SourceLocation& loc)
        : name_(name), parameters_(std::move(parameters)), returnType_(std::move(returnType)),
          body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    const String& getName() const { return name_; }
    const std::vector<Parameter>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    Statement* getBody() const { return body_.get(); }

private:
    String name_;
    std::vector<Parameter> parameters_;
    shared_ptr<Type> returnType_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

/**
 * @brief Move expression (for move semantics)
 */
class MoveExpression : public Expression {
public:
    explicit MoveExpression(unique_ptr<Expression> operand, const SourceLocation& loc)
        : operand_(std::move(operand)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::XValue; }
    String toString() const override;
    
    Expression* getOperand() const { return operand_.get(); }

private:
    unique_ptr<Expression> operand_;
    SourceLocation location_;
};

// ============================================================================
// STATEMENTS
// ============================================================================

/**
 * @brief Expression statement
 */
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

/**
 * @brief Block statement
 */
class BlockStatement : public Statement {
public:
    explicit BlockStatement(std::vector<unique_ptr<Statement>> statements, 
                           const SourceLocation& loc)
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

// ============================================================================
// CONTROL FLOW STATEMENTS
// ============================================================================

/**
 * @brief If statement
 */
class IfStatement : public Statement {
public:
    IfStatement(unique_ptr<Expression> condition,
                unique_ptr<Statement> thenStmt,
                unique_ptr<Statement> elseStmt,
                const SourceLocation& loc)
        : condition_(std::move(condition)), thenStmt_(std::move(thenStmt)), 
          elseStmt_(std::move(elseStmt)), location_(loc) {}
    
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

/**
 * @brief While statement
 */
class WhileStatement : public Statement {
public:
    WhileStatement(unique_ptr<Expression> condition,
                  unique_ptr<Statement> body,
                  const SourceLocation& loc)
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

/**
 * @brief Do-while statement
 */
class DoWhileStatement : public Statement {
public:
    DoWhileStatement(unique_ptr<Statement> body,
                    unique_ptr<Expression> condition,
                    const SourceLocation& loc)
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

/**
 * @brief For statement
 */
class ForStatement : public Statement {
public:
    ForStatement(unique_ptr<Statement> init,
                 unique_ptr<Expression> condition,
                 unique_ptr<Expression> update,
                 unique_ptr<Statement> body,
                 const SourceLocation& loc)
        : init_(std::move(init)), condition_(std::move(condition)), 
          update_(std::move(update)), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::For; }
    String toString() const override;
    
    Statement* getInit() const { return init_.get(); }
    Expression* getCondition() const { return condition_.get(); }
    Expression* getUpdate() const { return update_.get(); }
    Statement* getBody() const { return body_.get(); }

private:
    unique_ptr<Statement> init_;
    unique_ptr<Expression> condition_;
    unique_ptr<Expression> update_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

/**
 * @brief For-of statement
 */
class ForOfStatement : public Statement {
public:
    ForOfStatement(unique_ptr<Expression> variable,
                   unique_ptr<Expression> iterable,
                   unique_ptr<Statement> body,
                   const SourceLocation& loc)
        : variable_(std::move(variable)), iterable_(std::move(iterable)), 
          body_(std::move(body)), location_(loc) {}
    
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

/**
 * @brief Switch statement
 */
class SwitchStatement : public Statement {
public:
    SwitchStatement(unique_ptr<Expression> expression,
                    std::vector<unique_ptr<CaseClause>> cases,
                    const SourceLocation& loc)
        : expression_(std::move(expression)), cases_(std::move(cases)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Switch; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }
    const std::vector<unique_ptr<CaseClause>>& getCases() const { return cases_; }

private:
    unique_ptr<Expression> expression_;
    std::vector<unique_ptr<CaseClause>> cases_;
    SourceLocation location_;
};

/**
 * @brief Try statement
 */
class TryStatement : public Statement {
public:
    TryStatement(unique_ptr<Statement> tryBlock,
                 std::vector<unique_ptr<CatchClause>> catchClauses,
                 unique_ptr<Statement> finallyBlock,
                 const SourceLocation& loc)
        : tryBlock_(std::move(tryBlock)), catchClauses_(std::move(catchClauses)),
          finallyBlock_(std::move(finallyBlock)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Try; }
    String toString() const override;
    
    Statement* getTryBlock() const { return tryBlock_.get(); }
    const std::vector<unique_ptr<CatchClause>>& getCatchClauses() const { return catchClauses_; }
    Statement* getFinallyBlock() const { return finallyBlock_.get(); }

private:
    unique_ptr<Statement> tryBlock_;
    std::vector<unique_ptr<CatchClause>> catchClauses_;
    unique_ptr<Statement> finallyBlock_;
    SourceLocation location_;
};

// ============================================================================
// JUMP STATEMENTS
// ============================================================================

/**
 * @brief Return statement
 */
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

/**
 * @brief Break statement
 */
class BreakStatement : public Statement {
public:
    explicit BreakStatement(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Break; }
    String toString() const override;

private:
    SourceLocation location_;
};

/**
 * @brief Continue statement
 */
class ContinueStatement : public Statement {
public:
    explicit ContinueStatement(const SourceLocation& loc) : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Continue; }
    String toString() const override;

private:
    SourceLocation location_;
};

/**
 * @brief Throw statement
 */
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

// ============================================================================
// DECLARATIONS
// ============================================================================

/**
 * @brief Base class for all declarations
 */
class Declaration : public Statement {
public:
    Kind getKind() const override { return Kind::Declaration; }
    
    virtual String getName() const = 0;
    virtual bool isExported() const { return exported_; }
    virtual void setExported(bool exported) { exported_ = exported; }

protected:
    bool exported_ = false;
};

/**
 * @brief Variable declaration
 */
class VariableDeclaration : public Declaration {
public:
    struct Variable {
        String name;
        unique_ptr<Expression> initializer;
        shared_ptr<Type> type;
        
        Variable(const String& name, unique_ptr<Expression> init = nullptr, 
                 shared_ptr<Type> type = nullptr)
            : name(name), initializer(std::move(init)), type(std::move(type)) {}
    };
    
    VariableDeclaration(std::vector<Variable> variables, const SourceLocation& loc)
        : variables_(std::move(variables)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override;
    
    const std::vector<Variable>& getVariables() const { return variables_; }

private:
    std::vector<Variable> variables_;
    SourceLocation location_;
};

/**
 * @brief Function declaration
 */
class FunctionDeclaration : public Declaration {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        unique_ptr<Expression> defaultValue;
        bool isRest = false;
        
        Parameter(const String& name, shared_ptr<Type> type = nullptr, 
                  unique_ptr<Expression> defaultValue = nullptr)
            : name(name), type(std::move(type)), defaultValue(std::move(defaultValue)) {}
    };
    
    FunctionDeclaration(const String& name,
                        std::vector<Parameter> parameters,
                        shared_ptr<Type> returnType,
                        unique_ptr<Statement> body,
                        const SourceLocation& loc)
        : name_(name), parameters_(std::move(parameters)), returnType_(std::move(returnType)),
          body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    const std::vector<Parameter>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    Statement* getBody() const { return body_.get(); }

private:
    String name_;
    std::vector<Parameter> parameters_;
    shared_ptr<Type> returnType_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

/**
 * @brief Property declaration (used in classes and interfaces)
 */
class PropertyDeclaration : public Declaration {
public:
    PropertyDeclaration(const String& name, shared_ptr<Type> type = nullptr,
                       unique_ptr<Expression> initializer = nullptr, const SourceLocation& loc = {})
        : name_(name), type_(std::move(type)), initializer_(std::move(initializer)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    shared_ptr<Type> getType() const override { return type_; }
    Expression* getInitializer() const { return initializer_.get(); }
    
    bool isStatic = false;
    bool isReadonly = false;
    bool isPublic = true;
    bool isPrivate = false;
    bool isProtected = false;

private:
    String name_;
    shared_ptr<Type> type_;
    unique_ptr<Expression> initializer_;
    SourceLocation location_;
};

/**
 * @brief Class declaration
 */
class ClassDeclaration : public Declaration {
public:
    
    ClassDeclaration(const String& name,
                     unique_ptr<Expression> superClass,
                     std::vector<unique_ptr<PropertyDeclaration>> properties,
                     std::vector<unique_ptr<FunctionDeclaration>> methods,
                     const SourceLocation& loc)
        : name_(name), superClass_(std::move(superClass)), 
          properties_(std::move(properties)), methods_(std::move(methods)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    Expression* getSuperClass() const { return superClass_.get(); }
    const std::vector<unique_ptr<PropertyDeclaration>>& getProperties() const { return properties_; }
    const std::vector<unique_ptr<FunctionDeclaration>>& getMethods() const { return methods_; }

private:
    String name_;
    unique_ptr<Expression> superClass_;
    std::vector<unique_ptr<PropertyDeclaration>> properties_;
    std::vector<unique_ptr<FunctionDeclaration>> methods_;
    SourceLocation location_;
};

/**
 * @brief Type parameter declaration
 */
class TypeParameter : public Declaration {
public:
    TypeParameter(const String& name, shared_ptr<Type> constraint = nullptr,
                  shared_ptr<Type> defaultType = nullptr, const SourceLocation& loc = {})
        : name_(name), constraint_(std::move(constraint)), defaultType_(std::move(defaultType)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    shared_ptr<Type> getConstraint() const { return constraint_; }
    shared_ptr<Type> getDefaultType() const { return defaultType_; }

private:
    String name_;
    shared_ptr<Type> constraint_;
    shared_ptr<Type> defaultType_;
    SourceLocation location_;
};

/**
 * @brief Interface declaration
 */
class InterfaceDeclaration : public Declaration {
public:
    InterfaceDeclaration(const String& name,
                         std::vector<unique_ptr<Expression>> extends,
                         std::vector<unique_ptr<PropertyDeclaration>> properties,
                         std::vector<unique_ptr<MethodDeclaration>> methods,
                         const SourceLocation& loc)
        : name_(name), extends_(std::move(extends)), 
          properties_(std::move(properties)), methods_(std::move(methods)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    const std::vector<unique_ptr<Expression>>& getExtends() const { return extends_; }
    const std::vector<unique_ptr<PropertyDeclaration>>& getProperties() const { return properties_; }
    const std::vector<unique_ptr<MethodDeclaration>>& getMethods() const { return methods_; }

private:
    String name_;
    std::vector<unique_ptr<Expression>> extends_;
    std::vector<unique_ptr<PropertyDeclaration>> properties_;
    std::vector<unique_ptr<MethodDeclaration>> methods_;
    SourceLocation location_;
};

/**
 * @brief Destructor declaration
 */
class DestructorDeclaration : public Declaration {
public:
    DestructorDeclaration(const String& name, const SourceLocation& loc)
        : name_(name), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }

private:
    String name_;
    SourceLocation location_;
};

/**
 * @brief Optional property access expression
 */
class OptionalPropertyAccess : public Expression {
public:
    OptionalPropertyAccess(unique_ptr<Expression> object, const String& property, const SourceLocation& loc)
        : object_(std::move(object)), property_(property), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    Expression* getObject() const { return object_.get(); }
    const String& getProperty() const { return property_; }

private:
    unique_ptr<Expression> object_;
    String property_;
    SourceLocation location_;
};

/**
 * @brief Optional index access expression
 */
class OptionalIndexAccess : public Expression {
public:
    OptionalIndexAccess(unique_ptr<Expression> object, unique_ptr<Expression> index, const SourceLocation& loc)
        : object_(std::move(object)), index_(std::move(index)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    Expression* getObject() const { return object_.get(); }
    Expression* getIndex() const { return index_.get(); }

private:
    unique_ptr<Expression> object_;
    unique_ptr<Expression> index_;
    SourceLocation location_;
};

/**
 * @brief Optional call expression
 */
class OptionalCallExpr : public Expression {
public:
    OptionalCallExpr(unique_ptr<Expression> callee, std::vector<unique_ptr<Expression>> args, const SourceLocation& loc)
        : callee_(std::move(callee)), arguments_(std::move(args)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    Expression* getCallee() const { return callee_.get(); }
    const std::vector<unique_ptr<Expression>>& getArguments() const { return arguments_; }

private:
    unique_ptr<Expression> callee_;
    std::vector<unique_ptr<Expression>> arguments_;
    SourceLocation location_;
};

/**
 * @brief Spread element
 */
class SpreadElement : public Expression {
public:
    SpreadElement(unique_ptr<Expression> expression, const SourceLocation& loc)
        : expression_(std::move(expression)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }

private:
    unique_ptr<Expression> expression_;
    SourceLocation location_;
};

/**
 * @brief Destructuring pattern base class
 */
class DestructuringPattern : public Expression {
public:
    DestructuringPattern(const SourceLocation& loc) : location_(loc) {}
    
    SourceLocation getLocation() const override { return location_; }

protected:
    SourceLocation location_;
};

/**
 * @brief Array destructuring pattern
 */
class ArrayDestructuringPattern : public DestructuringPattern {
public:
    ArrayDestructuringPattern(std::vector<unique_ptr<Expression>> elements, const SourceLocation& loc)
        : DestructuringPattern(loc), elements_(std::move(elements)) {}
    
    void accept(ASTVisitor& visitor) override;
    String toString() const override;
    
    const std::vector<unique_ptr<Expression>>& getElements() const { return elements_; }

private:
    std::vector<unique_ptr<Expression>> elements_;
};

/**
 * @brief Object destructuring pattern
 */
class ObjectDestructuringPattern : public DestructuringPattern {
public:
    struct Property {
        String name;
        unique_ptr<Expression> value;
        bool isShorthand;
        
        Property(const String& n, unique_ptr<Expression> v, bool shorthand = false)
            : name(n), value(std::move(v)), isShorthand(shorthand) {}
    };
    
    ObjectDestructuringPattern(std::vector<Property> properties, const SourceLocation& loc)
        : DestructuringPattern(loc), properties_(std::move(properties)) {}
    
    void accept(ASTVisitor& visitor) override;
    String toString() const override;
    
    const std::vector<Property>& getProperties() const { return properties_; }

private:
    std::vector<Property> properties_;
};

/**
 * @brief Identifier pattern
 */
class IdentifierPattern : public DestructuringPattern {
public:
    IdentifierPattern(const String& name, const SourceLocation& loc)
        : DestructuringPattern(loc), name_(name) {}
    
    void accept(ASTVisitor& visitor) override;
    String toString() const override;
    
    const String& getName() const { return name_; }

private:
    String name_;
};

/**
 * @brief Destructuring assignment
 */
class DestructuringAssignment : public Statement {
public:
    DestructuringAssignment(unique_ptr<DestructuringPattern> left, unique_ptr<Expression> right, const SourceLocation& loc)
        : left_(std::move(left)), right_(std::move(right)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    DestructuringPattern* getLeft() const { return left_.get(); }
    Expression* getRight() const { return right_.get(); }

private:
    unique_ptr<DestructuringPattern> left_;
    unique_ptr<Expression> right_;
    SourceLocation location_;
};

/**
 * @brief Case clause for switch statements
 */
class CaseClause : public ASTNode {
public:
    CaseClause(unique_ptr<Expression> expression, std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc)
        : expression_(std::move(expression)), statements_(std::move(statements)), isDefault_(false), location_(loc) {}
    
    CaseClause(std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc)
        : statements_(std::move(statements)), isDefault_(true), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    Expression* getExpression() const { return expression_.get(); }
    const std::vector<unique_ptr<Statement>>& getStatements() const { return statements_; }
    bool isDefault() const { return isDefault_; }

private:
    unique_ptr<Expression> expression_;
    std::vector<unique_ptr<Statement>> statements_;
    bool isDefault_;
    SourceLocation location_;
};

/**
 * @brief Catch clause for try statements
 */
class CatchClause : public ASTNode {
public:
    CatchClause(const String& parameter, unique_ptr<Statement> body, const SourceLocation& loc)
        : parameter_(parameter), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    const String& getParameter() const { return parameter_; }
    Statement* getBody() const { return body_.get(); }

private:
    String parameter_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

/**
 * @brief Method declaration for classes and interfaces
 */
class MethodDeclaration : public ASTNode {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        unique_ptr<Expression> defaultValue;
        
        Parameter(const String& n, shared_ptr<Type> t = nullptr, unique_ptr<Expression> def = nullptr)
            : name(n), type(std::move(t)), defaultValue(std::move(def)) {}
    };
    
    MethodDeclaration(const String& name, std::vector<Parameter> parameters, shared_ptr<Type> returnType = nullptr, const SourceLocation& loc = {})
        : name_(name), parameters_(std::move(parameters)), returnType_(std::move(returnType)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    const String& getName() const { return name_; }
    const std::vector<Parameter>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }

private:
    String name_;
    std::vector<Parameter> parameters_;
    shared_ptr<Type> returnType_;
    SourceLocation location_;
};

/**
 * @brief Enum member for enum declarations
 */
class EnumMember : public ASTNode {
public:
    EnumMember(const String& name, unique_ptr<Expression> value = nullptr, const SourceLocation& loc = {})
        : name_(name), value_(std::move(value)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    const String& getName() const { return name_; }
    Expression* getValue() const { return value_.get(); }

private:
    String name_;
    unique_ptr<Expression> value_;
    SourceLocation location_;
};

/**
 * @brief Import clause for module imports
 */
struct ImportClause {
    String defaultImport;
    std::vector<String> namedImports;
    String namespaceImport;
    
    ImportClause() = default;
    ImportClause(const String& defaultImp, const std::vector<String>& named, const String& namespaceImp)
        : defaultImport(defaultImp), namedImports(named), namespaceImport(namespaceImp) {}
};

/**
 * @brief Import specification
 */
struct ImportSpec {
    String name;
    String alias;
    
    ImportSpec(const String& n, const String& a = "") : name(n), alias(a) {}
};

/**
 * @brief Export clause for module exports
 */
struct ExportClause {
    String name;
    unique_ptr<Expression> expression;
    
    ExportClause() = default;
    ExportClause(const String& n) : name(n) {}
    ExportClause(unique_ptr<Expression> expr) : expression(std::move(expr)) {}
};

/**
 * @brief Export specification
 */
struct ExportSpec {
    String name;
    String alias;
    
    ExportSpec(const String& n, const String& a = "") : name(n), alias(a) {}
};

/**
 * @brief Enum declaration
 */
class EnumDeclaration : public Declaration {
public:
    EnumDeclaration(const String& name,
                    std::vector<unique_ptr<EnumMember>> members,
                    const SourceLocation& loc)
        : name_(name), members_(std::move(members)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    const std::vector<unique_ptr<EnumMember>>& getMembers() const { return members_; }

private:
    String name_;
    std::vector<unique_ptr<EnumMember>> members_;
    SourceLocation location_;
};

/**
 * @brief Type alias declaration
 */
class TypeAliasDeclaration : public Declaration {
public:
    TypeAliasDeclaration(const String& name,
                         std::vector<TypeParameter> typeParameters,
                         shared_ptr<Type> type,
                         const SourceLocation& loc)
        : name_(name), typeParameters_(std::move(typeParameters)), type_(std::move(type)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    const std::vector<TypeParameter>& getTypeParameters() const { return typeParameters_; }
    shared_ptr<Type> getType() const override { return type_; }

private:
    String name_;
    std::vector<TypeParameter> typeParameters_;
    shared_ptr<Type> type_;
    SourceLocation location_;
};

/**
 * @brief Import declaration
 */
class ImportDeclaration : public Declaration {
public:
    ImportDeclaration(const String& module,
                      std::vector<String> namedImports,
                      const String& defaultImport,
                      const String& namespaceImport,
                      const SourceLocation& loc)
        : module_(module), namedImports_(std::move(namedImports)), 
          defaultImport_(defaultImport), namespaceImport_(namespaceImport), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return module_; }
    
    const String& getModule() const { return module_; }
    const std::vector<String>& getNamedImports() const { return namedImports_; }
    const String& getDefaultImport() const { return defaultImport_; }
    const String& getNamespaceImport() const { return namespaceImport_; }

private:
    String module_;
    std::vector<String> namedImports_;
    String defaultImport_;
    String namespaceImport_;
    SourceLocation location_;
};

/**
 * @brief Export declaration
 */
class ExportDeclaration : public Declaration {
public:
    ExportDeclaration(const String& name,
                      unique_ptr<Expression> expression,
                      const SourceLocation& loc)
        : name_(name), expression_(std::move(expression)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    String getName() const override { return name_; }
    
    Expression* getExpression() const { return expression_.get(); }

private:
    String name_;
    unique_ptr<Expression> expression_;
    SourceLocation location_;
};

// ============================================================================
// MODULE SYSTEM
// ============================================================================

/**
 * @brief Module declaration
 */
class Module : public ASTNode {
public:
    explicit Module(std::vector<unique_ptr<Statement>> statements, const String& filename = "")
        : statements_(std::move(statements)), filename_(filename) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override;
    String toString() const override;
    
    const std::vector<unique_ptr<Statement>>& getStatements() const { return statements_; }
    const String& getFilename() const { return filename_; }

private:
    std::vector<unique_ptr<Statement>> statements_;
    String filename_;
};

// ============================================================================
// VISITOR INTERFACE
// ============================================================================

/**
 * @brief Abstract visitor interface for AST traversal
 * 
 * Provides methods for visiting all types of AST nodes, following
 * the visitor pattern for tree traversal and analysis.
 */
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // Module
    virtual void visit(Module& node) = 0;
    
    // Expressions
    virtual void visit(NumericLiteral& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(TemplateLiteral& node) = 0;
    virtual void visit(BooleanLiteral& node) = 0;
    virtual void visit(NullLiteral& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(ThisExpression& node) = 0;
    virtual void visit(SuperExpression& node) = 0;
    virtual void visit(PropertyAccess& node) = 0;
    virtual void visit(IndexExpression& node) = 0;
    virtual void visit(ObjectLiteral& node) = 0;
    virtual void visit(ArrayLiteral& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(UnaryExpression& node) = 0;
    virtual void visit(AssignmentExpression& node) = 0;
    virtual void visit(CallExpression& node) = 0;
    virtual void visit(NewExpression& node) = 0;
    virtual void visit(ConditionalExpression& node) = 0;
    virtual void visit(ArrowFunction& node) = 0;
    virtual void visit(FunctionExpression& node) = 0;
    virtual void visit(MoveExpression& node) = 0;
    
    // Statements
    virtual void visit(ExpressionStatement& node) = 0;
    virtual void visit(BlockStatement& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(WhileStatement& node) = 0;
    virtual void visit(DoWhileStatement& node) = 0;
    virtual void visit(ForStatement& node) = 0;
    virtual void visit(ForOfStatement& node) = 0;
    virtual void visit(SwitchStatement& node) = 0;
    virtual void visit(TryStatement& node) = 0;
    virtual void visit(ReturnStatement& node) = 0;
    virtual void visit(BreakStatement& node) = 0;
    virtual void visit(ContinueStatement& node) = 0;
    virtual void visit(ThrowStatement& node) = 0;
    
    // Declarations
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(FunctionDeclaration& node) = 0;
    virtual void visit(ClassDeclaration& node) = 0;
    virtual void visit(TypeParameter& node) = 0;
    virtual void visit(InterfaceDeclaration& node) = 0;
    virtual void visit(EnumDeclaration& node) = 0;
    virtual void visit(TypeAliasDeclaration& node) = 0;
    virtual void visit(ImportDeclaration& node) = 0;
    virtual void visit(ExportDeclaration& node) = 0;
    
    // Additional AST node types
    virtual void visit(DestructorDeclaration& node) = 0;
    virtual void visit(OptionalPropertyAccess& node) = 0;
    virtual void visit(OptionalIndexAccess& node) = 0;
    virtual void visit(OptionalCallExpr& node) = 0;
    virtual void visit(SpreadElement& node) = 0;
    virtual void visit(DestructuringPattern& node) = 0;
    virtual void visit(ArrayDestructuringPattern& node) = 0;
    virtual void visit(ObjectDestructuringPattern& node) = 0;
    virtual void visit(IdentifierPattern& node) = 0;
    virtual void visit(DestructuringAssignment& node) = 0;
    
    // Standalone classes (formerly nested structs)
    virtual void visit(CaseClause& node) = 0;
    virtual void visit(CatchClause& node) = 0;
    virtual void visit(MethodDeclaration& node) = 0;
    virtual void visit(EnumMember& node) = 0;
};

} // namespace tsc