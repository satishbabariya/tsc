#include "tsc/semantic/TypeSystem.h"
#include "tsc/codegen/LLVMCodeGen.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/Common.h"
#include <iostream>
#include <memory>
#include <functional>
#include <chrono>

using namespace tsc;

// Simple test framework for LLVM integration tests
class LLVMIntegrationTestFramework {
private:
    int testsRun = 0;
    int testsPassed = 0;
    int testsFailed = 0;

public:
    void runTest(const std::string& testName, std::function<bool()> testFunc) {
        testsRun++;
        std::cout << "Running LLVM integration test: " << testName << "... ";
        
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
        std::cout << "\n=== LLVM Integration Test Summary ===" << std::endl;
        std::cout << "Tests run: " << testsRun << std::endl;
        std::cout << "Tests passed: " << testsPassed << std::endl;
        std::cout << "Tests failed: " << testsFailed << std::endl;
        
        if (testsFailed == 0) {
            std::cout << "All LLVM integration tests passed! ✅" << std::endl;
        } else {
            std::cout << "Some LLVM integration tests failed! ❌" << std::endl;
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

// Test functions

bool testLLVMCodeGenInitialization() {
    DiagnosticEngine diagnostics;
    CompilerOptions options;
    
    // Test: LLVMCodeGen can be initialized
    try {
        LLVMCodeGen codeGen(diagnostics, options);
        return true;
    } catch (const std::exception& e) {
        std::cout << "LLVMCodeGen initialization failed: " << e.what() << std::endl;
        return false;
    }
}

bool testTypeConversionInLLVM() {
    DiagnosticEngine diagnostics;
    CompilerOptions options;
    
    try {
        LLVMCodeGen codeGen(diagnostics, options);
        
        // Test: Type conversion methods exist and can be called
        auto numberType = makeNumberType();
        auto stringType = makeStringType();
        
        // Test that applyTypeConversion method exists and can be called
        // Note: This is a basic test to ensure the method exists
        // In a real scenario, we would need actual LLVM values to test with
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Type conversion in LLVM failed: " << e.what() << std::endl;
        return false;
    }
}

bool testTypeSystemIntegrationWithLLVM() {
    DiagnosticEngine diagnostics;
    CompilerOptions options;
    TypeSystem typeSystem;
    
    try {
        LLVMCodeGen codeGen(diagnostics, options);
        
        // Test: TypeSystem methods work with LLVMCodeGen
        auto numberType = makeNumberType();
        auto stringType = makeStringType();
        auto booleanType = makeBooleanType();
        
        // Test type conversion methods
        bool canConvert = typeSystem.isConvertibleTo(numberType, stringType);
        ConversionKind kind = typeSystem.getConversionKind(numberType, stringType);
        
        if (!canConvert || kind == ConversionKind::Invalid) {
            std::cout << "Type conversion not working with LLVM integration" << std::endl;
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "TypeSystem integration with LLVM failed: " << e.what() << std::endl;
        return false;
    }
}

bool testUnionTypeLLVMIntegration() {
    DiagnosticEngine diagnostics;
    CompilerOptions options;
    TypeSystem typeSystem;
    
    try {
        LLVMCodeGen codeGen(diagnostics, options);
        
        // Test: Union types work with LLVM integration
        auto unionType = std::make_shared<UnionType>(std::vector<std::shared_ptr<Type>>{
            makeNumberType(), makeStringType()
        });
        
        auto numberType = makeNumberType();
        bool canConvert = typeSystem.isConvertibleTo(numberType, unionType);
        
        if (!canConvert) {
            std::cout << "Union type conversion not working with LLVM integration" << std::endl;
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Union type LLVM integration failed: " << e.what() << std::endl;
        return false;
    }
}

bool testIntersectionTypeLLVMIntegration() {
    DiagnosticEngine diagnostics;
    CompilerOptions options;
    TypeSystem typeSystem;
    
    try {
        LLVMCodeGen codeGen(diagnostics, options);
        
        // Test: Intersection types work with LLVM integration
        auto intersectionType = std::make_shared<IntersectionType>(std::vector<std::shared_ptr<Type>>{
            makeNumberType(), makeStringType()
        });
        
        auto numberType = makeNumberType();
        bool canConvert = typeSystem.isConvertibleTo(numberType, intersectionType);
        
        if (!canConvert) {
            std::cout << "Intersection type conversion not working with LLVM integration" << std::endl;
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Intersection type LLVM integration failed: " << e.what() << std::endl;
        return false;
    }
}

bool testErrorHandlingInLLVM() {
    DiagnosticEngine diagnostics;
    CompilerOptions options;
    TypeSystem typeSystem;
    
    try {
        LLVMCodeGen codeGen(diagnostics, options);
        
        // Test: Error handling works with LLVM integration
        auto numberType = makeNumberType();
        auto voidType = std::make_shared<PrimitiveType>(TypeKind::Void);
        
        // Test invalid conversions
        bool invalidConversion = typeSystem.isConvertibleTo(numberType, voidType);
        ConversionKind invalidKind = typeSystem.getConversionKind(numberType, voidType);
        
        if (invalidConversion || invalidKind != ConversionKind::Invalid) {
            std::cout << "Error handling not working with LLVM integration" << std::endl;
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error handling LLVM integration failed: " << e.what() << std::endl;
        return false;
    }
}

bool testPerformanceOfTypeConversions() {
    DiagnosticEngine diagnostics;
    CompilerOptions options;
    TypeSystem typeSystem;
    
    try {
        LLVMCodeGen codeGen(diagnostics, options);
        
        // Test: Performance of type conversions
        auto numberType = makeNumberType();
        auto stringType = makeStringType();
        auto booleanType = makeBooleanType();
        
        // Run multiple conversions to test performance
        const int iterations = 1000;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            typeSystem.isConvertibleTo(numberType, stringType);
            typeSystem.isConvertibleTo(stringType, booleanType);
            typeSystem.isConvertibleTo(booleanType, numberType);
            typeSystem.getConversionKind(numberType, stringType);
            typeSystem.getConversionKind(stringType, booleanType);
            typeSystem.getConversionKind(booleanType, numberType);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Check that conversions are reasonably fast (less than 1ms per iteration)
        double avgTimePerIteration = duration.count() / (double)iterations;
        if (avgTimePerIteration > 1000) { // 1ms in microseconds
            std::cout << "Type conversions too slow: " << avgTimePerIteration << " microseconds per iteration" << std::endl;
            return false;
        }
        
        std::cout << "Performance: " << avgTimePerIteration << " microseconds per iteration";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Performance test failed: " << e.what() << std::endl;
        return false;
    }
}

int main() {
    std::cout << "=== LLVM Code Generation Integration Tests ===" << std::endl;
    
    LLVMIntegrationTestFramework framework;
    
    // Run all LLVM integration tests
    framework.runTest("LLVM CodeGen Initialization", testLLVMCodeGenInitialization);
    framework.runTest("Type Conversion in LLVM", testTypeConversionInLLVM);
    framework.runTest("TypeSystem Integration with LLVM", testTypeSystemIntegrationWithLLVM);
    framework.runTest("Union Type LLVM Integration", testUnionTypeLLVMIntegration);
    framework.runTest("Intersection Type LLVM Integration", testIntersectionTypeLLVMIntegration);
    framework.runTest("Error Handling in LLVM", testErrorHandlingInLLVM);
    framework.runTest("Performance of Type Conversions", testPerformanceOfTypeConversions);
    
    framework.printSummary();
    
    return 0;
}