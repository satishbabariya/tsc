// Minimal regression test for source location handling fix
// This test focuses only on core functionality that was fixed

// Test 1: Basic variable declarations (triggers source location access)
let x: number = 42;
let y: string = "hello";
let z: boolean = true;

// Test 2: Function declarations (triggers source location access)
function testFunction(param: number): number {
    return param * 2;
}

// Test 3: Function calls (triggers source location access)
let result: number = testFunction(x);

// Test 4: Console.log calls (triggers source location in debug output)
console.log("x:", x);
console.log("y:", y);
console.log("z:", z);
console.log("result:", result);

// Test 5: Conditional statements (triggers source location access)
if (x > 0) {
    console.log("x is positive");
} else {
    console.log("x is not positive");
}

// Test 6: While loops (triggers source location access)
let counter: number = 0;
while (counter < 2) {
    console.log("counter:", counter);
    counter = counter + 1;
}

// Test 7: Array operations (triggers source location access)
let numbers: number[] = [1, 2, 3];
let first: number = numbers[0];
console.log("first number:", first);

// Test 8: Object operations (triggers source location access)
let obj = { value: 100, name: "test" };
let objValue: number = obj.value;
console.log("object value:", objValue);

// This test should compile successfully and exercise the source location fix
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()