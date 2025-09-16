#pragma once

#include "tsc/Common.h"
#include "tsc/TargetRegistry.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <llvm/IR/Module.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Triple.h>
#include <memory>

namespace tsc {

/**
 * ObjectFileGenerator - Generates native object files from LLVM IR using LLVM's MC layer
 * 
 * This class provides a production-ready implementation for converting LLVM IR to
 * native object files without relying on external tools like clang.
 */
class ObjectFileGenerator {
public:
    explicit ObjectFileGenerator(DiagnosticEngine& diagnostics);
    ~ObjectFileGenerator();

    /**
     * Generate object file from LLVM IR module
     * @param module The LLVM IR module to compile
     * @param outputFile Path to the output object file
     * @param targetTriple Target triple for compilation
     * @return true if successful, false otherwise
     */
    bool generateObjectFile(llvm::Module& module, 
                           const String& outputFile,
                           const String& targetTriple = "");

    /**
     * Generate object file from LLVM IR string
     * @param llvmIR The LLVM IR as a string
     * @param outputFile Path to the output object file
     * @param targetTriple Target triple for compilation
     * @return true if successful, false otherwise
     */
    bool generateObjectFile(const String& llvmIR,
                           const String& outputFile,
                           const String& targetTriple = "");

    /**
     * Set optimization level
     * @param level Optimization level (0-3)
     */
    void setOptimizationLevel(unsigned level);

    /**
     * Set target-specific features
     * @param features Vector of target features to enable
     */
    void setTargetFeatures(const std::vector<String>& features);

    /**
     * Check if target is supported
     * @param targetTriple Target triple to check
     * @return true if supported, false otherwise
     */
    bool isTargetSupported(const String& targetTriple) const;

private:
    DiagnosticEngine& diagnostics_;
    std::unique_ptr<llvm::TargetMachine> targetMachine_;
    String currentTargetTriple_;
    unsigned optimizationLevel_;
    std::vector<String> targetFeatures_;

    /**
     * Initialize target machine for the given triple
     * @param targetTriple Target triple
     * @return true if successful, false otherwise
     */
    bool initializeTargetMachine(const String& targetTriple);

    /**
     * Get default target triple for current platform
     * @return Default target triple
     */
    String getDefaultTargetTriple() const;

    /**
     * Create target machine with specified options
     * @param targetTriple Target triple
     * @param cpu CPU name (empty for generic)
     * @param features Target features
     * @param options Target options
     * @return Target machine instance or nullptr on failure
     */
    std::unique_ptr<llvm::TargetMachine> createTargetMachine(
        const String& targetTriple,
        const String& cpu = "",
        const std::vector<String>& features = {},
        const llvm::TargetOptions& options = llvm::TargetOptions()) const;

    /**
     * Report error with diagnostic information
     * @param message Error message
     */
    void reportError(const String& message) const;
};

} // namespace tsc
