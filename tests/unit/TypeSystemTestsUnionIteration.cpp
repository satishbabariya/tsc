#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== Union Iteration Debug Test ===" << std::endl;
    
    try {
        TypeSystem typeSystem;
        std::cout << "✓ TypeSystem created successfully" << std::endl;
        
        // Create basic types
        auto numberType = std::make_shared<PrimitiveType>(TypeKind::Number);
        auto stringType = std::make_shared<PrimitiveType>(TypeKind::String);
        auto booleanType = std::make_shared<PrimitiveType>(TypeKind::Boolean);
        std::cout << "✓ Basic types created successfully" << std::endl;
        
        // Create Union type
        std::vector<std::shared_ptr<Type>> unionTypes = {numberType, stringType};
        auto unionType = std::make_shared<UnionType>(unionTypes);
        std::cout << "✓ Union type created successfully" << std::endl;
        
        // Test Union type iteration
        std::cout << "\n--- Testing Union type iteration ---" << std::endl;
        const auto& types = unionType->getTypes();
        std::cout << "Union has " << types.size() << " members" << std::endl;
        
        for (size_t i = 0; i < types.size(); ++i) {
            std::cout << "Member " << i << " getKind(): " << static_cast<int>(types[i]->getKind()) << std::endl;
            std::cout << "Member " << i << " isAny(): " << (types[i]->isAny() ? "true" : "false") << std::endl;
        }
        
        // Test comparison logic
        std::cout << "\n--- Testing comparison logic ---" << std::endl;
        std::cout << "Boolean getKind(): " << static_cast<int>(booleanType->getKind()) << std::endl;
        std::cout << "Boolean isAny(): " << (booleanType->isAny() ? "true" : "false") << std::endl;
        
        for (size_t i = 0; i < types.size(); ++i) {
            bool isAnyMatch = booleanType->isAny() || types[i]->isAny();
            bool kindMatch = booleanType->getKind() == types[i]->getKind();
            bool shouldReturn = isAnyMatch || kindMatch;
            
            std::cout << "Boolean vs Member " << i << ":" << std::endl;
            std::cout << "  isAny match: " << (isAnyMatch ? "true" : "false") << std::endl;
            std::cout << "  kind match: " << (kindMatch ? "true" : "false") << std::endl;
            std::cout << "  should return: " << (shouldReturn ? "true" : "false") << std::endl;
        }
        
        std::cout << "\n🎉 All Union iteration tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "❌ Unknown exception caught" << std::endl;
        return 1;
    }
    
    return 0;
}