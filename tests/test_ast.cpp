#include <gtest/gtest.h>
#include "tsc/AST.h"

using namespace tsc;

TEST(ASTTest, NumericLiteral) {
    SourceLocation loc("test.ts", 1, 1);
    NumericLiteral literal(42.0, loc);
    
    EXPECT_EQ(literal.getValue(), 42.0);
    EXPECT_EQ(literal.getLocation().getFilename(), "test.ts");
    EXPECT_TRUE(literal.isConstant());
    EXPECT_EQ(literal.getCategory(), Expression::Category::RValue);
}

TEST(ASTTest, StringLiteral) {
    SourceLocation loc("test.ts", 1, 1);
    StringLiteral literal("hello", loc);
    
    EXPECT_EQ(literal.getValue(), "hello");
    EXPECT_TRUE(literal.isConstant());
    EXPECT_EQ(literal.toString(), "\"hello\"");
}

TEST(ASTTest, BooleanLiteral) {
    SourceLocation loc("test.ts", 1, 1);
    BooleanLiteral literal(true, loc);
    
    EXPECT_TRUE(literal.getValue());
    EXPECT_TRUE(literal.isConstant());
    EXPECT_EQ(literal.toString(), "true");
}

TEST(ASTTest, Identifier) {
    SourceLocation loc("test.ts", 1, 1);
    Identifier id("myVar", loc);
    
    EXPECT_EQ(id.getName(), "myVar");
    EXPECT_EQ(id.getCategory(), Expression::Category::LValue);
    EXPECT_FALSE(id.isConstant());
}