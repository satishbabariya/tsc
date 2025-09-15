#include "tsc/utils/ASTPrinter.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {

ASTPrinter::ASTPrinter(std::ostream& output, int indent) 
    : output_(output), indent_(indent) {}

void ASTPrinter::print(const ASTNode& node) {
    // Cast to non-const to call accept (visitor pattern limitation)
    const_cast<ASTNode&>(node).accept(*this);
}

void ASTPrinter::visit(NumericLiteral& node) {
    printIndent();
    output_ << "NumericLiteral: " << node.getValue() << std::endl;
}

void ASTPrinter::visit(StringLiteral& node) {
    printIndent();
    output_ << "StringLiteral: \"" << node.getValue() << "\"" << std::endl;
}

void ASTPrinter::visit(TemplateLiteral& node) {
    printIndent();
    output_ << "TemplateLiteral:" << std::endl;
    
    increaseIndent();
    for (size_t i = 0; i < node.getElements().size(); ++i) {
        const auto& element = node.getElements()[i];
        printIndent();
        if (element.isExpression()) {
            output_ << "Expression[" << i << "]:" << std::endl;
            increaseIndent();
            element.getExpression()->accept(*this);
            decreaseIndent();
        } else {
            output_ << "Text[" << i << "]: \"" << element.getText() << "\"" << std::endl;
        }
    }
    decreaseIndent();
}

void ASTPrinter::visit(BooleanLiteral& node) {
    printIndent();
    output_ << "BooleanLiteral: " << (node.getValue() ? "true" : "false") << std::endl;
}

void ASTPrinter::visit(NullLiteral& node) {
    printIndent();
    output_ << "NullLiteral" << std::endl;
}

void ASTPrinter::visit(Identifier& node) {
    printIndent();
    output_ << "Identifier: " << node.getName() << std::endl;
}

void ASTPrinter::visit(ThisExpression& node) {
    printIndent();
    output_ << "ThisExpression: this" << std::endl;
}

void ASTPrinter::visit(SuperExpression& node) {
    printIndent();
    output_ << "SuperExpression: super" << std::endl;
}

void ASTPrinter::visit(NewExpression& node) {
    printIndent();
    output_ << "NewExpression:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Constructor:" << std::endl;
    increaseIndent();
    node.getConstructor()->accept(*this);
    decreaseIndent();
    
    if (!node.getArguments().empty()) {
        printIndent();
        output_ << "Arguments (" << node.getArguments().size() << "):" << std::endl;
        increaseIndent();
        for (const auto& arg : node.getArguments()) {
            arg->accept(*this);
        }
        decreaseIndent();
    }
    decreaseIndent();
}

void ASTPrinter::visit(BinaryExpression& node) {
    printIndent();
    output_ << "BinaryExpression: " << getOperatorString(node.getOperator()) << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Left:" << std::endl;
    increaseIndent();
    node.getLeft()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Right:" << std::endl;
    increaseIndent();
    node.getRight()->accept(*this);
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(UnaryExpression& node) {
    printIndent();
    output_ << "UnaryExpression: " << getOperatorString(node.getOperator()) 
            << " (" << (node.isPrefix() ? "prefix" : "postfix") << ")" << std::endl;
    
    increaseIndent();
    node.getOperand()->accept(*this);
    decreaseIndent();
}

void ASTPrinter::visit(AssignmentExpression& node) {
    printIndent();
    output_ << "AssignmentExpression: " << getOperatorString(node.getOperator()) << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Left:" << std::endl;
    increaseIndent();
    node.getLeft()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Right:" << std::endl;
    increaseIndent();
    node.getRight()->accept(*this);
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(ConditionalExpression& node) {
    printIndent();
    output_ << "ConditionalExpression:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Condition:" << std::endl;
    increaseIndent();
    node.getCondition()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "TrueExpression:" << std::endl;
    increaseIndent();
    node.getTrueExpression()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "FalseExpression:" << std::endl;
    increaseIndent();
    node.getFalseExpression()->accept(*this);
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(CallExpression& node) {
    printIndent();
    output_ << "CallExpression:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Callee:" << std::endl;
    increaseIndent();
    node.getCallee()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Arguments (" << node.getArguments().size() << "):" << std::endl;
    increaseIndent();
    for (const auto& arg : node.getArguments()) {
        arg->accept(*this);
    }
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(ArrayLiteral& node) {
    printIndent();
    output_ << "ArrayLiteral:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Elements (" << node.getElements().size() << "):" << std::endl;
    increaseIndent();
    for (const auto& element : node.getElements()) {
        element->accept(*this);
    }
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(IndexExpression& node) {
    printIndent();
    output_ << "IndexExpression:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Object:" << std::endl;
    increaseIndent();
    node.getObject()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Index:" << std::endl;
    increaseIndent();
    node.getIndex()->accept(*this);
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(ObjectLiteral& node) {
    printIndent();
    output_ << "ObjectLiteral:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Properties (" << node.getProperties().size() << "):" << std::endl;
    increaseIndent();
    for (const auto& property : node.getProperties()) {
        printIndent();
        output_ << "Property: " << property.getKey() << std::endl;
        increaseIndent();
        property.getValue()->accept(*this);
        decreaseIndent();
    }
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(PropertyAccess& node) {
    printIndent();
    output_ << "PropertyAccess: " << node.getProperty() << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Object:" << std::endl;
    increaseIndent();
    node.getObject()->accept(*this);
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(ArrowFunction& node) {
    printIndent();
    output_ << "ArrowFunction:" << std::endl;
    
    increaseIndent();
    
    // Print parameters
    if (!node.getParameters().empty()) {
        printIndent();
        output_ << "Parameters:" << std::endl;
        increaseIndent();
        for (const auto& param : node.getParameters()) {
            printIndent();
            output_ << "Parameter: " << param.name;
            if (param.type) {
                output_ << " : " << param.type->toString();
            }
            output_ << std::endl;
        }
        decreaseIndent();
    }
    
    // Print return type
    if (node.getReturnType()) {
        printIndent();
        output_ << "ReturnType: " << node.getReturnType()->toString() << std::endl;
    }
    
    // Print body
    printIndent();
    output_ << "Body:" << std::endl;
    increaseIndent();
    node.getBody()->accept(*this);
    decreaseIndent();
    
    decreaseIndent();
}

void ASTPrinter::visit(ExpressionStatement& node) {
    printIndent();
    output_ << "ExpressionStatement:" << std::endl;
    
    increaseIndent();
    node.getExpression()->accept(*this);
    decreaseIndent();
}

void ASTPrinter::visit(BlockStatement& node) {
    printIndent();
    output_ << "BlockStatement (" << node.getStatements().size() << " statements):" << std::endl;
    
    increaseIndent();
    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
    }
    decreaseIndent();
}

void ASTPrinter::visit(ReturnStatement& node) {
    printIndent();
    output_ << "ReturnStatement";
    
    if (node.hasValue()) {
        output_ << ":" << std::endl;
        increaseIndent();
        printIndent();
        output_ << "Value:" << std::endl;
        increaseIndent();
        node.getValue()->accept(*this);
        decreaseIndent();
        decreaseIndent();
    } else {
        output_ << " (void)" << std::endl;
    }
}

void ASTPrinter::visit(IfStatement& node) {
    printIndent();
    output_ << "IfStatement:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Condition:" << std::endl;
    increaseIndent();
    node.getCondition()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Then:" << std::endl;
    increaseIndent();
    node.getThenStatement()->accept(*this);
    decreaseIndent();
    
    if (node.hasElse()) {
        printIndent();
        output_ << "Else:" << std::endl;
        increaseIndent();
        node.getElseStatement()->accept(*this);
        decreaseIndent();
    }
    
    decreaseIndent();
}

void ASTPrinter::visit(WhileStatement& node) {
    printIndent();
    output_ << "WhileStatement:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Condition:" << std::endl;
    increaseIndent();
    node.getCondition()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Body:" << std::endl;
    increaseIndent();
    node.getBody()->accept(*this);
    decreaseIndent();
    
    decreaseIndent();
}

void ASTPrinter::visit(DoWhileStatement& node) {
    printIndent();
    output_ << "DoWhileStatement:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Body:" << std::endl;
    increaseIndent();
    node.getBody()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Condition:" << std::endl;
    increaseIndent();
    node.getCondition()->accept(*this);
    decreaseIndent();
    
    decreaseIndent();
}

void ASTPrinter::visit(ForStatement& node) {
    printIndent();
    output_ << "ForStatement:" << std::endl;
    
    increaseIndent();
    
    if (node.getInit()) {
        printIndent();
        output_ << "Init:" << std::endl;
        increaseIndent();
        node.getInit()->accept(*this);
        decreaseIndent();
    }
    
    if (node.getCondition()) {
        printIndent();
        output_ << "Condition:" << std::endl;
        increaseIndent();
        node.getCondition()->accept(*this);
        decreaseIndent();
    }
    
    if (node.getIncrement()) {
        printIndent();
        output_ << "Increment:" << std::endl;
        increaseIndent();
        node.getIncrement()->accept(*this);
        decreaseIndent();
    }
    
    printIndent();
    output_ << "Body:" << std::endl;
    increaseIndent();
    node.getBody()->accept(*this);
    decreaseIndent();
    
    decreaseIndent();
}

void ASTPrinter::visit(ForOfStatement& node) {
    printIndent();
    output_ << "ForOfStatement:" << std::endl;
    
    increaseIndent();
    
    printIndent();
    output_ << "Variable: " << node.getVariable() << std::endl;
    
    printIndent();
    output_ << "Iterable:" << std::endl;
    increaseIndent();
    node.getIterable()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Body:" << std::endl;
    increaseIndent();
    node.getBody()->accept(*this);
    decreaseIndent();
    
    decreaseIndent();
}

void ASTPrinter::visit(SwitchStatement& node) {
    printIndent();
    output_ << "SwitchStatement:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Discriminant:" << std::endl;
    increaseIndent();
    node.getDiscriminant()->accept(*this);
    decreaseIndent();
    
    printIndent();
    output_ << "Cases (" << node.getCases().size() << "):" << std::endl;
    increaseIndent();
    for (const auto& caseClause : node.getCases()) {
        caseClause->accept(*this);
    }
    decreaseIndent();
    
    decreaseIndent();
}

void ASTPrinter::visit(CaseClause& node) {
    printIndent();
    if (node.isDefault()) {
        output_ << "DefaultClause:" << std::endl;
    } else {
        output_ << "CaseClause:" << std::endl;
        increaseIndent();
        printIndent();
        output_ << "Test:" << std::endl;
        increaseIndent();
        node.getTest()->accept(*this);
        decreaseIndent();
        decreaseIndent();
    }
    
    if (!node.getStatements().empty()) {
        increaseIndent();
        printIndent();
        output_ << "Statements (" << node.getStatements().size() << "):" << std::endl;
        increaseIndent();
        for (const auto& stmt : node.getStatements()) {
            stmt->accept(*this);
        }
        decreaseIndent();
        decreaseIndent();
    }
}

void ASTPrinter::visit(BreakStatement& node) {
    printIndent();
    output_ << "BreakStatement" << std::endl;
}

void ASTPrinter::visit(ContinueStatement& node) {
    printIndent();
    output_ << "ContinueStatement" << std::endl;
}

void ASTPrinter::visit(TryStatement& node) {
    printIndent();
    output_ << "TryStatement:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "TryBlock:" << std::endl;
    increaseIndent();
    node.getTryBlock()->accept(*this);
    decreaseIndent();
    
    if (node.hasCatch()) {
        printIndent();
        output_ << "CatchClause:" << std::endl;
        increaseIndent();
        node.getCatchClause()->accept(*this);
        decreaseIndent();
    }
    
    if (node.hasFinally()) {
        printIndent();
        output_ << "FinallyBlock:" << std::endl;
        increaseIndent();
        node.getFinallyBlock()->accept(*this);
        decreaseIndent();
    }
    decreaseIndent();
}

void ASTPrinter::visit(CatchClause& node) {
    printIndent();
    output_ << "CatchClause";
    
    if (node.hasParameter()) {
        output_ << ": " << node.getParameter();
    }
    output_ << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Body:" << std::endl;
    increaseIndent();
    node.getBody()->accept(*this);
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(ThrowStatement& node) {
    printIndent();
    output_ << "ThrowStatement:" << std::endl;
    
    increaseIndent();
    printIndent();
    output_ << "Expression:" << std::endl;
    increaseIndent();
    node.getExpression()->accept(*this);
    decreaseIndent();
    decreaseIndent();
}

void ASTPrinter::visit(VariableDeclaration& node) {
    printIndent();
    output_ << "VariableDeclaration: " << node.getName();
    
    switch (node.getDeclarationKind()) {
        case VariableDeclaration::Kind::Var: output_ << " (var)"; break;
        case VariableDeclaration::Kind::Let: output_ << " (let)"; break;
        case VariableDeclaration::Kind::Const: output_ << " (const)"; break;
    }
    
    output_ << std::endl;
    
    if (node.getInitializer()) {
        increaseIndent();
        printIndent();
        output_ << "Initializer:" << std::endl;
        increaseIndent();
        node.getInitializer()->accept(*this);
        decreaseIndent();
        decreaseIndent();
    }
}

void ASTPrinter::visit(FunctionDeclaration& node) {
    printIndent();
    output_ << "FunctionDeclaration: " << node.getName();
    
    if (node.isAsync()) output_ << " (async)";
    if (node.isGenerator()) output_ << " (generator)";
    
    output_ << std::endl;
    
    increaseIndent();
    
    // Print type parameters if any
    if (!node.getTypeParameters().empty()) {
        printIndent();
        output_ << "Type Parameters (" << node.getTypeParameters().size() << "):" << std::endl;
        increaseIndent();
        for (const auto& typeParam : node.getTypeParameters()) {
            typeParam->accept(*this);
        }
        decreaseIndent();
    }
    
    printIndent();
    output_ << "Parameters (" << node.getParameters().size() << "):" << std::endl;
    increaseIndent();
    for (const auto& param : node.getParameters()) {
        printIndent();
        output_ << "Parameter: " << param.name;
        if (param.optional) output_ << " (optional)";
        if (param.rest) output_ << " (rest)";
        output_ << std::endl;
    }
    decreaseIndent();
    
    if (node.getBody()) {
        printIndent();
        output_ << "Body:" << std::endl;
        increaseIndent();
        node.getBody()->accept(*this);
        decreaseIndent();
    }
    decreaseIndent();
}

void ASTPrinter::visit(TypeParameter& node) {
    printIndent();
    output_ << "TypeParameter: " << node.getName();
    if (node.getConstraint()) {
        output_ << " extends " << node.getConstraint()->toString();
    }
    output_ << std::endl;
}

void ASTPrinter::visit(Module& node) {
    printIndent();
    output_ << "Module: " << node.getFilename() 
            << " (" << node.getStatements().size() << " statements)" << std::endl;
    
    increaseIndent();
    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
    }
    decreaseIndent();
}

void ASTPrinter::printIndent() {
    for (int i = 0; i < indent_; ++i) {
        output_ << "  ";
    }
}

void ASTPrinter::increaseIndent() {
    indent_++;
}

void ASTPrinter::decreaseIndent() {
    if (indent_ > 0) {
        indent_--;
    }
}

String ASTPrinter::getOperatorString(BinaryExpression::Operator op) {
    switch (op) {
        case BinaryExpression::Operator::Add: return "+";
        case BinaryExpression::Operator::Subtract: return "-";
        case BinaryExpression::Operator::Multiply: return "*";
        case BinaryExpression::Operator::Divide: return "/";
        case BinaryExpression::Operator::Modulo: return "%";
        case BinaryExpression::Operator::Power: return "**";
        case BinaryExpression::Operator::Equal: return "==";
        case BinaryExpression::Operator::NotEqual: return "!=";
        case BinaryExpression::Operator::StrictEqual: return "===";
        case BinaryExpression::Operator::StrictNotEqual: return "!==";
        case BinaryExpression::Operator::Less: return "<";
        case BinaryExpression::Operator::Greater: return ">";
        case BinaryExpression::Operator::LessEqual: return "<=";
        case BinaryExpression::Operator::GreaterEqual: return ">=";
        case BinaryExpression::Operator::LogicalAnd: return "&&";
        case BinaryExpression::Operator::LogicalOr: return "||";
        case BinaryExpression::Operator::BitwiseAnd: return "&";
        case BinaryExpression::Operator::BitwiseOr: return "|";
        case BinaryExpression::Operator::BitwiseXor: return "^";
        case BinaryExpression::Operator::LeftShift: return "<<";
        case BinaryExpression::Operator::RightShift: return ">>";
        case BinaryExpression::Operator::UnsignedRightShift: return ">>>";
        case BinaryExpression::Operator::In: return "in";
        case BinaryExpression::Operator::Instanceof: return "instanceof";
        case BinaryExpression::Operator::As: return "as";
        case BinaryExpression::Operator::Satisfies: return "satisfies";
        default: return "unknown";
    }
}

String ASTPrinter::getOperatorString(UnaryExpression::Operator op) {
    switch (op) {
        case UnaryExpression::Operator::Plus: return "+";
        case UnaryExpression::Operator::Minus: return "-";
        case UnaryExpression::Operator::BitwiseNot: return "~";
        case UnaryExpression::Operator::LogicalNot: return "!";
        case UnaryExpression::Operator::PreIncrement: return "++";
        case UnaryExpression::Operator::PostIncrement: return "++";
        case UnaryExpression::Operator::PreDecrement: return "--";
        case UnaryExpression::Operator::PostDecrement: return "--";
        case UnaryExpression::Operator::Typeof: return "typeof";
        case UnaryExpression::Operator::Delete: return "delete";
        case UnaryExpression::Operator::Void: return "void";
        case UnaryExpression::Operator::Await: return "await";
        default: return "unknown";
    }
}

String ASTPrinter::getOperatorString(AssignmentExpression::Operator op) {
    switch (op) {
        case AssignmentExpression::Operator::Assign: return "=";
        case AssignmentExpression::Operator::AddAssign: return "+=";
        case AssignmentExpression::Operator::SubtractAssign: return "-=";
        case AssignmentExpression::Operator::MultiplyAssign: return "*=";
        case AssignmentExpression::Operator::DivideAssign: return "/=";
        case AssignmentExpression::Operator::ModuloAssign: return "%=";
        case AssignmentExpression::Operator::LeftShiftAssign: return "<<=";
        case AssignmentExpression::Operator::RightShiftAssign: return ">>=";
        case AssignmentExpression::Operator::UnsignedRightShiftAssign: return ">>>=";
        case AssignmentExpression::Operator::BitwiseAndAssign: return "&=";
        case AssignmentExpression::Operator::BitwiseOrAssign: return "|=";
        case AssignmentExpression::Operator::BitwiseXorAssign: return "^=";
        case AssignmentExpression::Operator::PowerAssign: return "**=";
        case AssignmentExpression::Operator::NullishCoalescingAssign: return "??=";
        case AssignmentExpression::Operator::LogicalAndAssign: return "&&=";
        case AssignmentExpression::Operator::LogicalOrAssign: return "||=";
        default: return "unknown";
    }
}

// Class-related visitor implementations
void ASTPrinter::visit(PropertyDeclaration& node) {
    printIndent();
    output_ << "PropertyDeclaration: " << node.getName() << std::endl;
    increaseIndent();
    
    if (node.isPrivate()) {
        printIndent();
        output_ << "Access: private" << std::endl;
    } else if (node.isProtected()) {
        printIndent();
        output_ << "Access: protected" << std::endl;
    } else {
        printIndent();
        output_ << "Access: public" << std::endl;
    }
    
    if (node.isStatic()) {
        printIndent();
        output_ << "Static: true" << std::endl;
    }
    
    if (node.isReadonly()) {
        printIndent();
        output_ << "Readonly: true" << std::endl;
    }
    
    if (node.getType()) {
        printIndent();
        output_ << "Type: " << node.getType()->toString() << std::endl;
    }
    
    if (node.getInitializer()) {
        printIndent();
        output_ << "Initializer:" << std::endl;
        increaseIndent();
        node.getInitializer()->accept(*this);
        decreaseIndent();
    }
    
    decreaseIndent();
}

void ASTPrinter::visit(MethodDeclaration& node) {
    printIndent();
    output_ << "MethodDeclaration: " << node.getName() << std::endl;
    increaseIndent();
    
    if (node.isPrivate()) {
        printIndent();
        output_ << "Access: private" << std::endl;
    } else if (node.isProtected()) {
        printIndent();
        output_ << "Access: protected" << std::endl;
    } else {
        printIndent();
        output_ << "Access: public" << std::endl;
    }
    
    if (node.isStatic()) {
        printIndent();
        output_ << "Static: true" << std::endl;
    }
    
    if (node.isAbstract()) {
        printIndent();
        output_ << "Abstract: true" << std::endl;
    }
    
    if (!node.getParameters().empty()) {
        printIndent();
        output_ << "Parameters:" << std::endl;
        increaseIndent();
        for (const auto& param : node.getParameters()) {
            printIndent();
            output_ << param.name;
            if (param.type) {
                output_ << ": " << param.type->toString();
            }
            if (param.optional) {
                output_ << " (optional)";
            }
            if (param.rest) {
                output_ << " (rest)";
            }
            output_ << std::endl;
        }
        decreaseIndent();
    }
    
    if (node.getReturnType()) {
        printIndent();
        output_ << "Return Type: " << node.getReturnType()->toString() << std::endl;
    }
    
    if (node.getBody()) {
        printIndent();
        output_ << "Body:" << std::endl;
        increaseIndent();
        node.getBody()->accept(*this);
        decreaseIndent();
    }
    
    decreaseIndent();
}

void ASTPrinter::visit(ClassDeclaration& node) {
    printIndent();
    output_ << "ClassDeclaration: " << node.getName() << std::endl;
    increaseIndent();
    
    if (node.isAbstract()) {
        printIndent();
        output_ << "Abstract: true" << std::endl;
    }
    
    if (node.getBaseClass()) {
        printIndent();
        output_ << "Base Class: " << node.getBaseClass()->toString() << std::endl;
    }
    
    if (!node.getInterfaces().empty()) {
        printIndent();
        output_ << "Interfaces:" << std::endl;
        increaseIndent();
        for (const auto& interface : node.getInterfaces()) {
            printIndent();
            output_ << interface->toString() << std::endl;
        }
        decreaseIndent();
    }
    
    if (node.getConstructor()) {
        printIndent();
        output_ << "Constructor:" << std::endl;
        increaseIndent();
        node.getConstructor()->accept(*this);
        decreaseIndent();
    }
    
    if (!node.getProperties().empty()) {
        printIndent();
        output_ << "Properties:" << std::endl;
        increaseIndent();
        for (const auto& property : node.getProperties()) {
            property->accept(*this);
        }
        decreaseIndent();
    }
    
    if (!node.getMethods().empty()) {
        printIndent();
        output_ << "Methods:" << std::endl;
        increaseIndent();
        for (const auto& method : node.getMethods()) {
            method->accept(*this);
        }
        decreaseIndent();
    }
    
    decreaseIndent();
}

void ASTPrinter::visit(InterfaceDeclaration& node) {
    printIndent();
    output_ << "InterfaceDeclaration: " << node.getName() << std::endl;
    increaseIndent();
    
    if (!node.getExtends().empty()) {
        printIndent();
        output_ << "Extends:" << std::endl;
        increaseIndent();
        for (const auto& extended : node.getExtends()) {
            printIndent();
            output_ << extended->toString() << std::endl;
        }
        decreaseIndent();
    }
    
    if (!node.getProperties().empty()) {
        printIndent();
        output_ << "Properties:" << std::endl;
        increaseIndent();
        for (const auto& property : node.getProperties()) {
            property->accept(*this);
        }
        decreaseIndent();
    }
    
    if (!node.getMethods().empty()) {
        printIndent();
        output_ << "Method Signatures:" << std::endl;
        increaseIndent();
        for (const auto& method : node.getMethods()) {
            method->accept(*this);
        }
        decreaseIndent();
    }
    
    decreaseIndent();
}

void ASTPrinter::visit(EnumMember& node) {
    printIndent();
    output_ << "EnumMember: " << node.getName();
    if (node.hasValue()) {
        output_ << " = ";
        node.getValue()->accept(*this);
    }
    output_ << std::endl;
}

void ASTPrinter::visit(EnumDeclaration& node) {
    printIndent();
    output_ << "EnumDeclaration: ";
    if (node.isConst()) {
        output_ << "const ";
    }
    output_ << node.getName() << std::endl;
    increaseIndent();
    
    if (!node.getMembers().empty()) {
        printIndent();
        output_ << "Members:" << std::endl;
        increaseIndent();
        for (const auto& member : node.getMembers()) {
            member->accept(*this);
        }
        decreaseIndent();
    }
    
    decreaseIndent();
}

void ASTPrinter::visit(TypeAliasDeclaration& node) {
    printIndent();
    output_ << "TypeAliasDeclaration: " << node.getName() 
            << " = " << node.getAliasedType()->toString() << std::endl;
}

void ASTPrinter::visit(ImportDeclaration& node) {
    printIndent();
    output_ << "ImportDeclaration: " << node.toString() << std::endl;
}

void ASTPrinter::visit(ExportDeclaration& node) {
    printIndent();
    output_ << "ExportDeclaration: " << node.toString() << std::endl;
}

void ASTPrinter::visit(FunctionExpression& node) {
    printIndent();
    output_ << "FunctionExpression";
    if (!node.getName().empty()) {
        output_ << " (" << node.getName() << ")";
    }
    output_ << ":" << std::endl;
    increaseIndent();
    
    if (!node.getParameters().empty()) {
        printIndent();
        output_ << "Parameters:" << std::endl;
        increaseIndent();
        for (const auto& param : node.getParameters()) {
            printIndent();
            output_ << param.name;
            if (param.type) {
                output_ << ": " << param.type->toString();
            }
            if (param.optional) {
                output_ << " (optional)";
            }
            if (param.rest) {
                output_ << " (rest)";
            }
            output_ << std::endl;
        }
        decreaseIndent();
    }
    
    if (node.getReturnType()) {
        printIndent();
        output_ << "Return Type: " << node.getReturnType()->toString() << std::endl;
    }
    
    if (node.getBody()) {
        printIndent();
        output_ << "Body:" << std::endl;
        increaseIndent();
        node.getBody()->accept(*this);
        decreaseIndent();
    }
    
    decreaseIndent();
}

} // namespace tsc