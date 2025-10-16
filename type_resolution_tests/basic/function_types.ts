// Basic function type resolution tests
// Test: function parameter types
function add(x: number, y: number): number {
    return x + y;
}

// Test: function return type inference
function multiply(a: number, b: number) {
    return a * b;
}

// Test: function as variable
let func: (x: number, y: number) => number = add;

// Test: function calls with type resolution
let result1: number = add(5, 10);
let result2: number = multiply(3, 4);
let result3: number = func(2, 8);

console.log("Add result:", result1);
console.log("Multiply result:", result2);
console.log("Func result:", result3);