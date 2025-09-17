// Test array methods with generic elements and nested property access

class GenericArrayMethods<T> {
    items: T[];

    constructor() {
        this.items = [];
    }

    // Test push method with nested property access
    pushAndGetLength(item: T): number {
        this.items.push(item);
        return this.items.length;
    }

    // Test pop method with nested property access
    popAndGetLength(): T {
        let result = this.items.pop();
        return result;
    }

    // Test length property in various contexts
    getLengthString(): string {
        return "Current length: " + this.items.length.toString();
    }

    // Test array indexing with length calculations
    getItemAt(index: number): T {
        if (index >= 0 && index < this.items.length) {
            return this.items[index];
        }
        return this.items[this.items.length - 1]; // Return last item if index out of bounds
    }

    // Test nested operations with length
    getLastItemIndex(): number {
        return this.items.length - 1;
    }

    getSecondLastIndex(): number {
        return this.items.length - 2;
    }

    // Test complex nested operations
    getLengthOperations(): string {
        let len = this.items.length;
        let lenStr = len.toString();
        let lenPlusOne = len + 1;
        let lenMinusOne = len - 1;

        return "Length: " + lenStr + ", +1: " + lenPlusOne.toString() + ", -1: " + lenMinusOne.toString();
    }

    // Test array manipulation with nested property access
    removeFirstAndGetLength(): number {
        if (this.items.length > 0) {
            // Simulate shift operation
            for (let i = 0; i < this.items.length - 1; i++) {
                this.items[i] = this.items[i + 1];
            }
            this.items.length = this.items.length - 1;
        }
        return this.items.length;
    }

    // Test conditional operations based on length
    isEmpty(): boolean {
        return this.items.length === 0;
    }

    hasItems(): boolean {
        return this.items.length > 0;
    }

    // Test length-based array operations
    getMiddleItem(): T {
        if (this.items.length === 0) {
            return this.items[0]; // This will be undefined, but syntactically valid
        }
        let middleIndex = Math.floor(this.items.length / 2);
        return this.items[middleIndex];
    }
}

class NumberArrayMethods extends GenericArrayMethods<number> {
    // Test arithmetic operations with nested property access
    getSum(): number {
        let sum = 0;
        for (let i = 0; i < this.items.length; i++) {
            sum += this.items[i];
        }
        return sum;
    }

    // Test nested property access in arithmetic operations
    getSumAsString(): string {
        let sum = this.getSum();
        return "Sum: " + sum.toString() + ", Length: " + this.items.length.toString();
    }

    // Test array operations with length calculations
    addToAll(value: number): void {
        for (let i = 0; i < this.items.length; i++) {
            this.items[i] = this.items[i] + value;
        }
    }

    // Test nested operations
    getAverageAsString(): string {
        if (this.items.length === 0) return "Empty array";
        let avg = this.getSum() / this.items.length;
        return "Average: " + avg.toString() + " of " + this.items.length.toString() + " items";
    }
}

class StringArrayMethods extends GenericArrayMethods<string> {
    // Test string operations with nested property access
    concatenateAll(): string {
        let result = "";
        for (let i = 0; i < this.items.length; i++) {
            result += this.items[i];
        }
        return result;
    }

    // Test nested property access in string operations
    getConcatenationInfo(): string {
        let concat = this.concatenateAll();
        return "Concatenated: " + concat + ", Length: " + this.items.length.toString();
    }

    // Test array operations with string length
    getTotalStringLength(): number {
        let total = 0;
        for (let i = 0; i < this.items.length; i++) {
            total += this.items[i].length;
        }
        return total;
    }

    // Test nested operations with string and array lengths
    getStringLengthInfo(): string {
        let totalLength = this.getTotalStringLength();
        let arrayLength = this.items.length;
        return "Total string length: " + totalLength.toString() + ", Array length: " + arrayLength.toString();
    }
}

// Test functions with generic array methods
function processGenericArray<T>(arr: T[]): string {
    return "Array length: " + arr.length.toString();
}

function processGenericArrayWithIndex<T>(arr: T[], index: number): T {
    if (index >= 0 && index < arr.length) {
        return arr[index];
    }
    return arr[arr.length - 1];
}

function processGenericArrayOperations<T>(arr: T[]): string {
    let len = arr.length;
    let lenStr = len.toString();
    let lastIndex = len - 1;
    let lastIndexStr = lastIndex.toString();

    return "Length: " + lenStr + ", Last index: " + lastIndexStr;
}

// Test cases
let numberMethods = new NumberArrayMethods();
numberMethods.pushAndGetLength(10);
numberMethods.pushAndGetLength(20);
numberMethods.pushAndGetLength(30);

let stringMethods = new StringArrayMethods();
stringMethods.pushAndGetLength("hello");
stringMethods.pushAndGetLength("world");
stringMethods.pushAndGetLength("test");

let genericMethods = new GenericArrayMethods<number>();
genericMethods.pushAndGetLength(1);
genericMethods.pushAndGetLength(2);
genericMethods.pushAndGetLength(3);

// Test nested property access
let numberSumStr = numberMethods.getSumAsString();
let numberAvgStr = numberMethods.getAverageAsString();
let numberLengthStr = numberMethods.getLengthString();

let stringConcatInfo = stringMethods.getConcatenationInfo();
let stringLengthInfo = stringMethods.getStringLengthInfo();
let stringLengthStr = stringMethods.getLengthString();

let genericLengthStr = genericMethods.getLengthString();
let genericLengthOps = genericMethods.getLengthOperations();

// Test function calls
let processedGeneric = processGenericArray([1, 2, 3, 4, 5]);
let processedWithIndex = processGenericArrayWithIndex(["a", "b", "c"], 1);
let processedOperations = processGenericArrayOperations([1, 2, 3]);

// Test array method operations
let numberPop = numberMethods.popAndGetLength();
let numberLengthAfterPop = numberMethods.getLengthString();

let stringPop = stringMethods.popAndGetLength();
let stringLengthAfterPop = stringMethods.getLengthString();

// Test nested operations
let lastItemIndex = numberMethods.getLastItemIndex();
let secondLastIndex = numberMethods.getSecondLastIndex();
let middleItem = numberMethods.getMiddleItem();