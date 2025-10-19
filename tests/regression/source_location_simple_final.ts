// Final comprehensive regression test for source location handling
// This test focuses on core features without using toString() method

// Test 1: Basic variable declarations with source location
let x: number = 42;
let y: string = "hello";
let z: boolean = true;

// Test 2: Function declarations with source location
function simpleFunction(param: number): number {
    return param * 2;
}

function complexFunction(a: number, b: string): string {
    return b + " " + a.toString();
}

// Test 3: Function calls with source location
let result1: number = simpleFunction(x);
let result2: string = complexFunction(x, y);

// Test 4: Console.log calls (triggers source location in debug output)
console.log("Test 1:", x);
console.log("Test 2:", y);
console.log("Test 3:", z);
console.log("Result 1:", result1);
console.log("Result 2:", result2);

// Test 5: Conditional statements with source location
if (x > 0) {
    console.log("x is positive");
} else {
    console.log("x is not positive");
}

if (y.length > 0) {
    console.log("y has content");
} else {
    console.log("y is empty");
}

// Test 6: While loops with source location
let counter: number = 0;
while (counter < 3) {
    console.log("Counter:", counter);
    counter = counter + 1;
}

// Test 7: Nested function calls with source location
function outerFunction(value: number): number {
    function innerFunction(nested: number): number {
        return nested + 10;
    }
    
    let innerResult: number = innerFunction(value);
    return innerResult * 2;
}

let nestedResult: number = outerFunction(x);
console.log("Nested result:", nestedResult);

// Test 8: Complex expressions requiring source location
let complex1: number = x + y.length;
let complex2: boolean = (x > 0) && (y.length > 0);

console.log("Complex 1:", complex1);
console.log("Complex 2:", complex2);

// Test 9: Array operations with source location
let numbers: number[] = [1, 2, 3, 4, 5];
let firstNumber: number = numbers[0];
let secondNumber: number = numbers[1];

console.log("First number:", firstNumber);
console.log("Second number:", secondNumber);

// Test 10: Object operations with source location
let obj = { 
    value: 100, 
    name: "test", 
    active: true 
};

let objValue: number = obj.value;
let objName: string = obj.name;
let objActive: boolean = obj.active;

console.log("Object value:", objValue);
console.log("Object name:", objName);
console.log("Object active:", objActive);

// Test 11: Multiple variable declarations in sequence
let a: number = 1;
let b: number = 2;
let c: number = 3;
let d: number = a + b + c;

console.log("Sum:", d);

// Test 12: Function calls with multiple arguments
function multiParamFunction(p1: number, p2: string, p3: boolean): string {
    return p2 + " " + p1.toString();
}

let multiResult: string = multiParamFunction(a, y, z);
console.log("Multi param result:", multiResult);

// Test 13: Nested conditionals with source location
if (a > 0) {
    if (b > 0) {
        console.log("Both a and b are positive");
    } else {
        console.log("a is positive but b is not");
    }
} else {
    console.log("a is not positive");
}

// Test 14: Complex arithmetic expressions
let arithmetic1: number = (a + b) * c;
let arithmetic2: number = a * b + c;
let arithmetic3: number = (a + b) / c;

console.log("Arithmetic 1:", arithmetic1);
console.log("Arithmetic 2:", arithmetic2);
console.log("Arithmetic 3:", arithmetic3);

// Test 15: String concatenation with source location
let string1: string = "Hello";
let string2: string = "World";
let string3: string = string1 + " " + string2;

console.log("String 1:", string1);
console.log("String 2:", string2);
console.log("String 3:", string3);

// This test should compile successfully and exercise source location handling
// across all basic TypeScript features that are currently supported
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()