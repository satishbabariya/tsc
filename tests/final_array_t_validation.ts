// Final comprehensive validation test for Array<T> vs T[] functionality

// Test 1: Basic Array<T> syntax
let numberArray1: Array<number> = [];
let stringArray1: Array<string> = [];
let booleanArray1: Array<boolean> = [];

// Test 2: Basic T[] syntax (existing)
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

// Test 5: Function parameters with Array<T>
function processArrayT<T>(arr: Array<T>): number {
    return arr.length;
}

// Test 6: Function parameters with T[]
function processArrayBracket<T>(arr: T[]): number {
    return arr.length;
}

// Test 7: Type conversion between Array<T> and T[]
let testArrayT: Array<number> = [];
let testArrayBracket: number[] = [];

// Test 8: Basic operations
testArrayT.push(1);
testArrayT.push(2);
testArrayBracket.push(3);
testArrayBracket.push(4);

console.log("Array<T> length:", testArrayT.length);
console.log("T[] length:", testArrayBracket.length);

// Test 9: Element access
console.log("Array<T> first element:", testArrayT[0]);
console.log("T[] first element:", testArrayBracket[0]);

// Test 10: Generic class instantiation
let numberContainer = new GenericArrayContainer<number>();
numberContainer.addItem(42);
console.log("Number array length:", numberContainer.getLength());

let stringContainer = new GenericArrayContainer<string>();
stringContainer.addItem("hello");
console.log("String array length:", stringContainer.getLength());

// Test 11: Function calls with generic arrays
console.log("Function with Array<T> parameter:", processArrayT(testArrayT));
console.log("Function with T[] parameter:", processArrayBracket(testArrayBracket));

console.log("All Array<T> vs T[] tests completed successfully!");