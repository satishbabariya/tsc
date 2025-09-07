#include "tsc/lexer/Lexer.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <unordered_map>
#include <cctype>
#include <algorithm>

namespace tsc {

// Keyword mapping based on tsc.tm grammar
static const std::unordered_map<String, TokenType> keywords = {
    // Core keywords
    {"await", TokenType::Await},
    {"break", TokenType::Break},
    {"case", TokenType::Case},
    {"catch", TokenType::Catch},
    {"class", TokenType::Class},
    {"const", TokenType::Const},
    {"continue", TokenType::Continue},
    {"debugger", TokenType::Debugger},
    {"default", TokenType::Default},
    {"delete", TokenType::Delete},
    {"do", TokenType::Do},
    {"else", TokenType::Else},
    {"export", TokenType::Export},
    {"extends", TokenType::Extends},
    {"finally", TokenType::Finally},
    {"for", TokenType::For},
    {"function", TokenType::Function},
    {"if", TokenType::If},
    {"import", TokenType::Import},
    {"in", TokenType::In},
    {"instanceof", TokenType::Instanceof},
    {"new", TokenType::New},
    {"return", TokenType::Return},
    {"super", TokenType::Super},
    {"switch", TokenType::Switch},
    {"this", TokenType::This},
    {"throw", TokenType::Throw},
    {"try", TokenType::Try},
    {"typeof", TokenType::Typeof},
    {"var", TokenType::Var},
    {"void", TokenType::Void},
    {"while", TokenType::While},
    {"with", TokenType::With},
    {"yield", TokenType::Yield},
    
    // Future reserved
    {"enum", TokenType::Enum},
    
    // Literals
    {"null", TokenType::Null},
    {"true", TokenType::True},
    {"false", TokenType::False},
    
    // Soft keywords
    {"as", TokenType::As},
    {"assert", TokenType::Assert},
    {"asserts", TokenType::Asserts},
    {"async", TokenType::Async},
    {"from", TokenType::From},
    {"get", TokenType::Get},
    {"let", TokenType::Let},
    {"of", TokenType::Of},
    {"set", TokenType::Set},
    {"static", TokenType::Static},
    {"target", TokenType::Target},
    {"satisfies", TokenType::Satisfies},
    {"out", TokenType::Out},
    
    // TypeScript keywords
    {"implements", TokenType::Implements},
    {"interface", TokenType::Interface},
    {"private", TokenType::Private},
    {"protected", TokenType::Protected},
    {"public", TokenType::Public},
    {"any", TokenType::Any},
    {"unknown", TokenType::Unknown},
    {"boolean", TokenType::Boolean},
    {"number", TokenType::Number},
    {"string", TokenType::String},
    {"symbol", TokenType::Symbol},
    {"bigint", TokenType::Bigint},
    {"undefined", TokenType::Undefined},
    {"never", TokenType::Never},
    {"object", TokenType::Object},
    {"abstract", TokenType::Abstract},
    {"constructor", TokenType::Constructor},
    {"declare", TokenType::Declare},
    {"global", TokenType::Global},
    {"is", TokenType::Is},
    {"module", TokenType::Module},
    {"namespace", TokenType::Namespace},
    {"override", TokenType::Override},
    {"require", TokenType::Require},
    {"type", TokenType::Type},
    {"accessor", TokenType::Accessor},
    {"readonly", TokenType::Readonly},
    {"keyof", TokenType::Keyof},
    {"unique", TokenType::Unique},
    {"infer", TokenType::Infer}
};

Lexer::Lexer(DiagnosticEngine& diagnostics) : diagnostics_(diagnostics) {}

std::vector<Token> Lexer::tokenize(const String& source, const String& filename) {
    setSource(source, filename);
    
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        Token token = nextToken();
        
        // Skip whitespace and comments in the token stream
        if (token.getType() != TokenType::WhiteSpace &&
            token.getType() != TokenType::SingleLineComment &&
            token.getType() != TokenType::MultiLineComment) {
            tokens.push_back(token);
        }
        
        if (token.getType() == TokenType::EndOfInput) {
            break;
        }
    }
    
    return tokens;
}

void Lexer::setSource(const String& source, const String& filename) {
    source_ = source;
    filename_ = filename;
    current_ = 0;
    line_ = 1;
    column_ = 1;
    state_ = LexerState::Initial;
}

Token Lexer::nextToken() {
    if (isAtEnd()) {
        return makeToken(TokenType::EndOfInput);
    }
    
    return scanToken();
}

bool Lexer::isAtEnd() const {
    return current_ >= source_.length();
}

SourceLocation Lexer::getCurrentLocation() const {
    return SourceLocation(filename_, line_, column_);
}

char Lexer::peek(int offset) const {
    size_t pos = current_ + offset;
    if (pos >= source_.length()) {
        return '\0';
    }
    return source_[pos];
}

char Lexer::advance() {
    if (isAtEnd()) {
        return '\0';
    }
    
    char c = source_[current_++];
    
    if (c == '\n') {
        advanceLine();
    } else {
        column_++;
    }
    
    return c;
}

Token Lexer::scanToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return makeToken(TokenType::EndOfInput);
    }
    
    char c = peek();
    SourceLocation startLocation = getCurrentLocation();
    
    // Handle different token types
    if (isIdentifierStart(c)) {
        return scanIdentifier();
    }
    
    if (isDigit(c)) {
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
            if (isIdentifierStart(peek(1))) {
                return scanPrivateIdentifier();
            }
            break;
            
        // Comments
        case '/':
            if (peek(1) == '/') {
                return scanSingleLineComment();
            } else if (peek(1) == '*') {
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

Token Lexer::makeToken(TokenType type, TokenValue value) {
    return Token(type, getCurrentLocation(), value);
}

Token Lexer::makeErrorToken(const String& message) {
    reportError(message);
    return makeToken(TokenType::InvalidToken, message);
}

bool Lexer::isAtEnd(size_t pos) const {
    return pos >= source_.length();
}

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_' || c == '$';
}

bool Lexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isHexDigit(char c) const {
    return std::isxdigit(c);
}

bool Lexer::isOctalDigit(char c) const {
    return c >= '0' && c <= '7';
}

bool Lexer::isBinaryDigit(char c) const {
    return c == '0' || c == '1';
}

bool Lexer::isIdentifierStart(char c) const {
    return isAlpha(c) || c == '\\'; // Unicode escapes handled separately
}

bool Lexer::isIdentifierPart(char c) const {
    return isAlphaNumeric(c);
}

bool Lexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || 
           c == '\v' || c == '\f';
}

bool Lexer::isLineTerminator(char c) const {
    return c == '\n' || c == '\r';
}

Token Lexer::scanNumber() {
    size_t start = current_;
    
    // Handle different number formats
    if (peek() == '0') {
        advance();
        
        // Hexadecimal
        if (peek() == 'x' || peek() == 'X') {
            advance();
            while (isHexDigit(peek())) {
                advance();
            }
        }
        // Octal
        else if (peek() == 'o' || peek() == 'O') {
            advance();
            while (isOctalDigit(peek())) {
                advance();
            }
        }
        // Binary
        else if (peek() == 'b' || peek() == 'B') {
            advance();
            while (isBinaryDigit(peek())) {
                advance();
            }
        }
        // Regular number starting with 0
        else {
            while (isDigit(peek())) {
                advance();
            }
        }
    } else {
        // Regular decimal number
        while (isDigit(peek())) {
            advance();
        }
    }
    
    // Fractional part
    if (peek() == '.' && isDigit(peek(1))) {
        advance(); // consume '.'
        while (isDigit(peek())) {
            advance();
        }
    }
    
    // Exponential part
    if (peek() == 'e' || peek() == 'E') {
        advance();
        if (peek() == '+' || peek() == '-') {
            advance();
        }
        while (isDigit(peek())) {
            advance();
        }
    }
    
    // BigInt suffix
    bool isBigInt = false;
    if (peek() == 'n') {
        advance();
        isBigInt = true;
    }
    
    String numberStr = substring(start, current_);
    
    // Convert to numeric value
    try {
        if (isBigInt) {
            // For now, store as string - real implementation would use BigInt
            return makeToken(TokenType::NumericLiteral, numberStr);
        } else {
            double value = std::stod(numberStr);
            return makeToken(TokenType::NumericLiteral, value);
        }
    } catch (const std::exception&) {
        return makeErrorToken("Invalid number format");
    }
}

Token Lexer::scanString(char quote) {
    advance(); // consume opening quote
    size_t start = current_;
    
    while (!isAtEnd() && peek() != quote) {
        if (peek() == '\\') {
            advance(); // consume backslash
            if (!isAtEnd()) {
                advance(); // consume escaped character
            }
        } else if (isLineTerminator(peek())) {
            return makeErrorToken("Unterminated string literal");
        } else {
            advance();
        }
    }
    
    if (isAtEnd()) {
        return makeErrorToken("Unterminated string literal");
    }
    
    String value = substring(start, current_);
    advance(); // consume closing quote
    
    // TODO: Process escape sequences
    return makeToken(TokenType::StringLiteral, value);
}

Token Lexer::scanTemplate() {
    advance(); // consume opening `
    size_t start = current_;
    
    while (!isAtEnd() && peek() != '`') {
        if (peek() == '\\') {
            advance(); // consume backslash
            if (!isAtEnd()) {
                advance(); // consume escaped character
            }
        } else if (peek() == '$' && peek(1) == '{') {
            // Template expression - for now, simplified handling
            String value = substring(start, current_);
            advance(); // consume $
            advance(); // consume {
            return makeToken(TokenType::TemplateHead, value);
        } else {
            advance();
        }
    }
    
    if (isAtEnd()) {
        return makeErrorToken("Unterminated template literal");
    }
    
    String value = substring(start, current_);
    advance(); // consume closing `
    
    return makeToken(TokenType::NoSubstitutionTemplate, value);
}

Token Lexer::scanRegularExpression() {
    // Simplified regex scanning - real implementation needs parser context
    advance(); // consume opening /
    size_t start = current_;
    
    while (!isAtEnd() && peek() != '/') {
        if (peek() == '\\') {
            advance(); // consume backslash
            if (!isAtEnd()) {
                advance(); // consume escaped character
            }
        } else if (isLineTerminator(peek())) {
            return makeErrorToken("Unterminated regular expression");
        } else {
            advance();
        }
    }
    
    if (isAtEnd()) {
        return makeErrorToken("Unterminated regular expression");
    }
    
    String pattern = substring(start, current_);
    advance(); // consume closing /
    
    // Scan flags
    size_t flagStart = current_;
    while (isAlpha(peek())) {
        advance();
    }
    String flags = substring(flagStart, current_);
    
    return makeToken(TokenType::RegularExpressionLiteral, pattern + "/" + flags);
}

Token Lexer::scanIdentifier() {
    size_t start = current_;
    
    while (isIdentifierPart(peek())) {
        advance();
    }
    
    String text = substring(start, current_);
    TokenType type = getKeywordType(text);
    
    return makeToken(type, text);
}

Token Lexer::scanPrivateIdentifier() {
    advance(); // consume #
    size_t start = current_;
    
    while (isIdentifierPart(peek())) {
        advance();
    }
    
    String text = substring(start, current_);
    return makeToken(TokenType::PrivateIdentifier, "#" + text);
}

Token Lexer::scanSingleLineComment() {
    size_t start = current_;
    
    while (!isAtEnd() && !isLineTerminator(peek())) {
        advance();
    }
    
    String comment = substring(start, current_);
    return makeToken(TokenType::SingleLineComment, comment);
}

Token Lexer::scanMultiLineComment() {
    advance(); // consume /
    advance(); // consume *
    size_t start = current_;
    
    while (!isAtEnd()) {
        if (peek() == '*' && peek(1) == '/') {
            String comment = substring(start, current_);
            advance(); // consume *
            advance(); // consume /
            return makeToken(TokenType::MultiLineComment, comment);
        }
        advance();
    }
    
    return makeErrorToken("Unterminated comment");
}

Token Lexer::scanPunctuation() {
    char c = advance();
    
    switch (c) {
        case '{': return makeToken(TokenType::LeftBrace);
        case '}': return makeToken(TokenType::RightBrace);
        case '(': return makeToken(TokenType::LeftParen);
        case ')': return makeToken(TokenType::RightParen);
        case '[': return makeToken(TokenType::LeftBracket);
        case ']': return makeToken(TokenType::RightBracket);
        case ';': return makeToken(TokenType::Semicolon);
        case ',': return makeToken(TokenType::Comma);
        case '@': return makeToken(TokenType::At);
        case '~': return makeToken(TokenType::Tilde);
        case ':': return makeToken(TokenType::Colon);
        
        case '.':
            if (peek() == '.' && peek(1) == '.') {
                advance();
                advance();
                return makeToken(TokenType::DotDotDot);
            }
            return makeToken(TokenType::Dot);
            
        case '+':
            if (match('=')) return makeToken(TokenType::PlusEqual);
            if (match('+')) return makeToken(TokenType::PlusPlus);
            return makeToken(TokenType::Plus);
            
        case '-':
            if (match('=')) return makeToken(TokenType::MinusEqual);
            if (match('-')) return makeToken(TokenType::MinusMinus);
            return makeToken(TokenType::Minus);
            
        case '*':
            if (match('*')) {
                if (match('=')) return makeToken(TokenType::StarStarEqual);
                return makeToken(TokenType::StarStar);
            }
            if (match('=')) return makeToken(TokenType::StarEqual);
            return makeToken(TokenType::Star);
            
        case '/':
            if (match('=')) return makeToken(TokenType::SlashEqual);
            return makeToken(TokenType::Slash);
            
        case '%':
            if (match('=')) return makeToken(TokenType::PercentEqual);
            return makeToken(TokenType::Percent);
            
        case '&':
            if (match('&')) {
                if (match('=')) return makeToken(TokenType::AmpersandAmpersandEqual);
                return makeToken(TokenType::AmpersandAmpersand);
            }
            if (match('=')) return makeToken(TokenType::AmpersandEqual);
            return makeToken(TokenType::Ampersand);
            
        case '|':
            if (match('|')) {
                if (match('=')) return makeToken(TokenType::PipePipeEqual);
                return makeToken(TokenType::PipePipe);
            }
            if (match('=')) return makeToken(TokenType::PipeEqual);
            return makeToken(TokenType::Pipe);
            
        case '^':
            if (match('=')) return makeToken(TokenType::CaretEqual);
            return makeToken(TokenType::Caret);
            
        case '!':
            if (match('=')) {
                if (match('=')) return makeToken(TokenType::NotEqualEqual);
                return makeToken(TokenType::NotEqual);
            }
            return makeToken(TokenType::Exclamation);
            
        case '=':
            if (match('=')) {
                if (match('=')) return makeToken(TokenType::EqualEqualEqual);
                return makeToken(TokenType::EqualEqual);
            }
            if (match('>')) return makeToken(TokenType::Arrow);
            return makeToken(TokenType::Equal);
            
        case '<':
            if (match('<')) {
                if (match('=')) return makeToken(TokenType::LeftShiftEqual);
                return makeToken(TokenType::LeftShift);
            }
            if (match('=')) return makeToken(TokenType::LessEqual);
            return makeToken(TokenType::Less);
            
        case '>':
            if (match('>')) {
                if (match('>')) {
                    if (match('=')) return makeToken(TokenType::UnsignedRightShiftEqual);
                    return makeToken(TokenType::UnsignedRightShift);
                }
                if (match('=')) return makeToken(TokenType::RightShiftEqual);
                return makeToken(TokenType::RightShift);
            }
            if (match('=')) return makeToken(TokenType::GreaterEqual);
            return makeToken(TokenType::Greater);
            
        case '?':
            if (match('?')) {
                if (match('=')) return makeToken(TokenType::QuestionQuestionEqual);
                return makeToken(TokenType::QuestionQuestion);
            }
            if (match('.')) return makeToken(TokenType::QuestionDot);
            return makeToken(TokenType::Question);
    }
    
    return makeErrorToken("Unexpected character: " + String(1, c));
}

TokenType Lexer::getKeywordType(const String& text) const {
    auto it = keywords.find(text);
    return it != keywords.end() ? it->second : TokenType::Identifier;
}

void Lexer::reportError(const String& message) {
    diagnostics_.error(message, getCurrentLocation());
}

void Lexer::reportWarning(const String& message) {
    diagnostics_.warning(message, getCurrentLocation());
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && isWhitespace(peek())) {
        advance();
    }
}

bool Lexer::match(char expected) {
    if (isAtEnd() || peek() != expected) {
        return false;
    }
    advance();
    return true;
}

bool Lexer::matchNext(char expected) {
    return !isAtEnd() && peek() == expected;
}

String Lexer::substring(size_t start, size_t end) const {
    if (start >= source_.length()) return "";
    if (end > source_.length()) end = source_.length();
    return source_.substr(start, end - start);
}

void Lexer::advanceLine() {
    line_++;
    column_ = 1;
}

// LexerTokenStream implementation
LexerTokenStream::LexerTokenStream(unique_ptr<Lexer> lexer) 
    : lexer_(std::move(lexer)) {}

Token LexerTokenStream::peek() const {
    ensureCurrentToken();
    return *currentToken_;
}

Token LexerTokenStream::advance() {
    ensureCurrentToken();
    Token token = *currentToken_;
    currentToken_.reset();
    return token;
}

bool LexerTokenStream::isAtEnd() const {
    ensureCurrentToken();
    return currentToken_->getType() == TokenType::EndOfInput;
}

void LexerTokenStream::synchronize() {
    // Skip tokens until we find a statement boundary
    while (!isAtEnd()) {
        Token token = advance();
        if (token.getType() == TokenType::Semicolon) {
            return;
        }
        
        Token next = peek();
        if (next.isOneOf({TokenType::Class, TokenType::Function, TokenType::Var,
                         TokenType::For, TokenType::If, TokenType::While,
                         TokenType::Return, TokenType::Try})) {
            return;
        }
    }
}

void LexerTokenStream::skipUntil(TokenType type) {
    while (!isAtEnd() && peek().getType() != type) {
        advance();
    }
}

void LexerTokenStream::skipUntil(std::initializer_list<TokenType> types) {
    while (!isAtEnd() && !peek().isOneOf(types)) {
        advance();
    }
}

void LexerTokenStream::ensureCurrentToken() const {
    if (!currentToken_) {
        currentToken_ = lexer_->nextToken();
    }
}

} // namespace tsc