// Basic primitive type resolution tests
// Test: number type resolution
let num: number = 42;
let result: number = num + 10;

// Test: string type resolution
let str: string = "hello";
let concat: string = str + " world";

// Test: boolean type resolution
let flag: boolean = true;
let negated: boolean = !flag;

// Test: type inference
let inferred = 100; // should be number
let inferredStr = "test"; // should be string

console.log("Number:", result);
console.log("String:", concat);
console.log("Boolean:", negated);
console.log("Inferred number:", inferred);
console.log("Inferred string:", inferredStr);