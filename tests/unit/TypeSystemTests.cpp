#include "tsc/semantic/TypeSystem.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <functional>

using namespace tsc;

// Simple test framework
class TestFramework {
private:
    int testsRun = 0;
    int testsPassed = 0;
    int testsFailed = 0;

public:
    void runTest(const std::string& testName, std::function<bool()> testFunc) {
        testsRun++;
        std::cout << "Running test: " << testName << "... ";
        
        try {
            bool result = testFunc();
            if (result) {
                testsPassed++;
                std::cout << "PASSED" << std::endl;
            } else {
                testsFailed++;
                std::cout << "FAILED" << std::endl;
            }
        } catch (const std::exception& e) {
            testsFailed++;
            std::cout << "FAILED (Exception: " << e.what() << ")" << std::endl;
        }
    }

    void printSummary() {
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Tests run: " << testsRun << std::endl;
        std::cout << "Tests passed: " << testsPassed << std::endl;
        std::cout << "Tests failed: " << testsFailed << std::endl;
        
        if (testsFailed == 0) {
            std::cout << "All tests passed! ✅" << std::endl;
        } else {
            std::cout << "Some tests failed! ❌" << std::endl;
        }
    }
};

// Helper function to create types
std::shared_ptr<Type> makeNumberType() {
    return std::make_shared<PrimitiveType>(TypeKind::Number);
}

std::shared_ptr<Type> makeStringType() {
    return std::make_shared<PrimitiveType>(TypeKind::String);
}

std::shared_ptr<Type> makeBooleanType() {
    return std::make_shared<PrimitiveType>(TypeKind::Boolean);
}

std::shared_ptr<Type> makeAnyType() {
    return std::make_shared<PrimitiveType>(TypeKind::Any);
}

std::shared_ptr<Type> makeNullType() {
    return std::make_shared<PrimitiveType>(TypeKind::Null);
}

std::shared_ptr<Type> makeUndefinedType() {
    return std::make_shared<PrimitiveType>(TypeKind::Undefined);
}

std::shared_ptr<Type> makeVoidType() {
    return std::make_shared<PrimitiveType>(TypeKind::Void);
}

std::shared_ptr<Type> makeObjectType() {
    return std::make_shared<ObjectType>();
}

std::shared_ptr<Type> makeArrayType(std::shared_ptr<Type> elementType = nullptr) {
    if (!elementType) {
        elementType = makeAnyType();
    }
    return std::make_shared<ArrayType>(elementType);
}

std::shared_ptr<Type> makeClassType(const std::string& name) {
    return std::make_shared<ClassType>(name, nullptr);
}

std::shared_ptr<Type> makeInterfaceType(const std::string& name) {
    return std::make_shared<InterfaceType>(name, nullptr);
}

std::shared_ptr<Type> makeUnionType(const std::vector<std::shared_ptr<Type>>& types) {
    return std::make_shared<UnionType>(types);
}

std::shared_ptr<Type> makeIntersectionType(const std::vector<std::shared_ptr<Type>>& types) {
    return std::make_shared<IntersectionType>(types);
}

// Test functions
bool testIsConvertibleToSameType() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    
    // Same types should be convertible
    return typeSystem.isConvertibleTo(numberType, numberType) &&
           typeSystem.isConvertibleTo(stringType, stringType) &&
           typeSystem.isConvertibleTo(booleanType, booleanType);
}

bool testIsConvertibleToAnyType() {
    TypeSystem typeSystem;
    
    auto anyType = makeAnyType();
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    auto objectType = makeObjectType();
    
    // Any type can be converted to any other type
    return typeSystem.isConvertibleTo(anyType, numberType) &&
           typeSystem.isConvertibleTo(numberType, anyType) &&
           typeSystem.isConvertibleTo(anyType, stringType) &&
           typeSystem.isConvertibleTo(stringType, anyType) &&
           typeSystem.isConvertibleTo(anyType, booleanType) &&
           typeSystem.isConvertibleTo(booleanType, anyType) &&
           typeSystem.isConvertibleTo(anyType, objectType) &&
           typeSystem.isConvertibleTo(objectType, anyType);
}

bool testPrimitiveTypeConversions() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    
    // Test primitive type conversions
    return typeSystem.isConvertibleTo(numberType, stringType) &&
           typeSystem.isConvertibleTo(stringType, numberType) &&
           typeSystem.isConvertibleTo(booleanType, numberType) &&
           typeSystem.isConvertibleTo(numberType, booleanType) &&
           typeSystem.isConvertibleTo(stringType, booleanType) &&
           typeSystem.isConvertibleTo(booleanType, stringType);
}

bool testNullUndefinedConversions() {
    TypeSystem typeSystem;
    
    auto nullType = makeNullType();
    auto undefinedType = makeUndefinedType();
    auto objectType = makeObjectType();
    auto classType = makeClassType("TestClass");
    auto interfaceType = makeInterfaceType("TestInterface");
    auto arrayType = makeArrayType();
    
    // Null and undefined should be convertible to object types
    return typeSystem.isConvertibleTo(nullType, objectType) &&
           typeSystem.isConvertibleTo(undefinedType, objectType) &&
           typeSystem.isConvertibleTo(nullType, classType) &&
           typeSystem.isConvertibleTo(undefinedType, classType) &&
           typeSystem.isConvertibleTo(nullType, interfaceType) &&
           typeSystem.isConvertibleTo(undefinedType, interfaceType) &&
           typeSystem.isConvertibleTo(nullType, arrayType) &&
           typeSystem.isConvertibleTo(undefinedType, arrayType);
}

bool testObjectTypeConversions() {
    TypeSystem typeSystem;
    
    auto objectType = makeObjectType();
    auto classType = makeClassType("TestClass");
    auto interfaceType = makeInterfaceType("TestInterface");
    auto arrayType = makeArrayType();
    
    // Object type conversions
    return typeSystem.isConvertibleTo(objectType, objectType) &&
           typeSystem.isConvertibleTo(classType, objectType) &&
           typeSystem.isConvertibleTo(interfaceType, objectType) &&
           typeSystem.isConvertibleTo(arrayType, objectType);
}

bool testUnionTypeConversions() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    auto unionType = makeUnionType({numberType, stringType});
    
    // Union types should be convertible if any member is convertible
    return typeSystem.isConvertibleTo(numberType, unionType) &&
           typeSystem.isConvertibleTo(stringType, unionType) &&
           !typeSystem.isConvertibleTo(booleanType, unionType); // boolean is not in union
}

bool testIntersectionTypeConversions() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    auto intersectionType = makeIntersectionType({numberType, stringType});
    
    // Intersection types should be convertible if any member is convertible
    return typeSystem.isConvertibleTo(numberType, intersectionType) &&
           typeSystem.isConvertibleTo(stringType, intersectionType) &&
           !typeSystem.isConvertibleTo(booleanType, intersectionType); // boolean is not in intersection
}

bool testIsImplicitlyConvertible() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    auto anyType = makeAnyType();
    
    // Test implicit conversions
    return typeSystem.isImplicitlyConvertible(numberType, numberType) && // identity
           typeSystem.isImplicitlyConvertible(numberType, stringType) && // number to string
           typeSystem.isImplicitlyConvertible(stringType, numberType) && // string to number
           typeSystem.isImplicitlyConvertible(booleanType, numberType) && // boolean to number
           typeSystem.isImplicitlyConvertible(numberType, booleanType) && // number to boolean
           typeSystem.isImplicitlyConvertible(stringType, booleanType) && // string to boolean
           typeSystem.isImplicitlyConvertible(booleanType, stringType) && // boolean to string
           typeSystem.isImplicitlyConvertible(anyType, numberType) && // any to number
           typeSystem.isImplicitlyConvertible(numberType, anyType); // number to any
}

bool testIsExplicitlyConvertible() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    auto anyType = makeAnyType();
    
    // All implicit conversions should also be explicit
    return typeSystem.isExplicitlyConvertible(numberType, numberType) &&
           typeSystem.isExplicitlyConvertible(numberType, stringType) &&
           typeSystem.isExplicitlyConvertible(stringType, numberType) &&
           typeSystem.isExplicitlyConvertible(booleanType, numberType) &&
           typeSystem.isExplicitlyConvertible(numberType, booleanType) &&
           typeSystem.isExplicitlyConvertible(stringType, booleanType) &&
           typeSystem.isExplicitlyConvertible(booleanType, stringType) &&
           typeSystem.isExplicitlyConvertible(anyType, numberType) &&
           typeSystem.isExplicitlyConvertible(numberType, anyType);
}

bool testGetConversionKind() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    auto anyType = makeAnyType();
    
    // Test conversion kinds
    return typeSystem.getConversionKind(numberType, numberType) == ConversionKind::Identity &&
           typeSystem.getConversionKind(numberType, stringType) == ConversionKind::Implicit &&
           typeSystem.getConversionKind(stringType, numberType) == ConversionKind::Implicit &&
           typeSystem.getConversionKind(booleanType, numberType) == ConversionKind::Implicit &&
           typeSystem.getConversionKind(numberType, booleanType) == ConversionKind::Implicit &&
           typeSystem.getConversionKind(stringType, booleanType) == ConversionKind::Implicit &&
           typeSystem.getConversionKind(booleanType, stringType) == ConversionKind::Implicit &&
           typeSystem.getConversionKind(anyType, numberType) == ConversionKind::Implicit &&
           typeSystem.getConversionKind(numberType, anyType) == ConversionKind::Implicit;
}

bool testInvalidConversions() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    auto voidType = makeVoidType();
    
    // Test invalid conversions (these should return ConversionKind::Invalid)
    return typeSystem.getConversionKind(numberType, voidType) == ConversionKind::Invalid &&
           typeSystem.getConversionKind(stringType, voidType) == ConversionKind::Invalid &&
           typeSystem.getConversionKind(booleanType, voidType) == ConversionKind::Invalid &&
           typeSystem.getConversionKind(voidType, numberType) == ConversionKind::Invalid &&
           typeSystem.getConversionKind(voidType, stringType) == ConversionKind::Invalid &&
           typeSystem.getConversionKind(voidType, booleanType) == ConversionKind::Invalid;
}

bool testNullPointerHandling() {
    TypeSystem typeSystem;
    
    auto numberType = makeNumberType();
    
    // Test null pointer handling
    return !typeSystem.isConvertibleTo(nullptr, numberType) &&
           !typeSystem.isConvertibleTo(numberType, nullptr) &&
           !typeSystem.isConvertibleTo(nullptr, nullptr) &&
           !typeSystem.isImplicitlyConvertible(nullptr, numberType) &&
           !typeSystem.isImplicitlyConvertible(numberType, nullptr) &&
           !typeSystem.isExplicitlyConvertible(nullptr, numberType) &&
           !typeSystem.isExplicitlyConvertible(numberType, nullptr) &&
           typeSystem.getConversionKind(nullptr, numberType) == ConversionKind::Invalid &&
           typeSystem.getConversionKind(numberType, nullptr) == ConversionKind::Invalid;
}

int main() {
    std::cout << "=== TypeSystem Unit Tests ===" << std::endl;
    
    TestFramework framework;
    
    // Run all tests
    framework.runTest("Same Type Conversion", testIsConvertibleToSameType);
    framework.runTest("Any Type Conversion", testIsConvertibleToAnyType);
    framework.runTest("Primitive Type Conversions", testPrimitiveTypeConversions);
    framework.runTest("Null/Undefined Conversions", testNullUndefinedConversions);
    framework.runTest("Object Type Conversions", testObjectTypeConversions);
    framework.runTest("Union Type Conversions", testUnionTypeConversions);
    framework.runTest("Intersection Type Conversions", testIntersectionTypeConversions);
    framework.runTest("Implicit Conversions", testIsImplicitlyConvertible);
    framework.runTest("Explicit Conversions", testIsExplicitlyConvertible);
    framework.runTest("Conversion Kind Detection", testGetConversionKind);
    framework.runTest("Invalid Conversions", testInvalidConversions);
    framework.runTest("Null Pointer Handling", testNullPointerHandling);
    
    framework.printSummary();
    
    return 0;
}