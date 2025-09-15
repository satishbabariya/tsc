#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <regex>

// Simple demonstration of the Code Transformation Framework concepts
// This is a simplified version to show how the framework works

class SimpleTransformationRule {
public:
    virtual ~SimpleTransformationRule() = default;
    virtual std::string getName() const = 0;
    virtual std::string apply(const std::string& code) const = 0;
};

class RemoveSemicolonRule : public SimpleTransformationRule {
public:
    std::string getName() const override { return "RemoveSemicolon"; }
    std::string apply(const std::string& code) const override {
        std::string result = code;
        // Remove the first semicolon found
        size_t pos = result.find(';');
        if (pos != std::string::npos) {
            result.erase(pos, 1);
        }
        return result;
    }
};

class RemoveBraceRule : public SimpleTransformationRule {
public:
    std::string getName() const override { return "RemoveBrace"; }
    std::string apply(const std::string& code) const override {
        std::string result = code;
        // Remove the first brace found
        size_t pos = result.find('{');
        if (pos == std::string::npos) {
            pos = result.find('}');
        }
        if (pos != std::string::npos) {
            result.erase(pos, 1);
        }
        return result;
    }
};

class AddTypeMismatchRule : public SimpleTransformationRule {
public:
    std::string getName() const override { return "AddTypeMismatch"; }
    std::string apply(const std::string& code) const override {
        std::string result = code;
        // Change number assignments to string assignments
        std::regex assignmentPattern(R"((\w+)\s*=\s*(\d+))");
        result = std::regex_replace(result, assignmentPattern, "$1 = \"$2\"");
        return result;
    }
};

class AddInvalidTokenRule : public SimpleTransformationRule {
public:
    std::string getName() const override { return "AddInvalidToken"; }
    std::string apply(const std::string& code) const override {
        std::string result = code;
        // Add invalid tokens randomly
        std::vector<std::string> invalidTokens = {"@#$", "###", "$$$"};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, invalidTokens.size() - 1);
        
        std::string invalidToken = invalidTokens[dis(gen)];
        result.insert(result.length() / 2, invalidToken);
        return result;
    }
};

class SimpleCodeTransformer {
private:
    std::vector<std::unique_ptr<SimpleTransformationRule>> rules_;
    std::mt19937 rng_;

public:
    SimpleCodeTransformer() : rng_(std::random_device{}()) {
        // Register default rules
        rules_.push_back(std::make_unique<RemoveSemicolonRule>());
        rules_.push_back(std::make_unique<RemoveBraceRule>());
        rules_.push_back(std::make_unique<AddTypeMismatchRule>());
        rules_.push_back(std::make_unique<AddInvalidTokenRule>());
    }
    
    std::string transform(const std::string& code) {
        std::string result = code;
        std::vector<std::string> appliedTransformations;
        
        std::uniform_real_distribution<double> probDist(0.0, 1.0);
        std::uniform_int_distribution<size_t> ruleDist(0, rules_.size() - 1);
        
        // Apply 1-3 random transformations
        int numTransformations = 1 + (std::rand() % 3);
        
        for (int i = 0; i < numTransformations; ++i) {
            if (probDist(rng_) > 0.7) { // 30% chance to apply transformation
                size_t ruleIndex = ruleDist(rng_);
                auto& rule = rules_[ruleIndex];
                
                std::string newCode = rule->apply(result);
                if (newCode != result) {
                    result = newCode;
                    appliedTransformations.push_back(rule->getName());
                }
            }
        }
        
        std::cout << "Applied transformations: ";
        for (const auto& transformation : appliedTransformations) {
            std::cout << transformation << " ";
        }
        std::cout << std::endl;
        
        return result;
    }
};

class SimpleTestCaseGenerator {
private:
    SimpleCodeTransformer transformer_;

public:
    std::string generateSyntaxErrorTest() {
        std::string baseCode = R"(
function test(): void {
    let x: number = 42;
    let y: string = "hello";
    console.log(x);
    console.log(y);
}
)";
        
        return transformer_.transform(baseCode);
    }
    
    std::string generateTypeErrorTest() {
        std::string baseCode = R"(
function test(): void {
    let num: number = 42;
    let str: string = "hello";
    let bool: boolean = true;
    
    let result1 = num + str;
    let result2 = str + bool;
    let result3 = num + bool;
}
)";
        
        return transformer_.transform(baseCode);
    }
    
    std::string generateSemanticErrorTest() {
        std::string baseCode = R"(
function test(): void {
    let declaredVariable: number = 42;
    let anotherVariable: string = "test";
    
    console.log(declaredVariable);
    console.log(anotherVariable);
    console.log(undeclaredVariable);
}
)";
        
        return transformer_.transform(baseCode);
    }
};

void demonstrateTransformationFramework() {
    std::cout << "=== Code Transformation Framework Demonstration ===" << std::endl;
    
    SimpleTestCaseGenerator generator;
    SimpleCodeTransformer transformer;
    
    // Test 1: Syntax Error Generation
    std::cout << "\n--- Test 1: Syntax Error Generation ---" << std::endl;
    std::string syntaxTest = generator.generateSyntaxErrorTest();
    std::cout << "Generated syntax error test:" << std::endl;
    std::cout << syntaxTest << std::endl;
    
    // Test 2: Type Error Generation
    std::cout << "\n--- Test 2: Type Error Generation ---" << std::endl;
    std::string typeTest = generator.generateTypeErrorTest();
    std::cout << "Generated type error test:" << std::endl;
    std::cout << typeTest << std::endl;
    
    // Test 3: Semantic Error Generation
    std::cout << "\n--- Test 3: Semantic Error Generation ---" << std::endl;
    std::string semanticTest = generator.generateSemanticErrorTest();
    std::cout << "Generated semantic error test:" << std::endl;
    std::cout << semanticTest << std::endl;
    
    // Test 4: Multiple Transformations
    std::cout << "\n--- Test 4: Multiple Transformations ---" << std::endl;
    std::string baseCode = R"(
function complexTest(): void {
    let x: number = 42;
    let y: string = "hello";
    let z: boolean = true;
    
    if (x > 10) {
        console.log(y);
    } else {
        console.log(z);
    }
    
    for (let i = 0; i < 10; i++) {
        console.log(i);
    }
}
)";
    
    std::cout << "Original code:" << std::endl;
    std::cout << baseCode << std::endl;
    
    std::string transformedCode = transformer.transform(baseCode);
    std::cout << "Transformed code:" << std::endl;
    std::cout << transformedCode << std::endl;
    
    // Test 5: Stress Testing
    std::cout << "\n--- Test 5: Stress Testing ---" << std::endl;
    std::string stressCode = R"(
function stressTest(): void {
    let array = new Array(1000);
    for (let i = 0; i < 1000; i++) {
        array[i] = i * 2;
    }
    
    let sum = 0;
    for (let i = 0; i < 1000; i++) {
        sum += array[i];
    }
    
    console.log("Sum:", sum);
}
)";
    
    std::string stressTransformed = transformer.transform(stressCode);
    std::cout << "Stress test transformed:" << std::endl;
    std::cout << stressTransformed << std::endl;
    
    std::cout << "\n=== Demonstration Complete ===" << std::endl;
}

int main() {
    try {
        demonstrateTransformationFramework();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}