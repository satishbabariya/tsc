#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"

namespace tsc {
namespace lexer {

// Forward declarations
class LexerCore;
class CharacterClassifier;

// Token scanning engine - handles different token types
class TokenScanner {
public:
    explicit TokenScanner(LexerCore& lexer);
    
    // Main scanning interface
    Token scanToken();
    
    // Specific token scanners
    Token scanNumber();
    Token scanString(char quote);
    Token scanTemplate();
    Token scanTemplateTail();
    Token scanRegularExpression();
    Token scanIdentifier();
    Token scanPrivateIdentifier();
    Token scanSingleLineComment();
    Token scanMultiLineComment();
    
    // Operator and punctuation scanning
    Token scanOperator();
    Token scanPunctuation();
    
    // Helper methods
    Token scanHexNumber();
    Token scanOctalNumber();
    Token scanBinaryNumber();
    Token scanDecimalNumber();
    Token scanFloatNumber();
    Token scanExponent();

private:
    LexerCore& lexer_;
    CharacterClassifier* characterClassifier_;
    
    // Helper methods
    bool match(char expected);
    String getCurrentLexeme();
    void consumeDigits();
    bool consumeHexDigits();
    bool consumeOctalDigits();
    bool consumeBinaryDigits();
};

} // namespace lexer
} // namespace tsc