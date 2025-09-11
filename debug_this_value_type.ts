class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    test(): T {
        // What type does this.value resolve to?
        let temp = this.value;  // This should work and temp should have type T
        return temp;
    }
    
    testToString(): string {
        // What type does this.value resolve to in this context?
        let temp = this.value;       // Step 1: this should work
        return temp.toString();      // Step 2: this should work (we know it works for function params)
    }
}

function main(): number {
    let container = new Container<number>(42);
    return 0;
}