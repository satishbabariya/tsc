#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== getConversionKind Debug Test ===" << std::endl;
    
    try {
        TypeSystem typeSystem;
        std::cout << "✓ TypeSystem created successfully" << std::endl;
        
        // Create basic types
        auto numberType = std::make_shared<PrimitiveType>(TypeKind::Number);
        auto stringType = std::make_shared<PrimitiveType>(TypeKind::String);
        auto booleanType = std::make_shared<PrimitiveType>(TypeKind::Boolean);
        std::cout << "✓ Basic types created successfully" << std::endl;
        
        // Test getConversionKind with primitive types
        std::cout << "\n--- Testing getConversionKind with primitive types ---" << std::endl;
        ConversionKind kind1 = typeSystem.getConversionKind(numberType, stringType);
        std::cout << "Number to String: " << static_cast<int>(kind1) << std::endl;
        
        // Create Union type
        std::vector<std::shared_ptr<Type>> unionTypes = {numberType, stringType};
        auto unionType = std::make_shared<UnionType>(unionTypes);
        std::cout << "✓ Union type created successfully" << std::endl;
        
        // Test getConversionKind with Union type
        std::cout << "\n--- Testing getConversionKind with Union type ---" << std::endl;
        std::cout << "About to call getConversionKind(numberType, unionType)..." << std::endl;
        ConversionKind kind2 = typeSystem.getConversionKind(numberType, unionType);
        std::cout << "Number to Union: " << static_cast<int>(kind2) << std::endl;
        
        std::cout << "About to call getConversionKind(stringType, unionType)..." << std::endl;
        ConversionKind kind3 = typeSystem.getConversionKind(stringType, unionType);
        std::cout << "String to Union: " << static_cast<int>(kind3) << std::endl;
        
        std::cout << "About to call getConversionKind(booleanType, unionType)..." << std::endl;
        ConversionKind kind4 = typeSystem.getConversionKind(booleanType, unionType);
        std::cout << "Boolean to Union: " << static_cast<int>(kind4) << std::endl;
        
        std::cout << "\n🎉 All getConversionKind tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "❌ Unknown exception caught" << std::endl;
        return 1;
    }
    
    return 0;
}