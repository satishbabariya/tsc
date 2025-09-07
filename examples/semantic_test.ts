// Semantic Analysis Test - Demonstrating Phase 3 Implementation
// This file tests the semantic analyzer with various TypeScript constructs

// Valid variable declarations with type inference
let x = 42;                    // number
let message = "Hello World";   // string
let flag = true;               // boolean

// Valid arithmetic operations
let sum = x + 10;              // number + number = number
let doubled = sum * 2;         // number * number = number

// Valid string operations  
let greeting = "Hello " + message;  // string + string = string
let mixed = message + " " + x;      // string + string + number = string

// Valid boolean operations
let comparison = x > 10;       // number > number = boolean
let logical = flag && comparison;  // boolean && boolean = boolean

// Valid function declarations
function add(a, b) {
    let result = a + b;        // any + any (parameters default to any)
}

function greet(name) {
    let msg = "Hello " + name; // string + any = string
}

// Test variable usage
let calculated = sum + doubled;
let final = calculated * 2;

// Test nested scopes
{
    let scoped = x + 5;        // Access outer scope variable
    let inner = scoped * 2;    // Local scope variable
}