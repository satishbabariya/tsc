#pragma once

#include "tsc/Common.h"
#include "tsc/AST.h"

namespace tsc {

// Forward declarations
class Lexer;
class Parser;
class TypeChecker;
class CodeGenerator;
class DiagnosticEngine;

// Compilation phases
enum class CompilationPhase {
    Lexical,     // Tokenization
    Syntax,      // Parsing to AST
    Semantic,    // Type checking and symbol resolution
    CodeGen,     // LLVM IR generation
    Optimization,// LLVM optimization passes
    Linking      // Final binary generation
};

// Compilation result
struct CompilationResult {
    bool success = false;
    String errorMessage;
    CompilationPhase failedPhase = CompilationPhase::Lexical;
    std::vector<String> warnings;
    
    // Outputs
    unique_ptr<Module> ast;
    String llvmIR;
    String objectFile;
    String executable;
};

// Main compiler class following the 4-phase pipeline
class Compiler {
public:
    explicit Compiler(const CompilerOptions& options);
    ~Compiler();
    
    // Main compilation interface
    CompilationResult compile(const String& sourceFile);
    CompilationResult compileString(const String& source, const String& filename = "<string>");
    
    // Multi-file compilation for modules
    CompilationResult compileModule(const std::vector<String>& sourceFiles);
    
    // Individual phase access for testing and debugging
    std::vector<Token> tokenize(const String& source, const String& filename);
    unique_ptr<Module> parse(const std::vector<Token>& tokens, const String& filename);
    bool typeCheck(Module& module);
    String generateLLVMIR(const Module& module);
    bool generateObjectFile(const String& llvmIR, const String& outputFile);
    bool linkExecutable(const std::vector<String>& objectFiles, const String& outputFile);
    
    // Configuration
    const CompilerOptions& getOptions() const { return options_; }
    void setOptions(const CompilerOptions& options) { options_ = options; }
    
    // Diagnostics
    DiagnosticEngine& getDiagnostics() const { return *diagnostics_; }
    
    // Target information
    void setTarget(const TargetInfo& target);
    const TargetInfo& getTarget() const { return options_.target; }

private:
    CompilerOptions options_;
    unique_ptr<DiagnosticEngine> diagnostics_;
    unique_ptr<Lexer> lexer_;
    unique_ptr<Parser> parser_;
    unique_ptr<TypeChecker> typeChecker_;
    unique_ptr<CodeGenerator> codeGenerator_;
    
    // LLVM context and related objects
    void initializeLLVM();
    void shutdownLLVM();
    
    // Helper methods
    bool setupTarget();
    void reportPhaseCompletion(CompilationPhase phase, const String& message);
    void reportPhaseError(CompilationPhase phase, const String& error);
};

// Utility functions
String getDefaultTargetTriple();
std::vector<String> getSupportedTargets();
bool isTargetSupported(const String& triple);

// Version information
struct VersionInfo {
    int major = 1;
    int minor = 0;
    int patch = 0;
    String buildDate = __DATE__;
    String buildTime = __TIME__;
    String llvmVersion;
    String gitCommit = "unknown";
};

VersionInfo getVersionInfo();
String getVersionString();

} // namespace tsc