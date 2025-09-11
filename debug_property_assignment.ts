// Test property assignment specifically
class Container<T> {
    value: T;
    other: T;
    
    constructor(value: T) {
        // Skip this assignment for now
    }
    
    testPropertyAssignment(param: T): void {
        this.other = param;  // This should work - assign param: T to property: T
    }
}

function main(): number {
    return 0;
}