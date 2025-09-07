#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"
#include <vector>

namespace tsc {

// Forward declarations
class ASTVisitor;
class Type;

// Base class for all AST nodes
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

protected:
    shared_ptr<Type> type_;
};

// Expression base class
class Expression : public ASTNode {
public:
    // Expression categories
    enum class Category {
        LValue,     // Can be assigned to
        RValue,     // Temporary value
        XValue      // Expiring value (for move semantics)
    };
    
    virtual Category getCategory() const = 0;
    virtual bool isConstant() const { return false; }
};

// Statement base class  
class Statement : public ASTNode {
public:
    // Statement types for control flow analysis
    enum class Kind {
        Expression,
        Block,
        If,
        While,
        DoWhile,
        For,
        Return,
        Break,
        Continue,
        Switch,
        Try,
        Throw,
        Declaration
    };
    
    virtual Kind getKind() const = 0;
};

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

// Declaration base class
class Declaration : public Statement {
public:
    Kind getKind() const override { return Kind::Declaration; }
    
    virtual String getName() const = 0;
    virtual bool isExported() const { return exported_; }
    virtual void setExported(bool exported) { exported_ = exported; }

protected:
    bool exported_ = false;
};

// Type system is defined in semantic/TypeSystem.h
// Only forward declaration needed here

// Specific AST node types

// Literals
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
    String toString() const override { return "null"; }

private:
    SourceLocation location_;
};

// Identifiers
class Identifier : public Expression {
public:
    Identifier(const String& name, const SourceLocation& loc)
        : name_(name), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }
    String toString() const override { return name_; }
    
    const String& getName() const { return name_; }

private:
    String name_;
    SourceLocation location_;
};

// Binary expressions
class BinaryExpression : public Expression {
public:
    enum class Operator {
        // Arithmetic
        Add, Subtract, Multiply, Divide, Modulo, Power,
        // Comparison
        Equal, NotEqual, StrictEqual, StrictNotEqual,
        Less, Greater, LessEqual, GreaterEqual,
        // Logical
        LogicalAnd, LogicalOr, NullishCoalescing,
        // Bitwise
        BitwiseAnd, BitwiseOr, BitwiseXor,
        LeftShift, RightShift, UnsignedRightShift,
        // Other
        In, Instanceof, As, Satisfies
    };
    
    BinaryExpression(unique_ptr<Expression> left, Operator op, 
                    unique_ptr<Expression> right, const SourceLocation& loc)
        : left_(std::move(left)), operator_(op), 
          right_(std::move(right)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getLeft() const { return left_.get(); }
    Expression* getRight() const { return right_.get(); }
    Operator getOperator() const { return operator_; }

private:
    unique_ptr<Expression> left_;
    Operator operator_;
    unique_ptr<Expression> right_;
    SourceLocation location_;
};

// Unary expressions
class UnaryExpression : public Expression {
public:
    enum class Operator {
        Plus, Minus, BitwiseNot, LogicalNot,
        PreIncrement, PostIncrement, PreDecrement, PostDecrement,
        Typeof, Delete, Void, Await
    };
    
    UnaryExpression(Operator op, unique_ptr<Expression> operand, 
                   const SourceLocation& loc, bool prefix = true)
        : operator_(op), operand_(std::move(operand)), 
          location_(loc), prefix_(prefix) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getOperand() const { return operand_.get(); }
    Operator getOperator() const { return operator_; }
    bool isPrefix() const { return prefix_; }

private:
    Operator operator_;
    unique_ptr<Expression> operand_;
    SourceLocation location_;
    bool prefix_;
};

// Assignment expressions
class AssignmentExpression : public Expression {
public:
    enum class Operator {
        Assign,
        AddAssign, SubtractAssign, MultiplyAssign, DivideAssign, ModuloAssign,
        LeftShiftAssign, RightShiftAssign, UnsignedRightShiftAssign,
        BitwiseAndAssign, BitwiseOrAssign, BitwiseXorAssign,
        PowerAssign, NullishCoalescingAssign, LogicalAndAssign, LogicalOrAssign
    };
    
    AssignmentExpression(unique_ptr<Expression> left, Operator op,
                        unique_ptr<Expression> right, const SourceLocation& loc)
        : left_(std::move(left)), operator_(op),
          right_(std::move(right)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    Expression* getLeft() const { return left_.get(); }
    Expression* getRight() const { return right_.get(); }
    Operator getOperator() const { return operator_; }

private:
    unique_ptr<Expression> left_;
    Operator operator_;
    unique_ptr<Expression> right_;
    SourceLocation location_;
};

// Function calls
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

// Array literal expression
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

// Index expression (array access)
class IndexExpression : public Expression {
public:
    IndexExpression(unique_ptr<Expression> object, unique_ptr<Expression> index,
                    const SourceLocation& loc)
        : object_(std::move(object)), index_(std::move(index)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }  // Can be assigned to
    String toString() const override;
    
    Expression* getObject() const { return object_.get(); }
    Expression* getIndex() const { return index_.get(); }

private:
    unique_ptr<Expression> object_;
    unique_ptr<Expression> index_;
    SourceLocation location_;
};

// Object literal expression
class ObjectLiteral : public Expression {
public:
    // Property in an object literal
    class Property {
    public:
        Property(String key, unique_ptr<Expression> value, const SourceLocation& loc)
            : key_(key), value_(std::move(value)), location_(loc) {}
        
        const String& getKey() const { return key_; }
        Expression* getValue() const { return value_.get(); }
        SourceLocation getLocation() const { return location_; }
        
    private:
        String key_;
        unique_ptr<Expression> value_;
        SourceLocation location_;
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

// Property access expression (obj.prop)
class PropertyAccess : public Expression {
public:
    PropertyAccess(unique_ptr<Expression> object, String property, const SourceLocation& loc)
        : object_(std::move(object)), property_(property), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::LValue; }  // Can be assigned to
    String toString() const override;
    
    Expression* getObject() const { return object_.get(); }
    const String& getProperty() const { return property_; }

private:
    unique_ptr<Expression> object_;
    String property_;
    SourceLocation location_;
};

// Arrow function expression (param => body or (param1, param2) => body)
class ArrowFunction : public Expression {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        bool optional = false;
        bool rest = false;
        SourceLocation location;
    };

    ArrowFunction(std::vector<Parameter> parameters, unique_ptr<Statement> body, 
                  shared_ptr<Type> returnType, const SourceLocation& loc)
        : parameters_(std::move(parameters)), body_(std::move(body)), 
          returnType_(returnType), location_(loc) {}

    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;

    const std::vector<Parameter>& getParameters() const { return parameters_; }
    Statement* getBody() const { return body_.get(); }
    shared_ptr<Type> getReturnType() const { return returnType_; }

private:
    std::vector<Parameter> parameters_;
    unique_ptr<Statement> body_;
    shared_ptr<Type> returnType_;
    SourceLocation location_;
};

// Block statement
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

// Return statement
class ReturnStatement : public Statement {
public:
    ReturnStatement(unique_ptr<Expression> value, const SourceLocation& loc)
        : value_(std::move(value)), location_(loc) {}
    
    ReturnStatement(const SourceLocation& loc)  // For return without value
        : value_(nullptr), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Return; }
    String toString() const override;
    
    Expression* getValue() const { return value_.get(); }
    bool hasValue() const { return value_ != nullptr; }

private:
    unique_ptr<Expression> value_;
    SourceLocation location_;
};

// If statement
class IfStatement : public Statement {
public:
    IfStatement(unique_ptr<Expression> condition, unique_ptr<Statement> thenStmt,
                unique_ptr<Statement> elseStmt, const SourceLocation& loc)
        : condition_(std::move(condition)), thenStatement_(std::move(thenStmt)),
          elseStatement_(std::move(elseStmt)), location_(loc) {}
    
    IfStatement(unique_ptr<Expression> condition, unique_ptr<Statement> thenStmt,
                const SourceLocation& loc)
        : condition_(std::move(condition)), thenStatement_(std::move(thenStmt)),
          elseStatement_(nullptr), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::If; }
    String toString() const override;
    
    Expression* getCondition() const { return condition_.get(); }
    Statement* getThenStatement() const { return thenStatement_.get(); }
    Statement* getElseStatement() const { return elseStatement_.get(); }
    bool hasElse() const { return elseStatement_ != nullptr; }

private:
    unique_ptr<Expression> condition_;
    unique_ptr<Statement> thenStatement_;
    unique_ptr<Statement> elseStatement_;
    SourceLocation location_;
};

// While statement
class WhileStatement : public Statement {
public:
    WhileStatement(unique_ptr<Expression> condition, unique_ptr<Statement> body,
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

// Do-while statement
class DoWhileStatement : public Statement {
public:
    DoWhileStatement(unique_ptr<Statement> body, unique_ptr<Expression> condition,
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

// For statement
class ForStatement : public Statement {
public:
    ForStatement(unique_ptr<Statement> init, unique_ptr<Expression> condition,
                 unique_ptr<Expression> increment, unique_ptr<Statement> body,
                 const SourceLocation& loc)
        : init_(std::move(init)), condition_(std::move(condition)),
          increment_(std::move(increment)), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::For; }
    String toString() const override;
    
    Statement* getInit() const { return init_.get(); }
    Expression* getCondition() const { return condition_.get(); }
    Expression* getIncrement() const { return increment_.get(); }
    Statement* getBody() const { return body_.get(); }

private:
    unique_ptr<Statement> init_;
    unique_ptr<Expression> condition_;
    unique_ptr<Expression> increment_;
    unique_ptr<Statement> body_;
    SourceLocation location_;
};

// Case clause for switch statements
class CaseClause : public ASTNode {
public:
    CaseClause(unique_ptr<Expression> test, std::vector<unique_ptr<Statement>> statements,
               const SourceLocation& loc)
        : test_(std::move(test)), statements_(std::move(statements)), location_(loc) {}
    
    // Constructor for default case
    CaseClause(std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc)
        : test_(nullptr), statements_(std::move(statements)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    Expression* getTest() const { return test_.get(); }
    const std::vector<unique_ptr<Statement>>& getStatements() const { return statements_; }
    bool isDefault() const { return test_ == nullptr; }

private:
    unique_ptr<Expression> test_;  // null for default case
    std::vector<unique_ptr<Statement>> statements_;
    SourceLocation location_;
};

// Switch statement
class SwitchStatement : public Statement {
public:
    SwitchStatement(unique_ptr<Expression> discriminant, 
                    std::vector<unique_ptr<CaseClause>> cases,
                    const SourceLocation& loc)
        : discriminant_(std::move(discriminant)), cases_(std::move(cases)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Switch; }
    String toString() const override;
    
    Expression* getDiscriminant() const { return discriminant_.get(); }
    const std::vector<unique_ptr<CaseClause>>& getCases() const { return cases_; }

private:
    unique_ptr<Expression> discriminant_;
    std::vector<unique_ptr<CaseClause>> cases_;
    SourceLocation location_;
};

// Break statement
class BreakStatement : public Statement {
public:
    BreakStatement(const SourceLocation& loc)
        : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Break; }
    String toString() const override;

private:
    SourceLocation location_;
};

// Continue statement
class ContinueStatement : public Statement {
public:
    ContinueStatement(const SourceLocation& loc)
        : location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Continue; }
    String toString() const override;

private:
    SourceLocation location_;
};

// Forward declaration
class CatchClause;

// Try statement
class TryStatement : public Statement {
public:
    TryStatement(unique_ptr<BlockStatement> tryBlock,
                 unique_ptr<CatchClause> catchClause,
                 unique_ptr<BlockStatement> finallyBlock,
                 const SourceLocation& loc)
        : tryBlock_(std::move(tryBlock)), catchClause_(std::move(catchClause)),
          finallyBlock_(std::move(finallyBlock)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Kind getKind() const override { return Kind::Try; }
    String toString() const override;
    
    BlockStatement* getTryBlock() const { return tryBlock_.get(); }
    CatchClause* getCatchClause() const { return catchClause_.get(); }
    BlockStatement* getFinallyBlock() const { return finallyBlock_.get(); }
    
    bool hasCatch() const { return catchClause_ != nullptr; }
    bool hasFinally() const { return finallyBlock_ != nullptr; }

private:
    unique_ptr<BlockStatement> tryBlock_;
    unique_ptr<CatchClause> catchClause_;
    unique_ptr<BlockStatement> finallyBlock_;
    SourceLocation location_;
};

// Catch clause
class CatchClause : public ASTNode {
public:
    // Constructor with parameter (catch (e) { ... })
    CatchClause(String parameter, unique_ptr<BlockStatement> body, const SourceLocation& loc)
        : parameter_(parameter), hasParameter_(true), body_(std::move(body)), location_(loc) {}
    
    // Constructor without parameter (catch { ... })
    CatchClause(unique_ptr<BlockStatement> body, const SourceLocation& loc)
        : hasParameter_(false), body_(std::move(body)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    const String& getParameter() const { return parameter_; }
    BlockStatement* getBody() const { return body_.get(); }
    bool hasParameter() const { return hasParameter_; }

private:
    String parameter_;
    bool hasParameter_;
    unique_ptr<BlockStatement> body_;
    SourceLocation location_;
};

// Throw statement
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

// Variable declaration
class VariableDeclaration : public Declaration {
public:
    enum class Kind {
        Var, Let, Const
    };
    
    VariableDeclaration(Kind kind, const String& name, 
                       unique_ptr<Expression> initializer,
                       shared_ptr<Type> typeAnnotation,
                       const SourceLocation& loc)
        : kind_(kind), name_(name), initializer_(std::move(initializer)),
          typeAnnotation_(typeAnnotation), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    Kind getDeclarationKind() const { return kind_; }
    Expression* getInitializer() const { return initializer_.get(); }
    shared_ptr<Type> getTypeAnnotation() const { return typeAnnotation_; }

private:
    Kind kind_;
    String name_;
    unique_ptr<Expression> initializer_;
    shared_ptr<Type> typeAnnotation_;
    SourceLocation location_;
};

// Function declaration
class FunctionDeclaration : public Declaration {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        unique_ptr<Expression> defaultValue;
        bool optional = false;
        bool rest = false;
    };
    
    FunctionDeclaration(const String& name,
                       std::vector<Parameter> parameters,
                       shared_ptr<Type> returnType,
                       unique_ptr<BlockStatement> body,
                       const SourceLocation& loc,
                       bool async = false,
                       bool generator = false)
        : name_(name), parameters_(std::move(parameters)),
          returnType_(returnType), body_(std::move(body)),
          location_(loc), async_(async), generator_(generator) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<Parameter>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    BlockStatement* getBody() const { return body_.get(); }
    bool isAsync() const { return async_; }
    bool isGenerator() const { return generator_; }

private:
    String name_;
    std::vector<Parameter> parameters_;
    shared_ptr<Type> returnType_;
    unique_ptr<BlockStatement> body_;
    SourceLocation location_;
    bool async_;
    bool generator_;
};

// Class-related declarations
class PropertyDeclaration : public Declaration {
public:
    PropertyDeclaration(const String& name,
                       shared_ptr<Type> type,
                       unique_ptr<Expression> initializer,
                       const SourceLocation& loc,
                       bool isStatic = false,
                       bool isReadonly = false,
                       bool isPrivate = false,
                       bool isProtected = false)
        : name_(name), type_(type), initializer_(std::move(initializer)),
          location_(loc), isStatic_(isStatic), isReadonly_(isReadonly),
          isPrivate_(isPrivate), isProtected_(isProtected) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    shared_ptr<Type> getType() const override { return type_; }
    Expression* getInitializer() const { return initializer_.get(); }
    bool isStatic() const { return isStatic_; }
    bool isReadonly() const { return isReadonly_; }
    bool isPrivate() const { return isPrivate_; }
    bool isProtected() const { return isProtected_; }

private:
    String name_;
    shared_ptr<Type> type_;
    unique_ptr<Expression> initializer_;
    SourceLocation location_;
    bool isStatic_;
    bool isReadonly_;
    bool isPrivate_;
    bool isProtected_;
};

class MethodDeclaration : public Declaration {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        unique_ptr<Expression> defaultValue;
        bool optional = false;
        bool rest = false;
    };
    
    MethodDeclaration(const String& name,
                     std::vector<Parameter> parameters,
                     shared_ptr<Type> returnType,
                     unique_ptr<BlockStatement> body,
                     const SourceLocation& loc,
                     bool isStatic = false,
                     bool isPrivate = false,
                     bool isProtected = false,
                     bool isAbstract = false,
                     bool isAsync = false)
        : name_(name), parameters_(std::move(parameters)),
          returnType_(returnType), body_(std::move(body)),
          location_(loc), isStatic_(isStatic), isPrivate_(isPrivate),
          isProtected_(isProtected), isAbstract_(isAbstract), isAsync_(isAsync) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<Parameter>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    BlockStatement* getBody() const { return body_.get(); }
    bool isStatic() const { return isStatic_; }
    bool isPrivate() const { return isPrivate_; }
    bool isProtected() const { return isProtected_; }
    bool isAbstract() const { return isAbstract_; }
    bool isAsync() const { return isAsync_; }

private:
    String name_;
    std::vector<Parameter> parameters_;
    shared_ptr<Type> returnType_;
    unique_ptr<BlockStatement> body_;
    SourceLocation location_;
    bool isStatic_;
    bool isPrivate_;
    bool isProtected_;
    bool isAbstract_;
    bool isAsync_;
};

class ClassDeclaration : public Declaration {
public:
    ClassDeclaration(const String& name,
                    shared_ptr<Type> baseClass,
                    std::vector<shared_ptr<Type>> interfaces,
                    std::vector<unique_ptr<PropertyDeclaration>> properties,
                    std::vector<unique_ptr<MethodDeclaration>> methods,
                    unique_ptr<MethodDeclaration> constructor,
                    const SourceLocation& loc,
                    bool isAbstract = false)
        : name_(name), baseClass_(baseClass), interfaces_(std::move(interfaces)),
          properties_(std::move(properties)), methods_(std::move(methods)),
          constructor_(std::move(constructor)), location_(loc), isAbstract_(isAbstract) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    shared_ptr<Type> getBaseClass() const { return baseClass_; }
    const std::vector<shared_ptr<Type>>& getInterfaces() const { return interfaces_; }
    const std::vector<unique_ptr<PropertyDeclaration>>& getProperties() const { return properties_; }
    const std::vector<unique_ptr<MethodDeclaration>>& getMethods() const { return methods_; }
    MethodDeclaration* getConstructor() const { return constructor_.get(); }
    bool isAbstract() const { return isAbstract_; }

private:
    String name_;
    shared_ptr<Type> baseClass_;
    std::vector<shared_ptr<Type>> interfaces_;
    std::vector<unique_ptr<PropertyDeclaration>> properties_;
    std::vector<unique_ptr<MethodDeclaration>> methods_;
    unique_ptr<MethodDeclaration> constructor_;
    SourceLocation location_;
    bool isAbstract_;
};

// Module/File AST node
class Module : public ASTNode {
public:
    Module(const String& filename, std::vector<unique_ptr<Statement>> statements)
        : filename_(filename), statements_(std::move(statements)) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { 
        return SourceLocation(filename_, 1, 1); 
    }
    String toString() const override;
    
    const String& getFilename() const { return filename_; }
    const std::vector<unique_ptr<Statement>>& getStatements() const { return statements_; }

private:
    String filename_;
    std::vector<unique_ptr<Statement>> statements_;
};

// Visitor interface for AST traversal
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // Expressions
    virtual void visit(NumericLiteral& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(BooleanLiteral& node) = 0;
    virtual void visit(NullLiteral& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(BinaryExpression& node) = 0;
    virtual void visit(UnaryExpression& node) = 0;
    virtual void visit(AssignmentExpression& node) = 0;
    virtual void visit(CallExpression& node) = 0;
    virtual void visit(ArrayLiteral& node) = 0;
    virtual void visit(IndexExpression& node) = 0;
    virtual void visit(ObjectLiteral& node) = 0;
    virtual void visit(PropertyAccess& node) = 0;
    virtual void visit(ArrowFunction& node) = 0;
    
    // Statements
    virtual void visit(ExpressionStatement& node) = 0;
    virtual void visit(BlockStatement& node) = 0;
    virtual void visit(ReturnStatement& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(WhileStatement& node) = 0;
    virtual void visit(DoWhileStatement& node) = 0;
    virtual void visit(ForStatement& node) = 0;
    virtual void visit(SwitchStatement& node) = 0;
    virtual void visit(CaseClause& node) = 0;
    virtual void visit(BreakStatement& node) = 0;
    virtual void visit(ContinueStatement& node) = 0;
    virtual void visit(TryStatement& node) = 0;
    virtual void visit(CatchClause& node) = 0;
    virtual void visit(ThrowStatement& node) = 0;
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(FunctionDeclaration& node) = 0;
    
    // Class-related declarations
    virtual void visit(PropertyDeclaration& node) = 0;
    virtual void visit(MethodDeclaration& node) = 0;
    virtual void visit(ClassDeclaration& node) = 0;
    
    // Module
    virtual void visit(Module& node) = 0;
};

} // namespace tsc