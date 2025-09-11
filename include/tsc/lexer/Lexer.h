#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"

namespace tsc {

class DiagnosticEngine;

// Lexer state for template literal parsing
enum class LexerState {
    Initial,
    Template,
    TemplateExpression
};

// Lexer class for tokenizing TypeScript source code
// Based on the grammar defined in tsc.tm
class Lexer {
public:
    explicit Lexer(DiagnosticEngine& diagnostics);
    
    // Main tokenization interface
    std::vector<Token> tokenize(const String& source, const String& filename);
    
    // Single token interface for streaming
    void setSource(const String& source, const String& filename);
    Token nextToken();
    bool isAtEnd() const;
    
    // State management
    void setState(LexerState state) { state_ = state; }
    LexerState getState() const { return state_; }
    
    // Position tracking
    SourceLocation getCurrentLocation() const;
    char peek(int offset = 0) const;
    char advance();
    
private:
    DiagnosticEngine& diagnostics_;
    String source_;
    String filename_;
    size_t current_ = 0;
    unsigned line_ = 1;
    unsigned column_ = 1;
    LexerState state_ = LexerState::Initial;
    
    // Token scanning methods
    Token scanToken();
    Token makeToken(TokenType type, TokenValue value = {});
    Token makeErrorToken(const String& message);
    
    // Character classification
    bool isAtEnd(size_t pos) const;
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isHexDigit(char c) const;
    bool isOctalDigit(char c) const;
    bool isBinaryDigit(char c) const;
    bool isIdentifierStart(char c) const;
    bool isIdentifierPart(char c) const;
    bool isWhitespace(char c) const;
    bool isLineTerminator(char c) const;
    
    // Literal scanning
    Token scanNumber();
    Token scanString(char quote);
    Token scanTemplate();
    Token scanRegularExpression();
    Token scanIdentifier();
    Token scanPrivateIdentifier();
    
    // Comment scanning
    Token scanSingleLineComment();
    Token scanMultiLineComment();
    
    // Punctuation and operators
    Token scanPunctuation();
    
    // Unicode support
    bool isUnicodeIdentifierStart(char32_t codePoint) const;
    bool isUnicodeIdentifierPart(char32_t codePoint) const;
    char32_t scanUnicodeEscape();
    String scanUnicodeString();
    
    // Keyword recognition
    TokenType getKeywordType(const String& text) const;
    
    // Error handling
    void reportError(const String& message);
    void reportWarning(const String& message);
    
    // Utility methods
    void skipWhitespace();
    bool match(char expected);
    bool matchNext(char expected);
    String substring(size_t start, size_t end) const;
    void advanceLine();
    
    // Template literal state management
    void enterTemplateState();
    void exitTemplateState();
    bool inTemplateState() const;
};

// Token stream implementation using the lexer
class LexerTokenStream : public TokenStream {
public:
    LexerTokenStream(unique_ptr<Lexer> lexer);
    
    // TokenStream interface
    Token peek() const override;
    Token advance() override;
    bool isAtEnd() const override;
    
    // Error recovery
    void synchronize() override;
    void skipUntil(TokenType type) override;
    void skipUntil(std::initializer_list<TokenType> types) override;

private:
    unique_ptr<Lexer> lexer_;
    mutable std::optional<Token> currentToken_;
    
    void ensureCurrentToken() const;
};

} // namespace tsc