#include "tsc/utils/DiagnosticEngine.h"
#include <iostream>

namespace tsc {

DiagnosticEngine::DiagnosticEngine() 
    : errorCount_(0), warningCount_(0) {
}

void DiagnosticEngine::error(const String& message, const SourceLocation& location) {
    std::cerr << "Error";
    if (location >= 0) {
        std::cerr << " at line " << location;
    }
    std::cerr << ": " << message << std::endl;
    errorCount_++;
}

void DiagnosticEngine::warning(const String& message, const SourceLocation& location) {
    std::cerr << "Warning";
    if (location >= 0) {
        std::cerr << " at line " << location;
    }
    std::cerr << ": " << message << std::endl;
    warningCount_++;
}

void DiagnosticEngine::info(const String& message, const SourceLocation& location) {
    std::cout << "Info";
    if (location >= 0) {
        std::cout << " at line " << location;
    }
    std::cout << ": " << message << std::endl;
}

int DiagnosticEngine::getErrorCount() const {
    return errorCount_;
}

int DiagnosticEngine::getWarningCount() const {
    return warningCount_;
}

bool DiagnosticEngine::hasErrors() const {
    return errorCount_ > 0;
}

void DiagnosticEngine::reset() {
    errorCount_ = 0;
    warningCount_ = 0;
}

} // namespace tsc