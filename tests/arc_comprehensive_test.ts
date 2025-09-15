// Comprehensive ARC (Automatic Reference Counting) test

// Test basic variable declarations with ARC
let numberVar: number = 42;
let stringVar: string = "ARC test";
let booleanVar: boolean = true;

// Test array operations with ARC
let numberArray: Array<number> = [1, 2, 3, 4, 5];
numberArray.push(6);
numberArray.push(7);

let stringArray: Array<string> = ["hello", "world"];
stringArray.push("ARC");

// Test object operations with ARC
function createObject() {
    let obj = {
        value: 100,
        name: "test object"
    };
    return obj;
}

// Test function scope and ARC
function testFunctionScope() {
    let localVar: number = 200;
    let localArray: Array<number> = [10, 20, 30];
    
    // These variables should be automatically cleaned up when function exits
    localArray.push(40);
    
    console.log("Function scope test completed");
}

// Test nested scopes with ARC
function testNestedScopes() {
    let outerVar: number = 300;
    
    {
        let innerVar: number = 400;
        let innerArray: Array<string> = ["inner", "scope"];
        innerArray.push("test");
        
        console.log("Inner scope test completed");
    }
    
    // innerVar and innerArray should be cleaned up here
    console.log("Outer scope test completed");
}

// Test reference counting with assignments
function testReferenceCounting() {
    let originalArray: Array<number> = [1, 2, 3];
    let referenceToArray = originalArray;
    
    // Both variables point to the same array
    referenceToArray.push(4);
    
    // Reference count should be 2 here
    console.log("Reference counting test completed");
    
    // When function exits, reference count should drop to 0 and array should be cleaned up
}

// Test array method operations with ARC
function testArrayMethods() {
    let testArray: Array<number> = [5, 10, 15, 20];
    
    // Test various array methods
    let length = testArray.length;
    let firstElement = testArray[0];
    let lastElement = testArray[testArray.length - 1];
    
    testArray.push(25);
    testArray.push(30);
    
    let popped = testArray.pop();
    
    console.log("Array methods test completed");
}

// Test object property access with ARC
function testObjectProperties() {
    let person = {
        name: "Alice",
        age: 30,
        skills: ["TypeScript", "ARC", "LLVM"]
    };
    
    person.skills.push("Memory Management");
    
    console.log("Object properties test completed");
}

// Main test execution
console.log("Starting comprehensive ARC test");

testFunctionScope();
testNestedScopes();
testReferenceCounting();
testArrayMethods();
testObjectProperties();

console.log("Comprehensive ARC test completed successfully");