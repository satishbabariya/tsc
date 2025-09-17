// Basic test for generic function constraints
// This tests the core generic function constraint functionality

// Test 1: Basic generic function with number constraint
function processNumber<T extends number>(value: T): T {
    return value;
}

// Test 2: Generic function with string constraint
function processString<T extends string>(text: T): T {
    return text;
}

// Test 3: Generic function with array constraint
function processArray<T extends number[]>(items: T): T {
    return items;
}

// Test 4: Generic function with union type constraint
function processUnion<T extends string | number>(value: T): T {
    return value;
}

// Test 5: Generic function with multiple type parameters and constraints
function processMultiple<T extends number, U extends string>(numeric: T, text: U): string {
    return text + numeric.toString();
}

// Test 6: Generic function with complex constraint
function processComplex<T extends number[], U extends string>(array: T, text: U): string {
    return text + array.length.toString();
}

// Test 7: Generic function with return type constraint
function createArray<T extends number>(): T[] {
    return [];
}

// Test 8: Generic function with parameter constraint
function validateInput<T extends number>(input: T[]): boolean {
    return input.length > 0;
}

// Test 9: Generic function with method constraint
function processObject<T extends { id: number }>(obj: T): number {
    return obj.id;
}

// Test 10: Generic function with nested constraint
function processNested<T extends { data: number[] }>(obj: T): number {
    return obj.data.length;
}