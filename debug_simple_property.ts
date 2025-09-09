class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    // Test simple property access first
    test(): T {
        return this.value;  // This should work - just property access
    }
}

function main(): number {
    let container = new Container<number>(42);
    let val = container.test();  // This should work
    return 0;
}