// Negative tests for generic constraints - these should fail with proper error messages

// Test 1: Violating string constraint
interface InvalidStringContainer<T extends string> {
    value: T;
}

// This should fail because number doesn't extend string
let invalidString: InvalidStringContainer<number>;

// Test 2: Violating number constraint
interface InvalidNumberContainer<T extends number> {
    value: T;
}

// This should fail because string doesn't extend number
let invalidNumber: InvalidNumberContainer<string>;

// Test 3: Violating boolean constraint
interface InvalidBooleanContainer<T extends boolean> {
    value: T;
}

// This should fail because number doesn't extend boolean
let invalidBoolean: InvalidBooleanContainer<number>;

// Test 4: Violating union constraint
interface InvalidUnionContainer<T extends string | number> {
    value: T;
}

// This should fail because boolean doesn't extend string | number
let invalidUnion: InvalidUnionContainer<boolean>;

// Test 5: Missing constraint in function call
function constrainedFunction<T extends string>(value: T): T {
    return value;
}

// This should fail because number doesn't extend string
let result = constrainedFunction(42);