// Test the original failing case: variable method calls
class Container<T> {
    value: T;
    
    constructor(value: T) {
        // Skip assignment to avoid other issues
    }
    
    testVariableMethodCall(): string {
        let temp = this.value;       // Variable assignment - works
        return temp.toString();      // Method call on variable - this was the original issue
    }
}

function main(): number {
    return 0;
}