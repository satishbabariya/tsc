// Comprehensive test suite for interface type constraints (supported features only)
// This tests various scenarios of interface type constraints that are currently supported

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

// Test 5: Complex inheritance chain
interface BaseInterface<T> {
    data: T;
}

interface MiddleInterface<T extends number> extends BaseInterface<T> {
    numericValue(): number;
}

interface TopInterface<T extends number> extends MiddleInterface<T> {
    doubleValue(): number;
}

// Test 6: Interface with array constraints
interface ArrayProcessor<T extends number> {
    numbers: T[];
    sum(): T;
    average(): number;
}

// Test 7: Interface with multiple extends
interface FirstInterface<T> {
    first: T;
}

interface SecondInterface<T> {
    second: T;
}

interface CombinedInterface<T extends string> extends FirstInterface<T>, SecondInterface<T> {
    combined(): string;
}