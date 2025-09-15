// Functions
// Demonstrates function declarations, parameters, return types, and calls

// Basic function declaration
function greet(name: string): string {
    return "Hello, " + name + "!";
}

// Function with multiple parameters
function add(a: number, b: number): number {
    return a + b;
}

// Function with no parameters
function getCurrentTime(): number {
    return Date.now();
}

// Function with no return value (void)
function logMessage(message: string): void {
    // This function doesn't return anything
}

// Function with optional parameters
function createUser(name: string, age?: number): object {
    let user: object = { name: name };
    if (age !== undefined) {
        // Add age property if provided
    }
    return user;
}

// Function with default parameters
function multiply(a: number, b: number = 1): number {
    return a * b;
}

// Function with rest parameters
function sum(...numbers: number[]): number {
    let total: number = 0;
    for (let num of numbers) {
        total += num;
    }
    return total;
}

// Function expressions
let square = function(x: number): number {
    return x * x;
};

// Arrow functions
let cube = (x: number): number => {
    return x * x * x;
};

// Arrow function with single expression (implicit return)
let double = (x: number): number => x * 2;

// Arrow function with no parameters
let getRandom = (): number => Math.random();

// Higher-order functions
function applyOperation(x: number, operation: (n: number) => number): number {
    return operation(x);
}

// Function that returns a function
function createMultiplier(factor: number): (x: number) => number {
    return (x: number): number => x * factor;
}

// Recursive function
function factorial(n: number): number {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Function overloading (using union types)
function processValue(value: string): string;
function processValue(value: number): number;
function processValue(value: string | number): string | number {
    if (typeof value === "string") {
        return value.toUpperCase();
    } else {
        return value * 2;
    }
}

// Function calls
let greeting: string = greet("Alice");
let result: number = add(5, 3);
let currentTime: number = getCurrentTime();
let user: object = createUser("Bob", 25);
let product: number = multiply(4, 5);
let total: number = sum(1, 2, 3, 4, 5);
let squared: number = square(4);
let cubed: number = cube(3);
let doubled: number = double(7);
let random: number = getRandom();

// Using higher-order functions
let result1: number = applyOperation(5, square);
let result2: number = applyOperation(3, cube);

// Using function that returns a function
let multiplyByTwo = createMultiplier(2);
let result3: number = multiplyByTwo(10);

// Recursive function call
let fact: number = factorial(5);

// Function overloading calls
let upper: string = processValue("hello");
let doubledNum: number = processValue(21);

// This example demonstrates comprehensive function functionality