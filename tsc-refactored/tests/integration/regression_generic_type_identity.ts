// Regression test for Generic Type Identity Bug Fix
// This test specifically targets the bug where two identical generic types
// (e.g., Container<T>) were not being recognized as equivalent due to 
// incorrect parser behavior creating base types as TypeParameter instead of ClassType

// Test Case 1: Simple Generic Type Assignment
class SimpleContainer<T> {
    value: T;

    constructor(value: T) {
        this.value = value;
    }
}

class SimpleWrapper<T> {
    container: SimpleContainer<T>;

    constructor(value: T) {
        // This was failing with: "Type mismatch: expected SimpleContainer<T>, got SimpleContainer<T>"
        this.container = new SimpleContainer<T>(value);
    }
}

// Test Case 2: Multiple Type Parameters
class Dictionary<K, V> {
    key: K;
    value: V;

    constructor(key: K, value: V) {
        this.key = key;
        this.value = value;
    }
}

class DictionaryHolder<K, V> {
    dict: Dictionary<K, V>;

    constructor(key: K, value: V) {
        // This was failing with multiple type parameter mismatches
        this.dict = new Dictionary<K, V>(key, value);
    }
}

// Test Case 3: Complex Nested Generic Types
class ComplexContainer<T, U, V> {
    first: T;
    second: U;
    third: V;

    constructor(first: T, second: U, third: V) {
        this.first = first;
        this.second = second;
        this.third = third;
    }
}

class NestedWrapper<T> {
    container: ComplexContainer<T, string, number>;

    constructor(value: T) {
        // This was the exact failing case from Generic Edge Cases
        this.container = new ComplexContainer<T, string, number>(value, "nested", 42);
    }
}

// Test Case 4: Generic Assignment Chain
class ChainedContainer<T> {
    data: T;

    constructor(data: T) {
        this.data = data;
    }
}

class ChainedWrapper<T> {
    primary: ChainedContainer<T>;
    secondary: ChainedContainer<T>;

    constructor(value: T) {
        this.primary = new ChainedContainer<T>(value);
        // This assignment should work (same types)
        this.secondary = this.primary;
    }
}

function main(): number {
    // Test all scenarios to ensure they compile and work
    let simpleWrapper = new SimpleWrapper<number>(42);
    let dictHolder = new DictionaryHolder<string, boolean>("key", true);
    let nestedWrapper = new NestedWrapper<string>("test");
    let chainedWrapper = new ChainedWrapper<number>(123);

    return 0;
}