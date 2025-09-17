// Test unconstrained generics with nested property access

class UnconstrainedContainer<T> {
    value: T;
    items: T[];

    constructor(value: T) {
        this.value = value;
        this.items = [];
    }

    // Test basic operations with unconstrained generics
    getValue(): T {
        return this.value;
    }

    setValue(newValue: T): void {
        this.value = newValue;
    }

    // Test array operations with unconstrained generics
    addItem(item: T): void {
        this.items.push(item);
    }

    getItemCount(): number {
        return this.items.length;
    }

    getItemCountAsString(): string {
        return this.items.length.toString();
    }

    // Test nested property access with unconstrained generics
    getFirstItem(): T {
        if (this.items.length > 0) {
            return this.items[0];
        }
        return this.value;
    }

    getLastItem(): T {
        if (this.items.length > 0) {
            return this.items[this.items.length - 1];
        }
        return this.value;
    }

    // Test complex nested operations
    getItemAt(index: number): T {
        if (index >= 0 && index < this.items.length) {
            return this.items[index];
        }
        return this.items[this.items.length - 1];
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
        if (this.items.length === 0) return this.value;
        let middleIndex = this.getMiddleIndex();
        return this.items[middleIndex];
    }
}

class UnconstrainedArrayOperations<T> {
    items: T[];

    constructor() {
        this.items = [];
    }

    // Test various array operations with unconstrained generics
    pushItem(item: T): number {
        this.items.push(item);
        return this.items.length;
    }

    popItem(): T {
        if (this.items.length > 0) {
            return this.items.pop();
        }
        return this.items[0]; // Return first item if array is empty (undefined behavior)
    }

    // Test nested property access
    getLengthOperations(): string {
        let len = this.items.length;
        let lenStr = len.toString();
        let lenPlusOne = len + 1;
        let lenMinusOne = len - 1;

        return "Length: " + lenStr + ", +1: " + lenPlusOne.toString() + ", -1: " + lenMinusOne.toString();
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
        return this.items[0]; // Return first item if array is empty
    }

    // Test conditional operations
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

    // Test array length in various contexts
    getLengthInfo(): string {
        let len = this.items.length;
        let isEmpty = len === 0;
        let hasItems = len > 0;

        return "Length: " + len.toString() + ", Empty: " + isEmpty.toString() + ", Has items: " + hasItems.toString();
    }
}

// Test functions with unconstrained generics
function processUnconstrainedValue<T>(value: T): string {
    return "Value processed, type unknown";
}

function processUnconstrainedArray<T>(arr: T[]): string {
    return "Array length: " + arr.length.toString();
}

function processUnconstrainedArrayWithIndex<T>(arr: T[], index: number): T {
    if (index >= 0 && index < arr.length) {
        return arr[index];
    }
    return arr[arr.length - 1];
}

function processUnconstrainedArrayOperations<T>(arr: T[]): string {
    let len = arr.length;
    let lenStr = len.toString();
    let lastIndex = len - 1;
    let middleIndex = Math.floor(len / 2);

    return "Length: " + lenStr + ", Last index: " + lastIndex.toString() + ", Middle index: " + middleIndex.toString();
}

// Test cases with different types
let numberContainer = new UnconstrainedContainer<number>(42);
numberContainer.addItem(10);
numberContainer.addItem(20);
numberContainer.addItem(30);

let stringContainer = new UnconstrainedContainer<string>("hello");
stringContainer.addItem("world");
stringContainer.addItem("test");
stringContainer.addItem("generic");

let booleanContainer = new UnconstrainedContainer<boolean>(true);
booleanContainer.addItem(false);
booleanContainer.addItem(true);

// Test array operations
let numberArrayOps = new UnconstrainedArrayOperations<number>();
numberArrayOps.pushItem(1);
numberArrayOps.pushItem(2);
numberArrayOps.pushItem(3);

let stringArrayOps = new UnconstrainedArrayOperations<string>();
stringArrayOps.pushItem("a");
stringArrayOps.pushItem("b");
stringArrayOps.pushItem("c");

// Test nested property access
let numberValue = numberContainer.getValue();
let numberItemCount = numberContainer.getItemCount();
let numberItemCountStr = numberContainer.getItemCountAsString();
let numberFirstItem = numberContainer.getFirstItem();
let numberLastItem = numberContainer.getLastItem();

let stringValue = stringContainer.getValue();
let stringItemCount = stringContainer.getItemCount();
let stringItemCountStr = stringContainer.getItemCountAsString();
let stringFirstItem = stringContainer.getFirstItem();
let stringLastItem = stringContainer.getLastItem();

// Test array operations
let numberLengthOps = numberArrayOps.getLengthOperations();
let numberLengthInfo = numberArrayOps.getLengthInfo();
let numberLastIndex = numberArrayOps.getLastIndex();
let numberSecondLastIndex = numberArrayOps.getSecondLastIndex();

let stringLengthOps = stringArrayOps.getLengthOperations();
let stringLengthInfo = stringArrayOps.getLengthInfo();
let stringLastIndex = stringArrayOps.getLastIndex();
let stringSecondLastIndex = stringArrayOps.getSecondLastIndex();

// Test function calls
let processedNumberValue = processUnconstrainedValue(42);
let processedStringValue = processUnconstrainedValue("hello");
let processedBooleanValue = processUnconstrainedValue(true);

let processedNumberArray = processUnconstrainedArray([1, 2, 3]);
let processedStringArray = processUnconstrainedArray(["a", "b", "c"]);

let processedWithIndex = processUnconstrainedArrayWithIndex([1, 2, 3], 1);
let processedOperations = processUnconstrainedArrayOperations(["a", "b", "c"]);

// Test array method operations
let numberPop = numberArrayOps.popItem();
let numberLengthAfterPop = numberArrayOps.getLengthOperations();

let stringPop = stringArrayOps.popItem();
let stringLengthAfterPop = stringArrayOps.getLengthOperations();

// Test nested operations
let numberMiddleIndex = numberArrayOps.getMiddleIndex();
let numberMiddleItem = numberArrayOps.getMiddleItem();
let stringMiddleIndex = stringArrayOps.getMiddleIndex();
let stringMiddleItem = stringArrayOps.getMiddleItem();