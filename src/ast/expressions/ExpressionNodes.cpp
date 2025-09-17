#include "tsc/ast/expressions/ExpressionNodes.h"
#include "tsc/ast/core/ASTCore.h"
#include <sstream>

namespace tsc {
namespace ast {
namespace expressions {

// NumericLiteral implementation
void NumericLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String NumericLiteral::toString() const {
    return std::to_string(value_);
}

// StringLiteral implementation
void StringLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String StringLiteral::toString() const {
    return "\"" + value_ + "\"";
}

// TemplateLiteral implementation
void TemplateLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TemplateLiteral::toString() const {
    std::stringstream ss;
    ss << "`";
    for (const auto& element: elements_) {
        if (element.isExpression()) {
            ss << "${" << element.getExpression()->toString() << "}";
        } else {
            ss << element.getText();
        }
    }
    ss << "`";
    return ss.str();
}

// BooleanLiteral implementation
void BooleanLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BooleanLiteral::toString() const {
    return value_ ? "true" : "false";
}

// NullLiteral implementation
void NullLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String NullLiteral::toString() const {
    return "null";
}

// Identifier implementation
void Identifier::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String Identifier::toString() const {
    return name_;
}

// ThisExpression implementation
void ThisExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ThisExpression::toString() const {
    return "this";
}

// SuperExpression implementation
void SuperExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String SuperExpression::toString() const {
    return "super";
}

// NewExpression implementation
void NewExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String NewExpression::toString() const {
    String result = "new " + constructor_->toString() + "(";
    for (size_t i = 0; i < arguments_.size(); ++i) {
        if (i > 0) result += ", ";
        result += arguments_[i]->toString();
    }
    result += ")";
    return result;
}

// BinaryExpression implementation
void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BinaryExpression::toString() const {
    String opStr;
    switch (operator_) {
        case Operator::Plus: opStr = "+"; break;
        case Operator::Minus: opStr = "-"; break;
        case Operator::Multiply: opStr = "*"; break;
        case Operator::Divide: opStr = "/"; break;
        case Operator::Modulo: opStr = "%"; break;
        case Operator::Exponentiation: opStr = "**"; break;
        case Operator::Equal: opStr = "=="; break;
        case Operator::NotEqual: opStr = "!="; break;
        case Operator::Less: opStr = "<"; break;
        case Operator::Greater: opStr = ">"; break;
        case Operator::LessEqual: opStr = "<="; break;
        case Operator::GreaterEqual: opStr = ">="; break;
        case Operator::LogicalAnd: opStr = "&&"; break;
        case Operator::LogicalOr: opStr = "||"; break;
        case Operator::NullishCoalescing: opStr = "??"; break;
        case Operator::BitwiseAnd: opStr = "&"; break;
        case Operator::BitwiseOr: opStr = "|"; break;
        case Operator::BitwiseXor: opStr = "^"; break;
        case Operator::LeftShift: opStr = "<<"; break;
        case Operator::RightShift: opStr = ">>"; break;
        case Operator::UnsignedRightShift: opStr = ">>>"; break;
    }
    return "(" + left_->toString() + " " + opStr + " " + right_->toString() + ")";
}

// UnaryExpression implementation
void UnaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String UnaryExpression::toString() const {
    String opStr;
    switch (operator_) {
        case Operator::UnaryPlus: opStr = "+"; break;
        case Operator::UnaryMinus: opStr = "-"; break;
        case Operator::LogicalNot: opStr = "!"; break;
        case Operator::BitwiseNot: opStr = "~"; break;
        case Operator::PrefixIncrement: opStr = "++"; break;
        case Operator::PrefixDecrement: opStr = "--"; break;
        case Operator::PostfixIncrement: opStr = "++"; break;
        case Operator::PostfixDecrement: opStr = "--"; break;
        case Operator::TypeOf: opStr = "typeof "; break;
        case Operator::Void: opStr = "void "; break;
        case Operator::Delete: opStr = "delete "; break;
    }
    
    if (isPrefix_) {
        return "(" + opStr + operand_->toString() + ")";
    } else {
        return "(" + operand_->toString() + opStr + ")";
    }
}

// AssignmentExpression implementation
void AssignmentExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String AssignmentExpression::toString() const {
    String opStr;
    switch (operator_) {
        case Operator::Assign: opStr = "="; break;
        case Operator::PlusAssign: opStr = "+="; break;
        case Operator::MinusAssign: opStr = "-="; break;
        case Operator::MultiplyAssign: opStr = "*="; break;
        case Operator::DivideAssign: opStr = "/="; break;
        case Operator::ModuloAssign: opStr = "%="; break;
        case Operator::ExponentiationAssign: opStr = "**="; break;
        case Operator::LeftShiftAssign: opStr = "<<="; break;
        case Operator::RightShiftAssign: opStr = ">>="; break;
        case Operator::UnsignedRightShiftAssign: opStr = ">>>="; break;
        case Operator::BitwiseAndAssign: opStr = "&="; break;
        case Operator::BitwiseOrAssign: opStr = "|="; break;
        case Operator::BitwiseXorAssign: opStr = "^="; break;
    }
    return "(" + left_->toString() + " " + opStr + " " + right_->toString() + ")";
}

// ConditionalExpression implementation
void ConditionalExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ConditionalExpression::toString() const {
    return "(" + condition_->toString() + " ? " + thenExpr_->toString() + " : " + elseExpr_->toString() + ")";
}

// CallExpression implementation
void CallExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CallExpression::toString() const {
    String result = callee_->toString() + "(";
    for (size_t i = 0; i < arguments_.size(); ++i) {
        if (i > 0) result += ", ";
        result += arguments_[i]->toString();
    }
    result += ")";
    return result;
}

// ArrayLiteral implementation
void ArrayLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrayLiteral::toString() const {
    String result = "[";
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) result += ", ";
        result += elements_[i]->toString();
    }
    result += "]";
    return result;
}

// IndexExpression implementation
void IndexExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String IndexExpression::toString() const {
    return object_->toString() + "[" + index_->toString() + "]";
}

// ObjectLiteral implementation
void ObjectLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ObjectLiteral::toString() const {
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
            result += ": " + prop.value->toString();
        }
    }
    result += "}";
    return result;
}

// PropertyAccess implementation
void PropertyAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String PropertyAccess::toString() const {
    if (computed_) {
        return object_->toString() + "[" + property_ + "]";
    } else {
        return object_->toString() + "." + property_;
    }
}

// ArrowFunction implementation
void ArrowFunction::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrowFunction::toString() const {
    String result = "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) result += ", ";
        result += parameters_[i]->getName();
    }
    result += ") => " + body_->toString();
    return result;
}

// FunctionExpression implementation
void FunctionExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String FunctionExpression::toString() const {
    String result = "function " + name_ + "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) result += ", ";
        result += parameters_[i]->getName();
    }
    result += ") { ... }";
    return result;
}

// MoveExpression implementation
void MoveExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String MoveExpression::toString() const {
    return "move(" + expression_->toString() + ")";
}

} // namespace expressions
} // namespace ast
} // namespace tsc