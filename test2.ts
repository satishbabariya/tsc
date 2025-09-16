// Simple test without deferred initialization issue
function add(a: number, b: number): number {
    return a + b;
}

let result: number = add(5, 3);
console.log("Result:", result);