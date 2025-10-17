// Critical functionality test - covers the most important compiler features
// This test validates the core functionality that should always work

// Test 1: Basic variable declarations and assignments
let numberVar: number = 42;
let stringVar: string = "Hello World";
let booleanVar: boolean = true;

// Test 2: Function definitions and calls
function addNumbers(a: number, b: number): number {
    return a + b;
}

function greet(name: string): string {
    return "Hello, " + name + "!";
}

// Test 3: Class definitions with methods
class Calculator {
    private value: number;

    constructor(initialValue: number) {
        this.value = initialValue;
    }

    add(n: number): number {
        this.value += n;
        return this.value;
    }

    getValue(): number {
        return this.value;
    }
}

// Test 4: Generic class (our recent fix)
class GenericBox<T> {
    private item: T;

    constructor(item: T) {
        this.item = item;
    }

    getItem(): T {
        return this.item;
    }

    setItem(newItem: T): void {
        this.item = newItem;
    }
}

// Test 5: Array operations
function processArray(arr: number[]): number {
    let sum = 0;
    for (let i = 0; i < arr.length; i++) {
        sum += arr[i];
    }
    return sum;
}

// Test 6: Conditional statements
function getMax(a: number, b: number): number {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Test 7: Loops
function factorial(n: number): number {
    let result = 1;
    for (let i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Main test execution
function runCriticalTests(): void {
    _print("=== Critical Functionality Test ===");
    
    // Test basic operations
    let sum = addNumbers(10, 20);
    _print("Addition test:", sum);
    
    let greeting = greet("TypeScript");
    _print("String test:", greeting);
    
    // Test class operations
    let calc = new Calculator(100);
    let result = calc.add(50);
    _print("Calculator test:", result);
    
    // Test generic operations
    let numberBox = new GenericBox<number>(42);
    let boxValue = numberBox.getItem();
    _print("Generic number test:", boxValue);
    
    let stringBox = new GenericBox<string>("Generic String");
    let boxString = stringBox.getItem();
    _print("Generic string test:", boxString);
    
    // Test array operations
    let numbers = [1, 2, 3, 4, 5];
    let arraySum = processArray(numbers);
    _print("Array test:", arraySum);
    
    // Test conditionals
    let maxValue = getMax(15, 25);
    _print("Conditional test:", maxValue);
    
    // Test loops
    let fact = factorial(5);
    _print("Loop test (5!):", fact);
    
    _print("=== All Critical Tests Completed ===");
}

// Execute tests
runCriticalTests();