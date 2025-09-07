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

// Type base class (for TypeScript type system)
class Type {
public:
    virtual ~Type() = default;
    
    enum class Kind {
        Primitive,    // number, string, boolean, etc.
        Object,       // interface, class types
        Union,        // A | B
        Intersection, // A & B
        Function,     // (args) => return
        Array,        // T[]
        Tuple,        // [T1, T2, ...]
        Generic,      // T<U>
        TypeVar,      // T (type variable)
        Conditional,  // T extends U ? V : W
        Mapped,       // { [K in keyof T]: U }
        Literal,      // "string" | 42 | true
        Void,         // void
        Never,        // never
        Any,          // any
        Unknown       // unknown
    };
    
    virtual Kind getKind() const = 0;
    virtual String toString() const = 0;
    virtual bool isAssignableTo(const Type& other) const = 0;
    virtual bool equals(const Type& other) const = 0;
    
    // Type predicates
    bool isPrimitive() const { return getKind() == Kind::Primitive; }
    bool isObject() const { return getKind() == Kind::Object; }
    bool isFunction() const { return getKind() == Kind::Function; }
    bool isGeneric() const { return getKind() == Kind::Generic; }
};

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
    
    // Statements
    virtual void visit(BlockStatement& node) = 0;
    virtual void visit(VariableDeclaration& node) = 0;
    virtual void visit(FunctionDeclaration& node) = 0;
    
    // Module
    virtual void visit(Module& node) = 0;
};

} // namespace tsc