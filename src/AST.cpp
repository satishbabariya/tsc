#include "tsc/AST.h"
#include <sstream>

namespace tsc {

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

// ExpressionStatement implementation
void ExpressionStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ExpressionStatement::toString() const {
    return expression_->toString() + ";";
}

// Identifier implementation
void Identifier::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// BinaryExpression implementation
void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BinaryExpression::toString() const {
    std::stringstream ss;
    ss << "(" << left_->toString();
    
    switch (operator_) {
        case Operator::Add: ss << " + "; break;
        case Operator::Subtract: ss << " - "; break;
        case Operator::Multiply: ss << " * "; break;
        case Operator::Divide: ss << " / "; break;
        case Operator::Modulo: ss << " % "; break;
        case Operator::Power: ss << " ** "; break;
        case Operator::Equal: ss << " == "; break;
        case Operator::NotEqual: ss << " != "; break;
        case Operator::StrictEqual: ss << " === "; break;
        case Operator::StrictNotEqual: ss << " !== "; break;
        case Operator::Less: ss << " < "; break;
        case Operator::Greater: ss << " > "; break;
        case Operator::LessEqual: ss << " <= "; break;
        case Operator::GreaterEqual: ss << " >= "; break;
        case Operator::LogicalAnd: ss << " && "; break;
        case Operator::LogicalOr: ss << " || "; break;
        case Operator::NullishCoalescing: ss << " ?? "; break;
        case Operator::BitwiseAnd: ss << " & "; break;
        case Operator::BitwiseOr: ss << " | "; break;
        case Operator::BitwiseXor: ss << " ^ "; break;
        case Operator::LeftShift: ss << " << "; break;
        case Operator::RightShift: ss << " >> "; break;
        case Operator::UnsignedRightShift: ss << " >>> "; break;
        case Operator::In: ss << " in "; break;
        case Operator::Instanceof: ss << " instanceof "; break;
        case Operator::As: ss << " as "; break;
        case Operator::Satisfies: ss << " satisfies "; break;
    }
    
    ss << right_->toString() << ")";
    return ss.str();
}

// UnaryExpression implementation
void UnaryExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String UnaryExpression::toString() const {
    std::stringstream ss;
    
    if (prefix_) {
        switch (operator_) {
            case Operator::Plus: ss << "+"; break;
            case Operator::Minus: ss << "-"; break;
            case Operator::BitwiseNot: ss << "~"; break;
            case Operator::LogicalNot: ss << "!"; break;
            case Operator::PreIncrement: ss << "++"; break;
            case Operator::PreDecrement: ss << "--"; break;
            case Operator::Typeof: ss << "typeof "; break;
            case Operator::Delete: ss << "delete "; break;
            case Operator::Void: ss << "void "; break;
            case Operator::Await: ss << "await "; break;
            default: break;
        }
        ss << operand_->toString();
        
        if (operator_ == Operator::PostIncrement) ss << "++";
        if (operator_ == Operator::PostDecrement) ss << "--";
    } else {
        ss << operand_->toString();
        if (operator_ == Operator::PostIncrement) ss << "++";
        if (operator_ == Operator::PostDecrement) ss << "--";
    }
    
    return ss.str();
}

// AssignmentExpression implementation
void AssignmentExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String AssignmentExpression::toString() const {
    std::stringstream ss;
    ss << left_->toString();
    
    switch (operator_) {
        case Operator::Assign: ss << " = "; break;
        case Operator::AddAssign: ss << " += "; break;
        case Operator::SubtractAssign: ss << " -= "; break;
        case Operator::MultiplyAssign: ss << " *= "; break;
        case Operator::DivideAssign: ss << " /= "; break;
        case Operator::ModuloAssign: ss << " %= "; break;
        case Operator::LeftShiftAssign: ss << " <<= "; break;
        case Operator::RightShiftAssign: ss << " >>= "; break;
        case Operator::UnsignedRightShiftAssign: ss << " >>>= "; break;
        case Operator::BitwiseAndAssign: ss << " &= "; break;
        case Operator::BitwiseOrAssign: ss << " |= "; break;
        case Operator::BitwiseXorAssign: ss << " ^= "; break;
        case Operator::PowerAssign: ss << " **= "; break;
        case Operator::NullishCoalescingAssign: ss << " ??= "; break;
        case Operator::LogicalAndAssign: ss << " &&= "; break;
        case Operator::LogicalOrAssign: ss << " ||= "; break;
    }
    
    ss << right_->toString();
    return ss.str();
}

// CallExpression implementation
void CallExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CallExpression::toString() const {
    std::stringstream ss;
    ss << callee_->toString() << "(";
    
    for (size_t i = 0; i < arguments_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << arguments_[i]->toString();
    }
    
    ss << ")";
    return ss.str();
}

// BlockStatement implementation
void BlockStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BlockStatement::toString() const {
    std::stringstream ss;
    ss << "{\n";
    
    for (const auto& stmt : statements_) {
        ss << "  " << stmt->toString() << "\n";
    }
    
    ss << "}";
    return ss.str();
}

// ReturnStatement implementation
void ReturnStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ReturnStatement::toString() const {
    std::stringstream ss;
    ss << "return";
    
    if (value_) {
        ss << " " << value_->toString();
    }
    
    ss << ";";
    return ss.str();
}

// IfStatement implementation
void IfStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String IfStatement::toString() const {
    std::stringstream ss;
    ss << "if (" << condition_->toString() << ") ";
    
    if (auto block = dynamic_cast<BlockStatement*>(thenStatement_.get())) {
        ss << block->toString();
    } else {
        ss << "{\n  " << thenStatement_->toString() << "\n}";
    }
    
    if (elseStatement_) {
        ss << " else ";
        if (auto block = dynamic_cast<BlockStatement*>(elseStatement_.get())) {
            ss << block->toString();
        } else {
            ss << "{\n  " << elseStatement_->toString() << "\n}";
        }
    }
    
    return ss.str();
}

// WhileStatement implementation
void WhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String WhileStatement::toString() const {
    std::stringstream ss;
    ss << "while (" << condition_->toString() << ") ";
    
    if (auto block = dynamic_cast<BlockStatement*>(body_.get())) {
        ss << block->toString();
    } else {
        ss << "{\n  " << body_->toString() << "\n}";
    }
    
    return ss.str();
}

// DoWhileStatement implementation
void DoWhileStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String DoWhileStatement::toString() const {
    std::stringstream ss;
    ss << "do ";
    
    if (auto block = dynamic_cast<BlockStatement*>(body_.get())) {
        ss << block->toString();
    } else {
        ss << "{\n  " << body_->toString() << "\n}";
    }
    
    ss << " while (" << condition_->toString() << ");";
    
    return ss.str();
}

// ForStatement implementation
void ForStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ForStatement::toString() const {
    std::stringstream ss;
    ss << "for (";
    
    if (init_) {
        ss << init_->toString();
        // Remove trailing semicolon if present for cleaner output
        String initStr = init_->toString();
        if (!initStr.empty() && initStr.back() != ';') {
            ss << ";";
        }
    } else {
        ss << ";";
    }
    
    ss << " ";
    if (condition_) {
        ss << condition_->toString();
    }
    ss << ";";
    
    ss << " ";
    if (increment_) {
        ss << increment_->toString();
    }
    
    ss << ") ";
    
    if (auto block = dynamic_cast<BlockStatement*>(body_.get())) {
        ss << block->toString();
    } else {
        ss << "{\n  " << body_->toString() << "\n}";
    }
    
    return ss.str();
}

// VariableDeclaration implementation
void VariableDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String VariableDeclaration::toString() const {
    std::stringstream ss;
    
    switch (kind_) {
        case Kind::Var: ss << "var "; break;
        case Kind::Let: ss << "let "; break;
        case Kind::Const: ss << "const "; break;
    }
    
    ss << name_;
    
    if (typeAnnotation_) {
        ss << ": <type>"; // Type system not yet integrated
    }
    
    if (initializer_) {
        ss << " = " << initializer_->toString();
    }
    
    ss << ";";
    return ss.str();
}

// FunctionDeclaration implementation
void FunctionDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String FunctionDeclaration::toString() const {
    std::stringstream ss;
    
    if (async_) ss << "async ";
    ss << "function";
    if (generator_) ss << "*";
    ss << " " << name_ << "(";
    
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) ss << ", ";
        const auto& param = parameters_[i];
        if (param.rest) ss << "...";
        ss << param.name;
        if (param.optional) ss << "?";
        if (param.type) ss << ": <type>";
        if (param.defaultValue) ss << " = " << param.defaultValue->toString();
    }
    
    ss << ")";
    
    if (returnType_) {
        ss << ": <type>";
    }
    
    if (body_) {
        ss << " " << body_->toString();
    } else {
        ss << ";";
    }
    
    return ss.str();
}

// Module implementation
void Module::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String Module::toString() const {
    std::stringstream ss;
    ss << "// Module: " << filename_ << "\n";
    
    for (const auto& stmt : statements_) {
        ss << stmt->toString() << "\n";
    }
    
    return ss.str();
}

} // namespace tsc