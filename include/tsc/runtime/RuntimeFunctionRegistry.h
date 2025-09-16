#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace tsc {
namespace runtime {

/**
 * Represents a runtime function signature
 */
struct RuntimeFunctionSignature {
    std::string name;
    llvm::Type* returnType;
    std::vector<llvm::Type*> parameterTypes;
    bool isVarArg;
    
    // Default constructor
    RuntimeFunctionSignature() : name(""), returnType(nullptr), isVarArg(false) {}
    
    RuntimeFunctionSignature(const std::string& name, 
                           llvm::Type* returnType,
                           const std::vector<llvm::Type*>& parameterTypes,
                           bool isVarArg = false)
        : name(name), returnType(returnType), parameterTypes(parameterTypes), isVarArg(isVarArg) {}
};

/**
 * Registry for runtime functions
 * Maps function names to their LLVM function signatures and implementations
 */
class RuntimeFunctionRegistry {
private:
    std::unordered_map<std::string, RuntimeFunctionSignature> signatures_;
    std::unordered_map<std::string, llvm::Function*> functions_;
    llvm::LLVMContext* context_;
    llvm::Module* module_;
    
public:
    RuntimeFunctionRegistry(llvm::LLVMContext* context, llvm::Module* module);
    
    /**
     * Register a runtime function signature
     */
    void registerFunction(const RuntimeFunctionSignature& signature);
    
    /**
     * Get or create an LLVM function for a runtime function
     */
    llvm::Function* getOrCreateFunction(const std::string& name);
    
    /**
     * Check if a runtime function is registered
     */
    bool isRegistered(const std::string& name) const;
    
    /**
     * Get the signature of a registered function
     */
    const RuntimeFunctionSignature* getSignature(const std::string& name) const;
    
    /**
     * Initialize all standard runtime functions
     */
    void initializeStandardFunctions();
    
private:
    /**
     * Register console functions
     */
    void registerConsoleFunctions();
    
    /**
     * Register math functions
     */
    void registerMathFunctions();
    
    /**
     * Register string functions
     */
    void registerStringFunctions();
};

} // namespace runtime
} // namespace tsc