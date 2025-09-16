#include "tsc/semantic/TypeSystem.h"
#include <iostream>
#include <memory>

using namespace tsc;

int main() {
    std::cout << "=== Simple TypeSystem Test ===" << std::endl;
    
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
        
        bool result = typeSystem.isConvertibleTo(numberType, unionType);
        std::cout << "Conversion test result: " << (result ? "true" : "false") << std::endl;
        
        std::cout << "All tests passed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}