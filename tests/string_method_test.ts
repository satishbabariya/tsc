// Test string methods specifically
class BasicArrayOperations<T> {
    items: T[];
    
    constructor() {
        this.items = [];
    }
    
    addItem(item: T): void {
        // Simple implementation
    }
    
    getLengthAsString(): string {
        // This method involves string operations
        let length = this.items.length;
        return length.toString();
    }
}

// Create global instances
let numberOps = new BasicArrayOperations<number>();
let stringOps = new BasicArrayOperations<string>();

// Add items
numberOps.addItem(10);
numberOps.addItem(20);
stringOps.addItem("hello");
stringOps.addItem("world");

// Test string methods
let numberLengthStr = numberOps.getLengthAsString();
let stringLengthStr = stringOps.getLengthAsString();