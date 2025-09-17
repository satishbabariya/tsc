// Function-based tests for basic type interactions

// Test 1: Simple generic functions
function processValue<T>(value: T): T {
    return value;
}

// Test 2: Simple generic functions with arrays
function processArray<T>(items: T): T {
    return items;
}

// Test 3: Simple generic functions with boolean
function processBoolean<T>(status: T): T {
    return status;
}

// Test 4: Simple generic functions with multiple types
function processComplex<T, U>(primary: T, secondary: U): T {
    return primary;
}

// Test 5: Simple generic functions with return
function processNested<T>(value: T): T {
    return value;
}