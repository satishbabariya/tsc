// Test cases for array initialization scenarios
// This test covers various array initialization patterns to ensure
// the type resolution and memory allocation fixes work correctly

class BasicArrayOperations<T> {
    items: T[];

    constructor() {
        this.items = [];
    }

    addItem(item: T): void {
        this.items.push(item);
    }

    getLength(): number {
        return this.items.length;
    }
}

// Test 1: Number array initialization
let numberArray = new BasicArrayOperations<number>();
numberArray.addItem(42);
numberArray.addItem(100);
_print("Number array length:", numberArray.getLength());

// Test 2: String array initialization  
let stringArray = new BasicArrayOperations<string>();
stringArray.addItem("hello");
stringArray.addItem("world");
_print("String array length:", stringArray.getLength());

// Test 3: Multiple arrays of same type
let array1 = new BasicArrayOperations<number>();
let array2 = new BasicArrayOperations<number>();
array1.addItem(1);
array2.addItem(2);
_print("Array1 length:", array1.getLength());
_print("Array2 length:", array2.getLength());

// Test 4: Nested array operations
let mainArray = new BasicArrayOperations<number>();
mainArray.addItem(10);
mainArray.addItem(20);
mainArray.addItem(30);
_print("Main array length:", mainArray.getLength());

// Test 5: Empty array initialization
let emptyArray = new BasicArrayOperations<string>();
_print("Empty array length:", emptyArray.getLength());