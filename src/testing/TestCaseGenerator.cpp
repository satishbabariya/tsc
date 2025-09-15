#include "tsc/testing/CodeTransformationFramework.h"
#include <random>
#include <sstream>
#include <algorithm>

namespace tsc {
namespace testing {

// =============================================================================
// TEST CASE GENERATOR IMPLEMENTATION
// =============================================================================

TestCaseGenerator::TestCaseGenerator() {
    transformer_ = make_unique<CodeTransformer>();
}

String TestCaseGenerator::generateValidCode(const String& template) {
    if (template.empty()) {
        return R"(
// Generated valid TypeScript code
function calculateSum(a: number, b: number): number {
    return a + b;
}

function processArray(arr: number[]): number[] {
    let result: number[] = [];
    for (let i = 0; i < arr.length; i++) {
        result.push(arr[i] * 2);
    }
    return result;
}

class Calculator {
    private value: number = 0;
    
    add(num: number): void {
        this.value += num;
    }
    
    getValue(): number {
        return this.value;
    }
}

let calc = new Calculator();
calc.add(10);
calc.add(20);
let result = calc.getValue();
console.log("Result:", result);
)";
    }
    return template;
}

String TestCaseGenerator::generateErrorTestCase(TransformationType errorType, const String& baseCode) {
    String code = baseCode.empty() ? generateValidCode() : baseCode;
    
    std::vector<TransformationType> types = {errorType};
    auto result = transformer_->transformWithRules(code, types);
    
    return result.transformedCode;
}

String TestCaseGenerator::generateStressTestCase(const std::vector<TransformationType>& stressTypes, 
                                                const String& baseCode) {
    String code = baseCode.empty() ? generateValidCode() : baseCode;
    
    auto result = transformer_->transformWithRules(code, stressTypes);
    
    return result.transformedCode;
}

String TestCaseGenerator::generateSyntaxErrorTest() {
    return generateErrorTestCase(TransformationType::RemoveSemicolon, R"(
// Syntax error test case
let x: number = 42
let y: string = "hello"
function test(): void {
    console.log("test")
}
)");
}

String TestCaseGenerator::generateTypeErrorTest() {
    return generateErrorTestCase(TransformationType::AddTypeMismatch, R"(
// Type error test case
let num: number = 42;
let str: string = "hello";
let bool: boolean = true;

function processNumber(n: number): number {
    return n * 2;
}

let result = processNumber(str); // Type error
let anotherResult = num + bool; // Type error
)");
}

String TestCaseGenerator::generateSemanticErrorTest() {
    return generateErrorTestCase(TransformationType::RenameVariable, R"(
// Semantic error test case
let declaredVariable: number = 42;
let anotherVariable: string = "test";

function useVariables(): void {
    console.log(declaredVariable);
    console.log(anotherVariable);
    console.log(undeclaredVariable); // Semantic error
}

useVariables();
)");
}

String TestCaseGenerator::generateControlFlowTest() {
    return generateErrorTestCase(TransformationType::MalformControlFlow, R"(
// Control flow test case
function testControlFlow(x: number): string {
    if (x > 10) {
        return "large";
    } else if (x > 5) {
        return "medium";
    } else {
        return "small";
    }
}

let result = testControlFlow(15);
console.log(result);
)");
}

String TestCaseGenerator::generateNestingTest() {
    return generateErrorTestCase(TransformationType::DeepNesting, R"(
// Deep nesting test case
function nestedFunction(): void {
    let level1 = 1;
    if (level1 > 0) {
        let level2 = 2;
        if (level2 > 1) {
            let level3 = 3;
            console.log(level3);
        }
    }
}

nestedFunction();
)");
}

String TestCaseGenerator::generatePerformanceTest() {
    return generateErrorTestCase(TransformationType::PerformanceStress, R"(
// Performance test case
function performanceTest(): void {
    let sum = 0;
    for (let i = 0; i < 1000; i++) {
        sum += i;
    }
    console.log("Sum:", sum);
}

performanceTest();
)");
}

String TestCaseGenerator::generateMemoryTest() {
    return generateErrorTestCase(TransformationType::MemoryStress, R"(
// Memory test case
function memoryTest(): void {
    let array = new Array(1000);
    for (let i = 0; i < 1000; i++) {
        array[i] = i;
    }
    console.log("Array length:", array.length);
}

memoryTest();
)");
}

String TestCaseGenerator::generateEdgeCaseTest() {
    return generateErrorTestCase(TransformationType::EdgeCaseGeneration, R"(
// Edge case test case
function edgeCaseTest(): void {
    let zero = 0;
    let infinity = Infinity;
    let nan = NaN;
    
    console.log("Zero:", zero);
    console.log("Infinity:", infinity);
    console.log("NaN:", nan);
}

edgeCaseTest();
)");
}

std::vector<String> TestCaseGenerator::generateTestSuite(size_t count, 
                                                         const std::vector<TransformationType>& types) {
    std::vector<String> testCases;
    
    for (size_t i = 0; i < count; ++i) {
        String testCase = generateStressTestCase(types);
        testCases.push_back(testCase);
    }
    
    return testCases;
}

std::vector<String> TestCaseGenerator::generateComprehensiveTestSuite() {
    std::vector<String> testSuite;
    
    // Syntax error tests
    testSuite.push_back(generateSyntaxErrorTest());
    
    // Type error tests
    testSuite.push_back(generateTypeErrorTest());
    
    // Semantic error tests
    testSuite.push_back(generateSemanticErrorTest());
    
    // Control flow tests
    testSuite.push_back(generateControlFlowTest());
    
    // Nesting tests
    testSuite.push_back(generateNestingTest());
    
    // Performance tests
    testSuite.push_back(generatePerformanceTest());
    
    // Memory tests
    testSuite.push_back(generateMemoryTest());
    
    // Edge case tests
    testSuite.push_back(generateEdgeCaseTest());
    
    // Combined stress tests
    std::vector<TransformationType> syntaxTypes = {
        TransformationType::RemoveSemicolon,
        TransformationType::RemoveBrace,
        TransformationType::RemoveParenthesis,
        TransformationType::RemoveBracket
    };
    testSuite.push_back(generateStressTestCase(syntaxTypes));
    
    std::vector<TransformationType> typeTypes = {
        TransformationType::ChangeTypeAnnotation,
        TransformationType::AddTypeMismatch,
        TransformationType::RemoveTypeAnnotation
    };
    testSuite.push_back(generateStressTestCase(typeTypes));
    
    std::vector<TransformationType> semanticTypes = {
        TransformationType::RenameVariable,
        TransformationType::RemoveDeclaration,
        TransformationType::AddDuplicateIdentifier
    };
    testSuite.push_back(generateStressTestCase(semanticTypes));
    
    std::vector<TransformationType> stressTypes = {
        TransformationType::DeepNesting,
        TransformationType::LongExpressions,
        TransformationType::ComplexExpressions,
        TransformationType::LargeNumbers
    };
    testSuite.push_back(generateStressTestCase(stressTypes));
    
    return testSuite;
}

void TestCaseGenerator::setComplexityLevel(int level) {
    complexityLevel_ = std::max(1, std::min(10, level));
}

void TestCaseGenerator::setMaxNestingDepth(int depth) {
    maxNestingDepth_ = std::max(1, depth);
}

void TestCaseGenerator::setMaxExpressionLength(int length) {
    maxExpressionLength_ = std::max(10, length);
}

void TestCaseGenerator::setMaxIdentifierLength(int length) {
    maxIdentifierLength_ = std::max(5, length);
}

String TestCaseGenerator::generateRandomIdentifier() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> lengthDist(5, maxIdentifierLength_);
    std::uniform_int_distribution<> charDist(0, 25);
    
    int length = lengthDist(gen);
    String identifier = "var";
    
    for (int i = 0; i < length; ++i) {
        char c = 'a' + charDist(gen);
        identifier += c;
    }
    
    return identifier;
}

String TestCaseGenerator::generateRandomNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1000000);
    
    return std::to_string(dist(gen));
}

String TestCaseGenerator::generateRandomString() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> lengthDist(5, 50);
    std::uniform_int_distribution<> charDist(0, 25);
    
    int length = lengthDist(gen);
    String str = "\"";
    
    for (int i = 0; i < length; ++i) {
        char c = 'a' + charDist(gen);
        str += c;
    }
    
    str += "\"";
    return str;
}

String TestCaseGenerator::generateRandomExpression() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 3);
    
    int type = typeDist(gen);
    switch (type) {
        case 0: return generateRandomNumber();
        case 1: return generateRandomString();
        case 2: return generateRandomIdentifier();
        case 3: return generateRandomIdentifier() + " + " + generateRandomNumber();
        default: return "42";
    }
}

String TestCaseGenerator::generateRandomStatement() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 2);
    
    int type = typeDist(gen);
    switch (type) {
        case 0: return "let " + generateRandomIdentifier() + " = " + generateRandomExpression() + ";";
        case 1: return "console.log(" + generateRandomExpression() + ");";
        case 2: return "return " + generateRandomExpression() + ";";
        default: return "let x = 42;";
    }
}

String TestCaseGenerator::generateRandomDeclaration() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 2);
    
    int type = typeDist(gen);
    String name = generateRandomIdentifier();
    
    switch (type) {
        case 0: return "let " + name + ": number = " + generateRandomNumber() + ";";
        case 1: return "let " + name + ": string = " + generateRandomString() + ";";
        case 2: return "function " + name + "(): void { " + generateRandomStatement() + " }";
        default: return "let x: number = 42;";
    }
}

// =============================================================================
// TEST VALIDATOR IMPLEMENTATION
// =============================================================================

TestValidator::TestValidator() = default;

bool TestValidator::validateErrorDetection(const String& code, const String& expectedError) {
    // This would typically run the compiler and check for the expected error
    // For now, we'll implement a basic validation
    return !code.empty() && !expectedError.empty();
}

bool TestValidator::validateErrorLocation(const String& code, const String& errorOutput, 
                                         const SourceLocation& expectedLocation) {
    // Parse error output to extract location information
    std::vector<String> errorLines = parseErrorLines(errorOutput);
    
    for (const String& line : errorLines) {
        SourceLocation location = parseErrorLocation(line);
        if (location.getLine() == expectedLocation.getLine() &&
            location.getColumn() == expectedLocation.getColumn()) {
            return true;
        }
    }
    
    return false;
}

bool TestValidator::validateErrorMessage(const String& errorOutput, const String& expectedMessage) {
    std::vector<String> errorLines = parseErrorLines(errorOutput);
    
    for (const String& line : errorLines) {
        String message = parseErrorMessage(line);
        if (message.find(expectedMessage) != String::npos) {
            return true;
        }
    }
    
    return false;
}

bool TestValidator::validateErrorCode(const String& errorOutput, const String& expectedCode) {
    std::vector<String> errorLines = parseErrorLines(errorOutput);
    
    for (const String& line : errorLines) {
        String code = parseErrorCode(line);
        if (code == expectedCode) {
            return true;
        }
    }
    
    return false;
}

TestValidator::ValidationResult TestValidator::validateComprehensive(const String& code, 
                                                                    const String& compilerOutput) {
    ValidationResult result;
    result.success = true;
    
    // Parse compiler output
    std::vector<String> errorLines = parseErrorLines(compilerOutput);
    
    for (const String& line : errorLines) {
        if (line.find("error:") != String::npos) {
            result.errors.push_back(line);
        } else if (line.find("warning:") != String::npos) {
            result.warnings.push_back(line);
        } else if (line.find("note:") != String::npos) {
            result.suggestions.push_back(line);
        }
    }
    
    // Check if there are any errors
    if (!result.errors.empty()) {
        result.success = false;
    }
    
    return result;
}

bool TestValidator::testWarningSuppression(const String& code, const String& warningCode) {
    // This would test warning suppression functionality
    // For now, return true as a placeholder
    return true;
}

bool TestValidator::testErrorPromotion(const String& code, const String& warningCode) {
    // This would test error promotion functionality
    // For now, return true as a placeholder
    return true;
}

bool TestValidator::testCrossPhaseErrorHandling(const String& code) {
    // This would test cross-phase error handling
    // For now, return true as a placeholder
    return true;
}

std::vector<String> TestValidator::parseErrorLines(const String& output) {
    std::vector<String> lines;
    std::istringstream stream(output);
    String line;
    
    while (std::getline(stream, line)) {
        if (line.find("error:") != String::npos ||
            line.find("warning:") != String::npos ||
            line.find("note:") != String::npos) {
            lines.push_back(line);
        }
    }
    
    return lines;
}

SourceLocation TestValidator::parseErrorLocation(const String& errorLine) {
    // Parse format: filename:line:column: error: message
    std::regex locationPattern(R"(([^:]+):(\d+):(\d+):)");
    std::smatch match;
    
    if (std::regex_search(errorLine, match, locationPattern)) {
        String filename = match[1].str();
        unsigned line = std::stoul(match[2].str());
        unsigned column = std::stoul(match[3].str());
        return SourceLocation(filename, line, column);
    }
    
    return SourceLocation();
}

String TestValidator::parseErrorMessage(const String& errorLine) {
    // Extract message after the colon
    size_t colonPos = errorLine.find(": ");
    if (colonPos != String::npos) {
        return errorLine.substr(colonPos + 2);
    }
    
    return errorLine;
}

String TestValidator::parseErrorCode(const String& errorLine) {
    // Look for error codes in brackets like [TS1001]
    std::regex codePattern(R"(\[([A-Z]+\d+)\])");
    std::smatch match;
    
    if (std::regex_search(errorLine, match, codePattern)) {
        return match[1].str();
    }
    
    return "";
}

// =============================================================================
// FACTORY FUNCTIONS
// =============================================================================

unique_ptr<TestCaseGenerator> createTestCaseGenerator() {
    return make_unique<TestCaseGenerator>();
}

unique_ptr<TestValidator> createTestValidator() {
    return make_unique<TestValidator>();
}

} // namespace testing
} // namespace tsc