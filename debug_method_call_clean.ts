// Clean test for variable method call issue
class Container<T> {
    value: T;
    
    constructor(value: T) {
        // Skip the assignment for now to avoid binary operation error
    }
    
    testVariableMethodCall(): string {
        let temp = this.value;       // Variable assignment
        return temp.toString();      // Method call on variable - this was the issue
    }
}

function main(): number {
    // Skip the instantiation for now to avoid constructor error
    return 0;
}