#include "tsc/lexer/tokens/TokenScanner.h"
#include "tsc/lexer/core/LexerCore.h"
#include "tsc/lexer/scanners/CharacterClassifier.h"

namespace tsc {
namespace lexer {

TokenScanner::TokenScanner(LexerCore& lexer) 
    : lexer_(lexer) {
    characterClassifier_ = lexer.getCharacterClassifier();
}

Token TokenScanner::scanToken() {
    char c = lexer_.peek();
    SourceLocation startLocation = lexer_.getCurrentLocation();

    // Handle different token types
    if (characterClassifier_->isIdentifierStart(c)) {
        return scanIdentifier();
    }

    if (characterClassifier_->isDigit(c)) {
        return scanNumber();
    }

    switch (c) {
        // String literals
        case '"':
        case '\'':
            return scanString(c);

        // Template literals
        case '`':
            return scanTemplate();

        // Private identifiers
        case '#':
            if (characterClassifier_->isIdentifierStart(lexer_.peek(1))) {
                return scanPrivateIdentifier();
            }
            break;

        // Comments
        case '/':
            if (lexer_.peek(1) == '/') {
                return scanSingleLineComment();
            } else if (lexer_.peek(1) == '*') {
                return scanMultiLineComment();
            } else {
                // Could be division or regex - context dependent
                return scanPunctuation();
            }

        // Regular expression literals would be handled here
        // but require parser context, so fall through to punctuation
    }

    // Punctuation and operators
    return scanPunctuation();
}

Token TokenScanner::scanNumber() {
    size_t start = lexer_.getCurrentLocation().getColumn();

    // Handle different number formats
    if (lexer_.peek() == '0') {
        lexer_.advance();

        // Hexadecimal
        if (lexer_.peek() == 'x' || lexer_.peek() == 'X') {
            lexer_.advance();
            while (characterClassifier_->isHexDigit(lexer_.peek())) {
                lexer_.advance();
            }
        }
        // Octal
        else if (lexer_.peek() == 'o' || lexer_.peek() == 'O') {
            lexer_.advance();
            while (characterClassifier_->isOctalDigit(lexer_.peek())) {
                lexer_.advance();
            }
        }
        // Binary
        else if (lexer_.peek() == 'b' || lexer_.peek() == 'B') {
            lexer_.advance();
            while (characterClassifier_->isBinaryDigit(lexer_.peek())) {
                lexer_.advance();
            }
        }
        // Regular number starting with 0
        else {
            while (characterClassifier_->isDigit(lexer_.peek())) {
                lexer_.advance();
            }
        }
    } else {
        // Regular decimal number
        while (characterClassifier_->isDigit(lexer_.peek())) {
            lexer_.advance();
        }
    }

    // Fractional part
    if (lexer_.peek() == '.' && characterClassifier_->isDigit(lexer_.peek(1))) {
        lexer_.advance(); // consume '.'
        while (characterClassifier_->isDigit(lexer_.peek())) {
            lexer_.advance();
        }
    }

    // Exponential part
    if (lexer_.peek() == 'e' || lexer_.peek() == 'E') {
        lexer_.advance();
        if (lexer_.peek() == '+' || lexer_.peek() == '-') {
            lexer_.advance();
        }
        while (characterClassifier_->isDigit(lexer_.peek())) {
            lexer_.advance();
        }
    }

    // BigInt suffix
    bool isBigInt = false;
    if (lexer_.peek() == 'n') {
        lexer_.advance();
        isBigInt = true;
    }

    String numberStr = getCurrentLexeme();

    // Convert to numeric value
    try {
        if (isBigInt) {
            // For now, store as string - real implementation would use BigInt
            return lexer_.makeToken(TokenType::NumericLiteral, TokenValue(numberStr));
        } else {
            double value = std::stod(numberStr);
            return lexer_.makeToken(TokenType::NumericLiteral, TokenValue(value));
        }
    } catch (const std::exception&) {
        return lexer_.makeErrorToken("Invalid number format");
    }
}

Token TokenScanner::scanString(char quote) {
    lexer_.advance(); // consume opening quote
    size_t start = lexer_.getCurrentLocation().getColumn();

    while (!lexer_.isAtEnd() && lexer_.peek() != quote) {
        if (lexer_.peek() == '\\') {
            lexer_.advance(); // consume backslash
            if (!lexer_.isAtEnd()) {
                lexer_.advance(); // consume escaped character
            }
        } else if (characterClassifier_->isLineTerminator(lexer_.peek())) {
            return lexer_.makeErrorToken("Unterminated string literal");
        } else {
            lexer_.advance();
        }
    }

    if (lexer_.isAtEnd()) {
        return lexer_.makeErrorToken("Unterminated string literal");
    }

    String value = getCurrentLexeme();
    lexer_.advance(); // consume closing quote

    // TODO: Process escape sequences
    return lexer_.makeToken(TokenType::StringLiteral, TokenValue(value));
}

Token TokenScanner::scanTemplate() {
    lexer_.advance(); // consume opening `
    size_t start = lexer_.getCurrentLocation().getColumn();

    while (!lexer_.isAtEnd() && lexer_.peek() != '`') {
        if (lexer_.peek() == '\\') {
            lexer_.advance(); // consume backslash
            if (!lexer_.isAtEnd()) {
                lexer_.advance(); // consume escaped character
            }
        } else if (lexer_.peek() == '$' && lexer_.peek(1) == '{') {
            // Found template expression - return template head
            String value = getCurrentLexeme();
            lexer_.advance(); // consume $
            lexer_.advance(); // consume {
            // TODO: Set template expression state
            return lexer_.makeToken(TokenType::TemplateHead, TokenValue(value));
        } else {
            lexer_.advance();
        }
    }

    if (lexer_.isAtEnd()) {
        return lexer_.makeErrorToken("Unterminated template literal");
    }

    // No template expressions found - this is a no-substitution template
    String value = getCurrentLexeme();
    lexer_.advance(); // consume closing `

    return lexer_.makeToken(TokenType::NoSubstitutionTemplate, TokenValue(value));
}

Token TokenScanner::scanTemplateTail() {
    // TODO: Implement template tail scanning
    return lexer_.makeToken(TokenType::TemplateTail, TokenValue());
}

Token TokenScanner::scanRegularExpression() {
    // TODO: Implement regular expression scanning
    return lexer_.makeToken(TokenType::RegularExpression, TokenValue());
}

Token TokenScanner::scanIdentifier() {
    while (characterClassifier_->isIdentifierPart(lexer_.peek())) {
        lexer_.advance();
    }

    String text = getCurrentLexeme();
    
    // Check if it's a keyword
    TokenType type = lexer_.getKeywordMatcher()->matchKeyword(text);
    
    return lexer_.makeToken(type, TokenValue(text));
}

Token TokenScanner::scanPrivateIdentifier() {
    // TODO: Implement private identifier scanning
    return lexer_.makeToken(TokenType::PrivateIdentifier, TokenValue());
}

Token TokenScanner::scanSingleLineComment() {
    // TODO: Implement single line comment scanning
    return lexer_.makeToken(TokenType::Comment, TokenValue());
}

Token TokenScanner::scanMultiLineComment() {
    // TODO: Implement multi-line comment scanning
    return lexer_.makeToken(TokenType::Comment, TokenValue());
}

Token TokenScanner::scanOperator() {
    // TODO: Implement operator scanning
    return lexer_.makeToken(TokenType::Plus, TokenValue());
}

Token TokenScanner::scanPunctuation() {
    // TODO: Implement punctuation scanning
    return lexer_.makeToken(TokenType::Semicolon, TokenValue());
}

Token TokenScanner::scanHexNumber() {
    // TODO: Implement hex number scanning
    return lexer_.makeToken(TokenType::Number, TokenValue());
}

Token TokenScanner::scanOctalNumber() {
    // TODO: Implement octal number scanning
    return lexer_.makeToken(TokenType::Number, TokenValue());
}

Token TokenScanner::scanBinaryNumber() {
    // TODO: Implement binary number scanning
    return lexer_.makeToken(TokenType::Number, TokenValue());
}

Token TokenScanner::scanDecimalNumber() {
    // TODO: Implement decimal number scanning
    return lexer_.makeToken(TokenType::Number, TokenValue());
}

Token TokenScanner::scanFloatNumber() {
    // TODO: Implement float number scanning
    return lexer_.makeToken(TokenType::Number, TokenValue());
}

Token TokenScanner::scanExponent() {
    // TODO: Implement exponent scanning
    return lexer_.makeToken(TokenType::Number, TokenValue());
}

bool TokenScanner::match(char expected) {
    if (lexer_.isAtEnd()) return false;
    if (lexer_.peek() != expected) return false;
    
    lexer_.advance();
    return true;
}

String TokenScanner::getCurrentLexeme() {
    return lexer_.getCurrentLexeme();
}

void TokenScanner::consumeDigits() {
    // TODO: Implement digit consumption
}

bool TokenScanner::consumeHexDigits() {
    // TODO: Implement hex digit consumption
    return false;
}

bool TokenScanner::consumeOctalDigits() {
    // TODO: Implement octal digit consumption
    return false;
}

bool TokenScanner::consumeBinaryDigits() {
    // TODO: Implement binary digit consumption
    return false;
}

} // namespace lexer
} // namespace tsc