#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"
#include "tsc/parser/VectorTokenStream.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/semantic/TypeSystem.h"

namespace tsc {
namespace parser {

// Forward declarations
class ParserCore;

// Main parser interface - orchestrates all parsing components
class Parser {
public:
    explicit Parser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem);
    explicit Parser(utils::EnhancedDiagnosticEngine& enhancedDiagnostics, const TypeSystem& typeSystem);
    ~Parser();
    
    // Main parsing interface
    std::unique_ptr<Module> parse(const std::vector<Token>& tokens, const String& filename);
    std::unique_ptr<Module> parse(std::unique_ptr<TokenStream> tokenStream, const String& filename);
    
    // Error handling
    bool hasErrors() const;
    size_t getErrorCount() const;
    
    // Access to core components
    ParserCore* getCore() const { return core_.get(); }

private:
    DiagnosticEngine& diagnostics_;
    utils::EnhancedDiagnosticEngine* enhancedDiagnostics_;
    const TypeSystem& typeSystem_;
    
    std::unique_ptr<ParserCore> core_;
    
    // Error tracking
    size_t errorCount_ = 0;
};

} // namespace parser
} // namespace tsc