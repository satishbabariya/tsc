// Test constraint validation in different scenarios
interface StringContainer<T extends string> {
    value: T;
}

interface NumberContainer<T extends number> {
    value: T;
}

// Test 1: Valid constraint usage
let validString: StringContainer<string>;
let validNumber: NumberContainer<number>;

// Test 2: Function with constraint validation
function processString<T extends string>(value: T): T {
    return value;
}

// Test 3: Valid function calls
let result1 = processString("hello");
let result2 = processString("world");

// Test 4: Function with multiple constraints
function processMulti<T extends string, U extends number>(text: T, count: U): T {
    return text;
}

// Test 5: Valid multi-constraint function calls
let result3 = processMulti("test", 42);