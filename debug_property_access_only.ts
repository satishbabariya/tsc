class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    test(): T {
        // Just access this.value - what type does it have?
        return this.value;  // This should work
    }
    
    testPropertyAccess() {
        // Test if this.value.toString exists (property access, not call)
        let temp = this.value;
        let toStringMethod = temp.toString;  // This should resolve to FunctionType
    }
}

function main(): number {
    let container = new Container<number>(42);
    return 0;
}