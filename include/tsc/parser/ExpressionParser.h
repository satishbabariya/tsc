#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"

namespace tsc {

class Parser;
class DiagnosticEngine;

// Specialized parser for expressions
class ExpressionParser {
public:
    explicit ExpressionParser(Parser& parentParser, DiagnosticEngine& diagnostics);
    ~ExpressionParser() = default;
    
    // Expression parsing methods
    unique_ptr<Expression> parseExpression();
    unique_ptr<Expression> parseAssignmentExpression();
    unique_ptr<Expression> parseConditionalExpression();
    unique_ptr<Expression> parseBinaryExpression(int minPrecedence = 0);
    unique_ptr<Expression> parseUnaryExpression();
    unique_ptr<Expression> parsePostfixExpression();
    unique_ptr<Expression> parsePrimaryExpression();
    
    // Literal parsing
    unique_ptr<Expression> parseNumericLiteral();
    unique_ptr<Expression> parseStringLiteral();
    unique_ptr<Expression> parseBooleanLiteral();
    unique_ptr<Expression> parseNullLiteral();
    unique_ptr<Expression> parseArrayLiteral();
    unique_ptr<Expression> parseObjectLiteral();
    unique_ptr<Expression> parseTemplateLiteral();
    
    // Function expressions
    unique_ptr<Expression> parseFunctionExpression();
    unique_ptr<Expression> parseArrowFunction();
    unique_ptr<Expression> parseNewExpression();
    
    // Member access and calls
    unique_ptr<Expression> parseCallExpression(unique_ptr<Expression> callee);
    unique_ptr<Expression> parseMemberExpression(unique_ptr<Expression> object);
    
    // Arrow function detection
    bool looksLikeArrowFunction();
    
private:
    Parser& parentParser_;
    DiagnosticEngine& diagnostics_;
    
    // Helper methods
    int getBinaryOperatorPrecedence(TokenType type) const;
    BinaryExpression::Operator tokenToBinaryOperator(TokenType type) const;
    UnaryExpression::Operator tokenToUnaryOperator(TokenType type) const;
    AssignmentExpression::Operator tokenToAssignmentOperator(TokenType type) const;
    
    bool isExpressionStart(TokenType type) const;
    bool isAssignmentOperator(TokenType type) const;
    bool isBinaryOperator(TokenType type) const;
    bool isUnaryOperator(TokenType type) const;
};

} // namespace tsc