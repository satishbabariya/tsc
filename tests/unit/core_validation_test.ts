// Core validation test - tests the essential working functionality
// This test validates the core features that are known to work

// Test 1: Basic variables and types
let numberVar: number = 42;
let stringVar: string = "Hello World";

// Test 2: Simple function
function addNumbers(a: number, b: number): number {
    return a + b;
}

// Test 3: Simple class
class SimpleClass {
    private value: number;

    constructor(initialValue: number) {
        this.value = initialValue;
    }

    getValue(): number {
        return this.value;
    }

    add(n: number): number {
        this.value += n;
        return this.value;
    }
}

// Test 4: Generic class (our main fix)
class GenericBox<T> {
    private item: T;

    constructor(item: T) {
        this.item = item;
    }

    getItem(): T {
        return this.item;
    }
}

// Test 5: Conditional logic
function isPositive(n: number): boolean {
    if (n > 0) {
        return true;
    } else {
        return false;
    }
}

// Main test execution
function runCoreTests(): void {
    _print("=== Core Validation Test ===");
    
    // Test basic operations
    let sum = addNumbers(10, 20);
    _print("Addition test:", sum);
    
    // Test class operations
    let obj = new SimpleClass(100);
    let result = obj.add(50);
    _print("Class test:", result);
    
    // Test generic operations
    let numberBox = new GenericBox<number>(42);
    let boxValue = numberBox.getItem();
    _print("Generic number test:", boxValue);
    
    let stringBox = new GenericBox<string>("Generic String");
    let boxString = stringBox.getItem();
    _print("Generic string test:", boxString);
    
    // Test conditionals
    let positive = isPositive(15);
    _print("Conditional test:", positive);
    
    _print("=== Core Tests Completed ===");
}

// Execute tests
runCoreTests();