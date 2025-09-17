// Simplified Basic Generic Type Constraint System Tests
// Tests fundamental constraint validation with primitive types

// Test 1: String constraints
function processString<T extends string>(value: T): T {
    return value;
}

// Test 2: Number constraints  
function processNumber<T extends number>(value: T): T {
    return value;
}

// Test 3: Boolean constraints
function processBoolean<T extends boolean>(value: T): T {
    return value;
}

// Test 4: Multiple type parameters with different constraints
function processMultiple<T extends string, U extends number>(str: T, num: U): string {
    return str + num.toString();
}

// Test 5: Constraint with return type
function getLength<T extends string>(value: T): number {
    return value.length;
}

// Test 6: Nested generic calls
function processNested<T extends string>(value: T): T {
    return processString<T>(value);
}

// Test functions
function testBasicConstraints(): void {
    console.log("=== Basic Constraint Tests ===");
    
    // Valid string constraint
    let strResult = processString<string>("hello");
    console.log("String result: " + strResult);
    
    // Valid number constraint
    let numResult = processNumber<number>(42);
    console.log("Number result: " + numResult.toString());
    
    // Valid boolean constraint
    let boolResult = processBoolean<boolean>(true);
    console.log("Boolean result: " + boolResult.toString());
    
    // Multiple constraints
    let multiResult = processMultiple<string, number>("test", 123);
    console.log("Multiple result: " + multiResult);
    
    // Length function
    let lengthResult = getLength<string>("testing");
    console.log("Length result: " + lengthResult.toString());
    
    // Nested calls
    let nestedResult = processNested<string>("nested");
    console.log("Nested result: " + nestedResult);
}

// Test constraint violations (should fail compilation)
function testConstraintViolations(): void {
    console.log("=== Constraint Violation Tests (Expected Failures) ===");
    
    // These should cause compilation errors:
    // let invalidStr = processString<number>(123);        // number not assignable to string
    // let invalidNum = processNumber<string>("abc");     // string not assignable to number  
    // let invalidBool = processBoolean<number>(42);      // number not assignable to boolean
    // let invalidMulti = processMultiple<number, string>(123, "test"); // wrong order
    
    console.log("Constraint violation tests are commented out as they should fail compilation");
}

function main(): void {
    testBasicConstraints();
    testConstraintViolations();
}

main();