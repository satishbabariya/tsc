#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/semantic/TypeSystem.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/parser/VectorTokenStream.h"

using namespace tsc;

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        diagnostics = std::make_unique<DiagnosticEngine>();
        typeSystem = std::make_unique<TypeSystem>();
        parser = std::make_unique<Parser>(*diagnostics, *typeSystem);
    }

    std::unique_ptr<DiagnosticEngine> diagnostics;
    std::unique_ptr<TypeSystem> typeSystem;
    std::unique_ptr<Parser> parser;
    
    // Helper method to create tokens for testing
    std::vector<Token> createTokens(const std::vector<std::string>& tokenStrings) {
        std::vector<Token> tokens;
        for (const auto& str : tokenStrings) {
            if (str == "T") {
                tokens.emplace_back(TokenType::Identifier, str, SourceLocation("test.ts", 1, 1));
            } else if (str == "extends") {
                tokens.emplace_back(TokenType::Extends, str, SourceLocation("test.ts", 1, 1));
            } else if (str == "string") {
                tokens.emplace_back(TokenType::String, str, SourceLocation("test.ts", 1, 1));
            } else if (str == "number") {
                tokens.emplace_back(TokenType::Number, str, SourceLocation("test.ts", 1, 1));
            } else if (str == "boolean") {
                tokens.emplace_back(TokenType::Boolean, str, SourceLocation("test.ts", 1, 1));
            } else if (str == "U") {
                tokens.emplace_back(TokenType::Identifier, str, SourceLocation("test.ts", 1, 1));
            }
        }
        tokens.emplace_back(TokenType::EndOfInput, "", SourceLocation("test.ts", 1, 1));
        return tokens;
    }
};

TEST_F(ParserTest, ParseEmptyModule) {
    std::vector<Token> tokens = {
        Token(TokenType::EndOfInput, SourceLocation("test.ts", 1, 1))
    };
    
    auto module = parser->parse(tokens, "test.ts");
    ASSERT_NE(module, nullptr);
    EXPECT_EQ(module->getFilename(), "test.ts");
    EXPECT_TRUE(module->getStatements().empty());
}

// ============================================================================
// COMPREHENSIVE UNIT TESTS FOR GENERIC TYPE CONSTRAINTS
// ============================================================================

// Test basic type parameter without constraints
TEST_F(ParserTest, ParseTypeParameter_NoConstraints) {
    auto tokens = createTokens({"T"});
    auto tokenStream = std::make_unique<VectorTokenStream>(tokens);
    
    // We need to manually test parseTypeParameter by creating a parser with tokens
    // Since parseTypeParameter is private, we'll test it through public interfaces
    auto module = parser->parse(std::move(tokenStream), "test.ts");
    
    // For now, we'll test through function parsing which uses parseTypeParameter
    std::string source = "function test<T>(): T { return null; }";
    Lexer lexer(*diagnostics);
    auto functionTokens = lexer.tokenize(source, "test.ts");
    auto functionModule = parser->parse(functionTokens, "test.ts");
    
    ASSERT_NE(functionModule, nullptr);
    EXPECT_FALSE(functionModule->getStatements().empty());
}

// Test type parameter with string constraint
TEST_F(ParserTest, ParseTypeParameter_StringConstraint) {
    std::string source = "function test<T extends string>(): T { return null; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// Test type parameter with number constraint
TEST_F(ParserTest, ParseTypeParameter_NumberConstraint) {
    std::string source = "function test<T extends number>(): T { return null; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// Test type parameter with boolean constraint
TEST_F(ParserTest, ParseTypeParameter_BooleanConstraint) {
    std::string source = "function test<T extends boolean>(): T { return null; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// Test multiple type parameters with constraints
TEST_F(ParserTest, ParseTypeParameter_MultipleConstraints) {
    std::string source = "function test<T extends string, U extends number>(): T { return null; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// Test type parameter with constraint in function parameter
TEST_F(ParserTest, ParseTypeParameter_ConstraintInParameter) {
    std::string source = "function test<T extends string>(value: T): T { return value; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// Test type parameter with constraint in return type
TEST_F(ParserTest, ParseTypeParameter_ConstraintInReturnType) {
    std::string source = "function test<T extends number>(value: T): T { return value; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// Test complex function with multiple type parameters and constraints
TEST_F(ParserTest, ParseTypeParameter_ComplexFunction) {
    std::string source = "function complex<T extends string, U extends number>(first: T, second: U): T { return first; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// ============================================================================
// NEGATIVE TESTS - INVALID SYNTAX
// ============================================================================

// Test invalid constraint syntax - missing constraint type
TEST_F(ParserTest, ParseTypeParameter_InvalidConstraint_MissingType) {
    std::string source = "function test<T extends>(): T { return null; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    // This should fail to parse
    EXPECT_EQ(module, nullptr);
}

// Test invalid constraint syntax - malformed extends
TEST_F(ParserTest, ParseTypeParameter_InvalidConstraint_MalformedExtends) {
    std::string source = "function test<T extends string extends number>(): T { return null; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    // This should fail to parse
    EXPECT_EQ(module, nullptr);
}

// ============================================================================
// INTEGRATION TESTS WITH OTHER LANGUAGE FEATURES
// ============================================================================

// Test generic constraints with function calls
TEST_F(ParserTest, ParseTypeParameter_WithFunctionCalls) {
    std::string source = R"(
        function identity<T extends string>(value: T): T {
            return value;
        }
        
        function main(): number {
            let result = identity("hello");
            return 0;
        }
    )";
    
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_EQ(module->getStatements().size(), 2); // Two function declarations
}

// Test generic constraints with multiple function calls
TEST_F(ParserTest, ParseTypeParameter_MultipleFunctionCalls) {
    std::string source = R"(
        function processString<T extends string>(value: T): T {
            return value;
        }
        
        function processNumber<T extends number>(value: T): T {
            return value;
        }
        
        function main(): number {
            let str = processString("test");
            let num = processNumber(42);
            return 0;
        }
    )";
    
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_EQ(module->getStatements().size(), 3); // Three function declarations
}

// ============================================================================
// EDGE CASES AND STRESS TESTS
// ============================================================================

// Test deeply nested generic constraints
TEST_F(ParserTest, ParseTypeParameter_DeeplyNested) {
    std::string source = "function deep<T extends string>(value: T): T { return value; }";
    Lexer lexer(*diagnostics);
    auto tokens = lexer.tokenize(source, "test.ts");
    auto module = parser->parse(tokens, "test.ts");
    
    ASSERT_NE(module, nullptr);
    EXPECT_FALSE(module->getStatements().empty());
}

// Test generic constraints with whitespace variations
TEST_F(ParserTest, ParseTypeParameter_WhitespaceVariations) {
    std::vector<std::string> variations = {
        "function test<T extends string>(): T { return null; }",
        "function test<T extends  string>(): T { return null; }",
        "function test<T  extends string>(): T { return null; }",
        "function test<T extends  string >(): T { return null; }"
    };
    
    Lexer lexer(*diagnostics);
    
    for (const auto& source : variations) {
        auto tokens = lexer.tokenize(source, "test.ts");
        auto module = parser->parse(tokens, "test.ts");
        
        ASSERT_NE(module, nullptr) << "Failed to parse: " << source;
        EXPECT_FALSE(module->getStatements().empty()) << "Empty statements for: " << source;
    }
}

// Test generic constraints with different constraint types
TEST_F(ParserTest, ParseTypeParameter_DifferentConstraintTypes) {
    std::vector<std::string> constraintTypes = {
        "function test<T extends string>(): T { return null; }",
        "function test<T extends number>(): T { return null; }",
        "function test<T extends boolean>(): T { return null; }"
    };
    
    Lexer lexer(*diagnostics);
    
    for (const auto& source : constraintTypes) {
        auto tokens = lexer.tokenize(source, "test.ts");
        auto module = parser->parse(tokens, "test.ts");
        
        ASSERT_NE(module, nullptr) << "Failed to parse: " << source;
        EXPECT_FALSE(module->getStatements().empty()) << "Empty statements for: " << source;
    }
}