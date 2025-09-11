// Test case to isolate super method call issue with generics

class Base<T> {
    value: T;
    constructor(value: T) { this.value = value; }
    
    getValue(): T {
        return this.value;
    }
}

class Derived<T> extends Base<T> {
    constructor(value: T) {
        super(value);
    }
    
    getValue(): T {
        // This should work but might fail with "Expression is not callable"
        return super.getValue();
    }
}

function main(): number {
    let d = new Derived<number>(42);
    return 0;
}