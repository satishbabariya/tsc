#include "tsc/lexer/keywords/KeywordMatcher.h"

namespace tsc {
namespace lexer {

KeywordMatcher::KeywordMatcher() {
    initializeReservedKeywords();
    initializeFutureReservedKeywords();
    initializeSoftKeywords();
    initializeTypeScriptKeywords();
    initializeLiteralKeywords();
    initializeContextualKeywords();
}

TokenType KeywordMatcher::matchKeyword(const String& text) const {
    // Check reserved keywords first
    auto it = reservedKeywords_.find(text);
    if (it != reservedKeywords_.end()) {
        return it->second;
    }
    
    // Check TypeScript keywords
    it = typeScriptKeywords_.find(text);
    if (it != typeScriptKeywords_.end()) {
        return it->second;
    }
    
    // Check soft keywords
    it = softKeywords_.find(text);
    if (it != softKeywords_.end()) {
        return it->second;
    }
    
    // Check literal keywords
    it = literalKeywords_.find(text);
    if (it != literalKeywords_.end()) {
        return it->second;
    }
    
    // Check contextual keywords
    it = contextualKeywords_.find(text);
    if (it != contextualKeywords_.end()) {
        return it->second;
    }
    
    return TokenType::Identifier;
}

bool KeywordMatcher::isKeyword(const String& text) const {
    return matchKeyword(text) != TokenType::Identifier;
}

bool KeywordMatcher::isReservedKeyword(const String& text) const {
    return reservedKeywords_.find(text) != reservedKeywords_.end();
}

bool KeywordMatcher::isFutureReservedKeyword(const String& text) const {
    return futureReservedKeywords_.find(text) != futureReservedKeywords_.end();
}

bool KeywordMatcher::isSoftKeyword(const String& text) const {
    return softKeywords_.find(text) != softKeywords_.end();
}

bool KeywordMatcher::isTypeScriptKeyword(const String& text) const {
    return typeScriptKeywords_.find(text) != typeScriptKeywords_.end();
}

bool KeywordMatcher::isLiteralKeyword(const String& text) const {
    return literalKeywords_.find(text) != literalKeywords_.end();
}

bool KeywordMatcher::isContextualKeyword(const String& text) const {
    return contextualKeywords_.find(text) != contextualKeywords_.end();
}

TokenType KeywordMatcher::getContextualKeywordType(const String& text) const {
    auto it = contextualKeywords_.find(text);
    return it != contextualKeywords_.end() ? it->second : TokenType::Identifier;
}

void KeywordMatcher::initializeReservedKeywords() {
    reservedKeywords_ = {
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
        {"yield", TokenType::Yield}
    };
}

void KeywordMatcher::initializeFutureReservedKeywords() {
    futureReservedKeywords_ = {
        {"enum", TokenType::Enum}
    };
}

void KeywordMatcher::initializeSoftKeywords() {
    softKeywords_ = {
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
        {"out", TokenType::Out}
    };
}

void KeywordMatcher::initializeTypeScriptKeywords() {
    typeScriptKeywords_ = {
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
}

void KeywordMatcher::initializeLiteralKeywords() {
    literalKeywords_ = {
        {"null", TokenType::Null},
        {"true", TokenType::True},
        {"false", TokenType::False}
    };
}

void KeywordMatcher::initializeContextualKeywords() {
    contextualKeywords_ = {
        // Add contextual keywords as needed
    };
}

} // namespace lexer
} // namespace tsc