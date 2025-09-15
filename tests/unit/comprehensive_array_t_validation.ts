// Comprehensive validation test for Array<T> vs T[] functionality

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

// Test 5: Mixed Array<T> and T[] in same class
class MixedArrayContainer<T> {
    arrayT: Array<T>;
    arrayBracket: T[];
    
    constructor() {
        this.arrayT = [];
        this.arrayBracket = [];
    }
    
    addToArrayT(item: T): void {
        this.arrayT.push(item);
    }
    
    addToArrayBracket(item: T): void {
        this.arrayBracket.push(item);
    }
    
    getArrayTLength(): number {
        return this.arrayT.length;
    }
    
    getArrayBracketLength(): number {
        return this.arrayBracket.length;
    }
}

// Test 6: Function parameters with Array<T> and T[]
function processArrayT<T>(arr: Array<T>): number {
    return arr.length;
}

function processArrayBracket<T>(arr: T[]): number {
    return arr.length;
}

// Test 7: Return types with Array<T> and T[]
function createArrayT<T>(): Array<T> {
    return [];
}

function createArrayBracket<T>(): T[] {
    return [];
}

// Test 8: Nested generic types
class NestedGenericContainer<T, U> {
    arrayT: Array<T>;
    arrayU: Array<U>;
    
    constructor() {
        this.arrayT = [];
        this.arrayU = [];
    }
    
    addT(item: T): void {
        this.arrayT.push(item);
    }
    
    addU(item: U): void {
        this.arrayU.push(item);
    }
}

// Test execution
_print("=== Array<T> vs T[] Comprehensive Validation ===");

// Test basic functionality
let container1 = new GenericArrayContainer<number>();
container1.addItem(42);
_print("Array<T> number container length:", container1.getLength());

let container2 = new GenericArrayContainer2<string>();
container2.addItem("hello");
_print("T[] string container length:", container2.getLength());

// Test mixed container
let mixedContainer = new MixedArrayContainer<number>();
mixedContainer.addToArrayT(10);
mixedContainer.addToArrayBracket(20);
_print("Mixed Array<T> length:", mixedContainer.getArrayTLength());
_print("Mixed T[] length:", mixedContainer.getArrayBracketLength());

// Test function parameters
let testArrayT: Array<number> = [];
testArrayT.push(1);
testArrayT.push(2);
_print("Function with Array<T> parameter:", processArrayT(testArrayT));

let testArrayBracket: number[] = [];
testArrayBracket.push(3);
testArrayBracket.push(4);
testArrayBracket.push(5);
_print("Function with T[] parameter:", processArrayBracket(testArrayBracket));

// Test return types
let returnedArrayT = createArrayT<number>();
returnedArrayT.push(100);
_print("Returned Array<T> length:", returnedArrayT.length);

let returnedArrayBracket = createArrayBracket<string>();
returnedArrayBracket.push("world");
_print("Returned T[] length:", returnedArrayBracket.length);

// Test nested generics
let nestedContainer = new NestedGenericContainer<number, string>();
nestedContainer.addT(42);
nestedContainer.addT(84);
nestedContainer.addU("nested");
_print("Nested T array length:", nestedContainer.arrayT.length);
_print("Nested U array length:", nestedContainer.arrayU.length);

_print("=== All Array<T> vs T[] tests completed successfully ===");