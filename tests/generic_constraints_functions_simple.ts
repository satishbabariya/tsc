// Simple generic constraint tests for functions
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