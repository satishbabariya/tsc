#include <gtest/gtest.h>
#include "tsc/lexer/Lexer.h"
#include "tsc/utils/DiagnosticEngine.h"

using namespace tsc;

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {
        diagnostics = std::make_unique<DiagnosticEngine>();
        lexer = std::make_unique<Lexer>(*diagnostics);
    }

    std::unique_ptr<DiagnosticEngine> diagnostics;
    std::unique_ptr<Lexer> lexer;
};

TEST_F(LexerTest, TokenizeKeywords) {
    String source = "function let const var";
    auto tokens = lexer->tokenize(source, "test.ts");
    
    ASSERT_EQ(tokens.size(), 5); // 4 keywords + EOF
    EXPECT_EQ(tokens[0].getType(), TokenType::Function);
    EXPECT_EQ(tokens[1].getType(), TokenType::Let);
    EXPECT_EQ(tokens[2].getType(), TokenType::Const);
    EXPECT_EQ(tokens[3].getType(), TokenType::Var);
    EXPECT_EQ(tokens[4].getType(), TokenType::EndOfInput);
}

TEST_F(LexerTest, TokenizeIdentifiers) {
    String source = "myVariable _private $special";
    auto tokens = lexer->tokenize(source, "test.ts");
    
    ASSERT_EQ(tokens.size(), 4); // 3 identifiers + EOF
    EXPECT_EQ(tokens[0].getType(), TokenType::Identifier);
    EXPECT_EQ(tokens[0].getStringValue(), "myVariable");
    EXPECT_EQ(tokens[1].getType(), TokenType::Identifier);
    EXPECT_EQ(tokens[1].getStringValue(), "_private");
    EXPECT_EQ(tokens[2].getType(), TokenType::Identifier);
    EXPECT_EQ(tokens[2].getStringValue(), "$special");
}

TEST_F(LexerTest, TokenizeNumbers) {
    String source = "42 3.14 0x1A 0b1010 0o777";
    auto tokens = lexer->tokenize(source, "test.ts");
    
    ASSERT_EQ(tokens.size(), 6); // 5 numbers + EOF
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(tokens[i].getType(), TokenType::NumericLiteral);
    }
}

TEST_F(LexerTest, TokenizeStrings) {
    String source = R"("hello" 'world' `template`)";
    auto tokens = lexer->tokenize(source, "test.ts");
    
    ASSERT_EQ(tokens.size(), 4); // 3 strings + EOF
    EXPECT_EQ(tokens[0].getType(), TokenType::StringLiteral);
    EXPECT_EQ(tokens[1].getType(), TokenType::StringLiteral);
    EXPECT_EQ(tokens[2].getType(), TokenType::NoSubstitutionTemplate);
}

TEST_F(LexerTest, TokenizePunctuation) {
    String source = "(){};,.:";
    auto tokens = lexer->tokenize(source, "test.ts");
    
    ASSERT_EQ(tokens.size(), 9); // 8 punctuation + EOF
    EXPECT_EQ(tokens[0].getType(), TokenType::LeftParen);
    EXPECT_EQ(tokens[1].getType(), TokenType::RightParen);
    EXPECT_EQ(tokens[2].getType(), TokenType::LeftBrace);
    EXPECT_EQ(tokens[3].getType(), TokenType::RightBrace);
    EXPECT_EQ(tokens[4].getType(), TokenType::Semicolon);
    EXPECT_EQ(tokens[5].getType(), TokenType::Comma);
    EXPECT_EQ(tokens[6].getType(), TokenType::Dot);
    EXPECT_EQ(tokens[7].getType(), TokenType::Colon);
}