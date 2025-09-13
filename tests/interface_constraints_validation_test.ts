// Test for interface constraint validation
// This tests the semantic analysis for interface type constraints

// Test 1: Interface with type parameter constraints
interface NumberProcessor<T extends number> {
    value: T;
    process(): T;
}

// Test 2: Interface with string constraints
interface StringHandler<T extends string> {
    text: T;
    length(): number;
}

// Test 3: Interface extending another interface
interface BaseInterface<T> {
    data: T;
}

interface ExtendedInterface<T extends number> extends BaseInterface<T> {
    numericValue(): number;
}

// Test 4: Multiple type parameters with constraints
interface MultiConstrainedInterface<T extends string, U extends number> {
    text: T;
    count: U;
    combine(): string;
}