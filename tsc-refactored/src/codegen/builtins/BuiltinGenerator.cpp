#include "tsc/codegen/builtins/BuiltinGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"

namespace tsc {
namespace codegen {

BuiltinGenerator::BuiltinGenerator(LLVMCodeGen* codeGen) 
    : codeGen_(codeGen) {
}

void BuiltinGenerator::declareBuiltinFunctions() {
    // Declare all built-in functions
    getOrCreatePrintFunction();
    getOrCreateMallocFunction();
    getOrCreateFreeFunction();
    getOrCreateNumberToStringFunction();
    getOrCreateBooleanToStringFunction();
    getOrCreateObjectToStringFunction();
    getOrCreateStringConcatFunction();
    getOrCreatePointerToStringFunction();
    getOrCreateARCAllocFunction();
    getOrCreateARCRetainFunction();
    getOrCreateARCReleaseFunction();
    getOrCreateSetupExceptionHandlerFunction();
    getOrCreateTryExceptionHandlerFunction();
    getOrCreateCleanupExceptionHandlerFunction();
}

void BuiltinGenerator::declareBuiltinGlobals() {
    // Declare built-in global variables
    declareConsoleObject();
}

llvm::Function* BuiltinGenerator::getOrCreatePrintFunction() {
    llvm::Function* existingFunc = codeGen_->getModule()->getFunction("_print");
    if (existingFunc) {
        return existingFunc;
    }
    
    // Create _print function that takes a variable number of arguments
    // Make it variadic to handle multiple arguments like _print("msg", value)
    std::vector<llvm::Type*> paramTypes;
    paramTypes.push_back(codeGen_->getTypeGenerator()->getAnyType()); // First parameter of any type
    
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*codeGen_->getLLVMContext()), // Return type: void
        paramTypes, // Parameter types
        true // Variadic to accept multiple arguments
    );
    
    // Create the function as external (declaration only)
    // The actual implementation is in runtime.c
    llvm::Function* printFunc = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "_print",
        codeGen_->getModule()
    );
    
    return printFunc;
}

llvm::Function* BuiltinGenerator::getOrCreateMallocFunction() {
    llvm::Function* existingFunc = codeGen_->getModule()->getFunction("malloc");
    if (existingFunc) {
        return existingFunc;
    }
    
    // Create malloc function signature: void* malloc(size_t size)
    std::vector<llvm::Type*> paramTypes;
    paramTypes.push_back(llvm::Type::getInt64Ty(*codeGen_->getLLVMContext())); // size_t
    
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()), 0), // void*
        paramTypes,
        false
    );
    
    llvm::Function* mallocFunc = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "malloc",
        codeGen_->getModule()
    );
    
    return mallocFunc;
}

llvm::Function* BuiltinGenerator::getOrCreateFreeFunction() {
    llvm::Function* existingFunc = codeGen_->getModule()->getFunction("free");
    if (existingFunc) {
        return existingFunc;
    }
    
    // Create free function signature: void free(void* ptr)
    std::vector<llvm::Type*> paramTypes;
    paramTypes.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(*codeGen_->getLLVMContext()), 0)); // void*
    
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*codeGen_->getLLVMContext()),
        paramTypes,
        false
    );
    
    llvm::Function* freeFunc = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        "free",
        codeGen_->getModule()
    );
    
    return freeFunc;
}

llvm::Function* BuiltinGenerator::getOrCreateNumberToStringFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("number_to_string")) {
        return existing;
    }
    
    llvm::FunctionType* numberToStringType = llvm::FunctionType::get(
        codeGen_->getTypeGenerator()->getStringType(),
        {codeGen_->getTypeGenerator()->getNumberType()},
        false
    );
    
    return llvm::Function::Create(numberToStringType, llvm::Function::ExternalLinkage, "number_to_string",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreateBooleanToStringFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("boolean_to_string")) {
        return existing;
    }
    
    llvm::FunctionType* booleanToStringType = llvm::FunctionType::get(
        codeGen_->getTypeGenerator()->getStringType(),
        {codeGen_->getTypeGenerator()->getBooleanType()},
        false
    );
    
    return llvm::Function::Create(booleanToStringType, llvm::Function::ExternalLinkage, "boolean_to_string",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreateObjectToStringFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("object_to_string")) {
        return existing;
    }
    
    llvm::FunctionType* objectToStringType = llvm::FunctionType::get(
        codeGen_->getTypeGenerator()->getStringType(),
        {codeGen_->getTypeGenerator()->getAnyType()},
        false
    );
    
    return llvm::Function::Create(objectToStringType, llvm::Function::ExternalLinkage, "object_to_string",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreateStringConcatFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("string_concat")) {
        return existing;
    }
    
    llvm::FunctionType* stringConcatType = llvm::FunctionType::get(
        codeGen_->getTypeGenerator()->getStringType(),
        {codeGen_->getTypeGenerator()->getStringType(), codeGen_->getTypeGenerator()->getStringType()},
        false
    );
    
    return llvm::Function::Create(stringConcatType, llvm::Function::ExternalLinkage, "string_concat",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreatePointerToStringFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("pointer_to_string")) {
        return existing;
    }
    
    llvm::FunctionType* pointerToStringType = llvm::FunctionType::get(
        codeGen_->getTypeGenerator()->getStringType(),
        {codeGen_->getTypeGenerator()->getAnyType()},
        false
    );
    
    return llvm::Function::Create(pointerToStringType, llvm::Function::ExternalLinkage, "pointer_to_string",
                                 codeGen_->getModule());
}

// ARC (Automatic Reference Counting) functions
llvm::Function* BuiltinGenerator::getOrCreateARCAllocFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("__tsc_alloc")) {
        return existing;
    }
    
    // ARC allocation function: void* __tsc_alloc(size_t size)
    llvm::FunctionType* arcAllocType = llvm::FunctionType::get(
        codeGen_->getTypeGenerator()->getAnyType(),
        {llvm::Type::getInt64Ty(*codeGen_->getLLVMContext())},
        false
    );
    
    return llvm::Function::Create(arcAllocType, llvm::Function::ExternalLinkage, "__tsc_alloc",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreateARCRetainFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("__tsc_retain")) {
        return existing;
    }
    
    // ARC retain function: void* __tsc_retain(void* obj)
    llvm::FunctionType* arcRetainType = llvm::FunctionType::get(
        codeGen_->getTypeGenerator()->getAnyType(),
        {codeGen_->getTypeGenerator()->getAnyType()},
        false
    );
    
    return llvm::Function::Create(arcRetainType, llvm::Function::ExternalLinkage, "__tsc_retain",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreateARCReleaseFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("__tsc_release")) {
        return existing;
    }
    
    // ARC release function: void __tsc_release(void* obj)
    llvm::FunctionType* arcReleaseType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*codeGen_->getLLVMContext()),
        {codeGen_->getTypeGenerator()->getAnyType()},
        false
    );
    
    return llvm::Function::Create(arcReleaseType, llvm::Function::ExternalLinkage, "__tsc_release",
                                 codeGen_->getModule());
}

// Exception handling functions
llvm::Function* BuiltinGenerator::getOrCreateSetupExceptionHandlerFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("__tsc_setup_exception_handler")) {
        return existing;
    }
    
    llvm::FunctionType* setupHandlerType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*codeGen_->getLLVMContext()),
        {},
        false
    );
    
    return llvm::Function::Create(setupHandlerType, llvm::Function::ExternalLinkage, "__tsc_setup_exception_handler",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreateTryExceptionHandlerFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("__tsc_try_exception_handler")) {
        return existing;
    }
    
    llvm::FunctionType* tryHandlerType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*codeGen_->getLLVMContext()), // Returns exception code
        {},
        false
    );
    
    return llvm::Function::Create(tryHandlerType, llvm::Function::ExternalLinkage, "__tsc_try_exception_handler",
                                 codeGen_->getModule());
}

llvm::Function* BuiltinGenerator::getOrCreateCleanupExceptionHandlerFunction() {
    if (auto existing = codeGen_->getModule()->getFunction("__tsc_cleanup_exception_handler")) {
        return existing;
    }
    
    llvm::FunctionType* cleanupHandlerType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*codeGen_->getLLVMContext()),
        {},
        false
    );
    
    return llvm::Function::Create(cleanupHandlerType, llvm::Function::ExternalLinkage, "__tsc_cleanup_exception_handler",
                                 codeGen_->getModule());
}

void BuiltinGenerator::declareConsoleObject() {
    // Create a global console object
    // For now, this is a placeholder - console methods are handled specially
    llvm::Type* consoleType = codeGen_->getTypeGenerator()->getAnyType();
    
    llvm::GlobalVariable* consoleGlobal = new llvm::GlobalVariable(
        *codeGen_->getModule(),
        consoleType,
        true, // isConstant
        llvm::GlobalValue::ExternalLinkage,
        nullptr, // Initializer (will be set by runtime)
        "console"
    );
    
    // The console object will be initialized by the runtime
}

llvm::Function* BuiltinGenerator::getBuiltinFunction(const String& name) {
    return codeGen_->getModule()->getFunction(name);
}

bool BuiltinGenerator::isBuiltinFunction(const String& name) const {
    static const std::unordered_set<String> builtinNames = {
        "_print",
        "malloc",
        "free",
        "number_to_string",
        "boolean_to_string",
        "object_to_string",
        "string_concat",
        "pointer_to_string",
        "__tsc_alloc",
        "__tsc_retain",
        "__tsc_release",
        "__tsc_setup_exception_handler",
        "__tsc_try_exception_handler",
        "__tsc_cleanup_exception_handler"
    };
    
    return builtinNames.find(name) != builtinNames.end();
}

llvm::Value* BuiltinGenerator::generateBuiltinCall(const String& functionName, const std::vector<llvm::Value*>& args) {
    llvm::Function* function = getBuiltinFunction(functionName);
    if (!function) {
        return nullptr;
    }
    
    // Perform basic argument count validation
    if (!function->isVarArg() && args.size() != function->arg_size()) {
        return nullptr;
    }
    
    // Generate the call
    return codeGen_->getBuilder()->CreateCall(function, args, functionName + "_call");
}

} // namespace codegen
} // namespace tsc