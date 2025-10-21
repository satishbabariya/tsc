// Regression test for source location handling in nested scopes
// This test exercises complex source location tracking across different scoping levels

// Global scope variables
let globalVar: number = 42;
let globalString: string = "global";

// Function with parameters (creates new scope)
function outerFunction(param1: number, param2: string): string {
    // Function scope variables
    let localVar: number = param1 * 2;
    let localString: string = param2 + " processed";
    
    // Nested function (creates another scope level)
    function innerFunction(nestedParam: number): number {
        // Inner function scope variables
        let innerVar: number = nestedParam + localVar;
        let innerString: string = localString + " inner";
        
        // Complex expressions requiring source location tracking
        if (innerVar > 0) {
            console.log("Inner positive:", innerVar);
            return innerVar * 2;
        } else {
            console.log("Inner non-positive:", innerVar);
            return 0;
        }
    }
    
    // Call nested function
    let result: number = innerFunction(localVar);
    
    // More complex expressions
    let complexResult: string = localString + " " + result.toString();
    
    return complexResult;
}

// Class with methods (creates multiple scopes)
class TestClass {
    private value: number;
    private name: string;
    
    constructor(initValue: number, initName: string) {
        this.value = initValue;
        this.name = initName;
    }
    
    // Method with source location tracking
    public processValue(multiplier: number): string {
        let processed: number = this.value * multiplier;
        let result: string = this.name + " processed: " + processed;
        
        // Nested conditional with source location
        if (processed > 100) {
            console.log("High value:", result);
            return result + " (high)";
        } else {
            console.log("Normal value:", result);
            return result + " (normal)";
        }
    }
    
    // Another method for additional scope testing
    public static createInstance(value: number, name: string): TestClass {
        return new TestClass(value, name);
    }
}

// Test complex scope interactions
let instance: TestClass = TestClass.createInstance(globalVar, globalString);
let processed: string = instance.processValue(3);

// Call outer function with complex parameters
let outerResult: string = outerFunction(globalVar, globalString);

// Test error scenarios that might trigger source location access
let errorTest: number = 0;
if (errorTest === 0) {
    console.log("Error test condition met");
    errorTest = 1;
}

// Test array and object access with source location
let testArray: number[] = [1, 2, 3, 4, 5];
let testObject = { 
    prop1: "value1", 
    prop2: 42, 
    prop3: true 
};

// Access array elements (requires source location for error reporting)
let arrayElement: number = testArray[2];
let objectProp: string = testObject.prop1;

// Complex nested expressions requiring source location tracking
let complexExpression: string = outerResult + " " + processed + " " + arrayElement.toString();

// This test should compile successfully and exercise all source location handling
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()