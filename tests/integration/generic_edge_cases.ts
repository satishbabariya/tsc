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

// Additional Edge Cases for Comprehensive Testing

// Test Case 5: Nested Generics (Simplified)
class NestedList<T> {
    value: T;

    constructor(value: T) {
        this.value = value;
    }

    getValue(): T {
        return this.value;
    }
}

// Test Case 6: Multiple Inheritance Levels with Generics
class BaseGeneric<T> {
    baseValue: T;

    constructor(value: T) {
        this.baseValue = value;
    }

    getBase(): T {
        return this.baseValue;
    }
}

class MiddleGeneric<T, U> extends BaseGeneric<T> {
    middleValue: U;

    constructor(baseVal: T, middleVal: U) {
        super(baseVal);
        this.middleValue = middleVal;
    }

    getMiddle(): U {
        return this.middleValue;
    }
}

class TopGeneric<T, U, V> extends MiddleGeneric<T, U> {
    topValue: V;

    constructor(baseVal: T, middleVal: U, topVal: V) {
        super(baseVal, middleVal);
        this.topValue = topVal;
    }

    getTop(): V {
        return this.topValue;
    }

    // Complex method using all type parameters
    combineAll(): T {
        // This exercises multiple inheritance levels with generics
        let base = this.getBase();
        let middle = this.getMiddle();
        let top = this.getTop();
        return base;  // Simplified return for now
    }
}

// Test Case 7: Generic Type Constraints and Complex Assignments
class ConstrainedContainer<T> {
    data: T;
    metadata: ComplexContainer<T, string, boolean>;

    constructor(value: T) {
        this.data = value;
        // This tests complex generic type assignments with constraints
        this.metadata = new ComplexContainer<T, string, boolean>(value, "metadata", true);
    }

    processData(): T {
        // Complex processing that exercises type system
        let temp = this.metadata.first;
        return temp;
    }
}

// Test Case 8: Generic Collections
class GenericCollection<T> {
    items: T;  // Simplified - would be array in full implementation
    count: number;

    constructor(item: T) {
        this.items = item;
        this.count = 1;
    }

    add(item: T): number {
        // This would add to collection in full implementation
        this.count = this.count + 1;
        return this.count;
    }

    getItem(): T {
        return this.items;
    }
}

function main(): number {
    // Test original complex scenarios
    let complex = new ComplexContainer<string, boolean, number>("test", true, 123);
    let result1 = complex.complexOperation();

    let nested = new NestedGeneric<number>(456);
    let result2 = nested.processNested();

    let extended = new ExtendedContainer<string>("extended", 789);
    let value = extended.getValue();

    // Test new edge cases

    // Test nested generics
    let nestedList = new NestedList<number>(42);
    let nestedValue = nestedList.value;

    // Test multiple inheritance levels
    let topGeneric = new TopGeneric<string, number, boolean>("base", 123, true);
    let baseVal = topGeneric.baseValue;
    let middleVal = topGeneric.middleValue;
    let topVal = topGeneric.topValue;

    // Test constrained containers
    let constrained = new ConstrainedContainer<string>("test");
    let processed = constrained.processData();

    // Test generic collections
    let collection = new GenericCollection<number>(100);
    let collectionItem = collection.items;
    let collectionCount = collection.count;

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