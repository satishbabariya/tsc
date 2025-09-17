// Integration tests for array method support
// These tests cover various combinations of array methods and generic types

// Test 1: Array method chaining
function testArrayChaining(): void {
    let numbers: Array<number> = [1, 2, 3];

    // Chain multiple array operations
    let result = numbers
        .push(4, 5)           // Returns new length
        .toString();          // Convert to string

    _print("Array chaining result:", result);
}

// Test 2: Generic array with different element types
class GenericArrayContainer<T> {
    items: Array<T>;

    constructor(initialItems: Array<T> = []) {
        this.items = initialItems;
    }

    addItem(item: T): number {
        return this.items.push(item);
    }

    removeItem(): T | undefined {
        return this.items.pop();
    }

    getLength(): number {
        return this.items.length;
    }

    getAllItems(): Array<T> {
        return this.items.slice(); // Return copy
    }
}

// Test 3: Array methods with function parameters
function testArrayMethodsWithFunctions(): void {
    let numbers: Array<number> = [1, 2, 3, 4, 5];

    // Test forEach
    numbers.forEach((num) => {
        _print("Number:", num);
    });

    // Test map
    let doubled: Array<number> = numbers.map((num) => num * 2);
    _print("Doubled:", doubled);

    // Test filter
    let evens: Array<number> = numbers.filter((num) => num % 2 === 0);
    _print("Even numbers:", evens);

    // Test reduce
    let sum: number = numbers.reduce((acc, num) => acc + num, 0);
    _print("Sum:", sum);
}

// Test 4: Array methods with complex generic types
interface User {
    id: number;
    name: string;
    active: boolean;
}

function testComplexGenericArrays(): void {
    let users: Array<User> = [
        {id: 1, name: "Alice", active: true},
        {id: 2, name: "Bob", active: false},
        {id: 3, name: "Charlie", active: true}
    ];

    // Test array methods on complex objects
    let activeUsers: Array<User> = users.filter(user => user.active);
    let userNames: Array<string> = users.map(user => user.name);
    let totalUsers: number = users.reduce((count, user) => count + 1, 0);

    _print("Active users count:", activeUsers.length);
    _print("User names:", userNames);
    _print("Total users:", totalUsers);
}

// Test 5: Array methods with type parameters
function processArray<T>(items: Array<T>, processor: (item: T) => T): Array<T> {
    return items.map(processor);
}

function testGenericArrayProcessing(): void {
    let numbers: Array<number> = [1, 2, 3, 4, 5];
    let strings: Array<string> = ["hello", "world", "test"];

    let processedNumbers = processArray(numbers, (n) => n * 2);
    let processedStrings = processArray(strings, (s) => s.toUpperCase());

    _print("Processed numbers:", processedNumbers);
    _print("Processed strings:", processedStrings);
}

// Test 6: Array method error handling
function testArrayMethodErrors(): void {
    let numbers: Array<number> = [];

    // Test operations on empty array
    let length = numbers.length;  // Should be 0
    let popped = numbers.pop();   // Should be undefined
    let shifted = numbers.shift(); // Should be undefined

    _print("Empty array length:", length);
    _print("Popped from empty:", popped);
    _print("Shifted from empty:", shifted);
}

// Test 7: Array methods with different array sizes
function testDifferentArraySizes(): void {
    // Small array
    let small: Array<number> = [1];
    let smallLength = small.length;
    let smallPop = small.pop();

    // Large array simulation
    let large: Array<number> = [];
    for (let i = 0; i < 1000; i++) {
        large.push(i);
    }
    let largeLength = large.length;
    let largeSlice = large.slice(0, 10);

    _print("Small array operations completed");
    _print("Large array operations completed");
}

// Run all tests
function runIntegrationTests(): void {
    _print("Running array method integration tests...");

    testArrayChaining();

    let numberContainer = new GenericArrayContainer<number>([1, 2, 3]);
    let stringContainer = new GenericArrayContainer<string>(["a", "b", "c"]);

    _print("Number container length:", numberContainer.getLength());
    _print("String container length:", stringContainer.getLength());

    testArrayMethodsWithFunctions();
    testComplexGenericArrays();
    testGenericArrayProcessing();
    testArrayMethodErrors();
    testDifferentArraySizes();

    _print("All integration tests completed successfully!");
}

// Execute tests
runIntegrationTests();