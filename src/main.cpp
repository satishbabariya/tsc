#include "tsc/Compiler.h"
#include "tsc/TargetRegistry.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/utils/ASTPrinter.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace tsc;

void printUsage(const char* programName) {
    std::cout << "TSC - TypeScript Static Compiler\n";
    std::cout << "Usage: " << programName << " [options] <input-files>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o, --output <file>     Output file name\n";
    std::cout << "  -O<level>               Optimization level (0, 1, 2, 3, s, z)\n";
    std::cout << "  -g, --debug             Generate debug information\n";
    std::cout << "  --target <triple>       Target triple (e.g., x86_64-pc-linux-gnu)\n";
    std::cout << "  --list-targets          List all supported target triples\n";
    std::cout << "  --emit-llvm             Emit LLVM IR instead of object file\n";
    std::cout << "  --emit-obj              Emit object file (default)\n";
    std::cout << "  --print-ast             Print the Abstract Syntax Tree\n";
    std::cout << "  --print-symbols         Print the Symbol Table\n";
    std::cout << "  -j<N>                   Parallel compilation jobs (default: 10)\n";
    std::cout << "  -h, --help              Show this help message\n";
    std::cout << "  -v, --version           Show version information\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " hello.ts\n";
    std::cout << "  " << programName << " -o hello hello.ts\n";
    std::cout << "  " << programName << " -O2 -g src/*.ts\n";
}

void printVersion() {
    std::cout << getVersionString() << std::endl;
    std::cout << "Target: " << getDefaultTargetTriple() << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        CompilerOptions options;
        std::vector<String> inputFiles;
        bool emitLLVM = false;
        bool printAST = false;
        bool printSymbols = false;
        
        // Parse command line arguments
        for (int i = 1; i < argc; ++i) {
            String arg = argv[i];
            
            if (arg == "-h" || arg == "--help") {
                printUsage(argv[0]);
                return 0;
            } else if (arg == "-v" || arg == "--version") {
                printVersion();
                return 0;
            } else if (arg == "-o" || arg == "--output") {
                if (i + 1 >= argc) {
                    std::cerr << "Error: -o requires an argument\n";
                    return 1;
                }
                options.outputFile = argv[++i];
            } else if (arg == "-g" || arg == "--debug") {
                options.generateDebugInfo = true;
            } else if (arg == "--target") {
                if (i + 1 >= argc) {
                    std::cerr << "Error: --target requires an argument\n";
                    return 1;
                }
                options.target.triple = argv[++i];
            } else if (arg == "--list-targets") {
                auto& registry = TargetRegistry::getInstance();
                registry.initializeAllTargets();
                auto targets = registry.getAllTargets();
                
                std::cout << "Supported targets:\n";
                for (const auto& target : targets) {
                    if (target.isSupported) {
                        std::cout << "  " << target.triple;
                        std::cout << " (" << target.arch.description;
                        std::cout << " - " << target.os.description << ")\n";
                    }
                }
                return 0;
            } else if (arg == "--emit-llvm") {
                emitLLVM = true;
            } else if (arg == "--emit-obj") {
                emitLLVM = false;
            } else if (arg == "--print-ast") {
                printAST = true;
            } else if (arg == "--print-symbols") {
                printSymbols = true;
            } else if (arg.substr(0, 2) == "-O") {
                String level = arg.substr(2);
                if (level == "0") options.optimizationLevel = CompilerOptions::OptLevel::O0;
                else if (level == "1") options.optimizationLevel = CompilerOptions::OptLevel::O1;
                else if (level == "2") options.optimizationLevel = CompilerOptions::OptLevel::O2;
                else if (level == "3") options.optimizationLevel = CompilerOptions::OptLevel::O3;
                else if (level == "s") options.optimizationLevel = CompilerOptions::OptLevel::Os;
                else if (level == "z") options.optimizationLevel = CompilerOptions::OptLevel::Oz;
                else {
                    std::cerr << "Error: Invalid optimization level: " << level << "\n";
                    return 1;
                }
            } else if (arg.substr(0, 2) == "-j") {
                String jobs = arg.substr(2);
                if (!jobs.empty()) {
                    options.parallelJobs = std::stoi(jobs);
                }
            } else if (arg[0] == '-') {
                std::cerr << "Error: Unknown option: " << arg << "\n";
                return 1;
            } else {
                inputFiles.push_back(arg);
            }
        }
        
        if (inputFiles.empty()) {
            std::cerr << "Error: No input files specified\n";
            return 1;
        }
        
        // Set default output file if not specified
        if (options.outputFile.empty()) {
            if (inputFiles.size() == 1) {
                String inputFile = inputFiles[0];
                size_t dotPos = inputFile.find_last_of('.');
                if (dotPos != String::npos) {
                    options.outputFile = inputFile.substr(0, dotPos);
                } else {
                    options.outputFile = inputFile;
                }
                
                if (emitLLVM) {
                    options.outputFile += ".ll";
                } else {
                    // Default to executable
                }
            } else {
                options.outputFile = "a.out";
            }
        }
        
        options.inputFiles = inputFiles;
        
        // Create compiler and compile
        Compiler compiler(options);
        
        CompilationResult result;
        if (inputFiles.size() == 1) {
            result = compiler.compile(inputFiles[0]);
        } else {
            result = compiler.compileModule(inputFiles);
        }
        
        // Print diagnostics
        compiler.getDiagnostics().printDiagnostics();
        
        if (!result.success) {
            std::cerr << "Compilation failed: " << result.errorMessage << std::endl;
            compiler.getDiagnostics().printSummary();
            return 1;
        }
        
        // Print AST if requested
        if (printAST && result.ast) {
            std::cout << "\n=== Abstract Syntax Tree ===\n";
            ASTPrinter printer(std::cout);
            printer.print(*result.ast);
            std::cout << "=============================\n\n";
        }
        
        // Print symbol table if requested
        if (printSymbols && result.success) {
            std::cout << "\n=== Symbol Table ===\n";
            // Access symbol table through compiler
            // This would need to be implemented in the Compiler class
            std::cout << "Symbol table printing not yet implemented\n";
            std::cout << "====================\n\n";
        }
        
        // Handle emit-llvm option
        if (emitLLVM && !result.llvmIR.empty()) {
            String llvmFile = options.outputFile;
            if (llvmFile.substr(llvmFile.length() - 3) != ".ll") {
                llvmFile += ".ll";
            }
            
            std::ofstream file(llvmFile);
            if (file.is_open()) {
                file << result.llvmIR;
                file.close();
                std::cout << "LLVM IR written to: " << llvmFile << std::endl;
            } else {
                std::cerr << "Error: Could not write LLVM IR to " << llvmFile << std::endl;
                return 1;
            }
        } else if (!result.executable.empty()) {
            std::cout << "Executable generated: " << result.executable << std::endl;
        } else if (!result.objectFile.empty()) {
            std::cout << "Object file generated: " << result.objectFile << std::endl;
        }
        
        compiler.getDiagnostics().printSummary();
        return 0;
        
    } catch (const CompilerError& e) {
        std::cerr << "Compiler error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Internal error: " << e.what() << std::endl;
        return 1;
    }
}