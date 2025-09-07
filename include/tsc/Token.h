#pragma once

#include "tsc/Common.h"
#include <variant>

namespace tsc {

// Token types based on the grammar file tsc.tm
enum class TokenType {
    // End of input
    EndOfInput,
    
    // Literals
    Identifier,
    PrivateIdentifier,
    NumericLiteral,
    StringLiteral,
    NoSubstitutionTemplate,
    TemplateHead,
    TemplateMiddle,
    TemplateTail,
    RegularExpressionLiteral,
    
    // Keywords
    Await, Break, Case, Catch, Class, Const, Continue, Debugger,
    Default, Delete, Do, Else, Export, Extends, Finally, For,
    Function, If, Import, In, Instanceof, New, Return, Super,
    Switch, This, Throw, Try, Typeof, Var, Void, While, With, Yield,
    
    // Future reserved
    Enum,
    
    // Literals
    Null, True, False,
    
    // Soft keywords
    As, Assert, Asserts, Async, From, Get, Let, Of, Set, Static,
    Target, Satisfies, Out,
    
    // TypeScript keywords
    Implements, Interface, Private, Protected, Public,
    Any, Unknown, Boolean, Number, String, Symbol, Bigint,
    Undefined, Never, Object, Abstract, Constructor, Declare,
    Global, Is, Module, Namespace, Override, Require, Type,
    Accessor, Readonly, Keyof, Unique, Infer,
    
    // Punctuation
    LeftBrace,          // {
    RightBrace,         // }
    LeftParen,          // (
    RightParen,         // )
    LeftBracket,        // [
    RightBracket,       // ]
    Dot,                // .
    DotDotDot,          // ...
    Semicolon,          // ;
    Comma,              // ,
    Less,               // <
    Greater,            // >
    LessEqual,          // <=
    GreaterEqual,       // >=
    EqualEqual,         // ==
    NotEqual,           // !=
    EqualEqualEqual,    // ===
    NotEqualEqual,      // !==
    At,                 // @
    Plus,               // +
    Minus,              // -
    Star,               // *
    Slash,              // /
    Percent,            // %
    PlusPlus,           // ++
    MinusMinus,         // --
    LeftShift,          // <<
    RightShift,         // >>
    UnsignedRightShift, // >>>
    Ampersand,          // &
    Pipe,               // |
    Caret,              // ^
    Exclamation,        // !
    Tilde,              // ~
    AmpersandAmpersand, // &&
    PipePipe,           // ||
    Question,           // ?
    QuestionQuestion,   // ??
    QuestionDot,        // ?.
    Colon,              // :
    Equal,              // =
    PlusEqual,          // +=
    MinusEqual,         // -=
    StarEqual,          // *=
    SlashEqual,         // /=
    PercentEqual,       // %=
    LeftShiftEqual,     // <<=
    RightShiftEqual,    // >>=
    UnsignedRightShiftEqual, // >>>=
    AmpersandEqual,     // &=
    PipeEqual,          // |=
    CaretEqual,         // ^=
    Arrow,              // =>
    StarStar,           // **
    StarStarEqual,      // **=
    QuestionQuestionEqual, // ??=
    PipePipeEqual,      // ||=
    AmpersandAmpersandEqual, // &&=
    
    // Comments and whitespace (usually filtered out)
    SingleLineComment,
    MultiLineComment,
    WhiteSpace,
    
    // Error tokens
    InvalidToken,
    
    // Special tokens for parser
    InsertedSemicolon,  // Automatic semicolon insertion
};

// Token value types
using TokenValue = std::variant<
    std::monostate,     // No value
    String,             // String literals, identifiers
    double,             // Numeric literals
    bool                // Boolean literals
>;

// Token class representing a single lexical unit
class Token {
public:
    Token() = default;
    Token(TokenType type, const SourceLocation& location, TokenValue value = {})
        : type_(type), location_(location), value_(value) {}
    
    TokenType getType() const { return type_; }
    const SourceLocation& getLocation() const { return location_; }
    const TokenValue& getValue() const { return value_; }
    
    // Convenience methods for checking token types
    bool is(TokenType type) const { return type_ == type; }
    bool isNot(TokenType type) const { return type_ != type; }
    bool isOneOf(std::initializer_list<TokenType> types) const {
        for (auto type : types) {
            if (type_ == type) return true;
        }
        return false;
    }
    
    // Type predicates
    bool isKeyword() const;
    bool isLiteral() const;
    bool isPunctuation() const;
    bool isOperator() const;
    bool isBinaryOperator() const;
    bool isUnaryOperator() const;
    bool isAssignmentOperator() const;
    
    // Value accessors
    String getStringValue() const;
    double getNumericValue() const;
    bool getBooleanValue() const;
    
    // String representation for debugging
    String toString() const;

private:
    TokenType type_ = TokenType::EndOfInput;
    SourceLocation location_;
    TokenValue value_;
};

// Token stream interface for the parser
class TokenStream {
public:
    virtual ~TokenStream() = default;
    
    // Core interface
    virtual Token peek() const = 0;
    virtual Token advance() = 0;
    virtual bool isAtEnd() const = 0;
    
    // Convenience methods
    bool check(TokenType type) const { return peek().is(type); }
    bool match(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const String& errorMessage);
    
    // Error recovery
    virtual void synchronize() = 0;
    virtual void skipUntil(TokenType type) = 0;
    virtual void skipUntil(std::initializer_list<TokenType> types) = 0;
};

// Utility functions
const char* tokenTypeToString(TokenType type);
bool isKeywordToken(TokenType type);
bool isLiteralToken(TokenType type);
bool isPunctuationToken(TokenType type);

} // namespace tsc