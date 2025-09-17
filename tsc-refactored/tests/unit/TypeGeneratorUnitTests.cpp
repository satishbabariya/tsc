#include "tsc/codegen/types/TypeGenerator.h"
#include "tsc/codegen/LLVMCodeGenRefactored.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/semantic/TypeSystem.h"

#include <iostream>
#include <memory>
#include <cassert>

namespace tsc {
namespace testing {

class TypeGeneratorUnitTests {
public:
    TypeGeneratorUnitTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
        setupTestEnvironment();
    }
    
    void runAllTests() {
        std::cout << "=== TypeGenerator Unit Tests ===" << std::endl;
        
        testBasicTypes();
        testTypeCompatibility();
        testTypeConversion();
        testArrayTypes();
        testFunctionTypes();
        testClassTypes();
        testUnionTypes();
        testGenericTypes();
        
        std::cout << "=== All TypeGenerator Unit Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    std::unique_ptr<codegen::LLVMCodeGenRefactored> codeGen_;
    std::unique_ptr<codegen::TypeGenerator> typeGen_;
    
    void setupTestEnvironment() {
        // Create code generator with test options
        CompilerOptions options;
        codeGen_ = std::make_unique<codegen::LLVMCodeGenRefactored>(*diagnostics_, options);
        typeGen_ = codeGen_->getTypeGenerator();
    }
    
    void testBasicTypes() {
        std::cout << "\n--- Testing Basic Types ---" << std::endl;
        
        try {
            // Test string type
            llvm::Type* stringType = typeGen_->getStringType();
            assert(stringType != nullptr);
            assert(stringType->isPointerTy());
            assert(typeGen_->isStringType(stringType));
            
            // Test number type
            llvm::Type* numberType = typeGen_->getNumberType();
            assert(numberType != nullptr);
            assert(numberType->isDoubleTy());
            assert(typeGen_->isNumericType(numberType));
            
            // Test boolean type
            llvm::Type* booleanType = typeGen_->getBooleanType();
            assert(booleanType != nullptr);
            assert(booleanType->isIntegerTy(1));
            assert(typeGen_->isBooleanType(booleanType));
            
            // Test void type
            llvm::Type* voidType = typeGen_->getVoidType();
            assert(voidType != nullptr);
            assert(voidType->isVoidTy());
            
            // Test any type
            llvm::Type* anyType = typeGen_->getAnyType();
            assert(anyType != nullptr);
            assert(anyType->isPointerTy());
            assert(typeGen_->isPointerType(anyType));
            
            // Test null type
            llvm::Type* nullType = typeGen_->getNullType();
            assert(nullType != nullptr);
            assert(nullType->isPointerTy());
            
            // Test undefined type
            llvm::Type* undefinedType = typeGen_->getUndefinedType();
            assert(undefinedType != nullptr);
            assert(undefinedType->isPointerTy());
            
            std::cout << "✓ Basic types: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Basic types: Exception: " << e.what() << std::endl;
        }
    }
    
    void testTypeCompatibility() {
        std::cout << "\n--- Testing Type Compatibility ---" << std::endl;
        
        try {
            llvm::Type* stringType = typeGen_->getStringType();
            llvm::Type* numberType = typeGen_->getNumberType();
            llvm::Type* booleanType = typeGen_->getBooleanType();
            llvm::Type* anyType = typeGen_->getAnyType();
            
            // Same types should be compatible
            assert(typeGen_->isCompatible(stringType, stringType));
            assert(typeGen_->isCompatible(numberType, numberType));
            assert(typeGen_->isCompatible(booleanType, booleanType));
            
            // Any type should be compatible with everything
            assert(typeGen_->isCompatible(anyType, stringType));
            assert(typeGen_->isCompatible(stringType, anyType));
            assert(typeGen_->isCompatible(anyType, numberType));
            assert(typeGen_->isCompatible(numberType, anyType));
            
            // Different basic types should not be compatible
            assert(!typeGen_->isCompatible(stringType, numberType));
            assert(!typeGen_->isCompatible(numberType, booleanType));
            assert(!typeGen_->isCompatible(booleanType, stringType));
            
            std::cout << "✓ Type compatibility: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Type compatibility: Exception: " << e.what() << std::endl;
        }
    }
    
    void testTypeConversion() {
        std::cout << "\n--- Testing Type Conversion ---" << std::endl;
        
        try {
            llvm::Type* numberType = typeGen_->getNumberType();
            llvm::Type* stringType = typeGen_->getStringType();
            llvm::Type* anyType = typeGen_->getAnyType();
            
            // Create a test value
            llvm::Value* numberValue = llvm::ConstantFP::get(numberType, 42.0);
            
            // Test conversion to any type
            llvm::Value* convertedToAny = typeGen_->convertValue(numberValue, numberType, anyType);
            assert(convertedToAny != nullptr);
            
            // Test conversion from same type (should return original)
            llvm::Value* sameType = typeGen_->convertValue(numberValue, numberType, numberType);
            assert(sameType == numberValue);
            
            std::cout << "✓ Type conversion: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Type conversion: Exception: " << e.what() << std::endl;
        }
    }
    
    void testArrayTypes() {
        std::cout << "\n--- Testing Array Types ---" << std::endl;
        
        try {
            // Create a simple array type for testing
            auto elementType = std::make_shared<NumberType>();
            ArrayType arrayType(elementType);
            
            llvm::Type* llvmArrayType = typeGen_->mapArrayType(arrayType);
            assert(llvmArrayType != nullptr);
            assert(llvmArrayType->isStructTy());
            
            // Array should be represented as a struct with data pointer and length
            llvm::StructType* structType = llvm::cast<llvm::StructType>(llvmArrayType);
            assert(structType->getNumElements() == 2);
            assert(structType->getElementType(0)->isPointerTy()); // data pointer
            assert(structType->getElementType(1)->isIntegerTy(64)); // length
            
            std::cout << "✓ Array types: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Array types: Exception: " << e.what() << std::endl;
        }
    }
    
    void testFunctionTypes() {
        std::cout << "\n--- Testing Function Types ---" << std::endl;
        
        try {
            // Create a simple function type for testing
            std::vector<FunctionType::Parameter> params;
            params.push_back({"param1", std::make_shared<NumberType>()});
            params.push_back({"param2", std::make_shared<StringType>()});
            
            auto returnType = std::make_shared<BooleanType>();
            FunctionType funcType(params, returnType);
            
            llvm::Type* llvmFuncType = typeGen_->mapFunctionType(funcType);
            assert(llvmFuncType != nullptr);
            assert(llvm::isa<llvm::FunctionType>(llvmFuncType));
            
            llvm::FunctionType* functionType = llvm::cast<llvm::FunctionType>(llvmFuncType);
            assert(functionType->getNumParams() == 2);
            assert(functionType->getReturnType()->isIntegerTy(1)); // boolean
            
            std::cout << "✓ Function types: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Function types: Exception: " << e.what() << std::endl;
        }
    }
    
    void testClassTypes() {
        std::cout << "\n--- Testing Class Types ---" << std::endl;
        
        try {
            // Create a simple class type for testing
            std::unordered_map<String, std::shared_ptr<Type>> properties;
            properties["field1"] = std::make_shared<NumberType>();
            properties["field2"] = std::make_shared<StringType>();
            
            ClassType classType("TestClass", properties);
            
            llvm::Type* llvmClassType = typeGen_->mapClassType(classType);
            assert(llvmClassType != nullptr);
            assert(llvmClassType->isStructTy());
            
            llvm::StructType* structType = llvm::cast<llvm::StructType>(llvmClassType);
            assert(structType->getNumElements() == 2);
            assert(structType->getName() == "TestClass");
            
            std::cout << "✓ Class types: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Class types: Exception: " << e.what() << std::endl;
        }
    }
    
    void testUnionTypes() {
        std::cout << "\n--- Testing Union Types ---" << std::endl;
        
        try {
            // Create a simple union type for testing
            std::vector<std::shared_ptr<Type>> types;
            types.push_back(std::make_shared<NumberType>());
            types.push_back(std::make_shared<StringType>());
            
            UnionType unionType(types);
            
            llvm::Type* llvmUnionType = typeGen_->mapUnionType(unionType);
            assert(llvmUnionType != nullptr);
            assert(llvmUnionType->isStructTy());
            
            // Union should be represented as a tagged union with type tag and value
            llvm::StructType* structType = llvm::cast<llvm::StructType>(llvmUnionType);
            assert(structType->getNumElements() == 2);
            assert(structType->getElementType(0)->isIntegerTy(32)); // type tag
            
            std::cout << "✓ Union types: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Union types: Exception: " << e.what() << std::endl;
        }
    }
    
    void testGenericTypes() {
        std::cout << "\n--- Testing Generic Types ---" << std::endl;
        
        try {
            // Create a simple generic type for testing
            auto baseType = std::make_shared<ClassType>("GenericClass", std::unordered_map<String, std::shared_ptr<Type>>());
            std::vector<std::shared_ptr<Type>> typeArgs;
            typeArgs.push_back(std::make_shared<NumberType>());
            
            GenericType genericType(baseType, typeArgs);
            
            llvm::Type* llvmGenericType = typeGen_->mapGenericType(genericType);
            assert(llvmGenericType != nullptr);
            
            std::cout << "✓ Generic types: PASSED" << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "✗ Generic types: Exception: " << e.what() << std::endl;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::TypeGeneratorUnitTests test;
    test.runAllTests();
    return 0;
}