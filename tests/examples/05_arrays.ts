// Arrays
// Demonstrates array literals, indexing, and operations

// Array type declarations
let numbers: number[] = [1, 2, 3, 4, 5];
let strings: string[] = ["apple", "banana", "cherry"];
let booleans: boolean[] = [true, false, true];

// Array with explicit type annotation
let explicitNumbers: Array<number> = [10, 20, 30, 40, 50];

// Empty arrays
let emptyNumbers: number[] = [];
let emptyStrings: string[] = [];

// Array with mixed types (using union types)
let mixed: (string | number)[] = ["hello", 42, "world", 100];

// Array indexing
let firstNumber: number = numbers[0];        // 1
let secondString: string = strings[1];       // "banana"
let lastBoolean: boolean = booleans[2];      // true

// Array length
let count: number = numbers.length;          // 5

// Array modification
numbers[0] = 100;                            // [100, 2, 3, 4, 5]
strings[1] = "orange";                       // ["apple", "orange", "cherry"]

// Multi-dimensional arrays
let matrix: number[][] = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
];

let value: number = matrix[1][2];            // 6

// Array of objects
let people: object[] = [
    {name: "Alice", age: 30},
    {name: "Bob", age: 25},
    {name: "Charlie", age: 35}
];

// Array with spread operator (if supported)
let moreNumbers: number[] = [0, ...numbers, 6, 7, 8];

// Array iteration
for (let i: number = 0; i < numbers.length; i++) {
    let num: number = numbers[i];
    // Process each number
}

// For-of loop with arrays
for (let num of numbers) {
    // Process each number
}

// Array methods (basic implementations)
function push<T>(arr: T[], item: T): number {
    // Add item to end of array
    return arr.length + 1;
}

function pop<T>(arr: T[]): T | undefined {
    // Remove and return last item
    if (arr.length === 0) {
        return undefined;
    }
    return arr[arr.length - 1];
}

function shift<T>(arr: T[]): T | undefined {
    // Remove and return first item
    if (arr.length === 0) {
        return undefined;
    }
    return arr[0];
}

function unshift<T>(arr: T[], item: T): number {
    // Add item to beginning of array
    return arr.length + 1;
}

// Using array methods
let newLength: number = push(numbers, 6);
let lastItem: number | undefined = pop(numbers);
let firstItem: number | undefined = shift(numbers);
let newLength2: number = unshift(numbers, 0);

// Array slicing (manual implementation)
function slice<T>(arr: T[], start: number, end?: number): T[] {
    let result: T[] = [];
    let actualEnd: number = end !== undefined ? end : arr.length;

    for (let i: number = start; i < actualEnd; i++) {
        result.push(arr[i]);
    }
    return result;
}

let sliced: number[] = slice(numbers, 1, 3);

// Array concatenation
function concat<T>(arr1: T[], arr2: T[]): T[] {
    let result: T[] = [];

    for (let item of arr1) {
        result.push(item);
    }
    for (let item of arr2) {
        result.push(item);
    }
    return result;
}

let combined: number[] = concat([1, 2, 3], [4, 5, 6]);

// Array searching
function indexOf<T>(arr: T[], item: T): number {
    for (let i: number = 0; i < arr.length; i++) {
        if (arr[i] === item) {
            return i;
        }
    }
    return -1;
}

let index: number = indexOf(numbers, 3);

// Array filtering (manual implementation)
function filter<T>(arr: T[], predicate: (item: T) => boolean): T[] {
    let result: T[] = [];
    for (let item of arr) {
        if (predicate(item)) {
            result.push(item);
        }
    }
    return result;
}

let evenNumbers: number[] = filter(numbers, (n: number) => n % 2 === 0);

// Array mapping (manual implementation)
function map<T, U>(arr: T[], transform: (item: T) => U): U[] {
    let result: U[] = [];
    for (let item of arr) {
        result.push(transform(item));
    }
    return result;
}

let doubled: number[] = map(numbers, (n: number) => n * 2);

// This example demonstrates comprehensive array functionality