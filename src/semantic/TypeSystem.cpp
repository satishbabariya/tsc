#include "tsc/semantic/TypeSystem.h"
#include "tsc/AST.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace tsc {

// Type base class implementation
bool Type::isAssignableTo(const Type& other) const {
    // Default implementation: types are assignable if they're equivalent
    return isEquivalentTo(other) || other.isAny() || isAny();
}

bool Type::isEquivalentTo(const Type& other) const {
    // Default implementation: types are equivalent if they're the same kind
    return kind_ == other.kind_;
}

bool Type::isSubtypeOf(const Type& other) const {
    // Default implementation: same as assignability
    return isAssignableTo(other);
}

bool Type::isPrimitive() const {
    return kind_ == TypeKind::Number || kind_ == TypeKind::String || 
           kind_ == TypeKind::Boolean || kind_ == TypeKind::Null ||
           kind_ == TypeKind::Undefined || kind_ == TypeKind::Void;
}

bool Type::isComposite() const {
    return kind_ == TypeKind::Array || kind_ == TypeKind::Tuple ||
           kind_ == TypeKind::Object || kind_ == TypeKind::Function;
}

bool Type::isNullable() const {
    return kind_ == TypeKind::Null || kind_ == TypeKind::Undefined;
}

bool Type::isCallable() const {
    return kind_ == TypeKind::Function || kind_ == TypeKind::Any;
}

bool Type::isIndexable() const {
    return kind_ == TypeKind::Array || kind_ == TypeKind::Tuple || kind_ == TypeKind::String;
}

String Type::toDebugString() const {
    return toString() + " (kind: " + std::to_string(static_cast<int>(kind_)) + ")";
}

// PrimitiveType implementation
String PrimitiveType::toString() const {
    switch (kind_) {
        case TypeKind::Number: return "number";
        case TypeKind::String: return "string";
        case TypeKind::Boolean: return "boolean";
        case TypeKind::Null: return "null";
        case TypeKind::Undefined: return "undefined";
        case TypeKind::Void: return "void";
        case TypeKind::Never: return "never";
        case TypeKind::Any: return "any";
        case TypeKind::Unknown: return "unknown";
        default: return "unknown-primitive";
    }
}

// LiteralType implementation
String LiteralType::toString() const {
    switch (kind_) {
        case TypeKind::String: return "\"" + value_ + "\"";
        case TypeKind::Number: return value_;
        case TypeKind::Boolean: return value_;
        default: return value_;
    }
}

bool LiteralType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != kind_) {
        return false;
    }
    
    const auto* otherLiteral = static_cast<const LiteralType*>(&other);
    return value_ == otherLiteral->value_;
}

// ArrayType implementation
String ArrayType::toString() const {
    return elementType_->toString() + "[]";
}

bool ArrayType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Array) {
        return false;
    }
    
    const auto* otherArray = static_cast<const ArrayType*>(&other);
    return elementType_->isEquivalentTo(*otherArray->elementType_);
}

// TupleType implementation
String TupleType::toString() const {
    std::stringstream ss;
    ss << "[";
    
    for (size_t i = 0; i < elementTypes_.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << elementTypes_[i]->toString();
    }
    
    ss << "]";
    return ss.str();
}

bool TupleType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Tuple) {
        return false;
    }
    
    const auto* otherTuple = static_cast<const TupleType*>(&other);
    if (elementTypes_.size() != otherTuple->elementTypes_.size()) {
        return false;
    }
    
    for (size_t i = 0; i < elementTypes_.size(); ++i) {
        if (!elementTypes_[i]->isEquivalentTo(*otherTuple->elementTypes_[i])) {
            return false;
        }
    }
    
    return true;
}

// FunctionType implementation
String FunctionType::toString() const {
    std::stringstream ss;
    ss << "(";
    
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) ss << ", ";
        if (parameters_[i].rest) ss << "...";
        ss << parameters_[i].name << ": " << parameters_[i].type->toString();
        if (parameters_[i].optional) ss << "?";
    }
    
    ss << ") => " << returnType_->toString();
    return ss.str();
}

bool FunctionType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Function) {
        return false;
    }
    
    const auto* otherFunc = static_cast<const FunctionType*>(&other);
    
    // Check return types
    if (!returnType_->isEquivalentTo(*otherFunc->returnType_)) {
        return false;
    }
    
    // Check parameters
    if (parameters_.size() != otherFunc->parameters_.size()) {
        return false;
    }
    
    for (size_t i = 0; i < parameters_.size(); ++i) {
        const auto& param1 = parameters_[i];
        const auto& param2 = otherFunc->parameters_[i];
        
        if (!param1.type->isEquivalentTo(*param2.type) ||
            param1.optional != param2.optional ||
            param1.rest != param2.rest) {
            return false;
        }
    }
    
    return true;
}

// ObjectType implementation
ObjectType::Property* ObjectType::findProperty(const String& name) {
    auto it = std::find_if(properties_.begin(), properties_.end(),
        [&name](const Property& prop) { return prop.name == name; });
    return it != properties_.end() ? &(*it) : nullptr;
}

const ObjectType::Property* ObjectType::findProperty(const String& name) const {
    auto it = std::find_if(properties_.begin(), properties_.end(),
        [&name](const Property& prop) { return prop.name == name; });
    return it != properties_.end() ? &(*it) : nullptr;
}

String ObjectType::toString() const {
    if (properties_.empty()) {
        return "{}";
    }
    
    std::stringstream ss;
    ss << "{ ";
    
    for (size_t i = 0; i < properties_.size(); ++i) {
        if (i > 0) ss << ", ";
        if (properties_[i].readonly) ss << "readonly ";
        ss << properties_[i].name;
        if (properties_[i].optional) ss << "?";
        ss << ": " << properties_[i].type->toString();
    }
    
    ss << " }";
    return ss.str();
}

bool ObjectType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Object) {
        return false;
    }
    
    const auto* otherObj = static_cast<const ObjectType*>(&other);
    if (properties_.size() != otherObj->properties_.size()) {
        return false;
    }
    
    // Check that all properties match
    for (const auto& prop : properties_) {
        const auto* otherProp = otherObj->findProperty(prop.name);
        if (!otherProp ||
            !prop.type->isEquivalentTo(*otherProp->type) ||
            prop.optional != otherProp->optional ||
            prop.readonly != otherProp->readonly) {
            return false;
        }
    }
    
    return true;
}

// UnionType implementation
bool UnionType::isAssignableTo(const Type& other) const {
    // A union type is assignable to another type if all constituent types are assignable
    return std::all_of(types_.begin(), types_.end(),
        [&other](const shared_ptr<Type>& type) {
            return type->isAssignableTo(other);
        });
}

String UnionType::toString() const {
    std::stringstream ss;
    for (size_t i = 0; i < types_.size(); ++i) {
        if (i > 0) ss << " | ";
        ss << types_[i]->toString();
    }
    return ss.str();
}

// IntersectionType implementation
bool IntersectionType::isAssignableTo(const Type& other) const {
    // An intersection type is assignable to another type if any constituent type is assignable
    return std::any_of(types_.begin(), types_.end(),
        [&other](const shared_ptr<Type>& type) {
            return type->isAssignableTo(other);
        });
}

String IntersectionType::toString() const {
    std::stringstream ss;
    for (size_t i = 0; i < types_.size(); ++i) {
        if (i > 0) ss << " & ";
        ss << types_[i]->toString();
    }
    return ss.str();
}

// TypeSystem implementation
TypeSystem::TypeSystem() {
    initializeBuiltinTypes();
}

TypeSystem::~TypeSystem() = default;

void TypeSystem::initializeBuiltinTypes() {
    numberType_ = make_shared<PrimitiveType>(TypeKind::Number);
    stringType_ = make_shared<PrimitiveType>(TypeKind::String);
    booleanType_ = make_shared<PrimitiveType>(TypeKind::Boolean);
    nullType_ = make_shared<PrimitiveType>(TypeKind::Null);
    undefinedType_ = make_shared<PrimitiveType>(TypeKind::Undefined);
    voidType_ = make_shared<PrimitiveType>(TypeKind::Void);
    anyType_ = make_shared<PrimitiveType>(TypeKind::Any);
    unknownType_ = make_shared<PrimitiveType>(TypeKind::Unknown);
    neverType_ = make_shared<PrimitiveType>(TypeKind::Never);
    errorType_ = make_shared<ErrorType>();
}

shared_ptr<Type> TypeSystem::createArrayType(shared_ptr<Type> elementType) const {
    return make_shared<ArrayType>(elementType);
}

shared_ptr<Type> TypeSystem::createTupleType(std::vector<shared_ptr<Type>> elementTypes) const {
    return make_shared<TupleType>(std::move(elementTypes));
}

shared_ptr<Type> TypeSystem::createFunctionType(std::vector<FunctionType::Parameter> parameters, 
                                               shared_ptr<Type> returnType) const {
    return make_shared<FunctionType>(std::move(parameters), returnType);
}

shared_ptr<Type> TypeSystem::createObjectType(std::vector<ObjectType::Property> properties) const {
    return make_shared<ObjectType>(std::move(properties));
}

shared_ptr<Type> TypeSystem::createUnionType(std::vector<shared_ptr<Type>> types) const {
    return make_shared<UnionType>(std::move(types));
}

shared_ptr<Type> TypeSystem::createIntersectionType(std::vector<shared_ptr<Type>> types) const {
    return make_shared<IntersectionType>(std::move(types));
}

shared_ptr<Type> TypeSystem::createLiteralType(TypeKind kind, const String& value) const {
    return make_shared<LiteralType>(kind, value);
}

// Generic type creation methods
shared_ptr<Type> TypeSystem::createTypeParameter(const String& name, shared_ptr<Type> constraint) const {
    return make_shared<TypeParameterType>(name, constraint);
}

shared_ptr<Type> TypeSystem::createGenericType(shared_ptr<Type> baseType, std::vector<shared_ptr<Type>> typeArguments) const {
    return make_shared<GenericType>(baseType, std::move(typeArguments));
}

shared_ptr<Type> TypeSystem::instantiateGenericType(shared_ptr<Type> genericType, 
                                                   const std::vector<shared_ptr<Type>>& typeArguments) const {
    // Perform proper type substitution for generic types
    if (auto generic = dynamic_cast<GenericType*>(genericType.get())) {
        // Create a substitution map from type parameters to type arguments
        std::unordered_map<String, shared_ptr<Type>> substitutionMap;
        const auto& typeParams = generic->getTypeParameters();
        
        if (typeParams.size() != typeArguments.size()) {
            // This should be caught earlier, but handle gracefully
            return genericType;
        }
        
        // Build substitution map
        for (size_t i = 0; i < typeParams.size(); ++i) {
            substitutionMap[typeParams[i].getName()] = typeArguments[i];
        }
        
        // Perform substitution on the base type
        return substituteType(generic->getBaseType(), substitutionMap);
    }
    
    // If it's not a generic type, just return it as-is
    return genericType;
}

// Class type creation method
shared_ptr<Type> TypeSystem::createClassType(const String& name, ClassDeclaration* declaration, 
                                            shared_ptr<Type> baseClass) const {
    // Create a cache key for the class type
    String cacheKey = "class:" + name;
    
    // Check if we already have a canonical instance for this class
    auto it = typeCache_.find(cacheKey);
    if (it != typeCache_.end()) {
        // Return the existing canonical instance, but update its properties if needed
        auto existingClassType = std::static_pointer_cast<ClassType>(it->second);
        
        // Update declaration pointer if it was missing and now provided
        if (!existingClassType->getDeclaration() && declaration) {
            existingClassType->setDeclaration(declaration);
        }
        
        // Update base class if it was missing and now provided
        if (!existingClassType->getBaseClass() && baseClass) {
            existingClassType->setBaseClass(baseClass);
        }
        
        return existingClassType;
    }
    
    // Create new canonical instance and cache it
    auto classType = make_shared<ClassType>(name, declaration, baseClass);
    typeCache_[cacheKey] = classType;
    return classType;
}

shared_ptr<Type> TypeSystem::createInterfaceType(const String& name, InterfaceDeclaration* declaration) const {
    return make_shared<InterfaceType>(name, declaration);
}

shared_ptr<Type> TypeSystem::createEnumType(const String& name, EnumDeclaration* declaration) const {
    return make_shared<EnumType>(name, declaration);
}

shared_ptr<Type> TypeSystem::createAliasType(const String& name, shared_ptr<Type> aliasedType, TypeAliasDeclaration* declaration) const {
    return make_shared<AliasType>(name, aliasedType, declaration);
}

bool TypeSystem::areTypesCompatible(const Type& from, const Type& to) const {
    return from.isAssignableTo(to);
}

shared_ptr<Type> TypeSystem::getCommonType(const Type& type1, const Type& type2) const {
    if (type1.isEquivalentTo(type2)) {
        // Return a copy of type1 (they're equivalent)
        return numberType_; // Placeholder - would need proper type cloning
    }
    
    // If one is assignable to the other, return the more general type
    if (type1.isAssignableTo(type2)) {
        return numberType_; // Placeholder
    }
    if (type2.isAssignableTo(type1)) {
        return numberType_; // Placeholder
    }
    
    // Otherwise, create a union type
    std::vector<shared_ptr<Type>> types = {numberType_, stringType_}; // Placeholder
    return make_shared<UnionType>(std::move(types));
}

shared_ptr<Type> TypeSystem::widenType(const Type& type) const {
    // Convert literal types to their base types
    switch (type.getKind()) {
        case TypeKind::Literal: {
            const auto* literal = static_cast<const LiteralType*>(&type);
            if (literal->toString().front() == '"') {
                return stringType_;
            }
            if (literal->toString() == "true" || literal->toString() == "false") {
                return booleanType_;
            }
            return numberType_;
        }
        default:
            return numberType_; // Placeholder - return the type as-is
    }
}

shared_ptr<Type> TypeSystem::inferTypeFromLiteral(const ASTNode& literal) const {
    if (auto numLit = dynamic_cast<const NumericLiteral*>(&literal)) {
        return numberType_;
    }
    if (auto strLit = dynamic_cast<const StringLiteral*>(&literal)) {
        return stringType_;
    }
    if (auto boolLit = dynamic_cast<const BooleanLiteral*>(&literal)) {
        return booleanType_;
    }
    if (dynamic_cast<const NullLiteral*>(&literal)) {
        return nullType_;
    }
    
    return errorType_;
}

shared_ptr<Type> TypeSystem::inferTypeFromBinaryExpression(const Type& leftType, const Type& rightType, 
                                                          int op) const {
    // Enhanced type inference with proper enum value mapping
    // Enums can be used in arithmetic operations and comparisons
    
    // Arithmetic operations
    if (op >= 0 && op <= 5) { // Add, Subtract, Multiply, Divide, etc.
        // Check for string concatenation (including constrained type parameters)
        if (leftType.isAssignableTo(*stringType_) || rightType.isAssignableTo(*stringType_)) {
            return stringType_; // String concatenation
        }
        
        // Handle enum types - they can participate in arithmetic operations
        bool leftIsEnum = leftType.isEnum() || (leftType.isNumber() && leftType.getKind() == TypeKind::Enum);
        bool rightIsEnum = rightType.isEnum() || (rightType.isNumber() && rightType.getKind() == TypeKind::Enum);
        
        // Check if both types are assignable to number (including constrained type parameters and enums)
        if ((leftType.isAssignableTo(*numberType_) || leftIsEnum) && 
            (rightType.isAssignableTo(*numberType_) || rightIsEnum)) {
            return numberType_;
        }
        
        // Allow operations with 'any'
        if (leftType.isAny() || rightType.isAny()) {
            if (leftType.isString() || rightType.isString()) {
                return stringType_;
            }
            return numberType_; // Default for any operations
        }
    }
    
    // Comparison operations  
    if (op >= 6 && op <= 13) { // Equal, NotEqual, StrictEqual, StrictNotEqual, Less, Greater, LessEqual, GreaterEqual
        return booleanType_;
    }
    
    // Logical operations
    if (op == 14 || op == 15) { // LogicalAnd, LogicalOr
        // Boolean logical operations - these should work with boolean types
        return booleanType_;
    }
    
    return errorType_;
}

shared_ptr<Type> TypeSystem::inferTypeFromUnaryExpression(const Type& operandType, 
                                                         int op) const {
    // Simplified type inference for unary operations
    if (op == 0 || op == 1) { // Plus, Minus
        return numberType_;
    }
    if (op == 2) { // LogicalNot
        return booleanType_;
    }
    if (op == 7) { // Typeof
        return stringType_;
    }
    
    // Most unary operators preserve the operand type
    return numberType_; // Simplified - would need proper type copying
}

String TypeSystem::typeToString(const Type& type) const {
    return type.toString();
}

void TypeSystem::printTypeHierarchy() const {
    std::cout << "=== Type System Hierarchy ===" << std::endl;
    std::cout << "Primitive Types:" << std::endl;
    std::cout << "  - " << numberType_->toString() << std::endl;
    std::cout << "  - " << stringType_->toString() << std::endl;
    std::cout << "  - " << booleanType_->toString() << std::endl;
    std::cout << "  - " << nullType_->toString() << std::endl;
    std::cout << "  - " << undefinedType_->toString() << std::endl;
    std::cout << "  - " << voidType_->toString() << std::endl;
    std::cout << "  - " << anyType_->toString() << std::endl;
    std::cout << "  - " << unknownType_->toString() << std::endl;
    std::cout << "  - " << neverType_->toString() << std::endl;
    std::cout << "=============================" << std::endl;
}

String TypeSystem::getCacheKey(TypeKind kind, const std::vector<shared_ptr<Type>>& types) const {
    std::stringstream ss;
    ss << static_cast<int>(kind);
    for (const auto& type : types) {
        ss << ":" << type->toString();
    }
    return ss.str();
}

bool TypeSystem::isConvertibleToBoolean(shared_ptr<Type> type) const {
    if (!type) return true; // any can be converted to boolean
    
    switch (type->getKind()) {
        case TypeKind::Boolean:
            return true;
        case TypeKind::Number:
        case TypeKind::String:
        case TypeKind::Any:
        case TypeKind::Unknown:
            return true; // These types can be converted to boolean
        case TypeKind::Null:
        case TypeKind::Undefined:
        case TypeKind::Void:
            return true; // These are falsy
        case TypeKind::Object:
        case TypeKind::Array:
        case TypeKind::Function:
            return true; // Objects are truthy
        default:
            return false; // Other types require explicit conversion
    }
}

bool TypeSystem::isArrayType(shared_ptr<Type> type) const {
    return type && type->getKind() == TypeKind::Array;
}

shared_ptr<Type> TypeSystem::getArrayElementType(shared_ptr<Type> arrayType) const {
    if (!isArrayType(arrayType)) {
        return nullptr;
    }
    
    auto array = static_cast<const ArrayType*>(arrayType.get());
    return array->getElementType();
}

// TypeParameterType implementation
bool TypeParameterType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::TypeParameter) {
        return false;
    }
    
    const auto& otherParam = static_cast<const TypeParameterType&>(other);
    // For now, treat all type parameters with the same name as equivalent
    // TODO: In a more sophisticated implementation, we'd check scope context
    // to ensure they're from the same generic declaration
    return name_ == otherParam.name_;
}

bool TypeParameterType::isAssignableTo(const Type& other) const {
    // If there's a constraint, the type parameter is assignable to the constraint
    if (constraint_) {
        return constraint_->isAssignableTo(other);
    }
    
    // If no constraint, use default behavior (equivalent types)
    return isEquivalentTo(other) || other.isAny() || isAny();
}

// GenericType implementation  
String GenericType::toString() const {
    std::ostringstream oss;
    oss << baseType_->toString() << "<";
    
    for (size_t i = 0; i < typeArguments_.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << typeArguments_[i]->toString();
    }
    
    oss << ">";
    return oss.str();
}

bool GenericType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Generic) {
        return false;
    }
    
    const auto& otherGeneric = static_cast<const GenericType&>(other);
    
    if (!baseType_->isEquivalentTo(*otherGeneric.baseType_)) {
        return false;
    }
    
    if (typeArguments_.size() != otherGeneric.typeArguments_.size()) {
        return false;
    }
    
    for (size_t i = 0; i < typeArguments_.size(); ++i) {
        if (!typeArguments_[i]->isEquivalentTo(*otherGeneric.typeArguments_[i])) {
            return false;
        }
    }
    
    return true;
}

bool ClassType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Class) {
        return false;
    }
    
    const auto& otherClass = static_cast<const ClassType&>(other);
    
    // Classes are equivalent if they have the same name
    // In a full implementation, we might also check for structural compatibility
    return name_ == otherClass.name_;
}

// InterfaceType implementation
bool InterfaceType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Interface) {
        return false;
    }
    
    const auto& otherInterface = static_cast<const InterfaceType&>(other);
    
    // Interfaces are equivalent if they have the same name
    // In a full implementation, we might also check for structural compatibility
    return name_ == otherInterface.name_;
}

// EnumType implementation
bool EnumType::isEquivalentTo(const Type& other) const {
    if (other.getKind() != TypeKind::Enum) {
        return false;
    }
    
    const auto& otherEnum = static_cast<const EnumType&>(other);
    
    // Enums are equivalent if they have the same name
    // In a full implementation, we might also check for structural compatibility
    return name_ == otherEnum.name_;
}

// AliasType implementation
bool AliasType::isEquivalentTo(const Type& other) const {
    // Alias types are equivalent if they resolve to equivalent types
    if (other.getKind() == TypeKind::Alias) {
        const auto& otherAlias = static_cast<const AliasType&>(other);
        return aliasedType_->isEquivalentTo(*otherAlias.aliasedType_);
    }
    
    // An alias type is equivalent to its aliased type
    return aliasedType_->isEquivalentTo(other);
}

bool AliasType::isAssignableTo(const Type& to) const {
    // Delegate to the aliased type
    return aliasedType_->isAssignableTo(to);
}

// Factory functions
shared_ptr<Type> createNumberType() {
    return make_shared<PrimitiveType>(TypeKind::Number);
}

shared_ptr<Type> createStringType() {
    return make_shared<PrimitiveType>(TypeKind::String);
}

shared_ptr<Type> createBooleanType() {
    return make_shared<PrimitiveType>(TypeKind::Boolean);
}

shared_ptr<Type> createVoidType() {
    return make_shared<PrimitiveType>(TypeKind::Void);
}

shared_ptr<Type> createAnyType() {
    return make_shared<PrimitiveType>(TypeKind::Any);
}

shared_ptr<Type> createErrorType() {
    return make_shared<ErrorType>();
}

shared_ptr<Type> TypeSystem::createUnresolvedType(const String& name) const {
    return make_shared<UnresolvedType>(name);
}

// SmartPointerType implementation
bool SmartPointerType::isAssignableTo(const Type& other) const {
    if (other.getKind() == TypeKind::Any) return true;
    
    if (auto* otherSmartPtr = dynamic_cast<const SmartPointerType*>(&other)) {
        // Smart pointers are assignable if they have the same kind and element types are assignable
        return kind_ == otherSmartPtr->kind_ && elementType_->isAssignableTo(*otherSmartPtr->elementType_);
    }
    
    return false;
}

bool SmartPointerType::isEquivalentTo(const Type& other) const {
    if (auto* otherSmartPtr = dynamic_cast<const SmartPointerType*>(&other)) {
        return kind_ == otherSmartPtr->kind_ && elementType_->isEquivalentTo(*otherSmartPtr->elementType_);
    }
    return false;
}

bool SmartPointerType::isSubtypeOf(const Type& other) const {
    return isAssignableTo(other);
}

// Smart pointer type creation methods
shared_ptr<Type> TypeSystem::createUniquePtrType(shared_ptr<Type> elementType) const {
    return make_shared<SmartPointerType>(SmartPointerType::Kind::Unique, elementType);
}

shared_ptr<Type> TypeSystem::createSharedPtrType(shared_ptr<Type> elementType) const {
    return make_shared<SmartPointerType>(SmartPointerType::Kind::Shared, elementType);
}

shared_ptr<Type> TypeSystem::createWeakPtrType(shared_ptr<Type> elementType) const {
    return make_shared<SmartPointerType>(SmartPointerType::Kind::Weak, elementType);
}

shared_ptr<Type> TypeSystem::substituteType(shared_ptr<Type> type, 
                                           const std::unordered_map<String, shared_ptr<Type>>& substitutionMap) const {
    if (!type) {
        return nullptr;
    }
    
    // Handle type parameters - these are what we substitute
    if (auto typeParam = dynamic_cast<TypeParameter*>(type.get())) {
        auto it = substitutionMap.find(typeParam->getName());
        if (it != substitutionMap.end()) {
            return it->second;
        }
        return type; // No substitution found, return original
    }
    
    // Handle generic types - substitute in type arguments
    if (auto generic = dynamic_cast<GenericType*>(type.get())) {
        std::vector<shared_ptr<Type>> substitutedArgs;
        for (const auto& arg : generic->getTypeArguments()) {
            substitutedArgs.push_back(substituteType(arg, substitutionMap));
        }
        return createGenericType(substituteType(generic->getBaseType(), substitutionMap), substitutedArgs);
    }
    
    // Handle array types - substitute element type
    if (auto array = dynamic_cast<ArrayType*>(type.get())) {
        return createArrayType(substituteType(array->getElementType(), substitutionMap));
    }
    
    // Handle tuple types - substitute all element types
    if (auto tuple = dynamic_cast<TupleType*>(type.get())) {
        std::vector<shared_ptr<Type>> substitutedElements;
        for (const auto& element : tuple->getElementTypes()) {
            substitutedElements.push_back(substituteType(element, substitutionMap));
        }
        return createTupleType(substitutedElements);
    }
    
    // Handle function types - substitute parameter and return types
    if (auto func = dynamic_cast<FunctionType*>(type.get())) {
        std::vector<FunctionType::Parameter> substitutedParams;
        for (const auto& param : func->getParameters()) {
            substitutedParams.push_back(FunctionType::Parameter{
                param.name,
                substituteType(param.type, substitutionMap),
                param.isOptional
            });
        }
        return createFunctionType(substitutedParams, substituteType(func->getReturnType(), substitutionMap));
    }
    
    // Handle union types - substitute all member types
    if (auto unionType = dynamic_cast<UnionType*>(type.get())) {
        std::vector<shared_ptr<Type>> substitutedMembers;
        for (const auto& member : unionType->getMemberTypes()) {
            substitutedMembers.push_back(substituteType(member, substitutionMap));
        }
        return createUnionType(substitutedMembers);
    }
    
    // Handle intersection types - substitute all member types
    if (auto intersection = dynamic_cast<IntersectionType*>(type.get())) {
        std::vector<shared_ptr<Type>> substitutedMembers;
        for (const auto& member : intersection->getMemberTypes()) {
            substitutedMembers.push_back(substituteType(member, substitutionMap));
        }
        return createIntersectionType(substitutedMembers);
    }
    
    // Handle smart pointer types - substitute element type
    if (auto smartPtr = dynamic_cast<SmartPointerType*>(type.get())) {
        switch (smartPtr->getKind()) {
            case SmartPointerType::Kind::Unique:
                return createUniquePtrType(substituteType(smartPtr->getElementType(), substitutionMap));
            case SmartPointerType::Kind::Shared:
                return createSharedPtrType(substituteType(smartPtr->getElementType(), substitutionMap));
            case SmartPointerType::Kind::Weak:
                return createWeakPtrType(substituteType(smartPtr->getElementType(), substitutionMap));
        }
    }
    
    // For all other types (primitives, classes, interfaces, etc.), no substitution needed
    return type;
}

} // namespace tsc