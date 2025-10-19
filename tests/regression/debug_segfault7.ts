// Test with objects
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

let numbers: number[] = [1, 2, 3];
let first: number = numbers[0];
console.log("first number:", first);

let obj = { 
    value: 100, 
    name: "test", 
    active: true 
};

let objValue: number = obj.value;
console.log("object value:", objValue);

console.log("result:", result);