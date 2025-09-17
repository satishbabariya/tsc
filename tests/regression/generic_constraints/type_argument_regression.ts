// Type Argument Parsing Regression Tests
// These tests ensure that type argument parsing continues to work correctly
// and prevent regressions in generic method calls with explicit type arguments

// Test 1: Function with type arguments (CRITICAL - must not regress)
function processString<T extends string>(value: T): T {
    return value;
}

function processNumber<T extends number>(value: T): T {
    return value;
}

function processMultiple<T extends string, U extends number>(str: T, num: U): string {
    return str + "123";
}

// Test functions that validate type argument parsing
function testTypeArgumentParsingRegression(): void {
    // These tests MUST pass - they represent core type argument parsing functionality
    
    // Function calls with explicit type arguments
    let strResult = processString<string>("hello");
    let numResult = processNumber<number>(42);
    let multiResult = processMultiple<string, number>("test", 123);
    
    // All tests completed successfully
    let allTestsPassed = true;
}

function main(): void {
    testTypeArgumentParsingRegression();
}

main();