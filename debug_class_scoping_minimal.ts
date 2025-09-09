// Minimal test case to isolate class-level type parameter scoping issue
class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    // This is the exact failing case
    test(): string {
        return this.value.toString();  // T.toString() - scoping issue
    }
}

function main(): number {
    let container = new Container<number>(42);
    let result = container.test();
    return 0;
}