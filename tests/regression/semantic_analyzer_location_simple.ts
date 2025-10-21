// Simple regression test for SemanticAnalyzer source location fix
// This test specifically exercises the source location handling that was broken
// due to private member access violations in SemanticAnalyzer.cpp

// Test case 1: Basic variable declarations (triggers source location access)
let x: number = 42;
let y: string = "hello";

// Test case 2: Function calls (triggers source location access in debug output)
console.log(x);
console.log(y);

// Test case 3: Conditional statement (triggers source location access)
if (x > 0) {
    console.log("x is positive");
}

// Test case 4: Function declaration (triggers source location access)
function testFunction(param: number): string {
    return "value: " + param;
}

// Test case 5: Function call (triggers source location access)
let result: string = testFunction(x);

// This test should compile successfully and generate an executable
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()
// The fix ensures proper source location tracking throughout compilation