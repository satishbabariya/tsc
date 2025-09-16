#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"

#include <unordered_map>

namespace tsc {
namespace parser {

// Grammar rules and precedence management
class GrammarRules {
public:
    GrammarRules();
    
    // Operator precedence
    int getOperatorPrecedence(TokenType type) const;
    bool isBinaryOperator(TokenType type) const;
    bool isUnaryOperator(TokenType type) const;
    bool isAssignmentOperator(TokenType type) const;
    bool isLeftAssociative(TokenType type) const;
    bool isRightAssociative(TokenType type) const;
    
    // Statement keywords
    bool isStatementKeyword(TokenType type) const;
    bool isDeclarationKeyword(TokenType type) const;
    bool isExpressionKeyword(TokenType type) const;
    
    // Type keywords
    bool isTypeKeyword(TokenType type) const;
    bool isPrimitiveTypeKeyword(TokenType type) const;
    bool isModifierKeyword(TokenType type) const;
    
    // Context-sensitive keywords
    bool isContextualKeyword(TokenType type) const;
    TokenType getContextualKeywordType(const String& text) const;
    
    // Grammar validation
    bool isValidStatementStart(TokenType type) const;
    bool isValidExpressionStart(TokenType type) const;
    bool isValidDeclarationStart(TokenType type) const;
    bool isValidTypeStart(TokenType type) const;

private:
    // Operator precedence table
    std::unordered_map<TokenType, int> operatorPrecedence_;
    
    // Keyword sets
    std::unordered_set<TokenType> statementKeywords_;
    std::unordered_set<TokenType> declarationKeywords_;
    std::unordered_set<TokenType> expressionKeywords_;
    std::unordered_set<TokenType> typeKeywords_;
    std::unordered_set<TokenType> primitiveTypeKeywords_;
    std::unordered_set<TokenType> modifierKeywords_;
    std::unordered_set<TokenType> contextualKeywords_;
    
    // Initialization
    void initializeOperatorPrecedence();
    void initializeKeywordSets();
};

} // namespace parser
} // namespace tsc