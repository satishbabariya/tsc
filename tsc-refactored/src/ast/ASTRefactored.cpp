#include "tsc/ast/ASTRefactored.h"
#include "tsc/ast/core/ASTCore.h"
#include "tsc/ast/expressions/ExpressionNodes.h"
#include "tsc/ast/statements/StatementNodes.h"
#include "tsc/ast/declarations/DeclarationNodes.h"

namespace tsc {
namespace ast {

// Module implementation
void Module::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String Module::toString() const {
    String result = "Module: " + filename_ + "\n";
    for (const auto& stmt : statements_) {
        result += stmt->toString() + "\n";
    }
    return result;
}

const std::vector<unique_ptr<Declaration>>& Module::getDeclarations() const {
    if (declarations_.empty()) {
        // Extract declarations from statements
        for (const auto& stmt : statements_) {
            if (auto decl = dynamic_cast<Declaration*>(stmt.get())) {
                declarations_.push_back(std::unique_ptr<Declaration>(decl));
            }
        }
    }
    return declarations_;
}

// DestructuringPattern implementations
void ArrayDestructuringPattern::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrayDestructuringPattern::toString() const {
    String result = "[";
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) result += ", ";
        result += elements_[i]->toString();
    }
    result += "]";
    return result;
}

void ObjectDestructuringPattern::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ObjectDestructuringPattern::toString() const {
    String result = "{";
    for (size_t i = 0; i < properties_.size(); ++i) {
        if (i > 0) result += ", ";
        const auto& prop = properties_[i];
        if (prop.computed) {
            result += "[" + prop.name + "]";
        } else {
            result += prop.name;
        }
        if (!prop.shorthand) {
            result += ": " + prop.pattern->toString();
        }
    }
    result += "}";
    return result;
}

void IdentifierPattern::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String IdentifierPattern::toString() const {
    return name_;
}

void DestructuringAssignment::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String DestructuringAssignment::toString() const {
    return pattern_->toString() + " = " + expression_->toString();
}

// ASTFactory implementations
unique_ptr<NumericLiteral> ASTFactory::createNumericLiteral(double value, const SourceLocation& loc) {
    return std::make_unique<NumericLiteral>(value, loc);
}

unique_ptr<StringLiteral> ASTFactory::createStringLiteral(const String& value, const SourceLocation& loc) {
    return std::make_unique<StringLiteral>(value, loc);
}

unique_ptr<BooleanLiteral> ASTFactory::createBooleanLiteral(bool value, const SourceLocation& loc) {
    return std::make_unique<BooleanLiteral>(value, loc);
}

unique_ptr<NullLiteral> ASTFactory::createNullLiteral(const SourceLocation& loc) {
    return std::make_unique<NullLiteral>(loc);
}

unique_ptr<Identifier> ASTFactory::createIdentifier(const String& name, const SourceLocation& loc) {
    return std::make_unique<Identifier>(name, loc);
}

unique_ptr<ThisExpression> ASTFactory::createThisExpression(const SourceLocation& loc) {
    return std::make_unique<ThisExpression>(loc);
}

unique_ptr<SuperExpression> ASTFactory::createSuperExpression(const SourceLocation& loc) {
    return std::make_unique<SuperExpression>(loc);
}

unique_ptr<NewExpression> ASTFactory::createNewExpression(unique_ptr<Expression> constructor, std::vector<unique_ptr<Expression>> arguments, const SourceLocation& loc) {
    return std::make_unique<NewExpression>(std::move(constructor), std::move(arguments), loc);
}

unique_ptr<BinaryExpression> ASTFactory::createBinaryExpression(unique_ptr<Expression> left, BinaryExpression::Operator op, unique_ptr<Expression> right, const SourceLocation& loc) {
    return std::make_unique<BinaryExpression>(std::move(left), op, std::move(right), loc);
}

unique_ptr<UnaryExpression> ASTFactory::createUnaryExpression(UnaryExpression::Operator op, unique_ptr<Expression> operand, const SourceLocation& loc, bool isPrefix) {
    return std::make_unique<UnaryExpression>(op, std::move(operand), loc, isPrefix);
}

unique_ptr<AssignmentExpression> ASTFactory::createAssignmentExpression(unique_ptr<Expression> left, AssignmentExpression::Operator op, unique_ptr<Expression> right, const SourceLocation& loc) {
    return std::make_unique<AssignmentExpression>(std::move(left), op, std::move(right), loc);
}

unique_ptr<ConditionalExpression> ASTFactory::createConditionalExpression(unique_ptr<Expression> condition, unique_ptr<Expression> thenExpr, unique_ptr<Expression> elseExpr, const SourceLocation& loc) {
    return std::make_unique<ConditionalExpression>(std::move(condition), std::move(thenExpr), std::move(elseExpr), loc);
}

unique_ptr<CallExpression> ASTFactory::createCallExpression(unique_ptr<Expression> callee, std::vector<unique_ptr<Expression>> arguments, const SourceLocation& loc) {
    return std::make_unique<CallExpression>(std::move(callee), std::move(arguments), loc);
}

unique_ptr<ArrayLiteral> ASTFactory::createArrayLiteral(std::vector<unique_ptr<Expression>> elements, const SourceLocation& loc) {
    return std::make_unique<ArrayLiteral>(std::move(elements), loc);
}

unique_ptr<IndexExpression> ASTFactory::createIndexExpression(unique_ptr<Expression> object, unique_ptr<Expression> index, const SourceLocation& loc) {
    return std::make_unique<IndexExpression>(std::move(object), std::move(index), loc);
}

unique_ptr<ObjectLiteral> ASTFactory::createObjectLiteral(std::vector<ObjectLiteral::Property> properties, const SourceLocation& loc) {
    return std::make_unique<ObjectLiteral>(std::move(properties), loc);
}

unique_ptr<PropertyAccess> ASTFactory::createPropertyAccess(unique_ptr<Expression> object, const String& property, const SourceLocation& loc, bool computed) {
    return std::make_unique<PropertyAccess>(std::move(object), property, loc, computed);
}

unique_ptr<ArrowFunction> ASTFactory::createArrowFunction(std::vector<unique_ptr<Parameter>> parameters, unique_ptr<Expression> body, shared_ptr<Type> returnType, const SourceLocation& loc) {
    return std::make_unique<ArrowFunction>(std::move(parameters), std::move(body), returnType, loc);
}

unique_ptr<FunctionExpression> ASTFactory::createFunctionExpression(const String& name, std::vector<unique_ptr<Parameter>> parameters, unique_ptr<BlockStatement> body, shared_ptr<Type> returnType, const SourceLocation& loc) {
    return std::make_unique<FunctionExpression>(name, std::move(parameters), std::move(body), returnType, loc);
}

unique_ptr<MoveExpression> ASTFactory::createMoveExpression(unique_ptr<Expression> expression, const SourceLocation& loc) {
    return std::make_unique<MoveExpression>(std::move(expression), loc);
}

// Statement factories
unique_ptr<ExpressionStatement> ASTFactory::createExpressionStatement(unique_ptr<Expression> expression, const SourceLocation& loc) {
    return std::make_unique<ExpressionStatement>(std::move(expression), loc);
}

unique_ptr<BlockStatement> ASTFactory::createBlockStatement(std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc) {
    return std::make_unique<BlockStatement>(std::move(statements), loc);
}

unique_ptr<ReturnStatement> ASTFactory::createReturnStatement(unique_ptr<Expression> expression, const SourceLocation& loc) {
    return std::make_unique<ReturnStatement>(std::move(expression), loc);
}

unique_ptr<IfStatement> ASTFactory::createIfStatement(unique_ptr<Expression> condition, unique_ptr<Statement> thenStmt, unique_ptr<Statement> elseStmt, const SourceLocation& loc) {
    return std::make_unique<IfStatement>(std::move(condition), std::move(thenStmt), std::move(elseStmt), loc);
}

unique_ptr<WhileStatement> ASTFactory::createWhileStatement(unique_ptr<Expression> condition, unique_ptr<Statement> body, const SourceLocation& loc) {
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body), loc);
}

unique_ptr<DoWhileStatement> ASTFactory::createDoWhileStatement(unique_ptr<Statement> body, unique_ptr<Expression> condition, const SourceLocation& loc) {
    return std::make_unique<DoWhileStatement>(std::move(body), std::move(condition), loc);
}

unique_ptr<ForStatement> ASTFactory::createForStatement(unique_ptr<Statement> initializer, unique_ptr<Expression> condition, unique_ptr<Expression> increment, unique_ptr<Statement> body, const SourceLocation& loc) {
    return std::make_unique<ForStatement>(std::move(initializer), std::move(condition), std::move(increment), std::move(body), loc);
}

unique_ptr<ForOfStatement> ASTFactory::createForOfStatement(unique_ptr<Expression> variable, unique_ptr<Expression> iterable, unique_ptr<Statement> body, const SourceLocation& loc) {
    return std::make_unique<ForOfStatement>(std::move(variable), std::move(iterable), std::move(body), loc);
}

unique_ptr<SwitchStatement> ASTFactory::createSwitchStatement(unique_ptr<Expression> expression, std::vector<unique_ptr<CaseClause>> cases, unique_ptr<CaseClause> defaultCase, const SourceLocation& loc) {
    return std::make_unique<SwitchStatement>(std::move(expression), std::move(cases), std::move(defaultCase), loc);
}

unique_ptr<CaseClause> ASTFactory::createCaseClause(unique_ptr<Expression> expression, std::vector<unique_ptr<Statement>> statements, const SourceLocation& loc) {
    return std::make_unique<CaseClause>(std::move(expression), std::move(statements), loc);
}

unique_ptr<BreakStatement> ASTFactory::createBreakStatement(const SourceLocation& loc) {
    return std::make_unique<BreakStatement>(loc);
}

unique_ptr<ContinueStatement> ASTFactory::createContinueStatement(const SourceLocation& loc) {
    return std::make_unique<ContinueStatement>(loc);
}

unique_ptr<TryStatement> ASTFactory::createTryStatement(unique_ptr<Statement> tryBlock, unique_ptr<CatchClause> catchClause, unique_ptr<Statement> finallyBlock, const SourceLocation& loc) {
    return std::make_unique<TryStatement>(std::move(tryBlock), std::move(catchClause), std::move(finallyBlock), loc);
}

unique_ptr<CatchClause> ASTFactory::createCatchClause(const String& parameter, unique_ptr<Statement> body, const SourceLocation& loc) {
    return std::make_unique<CatchClause>(parameter, std::move(body), loc);
}

unique_ptr<ThrowStatement> ASTFactory::createThrowStatement(unique_ptr<Expression> expression, const SourceLocation& loc) {
    return std::make_unique<ThrowStatement>(std::move(expression), loc);
}

// Declaration factories
unique_ptr<VariableDeclaration> ASTFactory::createVariableDeclaration(VariableDeclaration::Kind kind, const String& name, unique_ptr<Expression> initializer, shared_ptr<Type> typeAnnotation, const SourceLocation& loc) {
    return std::make_unique<VariableDeclaration>(kind, name, std::move(initializer), typeAnnotation, loc);
}

unique_ptr<FunctionDeclaration> ASTFactory::createFunctionDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, std::vector<unique_ptr<Parameter>> parameters, shared_ptr<Type> returnType, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool async, bool generator) {
    return std::make_unique<FunctionDeclaration>(name, std::move(typeParameters), std::move(parameters), returnType, std::move(body), loc, async, generator);
}

unique_ptr<ClassDeclaration> ASTFactory::createClassDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, unique_ptr<Expression> baseClass, std::vector<unique_ptr<Statement>> members, const SourceLocation& loc, bool abstract) {
    return std::make_unique<ClassDeclaration>(name, std::move(typeParameters), std::move(baseClass), std::move(members), loc, abstract);
}

unique_ptr<InterfaceDeclaration> ASTFactory::createInterfaceDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, std::vector<unique_ptr<Expression>> extends, std::vector<unique_ptr<Statement>> members, const SourceLocation& loc) {
    return std::make_unique<InterfaceDeclaration>(name, std::move(typeParameters), std::move(extends), std::move(members), loc);
}

unique_ptr<EnumDeclaration> ASTFactory::createEnumDeclaration(const String& name, std::vector<unique_ptr<EnumMember>> members, const SourceLocation& loc, EnumDeclaration::Kind kind) {
    return std::make_unique<EnumDeclaration>(name, std::move(members), loc, kind);
}

unique_ptr<TypeAliasDeclaration> ASTFactory::createTypeAliasDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, shared_ptr<Type> type, const SourceLocation& loc) {
    return std::make_unique<TypeAliasDeclaration>(name, std::move(typeParameters), type, loc);
}

unique_ptr<ImportDeclaration> ASTFactory::createImportDeclaration(const String& moduleSpecifier, std::vector<unique_ptr<ImportClause>> importClauses, const SourceLocation& loc) {
    return std::make_unique<ImportDeclaration>(moduleSpecifier, std::move(importClauses), loc);
}

unique_ptr<ExportDeclaration> ASTFactory::createExportDeclaration(const ExportClause& clause, const SourceLocation& loc) {
    return std::make_unique<ExportDeclaration>(clause, loc);
}

unique_ptr<PropertyDeclaration> ASTFactory::createPropertyDeclaration(const String& name, shared_ptr<Type> typeAnnotation, unique_ptr<Expression> initializer, const SourceLocation& loc, bool readonly, bool static_, bool public_, bool private_, bool protected_) {
    return std::make_unique<PropertyDeclaration>(name, typeAnnotation, std::move(initializer), loc, readonly, static_, public_, private_, protected_);
}

unique_ptr<MethodDeclaration> ASTFactory::createMethodDeclaration(const String& name, std::vector<unique_ptr<Parameter>> parameters, shared_ptr<Type> returnType, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool async, bool generator, bool static_, bool public_, bool private_, bool protected_) {
    return std::make_unique<MethodDeclaration>(name, std::move(parameters), returnType, std::move(body), loc, async, generator, static_, public_, private_, protected_);
}

unique_ptr<ConstructorDeclaration> ASTFactory::createConstructorDeclaration(std::vector<unique_ptr<Parameter>> parameters, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool public_, bool private_, bool protected_) {
    return std::make_unique<ConstructorDeclaration>(std::move(parameters), std::move(body), loc, public_, private_, protected_);
}

unique_ptr<DestructorDeclaration> ASTFactory::createDestructorDeclaration(unique_ptr<BlockStatement> body, const SourceLocation& loc, bool public_, bool private_, bool protected_) {
    return std::make_unique<DestructorDeclaration>(std::move(body), loc, public_, private_, protected_);
}

// Module factory
unique_ptr<Module> ASTFactory::createModule(const String& filename, std::vector<unique_ptr<Statement>> statements) {
    return std::make_unique<Module>(filename, std::move(statements));
}

// Destructuring factories
unique_ptr<ArrayDestructuringPattern> ASTFactory::createArrayDestructuringPattern(std::vector<unique_ptr<DestructuringPattern>> elements, const SourceLocation& loc) {
    return std::make_unique<ArrayDestructuringPattern>(std::move(elements), loc);
}

unique_ptr<ObjectDestructuringPattern> ASTFactory::createObjectDestructuringPattern(std::vector<ObjectDestructuringPattern::Property> properties, const SourceLocation& loc) {
    return std::make_unique<ObjectDestructuringPattern>(std::move(properties), loc);
}

unique_ptr<IdentifierPattern> ASTFactory::createIdentifierPattern(const String& name, const SourceLocation& loc) {
    return std::make_unique<IdentifierPattern>(name, loc);
}

unique_ptr<DestructuringAssignment> ASTFactory::createDestructuringAssignment(unique_ptr<DestructuringPattern> pattern, unique_ptr<Expression> expression, const SourceLocation& loc) {
    return std::make_unique<DestructuringAssignment>(std::move(pattern), std::move(expression), loc);
}

// ASTUtils implementations
bool ASTUtils::isExpression(const ASTNode& node) {
    return dynamic_cast<const Expression*>(&node) != nullptr;
}

bool ASTUtils::isStatement(const ASTNode& node) {
    return dynamic_cast<const Statement*>(&node) != nullptr;
}

bool ASTUtils::isDeclaration(const ASTNode& node) {
    return dynamic_cast<const Declaration*>(&node) != nullptr;
}

bool ASTUtils::isLiteral(const Expression& expr) {
    return dynamic_cast<const NumericLiteral*>(&expr) != nullptr ||
           dynamic_cast<const StringLiteral*>(&expr) != nullptr ||
           dynamic_cast<const BooleanLiteral*>(&expr) != nullptr ||
           dynamic_cast<const NullLiteral*>(&expr) != nullptr;
}

bool ASTUtils::isConstant(const Expression& expr) {
    return expr.isConstant();
}

String ASTUtils::operatorToString(BinaryExpression::Operator op) {
    switch (op) {
        case BinaryExpression::Operator::Plus: return "+";
        case BinaryExpression::Operator::Minus: return "-";
        case BinaryExpression::Operator::Multiply: return "*";
        case BinaryExpression::Operator::Divide: return "/";
        case BinaryExpression::Operator::Modulo: return "%";
        case BinaryExpression::Operator::Exponentiation: return "**";
        case BinaryExpression::Operator::Equal: return "==";
        case BinaryExpression::Operator::NotEqual: return "!=";
        case BinaryExpression::Operator::Less: return "<";
        case BinaryExpression::Operator::Greater: return ">";
        case BinaryExpression::Operator::LessEqual: return "<=";
        case BinaryExpression::Operator::GreaterEqual: return ">=";
        case BinaryExpression::Operator::LogicalAnd: return "&&";
        case BinaryExpression::Operator::LogicalOr: return "||";
        case BinaryExpression::Operator::NullishCoalescing: return "??";
        case BinaryExpression::Operator::BitwiseAnd: return "&";
        case BinaryExpression::Operator::BitwiseOr: return "|";
        case BinaryExpression::Operator::BitwiseXor: return "^";
        case BinaryExpression::Operator::LeftShift: return "<<";
        case BinaryExpression::Operator::RightShift: return ">>";
        case BinaryExpression::Operator::UnsignedRightShift: return ">>>";
    }
    return "unknown";
}

String ASTUtils::operatorToString(UnaryExpression::Operator op) {
    switch (op) {
        case UnaryExpression::Operator::UnaryPlus: return "+";
        case UnaryExpression::Operator::UnaryMinus: return "-";
        case UnaryExpression::Operator::LogicalNot: return "!";
        case UnaryExpression::Operator::BitwiseNot: return "~";
        case UnaryExpression::Operator::PrefixIncrement: return "++";
        case UnaryExpression::Operator::PrefixDecrement: return "--";
        case UnaryExpression::Operator::PostfixIncrement: return "++";
        case UnaryExpression::Operator::PostfixDecrement: return "--";
        case UnaryExpression::Operator::TypeOf: return "typeof";
        case UnaryExpression::Operator::Void: return "void";
        case UnaryExpression::Operator::Delete: return "delete";
    }
    return "unknown";
}

String ASTUtils::operatorToString(AssignmentExpression::Operator op) {
    switch (op) {
        case AssignmentExpression::Operator::Assign: return "=";
        case AssignmentExpression::Operator::PlusAssign: return "+=";
        case AssignmentExpression::Operator::MinusAssign: return "-=";
        case AssignmentExpression::Operator::MultiplyAssign: return "*=";
        case AssignmentExpression::Operator::DivideAssign: return "/=";
        case AssignmentExpression::Operator::ModuloAssign: return "%=";
        case AssignmentExpression::Operator::ExponentiationAssign: return "**=";
        case AssignmentExpression::Operator::LeftShiftAssign: return "<<=";
        case AssignmentExpression::Operator::RightShiftAssign: return ">>=";
        case AssignmentExpression::Operator::UnsignedRightShiftAssign: return ">>>=";
        case AssignmentExpression::Operator::BitwiseAndAssign: return "&=";
        case AssignmentExpression::Operator::BitwiseOrAssign: return "|=";
        case AssignmentExpression::Operator::BitwiseXorAssign: return "^=";
    }
    return "unknown";
}

String ASTUtils::kindToString(VariableDeclaration::Kind kind) {
    switch (kind) {
        case VariableDeclaration::Kind::Var: return "var";
        case VariableDeclaration::Kind::Let: return "let";
        case VariableDeclaration::Kind::Const: return "const";
    }
    return "unknown";
}

String ASTUtils::kindToString(EnumDeclaration::Kind kind) {
    switch (kind) {
        case EnumDeclaration::Kind::Numeric: return "numeric";
        case EnumDeclaration::Kind::String: return "string";
        case EnumDeclaration::Kind::Computed: return "computed";
    }
    return "unknown";
}

} // namespace ast
} // namespace tsc