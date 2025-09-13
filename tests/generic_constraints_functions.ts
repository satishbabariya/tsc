// Generic constraint tests for functions
function processString<T extends string>(value: T): T {
    return value;
}

function processNumber<T extends number>(value: T): T {
    return value;
}

function processBoolean<T extends boolean>(value: T): T {
    return value;
}

// Test multiple constraints in functions
function processMulti<T extends string, U extends number>(text: T, count: U): T {
    return text;
}

// Test constraint with union types in functions
function processUnion<T extends string | number>(value: T): T {
    return value;
}

// Test nested constraints
function processNested<T extends string>(value: T): T {
    return processString(value);
}