// Test case to isolate the type identity issue

class ComplexContainer<T, U, V> {
    first: T;
    second: U;
    third: V;
    
    constructor(first: T, second: U, third: V) {
        this.first = first;
        this.second = second;
        this.third = third;
    }
}

class NestedGeneric<T> {
    container: ComplexContainer<T, string, number>;
    
    constructor(value: T) {
        // This line causes: Type mismatch: expected ComplexContainer<T, string, number>, got ComplexContainer<T, string, number>
        this.container = new ComplexContainer<T, string, number>(value, "nested", 42);
    }
}

function main(): number {
    let nested = new NestedGeneric<number>(456);
    return 0;
}