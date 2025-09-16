#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== Debug TypeSystem Test ===" << std::endl;
    
    try {
        TypeSystem typeSystem;
        std::cout << "TypeSystem created successfully" << std::endl;
        
        auto numberType = std::make_shared<PrimitiveType>(TypeKind::Number);
        std::cout << "Number type created successfully" << std::endl;
        
        auto stringType = std::make_shared<PrimitiveType>(TypeKind::String);
        std::cout << "String type created successfully" << std::endl;
        
        std::vector<std::shared_ptr<Type>> types = {numberType, stringType};
        std::cout << "Types vector created successfully" << std::endl;
        
        auto intersectionType = std::make_shared<IntersectionType>(types);
        std::cout << "Intersection type created successfully" << std::endl;
        
        std::cout << "Intersection type has " << intersectionType->getTypes().size() << " types" << std::endl;
        
        // Test getConversionKind step by step
        std::cout << "Testing getConversionKind..." << std::endl;
        ConversionKind kind = typeSystem.getConversionKind(numberType, intersectionType);
        std::cout << "getConversionKind result: " << static_cast<int>(kind) << std::endl;
        
        std::cout << "All debug tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}