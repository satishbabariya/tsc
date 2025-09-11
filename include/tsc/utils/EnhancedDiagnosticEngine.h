#pragma once

#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/Token.h"
#include <vector>
#include <string>
#include <memory>

namespace tsc::utils {

// Enhanced diagnostic information structure
struct DiagnosticInfo {
    enum class Severity {
        Error,
        Warning,
        Info,
        Note
    };
    
    Severity severity;
    std::string message;
    SourceLocation location;
    std::string context;
    std::string suggestion;
    std::vector<SourceLocation> relatedLocations;
    
    DiagnosticInfo(Severity sev, const std::string& msg, const SourceLocation& loc,
                   const std::string& ctx = "", const std::string& sugg = "")
        : severity(sev), message(msg), location(loc), context(ctx), suggestion(sugg) {}
};

// Enhanced diagnostic engine with better error reporting
class EnhancedDiagnosticEngine : public DiagnosticEngine {
private:
    std::vector<DiagnosticInfo> diagnostics_;
    bool hasErrors_ = false;
    bool hasWarnings_ = false;
    
public:
    // Constructor
    EnhancedDiagnosticEngine() : DiagnosticEngine() {}
    // Report methods with enhanced information
    void error(const std::string& message, const SourceLocation& location,
               const std::string& context = "", const std::string& suggestion = "");
    void warning(const std::string& message, const SourceLocation& location,
                 const std::string& context = "", const std::string& suggestion = "");
    void info(const std::string& message, const SourceLocation& location,
              const std::string& context = "", const std::string& suggestion = "");
    void note(const std::string& message, const SourceLocation& location,
              const std::string& context = "", const std::string& suggestion = "");
    
    // Add related location to the last diagnostic
    void addRelatedLocation(const SourceLocation& location);
    
    // Query methods
    bool hasErrors() const { return hasErrors_; }
    bool hasWarnings() const { return hasWarnings_; }
    size_t getErrorCount() const;
    size_t getWarningCount() const;
    
    // Get all diagnostics
    const std::vector<DiagnosticInfo>& getDiagnostics() const { return diagnostics_; }
    
    // Clear diagnostics
    void clear();
    
    // Print diagnostics in a formatted way
    void printDiagnostics() const;
    
    // Get diagnostic summary
    std::string getSummary() const;
};

} // namespace tsc::utils