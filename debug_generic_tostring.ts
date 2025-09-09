class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    // This is the failing case - calling toString() on a generic type parameter
    getDescription(): string {
        return this.value.toString();  // T.toString() - this should work
    }
}

function main(): number {
    let container = new Container<number>(42);
    let desc = container.getDescription();  // Should return "42"
    return 0;
}