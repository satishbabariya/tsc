#include "tsc/testing/CodeTransformationFramework.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>

namespace tsc {
namespace testing {

class TargetedErrorTestGenerator {
public:
    TargetedErrorTestGenerator() {
        transformer_ = createCodeTransformer();
        validator_ = createTestValidator();
    }
    
    void generateComprehensiveTestSuite() {
        std::cout << "=== Generating Targeted Error Test Suite ===" << std::endl;
        
        // Generate tests for each compiler phase
        generateParserTests();
        generateSemanticTests();
        generateTypeCheckerTests();
        generateCodeGenTests();
        generateIntegrationTests();
        
        std::cout << "=== Test Suite Generation Complete ===" << std::endl;
    }
    
private:
    unique_ptr<CodeTransformer> transformer_;
    unique_ptr<TestValidator> validator_;
    
    void generateParserTests() {
        std::cout << "\n--- Generating Parser Tests ---" << std::endl;
        
        // Test 1: Missing semicolons
        generateMissingSemicolonTests();
        
        // Test 2: Missing braces
        generateMissingBraceTests();
        
        // Test 3: Missing parentheses
        generateMissingParenthesisTests();
        
        // Test 4: Invalid expressions
        generateInvalidExpressionTests();
        
        // Test 5: Malformed statements
        generateMalformedStatementTests();
    }
    
    void generateSemanticTests() {
        std::cout << "\n--- Generating Semantic Tests ---" << std::endl;
        
        // Test 1: Undefined variables
        generateUndefinedVariableTests();
        
        // Test 2: Duplicate declarations
        generateDuplicateDeclarationTests();
        
        // Test 3: Scope violations
        generateScopeViolationTests();
        
        // Test 4: Function signature mismatches
        generateFunctionSignatureTests();
    }
    
    void generateTypeCheckerTests() {
        std::cout << "\n--- Generating Type Checker Tests ---" << std::endl;
        
        // Test 1: Type mismatches
        generateTypeMismatchTests();
        
        // Test 2: Incompatible operations
        generateIncompatibleOperationTests();
        
        // Test 3: Missing type annotations
        generateMissingTypeAnnotationTests();
        
        // Test 4: Invalid type references
        generateInvalidTypeReferenceTests();
    }
    
    void generateCodeGenTests() {
        std::cout << "\n--- Generating Code Generation Tests ---" << std::endl;
        
        // Test 1: LLVM generation errors
        generateLLVMGenerationTests();
        
        // Test 2: Memory management errors
        generateMemoryManagementTests();
        
        // Test 3: Optimization errors
        generateOptimizationTests();
    }
    
    void generateIntegrationTests() {
        std::cout << "\n--- Generating Integration Tests ---" << std::endl;
        
        // Test 1: Cross-phase error propagation
        generateCrossPhaseTests();
        
        // Test 2: Complex error scenarios
        generateComplexErrorTests();
        
        // Test 3: Performance stress tests
        generatePerformanceStressTests();
    }
    
    void generateMissingSemicolonTests() {
        std::vector<String> testCases = {
            R"(
// Missing semicolon after variable declaration
let x: number = 42
let y: string = "hello"
let z: boolean = true
)",
            R"(
// Missing semicolon after expression
console.log("test")
let result = 42
)",
            R"(
// Missing semicolon after return statement
function test(): number {
    return 42
}
)",
            R"(
// Missing semicolon after assignment
let x = 42
x = 100
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "parser_missing_semicolon_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateMissingBraceTests() {
        std::vector<String> testCases = {
            R"(
// Missing opening brace
function test(): void {
    console.log("test")
)",
            R"(
// Missing closing brace
function test(): void {
    console.log("test")
    let x = 42
)",
            R"(
// Missing braces in if statement
if (true) {
    console.log("test")
)",
            R"(
// Missing braces in for loop
for (let i = 0; i < 10; i++) {
    console.log(i)
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "parser_missing_brace_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateMissingParenthesisTests() {
        std::vector<String> testCases = {
            R"(
// Missing opening parenthesis in function call
console.log("test")
)",
            R"(
// Missing closing parenthesis in function call
console.log("test"
)",
            R"(
// Missing parentheses in if condition
if true) {
    console.log("test")
}
)",
            R"(
// Missing parentheses in for loop
for let i = 0; i < 10; i++) {
    console.log(i)
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "parser_missing_parenthesis_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateInvalidExpressionTests() {
        std::vector<String> testCases = {
            R"(
// Invalid binary expression
let result = 42 + "hello"
)",
            R"(
// Invalid unary expression
let result = ++"hello"
)",
            R"(
// Invalid ternary expression
let result = true ? 42 : "hello"
)",
            R"(
// Invalid array access
let arr = [1, 2, 3]
let result = arr["invalid"]
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "parser_invalid_expression_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateMalformedStatementTests() {
        std::vector<String> testCases = {
            R"(
// Malformed if statement
if (true) {
    console.log("test")
} else {
    console.log("else")
)",
            R"(
// Malformed for loop
for (let i = 0; i < 10; i++) {
    console.log(i)
}
)",
            R"(
// Malformed while loop
while (true) {
    console.log("infinite")
}
)",
            R"(
// Malformed switch statement
switch (value) {
    case 1:
        console.log("one")
        break
    default:
        console.log("default")
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "parser_malformed_statement_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateUndefinedVariableTests() {
        std::vector<String> testCases = {
            R"(
// Undefined variable reference
function test(): void {
    console.log(undefinedVariable)
}
)",
            R"(
// Undefined function call
function test(): void {
    undefinedFunction()
}
)",
            R"(
// Undefined property access
function test(): void {
    let obj = { name: "test" }
    console.log(obj.undefinedProperty)
}
)",
            R"(
// Undefined method call
function test(): void {
    let str = "hello"
    str.undefinedMethod()
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "semantic_undefined_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateDuplicateDeclarationTests() {
        std::vector<String> testCases = {
            R"(
// Duplicate variable declaration
let x: number = 42
let x: string = "hello"
)",
            R"(
// Duplicate function declaration
function test(): void {
    console.log("first")
}
function test(): void {
    console.log("second")
}
)",
            R"(
// Duplicate class declaration
class TestClass {
    name: string = "test"
}
class TestClass {
    value: number = 42
}
)",
            R"(
// Duplicate interface declaration
interface TestInterface {
    name: string
}
interface TestInterface {
    value: number
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "semantic_duplicate_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateScopeViolationTests() {
        std::vector<String> testCases = {
            R"(
// Accessing private member
class TestClass {
    private privateMember: number = 42
    public publicMember: string = "public"
}
let instance = new TestClass()
console.log(instance.privateMember)
)",
            R"(
// Using 'this' outside class
function test(): void {
    console.log(this)
}
)",
            R"(
// Using 'super' outside class
function test(): void {
    super.test()
}
)",
            R"(
// Break outside loop
function test(): void {
    break
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "semantic_scope_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateFunctionSignatureTests() {
        std::vector<String> testCases = {
            R"(
// Wrong number of arguments
function test(a: number, b: string): void {
    console.log(a, b)
}
test(42)
)",
            R"(
// Wrong argument types
function test(a: number, b: string): void {
    console.log(a, b)
}
test("hello", 42)
)",
            R"(
// Missing return statement
function test(): number {
    console.log("test")
}
)",
            R"(
// Wrong return type
function test(): number {
    return "hello"
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "semantic_function_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateTypeMismatchTests() {
        std::vector<String> testCases = {
            R"(
// Type mismatch in assignment
let x: number = "hello"
)",
            R"(
// Type mismatch in binary operation
let result = 42 + "hello"
)",
            R"(
// Type mismatch in function parameter
function test(x: number): void {
    console.log(x)
}
test("hello")
)",
            R"(
// Type mismatch in return statement
function test(): number {
    return "hello"
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "type_mismatch_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateIncompatibleOperationTests() {
        std::vector<String> testCases = {
            R"(
// Incompatible binary operations
let result1 = 42 + true
let result2 = "hello" - "world"
let result3 = null * undefined
)",
            R"(
// Incompatible unary operations
let result1 = ++"hello"
let result2 = --true
let result3 = !42
)",
            R"(
// Incompatible array operations
let arr: number[] = [1, 2, 3]
arr.push("hello")
)",
            R"(
// Incompatible object operations
let obj: { name: string } = { name: "test" }
obj.name = 42
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "type_incompatible_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateMissingTypeAnnotationTests() {
        std::vector<String> testCases = {
            R"(
// Missing type annotation
let x = 42
let y = "hello"
let z = true
)",
            R"(
// Missing function return type
function test() {
    return 42
}
)",
            R"(
// Missing parameter type
function test(x) {
    return x * 2
}
)",
            R"(
// Missing array element type
let arr = [1, "hello", true]
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "type_missing_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateInvalidTypeReferenceTests() {
        std::vector<String> testCases = {
            R"(
// Invalid type reference
let x: UndefinedType = 42
)",
            R"(
// Circular type reference
type A = B
type B = A
)",
            R"(
// Invalid generic type
let x: Array<UndefinedType> = []
)",
            R"(
// Invalid union type
let x: number | UndefinedType = 42
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "type_invalid_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateLLVMGenerationTests() {
        std::vector<String> testCases = {
            R"(
// Complex expression that might cause LLVM generation issues
function complexExpression(): number {
    let result = 0
    for (let i = 0; i < 1000000; i++) {
        result += Math.sqrt(i) * Math.sin(i) + Math.cos(i)
    }
    return result
}
)",
            R"(
// Large array allocation
function largeArray(): void {
    let arr = new Array(1000000)
    for (let i = 0; i < 1000000; i++) {
        arr[i] = i * i
    }
}
)",
            R"(
// Deep recursion
function deepRecursion(n: number): number {
    if (n <= 0) return 1
    return deepRecursion(n - 1) + deepRecursion(n - 2)
}
)",
            R"(
// Complex object with many properties
function complexObject(): void {
    let obj = {
        prop1: 1, prop2: 2, prop3: 3, prop4: 4, prop5: 5,
        prop6: 6, prop7: 7, prop8: 8, prop9: 9, prop10: 10,
        prop11: 11, prop12: 12, prop13: 13, prop14: 14, prop15: 15
    }
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "codegen_llvm_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateMemoryManagementTests() {
        std::vector<String> testCases = {
            R"(
// Potential memory leak
function memoryLeak(): void {
    let arr = new Array(1000000)
    // No cleanup
}
)",
            R"(
// Double free simulation
function doubleFree(): void {
    let obj = { data: "test" }
    // Simulate double free
    obj = null
    obj = null
}
)",
            R"(
// Use after free simulation
function useAfterFree(): void {
    let obj = { data: "test" }
    obj = null
    console.log(obj.data)
}
)",
            R"(
// Buffer overflow simulation
function bufferOverflow(): void {
    let arr = new Array(10)
    for (let i = 0; i < 100; i++) {
        arr[i] = i
    }
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "codegen_memory_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateOptimizationTests() {
        std::vector<String> testCases = {
            R"(
// Dead code elimination test
function deadCode(): number {
    let x = 42
    if (false) {
        return x
    }
    return 0
}
)",
            R"(
// Constant propagation test
function constantPropagation(): number {
    const a = 42
    const b = 10
    return a + b
}
)",
            R"(
// Loop optimization test
function loopOptimization(): number {
    let sum = 0
    for (let i = 0; i < 1000; i++) {
        sum += i
    }
    return sum
}
)",
            R"(
// Function inlining test
function add(a: number, b: number): number {
    return a + b
}
function test(): number {
    return add(42, 10)
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "codegen_optimization_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateCrossPhaseTests() {
        std::vector<String> testCases = {
            R"(
// Syntax error that affects semantic analysis
let x: number = 42
let y: string = "hello"
// Missing semicolon will cause semantic analysis to fail
let z = x + y
)",
            R"(
// Type error that affects code generation
function test(x: number): number {
    return x * 2
}
let result = test("hello") // Type error affects code generation
)",
            R"(
// Semantic error that affects type checking
let x = 42
let y = "hello"
let z = x + y // Semantic error affects type checking
)",
            R"(
// Multiple phase errors
let x: number = "hello" // Type error
let y = undefinedVariable // Semantic error
let z = x + y // Code generation error
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "integration_cross_phase_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generateComplexErrorTests() {
        std::vector<String> testCases = {
            R"(
// Complex nested errors
function complexErrors(): void {
    if (true) {
        let x: number = "hello" // Type error
        console.log(undefinedVariable) // Semantic error
        let y = x + undefinedVariable // Multiple errors
    }
}
)",
            R"(
// Error in class context
class TestClass {
    private member: number = "hello" // Type error
    public method(): void {
        console.log(undefinedVariable) // Semantic error
        this.member = "world" // Type error
    }
}
)",
            R"(
// Error in function with generics
function genericFunction<T>(x: T): T {
    return x + 1 // Type error for non-numeric types
}
let result = genericFunction("hello")
)",
            R"(
// Error in async function
async function asyncFunction(): Promise<number> {
    let x: number = "hello" // Type error
    return await Promise.resolve(x)
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "integration_complex_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void generatePerformanceStressTests() {
        std::vector<String> testCases = {
            R"(
// Performance stress test with many errors
function performanceStress(): void {
    for (let i = 0; i < 1000; i++) {
        let x: number = "hello" // Type error
        console.log(undefinedVariable) // Semantic error
        let y = x + undefinedVariable // Multiple errors
    }
}
)",
            R"(
// Memory stress test
function memoryStress(): void {
    let arrays = []
    for (let i = 0; i < 1000; i++) {
        let arr = new Array(1000)
        arrays.push(arr)
    }
    // Access undefined property
    console.log(arrays.undefinedProperty)
}
)",
            R"(
// Deep nesting stress test
function deepNesting(): void {
    if (true) {
        if (true) {
            if (true) {
                if (true) {
                    if (true) {
                        let x: number = "hello" // Type error deep in nesting
                        console.log(undefinedVariable) // Semantic error deep in nesting
                    }
                }
            }
        }
    }
}
)",
            R"(
// Large expression stress test
function largeExpression(): number {
    let result = 0
    for (let i = 0; i < 100; i++) {
        result += (i * 2 + 1) * (i + 1) / (i + 2) * Math.sqrt(i + 1)
    }
    return result + undefinedVariable // Semantic error in large expression
}
)"
        };
        
        for (size_t i = 0; i < testCases.size(); ++i) {
            String filename = "integration_performance_" + std::to_string(i + 1) + ".ts";
            saveTestFile(filename, testCases[i]);
        }
    }
    
    void saveTestFile(const String& filename, const String& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Generated: " << filename << std::endl;
        } else {
            std::cerr << "Failed to create file: " << filename << std::endl;
        }
    }
};

// Main function for generating targeted tests
int generateTargetedTests() {
    try {
        TargetedErrorTestGenerator generator;
        generator.generateComprehensiveTestSuite();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error generating targeted tests: " << e.what() << std::endl;
        return 1;
    }
}

} // namespace testing
} // namespace tsc