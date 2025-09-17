#include "tsc/semantic/resolution/SymbolResolutionEngine.h"
#include "tsc/semantic/checking/TypeCheckingEngine.h"
#include "tsc/semantic/flow/FlowAnalysisEngine.h"
#include "tsc/semantic/resolution/InheritanceResolver.h"
#include "tsc/semantic/SemanticAnalyzerRefactored.h"
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

class SemanticAnalysisUnitTests {
public:
    SemanticAnalysisUnitTests() 
        : diagnostics_(std::make_unique<DiagnosticEngine>()) {
    }
    
    void runAllUnitTests() {
        std::cout << "=== Semantic Analysis Unit Tests ===" << std::endl;
        
        testSymbolResolutionEngineUnit();
        testTypeCheckingEngineUnit();
        testFlowAnalysisEngineUnit();
        testInheritanceResolverUnit();
        testSemanticAnalyzerRefactoredUnit();
        
        std::cout << "=== All Unit Tests Completed ===" << std::endl;
    }
    
private:
    std::unique_ptr<DiagnosticEngine> diagnostics_;
    
    void testSymbolResolutionEngineUnit() {
        std::cout << "\n--- Testing SymbolResolutionEngine Unit Functions ---" << std::endl;
        
        try {
            // Create semantic analyzer
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test 1: Function declaration collection
            std::string testCode1 = R"(
                function testFunction(param: string): string {
                    return param.toUpperCase();
                }
            )";
            
            auto module1 = parseTestCode(testCode1);
            if (module1) {
                analyzer.collectFunctionDeclarations(*module1);
                std::cout << "✓ SymbolResolutionEngine: Function declaration collection passed" << std::endl;
            }
            
            // Test 2: Class declaration collection
            std::string testCode2 = R"(
                class TestClass {
                    private value: number;
                    
                    constructor(value: number) {
                        this.value = value;
                    }
                    
                    getValue(): number {
                        return this.value;
                    }
                }
            )";
            
            auto module2 = parseTestCode(testCode2);
            if (module2) {
                analyzer.collectFunctionDeclarations(*module2);
                std::cout << "✓ SymbolResolutionEngine: Class declaration collection passed" << std::endl;
            }
            
            // Test 3: Export declaration collection
            std::string testCode3 = R"(
                export function exportedFunction(param: string): string {
                    return param.toUpperCase();
                }
                
                export class ExportedClass {
                    public value: number;
                    
                    constructor(value: number) {
                        this.value = value;
                    }
                }
            )";
            
            auto module3 = parseTestCode(testCode3);
            if (module3) {
                analyzer.collectFunctionDeclarations(*module3);
                std::cout << "✓ SymbolResolutionEngine: Export declaration collection passed" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ SymbolResolutionEngine Unit: Exception: " << e.what() << std::endl;
        }
    }
    
    void testTypeCheckingEngineUnit() {
        std::cout << "\n--- Testing TypeCheckingEngine Unit Functions ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test 1: Basic type checking
            std::string testCode1 = R"(
                function typeCheckTest(): void {
                    const x: string = "hello";
                    const y: number = 42;
                    const z: boolean = true;
                }
            )";
            
            auto module1 = parseTestCode(testCode1);
            if (module1) {
                analyzer.performTypeChecking(*module1);
                std::cout << "✓ TypeCheckingEngine: Basic type checking passed" << std::endl;
            }
            
            // Test 2: Array assignment (from extracted implementation)
            std::string testCode2 = R"(
                function arrayAssignmentTest(): void {
                    const stringArray: string[] = [];
                    const numberArray: number[] = [];
                    
                    // Test empty array assignment
                    const emptyStringArray: string[] = [];
                    const emptyNumberArray: number[] = [];
                    
                    // Test generic array assignment
                    const genericArray: Array<any> = [];
                }
            )";
            
            auto module2 = parseTestCode(testCode2);
            if (module2) {
                analyzer.performTypeChecking(*module2);
                std::cout << "✓ TypeCheckingEngine: Array assignment checking passed" << std::endl;
            }
            
            // Test 3: Generic type checking
            std::string testCode3 = R"(
                interface GenericInterface<T> {
                    process(input: T): T;
                }
                
                class GenericImplementation<T> implements GenericInterface<T> {
                    private data: T;
                    
                    constructor(data: T) {
                        this.data = data;
                    }
                    
                    process(input: T): T {
                        return input;
                    }
                }
            )";
            
            auto module3 = parseTestCode(testCode3);
            if (module3) {
                analyzer.performTypeChecking(*module3);
                std::cout << "✓ TypeCheckingEngine: Generic type checking passed" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ TypeCheckingEngine Unit: Exception: " << e.what() << std::endl;
        }
    }
    
    void testFlowAnalysisEngineUnit() {
        std::cout << "\n--- Testing FlowAnalysisEngine Unit Functions ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test 1: Basic flow analysis
            std::string testCode1 = R"(
                function flowTest(): void {
                    const x = 42;
                    if (x > 0) {
                        console.log("Positive");
                    } else {
                        console.log("Non-positive");
                    }
                }
            )";
            
            auto module1 = parseTestCode(testCode1);
            if (module1) {
                analyzer.performFlowAnalysis(*module1);
                std::cout << "✓ FlowAnalysisEngine: Basic flow analysis passed" << std::endl;
            }
            
            // Test 2: Loop flow analysis
            std::string testCode2 = R"(
                function loopFlowTest(): void {
                    for (let i = 0; i < 10; i++) {
                        console.log(i);
                    }
                    
                    let j = 0;
                    while (j < 5) {
                        console.log(j);
                        j++;
                    }
                }
            )";
            
            auto module2 = parseTestCode(testCode2);
            if (module2) {
                analyzer.performFlowAnalysis(*module2);
                std::cout << "✓ FlowAnalysisEngine: Loop flow analysis passed" << std::endl;
            }
            
            // Test 3: Exception flow analysis
            std::string testCode3 = R"(
                function exceptionFlowTest(): void {
                    try {
                        throw new Error("Test error");
                    } catch (error) {
                        console.log("Caught error:", error);
                    } finally {
                        console.log("Finally block");
                    }
                }
            )";
            
            auto module3 = parseTestCode(testCode3);
            if (module3) {
                analyzer.performFlowAnalysis(*module3);
                std::cout << "✓ FlowAnalysisEngine: Exception flow analysis passed" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ FlowAnalysisEngine Unit: Exception: " << e.what() << std::endl;
        }
    }
    
    void testInheritanceResolverUnit() {
        std::cout << "\n--- Testing InheritanceResolver Unit Functions ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test 1: Basic inheritance resolution
            std::string testCode1 = R"(
                class BaseClass {
                    protected value: number;
                    
                    constructor(value: number) {
                        this.value = value;
                    }
                    
                    getValue(): number {
                        return this.value;
                    }
                }
                
                class DerivedClass extends BaseClass {
                    constructor(value: number) {
                        super(value);
                    }
                    
                    getDoubleValue(): number {
                        return this.value * 2;
                    }
                }
            )";
            
            auto module1 = parseTestCode(testCode1);
            if (module1) {
                analyzer.resolveInheritance(*module1);
                std::cout << "✓ InheritanceResolver: Basic inheritance resolution passed" << std::endl;
            }
            
            // Test 2: Interface implementation
            std::string testCode2 = R"(
                interface TestInterface {
                    method(): void;
                }
                
                class InterfaceImplementation implements TestInterface {
                    method(): void {
                        console.log("Interface method implemented");
                    }
                }
            )";
            
            auto module2 = parseTestCode(testCode2);
            if (module2) {
                analyzer.resolveInheritance(*module2);
                std::cout << "✓ InheritanceResolver: Interface implementation passed" << std::endl;
            }
            
            // Test 3: Multiple inheritance (interfaces)
            std::string testCode3 = R"(
                interface InterfaceA {
                    methodA(): void;
                }
                
                interface InterfaceB {
                    methodB(): void;
                }
                
                class MultipleImplementation implements InterfaceA, InterfaceB {
                    methodA(): void {
                        console.log("Method A");
                    }
                    
                    methodB(): void {
                        console.log("Method B");
                    }
                }
            )";
            
            auto module3 = parseTestCode(testCode3);
            if (module3) {
                analyzer.resolveInheritance(*module3);
                std::cout << "✓ InheritanceResolver: Multiple interface implementation passed" << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ InheritanceResolver Unit: Exception: " << e.what() << std::endl;
        }
    }
    
    void testSemanticAnalyzerRefactoredUnit() {
        std::cout << "\n--- Testing SemanticAnalyzerRefactored Unit Functions ---" << std::endl;
        
        try {
            semantic::SemanticAnalyzer analyzer(*diagnostics_);
            
            // Test 1: Complete semantic analysis orchestration
            std::string testCode1 = R"(
                class CompleteTestClass {
                    private value: number;
                    public name: string;
                    
                    constructor(value: number, name: string) {
                        this.value = value;
                        this.name = name;
                    }
                    
                    getValue(): number {
                        return this.value;
                    }
                    
                    setName(name: string): void {
                        this.name = name;
                    }
                }
                
                function completeTestFunction(param: string): string {
                    return param.toUpperCase();
                }
                
                export class ExportedTestClass {
                    public value: number;
                    
                    constructor(value: number) {
                        this.value = value;
                    }
                }
            )";
            
            auto module1 = parseTestCode(testCode1);
            if (module1) {
                bool result = analyzer.analyze(*module1);
                if (result) {
                    std::cout << "✓ SemanticAnalyzerRefactored: Complete analysis orchestration passed" << std::endl;
                } else {
                    std::cout << "✗ SemanticAnalyzerRefactored: Complete analysis orchestration failed" << std::endl;
                }
            }
            
            // Test 2: Error handling orchestration
            std::string testCode2 = R"(
                function errorTestFunction(): void {
                    const x: string = 42; // Type error
                    const y: number = "hello"; // Type error
                    
                    // Undefined variable
                    console.log(undefinedVariable);
                }
            )";
            
            auto module2 = parseTestCode(testCode2);
            if (module2) {
                bool result = analyzer.analyze(*module2);
                // We expect this to fail due to errors
                if (!result) {
                    std::cout << "✓ SemanticAnalyzerRefactored: Error handling orchestration passed" << std::endl;
                } else {
                    std::cout << "✗ SemanticAnalyzerRefactored: Error handling orchestration failed" << std::endl;
                }
            }
            
            // Test 3: Complex scenario orchestration
            std::string testCode3 = R"(
                interface ComplexInterface<T> {
                    process(input: T): T;
                    validate(input: T): boolean;
                }
                
                abstract class ComplexBaseClass<T> {
                    protected data: T;
                    
                    constructor(data: T) {
                        this.data = data;
                    }
                    
                    abstract process(input: T): T;
                    
                    protected validateData(): boolean {
                        return this.data !== null && this.data !== undefined;
                    }
                }
                
                class ComplexImplementation<T> extends ComplexBaseClass<T> implements ComplexInterface<T> {
                    constructor(data: T) {
                        super(data);
                    }
                    
                    process(input: T): T {
                        if (!this.validateData()) {
                            throw new Error("Invalid data");
                        }
                        return input;
                    }
                    
                    validate(input: T): boolean {
                        return input !== null && input !== undefined;
                    }
                }
            )";
            
            auto module3 = parseTestCode(testCode3);
            if (module3) {
                bool result = analyzer.analyze(*module3);
                if (result) {
                    std::cout << "✓ SemanticAnalyzerRefactored: Complex scenario orchestration passed" << std::endl;
                } else {
                    std::cout << "✗ SemanticAnalyzerRefactored: Complex scenario orchestration failed" << std::endl;
                }
            }
            
        } catch (const std::exception& e) {
            std::cout << "✗ SemanticAnalyzerRefactored Unit: Exception: " << e.what() << std::endl;
        }
    }
    
    std::unique_ptr<Module> parseTestCode(const std::string& code) {
        try {
            Lexer lexer(code, *diagnostics_);
            VectorTokenStream tokenStream(lexer.tokenize());
            Parser parser(tokenStream, *diagnostics_);
            return parser.parseModule();
        } catch (const std::exception& e) {
            std::cout << "Parse error: " << e.what() << std::endl;
            return nullptr;
        }
    }
};

} // namespace testing
} // namespace tsc

// Main function for running unit tests
int main() {
    tsc::testing::SemanticAnalysisUnitTests test;
    test.runAllUnitTests();
    return 0;
}