#include "tsc/Token.h"
#include <sstream>

namespace tsc {

bool Token::isKeyword() const {
    return static_cast<int>(type_) >= static_cast<int>(TokenType::Await) &&
           static_cast<int>(type_) <= static_cast<int>(TokenType::Infer);
}

bool Token::isLiteral() const {
    switch (type_) {
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

bool Token::isPunctuation() const {
    return static_cast<int>(type_) >= static_cast<int>(TokenType::LeftBrace) &&
           static_cast<int>(type_) <= static_cast<int>(TokenType::AmpersandAmpersandEqual);
}

bool Token::isOperator() const {
    return isBinaryOperator() || isUnaryOperator() || isAssignmentOperator();
}

bool Token::isBinaryOperator() const {
    switch (type_) {
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
        case TokenType::Ampersand:
        case TokenType::Pipe:
        case TokenType::Caret:
        case TokenType::AmpersandAmpersand:
        case TokenType::PipePipe:
        case TokenType::QuestionQuestion:
        case TokenType::LeftShift:
        case TokenType::RightShift:
        case TokenType::UnsignedRightShift:
        case TokenType::In:
        case TokenType::Instanceof:
        case TokenType::As:
        case TokenType::Satisfies:
            return true;
        default:
            return false;
    }
}

bool Token::isUnaryOperator() const {
    switch (type_) {
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Exclamation:
        case TokenType::Tilde:
        case TokenType::PlusPlus:
        case TokenType::MinusMinus:
        case TokenType::Typeof:
        case TokenType::Delete:
        case TokenType::Void:
        case TokenType::Await:
            return true;
        default:
            return false;
    }
}

bool Token::isAssignmentOperator() const {
    switch (type_) {
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

String Token::toString() const {
    std::stringstream ss;
    ss << tokenTypeToString(type_);
    
    if (std::holds_alternative<String>(value_)) {
        ss << "(" << std::get<String>(value_) << ")";
    } else if (std::holds_alternative<double>(value_)) {
        ss << "(" << std::get<double>(value_) << ")";
    } else if (std::holds_alternative<bool>(value_)) {
        ss << "(" << (std::get<bool>(value_) ? "true" : "false") << ")";
    }
    
    return ss.str();
}

bool TokenStream::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool TokenStream::match(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token TokenStream::consume(TokenType type, const String& errorMessage) {
    if (check(type)) {
        return advance();
    }
    throw CompilerError(errorMessage, peek().getLocation());
}

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::EndOfInput: return "EndOfInput";
        case TokenType::Identifier: return "Identifier";
        case TokenType::PrivateIdentifier: return "PrivateIdentifier";
        case TokenType::NumericLiteral: return "NumericLiteral";
        case TokenType::StringLiteral: return "StringLiteral";
        case TokenType::NoSubstitutionTemplate: return "NoSubstitutionTemplate";
        case TokenType::TemplateHead: return "TemplateHead";
        case TokenType::TemplateMiddle: return "TemplateMiddle";
        case TokenType::TemplateTail: return "TemplateTail";
        case TokenType::RegularExpressionLiteral: return "RegularExpressionLiteral";
        
        // Keywords
        case TokenType::Await: return "await";
        case TokenType::Break: return "break";
        case TokenType::Case: return "case";
        case TokenType::Catch: return "catch";
        case TokenType::Class: return "class";
        case TokenType::Const: return "const";
        case TokenType::Continue: return "continue";
        case TokenType::Debugger: return "debugger";
        case TokenType::Default: return "default";
        case TokenType::Delete: return "delete";
        case TokenType::Do: return "do";
        case TokenType::Else: return "else";
        case TokenType::Export: return "export";
        case TokenType::Extends: return "extends";
        case TokenType::Finally: return "finally";
        case TokenType::For: return "for";
        case TokenType::Function: return "function";
        case TokenType::If: return "if";
        case TokenType::Import: return "import";
        case TokenType::In: return "in";
        case TokenType::Instanceof: return "instanceof";
        case TokenType::New: return "new";
        case TokenType::Return: return "return";
        case TokenType::Super: return "super";
        case TokenType::Switch: return "switch";
        case TokenType::This: return "this";
        case TokenType::Throw: return "throw";
        case TokenType::Try: return "try";
        case TokenType::Typeof: return "typeof";
        case TokenType::Var: return "var";
        case TokenType::Void: return "void";
        case TokenType::While: return "while";
        case TokenType::With: return "with";
        case TokenType::Yield: return "yield";
        case TokenType::Enum: return "enum";
        case TokenType::Null: return "null";
        case TokenType::True: return "true";
        case TokenType::False: return "false";
        
        // Soft keywords
        case TokenType::As: return "as";
        case TokenType::Assert: return "assert";
        case TokenType::Asserts: return "asserts";
        case TokenType::Async: return "async";
        case TokenType::From: return "from";
        case TokenType::Get: return "get";
        case TokenType::Let: return "let";
        case TokenType::Of: return "of";
        case TokenType::Set: return "set";
        case TokenType::Static: return "static";
        case TokenType::Target: return "target";
        case TokenType::Satisfies: return "satisfies";
        case TokenType::Out: return "out";
        
        // TypeScript keywords
        case TokenType::Implements: return "implements";
        case TokenType::Interface: return "interface";
        case TokenType::Private: return "private";
        case TokenType::Protected: return "protected";
        case TokenType::Public: return "public";
        case TokenType::Any: return "any";
        case TokenType::Unknown: return "unknown";
        case TokenType::Boolean: return "boolean";
        case TokenType::Number: return "number";
        case TokenType::String: return "string";
        case TokenType::Symbol: return "symbol";
        case TokenType::Bigint: return "bigint";
        case TokenType::Undefined: return "undefined";
        case TokenType::Never: return "never";
        case TokenType::Object: return "object";
        case TokenType::Abstract: return "abstract";
        case TokenType::Constructor: return "constructor";
        case TokenType::Declare: return "declare";
        case TokenType::Global: return "global";
        case TokenType::Is: return "is";
        case TokenType::Module: return "module";
        case TokenType::Namespace: return "namespace";
        case TokenType::Override: return "override";
        case TokenType::Require: return "require";
        case TokenType::Type: return "type";
        case TokenType::Accessor: return "accessor";
        case TokenType::Readonly: return "readonly";
        case TokenType::Keyof: return "keyof";
        case TokenType::Unique: return "unique";
        case TokenType::Infer: return "infer";
        
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
        case TokenType::Less: return "<";
        case TokenType::Greater: return ">";
        case TokenType::LessEqual: return "<=";
        case TokenType::GreaterEqual: return ">=";
        case TokenType::EqualEqual: return "==";
        case TokenType::NotEqual: return "!=";
        case TokenType::EqualEqualEqual: return "===";
        case TokenType::NotEqualEqual: return "!==";
        case TokenType::At: return "@";
        case TokenType::Plus: return "+";
        case TokenType::Minus: return "-";
        case TokenType::Star: return "*";
        case TokenType::Slash: return "/";
        case TokenType::Percent: return "%";
        case TokenType::PlusPlus: return "++";
        case TokenType::MinusMinus: return "--";
        case TokenType::LeftShift: return "<<";
        case TokenType::RightShift: return ">>";
        case TokenType::UnsignedRightShift: return ">>>";
        case TokenType::Ampersand: return "&";
        case TokenType::Pipe: return "|";
        case TokenType::Caret: return "^";
        case TokenType::Exclamation: return "!";
        case TokenType::Tilde: return "~";
        case TokenType::AmpersandAmpersand: return "&&";
        case TokenType::PipePipe: return "||";
        case TokenType::Question: return "?";
        case TokenType::QuestionQuestion: return "??";
        case TokenType::QuestionDot: return "?.";
        case TokenType::Colon: return ":";
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
        case TokenType::Arrow: return "=>";
        case TokenType::StarStar: return "**";
        case TokenType::StarStarEqual: return "**=";
        case TokenType::QuestionQuestionEqual: return "??=";
        case TokenType::PipePipeEqual: return "||=";
        case TokenType::AmpersandAmpersandEqual: return "&&=";
        
        // Comments and whitespace
        case TokenType::SingleLineComment: return "SingleLineComment";
        case TokenType::MultiLineComment: return "MultiLineComment";
        case TokenType::WhiteSpace: return "WhiteSpace";
        
        // Error tokens
        case TokenType::InvalidToken: return "InvalidToken";
        case TokenType::InsertedSemicolon: return "InsertedSemicolon";
        
        default: return "Unknown";
    }
}

bool isKeywordToken(TokenType type) {
    return static_cast<int>(type) >= static_cast<int>(TokenType::Await) &&
           static_cast<int>(type) <= static_cast<int>(TokenType::Infer);
}

bool isLiteralToken(TokenType type) {
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

bool isPunctuationToken(TokenType type) {
    return static_cast<int>(type) >= static_cast<int>(TokenType::LeftBrace) &&
           static_cast<int>(type) <= static_cast<int>(TokenType::AmpersandAmpersandEqual);
}

} // namespace tsc