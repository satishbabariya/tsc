#include "tsc/parser/grammar/GrammarRules.h"

namespace tsc {
namespace parser {

GrammarRules::GrammarRules() {
    initializeOperatorPrecedence();
    initializeKeywordSets();
}

int GrammarRules::getOperatorPrecedence(TokenType type) const {
    auto it = operatorPrecedence_.find(type);
    return it != operatorPrecedence_.end() ? it->second : 0;
}

bool GrammarRules::isBinaryOperator(TokenType type) const {
    return operatorPrecedence_.find(type) != operatorPrecedence_.end();
}

bool GrammarRules::isUnaryOperator(TokenType type) const {
    return type == TokenType::Plus || type == TokenType::Minus || 
           type == TokenType::Bang || type == TokenType::Tilde ||
           type == TokenType::PlusPlus || type == TokenType::MinusMinus;
}

bool GrammarRules::isAssignmentOperator(TokenType type) const {
    return type == TokenType::Equal || type == TokenType::PlusEqual ||
           type == TokenType::MinusEqual || type == TokenType::StarEqual ||
           type == TokenType::SlashEqual || type == TokenType::PercentEqual;
}

bool GrammarRules::isLeftAssociative(TokenType type) const {
    // Most operators are left associative
    return type != TokenType::StarStar && type != TokenType::Equal;
}

bool GrammarRules::isRightAssociative(TokenType type) const {
    return type == TokenType::StarStar || type == TokenType::Equal;
}

bool GrammarRules::isStatementKeyword(TokenType type) const {
    return statementKeywords_.find(type) != statementKeywords_.end();
}

bool GrammarRules::isDeclarationKeyword(TokenType type) const {
    return declarationKeywords_.find(type) != declarationKeywords_.end();
}

bool GrammarRules::isExpressionKeyword(TokenType type) const {
    return expressionKeywords_.find(type) != expressionKeywords_.end();
}

bool GrammarRules::isTypeKeyword(TokenType type) const {
    return typeKeywords_.find(type) != typeKeywords_.end();
}

bool GrammarRules::isPrimitiveTypeKeyword(TokenType type) const {
    return primitiveTypeKeywords_.find(type) != primitiveTypeKeywords_.end();
}

bool GrammarRules::isModifierKeyword(TokenType type) const {
    return modifierKeywords_.find(type) != modifierKeywords_.end();
}

bool GrammarRules::isContextualKeyword(TokenType type) const {
    return contextualKeywords_.find(type) != contextualKeywords_.end();
}

TokenType GrammarRules::getContextualKeywordType(const String& text) const {
    // TODO: Implement contextual keyword type lookup
    return TokenType::Identifier;
}

bool GrammarRules::isValidStatementStart(TokenType type) const {
    return isStatementKeyword(type) || isDeclarationKeyword(type) || 
           type == TokenType::LeftBrace || type == TokenType::Semicolon;
}

bool GrammarRules::isValidExpressionStart(TokenType type) const {
    return type == TokenType::Number || type == TokenType::String ||
           type == TokenType::True || type == TokenType::False ||
           type == TokenType::Null || type == TokenType::Identifier ||
           type == TokenType::LeftParen || type == TokenType::LeftBracket ||
           type == TokenType::LeftBrace || type == TokenType::This ||
           type == TokenType::Super || type == TokenType::New ||
           type == TokenType::Function || type == TokenType::Class ||
           isUnaryOperator(type);
}

bool GrammarRules::isValidDeclarationStart(TokenType type) const {
    return isDeclarationKeyword(type);
}

bool GrammarRules::isValidTypeStart(TokenType type) const {
    return isTypeKeyword(type) || type == TokenType::LeftParen ||
           type == TokenType::LeftBracket || type == TokenType::LeftBrace;
}

void GrammarRules::initializeOperatorPrecedence() {
    operatorPrecedence_ = {
        {TokenType::PipePipe, 3},
        {TokenType::QuestionQuestion, 4},
        {TokenType::AmpersandAmpersand, 4},
        {TokenType::EqualEqual, 8},
        {TokenType::NotEqual, 8},
        {TokenType::Less, 9},
        {TokenType::Greater, 9},
        {TokenType::LessEqual, 9},
        {TokenType::GreaterEqual, 9},
        {TokenType::Plus, 11},
        {TokenType::Minus, 11},
        {TokenType::Star, 12},
        {TokenType::Slash, 12},
        {TokenType::Percent, 12},
        {TokenType::StarStar, 13}
    };
}

void GrammarRules::initializeKeywordSets() {
    statementKeywords_ = {
        TokenType::If, TokenType::While, TokenType::Do, TokenType::For,
        TokenType::Switch, TokenType::Break, TokenType::Continue,
        TokenType::Return, TokenType::Try, TokenType::Throw,
        TokenType::With, TokenType::Debugger
    };
    
    declarationKeywords_ = {
        TokenType::Var, TokenType::Let, TokenType::Const,
        TokenType::Function, TokenType::Class, TokenType::Interface,
        TokenType::Enum, TokenType::Type, TokenType::Import, TokenType::Export
    };
    
    expressionKeywords_ = {
        TokenType::True, TokenType::False, TokenType::Null,
        TokenType::This, TokenType::Super, TokenType::New
    };
    
    typeKeywords_ = {
        TokenType::Any, TokenType::Unknown, TokenType::Boolean,
        TokenType::Number, TokenType::String, TokenType::Symbol,
        TokenType::Bigint, TokenType::Undefined, TokenType::Never,
        TokenType::Object, TokenType::Void
    };
    
    primitiveTypeKeywords_ = {
        TokenType::Boolean, TokenType::Number, TokenType::String,
        TokenType::Symbol, TokenType::Bigint
    };
    
    modifierKeywords_ = {
        TokenType::Public, TokenType::Private, TokenType::Protected,
        TokenType::Static, TokenType::Readonly, TokenType::Abstract,
        TokenType::Override, TokenType::Accessor
    };
    
    contextualKeywords_ = {
        TokenType::As, TokenType::Is, TokenType::Satisfies,
        TokenType::Assert, TokenType::Asserts
    };
}

} // namespace parser
} // namespace tsc