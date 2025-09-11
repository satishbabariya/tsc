class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    // Test just accessing this.value (no method call)
    test(): T {
        let temp = this.value;  // What type does this.value have?
        return temp;
    }
}

function main(): number {
    let container = new Container<number>(42);
    let result = container.test();
    return 0;
}