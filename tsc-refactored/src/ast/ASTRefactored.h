#pragma once

#include "tsc/ast/core/ASTCore.h"
#include "tsc/ast/expressions/ExpressionNodes.h"
#include "tsc/ast/statements/StatementNodes.h"
#include "tsc/ast/declarations/DeclarationNodes.h"

namespace tsc {
namespace ast {

// Forward declarations for types and other components
class ImportClause;
class ExportClause;

// Module class - represents a complete source file
class Module : public ASTNode {
public:
    Module(const String& filename, std::vector<unique_ptr<Statement>> statements)
        : filename_(filename), statements_(std::move(statements)) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return SourceLocation(filename_, 1, 1); }
    String toString() const override;
    
    const String& getFilename() const { return filename_; }
    const std::vector<unique_ptr<Statement>>& getStatements() const { return statements_; }
    const std::vector<unique_ptr<Declaration>>& getDeclarations() const;
    
    void setSourcePath(const String& path) { sourcePath_ = path; }
    const String& getSourcePath() const { return sourcePath_; }

private:
    String filename_;
    String sourcePath_;
    std::vector<unique_ptr<Statement>> statements_;
    mutable std::vector<unique_ptr<Declaration>> declarations_; // Cached declarations
};

// Destructuring pattern classes
class DestructuringPattern : public ASTNode {
public:
    virtual ~DestructuringPattern() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual SourceLocation getLocation() const = 0;
    virtual String toString() const = 0;
};

class ArrayDestructuringPattern : public DestructuringPattern {
public:
    ArrayDestructuringPattern(std::vector<unique_ptr<DestructuringPattern>> elements, const SourceLocation& loc)
        : elements_(std::move(elements)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    const std::vector<unique_ptr<DestructuringPattern>>& getElements() const { return elements_; }

private:
    std::vector<unique_ptr<DestructuringPattern>> elements_;
    SourceLocation location_;
};

class ObjectDestructuringPattern : public DestructuringPattern {
public:
    struct Property {
        String name;
        unique_ptr<DestructuringPattern> pattern;
        bool computed;
        bool shorthand;
        
        Property(const String& name, unique_ptr<DestructuringPattern> pattern, bool computed = false, bool shorthand = false)
            : name(name), pattern(std::move(pattern)), computed(computed), shorthand(shorthand) {}
    };
    
    ObjectDestructuringPattern(std::vector<Property> properties, const SourceLocation& loc)
        : properties_(std::move(properties)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    const std::vector<Property>& getProperties() const { return properties_; }

private:
    std::vector<Property> properties_;
    SourceLocation location_;
};

class IdentifierPattern : public DestructuringPattern {
public:
    IdentifierPattern(const String& name, const SourceLocation& loc)
        : name_(name), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String toString() const override;
    
    const String& getName() const { return name_; }

private:
    String name_;
    SourceLocation location_;
};

class DestructuringAssignment : public Expression {
public:
    DestructuringAssignment(unique_ptr<DestructuringPattern> pattern, unique_ptr<Expression> expression, const SourceLocation& loc)
        : pattern_(std::move(pattern)), expression_(std::move(expression)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    Category getCategory() const override { return Category::RValue; }
    String toString() const override;
    
    DestructuringPattern* getPattern() const { return pattern_.get(); }
    Expression* getExpression() const { return expression_.get(); }

private:
    unique_ptr<DestructuringPattern> pattern_;
    unique_ptr<Expression> expression_;
    SourceLocation location_;
};

// Import/Export clause classes
class ImportClause {
public:
    enum class Type {
        Named, Namespace, Default
    };
    
    ImportClause(Type type, const String& name, const String& alias = "")
        : type_(type), name_(name), alias_(alias.empty() ? name : alias) {}
    
    Type getType() const { return type_; }
    const String& getName() const { return name_; }
    const String& getAlias() const { return alias_; }

private:
    Type type_;
    String name_;
    String alias_;
};

class ExportClause {
public:
    enum class Type {
        Named, Namespace, Default, All
    };
    
    ExportClause(Type type, const String& name = "", unique_ptr<Statement> defaultExport = nullptr)
        : type_(type), name_(name), defaultExport_(std::move(defaultExport)) {}
    
    Type getType() const { return type_; }
    const String& getName() const { return name_; }
    Statement* getDefaultExport() const { return defaultExport_.get(); }

private:
    Type type_;
    String name_;
    unique_ptr<Statement> defaultExport_;
};

// AST Factory - creates AST nodes
class ASTFactory {
public:
    // Expression factories
    static unique_ptr<NumericLiteral> createNumericLiteral(double value, const SourceLocation& loc);
    static unique_ptr<StringLiteral> createStringLiteral(const String& value, const SourceLocation& loc);
    static unique_ptr<BooleanLiteral> createBooleanLiteral(bool value, const SourceLocation& loc);
    static unique_ptr<NullLiteral> createNullLiteral(const SourceLocation& loc);
    static unique_ptr<Identifier> createIdentifier(const String& name, const SourceLocation& loc);
    static unique_ptr<ThisExpression> createThisExpression(const SourceLocation& loc);
    static unique_ptr<SuperExpression> createSuperExpression(const SourceLocation& loc);
    static unique_ptr<NewExpression> createNewExpression(unique_ptr<Expression> constructor, std::vector<unique_ptr<Expression>> arguments, const SourceLocation& loc);
    static unique_ptr<BinaryExpression> createBinaryExpression(unique_ptr<Expression> left, BinaryExpression::Operator op, unique_ptr<Expression> right, const SourceLocation& loc);
    static unique_ptr<UnaryExpression> createUnaryExpression(UnaryExpression::Operator op, unique_ptr<Expression> operand, const SourceLocation& loc, bool isPrefix = true);
    static unique_ptr<AssignmentExpression> createAssignmentExpression(unique_ptr<Expression> left, AssignmentExpression::Operator op, unique_ptr<Expression> right, const SourceLocation& loc);
    static unique_ptr<ConditionalExpression> createConditionalExpression(unique_ptr<Expression> condition, unique_ptr<Expression> thenExpr, unique_ptr<Expression> elseExpr, const SourceLocation& loc);
    static unique_ptr<CallExpression> createCallExpression(unique_ptr<Expression> callee, std::vector<unique_ptr<Expression>> arguments, const SourceLocation& loc);
    static unique_ptr<ArrayLiteral> createArrayLiteral(std::vector<unique_ptr<Expression>> elements, const SourceLocation& loc);
    static unique_ptr<IndexExpression> createIndexExpression(unique_ptr<Expression> object, unique_ptr<Expression> index, const SourceLocation& loc);
    static unique_ptr<ObjectLiteral> createObjectLiteral(std::vector<ObjectLiteral::Property> properties, const SourceLocation& loc);
    static unique_ptr<PropertyAccess> createPropertyAccess(unique_ptr<Expression> object, const String& property, const SourceLocation& loc, bool computed = false);
    static unique_ptr<ArrowFunction> createArrowFunction(std::vector<unique_ptr<Parameter>> parameters, unique_ptr<Expression> body, shared_ptr<Type> returnType, const SourceLocation& loc);
    static unique_ptr<FunctionExpression> createFunctionExpression(const String& name, std::vector<unique_ptr<Parameter>> parameters, unique_ptr<BlockStatement> body, shared_ptr<Type> returnType, const SourceLocation& loc);
    static unique_ptr<MoveExpression> createMoveExpression(unique_ptr<Expression> expression, const SourceLocation& loc);
    
    // Statement factories
    static unique_ptr<ExpressionStatement> createExpressionStatement(unique_ptr<Expression> expression, const SourceLocation& loc);
    static unique_ptr<BlockStatement> createBlockStatement(std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc);
    static unique_ptr<ReturnStatement> createReturnStatement(unique_ptr<Expression> expression, const SourceLocation& loc);
    static unique_ptr<IfStatement> createIfStatement(unique_ptr<Expression> condition, unique_ptr<Statement> thenStmt, unique_ptr<Statement> elseStmt, const SourceLocation& loc);
    static unique_ptr<WhileStatement> createWhileStatement(unique_ptr<Expression> condition, unique_ptr<Statement> body, const SourceLocation& loc);
    static unique_ptr<DoWhileStatement> createDoWhileStatement(unique_ptr<Statement> body, unique_ptr<Expression> condition, const SourceLocation& loc);
    static unique_ptr<ForStatement> createForStatement(unique_ptr<Statement> initializer, unique_ptr<Expression> condition, unique_ptr<Expression> increment, unique_ptr<Statement> body, const SourceLocation& loc);
    static unique_ptr<ForOfStatement> createForOfStatement(unique_ptr<Expression> variable, unique_ptr<Expression> iterable, unique_ptr<Statement> body, const SourceLocation& loc);
    static unique_ptr<SwitchStatement> createSwitchStatement(unique_ptr<Expression> expression, std::vector<unique_ptr<CaseClause>> cases, unique_ptr<CaseClause> defaultCase, const SourceLocation& loc);
    static unique_ptr<CaseClause> createCaseClause(unique_ptr<Expression> expression, std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc);
    static unique_ptr<BreakStatement> createBreakStatement(const SourceLocation& loc);
    static unique_ptr<ContinueStatement> createContinueStatement(const SourceLocation& loc);
    static unique_ptr<TryStatement> createTryStatement(unique_ptr<Statement> tryBlock, unique_ptr<CatchClause> catchClause, unique_ptr<Statement> finallyBlock, const SourceLocation& loc);
    static unique_ptr<CatchClause> createCatchClause(const String& parameter, unique_ptr<Statement> body, const SourceLocation& loc);
    static unique_ptr<ThrowStatement> createThrowStatement(unique_ptr<Expression> expression, const SourceLocation& loc);
    
    // Declaration factories
    static unique_ptr<VariableDeclaration> createVariableDeclaration(VariableDeclaration::Kind kind, const String& name, unique_ptr<Expression> initializer, shared_ptr<Type> typeAnnotation, const SourceLocation& loc);
    static unique_ptr<FunctionDeclaration> createFunctionDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, std::vector<unique_ptr<Parameter>> parameters, shared_ptr<Type> returnType, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool async = false, bool generator = false);
    static unique_ptr<ClassDeclaration> createClassDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, unique_ptr<Expression> baseClass, std::vector<unique_ptr<Statement>> members, const SourceLocation& loc, bool abstract = false);
    static unique_ptr<InterfaceDeclaration> createInterfaceDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, std::vector<unique_ptr<Expression>> extends, std::vector<unique_ptr<Statement>> members, const SourceLocation& loc);
    static unique_ptr<EnumDeclaration> createEnumDeclaration(const String& name, std::vector<unique_ptr<EnumMember>> members, const SourceLocation& loc, EnumDeclaration::Kind kind = EnumDeclaration::Kind::Numeric);
    static unique_ptr<TypeAliasDeclaration> createTypeAliasDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, shared_ptr<Type> type, const SourceLocation& loc);
    static unique_ptr<ImportDeclaration> createImportDeclaration(const String& moduleSpecifier, std::vector<unique_ptr<ImportClause>> importClauses, const SourceLocation& loc);
    static unique_ptr<ExportDeclaration> createExportDeclaration(const ExportClause& clause, const SourceLocation& loc);
    static unique_ptr<PropertyDeclaration> createPropertyDeclaration(const String& name, shared_ptr<Type> typeAnnotation, unique_ptr<Expression> initializer, const SourceLocation& loc, bool readonly = false, bool static_ = false, bool public_ = true, bool private_ = false, bool protected_ = false);
    static unique_ptr<MethodDeclaration> createMethodDeclaration(const String& name, std::vector<unique_ptr<Parameter>> parameters, shared_ptr<Type> returnType, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool async = false, bool generator = false, bool static_ = false, bool public_ = true, bool private_ = false, bool protected_ = false);
    static unique_ptr<ConstructorDeclaration> createConstructorDeclaration(std::vector<unique_ptr<Parameter>> parameters, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool public_ = true, bool private_ = false, bool protected_ = false);
    static unique_ptr<DestructorDeclaration> createDestructorDeclaration(unique_ptr<BlockStatement> body, const SourceLocation& loc, bool public_ = true, bool private_ = false, bool protected_ = false);
    
    // Module factory
    static unique_ptr<Module> createModule(const String& filename, std::vector<unique_ptr<Statement>> statements);
    
    // Destructuring factories
    static unique_ptr<ArrayDestructuringPattern> createArrayDestructuringPattern(std::vector<unique_ptr<DestructuringPattern>> elements, const SourceLocation& loc);
    static unique_ptr<ObjectDestructuringPattern> createObjectDestructuringPattern(std::vector<ObjectDestructuringPattern::Property> properties, const SourceLocation& loc);
    static unique_ptr<IdentifierPattern> createIdentifierPattern(const String& name, const SourceLocation& loc);
    static unique_ptr<DestructuringAssignment> createDestructuringAssignment(unique_ptr<DestructuringPattern> pattern, unique_ptr<Expression> expression, const SourceLocation& loc);
};

// AST Utilities
class ASTUtils {
public:
    // Node type checking
    static bool isExpression(const ASTNode& node);
    static bool isStatement(const ASTNode& node);
    static bool isDeclaration(const ASTNode& node);
    static bool isLiteral(const Expression& expr);
    static bool isConstant(const Expression& expr);
    
    // Node traversal
    static std::vector<ASTNode*> getAllNodes(ASTNode& root);
    static std::vector<Expression*> getAllExpressions(ASTNode& root);
    static std::vector<Statement*> getAllStatements(ASTNode& root);
    static std::vector<Declaration*> getAllDeclarations(ASTNode& root);
    
    // Node analysis
    static bool hasSideEffects(const Expression& expr);
    static bool isPure(const Expression& expr);
    static bool isReachable(const Statement& stmt);
    
    // String conversion helpers
    static String operatorToString(BinaryExpression::Operator op);
    static String operatorToString(UnaryExpression::Operator op);
    static String operatorToString(AssignmentExpression::Operator op);
    static String kindToString(VariableDeclaration::Kind kind);
    static String kindToString(EnumDeclaration::Kind kind);
};

} // namespace ast
} // namespace tsc