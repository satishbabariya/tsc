// Integration test: Generic classes with string concatenation scenarios
// This specifically targets the LLVM verification bug that was fixed

class Container<T> {
    value: T;

    constructor(value: T) {
        this.value = value;
    }

    getValue(): T {
        return this.value;
    }

    // This method was causing LLVM verification errors before the fix
    getDescription(): string {
        return "Container with value: " + this.value.toString();
    }
}

class Dictionary<K, V> {
    key: K;
    value: V;

    constructor(key: K, value: V) {
        this.key = key;
        this.value = value;
    }

    // Multiple property access with string operations
    getFullDescription(): string {
        return "Key: " + this.key.toString() + ", Value: " + this.value.toString();
    }
}

function main(): number {
    // Test cases that previously caused LLVM verification errors
    let stringContainer = new Container<string>("hello");
    let numberContainer = new Container<number>(42);
    let boolContainer = new Container<boolean>(true);

    // These property accesses + string operations were problematic
    let desc1 = stringContainer.getDescription();
    let desc2 = numberContainer.getDescription();
    let desc3 = boolContainer.getDescription();

    // Multi-parameter generic with string concatenation
    let dict = new Dictionary<string, number>("count", 100);
    let fullDesc = dict.getFullDescription();

    return 0;
}