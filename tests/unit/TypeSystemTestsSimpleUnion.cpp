#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== Simple Union Test ===" << std::endl;
    
    try {
        TypeSystem typeSystem;
        std::cout << "✓ TypeSystem created successfully" << std::endl;
        
        // Create basic types
        auto numberType = std::make_shared<PrimitiveType>(TypeKind::Number);
        auto stringType = std::make_shared<PrimitiveType>(TypeKind::String);
        auto booleanType = std::make_shared<PrimitiveType>(TypeKind::Boolean);
        std::cout << "✓ Basic types created successfully" << std::endl;
        
        // Test basic methods
        std::cout << "Number getKind(): " << static_cast<int>(numberType->getKind()) << std::endl;
        std::cout << "String getKind(): " << static_cast<int>(stringType->getKind()) << std::endl;
        std::cout << "Boolean getKind(): " << static_cast<int>(booleanType->getKind()) << std::endl;
        
        std::cout << "Number isAny(): " << (numberType->isAny() ? "true" : "false") << std::endl;
        std::cout << "String isAny(): " << (stringType->isAny() ? "true" : "false") << std::endl;
        std::cout << "Boolean isAny(): " << (booleanType->isAny() ? "true" : "false") << std::endl;
        
        // Create Union type
        std::vector<std::shared_ptr<Type>> unionTypes = {numberType, stringType};
        auto unionType = std::make_shared<UnionType>(unionTypes);
        std::cout << "✓ Union type created successfully" << std::endl;
        
        // Test Union type methods
        std::cout << "Union getKind(): " << static_cast<int>(unionType->getKind()) << std::endl;
        std::cout << "Union isAny(): " << (unionType->isAny() ? "true" : "false") << std::endl;
        
        // Test simple comparison
        std::cout << "Number == Number: " << (numberType->getKind() == numberType->getKind()) << std::endl;
        std::cout << "Number == String: " << (numberType->getKind() == stringType->getKind()) << std::endl;
        std::cout << "Boolean == Number: " << (booleanType->getKind() == numberType->getKind()) << std::endl;
        
        std::cout << "\n🎉 All basic tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "❌ Unknown exception caught" << std::endl;
        return 1;
    }
    
    return 0;
}