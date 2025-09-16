#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== Minimal TypeSystem Test ===" << std::endl;
    
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
        
        // Test isEquivalentTo first
        std::cout << "Testing isEquivalentTo..." << std::endl;
        bool equiv = numberType->isEquivalentTo(*intersectionType);
        std::cout << "isEquivalentTo result: " << (equiv ? "true" : "false") << std::endl;
        
        // Test isAny
        std::cout << "Testing isAny..." << std::endl;
        bool isAny = numberType->isAny();
        std::cout << "isAny result: " << (isAny ? "true" : "false") << std::endl;
        
        // Test getKind
        std::cout << "Testing getKind..." << std::endl;
        TypeKind kind = numberType->getKind();
        std::cout << "getKind result: " << static_cast<int>(kind) << std::endl;
        
        std::cout << "All basic tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}