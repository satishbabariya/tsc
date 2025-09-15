// Test edge cases for array initialization and operations
// This test covers boundary conditions and complex scenarios

class TestArray<T> {
    data: T[];
    
    constructor() {
        this.data = [];
    }
    
    push(value: T): void {
        this.data.push(value);
    }
    
    size(): number {
        return this.data.length;
    }
}

// Test 1: Large number of operations
let bigArray = new TestArray<number>();
bigArray.push(0);
bigArray.push(10);
bigArray.push(20);
bigArray.push(30);
bigArray.push(40);
_print("Big array size:", bigArray.size());

// Test 2: Multiple arrays with different types
let numArray = new TestArray<number>();
let strArray = new TestArray<string>();

numArray.push(100);
strArray.push("test");

_print("Number array size:", numArray.size());
_print("String array size:", strArray.size());

// Test 3: Array operations in sequence
let sequentialArray = new TestArray<number>();
sequentialArray.push(1);
sequentialArray.push(2);
sequentialArray.push(3);
_print("Sequential array size:", sequentialArray.size());

// Test 4: Global array initialization
let globalArray = new TestArray<string>();
globalArray.push("global");
_print("Global array size:", globalArray.size());