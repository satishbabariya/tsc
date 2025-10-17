// Syntax validation test - tests the most common syntax patterns
// This test focuses on the syntax that should work based on our error analysis

// Test 1: Basic variable declarations (most common working pattern)
let numberVar: number = 42;
let stringVar: string = "Hello";
let booleanVar: boolean = true;

// Test 2: Simple function declarations
function add(a: number, b: number): number {
    return a + b;
}

function greet(name: string): string {
    return "Hello, " + name;
}

// Test 3: Basic class with simple syntax
class SimpleClass {
    private value: number;

    constructor(initialValue: number) {
        this.value = initialValue;
    }

    getValue(): number {
        return this.value;
    }

    setValue(newValue: number): void {
        this.value = newValue;
    }
}

// Test 4: Generic class with simple syntax (our working feature)
class SimpleGeneric<T> {
    private item: T;

    constructor(item: T) {
        this.item = item;
    }

    getItem(): T {
        return this.item;
    }
}

// Test 5: Basic conditional statements
function isPositive(n: number): boolean {
    if (n > 0) {
        return true;
    } else {
        return false;
    }
}

// Test 6: Simple loops
function sumArray(arr: number[]): number {
    let sum = 0;
    for (let i = 0; i < arr.length; i++) {
        sum += arr[i];
    }
    return sum;
}

// Test 7: Method calls (our working feature)
function testMethodCalls(): void {
    let obj = new SimpleClass(100);
    let value = obj.getValue();
    obj.setValue(200);
    let newValue = obj.getValue();
    
    _print("Method calls test:", value, newValue);
}

// Test 8: Generic method calls (our working feature)
function testGenericCalls(): void {
    let numberBox = new SimpleGeneric<number>(42);
    let stringBox = new SimpleGeneric<string>("Test");
    
    let numValue = numberBox.getItem();
    let strValue = stringBox.getItem();
    
    _print("Generic calls test:", numValue, strValue);
}

// Main test execution
function runSyntaxValidationTests(): void {
    _print("=== Syntax Validation Test ===");
    
    // Test basic operations
    let sum = add(10, 20);
    _print("Addition test:", sum);
    
    let greeting = greet("TypeScript");
    _print("Greeting test:", greeting);
    
    // Test conditionals
    let positive = isPositive(15);
    _print("Conditional test:", positive);
    
    // Test arrays
    let numbers = [1, 2, 3, 4, 5];
    let arraySum = sumArray(numbers);
    _print("Array test:", arraySum);
    
    // Test method calls
    testMethodCalls();
    
    // Test generic calls
    testGenericCalls();
    
    _print("=== All Syntax Tests Completed ===");
}

// Execute tests
runSyntaxValidationTests();