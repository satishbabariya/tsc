#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"
#include "tsc/semantic/SemanticAnalyzer.h"

namespace tsc {

class DiagnosticEngine;

// TypeChecker is now a wrapper around SemanticAnalyzer for backward compatibility
class TypeChecker {
public:
    explicit TypeChecker(DiagnosticEngine& diagnostics);
    ~TypeChecker();
    
    bool check(Module& module);
    
    // Access to semantic analysis results
    const SemanticAnalyzer& getSemanticAnalyzer() const { return *analyzer_; }
    const SymbolTable& getSymbolTable() const { return analyzer_->getSymbolTable(); }
    const TypeSystem& getTypeSystem() const { return analyzer_->getTypeSystem(); }

private:
    DiagnosticEngine& diagnostics_;
    unique_ptr<SemanticAnalyzer> analyzer_;
};

} // namespace tsc