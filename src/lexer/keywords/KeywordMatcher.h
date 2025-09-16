#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"

#include <unordered_map>

namespace tsc {
namespace lexer {

// Keyword matching engine - handles keyword recognition
class KeywordMatcher {
public:
    KeywordMatcher();
    
    // Main interface
    TokenType matchKeyword(const String& text) const;
    bool isKeyword(const String& text) const;
    
    // Keyword categories
    bool isReservedKeyword(const String& text) const;
    bool isFutureReservedKeyword(const String& text) const;
    bool isSoftKeyword(const String& text) const;
    bool isTypeScriptKeyword(const String& text) const;
    bool isLiteralKeyword(const String& text) const;
    
    // Context-sensitive keywords
    bool isContextualKeyword(const String& text) const;
    TokenType getContextualKeywordType(const String& text) const;

private:
    // Keyword tables
    std::unordered_map<String, TokenType> reservedKeywords_;
    std::unordered_map<String, TokenType> futureReservedKeywords_;
    std::unordered_map<String, TokenType> softKeywords_;
    std::unordered_map<String, TokenType> typeScriptKeywords_;
    std::unordered_map<String, TokenType> literalKeywords_;
    std::unordered_map<String, TokenType> contextualKeywords_;
    
    // Initialization
    void initializeReservedKeywords();
    void initializeFutureReservedKeywords();
    void initializeSoftKeywords();
    void initializeTypeScriptKeywords();
    void initializeLiteralKeywords();
    void initializeContextualKeywords();
};

} // namespace lexer
} // namespace tsc