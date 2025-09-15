// Function-based tests for complex type interactions

// Test 1: Functions with union type constraints
function processUnionValue<T extends string | number>(value: T): T {
    return value;
}

function handleStringOrNumber<T extends string | number>(input: T): T {
    return processUnionValue(input);
}

// Test 2: Functions with array type constraints
function processStringArray<T extends string[]>(items: T): T {
    return items;
}

function handleNumberArray<T extends number[]>(numbers: T): T {
    return numbers;
}

// Test 3: Functions with literal type constraints
function processStatus<T extends "success" | "error">(status: T): T {
    return status;
}

function handleBooleanLiteral<T extends true | false>(flag: T): T {
    return flag;
}

// Test 4: Complex multi-constraint functions
function processComplex<T extends string | number, U extends T[]>(primary: T, secondary: U): T {
    return primary;
}

function handleMultiType<T extends string | number, U extends boolean[]>(text: T, flags: U): T {
    return text;
}

// Test 5: Functions with nested type constraints
function processNestedConstraint<T extends string | number>(value: T): T {
    if (typeof value === "string") {
        return value;
    } else {
        return value;
    }
}