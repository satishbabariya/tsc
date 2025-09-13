// Simplified error handling tests for constraint validation

// Test 1: Constraint violations with simple types
interface StringConstraint<T extends string> {
    value: T;
}

interface NumberConstraint<T extends number> {
    value: T;
}

interface BooleanConstraint<T extends boolean> {
    value: T;
}

// These should fail with clear error messages
let invalidString: StringConstraint<number>; // number not assignable to string
let invalidNumber: NumberConstraint<string>; // string not assignable to number
let invalidBoolean: BooleanConstraint<string>; // string not assignable to boolean

// Test 2: Function constraint violations
function processString<T extends string>(value: T): T {
    return value;
}

function processNumber<T extends number>(value: T): T {
    return value;
}

// These should fail with clear error messages
let result1 = processString(42); // number not assignable to string
let result2 = processNumber("hello"); // string not assignable to number

// Test 3: Complex constraint violations
interface ComplexConstraint<T extends string, U extends number> {
    text: T;
    count: U;
}

// This should fail with clear error messages
let invalidComplex: ComplexConstraint<number, string>; // types swapped