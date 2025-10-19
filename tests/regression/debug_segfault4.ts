// Test with conditional statements
let x: number = 42;

function testFunction(param: number): number {
    return param * 2;
}

let result: number = testFunction(x);

if (x > 0) {
    console.log("x is positive");
} else {
    console.log("x is not positive");
}

console.log("result:", result);