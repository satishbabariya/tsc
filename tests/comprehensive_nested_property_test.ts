// Comprehensive test for nested property access with generic arrays
// Tests various combinations of array operations and nested property access

class ArrayOperations<T> {
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
    
    getFirstItem(): T {
        return this.items[0];
    }
    
    getLastItem(): T {
        return this.items[this.items.length - 1];
    }
    
    // Test nested property access with array length
    getLengthAsString(): string {
        return this.items.length.toString();
    }
    
    // Test array indexing with length
    getSecondToLast(): T {
        return this.items[this.items.length - 2];
    }
    
    // Test complex nested operations
    getLengthPlusOne(): number {
        return this.items.length + 1;
    }
    
    getLengthMinusOne(): number {
        return this.items.length - 1;
    }
}

class StringArrayOperations extends ArrayOperations<string> {
    // Test method overriding with nested property access
    getLengthAsString(): string {
        return "Length: " + this.items.length.toString();
    }
    
    // Test array methods with nested access
    getAllItemsAsString(): string {
        let result = "";
        let i = 0;
        while (i < this.items.length) {
            result += this.items[i];
            i = i + 1;
        }
        return result;
    }
}

class NumberArrayOperations extends ArrayOperations<number> {
    // Test arithmetic operations with nested property access
    getSum(): number {
        let sum = 0;
        let i = 0;
        while (i < this.items.length) {
            sum += this.items[i];
            i = i + 1;
        }
        return sum;
    }
    
    // Test nested property access in arithmetic
    getAverage(): number {
        if (this.items.length === 0) {
            return 0;
        }
        return this.getSum() / this.items.length;
    }
}

// Test function with generic constraints and nested property access
function processArray<T extends number>(arr: T[]): string {
    return "Array length: " + arr.length.toString() + ", First item: " + arr[0].toString();
}

function processStringArray<T extends string>(arr: T[]): string {
    return "String array length: " + arr.length.toString() + ", Last item: " + arr[arr.length - 1];
}

// Test unconstrained generics
function processAnyArray<T>(arr: T[]): number {
    return arr.length;
}

function processAnyArrayWithToString<T>(arr: T[]): string {
    return "Length: " + arr.length.toString();
}

// Test cases
let stringOps = new StringArrayOperations();
stringOps.addItem("hello");
stringOps.addItem("world");
stringOps.addItem("test");

let numberOps = new NumberArrayOperations();
numberOps.addItem(10);
numberOps.addItem(20);
numberOps.addItem(30);

let genericOps = new ArrayOperations<string>();
genericOps.addItem("generic");
genericOps.addItem("test");

// Test nested property access
let lengthStr = stringOps.getLengthAsString();
let secondToLast = stringOps.getSecondToLast();
let lengthPlusOne = stringOps.getLengthPlusOne();

let numberSum = numberOps.getSum();
let numberAvg = numberOps.getAverage();
let numberLengthStr = numberOps.getLengthAsString();

let genericLengthStr = genericOps.getLengthAsString();
let genericLengthPlusOne = genericOps.getLengthPlusOne();

// Test function calls with nested property access
let processedNumber = processArray([1, 2, 3, 4, 5]);
let processedString = processStringArray(["a", "b", "c"]);

let anyLength = processAnyArray(["test", "array"]);
let anyLengthStr = processAnyArrayWithToString([1, 2, 3]);