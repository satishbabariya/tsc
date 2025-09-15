#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <chrono>
#include <algorithm>

// Comprehensive Error Testing Driver
class ComprehensiveErrorTestingDriver {
public:
    void runComprehensiveTesting() {
        std::cout << "=== Comprehensive Error Testing Suite ===" << std::endl;
        
        // Step 1: Generate targeted test cases
        generateTargetedTestCases();
        
        // Step 2: Run validation tests
        runValidationTests();
        
        // Step 3: Analyze results
        analyzeResults();
        
        // Step 4: Generate coverage report
        generateCoverageReport();
        
        std::cout << "=== Comprehensive Testing Complete ===" << std::endl;
    }
    
private:
    struct TestResult {
        std::string testName;
        bool passed;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
        double executionTime;
        std::string category;
    };
    
    std::vector<TestResult> results_;
    
    void generateTargetedTestCases() {
        std::cout << "\n--- Generating Targeted Test Cases ---" << std::endl;
        
        // Generate parser tests
        generateParserTests();
        
        // Generate semantic tests
        generateSemanticTests();
        
        // Generate type checker tests
        generateTypeCheckerTests();
        
        // Generate code generation tests
        generateCodeGenTests();
        
        // Generate integration tests
        generateIntegrationTests();
    }
    
    void generateParserTests() {
        std::cout << "Generating parser tests..." << std::endl;
        
        std::vector<std::pair<std::string, std::string>> parserTests = {
            {"parser_missing_semicolon_1.ts", R"(
// Missing semicolon after variable declaration
let x: number = 42
let y: string = "hello"
let z: boolean = true
)"},
            {"parser_missing_semicolon_2.ts", R"(
// Missing semicolon after expression
console.log("test")
let result = 42
)"},
            {"parser_missing_brace_1.ts", R"(
// Missing opening brace
function test(): void {
    console.log("test")
)"},
            {"parser_missing_brace_2.ts", R"(
// Missing closing brace
function test(): void {
    console.log("test")
    let x = 42
)"},
            {"parser_missing_parenthesis_1.ts", R"(
// Missing opening parenthesis in function call
console.log("test")
)"},
            {"parser_missing_parenthesis_2.ts", R"(
// Missing closing parenthesis in function call
console.log("test"
)"},
            {"parser_invalid_expression_1.ts", R"(
// Invalid binary expression
let result = 42 + "hello"
)"},
            {"parser_invalid_expression_2.ts", R"(
// Invalid unary expression
let result = ++"hello"
)"},
            {"parser_malformed_statement_1.ts", R"(
// Malformed if statement
if (true) {
    console.log("test")
)"},
            {"parser_malformed_statement_2.ts", R"(
// Malformed for loop
for (let i = 0; i < 10; i++) {
    console.log(i)
)"}
        };
        
        for (const auto& test : parserTests) {
            saveTestFile(test.first, test.second);
        }
    }
    
    void generateSemanticTests() {
        std::cout << "Generating semantic tests..." << std::endl;
        
        std::vector<std::pair<std::string, std::string>> semanticTests = {
            {"semantic_undefined_1.ts", R"(
// Undefined variable reference
function test(): void {
    console.log(undefinedVariable)
}
)"},
            {"semantic_undefined_2.ts", R"(
// Undefined function call
function test(): void {
    undefinedFunction()
}
)"},
            {"semantic_duplicate_1.ts", R"(
// Duplicate variable declaration
let x: number = 42
let x: string = "hello"
)"},
            {"semantic_duplicate_2.ts", R"(
// Duplicate function declaration
function test(): void {
    console.log("first")
}
function test(): void {
    console.log("second")
}
)"},
            {"semantic_scope_1.ts", R"(
// Accessing private member
class TestClass {
    private privateMember: number = 42
    public publicMember: string = "public"
}
let instance = new TestClass()
console.log(instance.privateMember)
)"},
            {"semantic_scope_2.ts", R"(
// Using 'this' outside class
function test(): void {
    console.log(this)
}
)"},
            {"semantic_function_1.ts", R"(
// Wrong number of arguments
function test(a: number, b: string): void {
    console.log(a, b)
}
test(42)
)"},
            {"semantic_function_2.ts", R"(
// Wrong argument types
function test(a: number, b: string): void {
    console.log(a, b)
}
test("hello", 42)
)"}
        };
        
        for (const auto& test : semanticTests) {
            saveTestFile(test.first, test.second);
        }
    }
    
    void generateTypeCheckerTests() {
        std::cout << "Generating type checker tests..." << std::endl;
        
        std::vector<std::pair<std::string, std::string>> typeTests = {
            {"type_mismatch_1.ts", R"(
// Type mismatch in assignment
let x: number = "hello"
)"},
            {"type_mismatch_2.ts", R"(
// Type mismatch in binary operation
let result = 42 + "hello"
)"},
            {"type_mismatch_3.ts", R"(
// Type mismatch in function parameter
function test(x: number): void {
    console.log(x)
}
test("hello")
)"},
            {"type_incompatible_1.ts", R"(
// Incompatible binary operations
let result1 = 42 + true
let result2 = "hello" - "world"
let result3 = null * undefined
)"},
            {"type_incompatible_2.ts", R"(
// Incompatible unary operations
let result1 = ++"hello"
let result2 = --true
let result3 = !42
)"},
            {"type_missing_1.ts", R"(
// Missing type annotation
let x = 42
let y = "hello"
let z = true
)"},
            {"type_missing_2.ts", R"(
// Missing function return type
function test() {
    return 42
}
)"},
            {"type_invalid_1.ts", R"(
// Invalid type reference
let x: UndefinedType = 42
)"},
            {"type_invalid_2.ts", R"(
// Circular type reference
type A = B
type B = A
)"}
        };
        
        for (const auto& test : typeTests) {
            saveTestFile(test.first, test.second);
        }
    }
    
    void generateCodeGenTests() {
        std::cout << "Generating code generation tests..." << std::endl;
        
        std::vector<std::pair<std::string, std::string>> codeGenTests = {
            {"codegen_llvm_1.ts", R"(
// Complex expression that might cause LLVM generation issues
function complexExpression(): number {
    let result = 0
    for (let i = 0; i < 1000000; i++) {
        result += Math.sqrt(i) * Math.sin(i) + Math.cos(i)
    }
    return result
}
)"},
            {"codegen_llvm_2.ts", R"(
// Large array allocation
function largeArray(): void {
    let arr = new Array(1000000)
    for (let i = 0; i < 1000000; i++) {
        arr[i] = i * i
    }
}
)"},
            {"codegen_memory_1.ts", R"(
// Potential memory leak
function memoryLeak(): void {
    let arr = new Array(1000000)
    // No cleanup
}
)"},
            {"codegen_memory_2.ts", R"(
// Double free simulation
function doubleFree(): void {
    let obj = { data: "test" }
    // Simulate double free
    obj = null
    obj = null
}
)"},
            {"codegen_optimization_1.ts", R"(
// Dead code elimination test
function deadCode(): number {
    let x = 42
    if (false) {
        return x
    }
    return 0
}
)"},
            {"codegen_optimization_2.ts", R"(
// Constant propagation test
function constantPropagation(): number {
    const a = 42
    const b = 10
    return a + b
}
)"}
        };
        
        for (const auto& test : codeGenTests) {
            saveTestFile(test.first, test.second);
        }
    }
    
    void generateIntegrationTests() {
        std::cout << "Generating integration tests..." << std::endl;
        
        std::vector<std::pair<std::string, std::string>> integrationTests = {
            {"integration_cross_phase_1.ts", R"(
// Syntax error that affects semantic analysis
let x: number = 42
let y: string = "hello"
// Missing semicolon will cause semantic analysis to fail
let z = x + y
)"},
            {"integration_cross_phase_2.ts", R"(
// Type error that affects code generation
function test(x: number): number {
    return x * 2
}
let result = test("hello") // Type error affects code generation
)"},
            {"integration_cross_phase_3.ts", R"(
// Semantic error that affects type checking
let x = 42
let y = "hello"
let z = x + y // Semantic error affects type checking
)"},
            {"integration_cross_phase_4.ts", R"(
// Multiple phase errors
let x: number = "hello" // Type error
let y = undefinedVariable // Semantic error
let z = x + y // Code generation error
)"},
            {"integration_complex_1.ts", R"(
// Complex nested errors
function complexErrors(): void {
    if (true) {
        let x: number = "hello" // Type error
        console.log(undefinedVariable) // Semantic error
        let y = x + undefinedVariable // Multiple errors
    }
}
)"},
            {"integration_complex_2.ts", R"(
// Error in class context
class TestClass {
    private member: number = "hello" // Type error
    public method(): void {
        console.log(undefinedVariable) // Semantic error
        this.member = "world" // Type error
    }
}
)"},
            {"integration_performance_1.ts", R"(
// Performance stress test with many errors
function performanceStress(): void {
    for (let i = 0; i < 1000; i++) {
        let x: number = "hello" // Type error
        console.log(undefinedVariable) // Semantic error
        let y = x + undefinedVariable // Multiple errors
    }
}
)"},
            {"integration_performance_2.ts", R"(
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
)"}
        };
        
        for (const auto& test : integrationTests) {
            saveTestFile(test.first, test.second);
        }
    }
    
    void runValidationTests() {
        std::cout << "\n--- Running Validation Tests ---" << std::endl;
        
        // Get all generated test files
        std::vector<std::string> testFiles = {
            "parser_missing_semicolon_1.ts", "parser_missing_semicolon_2.ts",
            "parser_missing_brace_1.ts", "parser_missing_brace_2.ts",
            "parser_missing_parenthesis_1.ts", "parser_missing_parenthesis_2.ts",
            "parser_invalid_expression_1.ts", "parser_invalid_expression_2.ts",
            "parser_malformed_statement_1.ts", "parser_malformed_statement_2.ts",
            "semantic_undefined_1.ts", "semantic_undefined_2.ts",
            "semantic_duplicate_1.ts", "semantic_duplicate_2.ts",
            "semantic_scope_1.ts", "semantic_scope_2.ts",
            "semantic_function_1.ts", "semantic_function_2.ts",
            "type_mismatch_1.ts", "type_mismatch_2.ts", "type_mismatch_3.ts",
            "type_incompatible_1.ts", "type_incompatible_2.ts",
            "type_missing_1.ts", "type_missing_2.ts",
            "type_invalid_1.ts", "type_invalid_2.ts",
            "codegen_llvm_1.ts", "codegen_llvm_2.ts",
            "codegen_memory_1.ts", "codegen_memory_2.ts",
            "codegen_optimization_1.ts", "codegen_optimization_2.ts",
            "integration_cross_phase_1.ts", "integration_cross_phase_2.ts",
            "integration_cross_phase_3.ts", "integration_cross_phase_4.ts",
            "integration_complex_1.ts", "integration_complex_2.ts",
            "integration_performance_1.ts", "integration_performance_2.ts"
        };
        
        for (const std::string& testFile : testFiles) {
            TestResult result = runTest(testFile);
            results_.push_back(result);
        }
    }
    
    TestResult runTest(const std::string& filename) {
        TestResult result;
        result.testName = filename;
        result.passed = false;
        
        // Determine category
        if (filename.find("parser_") != std::string::npos) {
            result.category = "Parser";
        } else if (filename.find("semantic_") != std::string::npos) {
            result.category = "Semantic";
        } else if (filename.find("type_") != std::string::npos) {
            result.category = "Type Checker";
        } else if (filename.find("codegen_") != std::string::npos) {
            result.category = "Code Generation";
        } else if (filename.find("integration_") != std::string::npos) {
            result.category = "Integration";
        } else {
            result.category = "Unknown";
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Run the compiler
        std::string command = "./build/tsc " + filename + " 2>&1";
        std::string output = executeCommand(command);
        
        auto end = std::chrono::high_resolution_clock::now();
        result.executionTime = std::chrono::duration<double, std::milli>(end - start).count();
        
        // Parse errors and warnings
        parseCompilerOutput(output, result.errors, result.warnings);
        
        // Determine if test passed (should have errors for error tests)
        result.passed = !result.errors.empty();
        
        return result;
    }
    
    std::string executeCommand(const std::string& command) {
        std::string output;
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
    
    void parseCompilerOutput(const std::string& output, std::vector<std::string>& errors, std::vector<std::string>& warnings) {
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            if (line.find("error:") != std::string::npos) {
                errors.push_back(line);
            } else if (line.find("warning:") != std::string::npos) {
                warnings.push_back(line);
            }
        }
    }
    
    void analyzeResults() {
        std::cout << "\n--- Analyzing Results ---" << std::endl;
        
        int totalTests = results_.size();
        int passedTests = 0;
        int totalErrors = 0;
        int totalWarnings = 0;
        double totalTime = 0.0;
        
        std::map<std::string, int> categoryCounts;
        std::map<std::string, int> categoryErrors;
        
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
    
    void generateCoverageReport() {
        std::cout << "\n--- Coverage Report ---" << std::endl;
        
        std::map<std::string, int> categoryTests;
        std::map<std::string, int> categoryErrors;
        
        for (const TestResult& result : results_) {
            categoryTests[result.category]++;
            categoryErrors[result.category] += result.errors.size();
        }
        
        std::cout << "Error reporting coverage by component:" << std::endl;
        for (const auto& category : categoryTests) {
            double coverage = (double)categoryErrors[category.first] / categoryTests[category.first] * 100.0;
            std::cout << "  " << category.first << ": " << coverage << "% coverage" << std::endl;
        }
        
        // Analyze error patterns
        std::map<std::string, int> errorPatterns;
        for (const TestResult& result : results_) {
            for (const std::string& error : result.errors) {
                // Extract error type
                size_t errorPos = error.find("error:");
                if (errorPos != std::string::npos) {
                    std::string errorType = error.substr(errorPos + 6);
                    // Remove leading whitespace
                    errorType.erase(0, errorType.find_first_not_of(" \t"));
                    // Take first word
                    size_t spacePos = errorType.find(' ');
                    if (spacePos != std::string::npos) {
                        errorType = errorType.substr(0, spacePos);
                    }
                    errorPatterns[errorType]++;
                }
            }
        }
        
        std::cout << "\nMost common error types:" << std::endl;
        for (const auto& pattern : errorPatterns) {
            std::cout << "  " << pattern.first << ": " << pattern.second << " occurrences" << std::endl;
        }
    }
    
    void saveTestFile(const std::string& filename, const std::string& content) {
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

int main() {
    try {
        ComprehensiveErrorTestingDriver driver;
        driver.runComprehensiveTesting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}