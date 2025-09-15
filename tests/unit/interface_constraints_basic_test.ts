// Basic test for interface type constraints
// This tests the core functionality for interface extends clauses

// Test 1: Simple interface inheritance
interface BaseInterface {
    baseMethod(): void;
}

interface ExtendedInterface extends BaseInterface {
    extendedMethod(): void;
}

// Test 2: Generic interface with type constraints
interface ConstrainedInterface<T extends number> {
    value: T;
    process(): T;
}

// Test 3: Interface with multiple type constraints
interface MultiConstrainedInterface<T extends string, U extends number> {
    text: T;
    count: U;
    combine(): string;
}

// Test 4: Interface extending another interface with constraints
interface BaseGenericInterface<T> {
    data: T;
}

interface ExtendedGenericInterface<T extends string> extends BaseGenericInterface<T> {
    length(): number;
}

// Test 5: Interface with union type constraints
interface UnionConstrainedInterface<T extends string | number> {
    value: T;
    display(): string;
}