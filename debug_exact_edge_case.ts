// Exact reproduction of the Generic Edge Cases structure

class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
}

class ExtendedContainer<T> extends Container<T> {
    extra: number;
    
    constructor(value: T, extra: number) {
        super(value);
        this.extra = extra;
    }
    
    // Override with additional operations
    getValue(): T {
        // Property access + arithmetic (the problematic pattern)
        let processed = this.extra + 5;  // This was causing issues
        return super.getValue();
    }
}

function main(): number {
    let extended = new ExtendedContainer<string>("test", 10);
    return 0;
}