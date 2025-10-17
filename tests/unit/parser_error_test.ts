// Parser error test - reproduces the most common syntax errors
// This test helps identify and fix parser issues

// Test 1: Basic property access (should work)
let obj = { value: 42 };
let value = obj.value;

// Test 2: Method calls (should work)
function test() {
    return "test";
}
let result = test();

// Test 3: Simple array (this might cause issues)
let numbers = [1, 2, 3];

// Test 4: Basic class (should work)
class TestClass {
    private data: number;

    constructor(val: number) {
        this.data = val;
    }

    getData(): number {
        return this.data;
    }
}

// Test 5: Generic class (should work)
class GenericTest<T> {
    private item: T;

    constructor(item: T) {
        this.item = item;
    }

    getItem(): T {
        return this.item;
    }
}

// Main test
function runParserErrorTest(): void {
    _print("=== Parser Error Test ===");
    
    // Test basic operations
    _print("Basic test:", value);
    _print("Function test:", result);
    
    // Test class
    let testObj = new TestClass(100);
    let data = testObj.getData();
    _print("Class test:", data);
    
    // Test generic
    let genericObj = new GenericTest<number>(42);
    let item = genericObj.getItem();
    _print("Generic test:", item);
    
    _print("=== Parser Error Test Complete ===");
}

runParserErrorTest();