// Arithmetic Operations
// Demonstrates all arithmetic and comparison operators

// Basic arithmetic
let a: number = 10;
let b: number = 3;

// Addition
let sum: number = a + b;        // 13
let stringConcat: string = "Hello" + " " + "World";

// Subtraction
let difference: number = a - b; // 7

// Multiplication
let product: number = a * b;    // 30

// Division
let quotient: number = a / b;   // 3.333...

// Modulo
let remainder: number = a % b;  // 1

// Exponentiation
let power: number = a ** 2;     // 100

// Increment and decrement
let counter: number = 0;
counter++;                      // Post-increment: 1
++counter;                      // Pre-increment: 2
counter--;                      // Post-decrement: 1
--counter;                      // Pre-decrement: 0

// Assignment operators
let value: number = 10;
value += 5;                     // value = 15
value -= 3;                     // value = 12
value *= 2;                     // value = 24
value /= 4;                     // value = 6
value %= 5;                     // value = 1
value **= 3;                    // value = 1

// Comparison operators
let x: number = 10;
let y: number = 5;

let equal: boolean = (x == y);          // false
let notEqual: boolean = (x != y);       // true
let strictEqual: boolean = (x === y);   // false
let strictNotEqual: boolean = (x !== y); // true
let greaterThan: boolean = (x > y);     // true
let lessThan: boolean = (x < y);        // false
let greaterEqual: boolean = (x >= y);   // true
let lessEqual: boolean = (x <= y);      // false

// Logical operators
let p: boolean = true;
let q: boolean = false;

let and: boolean = p && q;      // false
let or: boolean = p || q;       // true
let not: boolean = !p;          // false

// Bitwise operators
let m: number = 12;  // 1100 in binary
let n: number = 10;  // 1010 in binary

let bitwiseAnd: number = m & n;     // 8 (1000)
let bitwiseOr: number = m | n;      // 14 (1110)
let bitwiseXor: number = m ^ n;     // 6 (0110)
let bitwiseNot: number = ~m;        // -13
let leftShift: number = m << 2;     // 48
let rightShift: number = m >> 2;    // 3
let unsignedRightShift: number = m >>> 2; // 3

// Ternary operator
let max: number = (x > y) ? x : y;  // 10

// Typeof operator
let typeOfNumber: string = typeof 42;        // "number"
let typeOfString: string = typeof "hello";   // "string"
let typeOfBoolean: string = typeof true;     // "boolean"

// This example demonstrates all arithmetic and logical operations