#include "tsc/TargetRegistry.h"
#include "tsc/utils/DiagnosticEngine.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/Process.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Program.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/MC/TargetRegistry.h>
#include <iostream>
#include <sstream>
#include <set>

namespace tsc {

TargetRegistry& TargetRegistry::getInstance() {
    static TargetRegistry instance;
    return instance;
}

void TargetRegistry::initializeAllTargets() {
    if (initialized_) return;
    
    std::string detectedTriple;
    
    // Step 1: Try to detect the host target triple
    try {
        detectedTriple = llvm::sys::getDefaultTargetTriple();
        std::cout << "Detected host target triple: " << detectedTriple << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to detect host target triple: " << e.what() << std::endl;
        detectedTriple = "unknown-unknown-unknown"; // Generic fallback
    }
    
    // Step 2: Initialize LLVM targets
    std::cout << "Initializing LLVM targets..." << std::endl;
    
    try {
        // Initialize all available targets, not just native
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();
        llvm::InitializeAllDisassemblers();
        std::cout << "Successfully initialized all LLVM targets" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Warning: Target initialization failed: " << e.what() << std::endl;
    }
    
    // Step 3: Create cross-platform target registry
    // Use generic triples that work across all platforms
    TargetDetails x86Target;
    x86Target.triple = "x86_64-unknown-unknown";
    x86Target.arch = parseArchitecture("x86_64");
    x86Target.os = parseOS("unknown");
    x86Target.vendor = parseVendor("unknown");
    x86Target.environment = "unknown";
    x86Target.isSupported = true;
    x86Target.dataLayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128";
    x86Target.supportedFeatures = {"generic"};
    targets_.push_back(x86Target);
    
    TargetDetails aarch64Target;
    aarch64Target.triple = "aarch64-unknown-unknown";
    aarch64Target.arch = parseArchitecture("aarch64");
    aarch64Target.os = parseOS("unknown");
    aarch64Target.vendor = parseVendor("unknown");
    aarch64Target.environment = "unknown";
    aarch64Target.isSupported = true;
    aarch64Target.dataLayout = "e-m:e-i8:8-i16:16-i32:32-i64:64-i128:128-f32:32-f64:64-v128:128-n32:64-S128";
    aarch64Target.supportedFeatures = {"generic"};
    targets_.push_back(aarch64Target);
    
    TargetDetails armTarget;
    armTarget.triple = "arm-unknown-unknown";
    armTarget.arch = parseArchitecture("arm");
    armTarget.os = parseOS("unknown");
    armTarget.vendor = parseVendor("unknown");
    armTarget.environment = "unknown";
    armTarget.isSupported = true;
    armTarget.dataLayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64";
    armTarget.supportedFeatures = {"generic"};
    targets_.push_back(armTarget);
    
    // Add more cross-platform targets
    TargetDetails riscvTarget;
    riscvTarget.triple = "riscv64-unknown-unknown";
    riscvTarget.arch = parseArchitecture("riscv64");
    riscvTarget.os = parseOS("unknown");
    riscvTarget.vendor = parseVendor("unknown");
    riscvTarget.environment = "unknown";
    riscvTarget.isSupported = true;
    riscvTarget.dataLayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128";
    riscvTarget.supportedFeatures = {"generic"};
    targets_.push_back(riscvTarget);
    
    TargetDetails wasmTarget;
    wasmTarget.triple = "wasm32-unknown-unknown";
    wasmTarget.arch = parseArchitecture("wasm32");
    wasmTarget.os = parseOS("unknown");
    wasmTarget.vendor = parseVendor("unknown");
    wasmTarget.environment = "unknown";
    wasmTarget.isSupported = true;
    wasmTarget.dataLayout = "e-m:e-p:32:32-i64:64-n32:64-S128";
    wasmTarget.supportedFeatures = {"generic"};
    targets_.push_back(wasmTarget);
    
    initialized_ = true;
    
    // Report initialization status
    std::cout << "Target registry initialized successfully with " << targets_.size() << " targets" << std::endl;
    size_t supportedCount = 0;
    for (const auto& target : targets_) {
        if (target.isSupported) supportedCount++;
    }
    std::cout << "Supported targets: " << supportedCount << "/" << targets_.size() << std::endl;
    
    if (supportedCount == 0) {
        std::cerr << "\n=== DIAGNOSTIC INFORMATION ===" << std::endl;
        std::cerr << "No supported targets found. This indicates a configuration problem." << std::endl;
        std::cerr << "===============================" << std::endl;
    }
}

bool TargetRegistry::supportsCrossCompilation() const {
    if (!initialized_) return false;
    
    // Count unique architectures and OSes
    std::set<String> architectures;
    std::set<String> oses;
    
    for (const auto& target : targets_) {
        if (target.isSupported) {
            architectures.insert(target.arch.name);
            oses.insert(target.os.name);
        }
    }
    
    // Cross-compilation requires at least 2 architectures or 2 OSes
    return architectures.size() > 1 || oses.size() > 1;
}

TargetRegistry::InitializationStatus TargetRegistry::getInitializationStatus() const {
    InitializationStatus status;
    status.success = initialized_;
    status.totalTargets = targets_.size();
    status.supportedTargets = 0;
    
    // Get host triple
    try {
        status.hostTriple = llvm::sys::getDefaultTargetTriple();
    } catch (const std::exception& e) {
        status.hostTriple = "unknown-unknown-unknown";
        status.errorMessage = "Failed to detect host triple: " + std::string(e.what());
    }
    
    // Count supported targets and collect unique architectures/OSes
    std::set<String> architectures;
    std::set<String> oses;
    
    for (const auto& target : targets_) {
        if (target.isSupported) {
            status.supportedTargets++;
            architectures.insert(target.arch.name);
            oses.insert(target.os.name);
        }
    }
    
    status.availableArchitectures.assign(architectures.begin(), architectures.end());
    status.availableOSes.assign(oses.begin(), oses.end());
    
    if (!status.success) {
        status.errorMessage = "Target registry not initialized";
    } else if (status.supportedTargets == 0) {
        status.errorMessage = "No supported targets found - LLVM may be misconfigured";
    }
    
    return status;
}

void TargetRegistry::populateTargets() {
    targets_.clear();
    
    // Get all registered targets from LLVM
    for (const llvm::Target& target : llvm::TargetRegistry::targets()) {
        TargetDetails info;
        info.triple = target.getName();
        
        // Parse the target name to extract components
        llvm::Triple triple(target.getName());
        
        // Set architecture
        info.arch = parseArchitecture(triple.getArchName().str());
        
        // Set OS
        info.os = parseOS(triple.getOSName().str());
        
        // Set vendor
        info.vendor = parseVendor(triple.getVendorName().str());
        
        // Set environment
        info.environment = triple.getEnvironmentName().str();
        
        // Check if target is supported
        std::string error;
        const llvm::Target* llvmTarget = llvm::TargetRegistry::lookupTarget(target.getName(), error);
        info.isSupported = (llvmTarget != nullptr);
        
        // Get target-specific information
        if (info.isSupported) {
            llvm::TargetOptions targetOptions;
            auto targetMachine = std::unique_ptr<llvm::TargetMachine>(
                llvmTarget->createTargetMachine(target.getName(), "generic", "", targetOptions, 
                                               std::nullopt, std::nullopt));
            
            if (targetMachine) {
                info.dataLayout = targetMachine->createDataLayout().getStringRepresentation();
                
                // Get supported features (simplified for now)
                info.supportedFeatures = {"generic"};
            }
        }
        
        targets_.push_back(info);
    }
}

TargetArchitecture TargetRegistry::parseArchitecture(const String& arch) const {
    TargetArchitecture result;
    result.name = arch;
    
    if (arch == "x86_64" || arch == "amd64") {
        result.description = "64-bit x86";
        result.is64Bit = true;
        result.pointerSize = 8;
        result.endianness = "little";
        result.features = {"sse", "sse2", "sse3", "ssse3", "sse4.1", "sse4.2", "avx", "avx2"};
    } else if (arch == "i386" || arch == "i686") {
        result.description = "32-bit x86";
        result.is64Bit = false;
        result.pointerSize = 4;
        result.endianness = "little";
        result.features = {"sse", "sse2"};
    } else if (arch == "aarch64" || arch == "arm64") {
        result.description = "64-bit ARM";
        result.is64Bit = true;
        result.pointerSize = 8;
        result.endianness = "little";
        result.features = {"neon", "crc", "crypto"};
    } else if (arch == "arm" || arch == "armv7") {
        result.description = "32-bit ARM";
        result.is64Bit = false;
        result.pointerSize = 4;
        result.endianness = "little";
        result.features = {"neon", "vfp3", "vfp4"};
    } else if (arch == "riscv64") {
        result.description = "64-bit RISC-V";
        result.is64Bit = true;
        result.pointerSize = 8;
        result.endianness = "little";
        result.features = {"m", "a", "f", "d", "c"};
    } else if (arch == "riscv32") {
        result.description = "32-bit RISC-V";
        result.is64Bit = false;
        result.pointerSize = 4;
        result.endianness = "little";
        result.features = {"m", "a", "f", "c"};
    } else if (arch == "mips64") {
        result.description = "64-bit MIPS";
        result.is64Bit = true;
        result.pointerSize = 8;
        result.endianness = "little";
        result.features = {"mips32r2", "mips64r2"};
    } else if (arch == "mips") {
        result.description = "32-bit MIPS";
        result.is64Bit = false;
        result.pointerSize = 4;
        result.endianness = "little";
        result.features = {"mips32r2"};
    } else if (arch == "ppc64") {
        result.description = "64-bit PowerPC";
        result.is64Bit = true;
        result.pointerSize = 8;
        result.endianness = "big";
        result.features = {"altivec", "vsx"};
    } else if (arch == "ppc") {
        result.description = "32-bit PowerPC";
        result.is64Bit = false;
        result.pointerSize = 4;
        result.endianness = "big";
        result.features = {"altivec"};
    } else if (arch == "s390x") {
        result.description = "64-bit System Z";
        result.is64Bit = true;
        result.pointerSize = 8;
        result.endianness = "big";
        result.features = {"vector", "transactional-memory"};
    } else {
        result.description = "Unknown architecture";
        result.is64Bit = false;
        result.pointerSize = 8;
        result.endianness = "little";
    }
    
    return result;
}

TargetOS TargetRegistry::parseOS(const String& os) const {
    TargetOS result;
    result.name = os;
    
    if (os == "linux") {
        result.description = "Linux";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "darwin" || os == "macos") {
        result.description = "macOS";
        result.objectFormat = "macho";
        result.executableExtension = "";
        result.sharedLibExtension = ".dylib";
    } else if (os == "windows" || os == "win32") {
        result.description = "Windows";
        result.objectFormat = "coff";
        result.executableExtension = ".exe";
        result.sharedLibExtension = ".dll";
    } else if (os == "freebsd") {
        result.description = "FreeBSD";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "openbsd") {
        result.description = "OpenBSD";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "netbsd") {
        result.description = "NetBSD";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "dragonfly") {
        result.description = "DragonFly BSD";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "solaris") {
        result.description = "Solaris";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "haiku") {
        result.description = "Haiku";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "fuchsia") {
        result.description = "Fuchsia";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "hermit") {
        result.description = "Hermit";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else if (os == "none" || os == "unknown") {
        result.description = "Bare metal";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    } else {
        result.description = "Unknown OS";
        result.objectFormat = "elf";
        result.executableExtension = "";
        result.sharedLibExtension = ".so";
    }
    
    return result;
}

TargetVendor TargetRegistry::parseVendor(const String& vendor) const {
    TargetVendor result;
    result.name = vendor;
    
    if (vendor == "pc") {
        result.description = "PC (generic)";
    } else if (vendor == "apple") {
        result.description = "Apple";
    } else if (vendor == "unknown") {
        result.description = "Unknown vendor";
    } else {
        result.description = vendor;
    }
    
    return result;
}

std::vector<TargetDetails> TargetRegistry::getAllTargets() const {
    return targets_;
}

std::vector<TargetDetails> TargetRegistry::getTargetsByArch(const String& arch) const {
    std::vector<TargetDetails> result;
    for (const auto& target : targets_) {
        if (target.arch.name == arch) {
            result.push_back(target);
        }
    }
    return result;
}

std::vector<TargetDetails> TargetRegistry::getTargetsByOS(const String& os) const {
    std::vector<TargetDetails> result;
    for (const auto& target : targets_) {
        if (target.os.name == os) {
            result.push_back(target);
        }
    }
    return result;
}

bool TargetRegistry::isValidTarget(const String& triple) const {
    return getTargetInfo(triple).has_value();
}

std::optional<TargetDetails> TargetRegistry::getTargetInfo(const String& triple) const {
    for (const auto& target : targets_) {
        if (target.triple == triple) {
            return target;
        }
    }
    return std::nullopt;
}

TargetDetails TargetRegistry::getDefaultTarget() const {
    // Get the native target
    String nativeTriple = llvm::sys::getDefaultTargetTriple();
    
    auto targetInfo = getTargetInfo(nativeTriple);
    if (targetInfo) {
        return *targetInfo;
    }
    
    // Fallback to first available target
    if (!targets_.empty()) {
        return targets_[0];
    }
    
    // Last resort - create a default target
    TargetDetails defaultTarget;
    defaultTarget.triple = "x86_64-unknown-linux-gnu";
    defaultTarget.arch = parseArchitecture("x86_64");
    defaultTarget.os = parseOS("linux");
    defaultTarget.vendor = parseVendor("unknown");
    defaultTarget.environment = "gnu";
    return defaultTarget;
}

std::vector<TargetDetails> TargetRegistry::getRecommendedTargets() const {
    std::vector<TargetDetails> recommended;
    
    // Add common desktop targets
    std::vector<String> commonTriples = {
        "x86_64-unknown-linux-gnu",
        "x86_64-apple-darwin",
        "x86_64-pc-windows-msvc",
        "aarch64-apple-darwin",
        "aarch64-unknown-linux-gnu",
        "riscv64-unknown-linux-gnu"
    };
    
    for (const auto& triple : commonTriples) {
        auto targetInfo = getTargetInfo(triple);
        if (targetInfo && targetInfo->isSupported) {
            recommended.push_back(*targetInfo);
        }
    }
    
    return recommended;
}

bool TargetRegistry::supportsFeature(const String& triple, const String& feature) const {
    auto targetInfo = getTargetInfo(triple);
    if (!targetInfo) return false;
    
    for (const auto& supportedFeature : targetInfo->supportedFeatures) {
        if (supportedFeature == feature) {
            return true;
        }
    }
    return false;
}

std::vector<String> TargetRegistry::getTargetFlags(const String& triple) const {
    std::vector<String> flags;
    
    auto targetInfo = getTargetInfo(triple);
    if (!targetInfo) return flags;
    
    // Add architecture-specific flags
    if (targetInfo->arch.name == "x86_64") {
        flags.push_back("-m64");
    } else if (targetInfo->arch.name == "i386") {
        flags.push_back("-m32");
    }
    
    // Add OS-specific flags
    if (targetInfo->os.name == "windows") {
        flags.push_back("-fms-extensions");
        flags.push_back("-fms-compatibility");
    }
    
    return flags;
}

TargetRegistry::ToolchainInfo TargetRegistry::getToolchainInfo(const String& triple) const {
    ToolchainInfo info;
    
    auto targetInfo = getTargetInfo(triple);
    if (!targetInfo) return info;
    
    // Set default toolchain based on target
    if (targetInfo->os.name == "windows") {
        info.linker = "lld-link";
        info.assembler = "clang";
        info.objcopy = "llvm-objcopy";
        info.strip = "llvm-strip";
        info.ar = "llvm-lib";
        info.ranlib = "llvm-ranlib";
        info.linkerFlags = {"/SUBSYSTEM:CONSOLE"};
        info.assemblerFlags = {"-c"};
    } else {
        info.linker = "ld.lld";
        info.assembler = "clang";
        info.objcopy = "llvm-objcopy";
        info.strip = "llvm-strip";
        info.ar = "llvm-ar";
        info.ranlib = "llvm-ranlib";
        info.linkerFlags = {"-static"};
        info.assemblerFlags = {"-c"};
    }
    
    return info;
}

// TargetOptimizer implementation
TargetOptimizer::TargetOptimizer(const TargetDetails& target) : target_(target) {}

std::vector<String> TargetOptimizer::getOptimizationPasses() const {
    std::vector<String> passes;
    
    // Add target-specific optimization passes
    if (target_.arch.name == "x86_64") {
        passes.push_back("x86-optimize");
        passes.push_back("x86-sse-optimize");
    } else if (target_.arch.name == "aarch64") {
        passes.push_back("aarch64-optimize");
        passes.push_back("aarch64-neon-optimize");
    }
    
    return passes;
}

llvm::TargetOptions TargetOptimizer::getTargetOptions() const {
    llvm::TargetOptions options;
    
    // Set target-specific options
    if (target_.os.name == "windows") {
        options.ExceptionModel = llvm::ExceptionHandling::WinEH;
    } else {
        options.ExceptionModel = llvm::ExceptionHandling::DwarfCFI;
    }
    
    return options;
}

std::vector<String> TargetOptimizer::getCPUFeatures() const {
    return target_.arch.features;
}

TargetOptimizer::ABIInfo TargetOptimizer::getABIInfo() const {
    ABIInfo info;
    
    if (target_.arch.name == "x86_64") {
        info.callingConvention = "x86_64";
        info.stackAlignment = 16;
        info.supportsTailCalls = true;
        info.supportsVarArgs = true;
    } else if (target_.arch.name == "aarch64") {
        info.callingConvention = "aarch64";
        info.stackAlignment = 16;
        info.supportsTailCalls = true;
        info.supportsVarArgs = true;
    } else {
        info.callingConvention = "default";
        info.stackAlignment = 8;
        info.supportsTailCalls = false;
        info.supportsVarArgs = true;
    }
    
    return info;
}

// CrossCompilationToolchain implementation
CrossCompilationToolchain::CrossCompilationToolchain(const TargetDetails& target) : target_(target) {}

bool CrossCompilationToolchain::detectToolchain() {
    if (toolchainDetected_) return true;
    
    // Try to find LLVM toolchain
    std::vector<String> possibleLinkers = {"ld.lld", "lld", "clang"};
    std::vector<String> possibleAssemblers = {"clang", "llvm-mc"};
    
    for (const auto& linker : possibleLinkers) {
        if (llvm::sys::findProgramByName(linker)) {
            linkerPath_ = linker;
            break;
        }
    }
    
    for (const auto& assembler : possibleAssemblers) {
        if (llvm::sys::findProgramByName(assembler)) {
            assemblerPath_ = assembler;
            break;
        }
    }
    
    toolchainDetected_ = !linkerPath_.empty() && !assemblerPath_.empty();
    return toolchainDetected_;
}

String CrossCompilationToolchain::getLinkerCommand() const {
    return linkerPath_;
}

String CrossCompilationToolchain::getAssemblerCommand() const {
    return assemblerPath_;
}

String CrossCompilationToolchain::getObjcopyCommand() const {
    return "llvm-objcopy";
}

std::vector<String> CrossCompilationToolchain::getLinkerFlags() const {
    std::vector<String> flags;
    
    if (target_.os.name == "windows") {
        flags.push_back("/SUBSYSTEM:CONSOLE");
    } else {
        flags.push_back("-static");
    }
    
    return flags;
}

std::vector<String> CrossCompilationToolchain::getAssemblerFlags() const {
    return {"-c"};
}

bool CrossCompilationToolchain::isToolchainAvailable() const {
    return toolchainDetected_;
}

String CrossCompilationToolchain::getToolchainVersion() const {
    // TODO: Implement version detection
    return "unknown";
}

} // namespace tsc
