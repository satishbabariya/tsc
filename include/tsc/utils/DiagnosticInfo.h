#pragma once

#include "tsc/Common.h"
#include "tsc/Token.h"

namespace tsc {

enum class DiagnosticSeverity {
    Error,
    Warning,
    Info,
    Note
};

struct DiagnosticInfo {
    String message;
    SourceLocation location;
    String context;
    String suggestion;
    DiagnosticSeverity severity;
    std::vector<SourceLocation> relatedLocations;
    
    DiagnosticInfo(const String& msg, const SourceLocation& loc, DiagnosticSeverity sev = DiagnosticSeverity::Error)
        : message(msg), location(loc), severity(sev) {}
    
    DiagnosticInfo(const String& msg, const SourceLocation& loc, const String& ctx, 
                   const String& sugg = "", DiagnosticSeverity sev = DiagnosticSeverity::Error)
        : message(msg), location(loc), context(ctx), suggestion(sugg), severity(sev) {}
};

class EnhancedDiagnosticEngine {
public:
    void report(const DiagnosticInfo& info);
    void error(const String& message, const SourceLocation& location);
    void warning(const String& message, const SourceLocation& location);
    void info(const String& message, const SourceLocation& location);
    void note(const String& message, const SourceLocation& location);
    
    // Enhanced error reporting with context and suggestions
    void reportError(const String& message, const SourceLocation& location, 
                    const String& context = "", const String& suggestion = "");
    void reportWarning(const String& message, const SourceLocation& location, 
                      const String& context = "", const String& suggestion = "");
    
    // Error counting and retrieval
    size_t getErrorCount() const;
    size_t getWarningCount() const;
    const std::vector<DiagnosticInfo>& getDiagnostics() const;
    
    // Clear diagnostics
    void clear();

private:
    std::vector<DiagnosticInfo> diagnostics_;
};

} // namespace tsc