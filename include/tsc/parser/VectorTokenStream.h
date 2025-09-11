#pragma once

#include "tsc/Token.h"
#include <vector>

namespace tsc {

// Simple token stream implementation that wraps a vector of tokens
class VectorTokenStream : public TokenStream {
public:
    explicit VectorTokenStream(std::vector<Token> tokens);
    
    // TokenStream interface
    Token peek() const override;
    Token advance() override;
    bool isAtEnd() const override;
    
    // Lookahead interface
    Token peekAhead(size_t offset) const override;
    bool hasAhead(size_t offset) const override;
    
    // Error recovery
    void synchronize() override;
    void skipUntil(TokenType type) override;
    void skipUntil(std::initializer_list<TokenType> types) override;

private:
    std::vector<Token> tokens_;
    size_t current_ = 0;
};

} // namespace tsc