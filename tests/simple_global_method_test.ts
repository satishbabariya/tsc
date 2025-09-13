// Simple test for global method calls
class BasicArrayOperations<T> {
    items: T[];
    
    constructor() {
        this.items = [];
    }
    
    addItem(item: T): void {
        // Simple implementation
    }
    
    getLength(): number {
        return this.items.length;
    }
}

// Create a global instance
let numberOps = new BasicArrayOperations<number>();

// Call a method on the global instance
numberOps.addItem(42);
let length = numberOps.getLength();