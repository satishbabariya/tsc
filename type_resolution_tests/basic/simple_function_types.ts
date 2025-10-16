// Simplified function type tests (without complex syntax)
function add(x: number, y: number): number {
    return x + y;
}

function multiply(a: number, b: number): number {
    return a * b;
}

// Test: function calls with type resolution
let result1: number = add(5, 10);
let result2: number = multiply(3, 4);

console.log("Add result:", result1);
console.log("Multiply result:", result2);