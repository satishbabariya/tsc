#include "tsc/Compiler.h"
#include "tsc/utils/ASTPrinter.h"
#include <iostream>
#include <fstream>

int main() {
    try {
        // Create compiler instance
        tsc::CompilerOptions options;
        tsc::Compiler compiler(options);
        
        // Read the test file
        std::ifstream file("/workspace/test_simple_method.ts");
        if (!file.is_open()) {
            std::cerr << "Failed to open test_simple_method.ts" << std::endl;
            return 1;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        
        // Tokenize and parse the file
        auto tokens = compiler.tokenize(content, "test_simple_method.ts");
        auto module = compiler.parse(tokens, "test_simple_method.ts");
        if (!module) {
            std::cerr << "Failed to parse file" << std::endl;
            return 1;
        }
        
        // Print AST
        std::cout << "=== AST Structure ===" << std::endl;
        tsc::ASTPrinter printer(std::cout, 0);
        printer.print(*module);
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}