// Error handling tests for complex type interactions

// Test 1: Constraint violations with union types
interface UnionConstraint<T extends string | number> {
    value: T;
}

// These should fail with clear error messages
let invalidUnion1: UnionConstraint<boolean>; // boolean not in string | number
let invalidUnion2: UnionConstraint<null>; // null not in string | number

// Test 2: Constraint violations with array types
interface ArrayConstraint<T extends string[]> {
    items: T;
}

// These should fail with clear error messages
let invalidArray1: ArrayConstraint<number[]>; // number[] not assignable to string[]
let invalidArray2: ArrayConstraint<boolean[]>; // boolean[] not assignable to string[]

// Test 3: Constraint violations with literal types
interface LiteralConstraint<T extends "success" | "error"> {
    status: T;
}

// These should fail with clear error messages
let invalidLiteral1: LiteralConstraint<"pending">; // "pending" not in "success" | "error"
let invalidLiteral2: LiteralConstraint<"warning">; // "warning" not in "success" | "error"

// Test 4: Complex constraint violations
interface ComplexConstraint<T extends string | number, U extends T[]> {
    primary: T;
    secondary: U;
}

// These should fail with clear error messages
let invalidComplex1: ComplexConstraint<boolean, boolean[]>; // boolean not in string | number
let invalidComplex2: ComplexConstraint<string, number[]>; // number[] not assignable to string[]

// Test 5: Function constraint violations
function processValue<T extends string | number>(value: T): T {
    return value;
}

// These should fail with clear error messages
let result1 = processValue(true); // boolean not in string | number
let result2 = processValue(null); // null not in string | number

// Test 6: Nested constraint violations
interface OuterConstraint<T extends string | number> {
    inner: InnerConstraint<T>;
}

interface InnerConstraint<T extends string | number> {
    value: T;
}

// This should fail with clear error messages
let invalidNested: OuterConstraint<boolean>; // boolean not in string | number