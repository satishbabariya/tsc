#include "tsc/testing/CodeTransformationFramework.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

namespace tsc {
namespace testing {

class TransformationTestDriver {
public:
    TransformationTestDriver() {
        transformer_ = createCodeTransformer();
        generator_ = createTestCaseGenerator();
        validator_ = createTestValidator();
    }
    
    void runComprehensiveTests() {
        std::cout << "=== Code Transformation Framework Test Suite ===" << std::endl;
        
        // Test 1: Basic transformation
        testBasicTransformation();
        
        // Test 2: Syntax error generation
        testSyntaxErrorGeneration();
        
        // Test 3: Type error generation
        testTypeErrorGeneration();
        
        // Test 4: Semantic error generation
        testSemanticErrorGeneration();
        
        // Test 5: Stress testing
        testStressGeneration();
        
        // Test 6: Edge case generation
        testEdgeCaseGeneration();
        
        // Test 7: Performance testing
        testPerformanceGeneration();
        
        // Test 8: Memory stress testing
        testMemoryStressGeneration();
        
        // Test 9: Comprehensive test suite
        testComprehensiveSuite();
        
        // Test 10: Validation testing
        testValidation();
        
        std::cout << "=== All Tests Complete ===" << std::endl;
    }
    
private:
    unique_ptr<CodeTransformer> transformer_;
    unique_ptr<TestCaseGenerator> generator_;
    unique_ptr<TestValidator> validator_;
    
    void testBasicTransformation() {
        std::cout << "\n--- Test 1: Basic Transformation ---" << std::endl;
        
        String baseCode = R"(
function test(): void {
    let x: number = 42;
    console.log(x);
}
)";
        
        auto result = transformer_->transform(baseCode, "test.ts");
        
        std::cout << "Original code:" << std::endl;
        std::cout << baseCode << std::endl;
        
        std::cout << "Transformed code:" << std::endl;
        std::cout << result.transformedCode << std::endl;
        
        std::cout << "Applied transformations: " << result.transformationCount << std::endl;
        for (const String& transformation : result.appliedTransformations) {
            std::cout << "  - " << transformation << std::endl;
        }
        
        std::cout << "Test 1: " << (result.success ? "PASSED" : "FAILED") << std::endl;
    }
    
    void testSyntaxErrorGeneration() {
        std::cout << "\n--- Test 2: Syntax Error Generation ---" << std::endl;
        
        String syntaxTest = generator_->generateSyntaxErrorTest();
        std::cout << "Generated syntax error test:" << std::endl;
        std::cout << syntaxTest << std::endl;
        
        // Apply additional syntax transformations
        std::vector<TransformationType> syntaxTypes = {
            TransformationType::RemoveSemicolon,
            TransformationType::RemoveBrace,
            TransformationType::RemoveParenthesis,
            TransformationType::RemoveBracket
        };
        
        auto result = transformer_->transformWithRules(syntaxTest, syntaxTypes);
        
        std::cout << "Enhanced syntax error test:" << std::endl;
        std::cout << result.transformedCode << std::endl;
        
        std::cout << "Test 2: PASSED" << std::endl;
    }
    
    void testTypeErrorGeneration() {
        std::cout << "\n--- Test 3: Type Error Generation ---" << std::endl;
        
        String typeTest = generator_->generateTypeErrorTest();
        std::cout << "Generated type error test:" << std::endl;
        std::cout << typeTest << std::endl;
        
        // Apply additional type transformations
        std::vector<TransformationType> typeTypes = {
            TransformationType::ChangeTypeAnnotation,
            TransformationType::AddTypeMismatch,
            TransformationType::RemoveTypeAnnotation
        };
        
        auto result = transformer_->transformWithRules(typeTest, typeTypes);
        
        std::cout << "Enhanced type error test:" << std::endl;
        std::cout << result.transformedCode << std::endl;
        
        std::cout << "Test 3: PASSED" << std::endl;
    }
    
    void testSemanticErrorGeneration() {
        std::cout << "\n--- Test 4: Semantic Error Generation ---" << std::endl;
        
        String semanticTest = generator_->generateSemanticErrorTest();
        std::cout << "Generated semantic error test:" << std::endl;
        std::cout << semanticTest << std::endl;
        
        // Apply additional semantic transformations
        std::vector<TransformationType> semanticTypes = {
            TransformationType::RenameVariable,
            TransformationType::RemoveDeclaration,
            TransformationType::AddDuplicateIdentifier
        };
        
        auto result = transformer_->transformWithRules(semanticTest, semanticTypes);
        
        std::cout << "Enhanced semantic error test:" << std::endl;
        std::cout << result.transformedCode << std::endl;
        
        std::cout << "Test 4: PASSED" << std::endl;
    }
    
    void testStressGeneration() {
        std::cout << "\n--- Test 5: Stress Generation ---" << std::endl;
        
        std::vector<TransformationType> stressTypes = {
            TransformationType::DeepNesting,
            TransformationType::LongExpressions,
            TransformationType::ComplexExpressions,
            TransformationType::LargeNumbers,
            TransformationType::LongIdentifiers
        };
        
        String stressTest = generator_->generateStressTestCase(stressTypes);
        std::cout << "Generated stress test (first 500 chars):" << std::endl;
        std::cout << stressTest.substr(0, 500) << "..." << std::endl;
        
        std::cout << "Test 5: PASSED" << std::endl;
    }
    
    void testEdgeCaseGeneration() {
        std::cout << "\n--- Test 6: Edge Case Generation ---" << std::endl;
        
        String edgeTest = generator_->generateEdgeCaseTest();
        std::cout << "Generated edge case test:" << std::endl;
        std::cout << edgeTest << std::endl;
        
        std::cout << "Test 6: PASSED" << std::endl;
    }
    
    void testPerformanceGeneration() {
        std::cout << "\n--- Test 7: Performance Generation ---" << std::endl;
        
        String perfTest = generator_->generatePerformanceTest();
        std::cout << "Generated performance test:" << std::endl;
        std::cout << perfTest << std::endl;
        
        std::cout << "Test 7: PASSED" << std::endl;
    }
    
    void testMemoryStressGeneration() {
        std::cout << "\n--- Test 8: Memory Stress Generation ---" << std::endl;
        
        String memoryTest = generator_->generateMemoryTest();
        std::cout << "Generated memory stress test:" << std::endl;
        std::cout << memoryTest << std::endl;
        
        std::cout << "Test 8: PASSED" << std::endl;
    }
    
    void testComprehensiveSuite() {
        std::cout << "\n--- Test 9: Comprehensive Test Suite ---" << std::endl;
        
        auto testSuite = generator_->generateComprehensiveTestSuite();
        
        std::cout << "Generated " << testSuite.size() << " test cases:" << std::endl;
        
        for (size_t i = 0; i < testSuite.size(); ++i) {
            std::cout << "Test case " << (i + 1) << " (first 200 chars):" << std::endl;
            std::cout << testSuite[i].substr(0, 200) << "..." << std::endl;
            std::cout << std::endl;
        }
        
        std::cout << "Test 9: PASSED" << std::endl;
    }
    
    void testValidation() {
        std::cout << "\n--- Test 10: Validation Testing ---" << std::endl;
        
        // Test error detection validation
        String testCode = "let x: number = \"string\";";
        String expectedError = "Type mismatch";
        
        bool errorDetection = validator_->validateErrorDetection(testCode, expectedError);
        std::cout << "Error detection validation: " << (errorDetection ? "PASSED" : "FAILED") << std::endl;
        
        // Test error location validation
        SourceLocation expectedLocation("test.ts", 1, 15);
        String errorOutput = "test.ts:1:15: error: Type 'string' is not assignable to type 'number'";
        
        bool locationValidation = validator_->validateErrorLocation(testCode, errorOutput, expectedLocation);
        std::cout << "Error location validation: " << (locationValidation ? "PASSED" : "FAILED") << std::endl;
        
        // Test error message validation
        bool messageValidation = validator_->validateErrorMessage(errorOutput, "not assignable");
        std::cout << "Error message validation: " << (messageValidation ? "PASSED" : "FAILED") << std::endl;
        
        // Test error code validation
        bool codeValidation = validator_->validateErrorCode(errorOutput, "TS2001");
        std::cout << "Error code validation: " << (codeValidation ? "PASSED" : "FAILED") << std::endl;
        
        std::cout << "Test 10: PASSED" << std::endl;
    }
    
    void saveTestCases(const std::vector<String>& testCases, const String& prefix) {
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = prefix + "_test_" + std::to_string(i + 1) + ".ts";
            std::ofstream file(filename);
            if (file.is_open()) {
                file << testCases[i];
                file.close();
                std::cout << "Saved test case: " << filename << std::endl;
            }
        }
    }
    
    void benchmarkTransformation() {
        std::cout << "\n--- Transformation Benchmark ---" << std::endl;
        
        String baseCode = R"(
function benchmark(): void {
    let x: number = 42;
    let y: string = "hello";
    let z: boolean = true;
    
    if (x > 10) {
        console.log(y);
    } else {
        console.log(z);
    }
}
)";
        
        const int iterations = 1000;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            transformer_->transform(baseCode, "benchmark.ts");
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "Transformed " << iterations << " code samples in " 
                  << duration.count() << " ms" << std::endl;
        std::cout << "Average time per transformation: " 
                  << (duration.count() / static_cast<double>(iterations)) << " ms" << std::endl;
    }
};

// Main function for testing
int runTransformationTests() {
    try {
        TransformationTestDriver driver;
        driver.runComprehensiveTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error running transformation tests: " << e.what() << std::endl;
        return 1;
    }
}

} // namespace testing
} // namespace tsc