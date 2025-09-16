#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== Union Type Test ===" << std::endl;
    
    try {
        TypeSystem typeSystem;
        std::cout << "TypeSystem created successfully" << std::endl;
        
        auto numberType = std::make_shared<PrimitiveType>(TypeKind::Number);
        std::cout << "Number type created successfully" << std::endl;
        
        auto stringType = std::make_shared<PrimitiveType>(TypeKind::String);
        std::cout << "String type created successfully" << std::endl;
        
        std::vector<std::shared_ptr<Type>> types = {numberType, stringType};
        std::cout << "Types vector created successfully" << std::endl;
        
        auto unionType = std::make_shared<UnionType>(types);
        std::cout << "Union type created successfully" << std::endl;
        
        std::cout << "Union type has " << unionType->getTypes().size() << " types" << std::endl;
        
        // Test basic methods
        std::cout << "Testing isAny..." << std::endl;
        bool isAny = numberType->isAny();
        std::cout << "Number isAny: " << (isAny ? "true" : "false") << std::endl;
        
        bool isAny2 = unionType->isAny();
        std::cout << "Union isAny: " << (isAny2 ? "true" : "false") << std::endl;
        
        // Test getKind
        std::cout << "Testing getKind..." << std::endl;
        TypeKind kind1 = numberType->getKind();
        std::cout << "Number getKind: " << static_cast<int>(kind1) << std::endl;
        
        TypeKind kind2 = unionType->getKind();
        std::cout << "Union getKind: " << static_cast<int>(kind2) << std::endl;
        
        // Test simple conversion
        std::cout << "Testing simple conversion..." << std::endl;
        bool result = typeSystem.isConvertibleTo(numberType, stringType);
        std::cout << "Number to String conversion: " << (result ? "true" : "false") << std::endl;
        
        std::cout << "All union tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}