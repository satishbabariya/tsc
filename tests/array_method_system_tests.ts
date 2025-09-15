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
    console.log("=== Complex Array Operations Test ===");
    
    // Create processor for numbers
    let numberProcessor = new ArrayProcessor<number>([1, 2, 3]);
    
    // Test basic operations
    let initialLength = numberProcessor.getLength();
    console.log("Initial length:", initialLength);
    
    let newLength = numberProcessor.addItems(4, 5, 6);
    console.log("After adding items, length:", newLength);
    
    let removed = numberProcessor.removeLast();
    console.log("Removed item:", removed);
    
    let firstRemoved = numberProcessor.removeFirst();
    console.log("First removed item:", firstRemoved);
    
    let frontLength = numberProcessor.addToFront(0);
    console.log("After adding to front, length:", frontLength);
    
    // Test advanced operations
    let index = numberProcessor.findIndex(3);
    console.log("Index of 3:", index);
    
    let allData = numberProcessor.getAllData();
    console.log("All data:", allData);
    
    let concatenated = numberProcessor.concatWith([7, 8, 9]);
    console.log("Concatenated:", concatenated);
    
    let sliced = numberProcessor.sliceData(1, 4);
    console.log("Sliced (1-4):", sliced);
    
    let spliced = numberProcessor.spliceData(2, 1, 99, 100);
    console.log("Spliced result:", spliced);
}

// Test 4: Array operations with different data types
function testArrayOperationsWithDifferentTypes(): void {
    console.log("=== Different Types Array Operations Test ===");
    
    // String arrays
    let stringProcessor = new ArrayProcessor<string>(["hello", "world"]);
    let stringLength = stringProcessor.addItems("test", "array");
    console.log("String array length:", stringLength);
    
    // Boolean arrays
    let booleanProcessor = new ArrayProcessor<boolean>([true, false]);
    let booleanLength = booleanProcessor.addItems(true, false, true);
    console.log("Boolean array length:", booleanLength);
    
    // Mixed type arrays (using any)
    let mixedProcessor = new ArrayProcessor<any>([1, "hello", true]);
    let mixedLength = mixedProcessor.addItems(42, "world", false);
    console.log("Mixed array length:", mixedLength);
}

// Test 5: Advanced array operations with callbacks
function testAdvancedArrayOperations(): void {
    console.log("=== Advanced Array Operations Test ===");
    
    let numbers = new AdvancedArrayOperations<number>([1, 2, 3, 4, 5]);
    
    // Test forEach
    console.log("forEach test:");
    numbers.forEachItem((item, index) => {
        console.log(`  Item ${index}: ${item}`);
    });
    
    // Test map
    let doubled = numbers.mapItems((item) => item * 2);
    console.log("Mapped (doubled):", doubled);
    
    // Test filter
    let evens = numbers.filterItems((item) => item % 2 === 0);
    console.log("Filtered (even):", evens);
    
    // Test reduce
    let sum = numbers.reduceItems((acc, item) => acc + item, 0);
    console.log("Reduced (sum):", sum);
    
    // Test with strings
    let strings = new AdvancedArrayOperations<string>(["hello", "world", "test"]);
    let upperCased = strings.mapItems((str) => str.toUpperCase());
    console.log("Uppercased strings:", upperCased);
    
    let longStrings = strings.filterItems((str) => str.length > 4);
    console.log("Long strings:", longStrings);
}

// Test 6: Array method performance and edge cases
function testArrayMethodEdgeCases(): void {
    console.log("=== Array Method Edge Cases Test ===");
    
    // Empty array operations
    let emptyProcessor = new ArrayProcessor<number>();
    console.log("Empty array length:", emptyProcessor.getLength());
    console.log("Empty array pop:", emptyProcessor.removeLast());
    console.log("Empty array shift:", emptyProcessor.removeFirst());
    
    // Single element array
    let singleProcessor = new ArrayProcessor<number>([42]);
    console.log("Single element length:", singleProcessor.getLength());
    console.log("Single element pop:", singleProcessor.removeLast());
    
    // Large array operations
    let largeProcessor = new ArrayProcessor<number>();
    for (let i = 0; i < 100; i++) {
        largeProcessor.addItems(i);
    }
    console.log("Large array length:", largeProcessor.getLength());
    
    let largeSlice = largeProcessor.sliceData(10, 20);
    console.log("Large array slice length:", largeSlice.length);
}

// Test 7: Array method type safety and error handling
function testArrayMethodTypeSafety(): void {
    console.log("=== Array Method Type Safety Test ===");
    
    let numberProcessor = new ArrayProcessor<number>([1, 2, 3]);
    
    // Test type-safe operations
    let length: number = numberProcessor.getLength();
    let popped: number | undefined = numberProcessor.removeLast();
    let shifted: number | undefined = numberProcessor.removeFirst();
    
    console.log("Type-safe length:", length);
    console.log("Type-safe pop result:", popped);
    console.log("Type-safe shift result:", shifted);
    
    // Test array method chaining
    let result = numberProcessor
        .addItems(4, 5)
        .toString();
    console.log("Method chaining result:", result);
}

// Main test runner
function runSystemTests(): void {
    console.log("Starting Array Method System Tests...");
    
    performComplexArrayOperations();
    testArrayOperationsWithDifferentTypes();
    testAdvancedArrayOperations();
    testArrayMethodEdgeCases();
    testArrayMethodTypeSafety();
    
    console.log("All system tests completed successfully!");
}

// Execute system tests
runSystemTests();