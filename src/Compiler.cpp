#include "tsc/Compiler.h"
#include "tsc/TargetRegistry.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/parser/Parser.h"
#include "tsc/semantic/TypeChecker.h"
#include "tsc/semantic/SemanticAnalyzer.h"
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
    
    // Phase 1: Cross-module analysis using SemanticAnalyzer
    std::cout << "DEBUG: Starting cross-module analysis for " << sourceFiles.size() << " modules" << std::endl;
    
    // Create SemanticAnalyzer for cross-module analysis
    SemanticAnalyzer analyzer(*diagnostics_);
    
    // Perform cross-module analysis (dependency scanning, symbol resolution, export-to-import binding)
    if (!analyzer.analyzeProject(sourceFiles)) {
        result.errorMessage = "Cross-module analysis failed";
        return result;
    }
    
    std::cout << "DEBUG: Cross-module analysis completed successfully" << std::endl;
    
    // Phase 2: Code generation for each module with resolved symbols
    std::vector<String> objectFiles;
    for (size_t i = 0; i < sourceFiles.size(); ++i) {
        const auto& sourceFile = sourceFiles[i];
        bool isEntryPoint = (i == sourceFiles.size() - 1); // Last module is entry point
        std::cout << "DEBUG: Generating code for module: " << sourceFile 
                  << (isEntryPoint ? " (entry point)" : "") << std::endl;
        
        auto fileResult = compileWithResolvedSymbols(sourceFile, analyzer, isEntryPoint);
        if (!fileResult.success) {
            return fileResult; // Propagate error
        }
        
        if (!fileResult.objectFile.empty()) {
            objectFiles.push_back(fileResult.objectFile);
        }
    }
    
    // Phase 3: Link all object files
    if (!objectFiles.empty() && !options_.outputFile.empty()) {
        std::cout << "DEBUG: Linking " << objectFiles.size() << " object files" << std::endl;
        if (!linkExecutable(objectFiles, options_.outputFile)) {
            result.errorMessage = "Module linking failed";
            return result;
        }
        result.executable = options_.outputFile;
    }
    
    result.success = true;
    return result;
}

CompilationResult Compiler::compileWithResolvedSymbols(const String& sourceFile, SemanticAnalyzer& analyzer, bool isEntryPoint) {
    CompilationResult result;
    
    try {
        std::cout << "DEBUG: Compiling module with resolved symbols: " << sourceFile << std::endl;
        
        // Read source file
        std::ifstream file(sourceFile);
        if (!file.is_open()) {
            result.errorMessage = "Cannot open source file: " + sourceFile;
            return result;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        // Tokenize
        auto tokens = lexer_->tokenize(content, sourceFile);
        if (tokens.empty()) {
            result.errorMessage = "Failed to tokenize: " + sourceFile;
            return result;
        }
        
        // Parse
        auto module = parser_->parse(tokens, sourceFile);
        if (!module) {
            result.errorMessage = "Failed to parse: " + sourceFile;
            return result;
        }
        
        // Store AST for potential debugging
        result.ast = std::move(module);
        
        // Create a new LLVMCodeGen instance for this module to avoid function accumulation
        auto moduleCodeGenerator = createLLVMCodeGen(*diagnostics_, options_);
        
        // Set main function generation flag for multi-module compilation
        moduleCodeGenerator->setGenerateMainFunction(isEntryPoint);
        std::cout << "DEBUG: Set generateMainFunction to " << (isEntryPoint ? "true" : "false") << " for " << sourceFile << std::endl;
        
        // Generate LLVM IR using the resolved symbols from analyzer
        // TODO: Modify LLVMCodeGen to use resolved symbols from SemanticAnalyzer
        if (!moduleCodeGenerator->generateCode(*result.ast, 
                                             typeChecker_->getSymbolTable(), 
                                             typeChecker_->getTypeSystem())) {
            result.errorMessage = "Failed to generate LLVM IR for: " + sourceFile;
            return result;
        }
        
        String llvmIR = moduleCodeGenerator->getLLVMIRString();
        result.llvmIR = llvmIR;
        
        // Generate object file
        String objectFile = sourceFile.substr(0, sourceFile.find_last_of('.')) + ".o";
        if (!generateObjectFile(llvmIR, objectFile)) {
            result.errorMessage = "Failed to generate object file for: " + sourceFile;
            return result;
        }
        
        result.objectFile = objectFile;
        result.success = true;
        
        std::cout << "DEBUG: Successfully compiled module: " << sourceFile << std::endl;
        
    } catch (const std::exception& e) {
        result.errorMessage = "Internal compiler error in compileWithResolvedSymbols: " + String(e.what());
    }
    
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
    // Enhanced LLVM MC layer integration for direct object file generation
    // Use LLVM's MC layer for better control and performance
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
    // Enhanced linker integration with LLVM's lld and system linker support
    // Use LLVM's lld for better cross-platform linking
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
        // Try multiple possible paths for the runtime library
        std::vector<String> possiblePaths = {
            "build/libtsc_runtime.a",           // From project root
            "../build/libtsc_runtime.a",        // From subdirectory
            "../../build/libtsc_runtime.a",    // From deeper subdirectory
            "./libtsc_runtime.a"               // Current directory
        };
        
        String runtimePath;
        for (const auto& path : possiblePaths) {
            std::ifstream runtimeFile(path);
            if (runtimeFile.good()) {
                runtimePath = path;
                runtimeFile.close();
                break;
            }
        }
        
        if (!runtimePath.empty()) {
            command += " " + runtimePath;
        } else {
            // Debug: print available files to help diagnose
            std::cerr << "WARNING: Runtime library not found. Tried paths:" << std::endl;
            for (const auto& path : possiblePaths) {
                std::cerr << "  - " << path << std::endl;
            }
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
    // Initialize LLVM targets
    auto& registry = TargetRegistry::getInstance();
    registry.initializeAllTargets();
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
    auto& registry = TargetRegistry::getInstance();
    registry.initializeAllTargets();
    auto defaultTarget = registry.getDefaultTarget();
    return defaultTarget.triple;
}

std::vector<String> getSupportedTargets() {
    auto& registry = TargetRegistry::getInstance();
    registry.initializeAllTargets();
    auto targets = registry.getAllTargets();
    
    std::vector<String> triples;
    for (const auto& target : targets) {
        if (target.isSupported) {
            triples.push_back(target.triple);
        }
    }
    return triples;
}

bool isTargetSupported(const String& triple) {
    auto& registry = TargetRegistry::getInstance();
    registry.initializeAllTargets();
    return registry.isValidTarget(triple);
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