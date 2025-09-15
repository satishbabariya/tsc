#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <regex>

// Simple test case generator for TSC compiler testing
class TestCaseGenerator {
private:
    std::mt19937 rng_;

public:
    TestCaseGenerator() : rng_(std::random_device{}()) {}
    
    std::string generateSyntaxErrorTest() {
        return R"(
// Syntax Error Test Case
function syntaxErrorTest(): void {
    let x: number = 42
    let y: string = "hello"
    let z: boolean = true
    
    if (x > 10) {
        console.log(y)
    } else {
        console.log(z)
    }
    
    for (let i = 0; i < 10; i++) {
        console.log(i)
    }
    
    let array = [1, 2, 3
    let obj = { name: "test", value: 42
}
)";
    }
    
    std::string generateTypeErrorTest() {
        return R"(
// Type Error Test Case
function typeErrorTest(): void {
    let num: number = 42;
    let str: string = "hello";
    let bool: boolean = true;
    
    // Type mismatches
    let result1 = num + str;
    let result2 = str + bool;
    let result3 = num + bool;
    
    // Assignment type mismatches
    let numberVar: number = "string";
    let stringVar: string = 42;
    let booleanVar: boolean = "true";
    
    // Function parameter type mismatches
    function processNumber(n: number): number {
        return n * 2;
    }
    
    let result4 = processNumber(str);
    let result5 = processNumber(bool);
    
    // Array type mismatches
    let numberArray: number[] = ["1", "2", "3"];
    let stringArray: string[] = [1, 2, 3];
}
)";
    }
    
    std::string generateSemanticErrorTest() {
        return R"(
// Semantic Error Test Case
function semanticErrorTest(): void {
    let declaredVariable: number = 42;
    let anotherVariable: string = "test";
    
    // Undefined variable references
    console.log(declaredVariable);
    console.log(anotherVariable);
    console.log(undeclaredVariable);
    console.log(anotherUndeclaredVariable);
    
    // Duplicate declarations
    let duplicateVar: number = 10;
    let duplicateVar: string = "duplicate";
    
    // Function before declaration
    let result = useBeforeDeclared();
    
    function useBeforeDeclared(): number {
        return 42;
    }
    
    // Class member access errors
    class TestClass {
        private privateMember: number = 42;
        public publicMember: string = "public";
    }
    
    let instance = new TestClass();
    let privateAccess = instance.privateMember;
    let publicAccess = instance.publicMember;
}
)";
    }
    
    std::string generateControlFlowErrorTest() {
        return R"(
// Control Flow Error Test Case
function controlFlowErrorTest(): void {
    // Malformed if statements
    if (true) {
        console.log("valid if");
    } else {
        console.log("valid else");
    }
    
    if (true) {
        console.log("missing closing brace");
    
    // Malformed loops
    for (let i = 0; i < 10; i++) {
        console.log(i);
    
    while (true) {
        console.log("infinite loop");
    
    // Malformed switch statements
    let value = 1;
    switch (value) {
        case 1:
            console.log("one");
        case 2:
            console.log("two");
        default:
            console.log("default");
    }
    
    // Break/continue outside loops
    function invalidBreak(): void {
        break;
    }
    
    function invalidContinue(): void {
        continue;
    }
}
)";
    }
    
    std::string generateNestingStressTest() {
        return R"(
// Deep Nesting Stress Test
function nestingStressTest(): void {
    let level1 = 1;
    if (level1 > 0) {
        let level2 = 2;
        if (level2 > 1) {
            let level3 = 3;
            if (level3 > 2) {
                let level4 = 4;
                if (level4 > 3) {
                    let level5 = 5;
                    if (level5 > 4) {
                        let level6 = 6;
                        if (level6 > 5) {
                            let level7 = 7;
                            if (level7 > 6) {
                                let level8 = 8;
                                if (level8 > 7) {
                                    let level9 = 9;
                                    if (level9 > 8) {
                                        let level10 = 10;
                                        console.log("Deep nesting level 10");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Nested loops
    for (let i = 0; i < 10; i++) {
        for (let j = 0; j < 10; j++) {
            for (let k = 0; k < 10; k++) {
                for (let l = 0; l < 10; l++) {
                    for (let m = 0; m < 10; m++) {
                        console.log(i, j, k, l, m);
                    }
                }
            }
        }
    }
}
)";
    }
    
    std::string generateLongExpressionTest() {
        return R"(
// Long Expression Test
function longExpressionTest(): void {
    let veryLongExpression = ((1 + 2) * (3 - 4)) / ((5 + 6) * (7 - 8)) + 
                            ((9 + 10) * (11 - 12)) / ((13 + 14) * (15 - 16)) +
                            ((17 + 18) * (19 - 20)) / ((21 + 22) * (23 - 24)) +
                            ((25 + 26) * (27 - 28)) / ((29 + 30) * (31 - 32)) +
                            ((33 + 34) * (35 - 36)) / ((37 + 38) * (39 - 40)) +
                            ((41 + 42) * (43 - 44)) / ((45 + 46) * (47 - 48)) +
                            ((49 + 50) * (51 - 52)) / ((53 + 54) * (55 - 56)) +
                            ((57 + 58) * (59 - 60)) / ((61 + 62) * (63 - 64)) +
                            ((65 + 66) * (67 - 68)) / ((69 + 70) * (71 - 72)) +
                            ((73 + 74) * (75 - 76)) / ((77 + 78) * (79 - 80));
    
    console.log("Long expression result:", veryLongExpression);
}
)";
    }
    
    std::string generateLargeNumberTest() {
        return R"(
// Large Number Test
function largeNumberTest(): void {
    let hugeNumber = 9223372036854775807;
    let anotherHugeNumber = 18446744073709551615;
    let maxSafeInteger = 9007199254740991;
    let minSafeInteger = -9007199254740991;
    
    let infinity = Infinity;
    let negativeInfinity = -Infinity;
    let nan = NaN;
    
    let result1 = hugeNumber + anotherHugeNumber;
    let result2 = maxSafeInteger * 2;
    let result3 = minSafeInteger - 1;
    
    console.log("Huge number:", hugeNumber);
    console.log("Another huge number:", anotherHugeNumber);
    console.log("Max safe integer:", maxSafeInteger);
    console.log("Min safe integer:", minSafeInteger);
    console.log("Infinity:", infinity);
    console.log("Negative infinity:", negativeInfinity);
    console.log("NaN:", nan);
}
)";
    }
    
    std::string generateUnusualTextTest() {
        return R"(
// Unusual Text Test
function unusualTextTest(): void {
    let emojiString = "🚀🌟💫⭐✨🔥💥⚡🌈🎉🎊🎈🎁🎀🎂🍰🎪🎭🎨🎬🎵🎶🎸🎹🎺🎻🎼🎽🎾🎿🏀🏁🏂🏃🏄🏅🏆🏇🏈🏉🏊🏋🏌🏍🏎🏏🏐🏑🏒🏓🏔🏕🏖🏗🏘🏙🏚🏛🏜🏝🏞🏟🏠🏡🏢🏣🏤🏥🏦🏧🏨🏩🏪🏫🏬🏭🏮🏯🏰🏱🏲🏳🏴🏵🏶🏷🏸🏹🏺🏻🏼🏽🏾🏿";
    
    let unicodeString = "αβγδεζηθικλμνξοπρστυφχψωΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ";
    
    let specialChars = "!@#$%^&*()_+-=[]{}|;':\",./<>?`~";
    
    let mixedString = "Hello 世界 🌍 123 !@#";
    
    console.log("Emoji string:", emojiString);
    console.log("Unicode string:", unicodeString);
    console.log("Special chars:", specialChars);
    console.log("Mixed string:", mixedString);
}
)";
    }
    
    void generateAllTestCases() {
        std::vector<std::pair<std::string, std::string>> testCases = {
            {"syntax_error_test.ts", generateSyntaxErrorTest()},
            {"type_error_test.ts", generateTypeErrorTest()},
            {"semantic_error_test.ts", generateSemanticErrorTest()},
            {"control_flow_error_test.ts", generateControlFlowErrorTest()},
            {"nesting_stress_test.ts", generateNestingStressTest()},
            {"long_expression_test.ts", generateLongExpressionTest()},
            {"large_number_test.ts", generateLargeNumberTest()},
            {"unusual_text_test.ts", generateUnusualTextTest()}
        };
        
        for (const auto& testCase : testCases) {
            std::ofstream file(testCase.first);
            if (file.is_open()) {
                file << testCase.second;
                file.close();
                std::cout << "Generated test case: " << testCase.first << std::endl;
            } else {
                std::cerr << "Failed to create file: " << testCase.first << std::endl;
            }
        }
    }
};

int main() {
    try {
        TestCaseGenerator generator;
        generator.generateAllTestCases();
        std::cout << "\nAll test cases generated successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}