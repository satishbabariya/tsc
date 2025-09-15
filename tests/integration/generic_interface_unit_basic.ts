// Unit tests for basic generic interface functionality
interface SingleParam<T> {
    value: T;
    getValue(): T;
    setValue(value: T): void;
}

interface MultiParam<T, U> {
    first: T;
    second: U;
    getFirst(): T;
    getSecond(): U;
}

interface TripleParam<T, U, V> {
    first: T;
    second: U;
    third: V;
}

// Test type parameter resolution in properties
interface PropertyTest<T> {
    primitive: T;
    array: T[];
    union: T | string;
    optional: T | null;
}

// Test type parameter resolution in methods
interface MethodTest<T> {
    getValue(): T;
    setValue(value: T): void;
    processValue(value: T): T;
    getArray(): T[];
}