#include "tsc/Compiler.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/lexer/LexerRefactored.h"
#include "tsc/parser/ParserRefactored.h"
#include "tsc/semantic/SemanticAnalyzerRefactored.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/ast/ASTRefactored.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

namespace tsc {

Compiler::Compiler() 
    : diagnostics_(std::make_unique<DiagnosticEngine>()) {
}

Compiler::~Compiler() = default;

bool Compiler::compile(const String& inputFile, const String& outputFile, const CompilerOptions& options) {
    try {
        std::cout << "=== TSC Refactored Compiler ===" << std::endl;
        std::cout << "Input file: " << inputFile << std::endl;
        std::cout << "Output file: " << outputFile << std::endl;
        std::cout << "Build type: " << (options.debug ? "Debug" : "Release") << std::endl;
        
        // Read source file
        String sourceCode = readSourceFile(inputFile);
        if (sourceCode.empty()) {
            diagnostics_->error("Failed to read source file: " + inputFile, SourceLocation());
            return false;
        }
        
        std::cout << "Source file read successfully (" << sourceCode.length() << " characters)" << std::endl;
        
        // Phase 1: Lexical Analysis
        std::cout << "\n--- Phase 1: Lexical Analysis ---" << std::endl;
        auto lexer = std::make_unique<lexer::LexerRefactored>(*diagnostics_);
        auto tokens = lexer->tokenize(sourceCode);
        
        if (tokens.empty()) {
            diagnostics_->error("No tokens generated from source", SourceLocation());
            return false;
        }
        
        std::cout << "Generated " << tokens.size() << " tokens" << std::endl;
        
        // Phase 2: Syntax Analysis
        std::cout << "\n--- Phase 2: Syntax Analysis ---" << std::endl;
        auto parser = std::make_unique<parser::ParserRefactored>(*diagnostics_);
        auto ast = parser->parse(tokens);
        
        if (!ast) {
            diagnostics_->error("Failed to parse source into AST", SourceLocation());
            return false;
        }
        
        std::cout << "AST generated successfully" << std::endl;
        
        // Phase 3: Semantic Analysis
        std::cout << "\n--- Phase 3: Semantic Analysis ---" << std::endl;
        auto semanticAnalyzer = std::make_unique<semantic::SemanticAnalyzerRefactored>(*diagnostics_);
        bool semanticSuccess = semanticAnalyzer->analyze(*ast);
        
        if (!semanticSuccess) {
            diagnostics_->error("Semantic analysis failed", SourceLocation());
            return false;
        }
        
        std::cout << "Semantic analysis completed successfully" << std::endl;
        
        // Phase 4: Code Generation
        std::cout << "\n--- Phase 4: Code Generation ---" << std::endl;
        auto codeGen = std::make_unique<codegen::LLVMCodeGenRefactored>(*diagnostics_, options);
        bool codeGenSuccess = codeGen->generateCode(*ast);
        
        if (!codeGenSuccess) {
            diagnostics_->error("Code generation failed", SourceLocation());
            return false;
        }
        
        std::cout << "Code generation completed successfully" << std::endl;
        
        // Phase 5: Output Generation
        std::cout << "\n--- Phase 5: Output Generation ---" << std::endl;
        bool outputSuccess = codeGen->writeOutput(outputFile);
        
        if (!outputSuccess) {
            diagnostics_->error("Failed to write output file: " + outputFile, SourceLocation());
            return false;
        }
        
        std::cout << "Output written to: " << outputFile << std::endl;
        
        // Print compilation summary
        std::cout << "\n=== Compilation Summary ===" << std::endl;
        std::cout << "✓ Lexical Analysis: " << tokens.size() << " tokens" << std::endl;
        std::cout << "✓ Syntax Analysis: AST generated" << std::endl;
        std::cout << "✓ Semantic Analysis: Type checking completed" << std::endl;
        std::cout << "✓ Code Generation: LLVM IR generated" << std::endl;
        std::cout << "✓ Output Generation: " << outputFile << std::endl;
        
        if (diagnostics_->getErrorCount() > 0) {
            std::cout << "⚠ Warnings/Errors: " << diagnostics_->getErrorCount() << std::endl;
        } else {
            std::cout << "✓ No errors or warnings" << std::endl;
        }
        
        std::cout << "\n🎉 Compilation completed successfully!" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        diagnostics_->error("Compilation failed with exception: " + String(e.what()), SourceLocation());
        return false;
    }
}

String Compiler::readSourceFile(const String& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    String content;
    String line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    return content;
}

DiagnosticEngine& Compiler::getDiagnostics() {
    return *diagnostics_;
}

} // namespace tsc

// Main function
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.ts> <output.ll>" << std::endl;
        std::cerr << "Example: " << argv[0] << " hello.ts hello.ll" << std::endl;
        return 1;
    }
    
    String inputFile = argv[1];
    String outputFile = argv[2];
    
    // Parse command line options
    tsc::CompilerOptions options;
    for (int i = 3; i < argc; i++) {
        String arg = argv[i];
        if (arg == "--debug") {
            options.debug = true;
        } else if (arg == "--optimize") {
            options.optimize = true;
        } else if (arg == "--verbose") {
            options.verbose = true;
        }
    }
    
    // Create compiler and compile
    tsc::Compiler compiler;
    bool success = compiler.compile(inputFile, outputFile, options);
    
    return success ? 0 : 1;
}