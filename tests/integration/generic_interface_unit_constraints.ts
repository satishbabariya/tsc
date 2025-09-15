// Unit tests for generic interface constraints (parser level - constraints are parsed but not enforced yet)
interface ConstrainedInterface<T extends string> {
    value: T;
    process(): T;
}

interface MultiConstrainedInterface<T extends string, U extends number> {
    text: T;
    count: U;
    combine(): T;
}

interface NestedConstraintInterface<T extends string[]> {
    items: T;
    first(): string;
}

// Test variance annotations (parser level)
interface CovariantInterface<out T> {
    getValue(): T;
}

interface ContravariantInterface<in T> {
    setValue(value: T): void;
}

interface InvariantInterface<T> {
    value: T;
    getValue(): T;
    setValue(value: T): void;
}