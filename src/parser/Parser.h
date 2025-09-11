#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/Token.h"

namespace tsc {

class DiagnosticEngine;

class Parser {
public:
    explicit Parser(DiagnosticEngine& diagnostics);
    
    unique_ptr<Module> parse(const std::vector<Token>& tokens, const String& filename);

private:
    DiagnosticEngine& diagnostics_;
};

} // namespace tsc