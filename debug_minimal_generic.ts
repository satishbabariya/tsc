// Minimal test case for generic type assignment issue

class Container<T> {
    value: T;
    constructor(value: T) { this.value = value; }
}

class Holder<T> {
    item: Container<T>;
    
    constructor() {
        // This line should work but might fail with type mismatch
        this.item = new Container<T>("test" as any);
    }
}

function main(): number {
    return 0;
}