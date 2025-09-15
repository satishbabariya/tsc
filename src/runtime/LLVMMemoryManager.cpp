#include "LLVMMemoryManager.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalVariable.h>

namespace tsc::runtime {

LLVMMemoryManager::LLVMMemoryManager(llvm::LLVMContext& context, llvm::Module& module)
    : context_(context), module_(module), builder_(nullptr) {
    initializeTypes();
    declareRuntimeFunctions();
    generateReferenceCountingFunctions();
    generateGCFunctions();
}

void LLVMMemoryManager::initializeTypes() {
    // Create TypeScript object base type
    // struct TypeScriptObject {
    //     void* vtable;           // Virtual table pointer
    //     i64 refCount;          // Reference count
    //     i8* typeName;          // Type name string
    // }
    std::vector<llvm::Type*> objectFields = {
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_)), // vtable
        llvm::Type::getInt64Ty(context_),                              // refCount
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_)) // typeName
    };
    objectType_ = createObjectStruct("TypeScriptObject", objectFields);
    
    // Create Array type
    // struct TypeScriptArray {
    //     TypeScriptObject base;  // Base object
    //     i64 length;            // Array length
    //     i8** elements;         // Array elements
    // }
    std::vector<llvm::Type*> arrayFields = {
        objectType_,                                                    // base
        llvm::Type::getInt64Ty(context_),                              // length
        llvm::PointerType::getUnqual(llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_))) // elements
    };
    arrayType_ = createObjectStruct("TypeScriptArray", arrayFields);
    
    // Create ObjectLiteral type
    // struct TypeScriptObjectLiteral {
    //     TypeScriptObject base;  // Base object
    //     i64 propertyCount;     // Number of properties
    //     Property* properties;  // Property array
    // }
    std::vector<llvm::Type*> objectLiteralFields = {
        objectType_,                                                    // base
        llvm::Type::getInt64Ty(context_),                              // propertyCount
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_)) // properties (simplified)
    };
    objectLiteralType_ = createObjectStruct("TypeScriptObjectLiteral", objectLiteralFields);
}

llvm::StructType* LLVMMemoryManager::createObjectStruct(const std::string& name, 
                                                       const std::vector<llvm::Type*>& fields) {
    llvm::StructType* structType = llvm::StructType::create(context_, name);
    structType->setBody(fields);
    return structType;
}

void LLVMMemoryManager::declareRuntimeFunctions() {
    // Declare malloc function
    llvm::FunctionType* mallocType = llvm::FunctionType::get(
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_)),
        {llvm::Type::getInt64Ty(context_)}, // size
        false
    );
    mallocFunc_ = llvm::Function::Create(mallocType, llvm::Function::ExternalLinkage, "malloc", &module_);
    
    // Declare free function
    llvm::FunctionType* freeType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context_),
        {llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_))}, // ptr
        false
    );
    freeFunc_ = llvm::Function::Create(freeType, llvm::Function::ExternalLinkage, "free", &module_);
    
    // Declare addRef function
    llvm::FunctionType* addRefType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context_),
        {llvm::PointerType::getUnqual(objectType_)}, // obj
        false
    );
    addRefFunc_ = llvm::Function::Create(addRefType, llvm::Function::ExternalLinkage, "tsc_addRef", &module_);
    
    // Declare releaseRef function
    llvm::FunctionType* releaseRefType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context_),
        {llvm::PointerType::getUnqual(objectType_)}, // obj
        false
    );
    releaseRefFunc_ = llvm::Function::Create(releaseRefType, llvm::Function::ExternalLinkage, "tsc_releaseRef", &module_);
    
    // Declare garbage collection function
    llvm::FunctionType* gcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(context_),
        {},
        false
    );
    gcCollectFunc_ = llvm::Function::Create(gcType, llvm::Function::ExternalLinkage, "tsc_gc_collect", &module_);
}

llvm::Value* LLVMMemoryManager::createObject(llvm::IRBuilder<>& builder, const std::string& typeName) {
    // Allocate memory for object
    llvm::Value* size = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), 64); // Simplified size
    llvm::Value* ptr = builder.CreateCall(mallocFunc_, {size}, "obj_malloc");
    
    // Cast to object type
    llvm::Value* obj = builder.CreateBitCast(ptr, llvm::PointerType::getUnqual(objectType_), "obj");
    
    // Initialize reference count to 1
    llvm::Value* refCountPtr = builder.CreateStructGEP(objectType_, obj, 1, "ref_count_ptr");
    builder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), 1), refCountPtr);
    
    // Initialize type name
    llvm::Value* typeNamePtr = builder.CreateStructGEP(objectType_, obj, 2, "type_name_ptr");
    llvm::Value* typeNameStr = builder.CreateGlobalStringPtr(typeName, "type_name");
    builder.CreateStore(typeNameStr, typeNamePtr);
    
    return obj;
}

llvm::Value* LLVMMemoryManager::addRef(llvm::IRBuilder<>& builder, llvm::Value* object) {
    return builder.CreateCall(addRefFunc_, {object}, "add_ref");
}

llvm::Value* LLVMMemoryManager::releaseRef(llvm::IRBuilder<>& builder, llvm::Value* object) {
    return builder.CreateCall(releaseRefFunc_, {object}, "release_ref");
}

void LLVMMemoryManager::generateGC(llvm::IRBuilder<>& builder) {
    builder.CreateCall(gcCollectFunc_, {}, "gc_collect");
}

void LLVMMemoryManager::generateCleanup(llvm::IRBuilder<>& builder) {
    // Generate cleanup code for any remaining objects
    generateGC(builder);
}

void LLVMMemoryManager::generateReferenceCountingFunctions() {
    // Implementation would go here for the actual reference counting logic
    // This is a placeholder for the runtime functions
}

void LLVMMemoryManager::generateGCFunctions() {
    // Implementation would go here for the garbage collection logic
    // This is a placeholder for the runtime functions
}

// TypeScriptObjectBuilder implementation
TypeScriptObjectBuilder::TypeScriptObjectBuilder(LLVMMemoryManager& memoryManager, llvm::IRBuilder<>& builder)
    : memoryManager_(memoryManager), builder_(builder) {
}

llvm::Value* TypeScriptObjectBuilder::createArray() {
    llvm::Value* obj = memoryManager_.createObject(builder_, "Array");
    
    // Initialize array-specific fields
    llvm::Value* lengthPtr = builder_.CreateStructGEP(memoryManager_.getArrayType(), obj, 1, "length_ptr");
    builder_.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(builder_.getContext()), 0), lengthPtr);
    
    llvm::Value* elementsPtr = builder_.CreateStructGEP(memoryManager_.getArrayType(), obj, 2, "elements_ptr");
    builder_.CreateStore(llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(builder_.getContext()))), elementsPtr);
    
    return obj;
}

llvm::Value* TypeScriptObjectBuilder::createObjectLiteral() {
    llvm::Value* obj = memoryManager_.createObject(builder_, "Object");
    
    // Initialize object-specific fields
    llvm::Value* propertyCountPtr = builder_.CreateStructGEP(memoryManager_.getObjectLiteralType(), obj, 1, "property_count_ptr");
    builder_.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt64Ty(builder_.getContext()), 0), propertyCountPtr);
    
    llvm::Value* propertiesPtr = builder_.CreateStructGEP(memoryManager_.getObjectLiteralType(), obj, 2, "properties_ptr");
    builder_.CreateStore(llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(builder_.getContext()))), propertiesPtr);
    
    return obj;
}

llvm::Value* TypeScriptObjectBuilder::arrayLength(llvm::Value* array) {
    llvm::Value* lengthPtr = builder_.CreateStructGEP(memoryManager_.getArrayType(), array, 1, "length_ptr");
    return builder_.CreateLoad(llvm::Type::getInt64Ty(builder_.getContext()), lengthPtr, "length");
}

llvm::Value* TypeScriptObjectBuilder::addRef(llvm::Value* obj) {
    return memoryManager_.addRef(builder_, obj);
}

llvm::Value* TypeScriptObjectBuilder::releaseRef(llvm::Value* obj) {
    return memoryManager_.releaseRef(builder_, obj);
}

llvm::Value* TypeScriptObjectBuilder::allocateObject(llvm::Type* type) {
    llvm::Value* size = llvm::ConstantInt::get(llvm::Type::getInt64Ty(builder_.getContext()), 64); // Simplified
    llvm::Value* ptr = builder_.CreateCall(memoryManager_.getMallocFunc(), {size}, "alloc");
    return builder_.CreateBitCast(ptr, llvm::PointerType::getUnqual(type), "obj");
}

void TypeScriptObjectBuilder::deallocateObject(llvm::Value* obj) {
    llvm::Value* ptr = builder_.CreateBitCast(obj, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(builder_.getContext())), "free_ptr");
    builder_.CreateCall(memoryManager_.getFreeFunc(), {ptr}, "free");
}

// LLVMFunctionBuilder implementation
LLVMFunctionBuilder::LLVMFunctionBuilder(llvm::Module& module, llvm::LLVMContext& context,
                                       const std::string& name, llvm::FunctionType* type)
    : module_(module), context_(context), function_(nullptr), builder_(context) {
    function_ = llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, &module_);
    
    // Create entry block
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context_, "entry", function_);
    builder_.SetInsertPoint(entryBlock);
}

LLVMFunctionBuilder::~LLVMFunctionBuilder() {
    // Cleanup is handled by LLVM
}

void LLVMFunctionBuilder::generatePrologue() {
    // Generate function prologue with memory management setup
    // This would include setting up local object tracking
}

void LLVMFunctionBuilder::generateEpilogue() {
    // Generate function epilogue with cleanup
    cleanupLocalObjects();
}

void LLVMFunctionBuilder::addLocalObject(llvm::Value* obj) {
    localObjects_.push_back(obj);
}

void LLVMFunctionBuilder::cleanupLocalObjects() {
    // Generate cleanup code for all local objects
    for (auto* obj : localObjects_) {
        // Generate releaseRef calls for each local object
        // This would be implemented with the actual runtime functions
    }
}

} // namespace tsc::runtime