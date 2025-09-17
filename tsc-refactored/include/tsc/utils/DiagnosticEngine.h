#pragma once

#include "tsc/Common.h"

namespace tsc {

class DiagnosticEngine {
public:
    DiagnosticEngine();
    
    void error(const String& message, const SourceLocation& location = -1);
    void warning(const String& message, const SourceLocation& location = -1);
    void info(const String& message, const SourceLocation& location = -1);
    
    int getErrorCount() const;
    int getWarningCount() const;
    bool hasErrors() const;
    
    void reset();
    
private:
    int errorCount_;
    int warningCount_;
};

} // namespace tsc