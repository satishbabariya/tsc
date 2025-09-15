// Basic ARC (Automatic Reference Counting) test

// Test basic variable declarations with ARC
let numberVar: number = 42;
let stringVar: string = "ARC test";

// Test array operations with ARC (basic operations only)
let numberArray: Array<number> = [1, 2, 3];

// Test function scope and ARC
function testFunctionScope() {
    let localVar: number = 200;
    let localArray: Array<number> = [10, 20, 30];
    
    console.log("Function scope test completed");
}

// Test nested scopes with ARC
function testNestedScopes() {
    let outerVar: number = 300;
    
    {
        let innerVar: number = 400;
        let innerArray: Array<string> = ["inner", "scope"];
        
        console.log("Inner scope test completed");
    }
    
    console.log("Outer scope test completed");
}

// Test reference counting with assignments
function testReferenceCounting() {
    let originalArray: Array<number> = [1, 2, 3];
    let referenceToArray = originalArray;
    
    console.log("Reference counting test completed");
}

// Main test execution
console.log("Starting basic ARC test");

testFunctionScope();
testNestedScopes();
testReferenceCounting();

console.log("Basic ARC test completed successfully");