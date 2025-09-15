#pragma once

#include "tsc/ast/AST.h"
#include <vector>
#include <string>

namespace tsc {
namespace semantic {

class SemanticAnalyzer;

// =============================================================================
// CONSTRAINT TYPES
// =============================================================================

enum class ConstraintType {
    Extends,
    Implements,
    Constructor
};

class Constraint {
public:
    virtual ~Constraint() = default;
    virtual ConstraintType getType() const = 0;
};

class ExtendsConstraint : public Constraint {
private:
    Type* baseType_;
    
public:
    ExtendsConstraint(Type* baseType) : baseType_(baseType) {}
    
    ConstraintType getType() const override { return ConstraintType::Extends; }
    Type* getBaseType() const { return baseType_; }
};

class ImplementsConstraint : public Constraint {
private:
    Type* interfaceType_;
    
public:
    ImplementsConstraint(Type* interfaceType) : interfaceType_(interfaceType) {}
    
    ConstraintType getType() const override { return ConstraintType::Implements; }
    Type* getInterfaceType() const { return interfaceType_; }
};

class ConstructorConstraint : public Constraint {
public:
    ConstructorConstraint() = default;
    
    ConstraintType getType() const override { return ConstraintType::Constructor; }
};

// =============================================================================
// TYPE PARAMETER WITH CONSTRAINTS
// =============================================================================

class TypeParameter {
private:
    std::string name_;
    std::vector<Constraint*> constraints_;
    
public:
    TypeParameter(const std::string& name) : name_(name) {}
    ~TypeParameter() {
        for (auto& constraint : constraints_) {
            delete constraint;
        }
    }
    
    const std::string& getName() const { return name_; }
    const std::vector<Constraint*>& getConstraints() const { return constraints_; }
    
    void addConstraint(Constraint* constraint) {
        constraints_.push_back(constraint);
    }
    
    bool hasConstraint(ConstraintType type) const {
        for (auto& constraint : constraints_) {
            if (constraint->getType() == type) {
                return true;
            }
        }
        return false;
    }
};

// =============================================================================
// GENERIC CONSTRAINTS ANALYZER
// =============================================================================

class GenericConstraintsAnalyzer {
private:
    SemanticAnalyzer* analyzer_;
    
    // Constraint validation methods
    bool validateConstraint(Constraint* constraint);
    bool validateExtendsConstraint(Constraint* constraint);
    bool validateImplementsConstraint(Constraint* constraint);
    bool validateConstructorConstraint(Constraint* constraint);
    bool checkConstraintCompatibility(const std::vector<Constraint*>& constraints);
    
    // Method and class constraint analysis
    bool analyzeMethodBodyConstraints(GenericMethodDeclaration* method);
    bool analyzeMemberConstraints(ASTNode* member, const std::vector<TypeParameter*>& typeParams);
    bool analyzeMethodConstraints(MethodDeclaration* method, const std::vector<TypeParameter*>& typeParams);
    bool analyzePropertyConstraints(PropertyDeclaration* property, const std::vector<TypeParameter*>& typeParams);
    bool analyzeParameterConstraints(Parameter* param, const std::vector<TypeParameter*>& typeParams);
    
    // Type constraint analysis
    bool analyzeTypeConstraints(Type* type, const std::vector<TypeParameter*>& typeParams);
    bool analyzeGenericTypeConstraints(GenericType* genericType, const std::vector<TypeParameter*>& typeParams);
    bool analyzeArrayTypeConstraints(ArrayType* arrayType, const std::vector<TypeParameter*>& typeParams);
    bool analyzeUnionTypeConstraints(UnionType* unionType, const std::vector<TypeParameter*>& typeParams);
    bool analyzeIntersectionTypeConstraints(IntersectionType* intersectionType, const std::vector<TypeParameter*>& typeParams);
    
    // Node constraint analysis
    bool analyzeNodeConstraints(ASTNode* node, const std::vector<TypeParameter*>& typeParams);
    bool analyzeCallExpressionConstraints(CallExpression* callExpr, const std::vector<TypeParameter*>& typeParams);
    bool analyzeMemberExpressionConstraints(MemberExpression* memberExpr, const std::vector<TypeParameter*>& typeParams);
    bool analyzeNewExpressionConstraints(NewExpression* newExpr, const std::vector<TypeParameter*>& typeParams);
    
    // Utility methods
    TypeParameter* findTypeParameter(const std::string& name, const std::vector<TypeParameter*>& typeParams);
    
    // Generic method call validation
    bool validateGenericMethodCall(CallExpression* callExpr, const std::vector<Type*>& typeArguments);
    bool validateTypeArgumentAgainstConstraints(Type* typeArg, TypeParameter* typeParam);
    bool validateTypeArgumentAgainstConstraint(Type* typeArg, Constraint* constraint);
    bool validateExtendsConstraintForType(Type* typeArg, ExtendsConstraint* constraint);
    bool validateImplementsConstraintForType(Type* typeArg, ImplementsConstraint* constraint);
    bool validateConstructorConstraintForType(Type* typeArg, ConstructorConstraint* constraint);
    
    // Type relation checking
    bool checkSubtypeRelation(Type* subtype, Type* supertype);
    bool checkImplementsRelation(Type* type, Type* interface);
    MethodDeclaration* getMethodFromCall(CallExpression* callExpr);
    
public:
    GenericConstraintsAnalyzer(SemanticAnalyzer* analyzer);
    
    // Main analysis methods
    bool analyzeGenericConstraints(ASTNode* node);
    bool analyzeGenericMethodConstraints(GenericMethodDeclaration* method);
    bool analyzeGenericClassConstraints(GenericClassDeclaration* classDecl);
    bool analyzeGenericInterfaceConstraints(GenericInterfaceDeclaration* interfaceDecl);
    bool analyzeTypeParameterConstraints(TypeParameter* typeParam);
};

} // namespace semantic
} // namespace tsc