#include "tsc/codegen/ExecutableLinker.h"
#include <llvm/TargetParser/Triple.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

namespace tsc {

ExecutableLinker::ExecutableLinker(DiagnosticEngine& diagnostics)
    : diagnostics_(diagnostics) {
    // Set default system libraries based on common platforms
    // On macOS, we don't need explicit -lc and -lm as they're linked automatically
    systemLibraries_ = {};
}

ExecutableLinker::~ExecutableLinker() = default;

bool ExecutableLinker::linkExecutable(const std::vector<String>& objectFiles, const String& outputFile, const String& targetTriple) {
    if (objectFiles.empty()) {
        reportError("No object files provided for linking");
        return false;
    }

    // Try LLVM lld first, then fall back to system linker
    if (linkWithLLD(objectFiles, outputFile, targetTriple)) {
        return true;
    }
    
    // Fall back to system linker
    return linkWithSystemLinker(objectFiles, outputFile, targetTriple);
}

bool ExecutableLinker::linkWithLLD(const std::vector<String>& objectFiles, const String& outputFile, const String& targetTriple) {
    try {
        // Try to find lld executable
        std::vector<String> lldPaths = {
            "lld",
            "ld.lld",
            "/usr/bin/lld",
            "/usr/local/bin/lld"
        };
        
        String lldPath;
        for (const auto& path : lldPaths) {
            // Simple check if executable exists
            if (access(path.c_str(), X_OK) == 0) {
                lldPath = path;
                break;
            }
        }
        
        if (lldPath.empty()) {
            return false; // lld not found, try system linker
        }
        
        // Build lld command
        std::stringstream command;
        command << lldPath;
        
        // Add target-specific flags
        llvm::Triple triple(targetTriple);
        if (triple.isOSDarwin()) {
            command << " -arch";
            if (triple.getArch() == llvm::Triple::aarch64) {
                command << " arm64";
            } else if (triple.getArch() == llvm::Triple::x86_64) {
                command << " x86_64";
            }
        }
        
        // Add object files
        for (const auto& objFile : objectFiles) {
            command << " " << objFile;
        }
        
        // Add runtime library if found
        String runtimeLib = findRuntimeLibrary();
        if (!runtimeLib.empty()) {
            command << " " << runtimeLib;
        }
        
        // Add system libraries
        for (const auto& lib : systemLibraries_) {
            command << " " << lib;
        }
        
        // Add output file
        command << " -o " << outputFile;
        
        // Execute lld
        int result = system(command.str().c_str());
        return result == 0;
        
    } catch (const std::exception& e) {
        reportError("LLD linking failed: " + String(e.what()));
        return false;
    }
}

bool ExecutableLinker::linkWithSystemLinker(const std::vector<String>& objectFiles, const String& outputFile, const String& targetTriple) {
    try {
        // Determine system linker based on target
        llvm::Triple triple(targetTriple);
        String linker;
        
        if (triple.isOSDarwin()) {
            linker = "clang"; // Use clang as linker on macOS for better compatibility
        } else if (triple.isOSLinux()) {
            linker = "ld";
        } else if (triple.isOSWindows()) {
            linker = "link";
        } else {
            linker = "ld"; // Default
        }
        
        // Build linker command
        std::stringstream command;
        command << linker;
        
        // Add target-specific flags
        if (triple.isOSDarwin()) {
            command << " -arch";
            if (triple.getArch() == llvm::Triple::aarch64) {
                command << " arm64";
            } else if (triple.getArch() == llvm::Triple::x86_64) {
                command << " x86_64";
            }
            command << " -mmacosx-version-min=10.15"; // Minimum macOS version for clang
        }
        
        // Add object files
        for (const auto& objFile : objectFiles) {
            command << " " << objFile;
        }
        
        // Add runtime library if found
        String runtimeLib = findRuntimeLibrary();
        if (!runtimeLib.empty()) {
            command << " " << runtimeLib;
        }
        
        // Add system libraries
        for (const auto& lib : systemLibraries_) {
            command << " " << lib;
        }
        
        // Add output file
        command << " -o " << outputFile;
        
        // Execute linker
        int result = system(command.str().c_str());
        return result == 0;
        
    } catch (const std::exception& e) {
        reportError("System linker failed: " + String(e.what()));
        return false;
    }
}

String ExecutableLinker::findRuntimeLibrary() {
    // If explicitly set, use it
    if (!runtimeLibraryPath_.empty()) {
        std::ifstream file(runtimeLibraryPath_);
        if (file.good()) {
            file.close();
            return runtimeLibraryPath_;
        }
    }
    
    // Try to find runtime library in common locations
    std::vector<String> possiblePaths = {
        "build/libtsc_runtime.a",           // From project root
        "../build/libtsc_runtime.a",        // From subdirectory
        "../../build/libtsc_runtime.a",    // From deeper subdirectory
        "./libtsc_runtime.a",               // Current directory
        "/usr/local/lib/libtsc_runtime.a", // System installation
        "/usr/lib/libtsc_runtime.a"         // System installation
    };
    
    for (const auto& path : possiblePaths) {
        std::ifstream file(path);
        if (file.good()) {
            file.close();
            return path;
        }
    }
    
    return ""; // Not found
}

void ExecutableLinker::reportError(const String& message) {
    diagnostics_.error(message, {});
}

} // namespace tsc
