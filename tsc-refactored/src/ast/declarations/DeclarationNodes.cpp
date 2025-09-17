#include "tsc/ast/declarations/DeclarationNodes.h"
#include "tsc/ast/core/ASTCore.h"

namespace tsc {
namespace ast {
namespace declarations {

// VariableDeclaration implementation
void VariableDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String VariableDeclaration::toString() const {
    String result;
    switch (kind_) {
        case Kind::Var: result = "var "; break;
        case Kind::Let: result = "let "; break;
        case Kind::Const: result = "const "; break;
    }
    result += name_;
    if (typeAnnotation_) {
        result += ": " + typeAnnotation_->toString();
    }
    if (initializer_) {
        result += " = " + initializer_->toString();
    }
    return result + ";";
}

// FunctionDeclaration implementation
void FunctionDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String FunctionDeclaration::toString() const {
    String result = "function " + name_;
    if (!typeParameters_.empty()) {
        result += "<";
        for (size_t i = 0; i < typeParameters_.size(); ++i) {
            if (i > 0) result += ", ";
            result += typeParameters_[i]->getName();
        }
        result += ">";
    }
    result += "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) result += ", ";
        result += parameters_[i]->getName();
    }
    result += ")";
    if (returnType_) {
        result += ": " + returnType_->toString();
    }
    result += " { ... }";
    return result;
}

// ClassDeclaration implementation
void ClassDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ClassDeclaration::toString() const {
    String result = "class " + name_;
    if (!typeParameters_.empty()) {
        result += "<";
        for (size_t i = 0; i < typeParameters_.size(); ++i) {
            if (i > 0) result += ", ";
            result += typeParameters_[i]->getName();
        }
        result += ">";
    }
    if (baseClass_) {
        result += " extends " + baseClass_->toString();
    }
    result += " { ... }";
    return result;
}

// InterfaceDeclaration implementation
void InterfaceDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String InterfaceDeclaration::toString() const {
    String result = "interface " + name_;
    if (!typeParameters_.empty()) {
        result += "<";
        for (size_t i = 0; i < typeParameters_.size(); ++i) {
            if (i > 0) result += ", ";
            result += typeParameters_[i]->getName();
        }
        result += ">";
    }
    if (!extends_.empty()) {
        result += " extends ";
        for (size_t i = 0; i < extends_.size(); ++i) {
            if (i > 0) result += ", ";
            result += extends_[i]->toString();
        }
    }
    result += " { ... }";
    return result;
}

// EnumDeclaration implementation
void EnumDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String EnumDeclaration::toString() const {
    String result = "enum " + name_ + " {\n";
    for (const auto& member : members_) {
        result += "  " + member->getName();
        if (member->getValue()) {
            result += " = " + member->getValue()->toString();
        }
        result += ",\n";
    }
    result += "}";
    return result;
}

// TypeAliasDeclaration implementation
void TypeAliasDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String TypeAliasDeclaration::toString() const {
    String result = "type " + name_;
    if (!typeParameters_.empty()) {
        result += "<";
        for (size_t i = 0; i < typeParameters_.size(); ++i) {
            if (i > 0) result += ", ";
            result += typeParameters_[i]->getName();
        }
        result += ">";
    }
    result += " = " + type_->toString();
    return result;
}

// ImportDeclaration implementation
void ImportDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ImportDeclaration::toString() const {
    String result = "import ";
    if (!importClauses_.empty()) {
        result += "{ ";
        for (size_t i = 0; i < importClauses_.size(); ++i) {
            if (i > 0) result += ", ";
            result += importClauses_[i]->getName();
        }
        result += " } ";
    }
    result += "from \"" + moduleSpecifier_ + "\"";
    return result;
}

// ExportDeclaration implementation
void ExportDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ExportDeclaration::toString() const {
    String result = "export ";
    switch (clause_.getType()) {
        case ExportClause::Type::Named:
            result += "{ " + clause_.getName() + " }";
            break;
        case ExportClause::Type::Default:
            result += "default " + clause_.getDefaultExport()->toString();
            break;
        case ExportClause::Type::All:
            result += "*";
            break;
        case ExportClause::Type::Namespace:
            result += "* as " + clause_.getName();
            break;
    }
    return result;
}

// PropertyDeclaration implementation
void PropertyDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String PropertyDeclaration::toString() const {
    String result;
    if (private_) result += "private ";
    else if (protected_) result += "protected ";
    else if (public_) result += "public ";
    
    if (static_) result += "static ";
    if (readonly_) result += "readonly ";
    
    result += name_;
    if (typeAnnotation_) {
        result += ": " + typeAnnotation_->toString();
    }
    if (initializer_) {
        result += " = " + initializer_->toString();
    }
    return result + ";";
}

// MethodDeclaration implementation
void MethodDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String MethodDeclaration::toString() const {
    String result;
    if (private_) result += "private ";
    else if (protected_) result += "protected ";
    else if (public_) result += "public ";
    
    if (static_) result += "static ";
    if (async_) result += "async ";
    if (generator_) result += "* ";
    
    result += name_ + "(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) result += ", ";
        result += parameters_[i]->getName();
    }
    result += ")";
    if (returnType_) {
        result += ": " + returnType_->toString();
    }
    result += " { ... }";
    return result;
}

// ConstructorDeclaration implementation
void ConstructorDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String ConstructorDeclaration::toString() const {
    String result;
    if (private_) result += "private ";
    else if (protected_) result += "protected ";
    else if (public_) result += "public ";
    
    result += "constructor(";
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) result += ", ";
        result += parameters_[i]->getName();
    }
    result += ") { ... }";
    return result;
}

// DestructorDeclaration implementation
void DestructorDeclaration::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

String DestructorDeclaration::toString() const {
    String result;
    if (private_) result += "private ";
    else if (protected_) result += "protected ";
    else if (public_) result += "public ";
    
    result += "destructor() { ... }";
    return result;
}

} // namespace declarations
} // namespace ast
} // namespace tsc