#pragma once

#include "tsc/Common.h"
#include <vector>
#include <unordered_map>

// Forward declare AST classes to avoid circular dependency
namespace tsc {
    class BinaryExpression;
    class UnaryExpression;
    class DiagnosticEngine;
}

namespace tsc {

// Forward declarations
class ASTNode;
class BinaryExpression;
class UnaryExpression;
class InterfaceDeclaration;
class EnumDeclaration;
class TypeAliasDeclaration;
class GenericConstraintChecker;

// Type categories for the TypeScript type system
enum class TypeKind {
    // Primitive types
    Number,
    String,
    Boolean,
    Null,
    Undefined,
    Void,
    Never,
    Any,
    Unknown,
    
    // Composite types
    Array,
    Tuple,
    Object,
    Unresolved,
    Function,
    Union,
    Intersection,
    
    // TypeScript-specific
    Literal,
    StringLiteral,
    NumericLiteral,
    BooleanLiteral,
    Enum,
    Interface,
    Class,
    TypeParameter,
    Generic,
    Mapped,
    Conditional,
    Alias,
    
    // ARC Memory Management Types
    UniquePtr,
    SharedPtr,
    WeakPtr,
    
    // Special
    Error  // For error recovery
};

// Base class for all types in the type system
class Type {
public:
    explicit Type(TypeKind kind) : kind_(kind) {}
    virtual ~Type() = default;
    
    TypeKind getKind() const { return kind_; }
    
    // Type checking operations
    virtual bool isAssignableTo(const Type& other) const;
    virtual bool isEquivalentTo(const Type& other) const;
    virtual bool isSubtypeOf(const Type& other) const;
    
    // Type properties
    virtual bool isPrimitive() const;
    virtual bool isComposite() const;
    virtual bool isNullable() const;
    virtual bool isCallable() const;
    virtual bool isIndexable() const;
    
    // String representation
    virtual String toString() const = 0;
    virtual String toDebugString() const;
    
    // Type utilities
    bool isNumber() const { return kind_ == TypeKind::Number; }
    bool isString() const { return kind_ == TypeKind::String; }
    bool isBoolean() const { return kind_ == TypeKind::Boolean; }
    bool isNull() const { return kind_ == TypeKind::Null; }
    bool isUndefined() const { return kind_ == TypeKind::Undefined; }
    bool isVoid() const { return kind_ == TypeKind::Void; }
    bool isAny() const { return kind_ == TypeKind::Any; }
    bool isUnknown() const { return kind_ == TypeKind::Unknown; }
    bool isNever() const { return kind_ == TypeKind::Never; }
    bool isError() const { return kind_ == TypeKind::Error; }

protected:
    TypeKind kind_;
};

// Primitive types
class PrimitiveType : public Type {
public:
    explicit PrimitiveType(TypeKind kind) : Type(kind) {}
    
    bool isPrimitive() const override { return true; }
    String toString() const override;
};

// Literal types (string literals, number literals, boolean literals)
class LiteralType : public Type {
public:
    template<typename T>
    LiteralType(TypeKind kind, T value) : Type(kind), value_(std::to_string(value)) {}
    
    LiteralType(TypeKind kind, const String& value) : Type(kind), value_(value) {}
    
    const String& getValue() const { return value_; }
    String toString() const override;
    
    bool isEquivalentTo(const Type& other) const override;

private:
    String value_;
};

// Array types
class ArrayType : public Type {
public:
    explicit ArrayType(shared_ptr<Type> elementType) 
        : Type(TypeKind::Array), elementType_(elementType) {}
    
    shared_ptr<Type> getElementType() const { return elementType_; }
    
    bool isComposite() const override { return true; }
    bool isIndexable() const override { return true; }
    String toString() const override;
    
    bool isEquivalentTo(const Type& other) const override;

private:
    shared_ptr<Type> elementType_;
};

// Tuple types
class TupleType : public Type {
public:
    explicit TupleType(std::vector<shared_ptr<Type>> elementTypes)
        : Type(TypeKind::Tuple), elementTypes_(std::move(elementTypes)) {}
    
    const std::vector<shared_ptr<Type>>& getElementTypes() const { return elementTypes_; }
    size_t getElementCount() const { return elementTypes_.size(); }
    
    bool isComposite() const override { return true; }
    bool isIndexable() const override { return true; }
    String toString() const override;
    
    bool isEquivalentTo(const Type& other) const override;

private:
    std::vector<shared_ptr<Type>> elementTypes_;
};

// Function types
class FunctionType : public Type {
public:
    struct Parameter {
        String name;
        shared_ptr<Type> type;
        bool optional = false;
        bool rest = false;
        
        Parameter() = default;
        Parameter(const String& n, shared_ptr<Type> t, bool opt = false, bool r = false)
            : name(n), type(t), optional(opt), rest(r) {}
    };
    
    FunctionType(std::vector<Parameter> parameters, shared_ptr<Type> returnType)
        : Type(TypeKind::Function), parameters_(std::move(parameters)), returnType_(returnType) {}
    
    const std::vector<Parameter>& getParameters() const { return parameters_; }
    shared_ptr<Type> getReturnType() const { return returnType_; }
    
    bool isCallable() const override { return true; }
    String toString() const override;
    
    bool isEquivalentTo(const Type& other) const override;

private:
    std::vector<Parameter> parameters_;
    shared_ptr<Type> returnType_;
};

// Object types
class ObjectType : public Type {
public:
    struct Property {
        String name;
        shared_ptr<Type> type;
        bool optional = false;
        bool readonly = false;
        
        Property() = default;
        Property(const String& n, shared_ptr<Type> t, bool opt = false, bool ro = false)
            : name(n), type(t), optional(opt), readonly(ro) {}
    };
    
    explicit ObjectType(std::vector<Property> properties = {})
        : Type(TypeKind::Object), properties_(std::move(properties)) {}
    
    const std::vector<Property>& getProperties() const { return properties_; }
    void addProperty(Property property) { properties_.push_back(std::move(property)); }
    
    Property* findProperty(const String& name);
    const Property* findProperty(const String& name) const;
    
    bool isComposite() const override { return true; }
    String toString() const override;
    
    bool isEquivalentTo(const Type& other) const override;

private:
    std::vector<Property> properties_;
};

// Union types (T | U)
class UnionType : public Type {
public:
    explicit UnionType(std::vector<shared_ptr<Type>> types)
        : Type(TypeKind::Union), types_(std::move(types)) {}
    
    const std::vector<shared_ptr<Type>>& getTypes() const { return types_; }
    
    bool isAssignableTo(const Type& other) const override;
    String toString() const override;

private:
    std::vector<shared_ptr<Type>> types_;
};

// Intersection types (T & U)
class IntersectionType : public Type {
public:
    explicit IntersectionType(std::vector<shared_ptr<Type>> types)
        : Type(TypeKind::Intersection), types_(std::move(types)) {}
    
    const std::vector<shared_ptr<Type>>& getTypes() const { return types_; }
    
    bool isAssignableTo(const Type& other) const override;
    String toString() const override;

private:
    std::vector<shared_ptr<Type>> types_;
};

// Error type for error recovery
// Type parameter for generics (T, U, K, V, etc.)
class TypeParameterType : public Type {
public:
    explicit TypeParameterType(const String& name, shared_ptr<Type> constraint = nullptr)
        : Type(TypeKind::TypeParameter), name_(name), constraint_(constraint) {}
    
    const String& getName() const { return name_; }
    shared_ptr<Type> getConstraint() const { return constraint_; }
    
    String toString() const override { return name_; }
    bool isEquivalentTo(const Type& other) const override;
    bool isAssignableTo(const Type& other) const override;

private:
    String name_;
    shared_ptr<Type> constraint_; // For bounded type parameters like T extends string
};

// Generic type instantiation (Array<number>, Map<string, User>, etc.)
class GenericType : public Type {
public:
    GenericType(shared_ptr<Type> baseType, std::vector<shared_ptr<Type>> typeArguments)
        : Type(TypeKind::Generic), baseType_(baseType), typeArguments_(std::move(typeArguments)) {}
    
    shared_ptr<Type> getBaseType() const { return baseType_; }
    const std::vector<shared_ptr<Type>>& getTypeArguments() const { return typeArguments_; }
    
    String toString() const override;
    bool isEquivalentTo(const Type& other) const override;

private:
    shared_ptr<Type> baseType_;
    std::vector<shared_ptr<Type>> typeArguments_;
};

// Forward declaration for ClassDeclaration
class ClassDeclaration;

class ClassType : public Type {
public:
    ClassType(const String& name, ClassDeclaration* declaration = nullptr, shared_ptr<Type> baseClass = nullptr)
        : Type(TypeKind::Class), name_(name), declaration_(declaration), baseClass_(baseClass) {}
    
    const String& getName() const { return name_; }
    ClassDeclaration* getDeclaration() const { return declaration_; }
    shared_ptr<Type> getBaseClass() const { return baseClass_; }
    const std::vector<shared_ptr<Type>>& getInterfaces() const { return interfaces_; }
    
    void setDeclaration(ClassDeclaration* declaration) { declaration_ = declaration; }
    void setBaseClass(shared_ptr<Type> baseClass) { baseClass_ = baseClass; }
    void addInterface(shared_ptr<Type> interface) { interfaces_.push_back(interface); }
    
    String toString() const override { return name_; }
    bool isEquivalentTo(const Type& other) const override;
    bool isStructurallyCompatible(const ClassType& other) const;

private:
    String name_;
    ClassDeclaration* declaration_;
    shared_ptr<Type> baseClass_;
    std::vector<shared_ptr<Type>> interfaces_;
};

// Interface types
class InterfaceType : public Type {
public:
    InterfaceType(const String& name, InterfaceDeclaration* declaration = nullptr)
        : Type(TypeKind::Interface), name_(name), declaration_(declaration) {}
    
    const String& getName() const { return name_; }
    InterfaceDeclaration* getDeclaration() const { return declaration_; }
    const std::vector<shared_ptr<Type>>& getExtends() const { return extends_; }
    
    void setDeclaration(InterfaceDeclaration* declaration) { declaration_ = declaration; }
    void addExtends(shared_ptr<Type> interface) { extends_.push_back(interface); }
    
    String toString() const override { return name_; }
    bool isEquivalentTo(const Type& other) const override;
    bool isStructurallyCompatible(const InterfaceType& other) const;

private:
    String name_;
    InterfaceDeclaration* declaration_;
    std::vector<shared_ptr<Type>> extends_;
};

// Enum types
class EnumType : public Type {
public:
    EnumType(const String& name, EnumDeclaration* declaration = nullptr)
        : Type(TypeKind::Enum), name_(name), declaration_(declaration) {}
    
    const String& getName() const { return name_; }
    EnumDeclaration* getDeclaration() const { return declaration_; }
    
    void setDeclaration(EnumDeclaration* declaration) { declaration_ = declaration; }
    
    String toString() const override { return name_; }
    bool isEquivalentTo(const Type& other) const override;
    bool isStructurallyCompatible(const EnumType& other) const;

private:
    String name_;
    EnumDeclaration* declaration_;
};

// Type alias
class AliasType : public Type {
public:
    AliasType(const String& name, shared_ptr<Type> aliasedType, TypeAliasDeclaration* declaration = nullptr)
        : Type(TypeKind::Alias), name_(name), aliasedType_(aliasedType), declaration_(declaration) {}
    
    const String& getName() const { return name_; }
    shared_ptr<Type> getAliasedType() const { return aliasedType_; }
    TypeAliasDeclaration* getDeclaration() const { return declaration_; }
    
    void setDeclaration(TypeAliasDeclaration* declaration) { declaration_ = declaration; }
    
    String toString() const override { return name_; }
    bool isEquivalentTo(const Type& other) const override;
    bool isAssignableTo(const Type& to) const override;

private:
    String name_;
    shared_ptr<Type> aliasedType_;
    TypeAliasDeclaration* declaration_;
};

// Unresolved type (for identifiers that need to be resolved later)
class UnresolvedType : public Type {
public:
    UnresolvedType(const String& name) : Type(TypeKind::Unresolved), name_(name) {}
    
    const String& getName() const { return name_; }
    
    bool isEquivalentTo(const Type& other) const override {
        if (other.getKind() != TypeKind::Unresolved) return false;
        const auto& otherUnresolved = static_cast<const UnresolvedType&>(other);
        return name_ == otherUnresolved.name_;
    }
    
    String toString() const override {
        return name_;
    }

private:
    String name_;
};

// ARC Memory Management Types
class SmartPointerType : public Type {
public:
    enum class Kind {
        Unique,
        Shared,
        Weak
    };
    
    SmartPointerType(Kind kind, shared_ptr<Type> elementType) 
        : Type(kind == Kind::Unique ? TypeKind::UniquePtr : 
               kind == Kind::Shared ? TypeKind::SharedPtr : TypeKind::WeakPtr),
          kind_(kind), elementType_(elementType) {}
    
    Kind getSmartPointerKind() const { return kind_; }
    shared_ptr<Type> getElementType() const { return elementType_; }
    
    String toString() const override {
        String prefix;
        switch (kind_) {
            case Kind::Unique: prefix = "unique_ptr"; break;
            case Kind::Shared: prefix = "shared_ptr"; break;
            case Kind::Weak: prefix = "weak_ptr"; break;
        }
        return prefix + "<" + elementType_->toString() + ">";
    }
    
    bool isAssignableTo(const Type& other) const override;
    bool isEquivalentTo(const Type& other) const override;
    bool isSubtypeOf(const Type& other) const override;

private:
    Kind kind_;
    shared_ptr<Type> elementType_;
};

class ErrorType : public Type {
public:
    ErrorType() : Type(TypeKind::Error) {}
    String toString() const override { return "error"; }
};

// Type system manager
class TypeSystem {
public:
    TypeSystem();
    ~TypeSystem();
    
    // Built-in types
    shared_ptr<Type> getNumberType() const { return numberType_; }
    shared_ptr<Type> getStringType() const { return stringType_; }
    shared_ptr<Type> getBooleanType() const { return booleanType_; }
    shared_ptr<Type> getNullType() const { return nullType_; }
    shared_ptr<Type> getUndefinedType() const { return undefinedType_; }
    shared_ptr<Type> getVoidType() const { return voidType_; }
    shared_ptr<Type> getAnyType() const { return anyType_; }
    shared_ptr<Type> getUnknownType() const { return unknownType_; }
    shared_ptr<Type> getNeverType() const { return neverType_; }
    shared_ptr<Type> getErrorType() const { return errorType_; }
    
    // Type creation
    shared_ptr<Type> createArrayType(shared_ptr<Type> elementType) const;
    shared_ptr<Type> createTupleType(std::vector<shared_ptr<Type>> elementTypes) const;
    shared_ptr<Type> createFunctionType(std::vector<FunctionType::Parameter> parameters, 
                                       shared_ptr<Type> returnType) const;
    shared_ptr<Type> createObjectType(std::vector<ObjectType::Property> properties = {}) const;
    shared_ptr<Type> createUnionType(std::vector<shared_ptr<Type>> types) const;
    shared_ptr<Type> createIntersectionType(std::vector<shared_ptr<Type>> types) const;
    shared_ptr<Type> createLiteralType(TypeKind kind, const String& value) const;
    shared_ptr<Type> createUnresolvedType(const String& name) const;
    
    // Generic type creation
    shared_ptr<Type> createTypeParameter(const String& name, shared_ptr<Type> constraint = nullptr) const;
    shared_ptr<Type> createGenericType(shared_ptr<Type> baseType, std::vector<shared_ptr<Type>> typeArguments) const;
    shared_ptr<Type> instantiateGenericType(shared_ptr<Type> genericType, 
                                           const std::vector<shared_ptr<Type>>& typeArguments) const;
    
    // Class type creation
    shared_ptr<Type> createClassType(const String& name, ClassDeclaration* declaration = nullptr, 
                                    shared_ptr<Type> baseClass = nullptr) const;
    
    // Interface type creation  
    shared_ptr<Type> createInterfaceType(const String& name, InterfaceDeclaration* declaration = nullptr) const;
    
    // Enum type creation
    shared_ptr<Type> createEnumType(const String& name, EnumDeclaration* declaration = nullptr) const;
    
    // Type alias creation
    shared_ptr<Type> createAliasType(const String& name, shared_ptr<Type> aliasedType, TypeAliasDeclaration* declaration = nullptr) const;
    
    // ARC Memory Management type creation
    shared_ptr<Type> createUniquePtrType(shared_ptr<Type> elementType) const;
    shared_ptr<Type> createSharedPtrType(shared_ptr<Type> elementType) const;
    shared_ptr<Type> createWeakPtrType(shared_ptr<Type> elementType) const;
    
    // Type operations
    bool areTypesCompatible(const Type& from, const Type& to) const;
    shared_ptr<Type> getCommonType(const Type& type1, const Type& type2) const;
    shared_ptr<Type> widenType(const Type& type) const;
    bool isConvertibleToBoolean(shared_ptr<Type> type) const;
    bool isConvertibleToString(shared_ptr<Type> type) const;
    bool isArrayType(shared_ptr<Type> type) const;
    shared_ptr<Type> getArrayElementType(shared_ptr<Type> arrayType) const;
    
    // Type inference
    shared_ptr<Type> inferTypeFromLiteral(const ASTNode& literal) const;
    shared_ptr<Type> inferTypeFromBinaryExpression(const Type& leftType, const Type& rightType, 
                                                   int op) const;
    shared_ptr<Type> inferTypeFromUnaryExpression(const Type& operandType, 
                                                  int op) const;
    
    // Utilities
    String typeToString(const Type& type) const;
    void printTypeHierarchy() const;
    
    // Generic constraint checking
    GenericConstraintChecker* getConstraintChecker() const;
    void setDiagnosticEngine(DiagnosticEngine& diagnostics);

private:
    // Built-in types
    shared_ptr<Type> numberType_;
    shared_ptr<Type> stringType_;
    shared_ptr<Type> booleanType_;
    shared_ptr<Type> nullType_;
    shared_ptr<Type> undefinedType_;
    shared_ptr<Type> voidType_;
    shared_ptr<Type> anyType_;
    shared_ptr<Type> unknownType_;
    shared_ptr<Type> neverType_;
    shared_ptr<Type> errorType_;
    
    // Type cache for performance (mutable to allow caching in const methods)
    mutable std::unordered_map<String, shared_ptr<Type>> typeCache_;
    
    // Generic constraint checker for type substitution
    mutable std::unique_ptr<GenericConstraintChecker> constraintChecker_;
    
    // Diagnostic engine for error reporting
    DiagnosticEngine* diagnostics_;
    
    void initializeBuiltinTypes();
    String getCacheKey(TypeKind kind, const std::vector<shared_ptr<Type>>& types) const;
};

// Factory functions for common types
shared_ptr<Type> createNumberType();
shared_ptr<Type> createStringType();
shared_ptr<Type> createBooleanType();
shared_ptr<Type> createVoidType();
shared_ptr<Type> createAnyType();
shared_ptr<Type> createErrorType();

} // namespace tsc