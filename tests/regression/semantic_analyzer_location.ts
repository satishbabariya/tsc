// Regression test for SemanticAnalyzer source location fix
// This test exercises the source location handling that was previously broken
// due to private member access violations in SemanticAnalyzer.cpp

// Test case 1: Variable declarations with source location tracking
let x: number = 42;
let y: string = "hello";
let z: boolean = true;

// Test case 2: Function calls that trigger source location access
console.log(x);
console.log(y);
console.log(z);

// Test case 3: Complex expressions that require source location tracking
let result: number = x + 10;
let message: string = y + " world";

// Test case 4: Control flow statements with source location
if (x > 0) {
    console.log("x is positive");
} else {
    console.log("x is not positive");
}

// Test case 5: Loop statements with source location
let i: number = 0;
while (i < 3) {
    console.log("iteration", i);
    i = i + 1;
}

// Test case 6: Function declaration with source location
function testFunction(param: number): string {
    return "value: " + param;
}

// Test case 7: Function call with source location
let funcResult: string = testFunction(x);

// Test case 8: Object property access with source location
let obj = { value: 100 };
console.log(obj.value);

// Test case 9: Array access with source location
let arr: number[] = [1, 2, 3];
console.log(arr[0]);

// Test case 10: Nested expressions requiring source location tracking
let complex: number = (x + y.length) * (z ? 2 : 1);

// This test should compile successfully and generate an executable
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()