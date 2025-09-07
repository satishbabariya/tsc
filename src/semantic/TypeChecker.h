#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

namespace tsc {

class DiagnosticEngine;

class TypeChecker {
public:
    explicit TypeChecker(DiagnosticEngine& diagnostics);
    
    bool check(Module& module);

private:
    DiagnosticEngine& diagnostics_;
};

} // namespace tsc