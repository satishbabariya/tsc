// Integration test for refactored semantic analysis modules
// This test validates the interaction between SymbolResolutionEngine, TypeCheckingEngine, and SemanticAnalyzerRefactored

// Test 1: Symbol Resolution Engine Integration
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
    
    setName(name: string): void {
        this.name = name;
    }
}

// Test 2: Type Checking Engine Integration with Complex Types
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
    
    getData(): T {
        return this.data;
    }
}

// Test 3: Inheritance Resolution Integration
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

// Test 4: Complex Type Inference and Checking
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

// Test 5: Array Type Assignment (from extracted implementation)
function testArrayAssignment(): void {
    const stringArray: string[] = [];
    const numberArray: number[] = [];
    const genericArray: Array<any> = [];
    
    // Test empty array assignment (should work with extracted logic)
    const emptyStringArray: string[] = [];
    const emptyNumberArray: number[] = [];
    
    // Test generic array assignment
    const genericStringArray: Array<string> = [];
    const genericNumberArray: Array<number> = [];
}

// Test 6: Export Declaration Integration
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

export const exportedConstant = "test";

// Test 7: Function Overloading Integration
function overloadedFunction(input: string): string;
function overloadedFunction(input: number): number;
function overloadedFunction(input: string | number): string | number {
    if (typeof input === "string") {
        return input.toUpperCase();
    } else {
        return input * 2;
    }
}

// Test 8: Complex Generic Constraints
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

// Test 9: Type Parameter Variance
class CovariantProcessor<T> {
    private data: T;
    
    constructor(data: T) {
        this.data = data;
    }
    
    getData(): T {
        return this.data;
    }
    
    process<U extends T>(processor: (input: T) => U): CovariantProcessor<U> {
        return new CovariantProcessor<U>(processor(this.data));
    }
}

// Test 10: Error Handling Integration
function errorHandlingTest(): void {
    try {
        const processor = new StringProcessor({});
        const result = processor.process("test");
        console.log("Result:", result);
    } catch (error) {
        console.error("Error caught:", error);
    }
}

// Test 11: Complex Nested Types
interface NestedInterface {
    level1: {
        level2: {
            level3: {
                value: string;
                process: (input: string) => string;
            };
        };
    };
}

class NestedImplementation implements NestedInterface {
    level1 = {
        level2: {
            level3: {
                value: "nested",
                process: (input: string) => input.toUpperCase()
            }
        }
    };
}

// Test 12: Module Integration
import { exportedFunction as importedFunction } from "./semantic_analysis_modular_integration";

// Main test execution
function runSemanticAnalysisIntegrationTests(): void {
    console.log("=== Semantic Analysis Modular Integration Tests ===");
    
    // Test 1: Basic class instantiation and method calls
    const testInstance = new TestClass(42, "test");
    console.log("Test class value:", testInstance.getValue());
    testInstance.setName("updated");
    console.log("Test class name:", testInstance.name);
    
    // Test 2: Generic interface implementation
    const genericImpl = new GenericImplementation<string>("test");
    const processed = genericImpl.process("input");
    const isValid = genericImpl.validate("input");
    console.log("Generic processed:", processed, "Valid:", isValid);
    
    // Test 3: Inheritance resolution
    const stringProcessor = new StringProcessor({ enabled: true });
    const numberProcessor = new NumberProcessor({ multiplier: 2 });
    
    const stringResult = stringProcessor.process("hello");
    const numberResult = numberProcessor.process(21);
    console.log("String result:", stringResult, "Number result:", numberResult);
    
    // Test 4: Complex function with type inference
    const result1 = complexFunction("test", (s) => s.toUpperCase(), (s) => s.length > 0);
    const result2 = complexFunction(42, (n) => n * 2, (n) => n > 0);
    console.log("Complex function results:", result1, result2);
    
    // Test 5: Array assignment
    testArrayAssignment();
    
    // Test 6: Function overloading
    const overloadedResult1 = overloadedFunction("hello");
    const overloadedResult2 = overloadedFunction(21);
    console.log("Overloaded results:", overloadedResult1, overloadedResult2);
    
    // Test 7: Generic constraints
    const constraintImpl = new ConstraintImplementation();
    constrainedFunction(constraintImpl);
    
    // Test 8: Type parameter variance
    const covariantProcessor = new CovariantProcessor<string>("test");
    const processedProcessor = covariantProcessor.process((s) => s.toUpperCase());
    console.log("Covariant processed:", processedProcessor.getData());
    
    // Test 9: Error handling
    errorHandlingTest();
    
    // Test 10: Nested types
    const nestedImpl = new NestedImplementation();
    const nestedResult = nestedImpl.level1.level2.level3.process("test");
    console.log("Nested result:", nestedResult);
    
    console.log("=== All Semantic Analysis Integration Tests Completed ===");
}

// Export for module testing
export { runSemanticAnalysisIntegrationTests };

// Run tests if this is the main module
if (require.main === module) {
    runSemanticAnalysisIntegrationTests();
}