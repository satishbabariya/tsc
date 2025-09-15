#include "tsc/testing/CodeTransformationFramework.h"
#include <random>
#include <sstream>
#include <algorithm>
#include <regex>

namespace tsc {
namespace testing {

// =============================================================================
// TRANSFORMATION RULE IMPLEMENTATIONS
// =============================================================================

// RemoveSemicolonRule implementation
bool RemoveSemicolonRule::canApply(const String& code, const SourceLocation& location) const {
    // Check if there's a semicolon near the location
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        // Look for semicolons in the current line
        size_t lineStart = code.find_last_of('\n', pos);
        if (lineStart == String::npos) lineStart = 0;
        else lineStart++;
        
        size_t lineEnd = code.find('\n', pos);
        if (lineEnd == String::npos) lineEnd = code.length();
        
        String line = code.substr(lineStart, lineEnd - lineStart);
        return line.find(';') != String::npos;
    }
    return false;
}

String RemoveSemicolonRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    // Find the first semicolon in the current line
    size_t pos = location.getColumn() - 1;
    size_t lineStart = result.find_last_of('\n', pos);
    if (lineStart == String::npos) lineStart = 0;
    else lineStart++;
    
    size_t lineEnd = result.find('\n', pos);
    if (lineEnd == String::npos) lineEnd = result.length();
    
    String line = result.substr(lineStart, lineEnd - lineStart);
    size_t semicolonPos = line.find(';');
    
    if (semicolonPos != String::npos) {
        size_t globalPos = lineStart + semicolonPos;
        result.erase(globalPos, 1);
    }
    
    return result;
}

// RemoveBraceRule implementation
bool RemoveBraceRule::canApply(const String& code, const SourceLocation& location) const {
    // Look for braces near the location
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        // Check for opening or closing braces
        size_t searchStart = (pos > 100) ? pos - 100 : 0;
        size_t searchEnd = std::min(pos + 100, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find('{') != String::npos || searchArea.find('}') != String::npos;
    }
    return false;
}

String RemoveBraceRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    // Find the nearest brace
    size_t pos = location.getColumn() - 1;
    size_t searchStart = (pos > 100) ? pos - 100 : 0;
    size_t searchEnd = std::min(pos + 100, result.length());
    
    String searchArea = result.substr(searchStart, searchEnd - searchStart);
    size_t bracePos = searchArea.find('{');
    if (bracePos == String::npos) {
        bracePos = searchArea.find('}');
    }
    
    if (bracePos != String::npos) {
        size_t globalPos = searchStart + bracePos;
        result.erase(globalPos, 1);
    }
    
    return result;
}

// RemoveParenthesisRule implementation
bool RemoveParenthesisRule::canApply(const String& code, const SourceLocation& location) const {
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        size_t searchStart = (pos > 50) ? pos - 50 : 0;
        size_t searchEnd = std::min(pos + 50, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find('(') != String::npos || searchArea.find(')') != String::npos;
    }
    return false;
}

String RemoveParenthesisRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    size_t pos = location.getColumn() - 1;
    size_t searchStart = (pos > 50) ? pos - 50 : 0;
    size_t searchEnd = std::min(pos + 50, result.length());
    
    String searchArea = result.substr(searchStart, searchEnd - searchStart);
    size_t parenPos = searchArea.find('(');
    if (parenPos == String::npos) {
        parenPos = searchArea.find(')');
    }
    
    if (parenPos != String::npos) {
        size_t globalPos = searchStart + parenPos;
        result.erase(globalPos, 1);
    }
    
    return result;
}

// RemoveBracketRule implementation
bool RemoveBracketRule::canApply(const String& code, const SourceLocation& location) const {
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        size_t searchStart = (pos > 50) ? pos - 50 : 0;
        size_t searchEnd = std::min(pos + 50, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find('[') != String::npos || searchArea.find(']') != String::npos;
    }
    return false;
}

String RemoveBracketRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    size_t pos = location.getColumn() - 1;
    size_t searchStart = (pos > 50) ? pos - 50 : 0;
    size_t searchEnd = std::min(pos + 50, result.length());
    
    String searchArea = result.substr(searchStart, searchEnd - searchStart);
    size_t bracketPos = searchArea.find('[');
    if (bracketPos == String::npos) {
        bracketPos = searchArea.find(']');
    }
    
    if (bracketPos != String::npos) {
        size_t globalPos = searchStart + bracketPos;
        result.erase(globalPos, 1);
    }
    
    return result;
}

// AddInvalidTokenRule implementation
bool AddInvalidTokenRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add invalid tokens
}

String AddInvalidTokenRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Add invalid tokens
    std::vector<String> invalidTokens = {"@#$", "###", "$$$", "%%%", "^^^", "~~~"};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, invalidTokens.size() - 1);
    
    String invalidToken = invalidTokens[dis(gen)];
    result.insert(pos, invalidToken);
    
    return result;
}

// ChangeTypeAnnotationRule implementation
bool ChangeTypeAnnotationRule::canApply(const String& code, const SourceLocation& location) const {
    // Look for type annotations
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        size_t searchStart = (pos > 100) ? pos - 100 : 0;
        size_t searchEnd = std::min(pos + 100, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find(": ") != String::npos;
    }
    return false;
}

String ChangeTypeAnnotationRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    // Find type annotations and change them
    std::regex typePattern(R"(\:\s*(number|string|boolean|any|object|Array<[^>]+>))");
    std::smatch match;
    
    if (std::regex_search(result, match, typePattern)) {
        std::vector<String> incompatibleTypes = {"string", "number", "boolean", "object", "any"};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, incompatibleTypes.size() - 1);
        
        String newType = incompatibleTypes[dis(gen)];
        result = std::regex_replace(result, typePattern, ": " + newType);
    }
    
    return result;
}

// AddTypeMismatchRule implementation
bool AddTypeMismatchRule::canApply(const String& code, const SourceLocation& location) const {
    // Look for assignment statements
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        size_t searchStart = (pos > 100) ? pos - 100 : 0;
        size_t searchEnd = std::min(pos + 100, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find(" = ") != String::npos;
    }
    return false;
}

String AddTypeMismatchRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    // Find assignments and create type mismatches
    std::regex assignmentPattern(R"((\w+)\s*=\s*(\d+))");
    std::smatch match;
    
    if (std::regex_search(result, match, assignmentPattern)) {
        String varName = match[1].str();
        String value = match[2].str();
        
        // Change number to string
        String newValue = "\"" + value + "\"";
        result = std::regex_replace(result, assignmentPattern, varName + " = " + newValue);
    }
    
    return result;
}

// RenameVariableRule implementation
bool RenameVariableRule::canApply(const String& code, const SourceLocation& location) const {
    // Look for variable declarations
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        size_t searchStart = (pos > 100) ? pos - 100 : 0;
        size_t searchEnd = std::min(pos + 100, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find("let ") != String::npos || 
               searchArea.find("const ") != String::npos ||
               searchArea.find("var ") != String::npos;
    }
    return false;
}

String RenameVariableRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    // Find variable declarations and rename them
    std::regex varPattern(R"((let|const|var)\s+(\w+))");
    std::smatch match;
    
    if (std::regex_search(result, match, varPattern)) {
        String declaration = match[1].str();
        String varName = match[2].str();
        
        // Generate a new name
        String newName = varName + "_renamed_" + std::to_string(std::rand() % 1000);
        
        // Replace the declaration
        String oldDeclaration = declaration + " " + varName;
        String newDeclaration = declaration + " " + newName;
        result = std::regex_replace(result, std::regex(oldDeclaration), newDeclaration);
        
        // Don't replace all occurrences to create undefined references
    }
    
    return result;
}

// DeepNestingRule implementation
bool DeepNestingRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add nesting
}

String DeepNestingRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Add deeply nested structures
    String nestedCode = R"(
    if (true) {
        if (true) {
            if (true) {
                if (true) {
                    if (true) {
                        if (true) {
                            if (true) {
                                if (true) {
                                    if (true) {
                                        if (true) {
                                            console.log("deep nesting");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
)";
    
    result.insert(pos, nestedCode);
    return result;
}

// LongExpressionsRule implementation
bool LongExpressionsRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add long expressions
}

String LongExpressionsRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Create a very long expression
    String longExpression = "let veryLongExpression = ";
    for (int i = 0; i < 100; ++i) {
        longExpression += "(" + std::to_string(i) + " + ";
    }
    longExpression += "0";
    for (int i = 0; i < 100; ++i) {
        longExpression += ")";
    }
    longExpression += ";";
    
    result.insert(pos, longExpression + "\n");
    return result;
}

// ComplexExpressionsRule implementation
bool ComplexExpressionsRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add complex expressions
}

String ComplexExpressionsRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Create a complex nested expression
    String complexExpression = R"(
    let complexResult = ((a + b) * (c - d)) / ((e + f) * (g - h)) + 
                       ((i + j) * (k - l)) / ((m + n) * (o - p)) +
                       ((q + r) * (s - t)) / ((u + v) * (w - x)) +
                       ((y + z) * (aa - bb)) / ((cc + dd) * (ee - ff));
)";
    
    result.insert(pos, complexExpression);
    return result;
}

// LargeNumbersRule implementation
bool LargeNumbersRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add large numbers
}

String LargeNumbersRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Add very large numbers
    String largeNumber = "let hugeNumber = " + std::to_string(LLONG_MAX) + ";\n";
    result.insert(pos, largeNumber);
    return result;
}

// LongIdentifiersRule implementation
bool LongIdentifiersRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add long identifiers
}

String LongIdentifiersRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Create a very long identifier
    String longIdentifier = "let ";
    for (int i = 0; i < 1000; ++i) {
        longIdentifier += "veryLongIdentifierNamePart" + std::to_string(i);
    }
    longIdentifier += " = 42;\n";
    
    result.insert(pos, longIdentifier);
    return result;
}

// UnusualTextRule implementation
bool UnusualTextRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add unusual text
}

String UnusualTextRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Add unusual text and characters
    String unusualText = R"(
    let unusualString = "🚀🌟💫⭐✨🔥💥⚡🌈🎉🎊🎈🎁🎀🎂🍰🎪🎭🎨🎬🎵🎶🎸🎹🎺🎻🎼🎽🎾🎿🏀🏁🏂🏃🏄🏅🏆🏇🏈🏉🏊🏋🏌🏍🏎🏏🏐🏑🏒🏓🏔🏕🏖🏗🏘🏙🏚🏛🏜🏝🏞🏟🏠🏡🏢🏣🏤🏥🏦🏧🏨🏩🏪🏫🏬🏭🏮🏯🏰🏱🏲🏳🏴🏵🏶🏷🏸🏹🏺🏻🏼🏽🏾🏿";
    let unicodeText = "αβγδεζηθικλμνξοπρστυφχψω";
    let specialChars = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
)";
    
    result.insert(pos, unusualText);
    return result;
}

// MalformControlFlowRule implementation
bool MalformControlFlowRule::canApply(const String& code, const SourceLocation& location) const {
    // Look for control flow statements
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        size_t searchStart = (pos > 100) ? pos - 100 : 0;
        size_t searchEnd = std::min(pos + 100, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find("if ") != String::npos ||
               searchArea.find("while ") != String::npos ||
               searchArea.find("for ") != String::npos ||
               searchArea.find("switch ") != String::npos;
    }
    return false;
}

String MalformControlFlowRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    // Malform control flow statements
    std::regex ifPattern(R"(if\s*\()");
    std::regex whilePattern(R"(while\s*\()");
    std::regex forPattern(R"(for\s*\()");
    
    result = std::regex_replace(result, ifPattern, "if (");
    result = std::regex_replace(result, whilePattern, "while (");
    result = std::regex_replace(result, forPattern, "for (");
    
    // Remove some parentheses to create syntax errors
    size_t parenPos = result.find("(");
    if (parenPos != String::npos) {
        result.erase(parenPos, 1);
    }
    
    return result;
}

// AddUnreachableCodeRule implementation
bool AddUnreachableCodeRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add unreachable code
}

String AddUnreachableCodeRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Add unreachable code after return statements
    String unreachableCode = R"(
    function hasUnreachableCode(): number {
        return 42;
        console.log("This will never execute");
        let unreachable = 100;
        return unreachable;
    }
)";
    
    result.insert(pos, unreachableCode);
    return result;
}

// CreateInfiniteLoopRule implementation
bool CreateInfiniteLoopRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add infinite loops
}

String CreateInfiniteLoopRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Create infinite loops
    String infiniteLoop = R"(
    function infiniteLoop(): void {
        while (true) {
            console.log("infinite loop");
        }
    }
    
    function recursiveInfinite(): void {
        recursiveInfinite();
    }
)";
    
    result.insert(pos, infiniteLoop);
    return result;
}

// MalformSwitchCaseRule implementation
bool MalformSwitchCaseRule::canApply(const String& code, const SourceLocation& location) const {
    // Look for switch statements
    size_t pos = location.getColumn() - 1;
    if (pos < code.length()) {
        size_t searchStart = (pos > 100) ? pos - 100 : 0;
        size_t searchEnd = std::min(pos + 100, code.length());
        
        String searchArea = code.substr(searchStart, searchEnd - searchStart);
        return searchArea.find("switch ") != String::npos;
    }
    return false;
}

String MalformSwitchCaseRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    
    // Malform switch case statements
    std::regex casePattern(R"(case\s+(\d+)\s*:)");
    result = std::regex_replace(result, casePattern, "case $1");
    
    std::regex defaultPattern(R"(default\s*:)");
    result = std::regex_replace(result, defaultPattern, "default");
    
    return result;
}

// MemoryStressRule implementation
bool MemoryStressRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add memory stress
}

String MemoryStressRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Create memory stress conditions
    String memoryStress = R"(
    function memoryStress(): void {
        let largeArray = new Array(1000000);
        let anotherArray = new Array(1000000);
        let yetAnotherArray = new Array(1000000);
        
        for (let i = 0; i < 1000000; i++) {
            largeArray[i] = new Array(1000);
            anotherArray[i] = new Array(1000);
            yetAnotherArray[i] = new Array(1000);
        }
    }
)";
    
    result.insert(pos, memoryStress);
    return result;
}

// PerformanceStressRule implementation
bool PerformanceStressRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add performance stress
}

String PerformanceStressRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Create performance stress conditions
    String performanceStress = R"(
    function performanceStress(): void {
        for (let i = 0; i < 1000000; i++) {
            for (let j = 0; j < 1000; j++) {
                for (let k = 0; k < 100; k++) {
                    let result = i * j * k;
                    let complex = Math.sqrt(result) + Math.sin(result) + Math.cos(result);
                    let moreComplex = Math.pow(complex, 2) + Math.log(complex);
                }
            }
        }
    }
)";
    
    result.insert(pos, performanceStress);
    return result;
}

// EdgeCaseGenerationRule implementation
bool EdgeCaseGenerationRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add edge cases
}

String EdgeCaseGenerationRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Generate edge cases
    String edgeCases = R"(
    let edgeCases = {
        zero: 0,
        negativeZero: -0,
        infinity: Infinity,
        negativeInfinity: -Infinity,
        nan: NaN,
        maxSafeInteger: Number.MAX_SAFE_INTEGER,
        minSafeInteger: Number.MIN_SAFE_INTEGER,
        maxValue: Number.MAX_VALUE,
        minValue: Number.MIN_VALUE,
        epsilon: Number.EPSILON
    };
)";
    
    result.insert(pos, edgeCases);
    return result;
}

// BoundaryTestingRule implementation
bool BoundaryTestingRule::canApply(const String& code, const SourceLocation& location) const {
    return true; // Can always add boundary tests
}

String BoundaryTestingRule::apply(const String& code, const SourceLocation& location) const {
    String result = code;
    size_t pos = location.getColumn() - 1;
    
    // Add boundary testing code
    String boundaryTests = R"(
    function boundaryTests(): void {
        let array = [1, 2, 3];
        let index = array.length; // Out of bounds
        let value = array[index];
        
        let division = 42 / 0; // Division by zero
        let modulo = 42 % 0; // Modulo by zero
        
        let nullValue: any = null;
        let property = nullValue.property; // Null dereference
    }
)";
    
    result.insert(pos, boundaryTests);
    return result;
}

// =============================================================================
// CODE TRANSFORMER IMPLEMENTATION
// =============================================================================

CodeTransformer::CodeTransformer() : rng_(std::random_device{}()) {
    registerDefaultRules();
}

void CodeTransformer::registerRule(unique_ptr<TransformationRule> rule) {
    rules_.push_back(std::move(rule));
}

void CodeTransformer::registerDefaultRules() {
    rules_.clear();
    
    // Register all transformation rules
    rules_.push_back(make_unique<RemoveSemicolonRule>());
    rules_.push_back(make_unique<RemoveBraceRule>());
    rules_.push_back(make_unique<RemoveParenthesisRule>());
    rules_.push_back(make_unique<RemoveBracketRule>());
    rules_.push_back(make_unique<AddInvalidTokenRule>());
    rules_.push_back(make_unique<ChangeTypeAnnotationRule>());
    rules_.push_back(make_unique<AddTypeMismatchRule>());
    rules_.push_back(make_unique<RenameVariableRule>());
    rules_.push_back(make_unique<DeepNestingRule>());
    rules_.push_back(make_unique<LongExpressionsRule>());
    rules_.push_back(make_unique<ComplexExpressionsRule>());
    rules_.push_back(make_unique<LargeNumbersRule>());
    rules_.push_back(make_unique<LongIdentifiersRule>());
    rules_.push_back(make_unique<UnusualTextRule>());
    rules_.push_back(make_unique<MalformControlFlowRule>());
    rules_.push_back(make_unique<AddUnreachableCodeRule>());
    rules_.push_back(make_unique<CreateInfiniteLoopRule>());
    rules_.push_back(make_unique<MalformSwitchCaseRule>());
    rules_.push_back(make_unique<MemoryStressRule>());
    rules_.push_back(make_unique<PerformanceStressRule>());
    rules_.push_back(make_unique<EdgeCaseGenerationRule>());
    rules_.push_back(make_unique<BoundaryTestingRule>());
}

void CodeTransformer::setRandomSeed(unsigned int seed) {
    rng_.seed(seed);
}

void CodeTransformer::setMaxTransformations(size_t max) {
    maxTransformations_ = max;
}

void CodeTransformer::setTransformationProbability(double probability) {
    transformationProbability_ = probability;
}

TransformationResult CodeTransformer::transform(const String& code, const String& filename) {
    TransformationResult result;
    result.originalCode = code;
    result.transformedCode = code;
    result.success = true;
    
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    std::uniform_int_distribution<size_t> ruleDist(0, rules_.size() - 1);
    
    size_t transformations = 0;
    size_t maxAttempts = maxTransformations_ * 3; // Allow some failed attempts
    
    for (size_t attempt = 0; attempt < maxAttempts && transformations < maxTransformations_; ++attempt) {
        if (probDist(rng_) > transformationProbability_) {
            continue;
        }
        
        size_t ruleIndex = ruleDist(rng_);
        auto& rule = rules_[ruleIndex];
        
        if (shouldApplyTransformation(*rule)) {
            SourceLocation location = findRandomLocation(result.transformedCode);
            
            if (rule->canApply(result.transformedCode, location)) {
                String newCode = rule->apply(result.transformedCode, location);
                if (newCode != result.transformedCode) {
                    result.transformedCode = newCode;
                    result.appliedTransformations.push_back(rule->getName());
                    transformations++;
                }
            }
        }
    }
    
    result.transformationCount = transformations;
    return result;
}

TransformationResult CodeTransformer::transformWithRules(const String& code, 
                                                       const std::vector<TransformationType>& types,
                                                       const String& filename) {
    TransformationResult result;
    result.originalCode = code;
    result.transformedCode = code;
    result.success = true;
    
    // Filter rules by requested types
    std::vector<TransformationRule*> applicableRules;
    for (auto& rule : rules_) {
        for (auto type : types) {
            if (rule->getType() == type) {
                applicableRules.push_back(rule.get());
                break;
            }
        }
    }
    
    if (applicableRules.empty()) {
        result.errors.push_back("No applicable transformation rules found");
        return result;
    }
    
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    std::uniform_int_distribution<size_t> ruleDist(0, applicableRules.size() - 1);
    
    size_t transformations = 0;
    size_t maxAttempts = maxTransformations_ * 3;
    
    for (size_t attempt = 0; attempt < maxAttempts && transformations < maxTransformations_; ++attempt) {
        if (probDist(rng_) > transformationProbability_) {
            continue;
        }
        
        size_t ruleIndex = ruleDist(rng_);
        auto* rule = applicableRules[ruleIndex];
        
        if (shouldApplyTransformation(*rule)) {
            SourceLocation location = findRandomLocation(result.transformedCode);
            
            if (rule->canApply(result.transformedCode, location)) {
                String newCode = rule->apply(result.transformedCode, location);
                if (newCode != result.transformedCode) {
                    result.transformedCode = newCode;
                    result.appliedTransformations.push_back(rule->getName());
                    transformations++;
                }
            }
        }
    }
    
    result.transformationCount = transformations;
    return result;
}

std::vector<String> CodeTransformer::getAvailableRules() const {
    std::vector<String> ruleNames;
    for (const auto& rule : rules_) {
        ruleNames.push_back(rule->getName());
    }
    return ruleNames;
}

std::vector<String> CodeTransformer::getRuleDescriptions() const {
    std::vector<String> descriptions;
    for (const auto& rule : rules_) {
        descriptions.push_back(rule->getName() + ": " + rule->getDescription());
    }
    return descriptions;
}

void CodeTransformer::clearRules() {
    rules_.clear();
}

bool CodeTransformer::shouldApplyTransformation(const TransformationRule& rule) const {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng_) <= rule.getConfig().probability;
}

SourceLocation CodeTransformer::findRandomLocation(const String& code) const {
    if (code.empty()) {
        return SourceLocation("", 1, 1);
    }
    
    std::uniform_int_distribution<size_t> posDist(0, code.length() - 1);
    size_t pos = posDist(rng_);
    
    // Calculate line and column
    size_t line = 1;
    size_t column = 1;
    
    for (size_t i = 0; i < pos; ++i) {
        if (code[i] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
    
    return SourceLocation("", line, column);
}

std::vector<SourceLocation> CodeTransformer::findValidLocations(const String& code, 
                                                               const TransformationRule& rule) const {
    std::vector<SourceLocation> locations;
    
    // Simple approach: check every 100 characters
    for (size_t i = 0; i < code.length(); i += 100) {
        SourceLocation loc("", 1, i + 1);
        if (rule.canApply(code, loc)) {
            locations.push_back(loc);
        }
    }
    
    return locations;
}

// =============================================================================
// FACTORY FUNCTIONS
// =============================================================================

unique_ptr<CodeTransformer> createCodeTransformer() {
    return make_unique<CodeTransformer>();
}

} // namespace testing
} // namespace tsc