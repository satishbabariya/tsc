// Test forward reference issue - Container defined after ExtendedContainer

class ExtendedContainer<T> extends Container<T> {
    extra: number;
    
    constructor(value: T, extra: number) {
        super(value);
        this.extra = extra;
    }
    
    getValue(): T {
        let processed = this.extra + 5;
        return super.getValue();  // This should fail if forward reference is the issue
    }
}

class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
}

function main(): number {
    return 0;
}