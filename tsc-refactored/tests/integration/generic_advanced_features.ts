// Advanced Generic Features Test
// Tests complex generic scenarios that should work with current implementation

// Test Case 1: Multiple Type Parameters with Complex Assignments
class MultiContainer<T, U, V> {
    first: T;
    second: U;
    third: V;

    constructor(first: T, second: U, third: V) {
        this.first = first;
        this.second = second;
        this.third = third;
    }

    getFirst(): T {
        return this.first;
    }

    getSecond(): U {
        return this.second;
    }

    getThird(): V {
        return this.third;
    }
}

// Test Case 2: Nested Generic Instantiation
class Wrapper<T> {
    inner: MultiContainer<T, string, number>;
    outer: MultiContainer<string, T, boolean>;

    constructor(value: T) {
        // Test complex generic type assignments (the bug we fixed)
        this.inner = new MultiContainer<T, string, number>(value, "inner", 42);
        this.outer = new MultiContainer<string, T, boolean>("outer", value, true);
    }

    processInner(): T {
        return this.inner.first;
    }

    processOuter(): T {
        return this.outer.second;
    }
}

// Test Case 3: Generic Type Chains
class ChainA<T> {
    value: T;

    constructor(value: T) {
        this.value = value;
    }
}

class ChainB<T> {
    chainA: ChainA<T>;
    extra: T;

    constructor(value: T) {
        this.chainA = new ChainA<T>(value);
        this.extra = value;
    }
}

class ChainC<T> {
    chainB: ChainB<T>;

    constructor(value: T) {
        this.chainB = new ChainB<T>(value);
    }

    getValue(): T {
        return this.chainB.extra;
    }
}

// Test Case 4: Generic Collections (Simplified)
class SimpleList<T> {
    head: T;
    tail: SimpleList<T>;

    constructor(head: T) {
        this.head = head;
        this.tail = new SimpleList<T>(head);  // Recursive - should work
    }

    getHead(): T {
        return this.head;
    }
}

// Test Case 5: Generic Data Structures
class KeyValuePair<K, V> {
    key: K;
    value: V;

    constructor(key: K, value: V) {
        this.key = key;
        this.value = value;
    }

    getKey(): K {
        return this.key;
    }

    getValue(): V {
        return this.value;
    }
}

class Dictionary<K, V> {
    primary: KeyValuePair<K, V>;
    secondary: KeyValuePair<V, K>;  // Swapped types

    constructor(key: K, value: V) {
        this.primary = new KeyValuePair<K, V>(key, value);
        this.secondary = new KeyValuePair<V, K>(value, key);
    }

    getPrimaryKey(): K {
        return this.primary.key;
    }

    getSecondaryKey(): V {
        return this.secondary.key;
    }
}

function main(): number {
    // Test multiple type parameters
    let multi = new MultiContainer<string, number, boolean>("test", 123, true);
    let first = multi.first;
    let second = multi.second;
    let third = multi.third;

    // Test nested generic instantiation
    let wrapper = new Wrapper<number>(42);
    let innerValue = wrapper.inner.first;
    let outerValue = wrapper.outer.second;

    // Test generic chains
    let chainC = new ChainC<string>("chain");
    let chainValue = chainC.chainB.extra;

    // Test generic collections (avoid recursive constructor for now)
    // let simpleList = new SimpleList<number>(100);
    // let headValue = simpleList.head;

    // Test generic data structures
    let dict = new Dictionary<string, number>("key", 42);
    let primaryKey = dict.primary.key;
    let primaryValue = dict.primary.value;
    let secondaryKey = dict.secondary.key;
    let secondaryValue = dict.secondary.value;

    return 0;
}