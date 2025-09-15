#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <random>

namespace tsc {
namespace testing {

// Forward declarations
class TransformationRule;
class CodeTransformer;
class TestCaseGenerator;

// Transformation types for different error injection strategies
enum class TransformationType {
    // Syntax transformations
    RemoveSemicolon,
    RemoveBrace,
    RemoveParenthesis,
    RemoveBracket,
    AddInvalidToken,
    MalformExpression,
    MalformStatement,
    MalformDeclaration,
    
    // Type transformations
    ChangeTypeAnnotation,
    AddTypeMismatch,
    RemoveTypeAnnotation,
    AddInvalidType,
    CreateCircularType,
    
    // Semantic transformations
    RenameVariable,
    RemoveDeclaration,
    AddDuplicateIdentifier,
    CreateUndefinedReference,
    MalformImport,
    
    // Structural transformations
    DeepNesting,
    LongExpressions,
    ComplexExpressions,
    LargeNumbers,
    LongIdentifiers,
    UnusualText,
    
    // Control flow transformations
    MalformControlFlow,
    AddUnreachableCode,
    CreateInfiniteLoop,
    MalformSwitchCase,
    
    // Advanced transformations
    MemoryStress,
    PerformanceStress,
    EdgeCaseGeneration,
    BoundaryTesting
};

// Configuration for transformation rules
struct TransformationConfig {
    double probability = 1.0;           // Probability of applying this transformation
    int minOccurrences = 1;             // Minimum number of times to apply
    int maxOccurrences = 1;             // Maximum number of times to apply
    bool allowMultiple = false;         // Allow multiple applications
    std::vector<String> excludePatterns; // Patterns to exclude from transformation
    std::vector<String> includePatterns; // Patterns to include for transformation
};

// Result of a transformation operation
struct TransformationResult {
    bool success = false;
    String originalCode;
    String transformedCode;
    std::vector<String> appliedTransformations;
    std::vector<String> errors;
    size_t transformationCount = 0;
};

// Base class for transformation rules
class TransformationRule {
public:
    virtual ~TransformationRule() = default;
    
    virtual String getName() const = 0;
    virtual TransformationType getType() const = 0;
    virtual bool canApply(const String& code, const SourceLocation& location) const = 0;
    virtual String apply(const String& code, const SourceLocation& location) const = 0;
    virtual String getDescription() const = 0;
    
    // Configuration
    void setConfig(const TransformationConfig& config) { config_ = config; }
    const TransformationConfig& getConfig() const { return config_; }
    
protected:
    TransformationConfig config_;
};

// Specific transformation rules
class RemoveSemicolonRule : public TransformationRule {
public:
    String getName() const override { return "RemoveSemicolon"; }
    TransformationType getType() const override { return TransformationType::RemoveSemicolon; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Removes semicolons from statements"; }
};

class RemoveBraceRule : public TransformationRule {
public:
    String getName() const override { return "RemoveBrace"; }
    TransformationType getType() const override { return TransformationType::RemoveBrace; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Removes braces from blocks"; }
};

class RemoveParenthesisRule : public TransformationRule {
public:
    String getName() const override { return "RemoveParenthesis"; }
    TransformationType getType() const override { return TransformationType::RemoveParenthesis; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Removes parentheses from expressions"; }
};

class RemoveBracketRule : public TransformationRule {
public:
    String getName() const override { return "RemoveBracket"; }
    TransformationType getType() const override { return TransformationType::RemoveBracket; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Removes brackets from arrays"; }
};

class AddInvalidTokenRule : public TransformationRule {
public:
    String getName() const override { return "AddInvalidToken"; }
    TransformationType getType() const override { return TransformationType::AddInvalidToken; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Adds invalid tokens to code"; }
};

class ChangeTypeAnnotationRule : public TransformationRule {
public:
    String getName() const override { return "ChangeTypeAnnotation"; }
    TransformationType getType() const override { return TransformationType::ChangeTypeAnnotation; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Changes type annotations to incompatible types"; }
};

class AddTypeMismatchRule : public TransformationRule {
public:
    String getName() const override { return "AddTypeMismatch"; }
    TransformationType getType() const override { return TransformationType::AddTypeMismatch; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates type mismatches in assignments"; }
};

class RenameVariableRule : public TransformationRule {
public:
    String getName() const override { return "RenameVariable"; }
    TransformationType getType() const override { return TransformationType::RenameVariable; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Renames variables to create undefined references"; }
};

class DeepNestingRule : public TransformationRule {
public:
    String getName() const override { return "DeepNesting"; }
    TransformationType getType() const override { return TransformationType::DeepNesting; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates deeply nested code structures"; }
};

class LongExpressionsRule : public TransformationRule {
public:
    String getName() const override { return "LongExpressions"; }
    TransformationType getType() const override { return TransformationType::LongExpressions; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates very long expressions"; }
};

class ComplexExpressionsRule : public TransformationRule {
public:
    String getName() const override { return "ComplexExpressions"; }
    TransformationType getType() const override { return TransformationType::ComplexExpressions; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates complex nested expressions"; }
};

class LargeNumbersRule : public TransformationRule {
public:
    String getName() const override { return "LargeNumbers"; }
    TransformationType getType() const override { return TransformationType::LargeNumbers; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Uses very large numbers"; }
};

class LongIdentifiersRule : public TransformationRule {
public:
    String getName() const override { return "LongIdentifiers"; }
    TransformationType getType() const override { return TransformationType::LongIdentifiers; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates very long identifier names"; }
};

class UnusualTextRule : public TransformationRule {
public:
    String getName() const override { return "UnusualText"; }
    TransformationType getType() const override { return TransformationType::UnusualText; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Uses unusual text and characters"; }
};

class MalformControlFlowRule : public TransformationRule {
public:
    String getName() const override { return "MalformControlFlow"; }
    TransformationType getType() const override { return TransformationType::MalformControlFlow; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Malforms control flow constructs"; }
};

class AddUnreachableCodeRule : public TransformationRule {
public:
    String getName() const override { return "AddUnreachableCode"; }
    TransformationType getType() const override { return TransformationType::AddUnreachableCode; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Adds unreachable code"; }
};

class CreateInfiniteLoopRule : public TransformationRule {
public:
    String getName() const override { return "CreateInfiniteLoop"; }
    TransformationType getType() const override { return TransformationType::CreateInfiniteLoop; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates infinite loops"; }
};

class MalformSwitchCaseRule : public TransformationRule {
public:
    String getName() const override { return "MalformSwitchCase"; }
    TransformationType getType() const override { return TransformationType::MalformSwitchCase; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Malforms switch case statements"; }
};

class MemoryStressRule : public TransformationRule {
public:
    String getName() const override { return "MemoryStress"; }
    TransformationType getType() const override { return TransformationType::MemoryStress; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates memory stress conditions"; }
};

class PerformanceStressRule : public TransformationRule {
public:
    String getName() const override { return "PerformanceStress"; }
    TransformationType getType() const override { return TransformationType::PerformanceStress; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Creates performance stress conditions"; }
};

class EdgeCaseGenerationRule : public TransformationRule {
public:
    String getName() const override { return "EdgeCaseGeneration"; }
    TransformationType getType() const override { return TransformationType::EdgeCaseGeneration; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Generates edge cases"; }
};

class BoundaryTestingRule : public TransformationRule {
public:
    String getName() const override { return "BoundaryTesting"; }
    TransformationType getType() const override { return TransformationType::BoundaryTesting; }
    bool canApply(const String& code, const SourceLocation& location) const override;
    String apply(const String& code, const SourceLocation& location) const override;
    String getDescription() const override { return "Tests boundary conditions"; }
};

// Main code transformer class
class CodeTransformer {
public:
    CodeTransformer();
    ~CodeTransformer() = default;
    
    // Registration of transformation rules
    void registerRule(unique_ptr<TransformationRule> rule);
    void registerDefaultRules();
    
    // Configuration
    void setRandomSeed(unsigned int seed);
    void setMaxTransformations(size_t max);
    void setTransformationProbability(double probability);
    
    // Transformation execution
    TransformationResult transform(const String& code, const String& filename = "");
    TransformationResult transformWithRules(const String& code, 
                                          const std::vector<TransformationType>& types,
                                          const String& filename = "");
    
    // Utility methods
    std::vector<String> getAvailableRules() const;
    std::vector<String> getRuleDescriptions() const;
    void clearRules();
    
private:
    std::vector<unique_ptr<TransformationRule>> rules_;
    std::mt19937 rng_;
    size_t maxTransformations_ = 10;
    double transformationProbability_ = 0.8;
    
    // Helper methods
    bool shouldApplyTransformation(const TransformationRule& rule) const;
    SourceLocation findRandomLocation(const String& code) const;
    std::vector<SourceLocation> findValidLocations(const String& code, 
                                                  const TransformationRule& rule) const;
};

// Test case generator for automated testing
class TestCaseGenerator {
public:
    TestCaseGenerator();
    ~TestCaseGenerator() = default;
    
    // Test case generation
    String generateValidCode(const String& template = "");
    String generateErrorTestCase(TransformationType errorType, const String& baseCode = "");
    String generateStressTestCase(const std::vector<TransformationType>& stressTypes, 
                                 const String& baseCode = "");
    
    // Specific test case generators
    String generateSyntaxErrorTest();
    String generateTypeErrorTest();
    String generateSemanticErrorTest();
    String generateControlFlowTest();
    String generateNestingTest();
    String generatePerformanceTest();
    String generateMemoryTest();
    String generateEdgeCaseTest();
    
    // Batch generation
    std::vector<String> generateTestSuite(size_t count, 
                                         const std::vector<TransformationType>& types);
    std::vector<String> generateComprehensiveTestSuite();
    
    // Configuration
    void setComplexityLevel(int level); // 1-10 scale
    void setMaxNestingDepth(int depth);
    void setMaxExpressionLength(int length);
    void setMaxIdentifierLength(int length);
    
private:
    unique_ptr<CodeTransformer> transformer_;
    int complexityLevel_ = 5;
    int maxNestingDepth_ = 10;
    int maxExpressionLength_ = 1000;
    int maxIdentifierLength_ = 100;
    
    // Helper methods
    String generateRandomIdentifier();
    String generateRandomNumber();
    String generateRandomString();
    String generateRandomExpression();
    String generateRandomStatement();
    String generateRandomDeclaration();
};

// Test validation and verification
class TestValidator {
public:
    TestValidator();
    ~TestValidator() = default;
    
    // Validation methods
    bool validateErrorDetection(const String& code, const String& expectedError);
    bool validateErrorLocation(const String& code, const String& errorOutput, 
                              const SourceLocation& expectedLocation);
    bool validateErrorMessage(const String& errorOutput, const String& expectedMessage);
    bool validateErrorCode(const String& errorOutput, const String& expectedCode);
    
    // Comprehensive validation
    struct ValidationResult {
        bool success = false;
        std::vector<String> errors;
        std::vector<String> warnings;
        std::vector<String> suggestions;
    };
    
    ValidationResult validateComprehensive(const String& code, 
                                          const String& compilerOutput);
    
    // Error suppression and promotion testing
    bool testWarningSuppression(const String& code, const String& warningCode);
    bool testErrorPromotion(const String& code, const String& warningCode);
    bool testCrossPhaseErrorHandling(const String& code);
    
private:
    // Helper methods for parsing compiler output
    std::vector<String> parseErrorLines(const String& output);
    SourceLocation parseErrorLocation(const String& errorLine);
    String parseErrorMessage(const String& errorLine);
    String parseErrorCode(const String& errorLine);
};

// Factory functions
unique_ptr<CodeTransformer> createCodeTransformer();
unique_ptr<TestCaseGenerator> createTestCaseGenerator();
unique_ptr<TestValidator> createTestValidator();

} // namespace testing
} // namespace tsc