// Test union type interaction with generic arrays and nested property access

class UnionTypeContainer<T extends string | number> {
    items: T[];

    constructor() {
        this.items = [];
    }

    // Test array operations with union type constraints
    addItem(item: T): void {
        this.items.push(item);
    }

    getLength(): number {
        return this.items.length;
    }

    getLengthAsString(): string {
        return this.items.length.toString();
    }

    // Test nested property access with union types
    getFirstItem(): T {
        return this.items[0];
    }

    getLastItem(): T {
        return this.items[this.items.length - 1];
    }

    // Test array indexing with union types
    getItemAt(index: number): T {
        if (index >= 0 && index < this.items.length) {
            return this.items[index];
        }
        return this.items[this.items.length - 1];
    }

    // Test complex nested operations with union types
    getLengthOperations(): string {
        let len = this.items.length;
        let lenStr = len.toString();
        let lenPlusOne = len + 1;
        let lenMinusOne = len - 1;

        return "Length: " + lenStr + ", +1: " + lenPlusOne.toString() + ", -1: " + lenMinusOne.toString();
    }

    // Test conditional operations based on array length
    hasItems(): boolean {
        return this.items.length > 0;
    }

    isEmpty(): boolean {
        return this.items.length === 0;
    }

    // Test nested operations with length calculations
    getMiddleIndex(): number {
        if (this.items.length === 0) return 0;
        return Math.floor(this.items.length / 2);
    }

    getMiddleItem(): T {
        if (this.items.length === 0) return this.items[0];
        let middleIndex = this.getMiddleIndex();
        return this.items[middleIndex];
    }
}

class UnionTypeArrayOperations<T extends string | number> {
    items: T[];

    constructor() {
        this.items = [];
    }

    // Test various array operations with union type constraints
    pushItem(item: T): number {
        this.items.push(item);
        return this.items.length;
    }

    popItem(): T {
        if (this.items.length > 0) {
            return this.items.pop();
        }
        return this.items[0];
    }

    // Test nested property access with union types
    getLengthInfo(): string {
        let len = this.items.length;
        let isEmpty = len === 0;
        let hasItems = len > 0;

        return "Length: " + len.toString() + ", Empty: " + isEmpty.toString() + ", Has items: " + hasItems.toString();
    }

    // Test array manipulation with nested operations
    removeFirst(): T {
        if (this.items.length > 0) {
            let first = this.items[0];
            // Shift all items left
            for (let i = 0; i < this.items.length - 1; i++) {
                this.items[i] = this.items[i + 1];
            }
            this.items.length = this.items.length - 1;
            return first;
        }
        return this.items[0];
    }

    // Test conditional operations with union types
    getItemSafely(index: number): T {
        if (index >= 0 && index < this.items.length) {
            return this.items[index];
        }
        return this.items[this.items.length - 1];
    }

    // Test nested operations with array length
    getLastIndex(): number {
        return this.items.length - 1;
    }

    getSecondLastIndex(): number {
        return this.items.length - 2;
    }

    // Test array length in various contexts with union types
    getLengthOperations(): string {
        let len = this.items.length;
        let lenStr = len.toString();
        let lenPlusOne = len + 1;
        let lenMinusOne = len - 1;

        return "Length: " + lenStr + ", +1: " + lenPlusOne.toString() + ", -1: " + lenMinusOne.toString();
    }
}

// Test functions with union type constraints
function processUnionTypeArray<T extends string | number>(arr: T[]): string {
    return "Union type array length: " + arr.length.toString();
}

function processUnionTypeArrayWithIndex<T extends string | number>(arr: T[], index: number): T {
    if (index >= 0 && index < arr.length) {
        return arr[index];
    }
    return arr[arr.length - 1];
}

function processUnionTypeArrayOperations<T extends string | number>(arr: T[]): string {
    let len = arr.length;
    let lenStr = len.toString();
    let lastIndex = len - 1;
    let middleIndex = Math.floor(len / 2);

    return "Length: " + lenStr + ", Last index: " + lastIndex.toString() + ", Middle index: " + middleIndex.toString();
}

// Test specific union type combinations
class StringNumberContainer extends UnionTypeContainer<string | number> {
    // Test operations specific to string | number union
    getFirstItemAsString(): string {
        return this.items[0].toString();
    }

    getLastItemAsString(): string {
        return this.items[this.items.length - 1].toString();
    }

    // Test nested property access with toString
    getAllItemsAsString(): string {
        let result = "";
        for (let i = 0; i < this.items.length; i++) {
            result += this.items[i].toString();
        }
        return result;
    }

    // Test array operations with union types
    getLengthAndFirstItem(): string {
        let len = this.items.length;
        let first = this.items[0];
        return "Length: " + len.toString() + ", First: " + first.toString();
    }
}

// Test cases with different union type combinations
let stringNumberContainer = new StringNumberContainer();
stringNumberContainer.addItem("hello");
stringNumberContainer.addItem(42);
stringNumberContainer.addItem("world");
stringNumberContainer.addItem(100);

let unionContainer = new UnionTypeContainer<string | number>();
unionContainer.addItem("test");
unionContainer.addItem(123);
unionContainer.addItem("generic");

// Test array operations
let unionArrayOps = new UnionTypeArrayOperations<string | number>();
unionArrayOps.pushItem("a");
unionArrayOps.pushItem(1);
unionArrayOps.pushItem("b");
unionArrayOps.pushItem(2);

// Test nested property access
let stringNumberLength = stringNumberContainer.getLength();
let stringNumberLengthStr = stringNumberContainer.getLengthAsString();
let stringNumberFirst = stringNumberContainer.getFirstItem();
let stringNumberLast = stringNumberContainer.getLastItem();

let unionLength = unionContainer.getLength();
let unionLengthStr = unionContainer.getLengthAsString();
let unionFirst = unionContainer.getFirstItem();
let unionLast = unionContainer.getLastItem();

// Test array operations
let unionLengthOps = unionArrayOps.getLengthOperations();
let unionLengthInfo = unionArrayOps.getLengthInfo();
let unionLastIndex = unionArrayOps.getLastIndex();
let unionSecondLastIndex = unionArrayOps.getSecondLastIndex();

// Test function calls
let processedUnionArray = processUnionTypeArray(["a", 1, "b", 2]);
let processedWithIndex = processUnionTypeArrayWithIndex(["x", 10, "y"], 1);
let processedOperations = processUnionTypeArrayOperations([1, "a", 2, "b"]);

// Test specific union type operations
let firstAsString = stringNumberContainer.getFirstItemAsString();
let lastAsString = stringNumberContainer.getLastItemAsString();
let allAsString = stringNumberContainer.getAllItemsAsString();
let lengthAndFirst = stringNumberContainer.getLengthAndFirstItem();

// Test array method operations
let unionPop = unionArrayOps.popItem();
let unionLengthAfterPop = unionArrayOps.getLengthOperations();

let stringNumberPop = stringNumberContainer.getItemAt(0);
let stringNumberLengthAfterPop = stringNumberContainer.getLengthAsString();

// Test nested operations
let unionMiddleIndex = unionArrayOps.getMiddleIndex();
let unionMiddleItem = unionArrayOps.getMiddleItem();
let stringNumberMiddleIndex = stringNumberContainer.getMiddleIndex();
let stringNumberMiddleItem = stringNumberContainer.getMiddleItem();

// Test union type with array operations
let mixedArray: (string | number)[] = ["hello", 42, "world", 100];
let mixedLength = mixedArray.length;
let mixedLengthStr = mixedArray.length.toString();
let mixedFirst = mixedArray[0];
let mixedLast = mixedArray[mixedArray.length - 1];

// Test nested property access with union arrays
let mixedLengthPlusOne = mixedArray.length + 1;
let mixedLengthMinusOne = mixedArray.length - 1;
let mixedLengthOperations = "Length: " + mixedLengthStr + ", +1: " + mixedLengthPlusOne.toString() + ", -1: " + mixedLengthMinusOne.toString();