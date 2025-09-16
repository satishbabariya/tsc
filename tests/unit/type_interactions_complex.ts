// Complex integration tests for type interactions

// Test 1: Interface inheritance with constraints
interface BaseContainer<T extends string | number> {
    content: T;

    getContent(): T;
}

interface StringContainer<T extends string> extends BaseContainer<T> {
    length(): number;
}

interface NumberContainer<T extends number> extends BaseContainer<T> {
    value(): T;
}

// Test 2: Complex generic interfaces with multiple constraints
interface DataProcessor<T extends string | number, U extends T[]> {
    data: T;
    history: U;

    process(): T;

    addToHistory(item: T): void;
}

interface CacheManager<T extends string[], U extends number[]> {
    keys: T;
    values: U;

    get(key: string): number;

    set(key: string, value: number): void;
}

// Test 3: Nested generic constraints
interface OuterContainer<T extends string | number> {
    inner: InnerContainer<T>;

    process(): T;
}

interface InnerContainer<T extends string | number> {
    value: T;

    getValue(): T;
}

// Test 4: Complex array constraints
interface ArrayProcessor<T extends string[], U extends number[]> {
    strings: T;
    numbers: U;

    combine(): string;

    sum(): number;
}

// Test 5: Union and array combination
interface UnionArrayContainer<T extends (string | number)[]> {
    mixed: T;

    process(): T;

    filterStrings(): string[];

    filterNumbers(): number[];
}