#include "tsc/semantic/SemanticAnalyzerRefactored.h"
#include "tsc/semantic/resolution/SymbolResolutionEngine.h"
#include "tsc/semantic/checking/TypeCheckingEngine.h"
#include "tsc/semantic/flow/FlowAnalysisEngine.h"
#include "tsc/semantic/resolution/InheritanceResolver.h"
#include "tsc/lexer/Lexer.h"
#include "tsc/parser/Parser.h"
#include "tsc/parser/VectorTokenStream.h"
#include "tsc/utils/DiagnosticEngine.h"
#include "tsc/AST.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cassert>

namespace tsc {
namespace testing {

class SemanticAnalysisModularIntegrationTest {
public:
    SemanticAnalysisModularIntegrationTest() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
    }
    
    void runAllTests() {
        std::cout << "=== Semantic Analysis Modular Integration Tests ===" << std::endl;
        
        testSymbolResolutionEngine();
        testTypeCheckingEngine();
        testSemanticAnalyzerRefactored();
        testModuleIntegration();
        testErrorHandling();
        testPerformanceComparison();
        
        std::cout << "=== All Integration Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    
    void testSymbolResolutionEngine() {
        std::cout << "\n--- Testing SymbolResolutionEngine ---" << std::endl;
        
        try {
            // Create semantic analyzer with refactored components
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test basic symbol resolution
            std::string testCode = R"(
                class TestClass {
                    private value: number;
                    public name: string;
                    
                    constructor(value: number, name: string) {
                        this.value = value;
                        this.name = name;
                    }
                    
                    getValue(): number {
                        return this.value;
                    }
                }
                
                function testFunction(param: string): string {
                    return param.toUpperCase();
                }
            )";
            
            // Parse the test code
            Lexer lexer(testCode, *diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, *diagnostics_);
            auto module = parser.parseModule();
            
            if (module) {
                // Test symbol resolution
                bool analysisResult = analyzer.analyze(*module);
                
                if (analysisResult) {
                    std::cout << "✓ SymbolResolutionEngine: Basic symbol resolution passed" << std::endl;
                } else {
                    std::cout << "✗ SymbolResolutionEngine: Basic symbol resolution failed" << std::endl;
                }
            } else {
                std::cout << "✗ SymbolResolutionEngine: Failed to parse test code" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ SymbolResolutionEngine: Exception: " << e.what() << std::endl;
        }
    }
    
    void testTypeCheckingEngine() {
        std::cout << "\n--- Testing TypeCheckingEngine ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test complex type checking scenarios
            std::string testCode = R"(
                interface GenericInterface<T> {
                    process(input: T): T;
                    validate(input: T): boolean;
                }
                
                class GenericImplementation<T> implements GenericInterface<T> {
                    private data: T;
                    
                    constructor(data: T) {
                        this.data = data;
                    }
                    
                    process(input: T): T {
                        return input;
                    }
                    
                    validate(input: T): boolean {
                        return input !== null && input !== undefined;
                    }
                }
                
                function complexFunction<T extends string | number>(
                    input: T,
                    processor: (value: T) => T,
                    validator: (value: T) => boolean
                ): T | null {
                    if (!validator(input)) {
                        return null;
                    }
                    return processor(input);
                }
                
                // Test array assignment (from extracted implementation)
                function testArrayAssignment(): void {
                    const stringArray: string[] = [];
                    const numberArray: number[] = [];
                    const genericArray: Array<any> = [];
                    
                    // Test empty array assignment
                    const emptyStringArray: string[] = [];
                    const emptyNumberArray: number[] = [];
                }
            )";
            
            Lexer lexer(testCode, *diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, *diagnostics_);
            auto module = parser.parseModule();
            
            if (module) {
                bool analysisResult = analyzer.analyze(*module);
                
                if (analysisResult) {
                    std::cout << "✓ TypeCheckingEngine: Complex type checking passed" << std::endl;
                } else {
                    std::cout << "✗ TypeCheckingEngine: Complex type checking failed" << std::endl;
                }
            } else {
                std::cout << "✗ TypeCheckingEngine: Failed to parse test code" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ TypeCheckingEngine: Exception: " << e.what() << std::endl;
        }
    }
    
    void testSemanticAnalyzerRefactored() {
        std::cout << "\n--- Testing SemanticAnalyzerRefactored Orchestration ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test inheritance resolution
            std::string testCode = R"(
                abstract class BaseProcessor {
                    protected config: any;
                    
                    constructor(config: any) {
                        this.config = config;
                    }
                    
                    abstract process(data: any): any;
                    
                    protected validateConfig(): boolean {
                        return this.config !== null;
                    }
                }
                
                class StringProcessor extends BaseProcessor {
                    process(data: string): string {
                        if (!this.validateConfig()) {
                            throw new Error("Invalid configuration");
                        }
                        return data.toUpperCase();
                    }
                }
                
                class NumberProcessor extends BaseProcessor {
                    process(data: number): number {
                        if (!this.validateConfig()) {
                            throw new Error("Invalid configuration");
                        }
                        return data * 2;
                    }
                }
                
                // Test export declarations
                export class ExportedClass {
                    public value: number;
                    
                    constructor(value: number) {
                        this.value = value;
                    }
                    
                    getValue(): number {
                        return this.value;
                    }
                }
                
                export function exportedFunction(param: string): string {
                    return param.toUpperCase();
                }
            )";
            
            Lexer lexer(testCode, *diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, *diagnostics_);
            auto module = parser.parseModule();
            
            if (module) {
                bool analysisResult = analyzer.analyze(*module);
                
                if (analysisResult) {
                    std::cout << "✓ SemanticAnalyzerRefactored: Orchestration passed" << std::endl;
                } else {
                    std::cout << "✗ SemanticAnalyzerRefactored: Orchestration failed" << std::endl;
                }
            } else {
                std::cout << "✗ SemanticAnalyzerRefactored: Failed to parse test code" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ SemanticAnalyzerRefactored: Exception: " << e.what() << std::endl;
        }
    }
    
    void testModuleIntegration() {
        std::cout << "\n--- Testing Module Integration ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test function overloading
            std::string testCode = R"(
                function overloadedFunction(input: string): string;
                function overloadedFunction(input: number): number;
                function overloadedFunction(input: string | number): string | number {
                    if (typeof input === "string") {
                        return input.toUpperCase();
                    } else {
                        return input * 2;
                    }
                }
                
                // Test complex generic constraints
                interface ConstraintA {
                    methodA(): void;
                }
                
                interface ConstraintB {
                    methodB(): void;
                }
                
                class ConstraintImplementation implements ConstraintA, ConstraintB {
                    methodA(): void {
                        console.log("Method A");
                    }
                    
                    methodB(): void {
                        console.log("Method B");
                    }
                }
                
                function constrainedFunction<T extends ConstraintA & ConstraintB>(item: T): void {
                    item.methodA();
                    item.methodB();
                }
            )";
            
            Lexer lexer(testCode, *diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, *diagnostics_);
            auto module = parser.parseModule();
            
            if (module) {
                bool analysisResult = analyzer.analyze(*module);
                
                if (analysisResult) {
                    std::cout << "✓ Module Integration: Complex scenarios passed" << std::endl;
                } else {
                    std::cout << "✗ Module Integration: Complex scenarios failed" << std::endl;
                }
            } else {
                std::cout << "✗ Module Integration: Failed to parse test code" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ Module Integration: Exception: " << e.what() << std::endl;
        }
    }
    
    void testErrorHandling() {
        std::cout << "\n--- Testing Error Handling ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test error scenarios
            std::string testCode = R"(
                // This should cause type errors
                function errorFunction(): void {
                    const x: string = 42; // Type mismatch
                    const y: number = "hello"; // Type mismatch
                    
                    // Undefined variable
                    console.log(undefinedVariable);
                    
                    // Wrong number of arguments
                    const result = Math.max(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11); // Too many arguments
                }
                
                // Test symbol redeclaration
                const duplicateSymbol = "first";
                const duplicateSymbol = "second"; // Redeclaration error
            )";
            
            Lexer lexer(testCode, *diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, *diagnostics_);
            auto module = parser.parseModule();
            
            if (module) {
                bool analysisResult = analyzer.analyze(*module);
                
                // We expect this to fail due to errors
                if (!analysisResult) {
                    std::cout << "✓ Error Handling: Correctly detected errors" << std::endl;
                } else {
                    std::cout << "✗ Error Handling: Failed to detect expected errors" << std::endl;
                }
            } else {
                std::cout << "✗ Error Handling: Failed to parse test code" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ Error Handling: Exception: " << e.what() << std::endl;
        }
    }
    
    void testPerformanceComparison() {
        std::cout << "\n--- Testing Performance Comparison ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Generate a large test case
            std::string largeTestCode = generateLargeTestCode();
            
            auto start = std::chrono::high_resolution_clock::now();
            
            Lexer lexer(largeTestCode, *diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, *diagnostics_);
            auto module = parser.parseModule();
            
            if (module) {
                bool analysisResult = analyzer.analyze(*module);
                
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                
                if (analysisResult) {
                    std::cout << "✓ Performance: Large test case completed in " << duration.count() << "ms" << std::endl;
                } else {
                    std::cout << "✗ Performance: Large test case failed" << std::endl;
                }
            } else {
                std::cout << "✗ Performance: Failed to parse large test code" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ Performance: Exception: " << e.what() << std::endl;
        }
    }
    
    std::string generateLargeTestCode() {
        std::string code = "// Large test case for performance testing\n";
        
        // Generate multiple classes
        for (int i = 0; i < 50; ++i) {
            code += "class TestClass" + std::to_string(i) + " {\n";
            code += "    private value: number;\n";
            code += "    public name: string;\n";
            code += "    \n";
            code += "    constructor(value: number, name: string) {\n";
            code += "        this.value = value;\n";
            code += "        this.name = name;\n";
            code += "    }\n";
            code += "    \n";
            code += "    getValue(): number {\n";
            code += "        return this.value;\n";
            code += "    }\n";
            code += "    \n";
            code += "    setName(name: string): void {\n";
            code += "        this.name = name;\n";
            code += "    }\n";
            code += "}\n\n";
        }
        
        // Generate multiple functions
        for (int i = 0; i < 100; ++i) {
            code += "function testFunction" + std::to_string(i) + "(param: string): string {\n";
            code += "    return param.toUpperCase();\n";
            code += "}\n\n";
        }
        
        return code;
    }
};

} // namespace testing
} // namespace tsc

// Main function for running tests
int main() {
    tsc::testing::SemanticAnalysisModularIntegrationTest test;
    test.runAllTests();
    return 0;
}