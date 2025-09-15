// System tests for end-to-end scenarios with array method support
// These tests verify complete compilation and execution of complex array operations

// Test 1: Complete array manipulation workflow
class ArrayProcessor<T> {
    private data: Array<T>;
    
    constructor(initialData: Array<T> = []) {
        this.data = initialData;
    }
    
    addItems(...items: T[]): number {
        return this.data.push(...items);
    }
    
    removeLast(): T | undefined {
        return this.data.pop();
    }
    
    removeFirst(): T | undefined {
        return this.data.shift();
    }
    
    addToFront(...items: T[]): number {
        return this.data.unshift(...items);
    }
    
    getLength(): number {
        return this.data.length;
    }
    
    getAllData(): Array<T> {
        return this.data.slice();
    }
    
    findIndex(item: T): number {
        return this.data.indexOf(item);
    }
    
    concatWith(other: Array<T>): Array<T> {
        return this.data.concat(other);
    }
    
    sliceData(start: number, end?: number): Array<T> {
        return this.data.slice(start, end);
    }
    
    spliceData(start: number, deleteCount: number, ...items: T[]): Array<T> {
        return this.data.splice(start, deleteCount, ...items);
    }
}

// Test 2: Advanced array operations with callbacks
class AdvancedArrayOperations<T> {
    private items: Array<T>;
    
    constructor(items: Array<T> = []) {
        this.items = items;
    }
    
    forEachItem(callback: (item: T, index: number) => void): void {
        this.items.forEach(callback);
    }
    
    mapItems<U>(callback: (item: T, index: number) => U): Array<U> {
        return this.items.map(callback);
    }
    
    filterItems(predicate: (item: T, index: number) => boolean): Array<T> {
        return this.items.filter(predicate);
    }
    
    reduceItems<U>(callback: (accumulator: U, item: T, index: number) => U, initialValue: U): U {
        return this.items.reduce(callback, initialValue);
    }
}

// Test 3: Array method composition and complex operations
function performComplexArrayOperations(): void {
    _print("=== Complex Array Operations Test ===");
    
    // Create processor for numbers
    let numberProcessor = new ArrayProcessor<number>([1, 2, 3]);
    
    // Test basic operations
    let initialLength = numberProcessor.getLength();
    _print("Initial length:", initialLength);
    
    let newLength = numberProcessor.addItems(4, 5, 6);
    _print("After adding items, length:", newLength);
    
    let removed = numberProcessor.removeLast();
    _print("Removed item:", removed);
    
    let firstRemoved = numberProcessor.removeFirst();
    _print("First removed item:", firstRemoved);
    
    let frontLength = numberProcessor.addToFront(0);
    _print("After adding to front, length:", frontLength);
    
    // Test advanced operations
    let index = numberProcessor.findIndex(3);
    _print("Index of 3:", index);
    
    let allData = numberProcessor.getAllData();
    _print("All data:", allData);
    
    let concatenated = numberProcessor.concatWith([7, 8, 9]);
    _print("Concatenated:", concatenated);
    
    let sliced = numberProcessor.sliceData(1, 4);
    _print("Sliced (1-4):", sliced);
    
    let spliced = numberProcessor.spliceData(2, 1, 99, 100);
    _print("Spliced result:", spliced);
}

// Test 4: Array operations with different data types
function testArrayOperationsWithDifferentTypes(): void {
    _print("=== Different Types Array Operations Test ===");
    
    // String arrays
    let stringProcessor = new ArrayProcessor<string>(["hello", "world"]);
    let stringLength = stringProcessor.addItems("test", "array");
    _print("String array length:", stringLength);
    
    // Boolean arrays
    let booleanProcessor = new ArrayProcessor<boolean>([true, false]);
    let booleanLength = booleanProcessor.addItems(true, false, true);
    _print("Boolean array length:", booleanLength);
    
    // Mixed type arrays (using any)
    let mixedProcessor = new ArrayProcessor<any>([1, "hello", true]);
    let mixedLength = mixedProcessor.addItems(42, "world", false);
    _print("Mixed array length:", mixedLength);
}

// Test 5: Advanced array operations with callbacks
function testAdvancedArrayOperations(): void {
    _print("=== Advanced Array Operations Test ===");
    
    let numbers = new AdvancedArrayOperations<number>([1, 2, 3, 4, 5]);
    
    // Test forEach
    _print("forEach test:");
    numbers.forEachItem((item, index) => {
        _print(`  Item ${index}: ${item}`);
    });
    
    // Test map
    let doubled = numbers.mapItems((item) => item * 2);
    _print("Mapped (doubled):", doubled);
    
    // Test filter
    let evens = numbers.filterItems((item) => item % 2 === 0);
    _print("Filtered (even):", evens);
    
    // Test reduce
    let sum = numbers.reduceItems((acc, item) => acc + item, 0);
    _print("Reduced (sum):", sum);
    
    // Test with strings
    let strings = new AdvancedArrayOperations<string>(["hello", "world", "test"]);
    let upperCased = strings.mapItems((str) => str.toUpperCase());
    _print("Uppercased strings:", upperCased);
    
    let longStrings = strings.filterItems((str) => str.length > 4);
    _print("Long strings:", longStrings);
}

// Test 6: Array method performance and edge cases
function testArrayMethodEdgeCases(): void {
    _print("=== Array Method Edge Cases Test ===");
    
    // Empty array operations
    let emptyProcessor = new ArrayProcessor<number>();
    _print("Empty array length:", emptyProcessor.getLength());
    _print("Empty array pop:", emptyProcessor.removeLast());
    _print("Empty array shift:", emptyProcessor.removeFirst());
    
    // Single element array
    let singleProcessor = new ArrayProcessor<number>([42]);
    _print("Single element length:", singleProcessor.getLength());
    _print("Single element pop:", singleProcessor.removeLast());
    
    // Large array operations
    let largeProcessor = new ArrayProcessor<number>();
    for (let i = 0; i < 100; i++) {
        largeProcessor.addItems(i);
    }
    _print("Large array length:", largeProcessor.getLength());
    
    let largeSlice = largeProcessor.sliceData(10, 20);
    _print("Large array slice length:", largeSlice.length);
}

// Test 7: Array method type safety and error handling
function testArrayMethodTypeSafety(): void {
    _print("=== Array Method Type Safety Test ===");
    
    let numberProcessor = new ArrayProcessor<number>([1, 2, 3]);
    
    // Test type-safe operations
    let length: number = numberProcessor.getLength();
    let popped: number | undefined = numberProcessor.removeLast();
    let shifted: number | undefined = numberProcessor.removeFirst();
    
    _print("Type-safe length:", length);
    _print("Type-safe pop result:", popped);
    _print("Type-safe shift result:", shifted);
    
    // Test array method chaining
    let result = numberProcessor
        .addItems(4, 5)
        .toString();
    _print("Method chaining result:", result);
}

// Main test runner
function runSystemTests(): void {
    _print("Starting Array Method System Tests...");
    
    performComplexArrayOperations();
    testArrayOperationsWithDifferentTypes();
    testAdvancedArrayOperations();
    testArrayMethodEdgeCases();
    testArrayMethodTypeSafety();
    
    _print("All system tests completed successfully!");
}

// Execute system tests
runSystemTests();