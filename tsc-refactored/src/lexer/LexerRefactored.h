#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"
#include "tsc/utils/DiagnosticEngine.h"

namespace tsc {
namespace lexer {

// Forward declarations
class LexerCore;

// Main lexer interface - orchestrates all lexing components
class Lexer {
public:
    explicit Lexer(DiagnosticEngine& diagnostics);
    ~Lexer();
    
    // Main interface
    void setSource(const String& source, const String& filename);
    Token nextToken();
    bool isAtEnd() const;
    SourceLocation getCurrentLocation() const;
    
    // Batch tokenization
    std::vector<Token> tokenize();
    std::vector<Token> tokenizeFile(const String& filename);
    
    // Error handling
    bool hasErrors() const;
    size_t getErrorCount() const;
    
    // Access to core components
    LexerCore* getCore() const { return core_.get(); }

private:
    DiagnosticEngine& diagnostics_;
    std::unique_ptr<LexerCore> core_;
    
    // Error tracking
    size_t errorCount_ = 0;
};

} // namespace lexer
} // namespace tsc