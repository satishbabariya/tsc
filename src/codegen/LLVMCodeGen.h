#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

namespace tsc {

class DiagnosticEngine;

class CodeGenerator {
public:
    CodeGenerator(DiagnosticEngine& diagnostics, const CompilerOptions& options);
    
    String generate(const Module& module);

private:
    DiagnosticEngine& diagnostics_;
    const CompilerOptions& options_;
};

} // namespace tsc