// Test semantic analysis only (no code generation)
class Container<T> {
    value: T;
    
    testVariableMethodCall(): string {
        let temp = this.value;       // Variable assignment - should work  
        temp.toString;              // Property access only (no call) - should work
        return "test";              // Simple return to avoid complex codegen
    }
}

// No main function to avoid instantiation issues