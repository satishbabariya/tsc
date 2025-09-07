// Parser Test - Demonstrating Phase 2 Implementation
// This file tests the recursive descent parser

// Variable declarations
let x = 42;
let y = x + 10;
let z = y * 2;
let flag = true;
let message = "Hello Parser";

// Binary expressions with different operators
let sum = x + y;
let product = x * y;
let comparison = x < y;
let logical = flag && (x > 0);

// Function declarations
function add(a, b) {
    let result = a + b;
    let doubled = result * 2;
}

function greet(name) {
    let greeting = "Hello " + name;
}

// Nested blocks
function complex() {
    let outer = 1;
    {
        let inner = outer + 2;
        let nested = inner * 3;
    }
}