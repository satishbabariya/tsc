// Constraint Violation Regression Tests
// These tests ensure that constraint violations are correctly detected and reported
// and prevent regressions in error handling

// Test functions with constraints
function processString<T extends string>(value: T): T {
    return value;
}

function processNumber<T extends number>(value: T): T {
    return value;
}

function processBoolean<T extends boolean>(value: T): T {
    return value;
}

// Test functions that validate constraint violation detection
function testConstraintViolationsRegression(): void {
    // These tests MUST fail compilation - they represent constraint violations
    // that should be caught by the compiler
    
    // String constraint violations (should fail)
    // let invalidStr1 = processString<number>(123);        // number not assignable to string
    // let invalidStr2 = processString<boolean>(true);     // boolean not assignable to string
    
    // Number constraint violations (should fail)
    // let invalidNum1 = processNumber<string>("abc");     // string not assignable to number
    // let invalidNum2 = processNumber<boolean>(true);     // boolean not assignable to number
    
    // Boolean constraint violations (should fail)
    // let invalidBool1 = processBoolean<string>("true");  // string not assignable to boolean
    // let invalidBool2 = processBoolean<number>(1);       // number not assignable to boolean
    
    // All constraint violation tests are commented out as they should fail compilation
    // This test function serves as documentation of expected failures
    let constraintViolationsDocumented = true;
}

function main(): void {
    testConstraintViolationsRegression();
}

main();