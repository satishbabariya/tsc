// Regression test for source location handling in error scenarios
// This test focuses on error reporting and source location tracking

// Test 1: Basic variable declarations (should work)
let workingVar: number = 42;
let workingString: string = "working";

// Test 2: Function declarations (should work)
function workingFunction(param: number): string {
    return param.toString();
}

// Test 3: Valid function calls (should work)
let validResult: string = workingFunction(workingVar);
console.log("Valid result:", validResult);

// Test 4: Console.log calls (should work and trigger source location)
console.log("Working var:", workingVar);
console.log("Working string:", workingString);

// Test 5: Conditional statements (should work)
if (workingVar > 0) {
    console.log("workingVar is positive");
} else {
    console.log("workingVar is not positive");
}

// Test 6: While loops (should work)
let loopCounter: number = 0;
while (loopCounter < 2) {
    console.log("Loop iteration:", loopCounter);
    loopCounter = loopCounter + 1;
}

// Test 7: Array operations (should work)
let workingArray: number[] = [10, 20, 30];
let arrayElement: number = workingArray[0];
console.log("Array element:", arrayElement);

// Test 8: Object operations (should work)
let workingObj = { 
    value: 50, 
    name: "working object" 
};
let objValue: number = workingObj.value;
console.log("Object value:", objValue);

// Test 9: Complex expressions (should work)
let complexExpr1: number = workingVar + workingArray[0];
let complexExpr2: string = workingString + " " + workingVar.toString();
let complexExpr3: boolean = (workingVar > 0) && (workingString.length > 0);

console.log("Complex expr 1:", complexExpr1);
console.log("Complex expr 2:", complexExpr2);
console.log("Complex expr 3:", complexExpr3);

// Test 10: Nested function calls (should work)
function outerFunc(value: number): string {
    function innerFunc(nested: number): string {
        return "inner: " + nested.toString();
    }
    
    let innerResult: string = innerFunc(value);
    return "outer: " + innerResult;
}

let nestedResult: string = outerFunc(workingVar);
console.log("Nested result:", nestedResult);

// Test 11: Multiple variable declarations (should work)
let var1: number = 1;
let var2: number = 2;
let var3: number = 3;
let sum: number = var1 + var2 + var3;

console.log("Sum:", sum);

// Test 12: Function with multiple parameters (should work)
function multiParamFunc(p1: number, p2: string, p3: boolean): string {
    return p1.toString() + " " + p2 + " " + p3.toString();
}

let multiResult: string = multiParamFunc(var1, workingString, true);
console.log("Multi param result:", multiResult);

// Test 13: Nested conditionals (should work)
if (var1 > 0) {
    if (var2 > 0) {
        console.log("Both var1 and var2 are positive");
    } else {
        console.log("var1 is positive but var2 is not");
    }
} else {
    console.log("var1 is not positive");
}

// Test 14: Arithmetic expressions (should work)
let arithmetic1: number = (var1 + var2) * var3;
let arithmetic2: number = var1 * var2 + var3;
let arithmetic3: number = (var1 + var2) / var3;

console.log("Arithmetic 1:", arithmetic1);
console.log("Arithmetic 2:", arithmetic2);
console.log("Arithmetic 3:", arithmetic3);

// Test 15: String operations (should work)
let string1: string = "Hello";
let string2: string = "World";
let string3: string = string1 + " " + string2;
let string4: string = string3 + " " + workingVar.toString();

console.log("String 1:", string1);
console.log("String 2:", string2);
console.log("String 3:", string3);
console.log("String 4:", string4);

// This test should compile successfully and exercise source location handling
// in various scenarios that might trigger error reporting or debug output
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()