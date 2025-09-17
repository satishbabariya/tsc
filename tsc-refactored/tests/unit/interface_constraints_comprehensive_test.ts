// Comprehensive test suite for interface type constraints
// This tests various scenarios of interface type constraints

// Test 1: Basic type constraints
interface NumberContainer<T extends number> {
    value: T;

    double(): T;
}

interface StringContainer<T extends string> {
    text: T;

    length(): number;
}

// Test 2: Interface inheritance with constraints
interface BaseProcessor<T> {
    process(input: T): T;
}

interface NumberProcessor<T extends number> extends BaseProcessor<T> {
    add(other: T): T;
}

interface StringProcessor<T extends string> extends BaseProcessor<T> {
    toUpperCase(): T;
}

// Test 3: Multiple type parameters with different constraints
interface MultiTypeContainer<T extends string, U extends number, V extends boolean> {
    text: T;
    count: U;
    flag: V;

    combine(): string;
}

// Test 4: Nested generic constraints
interface GenericWrapper<T> {
    wrapped: T;
}

interface ConstrainedWrapper<T extends number> extends GenericWrapper<T> {
    increment(): T;
}

// Test 5: Interface with method constraints
interface MethodConstraintInterface<T extends string> {
    value: T;

    process<U extends number>(input: U): string;
}

// Test 6: Complex inheritance chain
interface BaseInterface<T> {
    data: T;
}

interface MiddleInterface<T extends number> extends BaseInterface<T> {
    numericValue(): number;
}

interface TopInterface<T extends number> extends MiddleInterface<T> {
    doubleValue(): number;
}

// Test 7: Interface with array constraints
interface ArrayProcessor<T extends number> {
    numbers: T[];

    sum(): T;

    average(): number;
}

// Test 8: Interface with function constraints
interface FunctionContainer<T extends string> {
    text: T;

    transform<U extends number>(fn: (input: T) => U): U;
}