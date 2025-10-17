// Minimal working test - only uses syntax that we know works
// This test focuses on the core functionality that compiles successfully

// Test 1: Basic variables
let numberVar: number = 42;
let stringVar: string = "Hello";

// Test 2: Simple function
function add(a: number, b: number): number {
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
}

// Test 4: Generic class (our working feature)
class SimpleGeneric<T> {
    private item: T;

    constructor(item: T) {
        this.item = item;
    }

    getItem(): T {
        return this.item;
    }
}

// Test 5: Conditional
function isPositive(n: number): boolean {
    if (n > 0) {
        return true;
    } else {
        return false;
    }
}

// Main test execution
function runMinimalTests(): void {
    _print("=== Minimal Working Test ===");
    
    // Test basic operations
    let sum = add(10, 20);
    _print("Addition test:", sum);
    
    // Test class operations
    let obj = new SimpleClass(100);
    let value = obj.getValue();
    _print("Class test:", value);
    
    // Test generic operations
    let numberBox = new SimpleGeneric<number>(42);
    let boxValue = numberBox.getItem();
    _print("Generic number test:", boxValue);
    
    let stringBox = new SimpleGeneric<string>("Test");
    let strValue = stringBox.getItem();
    _print("Generic string test:", strValue);
    
    // Test conditionals
    let positive = isPositive(15);
    _print("Conditional test:", positive);
    
    _print("=== All Tests Completed ===");
}

// Execute tests
runMinimalTests();