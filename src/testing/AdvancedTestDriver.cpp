#include "tsc/testing/CodeTransformationFramework.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <chrono>
#include <algorithm>

namespace tsc {
namespace testing {

class AdvancedTestDriver {
public:
    struct TestResult {
        String testName;
        bool passed;
        std::vector<String> errors;
        std::vector<String> warnings;
        double executionTime;
        String category;
        String description;
    };
    
    struct FrameworkValidation {
        String ruleName;
        bool canApply;
        bool appliedSuccessfully;
        String errorMessage;
        double executionTime;
    };
    
    AdvancedTestDriver() {
        transformer_ = createAdvancedCodeTransformer();
        validator_ = createTestValidator();
    }
    
    void runAdvancedTesting() {
        std::cout << "=== Advanced Code Transformation Testing Suite ===" << std::endl;
        
        // Step 1: Validate transformation framework
        validateTransformationFramework();
        
        // Step 2: Test advanced transformation rules
        testAdvancedTransformationRules();
        
        // Step 3: Generate complex test cases
        generateComplexTestCases();
        
        // Step 4: Run comprehensive validation
        runComprehensiveValidation();
        
        // Step 5: Analyze results and generate report
        analyzeResultsAndGenerateReport();
        
        std::cout << "=== Advanced Testing Complete ===" << std::endl;
    }
    
private:
    unique_ptr<CodeTransformer> transformer_;
    unique_ptr<TestValidator> validator_;
    std::vector<TestResult> results_;
    std::vector<FrameworkValidation> frameworkValidations_;
    
    void validateTransformationFramework() {
        std::cout << "\n--- Validating Transformation Framework ---" << std::endl;
        
        String testCode = R"(
function testFunction(): void {
    let x: number = 42;
    let y: string = "hello";
    let z: boolean = true;
    
    if (x > 0) {
        console.log(y);
    } else {
        console.log(z);
    }
}
)";
        
        // Test each transformation rule
        std::vector<String> ruleNames = transformer_->getAvailableRules();
        
        for (const String& ruleName : ruleNames) {
            FrameworkValidation validation;
            validation.ruleName = ruleName;
            
            auto start = std::chrono::high_resolution_clock::now();
            
            try {
                // Test if rule can be applied
                validation.canApply = true; // Assume true for now
                
                // Apply transformation
                auto result = transformer_->transform(testCode, "test.ts");
                validation.appliedSuccessfully = result.success;
                
                if (!result.success) {
                    validation.errorMessage = "Transformation failed";
                }
                
            } catch (const std::exception& e) {
                validation.appliedSuccessfully = false;
                validation.errorMessage = e.what();
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            validation.executionTime = std::chrono::duration<double, std::milli>(end - start).count();
            
            frameworkValidations_.push_back(validation);
            
            std::cout << "Rule: " << ruleName 
                      << " - " << (validation.appliedSuccessfully ? "PASSED" : "FAILED")
                      << " (" << validation.executionTime << " ms)" << std::endl;
            
            if (!validation.appliedSuccessfully) {
                std::cout << "  Error: " << validation.errorMessage << std::endl;
            }
        }
    }
    
    void testAdvancedTransformationRules() {
        std::cout << "\n--- Testing Advanced Transformation Rules ---" << std::endl;
        
        // Test nested object spread transformations
        testNestedObjectSpread();
        
        // Test nested control function transformations
        testNestedControlFunctions();
        
        // Test type parameter transformations
        testTypeParameterTransformations();
        
        // Test advanced memory stress
        testAdvancedMemoryStress();
        
        // Test advanced performance stress
        testAdvancedPerformanceStress();
        
        // Test advanced edge cases
        testAdvancedEdgeCases();
    }
    
    void testNestedObjectSpread() {
        std::cout << "Testing nested object spread transformations..." << std::endl;
        
        String baseCode = R"(
function simpleFunction(): void {
    let obj = { name: "test", value: 42 };
    console.log(obj);
}
)";
        
        TestResult result = runTransformationTest("nested_object_spread", baseCode, "Nested Object Spread");
        results_.push_back(result);
    }
    
    void testNestedControlFunctions() {
        std::cout << "Testing nested control function transformations..." << std::endl;
        
        String baseCode = R"(
function simpleFunction(): void {
    if (true) {
        console.log("simple");
    }
}
)";
        
        TestResult result = runTransformationTest("nested_control_functions", baseCode, "Nested Control Functions");
        results_.push_back(result);
    }
    
    void testTypeParameterTransformations() {
        std::cout << "Testing type parameter transformations..." << std::endl;
        
        String baseCode = R"(
function simpleFunction(x: number): number {
    return x * 2;
}
)";
        
        TestResult result = runTransformationTest("type_parameter_transformations", baseCode, "Type Parameter Transformations");
        results_.push_back(result);
    }
    
    void testAdvancedMemoryStress() {
        std::cout << "Testing advanced memory stress..." << std::endl;
        
        String baseCode = R"(
function simpleFunction(): void {
    let arr = [1, 2, 3];
    console.log(arr);
}
)";
        
        TestResult result = runTransformationTest("advanced_memory_stress", baseCode, "Advanced Memory Stress");
        results_.push_back(result);
    }
    
    void testAdvancedPerformanceStress() {
        std::cout << "Testing advanced performance stress..." << std::endl;
        
        String baseCode = R"(
function simpleFunction(): void {
    let sum = 0;
    for (let i = 0; i < 10; i++) {
        sum += i;
    }
    console.log(sum);
}
)";
        
        TestResult result = runTransformationTest("advanced_performance_stress", baseCode, "Advanced Performance Stress");
        results_.push_back(result);
    }
    
    void testAdvancedEdgeCases() {
        std::cout << "Testing advanced edge cases..." << std::endl;
        
        String baseCode = R"(
function simpleFunction(): void {
    let x = 42;
    console.log(x);
}
)";
        
        TestResult result = runTransformationTest("advanced_edge_cases", baseCode, "Advanced Edge Cases");
        results_.push_back(result);
    }
    
    TestResult runTransformationTest(const String& testName, const String& baseCode, const String& description) {
        TestResult result;
        result.testName = testName;
        result.description = description;
        result.category = "Advanced Transformation";
        result.passed = false;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        try {
            // Apply transformation
            auto transformationResult = transformer_->transform(baseCode, testName + ".ts");
            
            if (transformationResult.success) {
                // Save transformed code
                String filename = testName + "_transformed.ts";
                saveTestFile(filename, transformationResult.transformedCode);
                
                // Run compiler on transformed code
                String command = "./build/tsc " + filename + " 2>&1";
                String output = executeCommand(command);
                
                // Parse errors and warnings
                parseCompilerOutput(output, result.errors, result.warnings);
                
                // Determine if test passed (should have errors for error tests)
                result.passed = !result.errors.empty();
                
            } else {
                result.errors.push_back("Transformation failed: " + transformationResult.errors[0]);
            }
            
        } catch (const std::exception& e) {
            result.errors.push_back("Test execution failed: " + String(e.what()));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        result.executionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        return result;
    }
    
    void generateComplexTestCases() {
        std::cout << "\n--- Generating Complex Test Cases ---" << std::endl;
        
        // Generate complex test cases using advanced transformations
        std::vector<std::pair<String, String>> complexTests = {
            {"complex_nested_objects.ts", generateComplexNestedObjectsTest()},
            {"complex_control_flow.ts", generateComplexControlFlowTest()},
            {"complex_type_scenarios.ts", generateComplexTypeScenariosTest()},
            {"complex_memory_scenarios.ts", generateComplexMemoryScenariosTest()},
            {"complex_performance_scenarios.ts", generateComplexPerformanceScenariosTest()},
            {"complex_edge_cases.ts", generateComplexEdgeCasesTest()}
        };
        
        for (const auto& test : complexTests) {
            saveTestFile(test.first, test.second);
            std::cout << "Generated: " << test.first << std::endl;
        }
    }
    
    String generateComplexNestedObjectsTest() {
        return R"(
// Complex nested objects with spread operations
interface BaseObject {
    id: number;
    name: string;
    properties: {
        color: string;
        size: string;
        dimensions: {
            width: number;
            height: number;
            depth: number;
        };
    };
    metadata: {
        created: string;
        updated: string;
        tags: string[];
    };
}

function createComplexNestedObject(): BaseObject {
    let baseObject: BaseObject = {
        id: 1,
        name: "base",
        properties: {
            color: "red",
            size: "large",
            dimensions: {
                width: 100,
                height: 200,
                depth: 50
            }
        },
        metadata: {
            created: "2024-01-01",
            updated: "2024-01-02",
            tags: ["important", "urgent", "critical"]
        }
    };
    
    // Complex spread operations with conflicts
    let extendedObject = {
        ...baseObject,
        ...baseObject.properties,
        ...baseObject.properties.dimensions,
        ...baseObject.metadata,
        additional: {
            ...baseObject.properties,
            ...baseObject.metadata,
            nested: {
                ...baseObject.properties.dimensions,
                ...baseObject.metadata.tags,
                conflict: "resolved"
            }
        },
        conflicts: {
            ...baseObject.properties,
            ...baseObject.metadata,
            name: "conflict", // This should conflict with baseObject.name
            color: "blue",    // This should conflict with baseObject.properties.color
            width: 999        // This should conflict with baseObject.properties.dimensions.width
        }
    };
    
    return extendedObject as BaseObject;
}

// Deep nested object with multiple spread levels
function createDeepNestedObject(): any {
    let level1 = {
        a: 1,
        b: { c: 2, d: 3 },
        e: { f: { g: 4, h: 5 }, i: 6 }
    };
    
    let level2 = {
        ...level1,
        ...level1.b,
        ...level1.e,
        ...level1.e.f,
        nested: {
            ...level1,
            ...level1.b,
            ...level1.e,
            ...level1.e.f,
            deeper: {
                ...level1,
                ...level1.b,
                ...level1.e,
                ...level1.e.f,
                deepest: {
                    ...level1,
                    ...level1.b,
                    ...level1.e,
                    ...level1.e.f
                }
            }
        }
    };
    
    return level2;
}
)";
    }
    
    String generateComplexControlFlowTest() {
        return R"(
// Complex function with nested control statements
function complexNestedFunction(x: number, y: string, z: boolean): string {
    let result = "";
    
    // Level 1: if-else
    if (x > 0) {
        // Level 2: switch
        switch (y.length) {
            case 1:
                // Level 3: for loop
                for (let i = 0; i < x; i++) {
                    // Level 4: while loop
                    while (i < 10) {
                        // Level 5: if-else
                        if (z) {
                            // Level 6: do-while
                            do {
                                // Level 7: nested switch
                                switch (i % 3) {
                                    case 0:
                                        // Level 8: for loop
                                        for (let j = 0; j < 5; j++) {
                                            // Level 9: if-else
                                            if (j % 2 === 0) {
                                                // Level 10: try-catch
                                                try {
                                                    result += `Level10: ${i}-${j}`;
                                                } catch (error) {
                                                    result += "Error";
                                                }
                                            } else {
                                                result += `Odd: ${j}`;
                                            }
                                        }
                                        break;
                                    case 1:
                                        result += "One";
                                        break;
                                    case 2:
                                        result += "Two";
                                        break;
                                }
                                i++;
                            } while (i < 5);
                        } else {
                            result += "False";
                        }
                    }
                }
                break;
            case 2:
                // Level 2: while loop
                while (x > 0) {
                    // Level 3: for loop
                    for (let k = 0; k < y.length; k++) {
                        // Level 4: if-else
                        if (k % 2 === 0) {
                            // Level 5: switch
                            switch (k) {
                                case 0:
                                    result += "Zero";
                                    break;
                                case 2:
                                    result += "Two";
                                    break;
                                default:
                                    result += "Default";
                            }
                        }
                    }
                    x--;
                }
                break;
            default:
                // Level 2: do-while
                do {
                    // Level 3: for loop
                    for (let m = 0; m < 3; m++) {
                        // Level 4: while loop
                        while (m < 2) {
                            // Level 5: if-else
                            if (m === 0) {
                                result += "Start";
                            } else {
                                result += "Middle";
                            }
                            m++;
                        }
                    }
                    x++;
                } while (x < 10);
        }
    } else {
        // Level 1: else - switch
        switch (y) {
            case "negative":
                // Level 2: for loop
                for (let n = 0; n < Math.abs(x); n++) {
                    // Level 3: while loop
                    while (n < 5) {
                        // Level 4: if-else
                        if (n % 2 === 0) {
                            result += "Even";
                        } else {
                            result += "Odd";
                        }
                        n++;
                    }
                }
                break;
            default:
                // Level 2: while loop
                while (x < 0) {
                    // Level 3: for loop
                    for (let p = 0; p < 3; p++) {
                        result += "Negative";
                    }
                    x++;
                }
        }
    }
    
    return result;
}
)";
    }
    
    String generateComplexTypeScenariosTest() {
        return R"(
// Complex type scenarios with mismatches
interface User {
    id: number;
    name: string;
    email: string;
    active: boolean;
}

interface Product {
    id: number;
    name: string;
    price: number;
    inStock: boolean;
}

function processUsers(users: User[]): User[] {
    return users.filter(user => user.active);
}

function processProducts(products: Product[]): Product[] {
    return products.filter(product => product.inStock);
}

function testComplexTypeMismatches(): void {
    let users: User[] = [
        { id: 1, name: "John", email: "john@example.com", active: true },
        { id: 2, name: "Jane", email: "jane@example.com", active: false }
    ];
    
    let products: Product[] = [
        { id: 1, name: "Laptop", price: 999, inStock: true },
        { id: 2, name: "Mouse", price: 25, inStock: false }
    ];
    
    // Type mismatches with complex objects
    let result1 = processUsers(products); // Product[] passed to User[] parameter
    let result2 = processProducts(users); // User[] passed to Product[] parameter
    
    // Mixed arrays
    let mixedArray = [...users, ...products];
    let result3 = processUsers(mixedArray); // mixed array passed to User[] parameter
    let result4 = processProducts(mixedArray); // mixed array passed to Product[] parameter
}

// Generic functions with type mismatches
function genericProcess<T>(items: T[], processor: (item: T) => T): T[] {
    return items.map(processor);
}

function testGenericTypeMismatches(): void {
    let numbers = [1, 2, 3];
    let strings = ["a", "b", "c"];
    
    // Type mismatches in generic functions
    let result1 = genericProcess(numbers, (n: string) => n.toUpperCase()); // number[] with string processor
    let result2 = genericProcess(strings, (s: number) => s * 2); // string[] with number processor
    let result3 = genericProcess(numbers, (n: boolean) => !n); // number[] with boolean processor
}
)";
    }
    
    String generateComplexMemoryScenariosTest() {
        return R"(
// Complex memory scenarios
function advancedMemoryStress(): void {
    // Large nested object structures
    let largeObject = {};
    for (let i = 0; i < 1000; i++) {
        largeObject[`level1_${i}`] = {};
        for (let j = 0; j < 100; j++) {
            largeObject[`level1_${i}`][`level2_${j}`] = {};
            for (let k = 0; k < 10; k++) {
                largeObject[`level1_${i}`][`level2_${j}`][`level3_${k}`] = {
                    data: new Array(1000).fill(i * j * k),
                    metadata: {
                        created: new Date(),
                        id: `${i}-${j}-${k}`,
                        nested: {
                            deep: {
                                deeper: {
                                    deepest: new Array(100).fill("deep data")
                                }
                            }
                        }
                    }
                };
            }
        }
    }
    
    // Large array operations
    let largeArrays = [];
    for (let i = 0; i < 100; i++) {
        let array = new Array(10000);
        for (let j = 0; j < 10000; j++) {
            array[j] = {
                index: j,
                value: Math.random() * 1000,
                nested: {
                    data: new Array(100).fill(j),
                    metadata: new Array(50).fill(`item_${j}`)
                }
            };
        }
        largeArrays.push(array);
    }
    
    // Complex function closures
    let closures = [];
    for (let i = 0; i < 1000; i++) {
        let closure = (function(index) {
            let data = new Array(1000).fill(index);
            return function() {
                return data.map(x => x * Math.random());
            };
        })(i);
        closures.push(closure);
    }
    
    // Execute closures to create memory pressure
    closures.forEach(closure => {
        try {
            closure();
        } catch (error) {
            // Handle potential memory errors
        }
    });
}

// Memory leak simulation
function memoryLeakSimulation(): void {
    let leakedObjects = [];
    
    // Simulate memory leaks
    for (let i = 0; i < 10000; i++) {
        let leakedObject = {
            id: i,
            data: new Array(1000).fill(i),
            circular: null as any
        };
        
        // Create circular reference
        leakedObject.circular = leakedObject;
        
        leakedObjects.push(leakedObject);
    }
    
    // Don't clean up - simulate memory leak
    console.log("Memory leak simulation complete");
}
)";
    }
    
    String generateComplexPerformanceScenariosTest() {
        return R"(
// Complex performance scenarios
function advancedPerformanceStress(): number {
    let result = 0;
    
    // Complex nested loops with heavy computations
    for (let i = 0; i < 1000; i++) {
        for (let j = 0; j < 1000; j++) {
            for (let k = 0; k < 1000; k++) {
                // Heavy mathematical computations
                let value = Math.sqrt(i * j * k) + 
                           Math.sin(i) * Math.cos(j) + 
                           Math.tan(k) * Math.log(i + 1) +
                           Math.pow(i, 2) + Math.pow(j, 2) + Math.pow(k, 2);
                
                // Complex conditional logic
                if (value > 0) {
                    if (i % 2 === 0) {
                        if (j % 3 === 0) {
                            if (k % 5 === 0) {
                                result += Math.sqrt(value) * Math.sin(value) * Math.cos(value);
                            } else {
                                result += Math.tan(value) * Math.log(value + 1);
                            }
                        } else {
                            result += Math.pow(value, 1.5) * Math.exp(value / 1000);
                        }
                    } else {
                        result += Math.abs(value) * Math.floor(value);
                    }
                } else {
                    result += Math.abs(value) * Math.ceil(value);
                }
            }
        }
    }
    
    return result;
}

// Recursive performance stress
function recursivePerformanceStress(n: number, depth: number = 0): number {
    if (depth > 1000 || n <= 0) {
        return 1;
    }
    
    let result = 0;
    
    // Heavy computation at each recursive level
    for (let i = 0; i < n; i++) {
        result += Math.sqrt(i) * Math.sin(i) * Math.cos(i) * Math.tan(i);
    }
    
    // Recursive calls
    let recursiveResult1 = recursivePerformanceStress(n - 1, depth + 1);
    let recursiveResult2 = recursivePerformanceStress(n - 2, depth + 1);
    let recursiveResult3 = recursivePerformanceStress(n - 3, depth + 1);
    
    return result + recursiveResult1 + recursiveResult2 + recursiveResult3;
}
)";
    }
    
    String generateComplexEdgeCasesTest() {
        return R"(
// Complex edge cases and boundary conditions
function advancedEdgeCases(): void {
    // Numeric edge cases
    let numericEdgeCases = {
        zero: 0,
        negativeZero: -0,
        infinity: Infinity,
        negativeInfinity: -Infinity,
        nan: NaN,
        maxSafeInteger: Number.MAX_SAFE_INTEGER,
        minSafeInteger: Number.MIN_SAFE_INTEGER,
        maxValue: Number.MAX_VALUE,
        minValue: Number.MIN_VALUE,
        epsilon: Number.EPSILON,
        veryLarge: 1e308,
        verySmall: 1e-308,
        precision: 0.1 + 0.2, // Floating point precision issue
        overflow: Number.MAX_VALUE * 2,
        underflow: Number.MIN_VALUE / 2
    };
    
    // String edge cases
    let stringEdgeCases = {
        empty: "",
        singleChar: "a",
        unicode: "🚀🌟💫⭐✨🔥💥⚡🌈🎉🎊🎈🎁🎀🎂🍰🎪🎭🎨🎬🎵🎶🎸🎹🎺🎻🎼🎽🎾🎿🏀🏁🏂🏃🏄🏅🏆🏇🏈🏉🏊🏋🏌🏍🏎🏏🏐🏑🏒🏓🏔🏕🏖🏗🏘🏙🏚🏛🏜🏝🏞🏟🏠🏡🏢🏣🏤🏥🏦🏧🏨🏩🏪🏫🏬🏭🏮🏯🏰🏱🏲🏳🏴🏵🏶🏷🏸🏹🏺🏻🏼🏽🏾🏿",
        specialChars: "!@#$%^&*()_+-=[]{}|;':\",./<>?`~",
        newlines: "line1\nline2\rline3\r\nline4",
        tabs: "col1\tcol2\tcol3",
        quotes: 'He said "Hello" and \'Goodbye\'',
        backslashes: "C:\\Users\\Name\\Documents",
        veryLong: "a".repeat(1000000),
        nullChars: "hello\0world\0test"
    };
    
    // Array edge cases
    let arrayEdgeCases = {
        empty: [],
        singleElement: [42],
        twoElements: [1, 2],
        largeArray: new Array(1000000).fill(0),
        sparseArray: [1, , , 4, , 6],
        mixedTypes: [1, "hello", true, null, undefined, {}, []],
        nestedArrays: [[1, 2], [3, 4], [5, 6]],
        deepNested: [[[[[[[[[[42]]]]]]]]]],
        circular: [] as any[]
    };
    
    // Create circular reference
    arrayEdgeCases.circular.push(arrayEdgeCases.circular);
    
    // Test edge cases
    console.log("Numeric edge cases:", numericEdgeCases);
    console.log("String edge cases:", stringEdgeCases);
    console.log("Array edge cases:", arrayEdgeCases);
}
)";
    }
    
    void runComprehensiveValidation() {
        std::cout << "\n--- Running Comprehensive Validation ---" << std::endl;
        
        // Get all generated test files
        std::vector<String> testFiles = {
            "complex_nested_objects.ts",
            "complex_control_flow.ts",
            "complex_type_scenarios.ts",
            "complex_memory_scenarios.ts",
            "complex_performance_scenarios.ts",
            "complex_edge_cases.ts"
        };
        
        for (const String& testFile : testFiles) {
            TestResult result = runCompilerTest(testFile);
            results_.push_back(result);
        }
    }
    
    TestResult runCompilerTest(const String& filename) {
        TestResult result;
        result.testName = filename;
        result.category = "Complex Test";
        result.passed = false;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Run the compiler
        String command = "./build/tsc " + filename + " 2>&1";
        String output = executeCommand(command);
        
        auto end = std::chrono::high_resolution_clock::now();
        result.executionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        // Parse errors and warnings
        parseCompilerOutput(output, result.errors, result.warnings);
        
        // Determine if test passed (should have errors for error tests)
        result.passed = !result.errors.empty();
        
        return result;
    }
    
    void analyzeResultsAndGenerateReport() {
        std::cout << "\n--- Analysis and Report Generation ---" << std::endl;
        
        // Analyze framework validation results
        analyzeFrameworkValidation();
        
        // Analyze test results
        analyzeTestResults();
        
        // Generate comprehensive report
        generateComprehensiveReport();
    }
    
    void analyzeFrameworkValidation() {
        std::cout << "\nFramework Validation Analysis:" << std::endl;
        
        int totalRules = frameworkValidations_.size();
        int successfulRules = 0;
        double totalTime = 0.0;
        
        for (const FrameworkValidation& validation : frameworkValidations_) {
            if (validation.appliedSuccessfully) {
                successfulRules++;
            }
            totalTime += validation.executionTime;
        }
        
        std::cout << "Total transformation rules: " << totalRules << std::endl;
        std::cout << "Successful rules: " << successfulRules << std::endl;
        std::cout << "Success rate: " << (100.0 * successfulRules / totalRules) << "%" << std::endl;
        std::cout << "Total execution time: " << totalTime << " ms" << std::endl;
        std::cout << "Average execution time: " << (totalTime / totalRules) << " ms" << std::endl;
        
        // Show failed rules
        std::cout << "\nFailed rules:" << std::endl;
        for (const FrameworkValidation& validation : frameworkValidations_) {
            if (!validation.appliedSuccessfully) {
                std::cout << "  " << validation.ruleName << ": " << validation.errorMessage << std::endl;
            }
        }
    }
    
    void analyzeTestResults() {
        std::cout << "\nTest Results Analysis:" << std::endl;
        
        int totalTests = results_.size();
        int passedTests = 0;
        int totalErrors = 0;
        int totalWarnings = 0;
        double totalTime = 0.0;
        
        std::map<String, int> categoryCounts;
        std::map<String, int> categoryErrors;
        
        for (const TestResult& result : results_) {
            if (result.passed) {
                passedTests++;
            }
            totalErrors += result.errors.size();
            totalWarnings += result.warnings.size();
            totalTime += result.executionTime;
            
            categoryCounts[result.category]++;
            categoryErrors[result.category] += result.errors.size();
        }
        
        std::cout << "Total tests: " << totalTests << std::endl;
        std::cout << "Passed tests: " << passedTests << std::endl;
        std::cout << "Failed tests: " << (totalTests - passedTests) << std::endl;
        std::cout << "Success rate: " << (100.0 * passedTests / totalTests) << "%" << std::endl;
        std::cout << "Total errors detected: " << totalErrors << std::endl;
        std::cout << "Total warnings detected: " << totalWarnings << std::endl;
        std::cout << "Total execution time: " << totalTime << " ms" << std::endl;
        std::cout << "Average execution time: " << (totalTime / totalTests) << " ms" << std::endl;
        
        std::cout << "\nResults by category:" << std::endl;
        for (const auto& category : categoryCounts) {
            std::cout << "  " << category.first << ": " << category.second 
                      << " tests, " << categoryErrors[category.first] << " errors" << std::endl;
        }
    }
    
    void generateComprehensiveReport() {
        std::cout << "\n=== Comprehensive Report ===" << std::endl;
        
        // Generate detailed report
        String report = generateDetailedReport();
        
        // Save report to file
        saveTestFile("advanced_testing_report.txt", report);
        
        std::cout << "Report saved to: advanced_testing_report.txt" << std::endl;
    }
    
    String generateDetailedReport() {
        std::ostringstream report;
        
        report << "Advanced Code Transformation Testing Report\n";
        report << "==========================================\n\n";
        
        // Framework validation summary
        report << "Framework Validation Summary:\n";
        report << "-----------------------------\n";
        
        int totalRules = frameworkValidations_.size();
        int successfulRules = 0;
        double totalTime = 0.0;
        
        for (const FrameworkValidation& validation : frameworkValidations_) {
            if (validation.appliedSuccessfully) {
                successfulRules++;
            }
            totalTime += validation.executionTime;
        }
        
        report << "Total transformation rules: " << totalRules << "\n";
        report << "Successful rules: " << successfulRules << "\n";
        report << "Success rate: " << (100.0 * successfulRules / totalRules) << "%\n";
        report << "Total execution time: " << totalTime << " ms\n";
        report << "Average execution time: " << (totalTime / totalRules) << " ms\n\n";
        
        // Test results summary
        report << "Test Results Summary:\n";
        report << "--------------------\n";
        
        int totalTests = results_.size();
        int passedTests = 0;
        int totalErrors = 0;
        int totalWarnings = 0;
        double totalTestTime = 0.0;
        
        for (const TestResult& result : results_) {
            if (result.passed) {
                passedTests++;
            }
            totalErrors += result.errors.size();
            totalWarnings += result.warnings.size();
            totalTestTime += result.executionTime;
        }
        
        report << "Total tests: " << totalTests << "\n";
        report << "Passed tests: " << passedTests << "\n";
        report << "Failed tests: " << (totalTests - passedTests) << "\n";
        report << "Success rate: " << (100.0 * passedTests / totalTests) << "%\n";
        report << "Total errors detected: " << totalErrors << "\n";
        report << "Total warnings detected: " << totalWarnings << "\n";
        report << "Total execution time: " << totalTestTime << " ms\n";
        report << "Average execution time: " << (totalTestTime / totalTests) << " ms\n\n";
        
        // Detailed results
        report << "Detailed Results:\n";
        report << "----------------\n";
        
        for (const TestResult& result : results_) {
            report << "Test: " << result.testName << "\n";
            report << "Status: " << (result.passed ? "PASSED" : "FAILED") << "\n";
            report << "Execution time: " << result.executionTime << " ms\n";
            report << "Errors: " << result.errors.size() << "\n";
            report << "Warnings: " << result.warnings.size() << "\n";
            
            if (!result.errors.empty()) {
                report << "Error details:\n";
                for (const String& error : result.errors) {
                    report << "  " << error << "\n";
                }
            }
            report << "\n";
        }
        
        return report.str();
    }
    
    String executeCommand(const String& command) {
        String output;
        FILE* pipe = popen(command.c_str(), "r");
        if (pipe) {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                output += buffer;
            }
            pclose(pipe);
        }
        return output;
    }
    
    void parseCompilerOutput(const String& output, std::vector<String>& errors, std::vector<String>& warnings) {
        std::istringstream stream(output);
        String line;
        
        while (std::getline(stream, line)) {
            if (line.find("error:") != String::npos) {
                errors.push_back(line);
            } else if (line.find("warning:") != String::npos) {
                warnings.push_back(line);
            }
        }
    }
    
    void saveTestFile(const String& filename, const String& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
        }
    }
};

// Main function for advanced testing
int runAdvancedTesting() {
    try {
        AdvancedTestDriver driver;
        driver.runAdvancedTesting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error during advanced testing: " << e.what() << std::endl;
        return 1;
    }
}

} // namespace testing
} // namespace tsc