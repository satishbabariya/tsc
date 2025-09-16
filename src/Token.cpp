/**
 * @file Token.cpp
 * @brief Implementation of token system and utilities
 * @author TSC Compiler Team
 * @version 1.0
 * 
 * This file provides the implementation for token classification,
 * value access, and utility functions for the token system.
 */

#include "tsc/Token.h"

#include <sstream>
#include <unordered_set>
#include <unordered_map>

namespace tsc {

// ============================================================================
// TOKEN CLASSIFICATION IMPLEMENTATION
// ============================================================================

bool Token::isKeyword() const {
    return isKeyword(type_);
}

bool Token::isLiteral() const {
    return isLiteral(type_);
}

bool Token::isPunctuation() const {
    return isPunctuation(type_);
}

bool Token::isOperator() const {
    return isOperator(type_);
}

bool Token::isBinaryOperator() const {
    return isBinaryOperator(type_);
}

bool Token::isUnaryOperator() const {
    return isUnaryOperator(type_);
}

bool Token::isAssignmentOperator() const {
    return isAssignmentOperator(type_);
}

bool Token::isComparisonOperator() const {
    switch (type_) {
        case TokenType::Less:
        case TokenType::Greater:
        case TokenType::LessEqual:
        case TokenType::GreaterEqual:
        case TokenType::EqualEqual:
        case TokenType::NotEqual:
        case TokenType::EqualEqualEqual:
        case TokenType::NotEqualEqual:
            return true;
        default:
            return false;
    }
}

bool Token::isArithmeticOperator() const {
    switch (type_) {
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Star:
        case TokenType::Slash:
        case TokenType::Percent:
        case TokenType::StarStar:
            return true;
        default:
            return false;
    }
}

bool Token::isLogicalOperator() const {
    switch (type_) {
        case TokenType::AmpersandAmpersand:
        case TokenType::PipePipe:
        case TokenType::Exclamation:
        case TokenType::QuestionQuestion:
            return true;
        default:
            return false;
    }
}

// ============================================================================
// VALUE ACCESS IMPLEMENTATION
// ============================================================================

String Token::getStringValue() const {
    if (std::holds_alternative<String>(value_)) {
        return std::get<String>(value_);
    }
    return "";
}

double Token::getNumericValue() const {
    if (std::holds_alternative<double>(value_)) {
        return std::get<double>(value_);
    }
    return 0.0;
}

bool Token::getBooleanValue() const {
    if (std::holds_alternative<bool>(value_)) {
        return std::get<bool>(value_);
    }
    return false;
}

// ============================================================================
// STRING REPRESENTATION
// ============================================================================

String Token::toString() const {
    std::ostringstream ss;
    ss << toString(type_);
    
    if (!std::holds_alternative<std::monostate>(value_)) {
        ss << "(";
        if (std::holds_alternative<String>(value_)) {
            ss << "\"" << getStringValue() << "\"";
        } else if (std::holds_alternative<double>(value_)) {
            ss << getNumericValue();
        } else if (std::holds_alternative<bool>(value_)) {
            ss << (getBooleanValue() ? "true" : "false");
        }
        ss << ")";
    }
    
    ss << "@" << location_.toString();
    return ss.str();
}

// ============================================================================
// TOKEN STREAM IMPLEMENTATION
// ============================================================================

bool TokenStream::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool TokenStream::match(std::initializer_list<TokenType> types) {
    if (check(types)) {
        advance();
        return true;
    }
    return false;
}

Token TokenStream::consume(TokenType type, const String& errorMessage) {
    if (check(type)) {
        return advance();
    }
    throw CompilerError(errorMessage, peek().getLocation());
}

// ============================================================================
// UTILITY FUNCTIONS IMPLEMENTATION
// ============================================================================

const char* toString(TokenType type) noexcept {
    switch (type) {
        // Special tokens
        case TokenType::EndOfInput: return "EndOfInput";
        case TokenType::InvalidToken: return "InvalidToken";
        case TokenType::InsertedSemicolon: return "InsertedSemicolon";
        
        // Literals
        case TokenType::Identifier: return "Identifier";
        case TokenType::PrivateIdentifier: return "PrivateIdentifier";
        case TokenType::NumericLiteral: return "NumericLiteral";
        case TokenType::StringLiteral: return "StringLiteral";
        case TokenType::NoSubstitutionTemplate: return "NoSubstitutionTemplate";
        case TokenType::TemplateHead: return "TemplateHead";
        case TokenType::TemplateMiddle: return "TemplateMiddle";
        case TokenType::TemplateTail: return "TemplateTail";
        case TokenType::RegularExpressionLiteral: return "RegularExpressionLiteral";
        
        // Keywords - Control Flow
        case TokenType::Await: return "await";
        case TokenType::Break: return "break";
        case TokenType::Case: return "case";
        case TokenType::Catch: return "catch";
        case TokenType::Continue: return "continue";
        case TokenType::Default: return "default";
        case TokenType::Do: return "do";
        case TokenType::Else: return "else";
        case TokenType::Finally: return "finally";
        case TokenType::For: return "for";
        case TokenType::If: return "if";
        case TokenType::Return: return "return";
        case TokenType::Switch: return "switch";
        case TokenType::Throw: return "throw";
        case TokenType::Try: return "try";
        case TokenType::While: return "while";
        case TokenType::Yield: return "yield";
        
        // Keywords - Declarations
        case TokenType::Class: return "class";
        case TokenType::Const: return "const";
        case TokenType::Function: return "function";
        case TokenType::Let: return "let";
        case TokenType::Var: return "var";
        
        // Keywords - Module System
        case TokenType::Export: return "export";
        case TokenType::Import: return "import";
        case TokenType::From: return "from";
        
        // Keywords - Object-Oriented
        case TokenType::Extends: return "extends";
        case TokenType::Implements: return "implements";
        case TokenType::Interface: return "interface";
        case TokenType::Private: return "private";
        case TokenType::Protected: return "protected";
        case TokenType::Public: return "public";
        case TokenType::Static: return "static";
        case TokenType::Super: return "super";
        case TokenType::This: return "this";
        
        // Keywords - Type System
        case TokenType::Any: return "any";
        case TokenType::As: return "as";
        case TokenType::Boolean: return "boolean";
        case TokenType::Constructor: return "constructor";
        case TokenType::Declare: return "declare";
        case TokenType::Infer: return "infer";
        case TokenType::Is: return "is";
        case TokenType::Keyof: return "keyof";
        case TokenType::Never: return "never";
        case TokenType::Number: return "number";
        case TokenType::Object: return "object";
        case TokenType::Readonly: return "readonly";
        case TokenType::String: return "string";
        case TokenType::Symbol: return "symbol";
        case TokenType::Type: return "type";
        case TokenType::Typeof: return "typeof";
        case TokenType::Undefined: return "undefined";
        case TokenType::Unique: return "unique";
        case TokenType::Unknown: return "unknown";
        case TokenType::Void: return "void";
        
        // Keywords - Advanced Features
        case TokenType::Abstract: return "abstract";
        case TokenType::Accessor: return "accessor";
        case TokenType::Assert: return "assert";
        case TokenType::Asserts: return "asserts";
        case TokenType::Async: return "async";
        case TokenType::Bigint: return "bigint";
        case TokenType::Debugger: return "debugger";
        case TokenType::Enum: return "enum";
        case TokenType::Get: return "get";
        case TokenType::Global: return "global";
        case TokenType::In: return "in";
        case TokenType::Instanceof: return "instanceof";
        case TokenType::Module: return "module";
        case TokenType::Namespace: return "namespace";
        case TokenType::New: return "new";
        case TokenType::Of: return "of";
        case TokenType::Out: return "out";
        case TokenType::Override: return "override";
        case TokenType::Require: return "require";
        case TokenType::Satisfies: return "satisfies";
        case TokenType::Set: return "set";
        case TokenType::Target: return "target";
        case TokenType::With: return "with";
        
        // Boolean literals
        case TokenType::False: return "false";
        case TokenType::Null: return "null";
        case TokenType::True: return "true";
        
        // Punctuation
        case TokenType::LeftBrace: return "{";
        case TokenType::RightBrace: return "}";
        case TokenType::LeftParen: return "(";
        case TokenType::RightParen: return ")";
        case TokenType::LeftBracket: return "[";
        case TokenType::RightBracket: return "]";
        case TokenType::Dot: return ".";
        case TokenType::DotDotDot: return "...";
        case TokenType::Semicolon: return ";";
        case TokenType::Comma: return ",";
        case TokenType::Colon: return ":";
        case TokenType::At: return "@";
        
        // Comparison operators
        case TokenType::Less: return "<";
        case TokenType::Greater: return ">";
        case TokenType::LessEqual: return "<=";
        case TokenType::GreaterEqual: return ">=";
        case TokenType::EqualEqual: return "==";
        case TokenType::NotEqual: return "!=";
        case TokenType::EqualEqualEqual: return "===";
        case TokenType::NotEqualEqual: return "!==";
        
        // Arithmetic operators
        case TokenType::Plus: return "+";
        case TokenType::Minus: return "-";
        case TokenType::Star: return "*";
        case TokenType::Slash: return "/";
        case TokenType::Percent: return "%";
        case TokenType::StarStar: return "**";
        
        // Increment/decrement operators
        case TokenType::PlusPlus: return "++";
        case TokenType::MinusMinus: return "--";
        
        // Bitwise operators
        case TokenType::LeftShift: return "<<";
        case TokenType::RightShift: return ">>";
        case TokenType::UnsignedRightShift: return ">>>";
        case TokenType::Ampersand: return "&";
        case TokenType::Pipe: return "|";
        case TokenType::Caret: return "^";
        case TokenType::Tilde: return "~";
        
        // Logical operators
        case TokenType::AmpersandAmpersand: return "&&";
        case TokenType::PipePipe: return "||";
        case TokenType::Exclamation: return "!";
        
        // Conditional operators
        case TokenType::Question: return "?";
        case TokenType::QuestionQuestion: return "??";
        case TokenType::QuestionDot: return "?.";
        
        // Assignment operators
        case TokenType::Equal: return "=";
        case TokenType::PlusEqual: return "+=";
        case TokenType::MinusEqual: return "-=";
        case TokenType::StarEqual: return "*=";
        case TokenType::SlashEqual: return "/=";
        case TokenType::PercentEqual: return "%=";
        case TokenType::LeftShiftEqual: return "<<=";
        case TokenType::RightShiftEqual: return ">>=";
        case TokenType::UnsignedRightShiftEqual: return ">>>=";
        case TokenType::AmpersandEqual: return "&=";
        case TokenType::PipeEqual: return "|=";
        case TokenType::CaretEqual: return "^=";
        case TokenType::StarStarEqual: return "**=";
        case TokenType::QuestionQuestionEqual: return "??=";
        case TokenType::PipePipeEqual: return "||=";
        case TokenType::AmpersandAmpersandEqual: return "&&=";
        
        // Special operators
        case TokenType::Arrow: return "=>";
        
        // Comments and whitespace
        case TokenType::SingleLineComment: return "SingleLineComment";
        case TokenType::MultiLineComment: return "MultiLineComment";
        case TokenType::WhiteSpace: return "WhiteSpace";
        
        default: return "Unknown";
    }
}

bool isKeyword(TokenType type) noexcept {
    static const std::unordered_set<TokenType> keywords = {
        TokenType::Await, TokenType::Break, TokenType::Case, TokenType::Catch,
        TokenType::Class, TokenType::Const, TokenType::Continue, TokenType::Debugger,
        TokenType::Default, TokenType::Delete, TokenType::Do, TokenType::Else,
        TokenType::Export, TokenType::Extends, TokenType::Finally, TokenType::For,
        TokenType::Function, TokenType::If, TokenType::Import, TokenType::In,
        TokenType::Instanceof, TokenType::New, TokenType::Return, TokenType::Super,
        TokenType::Switch, TokenType::This, TokenType::Throw, TokenType::Try,
        TokenType::Typeof, TokenType::Var, TokenType::Void, TokenType::While,
        TokenType::With, TokenType::Yield, TokenType::Enum, TokenType::As,
        TokenType::Assert, TokenType::Asserts, TokenType::Async, TokenType::From,
        TokenType::Get, TokenType::Let, TokenType::Of, TokenType::Set,
        TokenType::Static, TokenType::Target, TokenType::Satisfies, TokenType::Out,
        TokenType::Implements, TokenType::Interface, TokenType::Private,
        TokenType::Protected, TokenType::Public, TokenType::Any, TokenType::Unknown,
        TokenType::Boolean, TokenType::Number, TokenType::String, TokenType::Symbol,
        TokenType::Bigint, TokenType::Undefined, TokenType::Never, TokenType::Object,
        TokenType::Abstract, TokenType::Constructor, TokenType::Declare,
        TokenType::Global, TokenType::Is, TokenType::Module, TokenType::Namespace,
        TokenType::Override, TokenType::Require, TokenType::Type, TokenType::Accessor,
        TokenType::Readonly, TokenType::Keyof, TokenType::Unique, TokenType::Infer
    };
    return keywords.find(type) != keywords.end();
}

bool isLiteral(TokenType type) noexcept {
    switch (type) {
        case TokenType::NumericLiteral:
        case TokenType::StringLiteral:
        case TokenType::NoSubstitutionTemplate:
        case TokenType::RegularExpressionLiteral:
        case TokenType::Null:
        case TokenType::True:
        case TokenType::False:
            return true;
        default:
            return false;
    }
}

bool isPunctuation(TokenType type) noexcept {
    switch (type) {
        case TokenType::LeftBrace:
        case TokenType::RightBrace:
        case TokenType::LeftParen:
        case TokenType::RightParen:
        case TokenType::LeftBracket:
        case TokenType::RightBracket:
        case TokenType::Dot:
        case TokenType::DotDotDot:
        case TokenType::Semicolon:
        case TokenType::Comma:
        case TokenType::Colon:
        case TokenType::At:
            return true;
        default:
            return false;
    }
}

bool isOperator(TokenType type) noexcept {
    return isBinaryOperator(type) || isUnaryOperator(type) || isAssignmentOperator(type);
}

bool isBinaryOperator(TokenType type) noexcept {
    switch (type) {
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Star:
        case TokenType::Slash:
        case TokenType::Percent:
        case TokenType::StarStar:
        case TokenType::Less:
        case TokenType::Greater:
        case TokenType::LessEqual:
        case TokenType::GreaterEqual:
        case TokenType::EqualEqual:
        case TokenType::NotEqual:
        case TokenType::EqualEqualEqual:
        case TokenType::NotEqualEqual:
        case TokenType::LeftShift:
        case TokenType::RightShift:
        case TokenType::UnsignedRightShift:
        case TokenType::Ampersand:
        case TokenType::Pipe:
        case TokenType::Caret:
        case TokenType::AmpersandAmpersand:
        case TokenType::PipePipe:
        case TokenType::QuestionQuestion:
        case TokenType::In:
        case TokenType::Instanceof:
            return true;
        default:
            return false;
    }
}

bool isUnaryOperator(TokenType type) noexcept {
    switch (type) {
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Exclamation:
        case TokenType::Tilde:
        case TokenType::Typeof:
        case TokenType::Void:
        case TokenType::Delete:
        case TokenType::PlusPlus:
        case TokenType::MinusMinus:
            return true;
        default:
            return false;
    }
}

bool isAssignmentOperator(TokenType type) noexcept {
    switch (type) {
        case TokenType::Equal:
        case TokenType::PlusEqual:
        case TokenType::MinusEqual:
        case TokenType::StarEqual:
        case TokenType::SlashEqual:
        case TokenType::PercentEqual:
        case TokenType::LeftShiftEqual:
        case TokenType::RightShiftEqual:
        case TokenType::UnsignedRightShiftEqual:
        case TokenType::AmpersandEqual:
        case TokenType::PipeEqual:
        case TokenType::CaretEqual:
        case TokenType::StarStarEqual:
        case TokenType::QuestionQuestionEqual:
        case TokenType::PipePipeEqual:
        case TokenType::AmpersandAmpersandEqual:
            return true;
        default:
            return false;
    }
}

int getOperatorPrecedence(TokenType type) noexcept {
    static const std::unordered_map<TokenType, int> precedence = {
        // Assignment operators (lowest precedence)
        {TokenType::Equal, 1},
        {TokenType::PlusEqual, 1}, {TokenType::MinusEqual, 1},
        {TokenType::StarEqual, 1}, {TokenType::SlashEqual, 1},
        {TokenType::PercentEqual, 1}, {TokenType::LeftShiftEqual, 1},
        {TokenType::RightShiftEqual, 1}, {TokenType::UnsignedRightShiftEqual, 1},
        {TokenType::AmpersandEqual, 1}, {TokenType::PipeEqual, 1},
        {TokenType::CaretEqual, 1}, {TokenType::StarStarEqual, 1},
        {TokenType::QuestionQuestionEqual, 1}, {TokenType::PipePipeEqual, 1},
        {TokenType::AmpersandAmpersandEqual, 1},
        
        // Conditional operators
        {TokenType::QuestionQuestion, 2},
        
        // Logical OR
        {TokenType::PipePipe, 3},
        
        // Logical AND
        {TokenType::AmpersandAmpersand, 4},
        
        // Bitwise OR
        {TokenType::Pipe, 5},
        
        // Bitwise XOR
        {TokenType::Caret, 6},
        
        // Bitwise AND
        {TokenType::Ampersand, 7},
        
        // Equality operators
        {TokenType::EqualEqual, 8}, {TokenType::NotEqual, 8},
        {TokenType::EqualEqualEqual, 8}, {TokenType::NotEqualEqual, 8},
        
        // Relational operators
        {TokenType::Less, 9}, {TokenType::Greater, 9},
        {TokenType::LessEqual, 9}, {TokenType::GreaterEqual, 9},
        {TokenType::In, 9}, {TokenType::Instanceof, 9},
        
        // Bitwise shift
        {TokenType::LeftShift, 10}, {TokenType::RightShift, 10},
        {TokenType::UnsignedRightShift, 10},
        
        // Addition and subtraction
        {TokenType::Plus, 11}, {TokenType::Minus, 11},
        
        // Multiplication, division, and remainder
        {TokenType::Star, 12}, {TokenType::Slash, 12}, {TokenType::Percent, 12},
        
        // Exponentiation (highest precedence)
        {TokenType::StarStar, 13}
    };
    
    auto it = precedence.find(type);
    return it != precedence.end() ? it->second : 0;
}

bool isLeftAssociative(TokenType type) noexcept {
    // Most operators are left-associative except:
    // - Assignment operators (right-associative)
    // - Exponentiation (**) (right-associative)
    // - Conditional operator (?:) (right-associative)
    switch (type) {
        case TokenType::Equal:
        case TokenType::PlusEqual:
        case TokenType::MinusEqual:
        case TokenType::StarEqual:
        case TokenType::SlashEqual:
        case TokenType::PercentEqual:
        case TokenType::LeftShiftEqual:
        case TokenType::RightShiftEqual:
        case TokenType::UnsignedRightShiftEqual:
        case TokenType::AmpersandEqual:
        case TokenType::PipeEqual:
        case TokenType::CaretEqual:
        case TokenType::StarStarEqual:
        case TokenType::QuestionQuestionEqual:
        case TokenType::PipePipeEqual:
        case TokenType::AmpersandAmpersandEqual:
        case TokenType::StarStar:
        case TokenType::Question:
            return false;
        default:
            return true;
    }
}

// ============================================================================
// CONSTANTS DEFINITION
// ============================================================================

const std::unordered_set<TokenType> KEYWORD_TOKENS = {
    TokenType::Await, TokenType::Break, TokenType::Case, TokenType::Catch,
    TokenType::Class, TokenType::Const, TokenType::Continue, TokenType::Debugger,
    TokenType::Default, TokenType::Delete, TokenType::Do, TokenType::Else,
    TokenType::Export, TokenType::Extends, TokenType::Finally, TokenType::For,
    TokenType::Function, TokenType::If, TokenType::Import, TokenType::In,
    TokenType::Instanceof, TokenType::New, TokenType::Return, TokenType::Super,
    TokenType::Switch, TokenType::This, TokenType::Throw, TokenType::Try,
    TokenType::Typeof, TokenType::Var, TokenType::Void, TokenType::While,
    TokenType::With, TokenType::Yield, TokenType::Enum, TokenType::As,
    TokenType::Assert, TokenType::Asserts, TokenType::Async, TokenType::From,
    TokenType::Get, TokenType::Let, TokenType::Of, TokenType::Set,
    TokenType::Static, TokenType::Target, TokenType::Satisfies, TokenType::Out,
    TokenType::Implements, TokenType::Interface, TokenType::Private,
    TokenType::Protected, TokenType::Public, TokenType::Any, TokenType::Unknown,
    TokenType::Boolean, TokenType::Number, TokenType::String, TokenType::Symbol,
    TokenType::Bigint, TokenType::Undefined, TokenType::Never, TokenType::Object,
    TokenType::Abstract, TokenType::Constructor, TokenType::Declare,
    TokenType::Global, TokenType::Is, TokenType::Module, TokenType::Namespace,
    TokenType::Override, TokenType::Require, TokenType::Type, TokenType::Accessor,
    TokenType::Readonly, TokenType::Keyof, TokenType::Unique, TokenType::Infer
};

const std::unordered_set<TokenType> OPERATOR_TOKENS = {
    TokenType::Plus, TokenType::Minus, TokenType::Star, TokenType::Slash,
    TokenType::Percent, TokenType::StarStar, TokenType::Less, TokenType::Greater,
    TokenType::LessEqual, TokenType::GreaterEqual, TokenType::EqualEqual,
    TokenType::NotEqual, TokenType::EqualEqualEqual, TokenType::NotEqualEqual,
    TokenType::LeftShift, TokenType::RightShift, TokenType::UnsignedRightShift,
    TokenType::Ampersand, TokenType::Pipe, TokenType::Caret, TokenType::Tilde,
    TokenType::AmpersandAmpersand, TokenType::PipePipe, TokenType::Exclamation,
    TokenType::Question, TokenType::QuestionQuestion, TokenType::QuestionDot,
    TokenType::Equal, TokenType::PlusEqual, TokenType::MinusEqual,
    TokenType::StarEqual, TokenType::SlashEqual, TokenType::PercentEqual,
    TokenType::LeftShiftEqual, TokenType::RightShiftEqual,
    TokenType::UnsignedRightShiftEqual, TokenType::AmpersandEqual,
    TokenType::PipeEqual, TokenType::CaretEqual, TokenType::StarStarEqual,
    TokenType::QuestionQuestionEqual, TokenType::PipePipeEqual,
    TokenType::AmpersandAmpersandEqual, TokenType::Arrow, TokenType::PlusPlus,
    TokenType::MinusMinus, TokenType::In, TokenType::Instanceof, TokenType::Typeof,
    TokenType::Void, TokenType::Delete
};

const std::unordered_set<TokenType> ASSIGNMENT_OPERATOR_TOKENS = {
    TokenType::Equal, TokenType::PlusEqual, TokenType::MinusEqual,
    TokenType::StarEqual, TokenType::SlashEqual, TokenType::PercentEqual,
    TokenType::LeftShiftEqual, TokenType::RightShiftEqual,
    TokenType::UnsignedRightShiftEqual, TokenType::AmpersandEqual,
    TokenType::PipeEqual, TokenType::CaretEqual, TokenType::StarStarEqual,
    TokenType::QuestionQuestionEqual, TokenType::PipePipeEqual,
    TokenType::AmpersandAmpersandEqual
};

const std::unordered_map<TokenType, int> OPERATOR_PRECEDENCE = {
    // Assignment operators (lowest precedence)
    {TokenType::Equal, 1},
    {TokenType::PlusEqual, 1}, {TokenType::MinusEqual, 1},
    {TokenType::StarEqual, 1}, {TokenType::SlashEqual, 1},
    {TokenType::PercentEqual, 1}, {TokenType::LeftShiftEqual, 1},
    {TokenType::RightShiftEqual, 1}, {TokenType::UnsignedRightShiftEqual, 1},
    {TokenType::AmpersandEqual, 1}, {TokenType::PipeEqual, 1},
    {TokenType::CaretEqual, 1}, {TokenType::StarStarEqual, 1},
    {TokenType::QuestionQuestionEqual, 1}, {TokenType::PipePipeEqual, 1},
    {TokenType::AmpersandAmpersandEqual, 1},
    
    // Conditional operators
    {TokenType::QuestionQuestion, 2},
    
    // Logical OR
    {TokenType::PipePipe, 3},
    
    // Logical AND
    {TokenType::AmpersandAmpersand, 4},
    
    // Bitwise OR
    {TokenType::Pipe, 5},
    
    // Bitwise XOR
    {TokenType::Caret, 6},
    
    // Bitwise AND
    {TokenType::Ampersand, 7},
    
    // Equality operators
    {TokenType::EqualEqual, 8}, {TokenType::NotEqual, 8},
    {TokenType::EqualEqualEqual, 8}, {TokenType::NotEqualEqual, 8},
    
    // Relational operators
    {TokenType::Less, 9}, {TokenType::Greater, 9},
    {TokenType::LessEqual, 9}, {TokenType::GreaterEqual, 9},
    {TokenType::In, 9}, {TokenType::Instanceof, 9},
    
    // Bitwise shift
    {TokenType::LeftShift, 10}, {TokenType::RightShift, 10},
    {TokenType::UnsignedRightShift, 10},
    
    // Addition and subtraction
    {TokenType::Plus, 11}, {TokenType::Minus, 11},
    
    // Multiplication, division, and remainder
    {TokenType::Star, 12}, {TokenType::Slash, 12}, {TokenType::Percent, 12},
    
    // Exponentiation (highest precedence)
    {TokenType::StarStar, 13}
};

} // namespace tsc