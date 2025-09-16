/**
 * @file Token.h
 * @brief Token definitions and lexical analysis interfaces
 * @author TSC Compiler Team
 * @version 1.0
 * 
 * This file defines the complete token system for the TSC compiler,
 * including token types, token values, and token stream interfaces
 * for efficient lexical analysis.
 */

#pragma once

#include "tsc/Common.h"

#include <variant>
#include <initializer_list>
#include <unordered_set>
#include <unordered_map>

namespace tsc {

// ============================================================================
// TOKEN TYPE DEFINITIONS
// ============================================================================

/**
 * @brief Complete enumeration of all token types in TypeScript
 * 
 * This enum covers all lexical elements including keywords, operators,
 * punctuation, literals, and special tokens for the TypeScript language.
 */
enum class TokenType : std::uint16_t {
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

// ============================================================================
// TOKEN CLASS
// ============================================================================

/**
 * @brief Represents a single lexical token
 * 
 * Contains the token type, source location, and optional value.
 * Provides convenient methods for token classification and value access.
 */
class Token {
public:
    /// @brief Default constructor for invalid tokens
    Token() = default;
    
    /// @brief Construct token with type, location, and optional value
    /// @param type Token type
    /// @param location Source location
    /// @param value Optional token value
    Token(TokenType type, const SourceLocation& location, TokenValue value = {})
        : type_(type), location_(location), value_(value) {}
    
    // ========================================================================
    // ACCESSORS
    // ========================================================================
    
    TokenType getType() const noexcept { return type_; }
    const SourceLocation& getLocation() const noexcept { return location_; }
    const TokenValue& getValue() const noexcept { return value_; }
    
    // ========================================================================
    // TYPE CHECKING METHODS
    // ========================================================================
    
    /// @brief Check if token matches specific type
    bool is(TokenType type) const noexcept { return type_ == type; }
    
    /// @brief Check if token does not match specific type
    bool isNot(TokenType type) const noexcept { return type_ != type; }
    
    /// @brief Check if token matches any of the given types
    bool isOneOf(std::initializer_list<TokenType> types) const noexcept {
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

// ============================================================================
// TOKEN STREAM INTERFACE
// ============================================================================

/**
 * @brief Abstract interface for token streams
 * 
 * Provides a unified interface for accessing tokens during parsing,
 * with support for lookahead, error recovery, and synchronization.
 */
class TokenStream {
public:
    virtual ~TokenStream() = default;
    
    // ========================================================================
    // CORE INTERFACE
    // ========================================================================
    
    /// @brief Peek at current token without consuming it
    virtual Token peek() const = 0;
    
    /// @brief Advance to next token and return current token
    virtual Token advance() = 0;
    
    /// @brief Check if stream is at end of input
    virtual bool isAtEnd() const = 0;
    
    // ========================================================================
    // LOOKAHEAD INTERFACE
    // ========================================================================
    
    /// @brief Peek at token at given offset without consuming
    /// @param offset Number of tokens ahead to peek
    virtual Token peekAhead(Size offset) const = 0;
    
    /// @brief Check if token exists at given offset
    /// @param offset Number of tokens ahead to check
    virtual bool hasAhead(Size offset) const = 0;
    
    // ========================================================================
    // CONVENIENCE METHODS
    // ========================================================================
    
    /// @brief Check if current token matches type
    bool check(TokenType type) const { return peek().is(type); }
    
    /// @brief Check if current token matches any of the given types
    bool check(std::initializer_list<TokenType> types) const {
        return peek().isOneOf(types);
    }
    
    /// @brief Match current token and advance if successful
    /// @param type Token type to match
    /// @return true if matched and advanced, false otherwise
    bool match(TokenType type);
    
    /// @brief Match current token against multiple types and advance if successful
    /// @param types Token types to match
    /// @return true if matched and advanced, false otherwise
    bool match(std::initializer_list<TokenType> types);
    
    /// @brief Consume token of specific type or throw error
    /// @param type Expected token type
    /// @param errorMessage Error message if token doesn't match
    /// @return The consumed token
    Token consume(TokenType type, const String& errorMessage);
    
    // ========================================================================
    // ERROR RECOVERY
    // ========================================================================
    
    /// @brief Synchronize stream after error (skip to next statement)
    virtual void synchronize() = 0;
    
    /// @brief Skip tokens until specific type is found
    /// @param type Token type to stop at
    virtual void skipUntil(TokenType type) = 0;
    
    /// @brief Skip tokens until one of the given types is found
    /// @param types Token types to stop at
    virtual void skipUntil(std::initializer_list<TokenType> types) = 0;
};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/// @brief Convert token type to string representation
const char* toString(TokenType type) noexcept;

/// @brief Check if token type is a keyword
bool isKeyword(TokenType type) noexcept;

/// @brief Check if token type is a literal
bool isLiteral(TokenType type) noexcept;

/// @brief Check if token type is punctuation
bool isPunctuation(TokenType type) noexcept;

/// @brief Check if token type is an operator
bool isOperator(TokenType type) noexcept;

/// @brief Check if token type is a binary operator
bool isBinaryOperator(TokenType type) noexcept;

/// @brief Check if token type is a unary operator
bool isUnaryOperator(TokenType type) noexcept;

/// @brief Check if token type is an assignment operator
bool isAssignmentOperator(TokenType type) noexcept;

/// @brief Get operator precedence for binary operators
int getOperatorPrecedence(TokenType type) noexcept;

/// @brief Get operator associativity
bool isLeftAssociative(TokenType type) noexcept;

// ============================================================================
// CONSTANTS
// ============================================================================

/// @brief Set of all keyword token types
extern const std::unordered_set<TokenType> KEYWORD_TOKENS;

/// @brief Set of all operator token types
extern const std::unordered_set<TokenType> OPERATOR_TOKENS;

/// @brief Set of all assignment operator token types
extern const std::unordered_set<TokenType> ASSIGNMENT_OPERATOR_TOKENS;

/// @brief Map of operator precedence values
extern const std::unordered_map<TokenType, int> OPERATOR_PRECEDENCE;

} // namespace tsc