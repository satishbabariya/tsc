// Basic generic constraint tests
interface StringContainer<T extends string> {
    value: T;
    getValue(): T;
    setValue(value: T): void;
}

interface NumberContainer<T extends number> {
    value: T;
    getValue(): T;
    setValue(value: T): void;
}

interface BooleanContainer<T extends boolean> {
    value: T;
    getValue(): T;
    setValue(value: T): void;
}

// Test multiple constraints
interface MultiConstraint<T extends string, U extends number> {
    text: T;
    count: U;
    combine(): T;
}

// Test constraint with union types
interface UnionConstraint<T extends string | number> {
    value: T;
    process(): T;
}