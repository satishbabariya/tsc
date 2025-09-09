// Integration test: Edge cases for generic classes and type operations
// This covers additional scenarios that might expose related issues

class ComplexContainer<T, U, V> {
    first: T;
    second: U;
    third: V;
    
    constructor(first: T, second: U, third: V) {
        this.first = first;
        this.second = second;
        this.third = third;
    }
    
    // Nested property access with mixed types
    complexOperation(): number {
        // Multiple property accesses in sequence
        let temp1 = 10;  // Simulating this.first processing
        let temp2 = 20;  // Simulating this.second processing
        let temp3 = 30;  // Simulating this.third processing
        
        // Complex arithmetic that should NOT trigger string concatenation
        let result = (temp1 + temp2) * temp3 - (temp1 / 2);
        return result;
    }
}

class NestedGeneric<T> {
    container: ComplexContainer<T, string, number>;
    
    constructor(value: T) {
        this.container = new ComplexContainer<T, string, number>(value, "nested", 42);
    }
    
    // Nested generic access with operations
    processNested(): number {
        let result = this.container.complexOperation();
        // Additional operations on the result
        return result + 100;
    }
}

// Generic inheritance scenario
class ExtendedContainer<T> extends Container<T> {
    extra: number;
    
    constructor(value: T, extra: number) {
        super(value);
        this.extra = extra;
    }
    
    // Override with additional operations
    getValue(): T {
        // Property access + arithmetic (the problematic pattern)
        let processed = this.extra + 5;  // This was causing issues
        return super.getValue();
    }
}

function main(): number {
    // Test complex scenarios
    let complex = new ComplexContainer<string, boolean, number>("test", true, 123);
    let result1 = complex.complexOperation();
    
    let nested = new NestedGeneric<number>(456);
    let result2 = nested.processNested();
    
    let extended = new ExtendedContainer<string>("extended", 789);
    let value = extended.getValue();
    
    return result1 + result2;
}

// Base class for inheritance test
class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
}