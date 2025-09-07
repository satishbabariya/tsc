#pragma once

#include "tsc/Common.h"
#include <vector>
#include <unordered_set>

namespace tsc {

// Diagnostic message
struct Diagnostic {
    DiagnosticLevel level;
    String message;
    SourceLocation location;
    std::vector<SourceLocation> relatedLocations;
    String code; // Error/warning code (e.g., "TS2304")
    
    Diagnostic(DiagnosticLevel level, const String& message, 
               const SourceLocation& location, const String& code = "")
        : level(level), message(message), location(location), code(code) {}
};

// Diagnostic engine for error reporting and management
class DiagnosticEngine {
    friend class DiagnosticBuilder;
public:
    DiagnosticEngine();
    ~DiagnosticEngine();
    
    // Diagnostic reporting
    void error(const String& message, const SourceLocation& location, 
               const String& code = "");
    void warning(const String& message, const SourceLocation& location,
                 const String& code = "");
    void note(const String& message, const SourceLocation& location,
              const String& code = "");
    void fatal(const String& message, const SourceLocation& location,
               const String& code = "");
    
    // Diagnostic queries
    bool hasErrors() const { return errorCount_ > 0; }
    bool hasWarnings() const { return warningCount_ > 0; }
    size_t getErrorCount() const { return errorCount_; }
    size_t getWarningCount() const { return warningCount_; }
    size_t getTotalCount() const { return diagnostics_.size(); }
    
    // Diagnostic access
    const std::vector<Diagnostic>& getDiagnostics() const { return diagnostics_; }
    void clearDiagnostics();
    
    // Configuration
    void setMaxErrors(size_t maxErrors) { maxErrors_ = maxErrors; }
    void setWarningsAsErrors(bool warningsAsErrors) { warningsAsErrors_ = warningsAsErrors; }
    void setSuppressWarnings(bool suppressWarnings) { suppressWarnings_ = suppressWarnings; }
    
    // Diagnostic filtering
    void suppressWarning(const String& code);
    void promoteWarningToError(const String& code);
    bool isWarningSuppressed(const String& code) const;
    bool isWarningPromotedToError(const String& code) const;
    
    // Output formatting
    String formatDiagnostic(const Diagnostic& diagnostic) const;
    void printDiagnostics() const;
    void printSummary() const;
    
    // Source code context
    void setSourceContent(const String& filename, const String& content);
    String getSourceLine(const String& filename, unsigned line) const;
    
private:
    std::vector<Diagnostic> diagnostics_;
    size_t errorCount_ = 0;
    size_t warningCount_ = 0;
    size_t maxErrors_ = 100;
    bool warningsAsErrors_ = false;
    bool suppressWarnings_ = false;
    
    // Warning configuration
    std::unordered_set<String> suppressedWarnings_;
    std::unordered_set<String> warningsPromotedToError_;
    
    // Source code cache for context display
    std::unordered_map<String, std::vector<String>> sourceLines_;
    
    // Helper methods
    void addDiagnostic(DiagnosticLevel level, const String& message,
                      const SourceLocation& location, const String& code);
    String getLevelString(DiagnosticLevel level) const;
    String getContextLines(const SourceLocation& location) const;
    bool shouldSuppressDiagnostic(DiagnosticLevel level, const String& code) const;
};

// Diagnostic builder for complex diagnostics
class DiagnosticBuilder {
public:
    DiagnosticBuilder(DiagnosticEngine& engine, DiagnosticLevel level,
                     const String& message, const SourceLocation& location);
    
    // Fluent interface
    DiagnosticBuilder& withCode(const String& code);
    DiagnosticBuilder& addRelatedLocation(const SourceLocation& location);
    DiagnosticBuilder& addNote(const String& note, const SourceLocation& location = {});
    
    // Emit the diagnostic
    void emit();
    
private:
    DiagnosticEngine& engine_;
    Diagnostic diagnostic_;
    std::vector<Diagnostic> notes_;
};

// Convenience macros for common diagnostic patterns
#define TSC_ERROR(engine, message, location) \
    DiagnosticBuilder(engine, DiagnosticLevel::Error, message, location)

#define TSC_WARNING(engine, message, location) \
    DiagnosticBuilder(engine, DiagnosticLevel::Warning, message, location)

#define TSC_NOTE(engine, message, location) \
    DiagnosticBuilder(engine, DiagnosticLevel::Note, message, location)

} // namespace tsc