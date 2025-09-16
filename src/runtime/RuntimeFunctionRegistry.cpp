#include "tsc/runtime/RuntimeFunctionRegistry.h"

namespace tsc {
namespace runtime {

RuntimeFunctionRegistry::RuntimeFunctionRegistry(llvm::LLVMContext* context, llvm::Module* module)
    : context_(context), module_(module) {
    initializeStandardFunctions();
}

void RuntimeFunctionRegistry::registerFunction(const RuntimeFunctionSignature& signature) {
    signatures_[signature.name] = signature;
}

llvm::Function* RuntimeFunctionRegistry::getOrCreateFunction(const std::string& name) {
    // Check if function already exists
    if (functions_.find(name) != functions_.end()) {
        return functions_[name];
    }
    
    // Check if signature is registered
    auto sigIt = signatures_.find(name);
    if (sigIt == signatures_.end()) {
        return nullptr; // Function not registered
    }
    
    const RuntimeFunctionSignature& signature = sigIt->second;
    
    // Create function type
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        signature.returnType,
        signature.parameterTypes,
        signature.isVarArg
    );
    
    // Create function declaration
    llvm::Function* func = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        name,
        module_
    );
    
    // Store the function
    functions_[name] = func;
    
    return func;
}

bool RuntimeFunctionRegistry::isRegistered(const std::string& name) const {
    return signatures_.find(name) != signatures_.end();
}

const RuntimeFunctionSignature* RuntimeFunctionRegistry::getSignature(const std::string& name) const {
    auto it = signatures_.find(name);
    if (it != signatures_.end()) {
        return &it->second;
    }
    return nullptr;
}

void RuntimeFunctionRegistry::initializeStandardFunctions() {
    registerConsoleFunctions();
    registerMathFunctions();
    registerStringFunctions();
    registerTypeConversionFunctions();
}

void RuntimeFunctionRegistry::registerConsoleFunctions() {
    // console_log(void* value) - for numbers
    registerFunction(RuntimeFunctionSignature(
        "console_log",
        llvm::Type::getVoidTy(*context_),
        {llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0)}
    ));
    
    // console_log_bool(int value) - for booleans
    registerFunction(RuntimeFunctionSignature(
        "console_log_bool",
        llvm::Type::getVoidTy(*context_),
        {llvm::Type::getInt32Ty(*context_)}
    ));
    
    // console_log_string(char* value) - for strings
    registerFunction(RuntimeFunctionSignature(
        "console_log_string",
        llvm::Type::getVoidTy(*context_),
        {llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0)}
    ));
}

void RuntimeFunctionRegistry::registerMathFunctions() {
    // Math.abs(double value) -> double
    registerFunction(RuntimeFunctionSignature(
        "Math_abs",
        llvm::Type::getDoubleTy(*context_),
        {llvm::Type::getDoubleTy(*context_)}
    ));
    
    // Math.sqrt(double value) -> double
    registerFunction(RuntimeFunctionSignature(
        "Math_sqrt",
        llvm::Type::getDoubleTy(*context_),
        {llvm::Type::getDoubleTy(*context_)}
    ));
}

void RuntimeFunctionRegistry::registerStringFunctions() {
    // String.length(char* str) -> int
    registerFunction(RuntimeFunctionSignature(
        "String_length",
        llvm::Type::getInt32Ty(*context_),
        {llvm::PointerType::get(llvm::Type::getInt8Ty(*context_), 0)}
    ));
}

void RuntimeFunctionRegistry::registerTypeConversionFunctions() {
    // _toBool(double value) -> int
    registerFunction(RuntimeFunctionSignature(
        "_toBool",
        llvm::Type::getInt32Ty(*context_),
        {llvm::Type::getDoubleTy(*context_)}
    ));
}

} // namespace runtime
} // namespace tsc