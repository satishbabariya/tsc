#pragma once

#include "tsc/ast/core/ASTCore.h"
#include "tsc/ast/expressions/ExpressionNodes.h"
#include "tsc/ast/statements/StatementNodes.h"

namespace tsc {
namespace ast {
namespace declarations {

// Forward declarations
class Parameter;
class TypeParameter;

// Parameter class
class Parameter {
public:
    Parameter(const String& name, shared_ptr<Type> type, bool optional = false, bool rest = false, const SourceLocation& loc = SourceLocation())
        : name_(name), type_(type), optional_(optional), rest_(rest), location_(loc) {}
    
    const String& getName() const { return name_; }
    shared_ptr<Type> getType() const { return type_; }
    bool isOptional() const { return optional_; }
    bool isRest() const { return rest_; }
    SourceLocation getLocation() const { return location_; }

private:
    String name_;
    shared_ptr<Type> type_;
    bool optional_;
    bool rest_;
    SourceLocation location_;
};

// Type parameter class
class TypeParameter {
public:
    TypeParameter(const String& name, shared_ptr<Type> constraint = nullptr, const SourceLocation& loc = SourceLocation())
        : name_(name), constraint_(constraint), location_(loc) {}
    
    const String& getName() const { return name_; }
    shared_ptr<Type> getConstraint() const { return constraint_; }
    SourceLocation getLocation() const { return location_; }

private:
    String name_;
    shared_ptr<Type> constraint_;
    SourceLocation location_;
};

// Variable declaration
class VariableDeclaration : public Declaration {
public:
    enum class Kind {
        Var, Let, Const
    };
    
    VariableDeclaration(Kind kind, const String& name, unique_ptr<Expression> initializer, shared_ptr<Type> typeAnnotation, const SourceLocation& loc)
        : kind_(kind), name_(name), initializer_(std::move(initializer)), typeAnnotation_(typeAnnotation), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    Kind getKind() const { return kind_; }
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
    FunctionDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, std::vector<unique_ptr<Parameter>> parameters, shared_ptr<Type> returnType, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool async = false, bool generator = false)
        : name_(name), typeParameters_(std::move(typeParameters)), parameters_(std::move(parameters)), returnType_(returnType), body_(std::move(body)), location_(loc), async_(async), generator_(generator) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<unique_ptr<TypeParameter>>& getTypeParameters() const { return typeParameters_; }
    const std::vector<unique_ptr<Parameter>>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    BlockStatement* getBody() const { return body_.get(); }
    bool isAsync() const { return async_; }
    bool isGenerator() const { return generator_; }

private:
    String name_;
    std::vector<unique_ptr<TypeParameter>> typeParameters_;
    std::vector<unique_ptr<Parameter>> parameters_;
    shared_ptr<Type> returnType_;
    unique_ptr<BlockStatement> body_;
    SourceLocation location_;
    bool async_;
    bool generator_;
};

// Class declaration
class ClassDeclaration : public Declaration {
public:
    ClassDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, unique_ptr<Expression> baseClass, std::vector<unique_ptr<Statement>> members, const SourceLocation& loc, bool abstract = false)
        : name_(name), typeParameters_(std::move(typeParameters)), baseClass_(std::move(baseClass)), members_(std::move(members)), location_(loc), abstract_(abstract) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<unique_ptr<TypeParameter>>& getTypeParameters() const { return typeParameters_; }
    Expression* getBaseClass() const { return baseClass_.get(); }
    const std::vector<unique_ptr<Statement>>& getMembers() const { return members_; }
    bool isAbstract() const { return abstract_; }

private:
    String name_;
    std::vector<unique_ptr<TypeParameter>> typeParameters_;
    unique_ptr<Expression> baseClass_;
    std::vector<unique_ptr<Statement>> members_;
    SourceLocation location_;
    bool abstract_;
};

// Interface declaration
class InterfaceDeclaration : public Declaration {
public:
    InterfaceDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, std::vector<unique_ptr<Expression>> extends, std::vector<unique_ptr<Statement>> members, const SourceLocation& loc)
        : name_(name), typeParameters_(std::move(typeParameters)), extends_(std::move(extends)), members_(std::move(members)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<unique_ptr<TypeParameter>>& getTypeParameters() const { return typeParameters_; }
    const std::vector<unique_ptr<Expression>>& getExtends() const { return extends_; }
    const std::vector<unique_ptr<Statement>>& getMembers() const { return members_; }

private:
    String name_;
    std::vector<unique_ptr<TypeParameter>> typeParameters_;
    std::vector<unique_ptr<Expression>> extends_;
    std::vector<unique_ptr<Statement>> members_;
    SourceLocation location_;
};

// Enum declaration
class EnumMember {
public:
    EnumMember(const String& name, unique_ptr<Expression> value, const SourceLocation& loc)
        : name_(name), value_(std::move(value)), location_(loc) {}
    
    const String& getName() const { return name_; }
    Expression* getValue() const { return value_.get(); }
    SourceLocation getLocation() const { return location_; }

private:
    String name_;
    unique_ptr<Expression> value_;
    SourceLocation location_;
};

class EnumDeclaration : public Declaration {
public:
    enum class Kind {
        Numeric, String, Computed
    };
    
    EnumDeclaration(const String& name, std::vector<unique_ptr<EnumMember>> members, const SourceLocation& loc, Kind kind = Kind::Numeric)
        : name_(name), members_(std::move(members)), location_(loc), kind_(kind) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<unique_ptr<EnumMember>>& getMembers() const { return members_; }
    Kind getKind() const { return kind_; }

private:
    String name_;
    std::vector<unique_ptr<EnumMember>> members_;
    SourceLocation location_;
    Kind kind_;
};

// Type alias declaration
class TypeAliasDeclaration : public Declaration {
public:
    TypeAliasDeclaration(const String& name, std::vector<unique_ptr<TypeParameter>> typeParameters, shared_ptr<Type> type, const SourceLocation& loc)
        : name_(name), typeParameters_(std::move(typeParameters)), type_(type), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<unique_ptr<TypeParameter>>& getTypeParameters() const { return typeParameters_; }
    shared_ptr<Type> getType() const { return type_; }

private:
    String name_;
    std::vector<unique_ptr<TypeParameter>> typeParameters_;
    shared_ptr<Type> type_;
    SourceLocation location_;
};

// Import declaration
class ImportDeclaration : public Declaration {
public:
    ImportDeclaration(const String& moduleSpecifier, std::vector<unique_ptr<ImportClause>> importClauses, const SourceLocation& loc)
        : moduleSpecifier_(moduleSpecifier), importClauses_(std::move(importClauses)), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return moduleSpecifier_; }
    String toString() const override;
    
    const String& getModuleSpecifier() const { return moduleSpecifier_; }
    const std::vector<unique_ptr<ImportClause>>& getImportClauses() const { return importClauses_; }

private:
    String moduleSpecifier_;
    std::vector<unique_ptr<ImportClause>> importClauses_;
    SourceLocation location_;
};

// Export declaration
class ExportDeclaration : public Declaration {
public:
    ExportDeclaration(const ExportClause& clause, const SourceLocation& loc)
        : clause_(clause), location_(loc) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return "export"; }
    String toString() const override;
    
    const ExportClause& getClause() const { return clause_; }

private:
    ExportClause clause_;
    SourceLocation location_;
};

// Property declaration
class PropertyDeclaration : public Declaration {
public:
    PropertyDeclaration(const String& name, shared_ptr<Type> typeAnnotation, unique_ptr<Expression> initializer, const SourceLocation& loc, bool readonly = false, bool static_ = false, bool public_ = true, bool private_ = false, bool protected_ = false)
        : name_(name), typeAnnotation_(typeAnnotation), initializer_(std::move(initializer)), location_(loc), readonly_(readonly), static_(static_), public_(public_), private_(private_), protected_(protected_) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    shared_ptr<Type> getTypeAnnotation() const { return typeAnnotation_; }
    Expression* getInitializer() const { return initializer_.get(); }
    bool isReadonly() const { return readonly_; }
    bool isStatic() const { return static_; }
    bool isPublic() const { return public_; }
    bool isPrivate() const { return private_; }
    bool isProtected() const { return protected_; }

private:
    String name_;
    shared_ptr<Type> typeAnnotation_;
    unique_ptr<Expression> initializer_;
    SourceLocation location_;
    bool readonly_;
    bool static_;
    bool public_;
    bool private_;
    bool protected_;
};

// Method declaration
class MethodDeclaration : public Declaration {
public:
    MethodDeclaration(const String& name, std::vector<unique_ptr<Parameter>> parameters, shared_ptr<Type> returnType, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool async = false, bool generator = false, bool static_ = false, bool public_ = true, bool private_ = false, bool protected_ = false)
        : name_(name), parameters_(std::move(parameters)), returnType_(returnType), body_(std::move(body)), location_(loc), async_(async), generator_(generator), static_(static_), public_(public_), private_(private_), protected_(protected_) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return name_; }
    String toString() const override;
    
    const std::vector<unique_ptr<Parameter>>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    BlockStatement* getBody() const { return body_.get(); }
    bool isAsync() const { return async_; }
    bool isGenerator() const { return generator_; }
    bool isStatic() const { return static_; }
    bool isPublic() const { return public_; }
    bool isPrivate() const { return private_; }
    bool isProtected() const { return protected_; }

private:
    String name_;
    std::vector<unique_ptr<Parameter>> parameters_;
    shared_ptr<Type> returnType_;
    unique_ptr<BlockStatement> body_;
    SourceLocation location_;
    bool async_;
    bool generator_;
    bool static_;
    bool public_;
    bool private_;
    bool protected_;
};

// Constructor declaration
class ConstructorDeclaration : public Declaration {
public:
    ConstructorDeclaration(std::vector<unique_ptr<Parameter>> parameters, unique_ptr<BlockStatement> body, const SourceLocation& loc, bool public_ = true, bool private_ = false, bool protected_ = false)
        : parameters_(std::move(parameters)), body_(std::move(body)), location_(loc), public_(public_), private_(private_), protected_(protected_) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return "constructor"; }
    String toString() const override;
    
    const std::vector<unique_ptr<Parameter>>& getParameters() const { return parameters_; }
    BlockStatement* getBody() const { return body_.get(); }
    bool isPublic() const { return public_; }
    bool isPrivate() const { return private_; }
    bool isProtected() const { return protected_; }

private:
    std::vector<unique_ptr<Parameter>> parameters_;
    unique_ptr<BlockStatement> body_;
    SourceLocation location_;
    bool public_;
    bool private_;
    bool protected_;
};

// Destructor declaration
class DestructorDeclaration : public Declaration {
public:
    DestructorDeclaration(unique_ptr<BlockStatement> body, const SourceLocation& loc, bool public_ = true, bool private_ = false, bool protected_ = false)
        : body_(std::move(body)), location_(loc), public_(public_), private_(private_), protected_(protected_) {}
    
    void accept(ASTVisitor& visitor) override;
    SourceLocation getLocation() const override { return location_; }
    String getName() const override { return "destructor"; }
    String toString() const override;
    
    BlockStatement* getBody() const { return body_.get(); }
    bool isPublic() const { return public_; }
    bool isPrivate() const { return private_; }
    bool isProtected() const { return protected_; }

private:
    unique_ptr<BlockStatement> body_;
    SourceLocation location_;
    bool public_;
    bool private_;
    bool protected_;
};

} // namespace declarations
} // namespace ast
} // namespace tsc