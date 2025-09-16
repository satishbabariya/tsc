/**
 * @file AST.cpp
 * @brief Implementation of Abstract Syntax Tree nodes
 * @author TSC Compiler Team
 * @version 1.0
 * 
 * This file provides the implementation for all AST node classes,
 * including visitor pattern support and utility methods.
 */

#include "tsc/AST.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/semantic/SymbolTable.h"

#include <sstream>
#include <algorithm>

namespace tsc {

// ============================================================================
// LITERAL EXPRESSIONS IMPLEMENTATION
// ============================================================================

// NumericLiteral
void NumericLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String NumericLiteral::toString() const {
    return std::to_string(value_);
}

// StringLiteral
void StringLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String StringLiteral::toString() const {
    return "\"" + value_ + "\"";
}

// TemplateLiteral
void TemplateLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TemplateLiteral::toString() const {
    std::ostringstream ss;
    ss << "`";
    for (const auto& element : elements_) {
        if (element.isExpressionElement()) {
            ss << "${" << element.getExpression()->toString() << "}";
        } else {
            ss << element.getText();
        }
    }
    ss << "`";
    return ss.str();
}

bool TemplateLiteral::isConstant() const {
    return std::all_of(elements_.begin(), elements_.end(),
        [](const Element& element) {
            return !element.isExpressionElement() || 
                   element.getExpression()->isConstant();
        });
}

// BooleanLiteral
void BooleanLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BooleanLiteral::toString() const {
    return value_ ? "true" : "false";
}

// NullLiteral
void NullLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String NullLiteral::toString() const {
    return "null";
}

// ============================================================================
// IDENTIFIER AND PROPERTY ACCESS IMPLEMENTATION
// ============================================================================

// Identifier
void Identifier::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String Identifier::toString() const {
    return name_;
}

// ThisExpression
void ThisExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ThisExpression::toString() const {
    return "this";
}

// SuperExpression
void SuperExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String SuperExpression::toString() const {
    return "super";
}

// PropertyAccess
void PropertyAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String PropertyAccess::toString() const {
    return object_->toString() + "." + property_;
}

// IndexExpression
void IndexExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String IndexExpression::toString() const {
    return object_->toString() + "[" + index_->toString() + "]";
}

// ============================================================================
// OBJECT AND ARRAY LITERALS IMPLEMENTATION
// ============================================================================

// ObjectLiteral
void ObjectLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ObjectLiteral::toString() const {
    std::ostringstream ss;
    ss << "{";
    for (Size i = 0; i < properties_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& prop = properties_[i];
        if (prop.isComputed) {
            ss << "[" << prop.name << "]";
        } else {
            ss << prop.name;
        }
        ss << ": " << prop.value->toString();
    }
    ss << "}";
    return ss.str();
}

// ArrayLiteral
void ArrayLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrayLiteral::toString() const {
    std::ostringstream ss;
    ss << "[";
    for (Size i = 0; i < elements_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << elements_[i]->toString();
    }
    ss << "]";
    return ss.str();
}

// ============================================================================
// BINARY AND UNARY EXPRESSIONS IMPLEMENTATION
// ============================================================================

// BinaryExpression
void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BinaryExpression::toString() const {
    return left_->toString() + " " + toString(operator_) + " " + right_->toString();
}

// UnaryExpression
void UnaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String UnaryExpression::toString() const {
    return toString(operator_) + operand_->toString();
}

// AssignmentExpression
void AssignmentExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String AssignmentExpression::toString() const {
    return left_->toString() + " " + toString(operator_) + " " + right_->toString();
}

// ============================================================================
// FUNCTION CALLS IMPLEMENTATION
// ============================================================================

// CallExpression
void CallExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CallExpression::toString() const {
    std::ostringstream ss;
    ss << callee_->toString() << "(";
    for (Size i = 0; i < arguments_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << arguments_[i]->toString();
    }
    ss << ")";
    return ss.str();
}

// NewExpression
void NewExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String NewExpression::toString() const {
    std::ostringstream ss;
    ss << "new " << constructor_->toString() << "(";
    for (Size i = 0; i < arguments_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << arguments_[i]->toString();
    }
    ss << ")";
    return ss.str();
}

// ============================================================================
// CONDITIONAL EXPRESSIONS IMPLEMENTATION
// ============================================================================

// ConditionalExpression
void ConditionalExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ConditionalExpression::toString() const {
    return condition_->toString() + " ? " + 
           trueExpr_->toString() + " : " + 
           falseExpr_->toString();
}

// ============================================================================
// FUNCTION EXPRESSIONS IMPLEMENTATION
// ============================================================================

// ArrowFunction
void ArrowFunction::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrowFunction::toString() const {
    std::ostringstream ss;
    ss << "(";
    for (Size i = 0; i < parameters_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& param = parameters_[i];
        ss << param.name;
        if (param.type) {
            ss << ": " << param.type->toString();
        }
        if (param.defaultValue) {
            ss << " = " << param.defaultValue->toString();
        }
    }
    ss << ") => " << body_->toString();
    return ss.str();
}

// FunctionExpression
void FunctionExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String FunctionExpression::toString() const {
    std::ostringstream ss;
    ss << "function " << name_ << "(";
    for (Size i = 0; i < parameters_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& param = parameters_[i];
        ss << param.name;
        if (param.type) {
            ss << ": " << param.type->toString();
        }
        if (param.defaultValue) {
            ss << " = " << param.defaultValue->toString();
        }
    }
    ss << ")";
    if (returnType_) {
        ss << ": " << returnType_->toString();
    }
    ss << " " << body_->toString();
    return ss.str();
}

// MoveExpression
void MoveExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String MoveExpression::toString() const {
    return "move(" + operand_->toString() + ")";
}

// ============================================================================
// STATEMENTS IMPLEMENTATION
// ============================================================================

// ExpressionStatement
void ExpressionStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ExpressionStatement::toString() const {
    return expression_->toString() + ";";
}

// BlockStatement
void BlockStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BlockStatement::toString() const {
    std::ostringstream ss;
    ss << "{\n";
    for (const auto& stmt : statements_) {
        ss << "  " << stmt->toString() << "\n";
    }
    ss << "}";
    return ss.str();
}

// ============================================================================
// CONTROL FLOW STATEMENTS IMPLEMENTATION
// ============================================================================

// IfStatement
void IfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String IfStatement::toString() const {
    std::ostringstream ss;
    ss << "if (" << condition_->toString() << ") " << thenStmt_->toString();
    if (elseStmt_) {
        ss << " else " << elseStmt_->toString();
    }
    return ss.str();
}

// WhileStatement
void WhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String WhileStatement::toString() const {
    return "while (" + condition_->toString() + ") " + body_->toString();
}

// DoWhileStatement
void DoWhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String DoWhileStatement::toString() const {
    return "do " + body_->toString() + " while (" + condition_->toString() + ")";
}

// ForStatement
void ForStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ForStatement::toString() const {
    std::ostringstream ss;
    ss << "for (";
    if (init_) ss << init_->toString();
    ss << "; ";
    if (condition_) ss << condition_->toString();
    ss << "; ";
    if (update_) ss << update_->toString();
    ss << ") " << body_->toString();
    return ss.str();
}

// ForOfStatement
void ForOfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ForOfStatement::toString() const {
    return "for (" + variable_->toString() + " of " + 
           iterable_->toString() + ") " + body_->toString();
}

// SwitchStatement
void SwitchStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String SwitchStatement::toString() const {
    std::ostringstream ss;
    ss << "switch (" << expression_->toString() << ") {\n";
    for (const auto& caseClause : cases_) {
        ss << "  " << caseClause->toString() << "\n";
    }
    ss << "}";
    return ss.str();
}

// TryStatement
void TryStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TryStatement::toString() const {
    std::ostringstream ss;
    ss << "try " << tryBlock_->toString();
    for (const auto& catchClause : catchClauses_) {
        ss << " " << catchClause->toString();
    }
    if (finallyBlock_) {
        ss << " finally " << finallyBlock_->toString();
    }
    return ss.str();
}

// ============================================================================
// JUMP STATEMENTS IMPLEMENTATION
// ============================================================================

// ReturnStatement
void ReturnStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ReturnStatement::toString() const {
    if (expression_) {
        return "return " + expression_->toString() + ";";
    }
    return "return;";
}

// BreakStatement
void BreakStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BreakStatement::toString() const {
    return "break;";
}

// ContinueStatement
void ContinueStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ContinueStatement::toString() const {
    return "continue;";
}

// ThrowStatement
void ThrowStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ThrowStatement::toString() const {
    return "throw " + expression_->toString() + ";";
}

// ============================================================================
// DECLARATIONS IMPLEMENTATION
// ============================================================================

// VariableDeclaration
void VariableDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String VariableDeclaration::toString() const {
    std::ostringstream ss;
    ss << "let ";
    for (Size i = 0; i < variables_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& var = variables_[i];
        ss << var.name;
        if (var.type) {
            ss << ": " << var.type->toString();
        }
        if (var.initializer) {
            ss << " = " << var.initializer->toString();
        }
    }
    ss << ";";
    return ss.str();
}

String VariableDeclaration::getName() const {
    if (variables_.empty()) return "";
    return variables_[0].name;
}

// FunctionDeclaration
void FunctionDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String FunctionDeclaration::toString() const {
    std::ostringstream ss;
    ss << "function " << name_ << "(";
    for (Size i = 0; i < parameters_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& param = parameters_[i];
        ss << param.name;
        if (param.type) {
            ss << ": " << param.type->toString();
        }
        if (param.defaultValue) {
            ss << " = " << param.defaultValue->toString();
        }
    }
    ss << ")";
    if (returnType_) {
        ss << ": " << returnType_->toString();
    }
    ss << " " << body_->toString();
    return ss.str();
}

// ClassDeclaration
void ClassDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ClassDeclaration::toString() const {
    std::ostringstream ss;
    ss << "class " << name_;
    if (superClass_) {
        ss << " extends " << superClass_->toString();
    }
    ss << " {\n";
    
    for (const auto& prop : properties_) {
        ss << "  " << prop.name;
        if (prop.type) {
            ss << ": " << prop.type->toString();
        }
        if (prop.initializer) {
            ss << " = " << prop.initializer->toString();
        }
        ss << ";\n";
    }
    
    for (const auto& method : methods_) {
        ss << "  " << method->toString() << "\n";
    }
    
    ss << "}";
    return ss.str();
}

// ============================================================================
// ADDITIONAL DECLARATIONS IMPLEMENTATION
// ============================================================================

// TypeParameter
void TypeParameter::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TypeParameter::toString() const {
    std::ostringstream ss;
    ss << name_;
    if (constraint_) {
        ss << " extends " << constraint_->toString();
    }
    if (defaultType_) {
        ss << " = " << defaultType_->toString();
    }
    return ss.str();
}

// InterfaceDeclaration
void InterfaceDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String InterfaceDeclaration::toString() const {
    std::ostringstream ss;
    ss << "interface " << name_;
    if (!extends_.empty()) {
        ss << " extends ";
        for (Size i = 0; i < extends_.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << extends_[i]->toString();
        }
    }
    ss << " {\n";
    
    for (const auto& prop : properties_) {
        ss << "  " << prop->toString() << ";\n";
    }
    
    for (const auto& method : methods_) {
        ss << "  " << method->toString() << ";\n";
    }
    
    ss << "}";
    return ss.str();
}

// EnumDeclaration
void EnumDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String EnumDeclaration::toString() const {
    std::ostringstream ss;
    ss << "enum " << name_ << " {\n";
    for (Size i = 0; i < members_.size(); ++i) {
        if (i > 0) ss << ",\n";
        ss << "  " << members_[i]->toString();
    }
    ss << "\n}";
    return ss.str();
}

// TypeAliasDeclaration
void TypeAliasDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TypeAliasDeclaration::toString() const {
    std::ostringstream ss;
    ss << "type " << name_;
    if (!typeParameters_.empty()) {
        ss << "<";
        for (Size i = 0; i < typeParameters_.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << typeParameters_[i].toString();
        }
        ss << ">";
    }
    ss << " = " << type_->toString();
    return ss.str();
}

// ImportDeclaration
void ImportDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ImportDeclaration::toString() const {
    std::ostringstream ss;
    ss << "import ";
    if (!defaultImport_.empty()) {
        ss << defaultImport_;
    }
    if (!namedImports_.empty()) {
        if (!defaultImport_.empty()) ss << ", ";
        ss << "{ ";
        for (Size i = 0; i < namedImports_.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << namedImports_[i];
        }
        ss << " }";
    }
    if (!namespaceImport_.empty()) {
        if (!defaultImport_.empty() || !namedImports_.empty()) ss << ", ";
        ss << "* as " << namespaceImport_;
    }
    ss << " from \"" << module_ << "\"";
    return ss.str();
}

// ExportDeclaration
void ExportDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ExportDeclaration::toString() const {
    std::ostringstream ss;
    ss << "export ";
    if (!name_.empty()) {
        ss << "{ " << name_ << " }";
    } else if (expression_) {
        ss << expression_->toString();
    }
    return ss.str();
}

// ============================================================================
// ADDITIONAL AST NODE IMPLEMENTATIONS
// ============================================================================

// DestructorDeclaration
void DestructorDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String DestructorDeclaration::toString() const {
    return "~" + name_ + "()";
}

// OptionalPropertyAccess
void OptionalPropertyAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String OptionalPropertyAccess::toString() const {
    return object_->toString() + "?." + property_;
}

// OptionalIndexAccess
void OptionalIndexAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String OptionalIndexAccess::toString() const {
    return object_->toString() + "?[" + index_->toString() + "]";
}

// OptionalCallExpr
void OptionalCallExpr::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String OptionalCallExpr::toString() const {
    std::ostringstream ss;
    ss << callee_->toString() << "?(";
    for (Size i = 0; i < arguments_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << arguments_[i]->toString();
    }
    ss << ")";
    return ss.str();
}

// SpreadElement
void SpreadElement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String SpreadElement::toString() const {
    return "..." + expression_->toString();
}

// DestructuringPattern
void DestructuringPattern::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// ArrayDestructuringPattern
void ArrayDestructuringPattern::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrayDestructuringPattern::toString() const {
    std::ostringstream ss;
    ss << "[";
    for (Size i = 0; i < elements_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << elements_[i]->toString();
    }
    ss << "]";
    return ss.str();
}

// ObjectDestructuringPattern
void ObjectDestructuringPattern::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ObjectDestructuringPattern::toString() const {
    std::ostringstream ss;
    ss << "{";
    for (Size i = 0; i < properties_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& prop = properties_[i];
        if (prop.isShorthand) {
            ss << prop.name;
        } else {
            ss << prop.name << ": " << prop.value->toString();
        }
    }
    ss << "}";
    return ss.str();
}

// IdentifierPattern
void IdentifierPattern::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String IdentifierPattern::toString() const {
    return name_;
}

// DestructuringAssignment
void DestructuringAssignment::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String DestructuringAssignment::toString() const {
    return left_->toString() + " = " + right_->toString();
}

// ============================================================================
// STANDALONE CLASS IMPLEMENTATIONS (formerly nested structs)
// ============================================================================

// CaseClause
void CaseClause::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CaseClause::toString() const {
    std::ostringstream ss;
    if (isDefault_) {
        ss << "default:";
    } else {
        ss << "case " << expression_->toString() << ":";
    }
    for (const auto& stmt : statements_) {
        ss << "\n  " << stmt->toString();
    }
    return ss.str();
}

// CatchClause
void CatchClause::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CatchClause::toString() const {
    return "catch (" + parameter_ + ") " + body_->toString();
}

// MethodDeclaration
void MethodDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String MethodDeclaration::toString() const {
    std::ostringstream ss;
    ss << name_ << "(";
    for (Size i = 0; i < parameters_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& param = parameters_[i];
        ss << param.name;
        if (param.type) {
            ss << ": " << param.type->toString();
        }
        if (param.defaultValue) {
            ss << " = " << param.defaultValue->toString();
        }
    }
    ss << ")";
    if (returnType_) {
        ss << ": " << returnType_->toString();
    }
    return ss.str();
}

// EnumMember
void EnumMember::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String EnumMember::toString() const {
    if (value_) {
        return name_ + " = " + value_->toString();
    } else {
        return name_;
    }
}

// ============================================================================
// MODULE IMPLEMENTATION
// ============================================================================

// Module
void Module::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

SourceLocation Module::getLocation() const {
    if (statements_.empty()) {
        return SourceLocation(filename_, 1, 1);
    }
    return statements_[0]->getLocation();
}

String Module::toString() const {
    std::ostringstream ss;
    for (const auto& stmt : statements_) {
        ss << stmt->toString() << "\n";
    }
    return ss.str();
}

} // namespace tsc