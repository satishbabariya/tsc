#include "tsc/utils/EnhancedDiagnosticEngine.h"
#include <iostream>
#include <iomanip>

namespace tsc::utils {

void EnhancedDiagnosticEngine::error(const std::string& message, const SourceLocation& location,
                                     const std::string& context, const std::string& suggestion) {
    diagnostics_.emplace_back(DiagnosticInfo::Severity::Error, message, location, context, suggestion);
    hasErrors_ = true;
}

void EnhancedDiagnosticEngine::warning(const std::string& message, const SourceLocation& location,
                                       const std::string& context, const std::string& suggestion) {
    diagnostics_.emplace_back(DiagnosticInfo::Severity::Warning, message, location, context, suggestion);
    hasWarnings_ = true;
}

void EnhancedDiagnosticEngine::info(const std::string& message, const SourceLocation& location,
                                    const std::string& context, const std::string& suggestion) {
    diagnostics_.emplace_back(DiagnosticInfo::Severity::Info, message, location, context, suggestion);
}

void EnhancedDiagnosticEngine::note(const std::string& message, const SourceLocation& location,
                                    const std::string& context, const std::string& suggestion) {
    diagnostics_.emplace_back(DiagnosticInfo::Severity::Note, message, location, context, suggestion);
}

void EnhancedDiagnosticEngine::addRelatedLocation(const SourceLocation& location) {
    if (!diagnostics_.empty()) {
        diagnostics_.back().relatedLocations.push_back(location);
    }
}

size_t EnhancedDiagnosticEngine::getErrorCount() const {
    size_t count = 0;
    for (const auto& diag : diagnostics_) {
        if (diag.severity == DiagnosticInfo::Severity::Error) {
            count++;
        }
    }
    return count;
}

size_t EnhancedDiagnosticEngine::getWarningCount() const {
    size_t count = 0;
    for (const auto& diag : diagnostics_) {
        if (diag.severity == DiagnosticInfo::Severity::Warning) {
            count++;
        }
    }
    return count;
}

void EnhancedDiagnosticEngine::clear() {
    diagnostics_.clear();
    hasErrors_ = false;
    hasWarnings_ = false;
}

void EnhancedDiagnosticEngine::printDiagnostics() const {
    for (const auto& diag : diagnostics_) {
        // Print severity
        std::string severityStr;
        switch (diag.severity) {
            case DiagnosticInfo::Severity::Error:
                severityStr = "error";
                break;
            case DiagnosticInfo::Severity::Warning:
                severityStr = "warning";
                break;
            case DiagnosticInfo::Severity::Info:
                severityStr = "info";
                break;
            case DiagnosticInfo::Severity::Note:
                severityStr = "note";
                break;
        }
        
        // Print location and message
        std::cout << diag.location.getFilename() << ":" 
                  << diag.location.getLine() << ":" 
                  << diag.location.getColumn() << ": " 
                  << severityStr << ": " << diag.message << std::endl;
        
        // Print context if available
        if (!diag.context.empty()) {
            std::cout << "  Context: " << diag.context << std::endl;
        }
        
        // Print suggestion if available
        if (!diag.suggestion.empty()) {
            std::cout << "  Suggestion: " << diag.suggestion << std::endl;
        }
        
        // Print related locations
        for (const auto& relatedLoc : diag.relatedLocations) {
            std::cout << "  Related: " << relatedLoc.getFilename() << ":" 
                      << relatedLoc.getLine() << ":" 
                      << relatedLoc.getColumn() << std::endl;
        }
        
        std::cout << std::endl;
    }
}

std::string EnhancedDiagnosticEngine::getSummary() const {
    size_t errorCount = getErrorCount();
    size_t warningCount = getWarningCount();
    
    std::string summary = "Compilation ";
    if (errorCount > 0) {
        summary += "failed with " + std::to_string(errorCount) + " error(s)";
        if (warningCount > 0) {
            summary += " and " + std::to_string(warningCount) + " warning(s)";
        }
    } else if (warningCount > 0) {
        summary += "succeeded with " + std::to_string(warningCount) + " warning(s)";
    } else {
        summary += "succeeded";
    }
    
    return summary;
}

} // namespace tsc::utils