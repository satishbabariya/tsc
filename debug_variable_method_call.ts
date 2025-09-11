class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    testVariableMethodCall(): string {
        let temp = this.value;       // ✅ This works (temp has type T)
        return temp.toString();      // ❌ This fails - method call on variable of type T
    }
}

// Compare with function parameter (which we know works)
function testFunctionParam<T>(value: T): string {
    return value.toString();  // ✅ This works - method call on parameter of type T
}

function main(): number {
    let container = new Container<number>(42);
    let result = testFunctionParam<number>(42);  // This should work
    return 0;
}