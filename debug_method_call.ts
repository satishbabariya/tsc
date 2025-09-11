class SimpleClass<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
}

function main(): number {
    let container = new SimpleClass<number>(42);
    let result = container.getValue();  // This should work
    return 0;
}