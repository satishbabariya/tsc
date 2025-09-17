// Comprehensive test suite for generic type arguments in properties and methods

// ===== INTERFACE TESTS =====

// Basic generic interface with type arguments in properties
interface BasicContainer<T> {
    value: T;
    items: T[];
    metadata: Container<string>;
}

// Generic interface with type arguments in methods
interface Processor<T, U> {
    process(input: T): U;

    transform(items: T[]): U[];

    combine(first: T, second: T): U;
}

// Nested generic types
interface NestedContainer<T, U> {
    primary: Container<T>;
    secondary: Container<U>;
    combined: Container<T | U>;
}

// ===== CLASS TESTS =====

// Basic generic class
class DataStore<T> {
    private storage: T[];
    private config: Container<T>;

    add(item: T): void {
        this.storage.push(item);
    }

    get(index: number): T {
        return this.storage[index];
    }

    getAll(): T[] {
        return this.storage;
    }

    getLength(): number {
        return this.storage.length;
    }
}

// Generic class with multiple type parameters
class KeyValueStore<K, V> {
    private data: Container<V>;
    private keys: K[];

    set(key: K, value: V): void {
        this.keys.push(key);
        this.data.value = value;
    }

    get(key: K): V {
        return this.data.value;
    }

    getKeys(): K[] {
        return this.keys;
    }
}

// Generic class with constrained type parameters
class NumberContainer<T extends number> {
    private numbers: T[];
    private sum: T;

    addNumber(num: T): void {
        this.numbers.push(num);
    }

    getNumbers(): T[] {
        return this.numbers;
    }

    calculateSum(): T {
        return this.sum;
    }
}

// ===== FUNCTION TESTS =====

// Generic function with type arguments
function createContainer<T>(data: T): Container<T> {
    return {
        value: data,
        items: [data],
        metadata: {value: "test", items: ["test"], metadata: null as any}
    };
}

// Generic function with array type arguments
function processArray<T>(items: T[]): T[] {
    return items;
}

// Generic function with nested type arguments
function createNestedContainer<T, U>(primary: T, secondary: U): NestedContainer<T, U> {
    return {
        primary: createContainer(primary),
        secondary: createContainer(secondary),
        combined: createContainer(primary)
    };
}

// ===== ARRAY METHOD TESTS =====

// Test all array methods with generic types
class ArrayMethodTester<T> {
    private items: T[];

    constructor() {
        this.items = [];
    }

    testPush(item: T): number {
        return this.items.push(item);
    }

    testPop(): T {
        return this.items.pop();
    }

    testShift(): T {
        return this.items.shift();
    }

    testUnshift(item: T): number {
        return this.items.unshift(item);
    }

    testLength(): number {
        return this.items.length;
    }

    testConcat(other: T[]): T[] {
        return this.items.concat(other);
    }

    testSlice(start: number, end: number): T[] {
        return this.items.slice(start, end);
    }
}

// ===== COMPLEX INTEGRATION TESTS =====

// Generic class with interface implementation
class GenericProcessor<T, U> implements Processor<T, U> {
    private cache: Container<U>;

    process(input: T): U {
        return this.cache.value;
    }

    transform(items: T[]): U[] {
        return [];
    }

    combine(first: T, second: T): U {
        return this.cache.value;
    }
}

// Generic class with inheritance
class ExtendedDataStore<T> extends DataStore<T> {
    private backup: Container<T>;

    backupData(): void {
        this.backup = createContainer(this.getAll()[0]);
    }

    restoreData(): void {
        this.add(this.backup.value);
    }
}

// ===== TYPE ALIAS TESTS =====

// Generic type aliases
type StringArray = string[];
type NumberContainer = Container<number>;
type GenericFunction<T> = (input: T) => T;

// ===== EDGE CASE TESTS =====

// Empty generic type parameters
class EmptyGeneric<T> {
    value: T;
}

// Single character type parameters
class SimpleGeneric<A> {
    data: A;
}

// Multiple single character type parameters
class MultiGeneric<A, B, C> {
    first: A;
    second: B;
    third: C;
}