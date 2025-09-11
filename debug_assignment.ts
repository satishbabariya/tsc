// Test assignment parsing
class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;  // This should be AssignmentExpression
    }
}

function main(): number {
    return 0;
}