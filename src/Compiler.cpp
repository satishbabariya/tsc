#include "tsc/Compiler.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/parser/Parser.h"
#include "tsc/semantic/TypeChecker.h"
#include "tsc/codegen/LLVMCodeGen.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <cstdlib>  // For system() and std::remove()

// LLVM includes - simplified for now
// Full LLVM integration will be added in Phase 4

#include <fstream>
#include <iostream>
#include <algorithm>

namespace tsc {

// Parser, TypeChecker, and CodeGenerator are now implemented in separate files

Compiler::Compiler(const CompilerOptions& options) : options_(options) {
    // Initialize LLVM
    initializeLLVM();
    
    // Create subsystems
    diagnostics_ = make_unique<DiagnosticEngine>();
    lexer_ = make_unique<Lexer>(*diagnostics_);
    typeChecker_ = make_unique<TypeChecker>(*diagnostics_);
    parser_ = createParser(*diagnostics_, typeChecker_->getTypeSystem());
    codeGenerator_ = createLLVMCodeGen(*diagnostics_, options_);
    
    // Setup target
    if (!setupTarget()) {
        throw CompilerError("Failed to setup compilation target: " + options_.target.triple);
    }
}

Compiler::~Compiler() {
    shutdownLLVM();
}

CompilationResult Compiler::compile(const String& sourceFile) {
    CompilationResult result;
    
    try {
        // Read source file
        std::ifstream file(sourceFile);
        if (!file.is_open()) {
            result.errorMessage = "Cannot open source file: " + sourceFile;
            return result;
        }
        
        String source((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
        file.close();
        
        return compileString(source, sourceFile);
        
    } catch (const CompilerError& e) {
        result.errorMessage = e.what();
        return result;
    } catch (const std::exception& e) {
        result.errorMessage = "Internal compiler error: " + String(e.what());
        return result;
    }
}

CompilationResult Compiler::compileString(const String& source, const String& filename) {
    CompilationResult result;
    
    try {
        reportPhaseCompletion(CompilationPhase::Lexical, "Starting lexical analysis");
        
        // Phase 1: Lexical Analysis
        auto tokens = tokenize(source, filename);
        if (diagnostics_->hasErrors()) {
            result.failedPhase = CompilationPhase::Lexical;
            result.errorMessage = "Lexical analysis failed";
            return result;
        }
        
        reportPhaseCompletion(CompilationPhase::Syntax, "Starting syntax analysis");
        
        // Phase 2: Syntax Analysis
        result.ast = parse(tokens, filename);
        if (!result.ast || diagnostics_->hasErrors()) {
            result.failedPhase = CompilationPhase::Syntax;
            result.errorMessage = "Syntax analysis failed";
            return result;
        }
        
        reportPhaseCompletion(CompilationPhase::Semantic, "Starting semantic analysis");
        
        // Phase 3: Semantic Analysis
        if (!typeCheck(*result.ast)) {
            result.failedPhase = CompilationPhase::Semantic;
            result.errorMessage = "Semantic analysis failed";
            return result;
        }
        
        reportPhaseCompletion(CompilationPhase::CodeGen, "Starting code generation");
        
        // Phase 4: Code Generation
        result.llvmIR = generateLLVMIR(*result.ast);
        if (result.llvmIR.empty() || diagnostics_->hasErrors()) {
            result.failedPhase = CompilationPhase::CodeGen;
            result.errorMessage = "Code generation failed";
            return result;
        }
        
        // Generate object file if requested
        if (!options_.outputFile.empty()) {
            String objFile = options_.outputFile;
            if (objFile.substr(objFile.length() - 2) != ".o") {
                objFile += ".o";
            }
            
            if (!generateObjectFile(result.llvmIR, objFile)) {
                result.errorMessage = "Object file generation failed";
                return result;
            }
            
            result.objectFile = objFile;
            
            // Link executable if not generating object file only
            if (options_.outputFile.substr(options_.outputFile.length() - 2) != ".o") {
                if (!linkExecutable({objFile}, options_.outputFile)) {
                    result.errorMessage = "Linking failed";
                    return result;
                }
                result.executable = options_.outputFile;
            }
        }
        
        result.success = true;
        return result;
        
    } catch (const CompilerError& e) {
        result.errorMessage = e.what();
        return result;
    } catch (const std::exception& e) {
        result.errorMessage = "Internal compiler error: " + String(e.what());
        return result;
    }
}

CompilationResult Compiler::compileModule(const std::vector<String>& sourceFiles) {
    CompilationResult result;
    
    // TODO: Implement separate compilation model
    // For now, compile files individually and link
    std::vector<String> objectFiles;
    
    for (const auto& sourceFile : sourceFiles) {
        auto fileResult = compile(sourceFile);
        if (!fileResult.success) {
            return fileResult; // Propagate error
        }
        
        if (!fileResult.objectFile.empty()) {
            objectFiles.push_back(fileResult.objectFile);
        }
    }
    
    // Link all object files
    if (!objectFiles.empty() && !options_.outputFile.empty()) {
        if (!linkExecutable(objectFiles, options_.outputFile)) {
            result.errorMessage = "Module linking failed";
            return result;
        }
        result.executable = options_.outputFile;
    }
    
    result.success = true;
    return result;
}

std::vector<Token> Compiler::tokenize(const String& source, const String& filename) {
    return lexer_->tokenize(source, filename);
}

unique_ptr<Module> Compiler::parse(const std::vector<Token>& tokens, const String& filename) {
    return parser_->parse(tokens, filename);
}

bool Compiler::typeCheck(Module& module) {
    return typeChecker_->check(module);
}

String Compiler::generateLLVMIR(const Module& module) {
    // Generate code using the LLVM backend
    if (!codeGenerator_->generateCode(const_cast<Module&>(module), 
                                     typeChecker_->getSymbolTable(), 
                                     typeChecker_->getTypeSystem())) {
        return ""; // Code generation failed
    }
    
    return codeGenerator_->getLLVMIRString();
}

bool Compiler::generateObjectFile(const String& llvmIR, const String& outputFile) {
    // For now, use clang to compile LLVM IR to object file
    // In a full implementation, we'd use LLVM's MC layer directly
    try {
        // Write LLVM IR to a temporary file
        String tempIRFile = outputFile + ".tmp.ll";
        std::ofstream irFile(tempIRFile);
        if (!irFile.is_open()) {
            return false;
        }
        irFile << llvmIR;
        irFile.close();
        
        // Use clang to compile LLVM IR to object file
        String command = "clang -c " + tempIRFile + " -o " + outputFile;
        int result = system(command.c_str());
        
        // Clean up temporary file
        std::remove(tempIRFile.c_str());
        
        return result == 0;
    } catch (...) {
        return false;
    }
}

bool Compiler::linkExecutable(const std::vector<String>& objectFiles, const String& outputFile) {
    // Use clang to link object files into executable
    // In a full implementation, we'd use LLVM's lld or system linker directly
    try {
        if (objectFiles.empty()) {
            return false;
        }
        
        // Build clang command
        String command = "clang";
        for (const auto& objFile : objectFiles) {
            command += " " + objFile;
        }
        
        // Add our runtime library (if it exists)
        String runtimePath = "runtime.o";
        std::ifstream runtimeFile(runtimePath);
        if (runtimeFile.good()) {
            command += " " + runtimePath;
            runtimeFile.close();
        }
        
        command += " -o " + outputFile;
        
        // Add standard libraries and runtime
        // For now, use default C runtime
        
        int result = system(command.c_str());
        return result == 0;
    } catch (...) {
        return false;
    }
}

void Compiler::setTarget(const TargetInfo& target) {
    options_.target = target;
    setupTarget();
}

void Compiler::initializeLLVM() {
    // TODO: Initialize LLVM targets when LLVM integration is complete
    // This will be implemented in Phase 4
}

void Compiler::shutdownLLVM() {
    // LLVM cleanup is handled automatically
}

bool Compiler::setupTarget() {
    if (options_.target.triple.empty()) {
        options_.target.triple = getDefaultTargetTriple();
    }
    
    if (!isTargetSupported(options_.target.triple)) {
        diagnostics_->error("Unsupported target: " + options_.target.triple, {});
        return false;
    }
    
    return true;
}

void Compiler::reportPhaseCompletion(CompilationPhase phase, const String& message) {
    if (options_.optimizationLevel != CompilerOptions::OptLevel::O0) {
        // Only report in debug/verbose mode
        return;
    }
    
    std::cout << "[" << static_cast<int>(phase) << "] " << message << std::endl;
}

void Compiler::reportPhaseError(CompilationPhase phase, const String& error) {
    std::cerr << "Error in phase " << static_cast<int>(phase) << ": " << error << std::endl;
}

// Utility functions
String getDefaultTargetTriple() {
    // TODO: Use LLVM to get default target triple
    // For now, return a reasonable default
#ifdef __x86_64__
    return "x86_64-pc-linux-gnu";
#elif __aarch64__
    return "aarch64-unknown-linux-gnu";
#else
    return "unknown-unknown-unknown";
#endif
}

std::vector<String> getSupportedTargets() {
    // TODO: Implement by querying LLVM target registry
    return {
        "x86_64-pc-linux-gnu",
        "x86_64-apple-darwin",
        "x86_64-pc-windows-msvc",
        "aarch64-unknown-linux-gnu",
        "aarch64-apple-darwin",
        "riscv64-unknown-linux-gnu"
    };
}

bool isTargetSupported(const String& triple) {
    auto supported = getSupportedTargets();
    return std::find(supported.begin(), supported.end(), triple) != supported.end();
}

VersionInfo getVersionInfo() {
    VersionInfo info;
    info.llvmVersion = "20.1.2"; // TODO: Get from LLVM
    return info;
}

String getVersionString() {
    auto info = getVersionInfo();
    return "TSC " + std::to_string(info.major) + "." + 
           std::to_string(info.minor) + "." + std::to_string(info.patch) +
           " (LLVM " + info.llvmVersion + ")";
}

} // namespace tsc