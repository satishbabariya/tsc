#include "tsc/AST.h"
#include "tsc/semantic/TypeSystem.h"
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

// ThisExpression implementation
void ThisExpression::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
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
    ss << callee_->toString();
    
    // Add type arguments if present
    if (!typeArguments_.empty()) {
        ss << "<";
        for (size_t i = 0; i < typeArguments_.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << typeArguments_[i]->toString();
        }
        ss << ">";
    }
    
    ss << "(";
    
    for (size_t i = 0; i < arguments_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << arguments_[i]->toString();
    }
    
    ss << ")";
    return ss.str();
}

// ArrayLiteral implementation
void ArrayLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrayLiteral::toString() const {
    std::stringstream ss;
    ss << "[";
    
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << elements_[i]->toString();
    }
    
    ss << "]";
    return ss.str();
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
    std::stringstream ss;
    ss << "{";
    
    for (size_t i = 0; i < properties_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << properties_[i].getKey() << ": " << properties_[i].getValue()->toString();
    }
    
    ss << "}";
    return ss.str();
}

// PropertyAccess implementation
void PropertyAccess::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String PropertyAccess::toString() const {
    return object_->toString() + "." + property_;
}

void ArrowFunction::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ArrowFunction::toString() const {
    std::ostringstream oss;
    
    if (parameters_.size() == 1 && !parameters_[0].type) {
        // Single parameter without type: param => body
        oss << parameters_[0].name;
    } else {
        // Multiple parameters or typed parameters: (param1, param2) => body
        oss << "(";
        for (size_t i = 0; i < parameters_.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << parameters_[i].name;
            if (parameters_[i].type) {
                oss << ": " << parameters_[i].type->toString();
            }
        }
        oss << ")";
    }
    
    oss << " => ";
    
    // For now, simplified body representation
    if (body_) {
        oss << "{ /* body */ }";
    }
    
    return oss.str();
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

// CaseClause implementation
void CaseClause::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CaseClause::toString() const {
    std::stringstream ss;
    
    if (isDefault()) {
        ss << "default:";
    } else {
        ss << "case " << test_->toString() << ":";
    }
    
    if (!statements_.empty()) {
        ss << "\n";
        for (const auto& stmt : statements_) {
            ss << "  " << stmt->toString() << "\n";
        }
    }
    
    return ss.str();
}

// SwitchStatement implementation
void SwitchStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String SwitchStatement::toString() const {
    std::stringstream ss;
    ss << "switch (" << discriminant_->toString() << ") {\n";
    
    for (const auto& caseClause : cases_) {
        String caseStr = caseClause->toString();
        // Add indentation to each line
        std::istringstream iss(caseStr);
        std::string line;
        while (std::getline(iss, line)) {
            if (!line.empty()) {
                ss << "  " << line << "\n";
            }
        }
    }
    
    ss << "}";
    return ss.str();
}

// BreakStatement implementation
void BreakStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String BreakStatement::toString() const {
    return "break;";
}

// ContinueStatement implementation
void ContinueStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ContinueStatement::toString() const {
    return "continue;";
}

// TryStatement implementation
void TryStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TryStatement::toString() const {
    std::stringstream ss;
    ss << "try " << tryBlock_->toString();
    
    if (hasCatch()) {
        ss << " " << catchClause_->toString();
    }
    
    if (hasFinally()) {
        ss << " finally " << finallyBlock_->toString();
    }
    
    return ss.str();
}

// CatchClause implementation
void CatchClause::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String CatchClause::toString() const {
    std::stringstream ss;
    ss << "catch";
    
    if (hasParameter()) {
        ss << " (" << parameter_ << ")";
    }
    
    ss << " " << body_->toString();
    return ss.str();
}

// ThrowStatement implementation
void ThrowStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ThrowStatement::toString() const {
    return "throw " + expression_->toString() + ";";
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

// TypeParameter implementation
void TypeParameter::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TypeParameter::toString() const {
    return name_ + (constraint_ ? " extends " + constraint_->toString() : "");
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
    ss << " " << name_;
    
    // Add type parameters
    if (!typeParameters_.empty()) {
        ss << "<";
        for (size_t i = 0; i < typeParameters_.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << typeParameters_[i]->toString();
        }
        ss << ">";
    }
    
    ss << "(";
    
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

// PropertyDeclaration implementation
void PropertyDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String PropertyDeclaration::toString() const {
    std::ostringstream oss;
    if (isPrivate_) oss << "private ";
    if (isProtected_) oss << "protected ";
    if (isStatic_) oss << "static ";
    if (isReadonly_) oss << "readonly ";
    oss << name_;
    if (type_) {
        oss << ": " << type_->toString();
    }
    if (initializer_) {
        oss << " = " << initializer_->toString();
    }
    return oss.str();
}

// MethodDeclaration implementation
void MethodDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String MethodDeclaration::toString() const {
    std::ostringstream oss;
    if (isPrivate_) oss << "private ";
    if (isProtected_) oss << "protected ";
    if (isStatic_) oss << "static ";
    if (isAbstract_) oss << "abstract ";
    if (isAsync_) oss << "async ";
    
    oss << name_ << "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << parameters_[i].name;
        if (parameters_[i].type) {
            oss << ": " << parameters_[i].type->toString();
        }
        if (parameters_[i].optional) oss << "?";
        if (parameters_[i].rest) oss << "...";
    }
    oss << ")";
    
    if (returnType_) {
        oss << ": " << returnType_->toString();
    }
    
    if (body_) {
        oss << " { /* method body */ }";
    } else {
        oss << ";";
    }
    
    return oss.str();
}

// ClassDeclaration implementation
void ClassDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ClassDeclaration::toString() const {
    std::ostringstream oss;
    if (isAbstract_) oss << "abstract ";
    oss << "class " << name_;
    
    if (baseClass_) {
        oss << " extends " << baseClass_->toString();
    }
    
    if (!interfaces_.empty()) {
        oss << " implements ";
        for (size_t i = 0; i < interfaces_.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << interfaces_[i]->toString();
        }
    }
    
    oss << " {\n";
    
    // Constructor
    if (constructor_) {
        oss << "  constructor" << constructor_->toString().substr(constructor_->getName().length()) << "\n";
    }
    
    // Properties
    for (const auto& prop : properties_) {
        oss << "  " << prop->toString() << ";\n";
    }
    
    // Methods
    for (const auto& method : methods_) {
        oss << "  " << method->toString() << "\n";
    }
    
    oss << "}";
    return oss.str();
}

// InterfaceDeclaration implementation
void InterfaceDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String InterfaceDeclaration::toString() const {
    std::ostringstream oss;
    oss << "interface " << name_;
    
    if (!extends_.empty()) {
        oss << " extends ";
        for (size_t i = 0; i < extends_.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << extends_[i]->toString();
        }
    }
    
    oss << " {\n";
    
    // Properties
    for (const auto& prop : properties_) {
        oss << "  " << prop->toString() << ";\n";
    }
    
    // Methods (interfaces only have method signatures)
    for (const auto& method : methods_) {
        oss << "  " << method->toString() << ";\n";
    }
    
    oss << "}";
    return oss.str();
}

// EnumMember implementation
void EnumMember::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String EnumMember::toString() const {
    if (hasValue()) {
        return name_ + " = " + value_->toString();
    } else {
        return name_;
    }
}

// EnumDeclaration implementation
void EnumDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String EnumDeclaration::toString() const {
    std::ostringstream oss;
    if (isConst_) oss << "const ";
    oss << "enum " << name_ << " {\n";
    
    for (size_t i = 0; i < members_.size(); ++i) {
        oss << "  " << members_[i]->toString();
        if (i < members_.size() - 1) oss << ",";
        oss << "\n";
    }
    
    oss << "}";
    return oss.str();
}

// TypeAliasDeclaration implementation
void TypeAliasDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TypeAliasDeclaration::toString() const {
    return "type " + name_ + " = " + aliasedType_->toString();
}

} // namespace tsc