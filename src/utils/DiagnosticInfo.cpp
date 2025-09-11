#include "tsc/utils/DiagnosticInfo.h"
#include <iostream>

namespace tsc {

void EnhancedDiagnosticEngine::report(const DiagnosticInfo& info) {
    diagnostics_.push_back(info);
    
    // Print to console for immediate feedback
    const char* severityStr = "ERROR";
    switch (info.severity) {
        case DiagnosticSeverity::Error: severityStr = "ERROR"; break;
        case DiagnosticSeverity::Warning: severityStr = "WARNING"; break;
        case DiagnosticSeverity::Info: severityStr = "INFO"; break;
        case DiagnosticSeverity::Note: severityStr = "NOTE"; break;
    }
    
    std::cout << severityStr << ": " << info.message;
    if (!info.context.empty()) {
        std::cout << " (" << info.context << ")";
    }
    if (!info.suggestion.empty()) {
        std::cout << " Suggestion: " << info.suggestion;
    }
    std::cout << " at " << info.location.toString() << std::endl;
}

void EnhancedDiagnosticEngine::error(const String& message, const SourceLocation& location) {
    report(DiagnosticInfo(message, location, DiagnosticSeverity::Error));
}

void EnhancedDiagnosticEngine::warning(const String& message, const SourceLocation& location) {
    report(DiagnosticInfo(message, location, DiagnosticSeverity::Warning));
}

void EnhancedDiagnosticEngine::info(const String& message, const SourceLocation& location) {
    report(DiagnosticInfo(message, location, DiagnosticSeverity::Info));
}

void EnhancedDiagnosticEngine::note(const String& message, const SourceLocation& location) {
    report(DiagnosticInfo(message, location, DiagnosticSeverity::Note));
}

void EnhancedDiagnosticEngine::reportError(const String& message, const SourceLocation& location, 
                                          const String& context, const String& suggestion) {
    report(DiagnosticInfo(message, location, context, suggestion, DiagnosticSeverity::Error));
}

void EnhancedDiagnosticEngine::reportWarning(const String& message, const SourceLocation& location, 
                                            const String& context, const String& suggestion) {
    report(DiagnosticInfo(message, location, context, suggestion, DiagnosticSeverity::Warning));
}

size_t EnhancedDiagnosticEngine::getErrorCount() const {
    size_t count = 0;
    for (const auto& diag : diagnostics_) {
        if (diag.severity == DiagnosticSeverity::Error) {
            count++;
        }
    }
    return count;
}

size_t EnhancedDiagnosticEngine::getWarningCount() const {
    size_t count = 0;
    for (const auto& diag : diagnostics_) {
        if (diag.severity == DiagnosticSeverity::Warning) {
            count++;
        }
    }
    return count;
}

const std::vector<DiagnosticInfo>& EnhancedDiagnosticEngine::getDiagnostics() const {
    return diagnostics_;
}

void EnhancedDiagnosticEngine::clear() {
    diagnostics_.clear();
}

} // namespace tsc