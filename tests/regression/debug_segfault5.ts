// Test with while loops
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

let counter: number = 0;
while (counter < 2) {
    console.log("counter:", counter);
    counter = counter + 1;
}

console.log("result:", result);