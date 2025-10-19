// Test with function call
let x: number = 42;

function testFunction(param: number): number {
    return param * 2;
}

let result: number = testFunction(x);
console.log("result:", result);