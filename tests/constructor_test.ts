// Simple test to verify constructor initialization
class BasicArrayOperations<T> {
    items: T[];
    
    constructor() {
        this.items = [];
    }
    
    getLength(): number {
        return this.items.length;
    }
}

// Test constructor initialization
let testArray = new BasicArrayOperations<number>();
console.log("Constructor test - array length:", testArray.getLength());