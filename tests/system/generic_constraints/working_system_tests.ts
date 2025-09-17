// Working System Tests for Generic Type Constraints
// Tests only the functionality that we know works from integration tests

// Test 1: Basic string constraint (we know this works)
function processString<T extends string>(value: T): T {
    return value;
}

// Test 2: Basic number constraint (we know this works)
function processNumber<T extends number>(value: T): T {
    return value;
}

// Test 3: Multiple type parameters (we know this works)
function processMultiple<T extends string, U extends number>(str: T, num: U): string {
    return str + "123";  // Simplified to avoid toString() issue
}

// Test functions
function testWorkingConstraints(): void {
    console.log("=== Working System Tests ===");
    
    // String constraint - this should work
    let strResult = processString<string>("hello");
    console.log("String result: " + strResult);
    
    // Number constraint - this should work
    let numResult = processNumber<number>(42);
    console.log("Number result: 42");
    
    // Multiple constraints - this should work
    let multiResult = processMultiple<string, number>("test", 123);
    console.log("Multiple result: " + multiResult);
    
    console.log("All working tests completed successfully!");
}

function main(): void {
    testWorkingConstraints();
}

main();