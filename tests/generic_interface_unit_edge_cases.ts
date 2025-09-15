// Unit tests for edge cases and complex scenarios
interface EmptyGeneric<T> {
    // Empty interface - should still parse correctly
}

interface SingleMethodGeneric<T> {
    process(value: T): void;
}

interface SinglePropertyGeneric<T> {
    data: T;
}

interface NestedGeneric<T> {
    container: Container<T>;
}

interface Container<T> {
    value: T;
}

interface RecursiveGeneric<T> {
    next: RecursiveGeneric<T> | null;
    value: T;
}

interface GenericWithLiterals<T> {
    value: T;
    status: "active" | "inactive";
    count: 0 | 1 | 2;
}

interface GenericWithUnions<T> {
    value: T | string;
    data: T | number | boolean;
}

interface GenericWithArrays<T> {
    items: T[];
    matrix: T[][];
    nested: (T | string)[];
}