// Test multiple global variables with method calls
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
    
    getLengthAsString(): string {
        return "3"; // Return a simple string
    }
}

// Create multiple global instances
let numberOps = new BasicArrayOperations<number>();
let stringOps = new BasicArrayOperations<string>();

// Call methods on different instances
numberOps.addItem(42);
stringOps.addItem("hello");

let numberLength = numberOps.getLength();
let stringLength = stringOps.getLength();