#pragma once

#include "tsc/Common.h"

namespace tsc {
namespace lexer {

// Character classification utilities
class CharacterClassifier {
public:
    CharacterClassifier() = default;
    
    // Basic character classification
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isHexDigit(char c) const;
    bool isOctalDigit(char c) const;
    bool isBinaryDigit(char c) const;
    
    // Identifier classification
    bool isIdentifierStart(char c) const;
    bool isIdentifierPart(char c) const;
    
    // Whitespace and line terminators
    bool isWhitespace(char c) const;
    bool isLineTerminator(char c) const;
    
    // Unicode support
    bool isUnicodeIdentifierStart(char c) const;
    bool isUnicodeIdentifierPart(char c) const;
    bool isUnicodeEscapeSequence(const String& text, size_t pos) const;
    
    // String and template literal support
    bool isStringEscapeChar(char c) const;
    bool isTemplateEscapeChar(char c) const;
    
    // Number classification
    bool isNumberStart(char c) const;
    bool isExponentIndicator(char c) const;
    bool isSign(char c) const;
    
    // Operator classification
    bool isOperatorStart(char c) const;
    bool isOperatorChar(char c) const;
    bool isPunctuation(char c) const;

private:
    // Unicode ranges for identifier characters
    bool isInUnicodeRange(char32_t codepoint, char32_t start, char32_t end) const;
    char32_t getUnicodeCodepoint(const String& text, size_t& pos) const;
};

} // namespace lexer
} // namespace tsc