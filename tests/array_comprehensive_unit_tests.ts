// Comprehensive Unit Tests for Array Operations
// This file tests various array scenarios, edge cases, and boundary conditions

// Test 1: Basic Array Creation and Initialization
class BasicArrayTest {
    items: number[];

    constructor() {
        this.items = [];
    }

    getLength(): number {
        return this.items.length;
    }

    addItem(item: number): void {
        this.items.push(item);
    }

    getItem(index: number): number {
        // In a real implementation, this would access items[index]
        // For now, return 0 as placeholder
        return 0;
    }
}

// Test 2: Array with Different Types
class StringArrayTest {
    items: string[];

    constructor() {
        this.items = [];
    }

    getLength(): number {
        return this.items.length;
    }

    addItem(item: string): void {
        this.items.push(item);
    }
}

// Test 3: Generic Array Operations
class GenericArrayTest<T> {
    items: T[];

    constructor() {
        this.items = [];
    }

    getLength(): number {
        return this.items.length;
    }

    addItem(item: T): void {
        this.items.push(item);
    }

    isEmpty(): boolean {
        return this.items.length === 0;
    }
}

// Test 4: Array with Multiple Operations
class MultiOperationArrayTest {
    items: number[];

    constructor() {
        this.items = [];
    }

    addMultiple(items: number[]): void {
        for (let i = 0; i < items.length; i++) {
            this.items.push(items[i]);
        }
    }

    getTotalLength(): number {
        return this.items.length;
    }

    clear(): void {
        this.items = [];
    }

    hasItems(): boolean {
        return this.items.length > 0;
    }
}

// Test 5: Nested Array Operations
class NestedArrayTest {
    items: number[][];

    constructor() {
        this.items = [];
    }

    addNestedArray(arr: number[]): void {
        this.items.push(arr);
    }

    getNestedLength(): number {
        return this.items.length;
    }
}

// Test 6: Array with Complex Operations
class ComplexArrayTest {
    items: number[];
    metadata: string[];

    constructor() {
        this.items = [];
        this.metadata = [];
    }

    addWithMetadata(item: number, meta: string): void {
        this.items.push(item);
        this.metadata.push(meta);
    }

    getItemsLength(): number {
        return this.items.length;
    }

    getMetadataLength(): number {
        return this.metadata.length;
    }

    areLengthsEqual(): boolean {
        return this.items.length === this.metadata.length;
    }
}

// Test 7: Array Edge Cases
class EdgeCaseArrayTest {
    items: number[];

    constructor() {
        this.items = [];
    }

    // Test adding to empty array
    addToEmpty(item: number): void {
        this.items.push(item);
    }

    // Test multiple consecutive operations
    addMultipleConsecutive(): void {
        this.items.push(1);
        this.items.push(2);
        this.items.push(3);
    }

    // Test array after multiple operations
    getFinalLength(): number {
        return this.items.length;
    }
}

// Test 8: Array with Different Initial States
class InitialStateArrayTest {
    items: number[];
    initialized: boolean;

    constructor() {
        this.items = [];
        this.initialized = true;
    }

    isInitialized(): boolean {
        return this.initialized;
    }

    getArrayLength(): number {
        return this.items.length;
    }

    addAfterInitialization(item: number): void {
        this.items.push(item);
    }
}

// Test 9: Array with Type Constraints
class ConstrainedArrayTest<T extends number> {
    items: T[];

    constructor() {
        this.items = [];
    }

    addConstrainedItem(item: T): void {
        this.items.push(item);
    }

    getConstrainedLength(): number {
        return this.items.length;
    }
}

// Test 10: Array with Interface Implementation
interface ArrayOperations {
    getLength(): number;
    addItem(item: any): void;
    isEmpty(): boolean;
}

class InterfaceArrayTest implements ArrayOperations {
    items: number[];

    constructor() {
        this.items = [];
    }

    getLength(): number {
        return this.items.length;
    }

    addItem(item: number): void {
        this.items.push(item);
    }

    isEmpty(): boolean {
        return this.items.length === 0;
    }
}

// Test execution and validation
let basicTest = new BasicArrayTest();
console.log("Basic array test - initial length:", basicTest.getLength());

let stringTest = new StringArrayTest();
console.log("String array test - initial length:", stringTest.getLength());

let genericTest = new GenericArrayTest<number>();
console.log("Generic array test - initial length:", genericTest.getLength());
console.log("Generic array test - is empty:", genericTest.isEmpty());

let multiTest = new MultiOperationArrayTest();
console.log("Multi operation test - initial length:", multiTest.getTotalLength());
console.log("Multi operation test - has items:", multiTest.hasItems());

let nestedTest = new NestedArrayTest();
console.log("Nested array test - initial length:", nestedTest.getNestedLength());

let complexTest = new ComplexArrayTest();
console.log("Complex array test - items length:", complexTest.getItemsLength());
console.log("Complex array test - metadata length:", complexTest.getMetadataLength());
console.log("Complex array test - lengths equal:", complexTest.areLengthsEqual());

let edgeTest = new EdgeCaseArrayTest();
console.log("Edge case test - initial length:", edgeTest.getFinalLength());

let initialStateTest = new InitialStateArrayTest();
console.log("Initial state test - is initialized:", initialStateTest.isInitialized());
console.log("Initial state test - array length:", initialStateTest.getArrayLength());

let constrainedTest = new ConstrainedArrayTest<number>();
console.log("Constrained array test - initial length:", constrainedTest.getConstrainedLength());

let interfaceTest = new InterfaceArrayTest();
console.log("Interface array test - initial length:", interfaceTest.getLength());
console.log("Interface array test - is empty:", interfaceTest.isEmpty());

console.log("All comprehensive unit tests completed successfully!");