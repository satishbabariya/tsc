// Comprehensive Feature Test Suite
// Tests all implemented functionality in the TypeScript compiler

// ============================================================================
// TEST 1: Basic TypeScript Features
// ============================================================================

// Variables and types
let numberVar: number = 42;
let stringVar: string = "Hello World";
let booleanVar: boolean = true;

// Functions
function addNumbers(a: number, b: number): number {
    return a + b;
}

function greet(name: string): string {
    return "Hello, " + name + "!";
}

// ============================================================================
// TEST 2: Class Operations
// ============================================================================

class BasicClass {
    private value: number;
    private name: string;

    constructor(value: number, name: string) {
        this.value = value;
        this.name = name;
    }

    getValue(): number {
        return this.value;
    }

    getName(): string {
        return this.name;
    }

    setValue(newValue: number): void {
        this.value = newValue;
    }

    toString(): string {
        return `${this.name}: ${this.value}`;
    }
}

// ============================================================================
// TEST 3: Generic Classes (Our Main Feature)
// ============================================================================

class GenericContainer<T> {
    private item: T;
    private id: number;

    constructor(item: T, id: number) {
        this.item = item;
        this.id = id;
    }

    getItem(): T {
        return this.item;
    }

    getId(): number {
        return this.id;
    }

    setItem(newItem: T): void {
        this.item = newItem;
    }
}

class GenericPair<A, B> {
    first: A;
    second: B;

    constructor(first: A, second: B) {
        this.first = first;
        this.second = second;
    }

    getFirst(): A {
        return this.first;
    }

    getSecond(): B {
        return this.second;
    }

    swap(): GenericPair<B, A> {
        return new GenericPair<B, A>(this.second, this.first);
    }
}

// ============================================================================
// TEST 4: Method Calls and Property Access
// ============================================================================

class MethodTestClass {
    private data: number;

    constructor(data: number) {
        this.data = data;
    }

    getData(): number {
        return this.data;
    }

    setData(newData: number): void {
        this.data = newData;
    }

    doubleData(): number {
        return this.data * 2;
    }

    addToData(value: number): number {
        this.data += value;
        return this.data;
    }
}

// ============================================================================
// TEST 5: Conditional Logic
// ============================================================================

function isPositive(n: number): boolean {
    if (n > 0) {
        return true;
    } else {
        return false;
    }
}

function getMax(a: number, b: number): number {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

function isEven(n: number): boolean {
    return n % 2 === 0;
}

// ============================================================================
// TEST 6: Variadic Function Support (Our Fix)
// ============================================================================

function testVariadicFunction(): void {
    _print("Testing variadic function with 1 argument");
    _print("Testing variadic function with 2 arguments", 42);
    _print("Testing variadic function with 3 arguments", "Hello", 123, true);
}

// ============================================================================
// MAIN TEST EXECUTION
// ============================================================================

function runComprehensiveTests(): void {
    _print("=== COMPREHENSIVE FEATURE TEST SUITE ===");
    
    // Test 1: Basic operations
    _print("--- Test 1: Basic Operations ---");
    let sum = addNumbers(10, 20);
    _print("Addition result:", sum);
    
    let greeting = greet("TypeScript Compiler");
    _print("Greeting result:", greeting);
    
    // Test 2: Class operations
    _print("--- Test 2: Class Operations ---");
    let basicObj = new BasicClass(100, "TestObject");
    _print("Basic class value:", basicObj.getValue());
    _print("Basic class name:", basicObj.getName());
    
    basicObj.setValue(200);
    _print("Updated value:", basicObj.getValue());
    _print("String representation:", basicObj.toString());
    
    // Test 3: Generic classes
    _print("--- Test 3: Generic Classes ---");
    let numberContainer = new GenericContainer<number>(42, 1);
    _print("Number container item:", numberContainer.getItem());
    _print("Number container ID:", numberContainer.getId());
    
    let stringContainer = new GenericContainer<string>("Hello", 2);
    _print("String container item:", stringContainer.getItem());
    _print("String container ID:", stringContainer.getId());
    
    let pair = new GenericPair<number, string>(42, "test");
    _print("Pair first:", pair.getFirst());
    _print("Pair second:", pair.getSecond());
    
    let swapped = pair.swap();
    _print("Swapped first:", swapped.getFirst());
    _print("Swapped second:", swapped.getSecond());
    
    // Test 4: Method calls
    _print("--- Test 4: Method Calls ---");
    let methodObj = new MethodTestClass(50);
    _print("Initial data:", methodObj.getData());
    
    let doubled = methodObj.doubleData();
    _print("Doubled data:", doubled);
    
    let added = methodObj.addToData(25);
    _print("Added data:", added);
    
    // Test 5: Conditional logic
    _print("--- Test 5: Conditional Logic ---");
    _print("Is 15 positive?", isPositive(15));
    _print("Is -5 positive?", isPositive(-5));
    _print("Max of 10 and 20:", getMax(10, 20));
    _print("Is 8 even?", isEven(8));
    _print("Is 7 even?", isEven(7));
    
    // Test 6: Variadic functions
    _print("--- Test 6: Variadic Functions ---");
    testVariadicFunction();
    
    _print("=== ALL TESTS COMPLETED SUCCESSFULLY ===");
}

// Execute the comprehensive test suite
runComprehensiveTests();