#include "tsc/lexer/scanners/CharacterClassifier.h"

namespace tsc {
namespace lexer {

bool CharacterClassifier::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}

bool CharacterClassifier::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool CharacterClassifier::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool CharacterClassifier::isHexDigit(char c) const {
    return isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool CharacterClassifier::isOctalDigit(char c) const {
    return c >= '0' && c <= '7';
}

bool CharacterClassifier::isBinaryDigit(char c) const {
    return c == '0' || c == '1';
}

bool CharacterClassifier::isIdentifierStart(char c) const {
    return isAlpha(c) || isUnicodeIdentifierStart(c);
}

bool CharacterClassifier::isIdentifierPart(char c) const {
    return isAlphaNumeric(c) || isUnicodeIdentifierPart(c);
}

bool CharacterClassifier::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r';
}

bool CharacterClassifier::isLineTerminator(char c) const {
    return c == '\n' || c == '\r';
}

bool CharacterClassifier::isUnicodeIdentifierStart(char c) const {
    // TODO: Implement Unicode identifier start character checking
    return false;
}

bool CharacterClassifier::isUnicodeIdentifierPart(char c) const {
    // TODO: Implement Unicode identifier part character checking
    return false;
}

bool CharacterClassifier::isUnicodeEscapeSequence(const String& text, size_t pos) const {
    // TODO: Implement Unicode escape sequence checking
    return false;
}

bool CharacterClassifier::isStringEscapeChar(char c) const {
    return c == '\\' || c == '"' || c == '\'' || c == 'n' || c == 't' || c == 'r' || c == 'v' || c == 'f';
}

bool CharacterClassifier::isTemplateEscapeChar(char c) const {
    return c == '\\' || c == '`' || c == '$';
}

bool CharacterClassifier::isNumberStart(char c) const {
    return isDigit(c) || c == '.';
}

bool CharacterClassifier::isExponentIndicator(char c) const {
    return c == 'e' || c == 'E';
}

bool CharacterClassifier::isSign(char c) const {
    return c == '+' || c == '-';
}

bool CharacterClassifier::isOperatorStart(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
           c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || 
           c == '|' || c == '^' || c == '~' || c == '?';
}

bool CharacterClassifier::isOperatorChar(char c) const {
    return isOperatorStart(c) || c == '=' || c == '&' || c == '|';
}

bool CharacterClassifier::isPunctuation(char c) const {
    return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
           c == ';' || c == ',' || c == '.' || c == ':' || c == '?' || c == '!';
}

bool CharacterClassifier::isInUnicodeRange(char32_t codepoint, char32_t start, char32_t end) const {
    return codepoint >= start && codepoint <= end;
}

char32_t CharacterClassifier::getUnicodeCodepoint(const String& text, size_t& pos) const {
    // TODO: Implement Unicode codepoint extraction
    return 0;
}

} // namespace lexer
} // namespace tsc