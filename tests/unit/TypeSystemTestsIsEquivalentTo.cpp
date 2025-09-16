#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== isEquivalentTo Debug Test ===" << std::endl;
    
    try {
        TypeSystem typeSystem;
        std::cout << "✓ TypeSystem created successfully" << std::endl;
        
        // Create basic types
        auto numberType = std::make_shared<PrimitiveType>(TypeKind::Number);
        auto stringType = std::make_shared<PrimitiveType>(TypeKind::String);
        auto booleanType = std::make_shared<PrimitiveType>(TypeKind::Boolean);
        std::cout << "✓ Basic types created successfully" << std::endl;
        
        // Test isEquivalentTo between primitive types
        std::cout << "\n--- Testing isEquivalentTo between primitive types ---" << std::endl;
        bool equiv1 = numberType->isEquivalentTo(*numberType);
        std::cout << "Number == Number: " << (equiv1 ? "true" : "false") << std::endl;
        
        bool equiv2 = numberType->isEquivalentTo(*stringType);
        std::cout << "Number == String: " << (equiv2 ? "true" : "false") << std::endl;
        
        bool equiv3 = booleanType->isEquivalentTo(*numberType);
        std::cout << "Boolean == Number: " << (equiv3 ? "true" : "false") << std::endl;
        
        // Create Union type
        std::vector<std::shared_ptr<Type>> unionTypes = {numberType, stringType};
        auto unionType = std::make_shared<UnionType>(unionTypes);
        std::cout << "✓ Union type created successfully" << std::endl;
        
        // Test isEquivalentTo with Union type
        std::cout << "\n--- Testing isEquivalentTo with Union type ---" << std::endl;
        std::cout << "About to call numberType->isEquivalentTo(*unionType)..." << std::endl;
        bool equiv4 = numberType->isEquivalentTo(*unionType);
        std::cout << "Number == Union: " << (equiv4 ? "true" : "false") << std::endl;
        
        std::cout << "About to call unionType->isEquivalentTo(*numberType)..." << std::endl;
        bool equiv5 = unionType->isEquivalentTo(*numberType);
        std::cout << "Union == Number: " << (equiv5 ? "true" : "false") << std::endl;
        
        std::cout << "About to call unionType->isEquivalentTo(*unionType)..." << std::endl;
        bool equiv6 = unionType->isEquivalentTo(*unionType);
        std::cout << "Union == Union: " << (equiv6 ? "true" : "false") << std::endl;
        
        std::cout << "\n🎉 All isEquivalentTo tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "❌ Unknown exception caught" << std::endl;
        return 1;
    }
    
    return 0;
}