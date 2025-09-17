// Basic test for nested property access with generic arrays
// Focus on simple nested property access without complex operations

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

    getLengthAsString(): string {
        return this.items.length.toString();
    }

    getFirstItem(): T {
        return this.items[0];
    }

    getLastItem(): T {
        return this.items[this.items.length - 1];
    }

    getSecondToLast(): T {
        return this.items[this.items.length - 2];
    }

    getLengthPlusOne(): number {
        return this.items.length + 1;
    }

    getLengthMinusOne(): number {
        return this.items.length - 1;
    }
}

// Test functions with generic constraints
function processArray<T extends number>(item: T): string {
    return "Item: " + item.toString();
}

function processStringArray<T extends string>(item: T): string {
    return "String item: " + item;
}

function processAnyArray<T>(arr: T[]): number {
    return arr.length;
}

function processAnyArrayWithToString<T>(arr: T[]): string {
    return "Length: " + arr.length.toString();
}

// Test cases
let numberOps = new BasicArrayOperations<number>();
numberOps.addItem(10);
numberOps.addItem(20);
numberOps.addItem(30);

let stringOps = new BasicArrayOperations<string>();
stringOps.addItem("hello");
stringOps.addItem("world");
stringOps.addItem("test");

let genericOps = new BasicArrayOperations<string>();
genericOps.addItem("generic");
genericOps.addItem("test");

// Test nested property access
let numberLengthStr = numberOps.getLengthAsString();
let numberLengthPlusOne = numberOps.getLengthPlusOne();
let numberLengthMinusOne = numberOps.getLengthMinusOne();

let stringLengthStr = stringOps.getLengthAsString();
let stringLengthPlusOne = stringOps.getLengthPlusOne();
let stringLengthMinusOne = stringOps.getLengthMinusOne();

let genericLengthStr = genericOps.getLengthAsString();
let genericLengthPlusOne = genericOps.getLengthPlusOne();
let genericLengthMinusOne = genericOps.getLengthMinusOne();

// Test function calls with nested property access
let processedNumber = processArray(42);
let processedString = processStringArray("hello");

let anyLength = processAnyArray(["test", "array"]);
let anyLengthStr = processAnyArrayWithToString([1, 2, 3]);

// Test array indexing with nested property access
let firstItem = numberOps.getFirstItem();
let lastItem = numberOps.getLastItem();
let secondToLast = numberOps.getSecondToLast();

let stringFirstItem = stringOps.getFirstItem();
let stringLastItem = stringOps.getLastItem();
let stringSecondToLast = stringOps.getSecondToLast();