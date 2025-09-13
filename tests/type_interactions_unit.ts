// Focused unit tests for complex type interactions
// Test 1: Union types within generic constraints
interface UnionConstraint<T extends string | number> {
    value: T;
    process(): T;
}

interface StringOrNumberContainer<T extends string | number> {
    data: T;
    getData(): T;
    setData(value: T): void;
}

// Test 2: Array types within generic constraints
interface ArrayConstraint<T extends string[]> {
    items: T;
    getItems(): T;
    addItem(item: string): void;
}

interface NumberArrayContainer<T extends number[]> {
    numbers: T;
    getNumbers(): T;
    sum(): number;
}

// Test 3: Literal types within generic constraints
interface LiteralConstraint<T extends "success" | "error"> {
    status: T;
    getStatus(): T;
    setStatus(status: T): void;
}

interface BooleanLiteralContainer<T extends true | false> {
    flag: T;
    getFlag(): T;
    toggle(): T;
}

// Test 4: Complex nested constraints
interface ComplexConstraint<T extends string | number, U extends T[]> {
    primary: T;
    secondary: U;
    process(): T;
}

interface MultiTypeContainer<T extends string | number, U extends boolean[]> {
    text: T;
    flags: U;
    combine(): T;
}