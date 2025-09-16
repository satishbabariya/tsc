#include "tsc/lexer/LexerRefactored.h"
#include "tsc/lexer/core/LexerCore.h"

#include <fstream>

namespace tsc {
namespace lexer {

Lexer::Lexer(DiagnosticEngine& diagnostics) 
    : diagnostics_(diagnostics) {
    core_ = std::make_unique<LexerCore>(diagnostics);
}

Lexer::~Lexer() = default;

void Lexer::setSource(const String& source, const String& filename) {
    core_->setSource(source, filename);
}

Token Lexer::nextToken() {
    Token token = core_->nextToken();
    
    if (token.getType() == TokenType::Error) {
        errorCount_++;
        diagnostics_.error("Lexical error: " + token.getValue().getString(), token.getLocation());
    }
    
    return token;
}

bool Lexer::isAtEnd() const {
    return core_->isAtEnd();
}

SourceLocation Lexer::getCurrentLocation() const {
    return core_->getCurrentLocation();
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        Token token = nextToken();
        tokens.push_back(token);
        
        if (token.getType() == TokenType::Eof) {
            break;
        }
    }
    
    return tokens;
}

std::vector<Token> Lexer::tokenizeFile(const String& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        diagnostics_.error("Failed to open file: " + filename, SourceLocation());
        return {};
    }
    
    String content((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
    file.close();
    
    setSource(content, filename);
    return tokenize();
}

bool Lexer::hasErrors() const {
    return errorCount_ > 0;
}

size_t Lexer::getErrorCount() const {
    return errorCount_;
}

} // namespace lexer
} // namespace tsc