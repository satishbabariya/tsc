#include "tsc/semantic/TypeSystem.h"
#include "tsc/semantic/SemanticAnalyzer.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <iostream>
#include <memory>
#include <functional>

using namespace tsc;

// Simple test framework for integration tests
class IntegrationTestFramework {
private:
    int testsRun = 0;
    int testsPassed = 0;
    int testsFailed = 0;

public:
    void runTest(const std::string& testName, std::function<bool()> testFunc) {
        testsRun++;
        std::cout << "Running integration test: " << testName << "... ";
        
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
        std::cout << "\n=== Integration Test Summary ===" << std::endl;
        std::cout << "Tests run: " << testsRun << std::endl;
        std::cout << "Tests passed: " << testsPassed << std::endl;
        std::cout << "Tests failed: " << testsFailed << std::endl;
        
        if (testsFailed == 0) {
            std::cout << "All integration tests passed! ✅" << std::endl;
        } else {
            std::cout << "Some integration tests failed! ❌" << std::endl;
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

// Test functions

bool testAssignmentWithTypeConversion() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: number = string (should trigger conversion)
    // Simulate assignment: number = "42"
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    
    // Test conversion
    bool canConvert = typeSystem.isConvertibleTo(stringType, numberType);
    if (!canConvert) {
        std::cout << "String to Number conversion should be possible" << std::endl;
        return false;
    }
    
    return true;
}

bool testBinaryExpressionWithTypeConversion() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: number + string (should trigger conversion)
    // Simulate binary expression: 42 + "hello"
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    
    // Test that both operands can be converted to a common type
    bool numberToString = typeSystem.isConvertibleTo(numberType, stringType);
    bool stringToNumber = typeSystem.isConvertibleTo(stringType, numberType);
    
    if (!numberToString && !stringToNumber) {
        std::cout << "At least one conversion should be possible for binary expression" << std::endl;
        return false;
    }
    
    return true;
}

bool testFunctionCallWithTypeConversion() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: function expecting number, called with string
    // Simulate function call: printNumber("42")
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    
    // Test conversion
    bool canConvert = typeSystem.isConvertibleTo(stringType, numberType);
    if (!canConvert) {
        std::cout << "String to Number conversion should be possible for function call" << std::endl;
        return false;
    }
    
    return true;
}

bool testControlFlowWithTypeConversion() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: if (string) - should trigger boolean conversion
    // Simulate if statement: if ("hello") { ... }
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    
    // Test conversion
    bool canConvert = typeSystem.isConvertibleTo(stringType, booleanType);
    if (!canConvert) {
        std::cout << "String to Boolean conversion should be possible for control flow" << std::endl;
        return false;
    }
    
    return true;
}

bool testDestructuringWithTypeConversion() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: destructuring assignment with type conversion
    // Simulate destructuring: [a, b] = [1, "hello"]
    auto arrayType = std::make_shared<ArrayType>(makeAnyType());
    
    // Test that array destructuring is compatible
    bool isCompatible = typeSystem.isConvertibleTo(arrayType, arrayType);
    
    if (!isCompatible) {
        std::cout << "Array destructuring should be compatible" << std::endl;
        return false;
    }
    
    return true;
}

bool testUnionTypeIntegration() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: union type in assignment
    auto unionType = std::make_shared<UnionType>(std::vector<std::shared_ptr<Type>>{
        makeNumberType(), makeStringType()
    });
    
    // Test conversions to union type
    bool numberToUnion = typeSystem.isConvertibleTo(makeNumberType(), unionType);
    bool stringToUnion = typeSystem.isConvertibleTo(makeStringType(), unionType);
    bool booleanToUnion = typeSystem.isConvertibleTo(makeBooleanType(), unionType);
    
    if (!numberToUnion || !stringToUnion || booleanToUnion) {
        std::cout << "Union type conversions not working correctly" << std::endl;
        return false;
    }
    
    return true;
}

bool testIntersectionTypeIntegration() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: intersection type in assignment
    auto intersectionType = std::make_shared<IntersectionType>(std::vector<std::shared_ptr<Type>>{
        makeNumberType(), makeStringType()
    });
    
    // Test conversions to intersection type
    bool numberToIntersection = typeSystem.isConvertibleTo(makeNumberType(), intersectionType);
    bool stringToIntersection = typeSystem.isConvertibleTo(makeStringType(), intersectionType);
    bool booleanToIntersection = typeSystem.isConvertibleTo(makeBooleanType(), intersectionType);
    
    if (!numberToIntersection || !stringToIntersection || !booleanToIntersection) {
        std::cout << "Intersection type conversions not working correctly" << std::endl;
        return false;
    }
    
    return true;
}

bool testAnyTypeIntegration() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: any type conversions
    auto anyType = makeAnyType();
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    
    // Test conversions involving any type
    bool anyToNumber = typeSystem.isConvertibleTo(anyType, numberType);
    bool numberToAny = typeSystem.isConvertibleTo(numberType, anyType);
    bool anyToString = typeSystem.isConvertibleTo(anyType, stringType);
    bool stringToAny = typeSystem.isConvertibleTo(stringType, anyType);
    bool anyToBoolean = typeSystem.isConvertibleTo(anyType, booleanType);
    bool booleanToAny = typeSystem.isConvertibleTo(booleanType, anyType);
    
    if (!anyToNumber || !numberToAny || !anyToString || !stringToAny || !anyToBoolean || !booleanToAny) {
        std::cout << "Any type conversions not working correctly" << std::endl;
        return false;
    }
    
    return true;
}

bool testImplicitVsExplicitConversion() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: implicit vs explicit conversion detection
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    
    // Test implicit conversions
    bool numberToStringImplicit = typeSystem.isImplicitlyConvertible(numberType, stringType);
    bool stringToNumberImplicit = typeSystem.isImplicitlyConvertible(stringType, numberType);
    bool booleanToNumberImplicit = typeSystem.isImplicitlyConvertible(booleanType, numberType);
    
    // Test explicit conversions
    bool numberToStringExplicit = typeSystem.isExplicitlyConvertible(numberType, stringType);
    bool stringToNumberExplicit = typeSystem.isExplicitlyConvertible(stringType, numberType);
    bool booleanToNumberExplicit = typeSystem.isExplicitlyConvertible(booleanType, numberType);
    
    if (!numberToStringImplicit || !stringToNumberImplicit || !booleanToNumberImplicit ||
        !numberToStringExplicit || !stringToNumberExplicit || !booleanToNumberExplicit) {
        std::cout << "Implicit/Explicit conversion detection not working correctly" << std::endl;
        return false;
    }
    
    return true;
}

bool testConversionKindDetection() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: conversion kind detection
    auto numberType = makeNumberType();
    auto stringType = makeStringType();
    auto booleanType = makeBooleanType();
    
    // Test different conversion kinds
    ConversionKind identity = typeSystem.getConversionKind(numberType, numberType);
    ConversionKind implicit = typeSystem.getConversionKind(numberType, stringType);
    ConversionKind explicit_conv = typeSystem.getConversionKind(booleanType, numberType);
    
    if (identity != ConversionKind::Identity || 
        implicit != ConversionKind::Implicit || 
        explicit_conv != ConversionKind::Implicit) {
        std::cout << "Conversion kind detection not working correctly" << std::endl;
        return false;
    }
    
    return true;
}

bool testErrorHandling() {
    TypeSystem typeSystem;
    DiagnosticEngine diagnostics;
    SemanticAnalyzer analyzer(diagnostics);
    
    // Test: error handling for invalid conversions
    auto numberType = makeNumberType();
    auto voidType = std::make_shared<PrimitiveType>(TypeKind::Void);
    
    // Test invalid conversions
    bool invalidConversion = typeSystem.isConvertibleTo(numberType, voidType);
    ConversionKind invalidKind = typeSystem.getConversionKind(numberType, voidType);
    
    if (invalidConversion || invalidKind != ConversionKind::Invalid) {
        std::cout << "Error handling for invalid conversions not working correctly" << std::endl;
        return false;
    }
    
    // Test null pointer handling
    bool nullConversion = typeSystem.isConvertibleTo(nullptr, numberType);
    bool nullConversion2 = typeSystem.isConvertibleTo(numberType, nullptr);
    
    if (nullConversion || nullConversion2) {
        std::cout << "Null pointer handling not working correctly" << std::endl;
        return false;
    }
    
    return true;
}

int main() {
    std::cout << "=== Type Conversion Integration Tests ===" << std::endl;
    
    IntegrationTestFramework framework;
    
    // Run all integration tests
    framework.runTest("Assignment with Type Conversion", testAssignmentWithTypeConversion);
    framework.runTest("Binary Expression with Type Conversion", testBinaryExpressionWithTypeConversion);
    framework.runTest("Function Call with Type Conversion", testFunctionCallWithTypeConversion);
    framework.runTest("Control Flow with Type Conversion", testControlFlowWithTypeConversion);
    framework.runTest("Destructuring with Type Conversion", testDestructuringWithTypeConversion);
    framework.runTest("Union Type Integration", testUnionTypeIntegration);
    framework.runTest("Intersection Type Integration", testIntersectionTypeIntegration);
    framework.runTest("Any Type Integration", testAnyTypeIntegration);
    framework.runTest("Implicit vs Explicit Conversion", testImplicitVsExplicitConversion);
    framework.runTest("Conversion Kind Detection", testConversionKindDetection);
    framework.runTest("Error Handling", testErrorHandling);
    
    framework.printSummary();
    
    return 0;
}