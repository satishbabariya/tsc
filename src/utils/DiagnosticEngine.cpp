#include "tsc/utils/DiagnosticEngine.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace tsc {

DiagnosticEngine::DiagnosticEngine() = default;
DiagnosticEngine::~DiagnosticEngine() = default;

void DiagnosticEngine::error(const String& message, const SourceLocation& location, const String& code) {
    addDiagnostic(DiagnosticLevel::Error, message, location, code);
}

void DiagnosticEngine::warning(const String& message, const SourceLocation& location, const String& code) {
    DiagnosticLevel level = DiagnosticLevel::Warning;
    if (warningsAsErrors_ || isWarningPromotedToError(code)) {
        level = DiagnosticLevel::Error;
    }
    addDiagnostic(level, message, location, code);
}

void DiagnosticEngine::note(const String& message, const SourceLocation& location, const String& code) {
    addDiagnostic(DiagnosticLevel::Note, message, location, code);
}

void DiagnosticEngine::fatal(const String& message, const SourceLocation& location, const String& code) {
    addDiagnostic(DiagnosticLevel::Fatal, message, location, code);
    throw CompilerError(message, location);
}

void DiagnosticEngine::clearDiagnostics() {
    diagnostics_.clear();
    errorCount_ = 0;
    warningCount_ = 0;
}

void DiagnosticEngine::suppressWarning(const String& code) {
    suppressedWarnings_.insert(code);
}

void DiagnosticEngine::promoteWarningToError(const String& code) {
    warningsPromotedToError_.insert(code);
}

bool DiagnosticEngine::isWarningSuppressed(const String& code) const {
    return suppressedWarnings_.count(code) > 0;
}

bool DiagnosticEngine::isWarningPromotedToError(const String& code) const {
    return warningsPromotedToError_.count(code) > 0;
}

String DiagnosticEngine::formatDiagnostic(const Diagnostic& diagnostic) const {
    std::ostringstream ss;
    
    // Format: filename:line:column: level: message [code]
    if (diagnostic.location.isValid()) {
        ss << diagnostic.location.getFilename() 
           << ":" << diagnostic.location.getLine()
           << ":" << diagnostic.location.getColumn() << ": ";
    }
    
    ss << getLevelString(diagnostic.level) << ": " << diagnostic.message;
    
    if (!diagnostic.code.empty()) {
        ss << " [" << diagnostic.code << "]";
    }
    
    // Add source context if available
    String context = getContextLines(diagnostic.location);
    if (!context.empty()) {
        ss << "\n" << context;
    }
    
    return ss.str();
}

void DiagnosticEngine::printDiagnostics() const {
    for (const auto& diagnostic : diagnostics_) {
        std::cerr << formatDiagnostic(diagnostic) << std::endl;
    }
}

void DiagnosticEngine::printSummary() const {
    if (errorCount_ > 0 || warningCount_ > 0) {
        std::cerr << "\nCompilation summary: ";
        if (errorCount_ > 0) {
            std::cerr << errorCount_ << " error(s)";
            if (warningCount_ > 0) {
                std::cerr << ", ";
            }
        }
        if (warningCount_ > 0) {
            std::cerr << warningCount_ << " warning(s)";
        }
        std::cerr << std::endl;
    }
}

void DiagnosticEngine::setSourceContent(const String& filename, const String& content) {
    std::vector<String> lines;
    std::istringstream stream(content);
    String line;
    
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    
    sourceLines_[filename] = std::move(lines);
}

String DiagnosticEngine::getSourceLine(const String& filename, unsigned line) const {
    auto it = sourceLines_.find(filename);
    if (it == sourceLines_.end() || line == 0 || line > it->second.size()) {
        return " + ";
    }
    return it->second[line - 1]; // Convert to 0-based indexing
}

void DiagnosticEngine::addDiagnostic(DiagnosticLevel level, const String& message,
                                   const SourceLocation& location, const String& code) {
    if (shouldSuppressDiagnostic(level, code)) {
        return;
    }
    
    diagnostics_.emplace_back(level, message, location, code);
    
    if (level == DiagnosticLevel::Error || level == DiagnosticLevel::Fatal) {
        errorCount_++;
        if (errorCount_ >= maxErrors_) {
            // Prevent infinite recursion by directly adding the fatal diagnostic
            // instead of calling fatal() which would call addDiagnostic() again
            diagnostics_.emplace_back(DiagnosticLevel::Fatal, 
                                    "Too many errors, stopping compilation", location);
            errorCount_++;
            throw CompilerError("Too many errors, stopping compilation", location);
        }
    } else if (level == DiagnosticLevel::Warning) {
        warningCount_++;
    }
}

String DiagnosticEngine::getLevelString(DiagnosticLevel level) const {
    switch (level) {
        case DiagnosticLevel::Note: return "note";
        case DiagnosticLevel::Warning: return "warning";
        case DiagnosticLevel::Error: return "error";
        case DiagnosticLevel::Fatal: return "fatal error";
        default: return "unknown";
    }
}

String DiagnosticEngine::getContextLines(const SourceLocation& location) const {
    if (!location.isValid()) {
        return " + ";
    }
    
    String sourceLine = getSourceLine(location.getFilename(), location.getLine());
    if (sourceLine.empty()) {
        return "";
    }
    
    std::ostringstream ss;
    ss << sourceLine << "\n";
    
    // Add caret pointing to the error location
    for (unsigned i = 1; i < location.getColumn(); ++i) {
        ss << " + ";
    }
    ss << "^";
    
    return ss.str();
}

bool DiagnosticEngine::shouldSuppressDiagnostic(DiagnosticLevel level, const String& code) const {
    if (level == DiagnosticLevel::Warning && suppressWarnings_) {
        return true;
    }
    
    if (level == DiagnosticLevel::Warning && isWarningSuppressed(code)) {
        return true;
    }
    
    return false;
}

// DiagnosticBuilder implementation
DiagnosticBuilder::DiagnosticBuilder(DiagnosticEngine& engine, DiagnosticLevel level,
                                   const String& message, const SourceLocation& location)
    : engine_(engine), diagnostic_(level, message, location) {}

DiagnosticBuilder& DiagnosticBuilder::withCode(const String& code) {
    diagnostic_.code = code;
    return *this;
}

DiagnosticBuilder& DiagnosticBuilder::addRelatedLocation(const SourceLocation& location) {
    diagnostic_.relatedLocations.push_back(location);
    return *this;
}

DiagnosticBuilder& DiagnosticBuilder::addNote(const String& note, const SourceLocation& location) {
    notes_.emplace_back(DiagnosticLevel::Note, note, location);
    return *this;
}

void DiagnosticBuilder::emit() {
    engine_.addDiagnostic(diagnostic_.level, diagnostic_.message, 
                         diagnostic_.location, diagnostic_.code);
    
    // Emit related notes
    for (const auto& note : notes_) {
        engine_.addDiagnostic(note.level, note.message, note.location, note.code);
    }
}

} // namespace tsc