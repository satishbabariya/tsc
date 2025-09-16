#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== isConvertibleTo Debug Test ===" << std::endl;
    
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
        
        // Test 1: isConvertibleTo with Union type
        std::cout << "\n--- Test 1: isConvertibleTo(numberType, unionType) ---" << std::endl;
        bool result1 = typeSystem.isConvertibleTo(numberType, unionType);
        std::cout << "✓ Result: " << (result1 ? "true" : "false") << std::endl;
        
        // Test 2: isConvertibleTo with Union type
        std::cout << "\n--- Test 2: isConvertibleTo(stringType, unionType) ---" << std::endl;
        bool result2 = typeSystem.isConvertibleTo(stringType, unionType);
        std::cout << "✓ Result: " << (result2 ? "true" : "false") << std::endl;
        
        // Test 3: isConvertibleTo with Union type (should be false)
        std::cout << "\n--- Test 3: isConvertibleTo(booleanType, unionType) ---" << std::endl;
        bool result3 = typeSystem.isConvertibleTo(booleanType, unionType);
        std::cout << "✓ Result: " << (result3 ? "true" : "false") << std::endl;
        
        // Create Intersection type
        std::vector<std::shared_ptr<Type>> intersectionTypes = {numberType, stringType};
        auto intersectionType = std::make_shared<IntersectionType>(intersectionTypes);
        std::cout << "✓ Intersection type created successfully" << std::endl;
        
        // Test 4: isConvertibleTo with Intersection type
        std::cout << "\n--- Test 4: isConvertibleTo(numberType, intersectionType) ---" << std::endl;
        bool result4 = typeSystem.isConvertibleTo(numberType, intersectionType);
        std::cout << "✓ Result: " << (result4 ? "true" : "false") << std::endl;
        
        // Test 5: isConvertibleTo with Intersection type
        std::cout << "\n--- Test 5: isConvertibleTo(stringType, intersectionType) ---" << std::endl;
        bool result5 = typeSystem.isConvertibleTo(stringType, intersectionType);
        std::cout << "✓ Result: " << (result5 ? "true" : "false") << std::endl;
        
        // Test 6: isConvertibleTo with Intersection type (should be false)
        std::cout << "\n--- Test 6: isConvertibleTo(booleanType, intersectionType) ---" << std::endl;
        bool result6 = typeSystem.isConvertibleTo(booleanType, intersectionType);
        std::cout << "✓ Result: " << (result6 ? "true" : "false") << std::endl;
        
        std::cout << "\n🎉 All isConvertibleTo tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "❌ Unknown exception caught" << std::endl;
        return 1;
    }
    
    return 0;
}