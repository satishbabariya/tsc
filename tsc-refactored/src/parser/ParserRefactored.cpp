#include "tsc/parser/ParserRefactored.h"
#include "tsc/parser/core/ParserCore.h"

namespace tsc {
namespace parser {

Parser::Parser(DiagnosticEngine& diagnostics, const TypeSystem& typeSystem)
    : diagnostics_(diagnostics), enhancedDiagnostics_(nullptr), typeSystem_(typeSystem) {
    core_ = std::make_unique<ParserCore>(diagnostics, typeSystem);
}

Parser::Parser(utils::EnhancedDiagnosticEngine& enhancedDiagnostics, const TypeSystem& typeSystem)
    : diagnostics_(enhancedDiagnostics), enhancedDiagnostics_(&enhancedDiagnostics), typeSystem_(typeSystem) {
    core_ = std::make_unique<ParserCore>(diagnostics, typeSystem);
}

Parser::~Parser() = default;

std::unique_ptr<Module> Parser::parse(const std::vector<Token>& tokens, const String& filename) {
    auto tokenStream = std::make_unique<VectorTokenStream>(tokens);
    return parse(std::move(tokenStream), filename);
}

std::unique_ptr<Module> Parser::parse(std::unique_ptr<TokenStream> tokenStream, const String& filename) {
    try {
        core_->setTokenStream(std::move(tokenStream), filename);
        return core_->parseModule();
    } catch (const CompilerError& e) {
        diagnostics_.error(e.what(), e.getLocation());
        errorCount_++;
        return nullptr;
    }
}

bool Parser::hasErrors() const {
    return errorCount_ > 0;
}

size_t Parser::getErrorCount() const {
    return errorCount_;
}

} // namespace parser
} // namespace tsc