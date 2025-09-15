
// Error scenarios for for...of loops
function testErrorScenarios() {
    // Test with null/undefined
    const nullArray: number[] | null = null;
    const undefinedArray: number[] | undefined = undefined;
    
    // These should cause errors
    // for (const item of nullArray) { // Error: Object is possibly 'null'
    //     console.log(item);
    // }
    
    // for (const item of undefinedArray) { // Error: Object is possibly 'undefined'
    //     console.log(item);
    // }
    
    // Safe iteration with null checks
    if (nullArray) {
        for (const item of nullArray) {
            console.log("Null array item:", item);
        }
    }
    
    if (undefinedArray) {
        for (const item of undefinedArray) {
            console.log("Undefined array item:", item);
        }
    }
}

// Test with type mismatches
function testTypeMismatches() {
    const numbers: number[] = [1, 2, 3, 4, 5];
    
    // This should cause a type error
    // const strings: string[] = [];
    // for (const num of numbers) {
    //     strings.push(num); // Error: Type 'number' is not assignable to type 'string'
    // }
    
    // Correct way
    const strings: string[] = [];
    for (const num of numbers) {
        strings.push(num.toString());
    }
    
    console.log("Number strings:", strings);
}

// Test with non-iterable types
function testNonIterableTypes() {
    const notIterable = 42;
    const alsoNotIterable = { name: "test" };
    
    // These should cause errors
    // for (const item of notIterable) { // Error: Type 'number' is not iterable
    //     console.log(item);
    // }
    
    // for (const item of alsoNotIterable) { // Error: Type '{ name: string; }' is not iterable
    //     console.log(item);
    // }
}

// Test with async context errors
function testAsyncContextErrors() {
    const regularArray = [1, 2, 3, 4, 5];
    
    // This should cause an error in async context
    // async function asyncFunction() {
    //     for await (const item of regularArray) { // Error: Type 'number[]' is not async iterable
    //         console.log(item);
    //     }
    // }
}

// Test with variable redeclaration
function testVariableRedeclaration() {
    const numbers = [1, 2, 3, 4, 5];
    
    // This should cause an error
    // for (const numbers of numbers) { // Error: Cannot redeclare block-scoped variable 'numbers'
    //     console.log(numbers);
    // }
    
    // Correct way
    for (const num of numbers) {
        console.log("Number:", num);
    }
}

// Test with const reassignment
function testConstReassignment() {
    const numbers = [1, 2, 3, 4, 5];
    
    // This should cause an error
    // for (const num of numbers) {
    //     num = 10; // Error: Cannot assign to 'num' because it is a constant
    // }
    
    // Use let if reassignment is needed
    for (let num of numbers) {
        num = num * 2;
        console.log("Doubled:", num);
    }
}

// Run tests
testErrorScenarios();
testTypeMismatches();
testNonIterableTypes();
testAsyncContextErrors();
testVariableRedeclaration();
testConstReassignment();
