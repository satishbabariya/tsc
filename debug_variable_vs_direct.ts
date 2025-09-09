class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    testDirect(): T {
        // Direct access - does this work?
        return this.value;  // This should work
    }
    
    testVariable(): T {
        // Variable assignment - does this work?
        let temp = this.value;  // Does temp get the right type?
        return temp;  // This should work if temp has the right type
    }
}

function main(): number {
    let container = new Container<number>(42);
    return 0;
}