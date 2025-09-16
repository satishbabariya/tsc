#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>
#include <cassert>

using namespace tsc;

int main() {
    std::cout << "=== Segfault Debug Test ===" << std::endl;
    
    try {
        TypeSystem typeSystem;
        std::cout << "✓ TypeSystem created successfully" << std::endl;
        
        // Create basic types
        auto numberType = std::make_shared<PrimitiveType>(TypeKind::Number);
        auto stringType = std::make_shared<PrimitiveType>(TypeKind::String);
        auto booleanType = std::make_shared<PrimitiveType>(TypeKind::Boolean);
        std::cout << "✓ Basic types created successfully" << std::endl;
        
        // Test 1: Basic getConversionKind with primitive types
        std::cout << "\n--- Test 1: Basic primitive type conversion ---" << std::endl;
        ConversionKind kind1 = typeSystem.getConversionKind(numberType, stringType);
        std::cout << "✓ Number to String conversion: " << static_cast<int>(kind1) << std::endl;
        
        // Test 2: Create Union type
        std::cout << "\n--- Test 2: Union type creation ---" << std::endl;
        std::vector<std::shared_ptr<Type>> unionTypes = {numberType, stringType};
        auto unionType = std::make_shared<UnionType>(unionTypes);
        std::cout << "✓ Union type created with " << unionType->getTypes().size() << " members" << std::endl;
        
        // Test 3: Basic Union type methods
        std::cout << "\n--- Test 3: Union type basic methods ---" << std::endl;
        std::cout << "Union getKind(): " << static_cast<int>(unionType->getKind()) << std::endl;
        std::cout << "Union isAny(): " << (unionType->isAny() ? "true" : "false") << std::endl;
        std::cout << "✓ Union type basic methods work" << std::endl;
        
        // Test 4: Create Intersection type
        std::cout << "\n--- Test 4: Intersection type creation ---" << std::endl;
        std::vector<std::shared_ptr<Type>> intersectionTypes = {numberType, stringType};
        auto intersectionType = std::make_shared<IntersectionType>(intersectionTypes);
        std::cout << "✓ Intersection type created with " << intersectionType->getTypes().size() << " members" << std::endl;
        
        // Test 5: Basic Intersection type methods
        std::cout << "\n--- Test 5: Intersection type basic methods ---" << std::endl;
        std::cout << "Intersection getKind(): " << static_cast<int>(intersectionType->getKind()) << std::endl;
        std::cout << "Intersection isAny(): " << (intersectionType->isAny() ? "true" : "false") << std::endl;
        std::cout << "✓ Intersection type basic methods work" << std::endl;
        
        // Test 6: getConversionKind with Union type (this is where segfault occurs)
        std::cout << "\n--- Test 6: getConversionKind with Union type ---" << std::endl;
        std::cout << "About to call getConversionKind(numberType, unionType)..." << std::endl;
        ConversionKind kind2 = typeSystem.getConversionKind(numberType, unionType);
        std::cout << "✓ Number to Union conversion: " << static_cast<int>(kind2) << std::endl;
        
        // Test 7: getConversionKind with Intersection type
        std::cout << "\n--- Test 7: getConversionKind with Intersection type ---" << std::endl;
        std::cout << "About to call getConversionKind(numberType, intersectionType)..." << std::endl;
        ConversionKind kind3 = typeSystem.getConversionKind(numberType, intersectionType);
        std::cout << "✓ Number to Intersection conversion: " << static_cast<int>(kind3) << std::endl;
        
        // Test 8: Reverse conversions
        std::cout << "\n--- Test 8: Reverse conversions ---" << std::endl;
        std::cout << "About to call getConversionKind(unionType, numberType)..." << std::endl;
        ConversionKind kind4 = typeSystem.getConversionKind(unionType, numberType);
        std::cout << "✓ Union to Number conversion: " << static_cast<int>(kind4) << std::endl;
        
        std::cout << "About to call getConversionKind(intersectionType, numberType)..." << std::endl;
        ConversionKind kind5 = typeSystem.getConversionKind(intersectionType, numberType);
        std::cout << "✓ Intersection to Number conversion: " << static_cast<int>(kind5) << std::endl;
        
        std::cout << "\n🎉 All tests passed! No segfault detected." << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "❌ Unknown exception caught" << std::endl;
        return 1;
    }
    
    return 0;
}