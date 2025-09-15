// Test for Array<T> vs T[] syntax support

// Test 1: Array<T> syntax
let numberArray1: Array<number> = [];
let stringArray1: Array<string> = [];
let booleanArray1: Array<boolean> = [];

// Test 2: T[] syntax (existing)
let numberArray2: number[] = [];
let stringArray2: string[] = [];
let booleanArray2: boolean[] = [];

// Test 3: Generic class with Array<T>
class GenericArrayContainer<T> {
    items: Array<T>;
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

// Test 4: Generic class with T[]
class GenericArrayContainer2<T> {
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

// Test 5: Function with Array<T> parameter
function processArray<T>(arr: Array<T>): number {
    return arr.length;
}

// Test 6: Function with T[] parameter
function processArray2<T>(arr: T[]): number {
    return arr.length;
}

// Test 7: Mixed usage
function processMixedArrays<T>(arr1: Array<T>, arr2: T[]): number {
    return arr1.length + arr2.length;
}

// Test 8: Nested generic arrays
let nestedArray1: Array<Array<number>> = [];
let nestedArray2: number[][] = [];

// Test 9: Generic with constraints
function processConstrainedArray<T extends number>(arr: Array<T>): T {
    // This would need array element access to work fully
    return arr.length as T; // Placeholder
}

// Test 10: Array of generic types
class GenericItem<T> {
    value: T;
    constructor(v: T) { this.value = v; }
}

let genericItemArray: Array<GenericItem<number>> = [];
let genericItemArray2: GenericItem<number>[] = [];

_print("Generic Array Type Test - All declarations compiled successfully");