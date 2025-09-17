#include "tsc/semantic/TypeChecker.h"
#include "tsc/utils/DiagnosticEngine.h"

namespace tsc {
    TypeChecker::TypeChecker(DiagnosticEngine &diagnostics) : diagnostics_(diagnostics) {
        analyzer_ = createSemanticAnalyzer(diagnostics_);
    }

    TypeChecker::~TypeChecker() = default;

    bool TypeChecker::check(Module &module) {
        return analyzer_->analyze(module);
    }
} // namespace tsc
