class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    test1(): string {
        // Two-step approach - should work
        let temp = this.value;      // temp has type T (TypeParameterType)
        return temp.toString();     // Should work
    }
    
    test2(): string {
        // Direct chained access - might not work
        return this.value.toString();  // Might fail
    }
}

function main(): number {
    let container = new Container<number>(42);
    return 0;
}