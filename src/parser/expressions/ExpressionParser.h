#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"

namespace tsc {
namespace parser {

// Forward declarations
class ParserCore;

// Expression parsing engine - handles all expression types
class ExpressionParser {
public:
    explicit ExpressionParser(ParserCore& parser);
    
    // Main expression parsing interface
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseAssignmentExpression();
    std::unique_ptr<Expression> parseConditionalExpression();
    std::unique_ptr<Expression> parseBinaryExpression(int minPrecedence = 0);
    std::unique_ptr<Expression> parseUnaryExpression();
    std::unique_ptr<Expression> parsePostfixExpression();
    std::unique_ptr<Expression> parsePrimaryExpression();
    
    // Specific expression types
    std::unique_ptr<Expression> parseCallExpression(std::unique_ptr<Expression> callee);
    std::unique_ptr<Expression> parseMemberExpression();
    std::unique_ptr<Expression> parseNewExpression();
    std::unique_ptr<Expression> parseArrayLiteral();
    std::unique_ptr<Expression> parseObjectLiteral();
    std::unique_ptr<Expression> parseTemplateLiteral();
    std::unique_ptr<Expression> parseArrowFunction();
    std::unique_ptr<Expression> parseFunctionExpression();
    
    // Literal parsing
    std::unique_ptr<Expression> parseNumericLiteral();
    std::unique_ptr<Expression> parseStringLiteral();
    std::unique_ptr<Expression> parseBooleanLiteral();
    std::unique_ptr<Expression> parseNullLiteral();
    std::unique_ptr<Expression> parseIdentifier();
    
    // Complex expressions
    std::unique_ptr<Expression> parseDestructuringPattern();
    std::unique_ptr<Expression> parseArrayDestructuringPattern();
    std::unique_ptr<Expression> parseObjectDestructuringPattern();
    std::unique_ptr<Expression> parseSpreadElement();
    std::unique_ptr<Expression> parseOptionalExpression();

private:
    ParserCore& parser_;
    
    // Helper methods
    std::unique_ptr<Expression> parseLeftAssociativeBinary(std::unique_ptr<Expression> left, int minPrecedence);
    std::unique_ptr<Expression> parseRightAssociativeBinary(std::unique_ptr<Expression> left, int minPrecedence);
    std::vector<std::unique_ptr<Expression>> parseArgumentList();
    std::vector<std::unique_ptr<Expression>> parseExpressionList();
    std::vector<std::unique_ptr<ObjectLiteral::Property>> parseObjectProperties();
};

} // namespace parser
} // namespace tsc