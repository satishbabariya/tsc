// Isolated test case for method call resolution issue
class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    // This is the exact failing case
    testMethodCall(): string {
        return this.value.toString();  // CallExpression with PropertyAccess callee
    }
}

function main(): number {
    let container = new Container<number>(42);
    let result = container.testMethodCall();
    return 0;
}