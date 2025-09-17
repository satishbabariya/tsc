#pragma once

#include "tsc/Common.h"
#include "tsc/utils/DiagnosticEngine.h"

namespace tsc {

struct CompilerOptions {
    bool debug = false;
    bool optimize = false;
    bool verbose = false;
    String targetTriple = "x86_64-unknown-linux-gnu";
    String cpu = "generic";
    String features = "";
};

class Compiler {
public:
    Compiler();
    ~Compiler();
    
    // Main compilation interface
    bool compile(const String& inputFile, const String& outputFile, const CompilerOptions& options = {});
    
    // Utility methods
    String readSourceFile(const String& filename);
    
    // Accessors
    DiagnosticEngine& getDiagnostics();
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
};

} // namespace tsc