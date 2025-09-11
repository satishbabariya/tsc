#include "tsc/parser/VectorTokenStream.h"

namespace tsc {

VectorTokenStream::VectorTokenStream(std::vector<Token> tokens) 
    : tokens_(std::move(tokens)) {
    // Ensure we have an EOF token at the end
    if (tokens_.empty() || tokens_.back().getType() != TokenType::EndOfInput) {
        tokens_.emplace_back(TokenType::EndOfInput, SourceLocation(), "");
    }
}

Token VectorTokenStream::peek() const {
    if (current_ >= tokens_.size()) {
        return Token(TokenType::EndOfInput, SourceLocation(), "");
    }
    return tokens_[current_];
}

Token VectorTokenStream::advance() {
    if (current_ >= tokens_.size()) {
        return Token(TokenType::EndOfInput, SourceLocation(), "");
    }
    return tokens_[current_++];
}

bool VectorTokenStream::isAtEnd() const {
    return current_ >= tokens_.size() || 
           tokens_[current_].getType() == TokenType::EndOfInput;
}

Token VectorTokenStream::peekAhead(size_t offset) const {
    size_t target = current_ + offset;
    if (target >= tokens_.size()) {
        return Token(TokenType::EndOfInput, SourceLocation(), "");
    }
    return tokens_[target];
}

bool VectorTokenStream::hasAhead(size_t offset) const {
    return (current_ + offset) < tokens_.size();
}

void VectorTokenStream::synchronize() {
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

void VectorTokenStream::skipUntil(TokenType type) {
    while (!isAtEnd() && peek().getType() != type) {
        advance();
    }
}

void VectorTokenStream::skipUntil(std::initializer_list<TokenType> types) {
    while (!isAtEnd() && !peek().isOneOf(types)) {
        advance();
    }
}

} // namespace tsc