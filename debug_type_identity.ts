// Test type parameter identity
class Container<T> {
    value: T;
    
    constructor(value: T) {
        // Skip assignment for now
    }
    
    testTypeIdentity(param: T): T {
        return param;  // This should work - same T to same T
    }
}

function main(): number {
    return 0;
}