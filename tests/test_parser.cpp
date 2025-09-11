#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "tsc/utils/DiagnosticEngine.h"

using namespace tsc;

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        diagnostics = std::make_unique<DiagnosticEngine>();
        parser = std::make_unique<Parser>(*diagnostics);
    }

    std::unique_ptr<DiagnosticEngine> diagnostics;
    std::unique_ptr<Parser> parser;
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

// More parser tests will be added as the parser is implemented