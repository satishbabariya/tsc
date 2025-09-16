#include "tsc/codegen/ObjectFileGenerator.h"
#include "tsc/TargetRegistry.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/AsmParser/Parser.h>
#include <llvm/Support/SourceMgr.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace tsc {

ObjectFileGenerator::ObjectFileGenerator(DiagnosticEngine& diagnostics)
    : diagnostics_(diagnostics), optimizationLevel_(0) {
    
    // Initialize LLVM targets
    auto& registry = TargetRegistry::getInstance();
    registry.initializeAllTargets();
    
    // Initialize only AArch64 target (the one we have libraries for)
    LLVMInitializeAArch64TargetInfo();
    LLVMInitializeAArch64Target();
    LLVMInitializeAArch64TargetMC();
    LLVMInitializeAArch64AsmParser();
    LLVMInitializeAArch64AsmPrinter();
}

ObjectFileGenerator::~ObjectFileGenerator() = default;

bool ObjectFileGenerator::generateObjectFile(llvm::Module& module, 
                                            const String& outputFile,
                                            const String& targetTriple) {
    try {
        // Determine target triple
        String triple = targetTriple.empty() ? getDefaultTargetTriple() : targetTriple;
        
        // Initialize target machine if needed
        if (!targetMachine_ || currentTargetTriple_ != triple) {
            if (!initializeTargetMachine(triple)) {
                reportError("Failed to initialize target machine for triple: " + triple);
                return false;
            }
        }
        
        // Set target triple on module
        module.setTargetTriple(triple);
        
        // Set data layout
        if (targetMachine_) {
            module.setDataLayout(targetMachine_->createDataLayout());
        }
        
        // Create output file
        std::error_code errorCode;
        llvm::raw_fd_ostream outputStream(outputFile, errorCode, llvm::sys::fs::OF_None);
        if (errorCode) {
            reportError("Failed to create output file '" + outputFile + "': " + errorCode.message());
            return false;
        }
        
        // Create pass manager for code generation
        llvm::legacy::PassManager passManager;
        
        // Add target-specific passes
        if (targetMachine_->addPassesToEmitFile(passManager, outputStream, nullptr, 
                                               llvm::CodeGenFileType::ObjectFile)) {
            reportError("Target machine does not support object file emission");
            return false;
        }
        
        // Run passes
        passManager.run(module);
        
        // Flush output
        outputStream.flush();
        
        return true;
        
    } catch (const std::exception& e) {
        reportError("Exception during object file generation: " + String(e.what()));
        return false;
    }
}

bool ObjectFileGenerator::generateObjectFile(const String& llvmIR,
                                            const String& outputFile,
                                            const String& targetTriple) {
    try {
        // Create LLVM context and parse IR
        llvm::LLVMContext context;
        llvm::SMDiagnostic error;
        
        // Create memory buffer from string
        auto buffer = llvm::MemoryBuffer::getMemBuffer(llvmIR, "input");
        
        // Parse LLVM IR from string
        auto module = llvm::parseAssembly(buffer->getMemBufferRef(), error, context);
        if (!module) {
            std::string errorMessage;
            llvm::raw_string_ostream errorStream(errorMessage);
            error.print("", errorStream);
            reportError("Failed to parse LLVM IR: " + errorMessage);
            return false;
        }
        
        // Generate object file
        return generateObjectFile(*module, outputFile, targetTriple);
        
    } catch (const std::exception& e) {
        reportError("Exception during LLVM IR parsing: " + String(e.what()));
        return false;
    }
}

void ObjectFileGenerator::setOptimizationLevel(unsigned level) {
    optimizationLevel_ = std::min(level, 3u);
}

void ObjectFileGenerator::setTargetFeatures(const std::vector<String>& features) {
    targetFeatures_ = features;
}

bool ObjectFileGenerator::isTargetSupported(const String& targetTriple) const {
    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    return target != nullptr;
}

bool ObjectFileGenerator::initializeTargetMachine(const String& targetTriple) {
    try {
        // Create target machine
        targetMachine_ = createTargetMachine(targetTriple, "", targetFeatures_);
        if (!targetMachine_) {
            return false;
        }
        
        currentTargetTriple_ = targetTriple;
        
        // Set optimization level
        if (optimizationLevel_ > 0) {
            llvm::CodeGenOptLevel optLevel;
            switch (optimizationLevel_) {
                case 1: optLevel = llvm::CodeGenOptLevel::Less; break;
                case 2: optLevel = llvm::CodeGenOptLevel::Default; break;
                case 3: optLevel = llvm::CodeGenOptLevel::Aggressive; break;
                default: optLevel = llvm::CodeGenOptLevel::None; break;
            }
            targetMachine_->setOptLevel(optLevel);
        }
        
        return true;
        
    } catch (const std::exception& e) {
        reportError("Exception during target machine initialization: " + String(e.what()));
        return false;
    }
}

String ObjectFileGenerator::getDefaultTargetTriple() const {
    try {
        return llvm::sys::getDefaultTargetTriple();
    } catch (const std::exception& e) {
        reportError("Failed to get default target triple: " + String(e.what()));
        return "x86_64-unknown-linux-gnu"; // Fallback
    }
}

std::unique_ptr<llvm::TargetMachine> ObjectFileGenerator::createTargetMachine(
    const String& targetTriple,
    const String& cpu,
    const std::vector<String>& features,
    const llvm::TargetOptions& options) const {
    
    try {
        std::string error;
        const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
        if (!target) {
            reportError("Target not found: " + targetTriple + " (" + error + ")");
            return nullptr;
        }
        
        // Convert features vector to string
        std::string featureString;
        for (const auto& feature : features) {
            if (!featureString.empty()) {
                featureString += ",";
            }
            featureString += feature;
        }
        
        // Create target machine
        auto targetMachine = std::unique_ptr<llvm::TargetMachine>(
            target->createTargetMachine(targetTriple, cpu, featureString, options, 
                                       std::nullopt, std::nullopt));
        
        if (!targetMachine) {
            reportError("Failed to create target machine for: " + targetTriple);
            return nullptr;
        }
        
        return targetMachine;
        
    } catch (const std::exception& e) {
        reportError("Exception during target machine creation: " + String(e.what()));
        return nullptr;
    }
}

void ObjectFileGenerator::reportError(const String& message) const {
    diagnostics_.error(message, {});
    std::cerr << "ObjectFileGenerator Error: " << message << std::endl;
}

} // namespace tsc
