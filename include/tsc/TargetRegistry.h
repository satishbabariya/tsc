#pragma once

#include "tsc/Common.h"
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/Process.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Program.h>
#include <llvm/TargetParser/Triple.h>

namespace tsc {

// Target architecture information
struct TargetArchitecture {
    String name;           // e.g., "x86_64", "aarch64", "riscv64"
    String description;    // Human-readable description
    bool is64Bit;         // 64-bit architecture
    unsigned pointerSize;  // Pointer size in bytes
    String endianness;    // "little" or "big"
    std::vector<String> features; // Supported features
};

// Target OS information
struct TargetOS {
    String name;           // e.g., "linux", "darwin", "windows"
    String description;    // Human-readable description
    String objectFormat;   // e.g., "elf", "macho", "coff"
    String executableExtension; // e.g., "", ".exe"
    String sharedLibExtension; // e.g., ".so", ".dylib", ".dll"
};

// Target vendor information
struct TargetVendor {
    String name;           // e.g., "pc", "apple", "unknown"
    String description;    // Human-readable description
};

// Complete target information
struct TargetDetails {
    String triple;                    // Full LLVM target triple
    TargetArchitecture arch;
    TargetOS os;
    TargetVendor vendor;
    String environment;               // e.g., "gnu", "msvc", "musl"
    
    // LLVM-specific information
    std::string dataLayout;
    std::vector<String> supportedFeatures;
    bool isSupported = false;
    
    // Toolchain information
    String linker;
    String assembler;
    String objcopy;
    String strip;
};

// Target registry for managing all supported targets
class TargetRegistry {
public:
    static TargetRegistry& getInstance();
    
    // Initialize all LLVM targets
    void initializeAllTargets();
    
    // Get all available targets
    std::vector<TargetDetails> getAllTargets() const;
    
    // Get targets by architecture
    std::vector<TargetDetails> getTargetsByArch(const String& arch) const;
    
    // Get targets by OS
    std::vector<TargetDetails> getTargetsByOS(const String& os) const;
    
    // Validate target triple
    bool isValidTarget(const String& triple) const;
    
    // Get target info from triple
    std::optional<TargetDetails> getTargetInfo(const String& triple) const;
    
    // Get default target for current platform
    TargetDetails getDefaultTarget() const;
    
    // Get recommended targets for cross-compilation
    std::vector<TargetDetails> getRecommendedTargets() const;
    
    // Check if target supports specific features
    bool supportsFeature(const String& triple, const String& feature) const;
    
    // Get target-specific compiler flags
    std::vector<String> getTargetFlags(const String& triple) const;
    
    // Get toolchain information for target
    struct ToolchainInfo {
        String linker;
        String assembler;
        String objcopy;
        String strip;
        String ar;
        String ranlib;
        std::vector<String> linkerFlags;
        std::vector<String> assemblerFlags;
    };
    
    ToolchainInfo getToolchainInfo(const String& triple) const;
    
    // Check if cross-compilation is supported
    bool supportsCrossCompilation() const;
    
    // Get initialization status and diagnostics
    struct InitializationStatus {
        bool success;
        String hostTriple;
        size_t totalTargets;
        size_t supportedTargets;
        std::vector<String> availableArchitectures;
        std::vector<String> availableOSes;
        String errorMessage;
    };
    
    InitializationStatus getInitializationStatus() const;

private:
    TargetRegistry() = default;
    void populateTargets();
    TargetArchitecture parseArchitecture(const String& arch) const;
    TargetOS parseOS(const String& os) const;
    TargetVendor parseVendor(const String& vendor) const;
    
    std::vector<TargetDetails> targets_;
    bool initialized_ = false;
};

// Target-specific code generation optimizations
class TargetOptimizer {
public:
    explicit TargetOptimizer(const TargetDetails& target);
    
    // Get target-specific optimization passes
    std::vector<String> getOptimizationPasses() const;
    
    // Get target-specific code generation options
    llvm::TargetOptions getTargetOptions() const;
    
    // Get target-specific CPU features
    std::vector<String> getCPUFeatures() const;
    
    // Get target-specific ABI information
    struct ABIInfo {
        String callingConvention;
        unsigned stackAlignment;
        bool supportsTailCalls;
        bool supportsVarArgs;
    };
    
    ABIInfo getABIInfo() const;

private:
    TargetDetails target_;
};

// Cross-compilation toolchain manager
class CrossCompilationToolchain {
public:
    explicit CrossCompilationToolchain(const TargetDetails& target);
    
    // Detect available toolchain
    bool detectToolchain();
    
    // Get toolchain commands
    String getLinkerCommand() const;
    String getAssemblerCommand() const;
    String getObjcopyCommand() const;
    
    // Get toolchain flags
    std::vector<String> getLinkerFlags() const;
    std::vector<String> getAssemblerFlags() const;
    
    // Check if toolchain is available
    bool isToolchainAvailable() const;
    
    // Get toolchain version
    String getToolchainVersion() const;

private:
    TargetDetails target_;
    bool toolchainDetected_ = false;
    String linkerPath_;
    String assemblerPath_;
    String objcopyPath_;
    std::vector<String> linkerFlags_;
    std::vector<String> assemblerFlags_;
};

} // namespace tsc
