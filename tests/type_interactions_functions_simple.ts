// Simplified function-based tests for complex type interactions

// Test 1: Functions with simple constraints
function processString<T extends string>(value: T): T {
    return value;
}

function processNumber<T extends number>(value: T): T {
    return value;
}

function processBoolean<T extends boolean>(value: T): T {
    return value;
}

// Test 2: Functions with array constraints
function processStringArray<T extends string[]>(items: T): T {
    return items;
}

function processNumberArray<T extends number[]>(numbers: T): T {
    return numbers;
}

// Test 3: Functions with literal constraints
function processSuccessStatus<T extends "success">(status: T): T {
    return status;
}

function processErrorStatus<T extends "error">(status: T): T {
    return status;
}

// Test 4: Complex multi-constraint functions
function processComplex<T extends string, U extends number>(text: T, count: U): T {
    return text;
}

function handleMultiType<T extends string, U extends boolean>(text: T, flag: U): T {
    return text;
}

// Test 5: Functions with nested constraints
function processNestedConstraint<T extends string>(value: T): T {
    return value;
}