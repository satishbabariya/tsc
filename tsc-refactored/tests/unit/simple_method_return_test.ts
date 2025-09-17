// Test method calls that return values
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

// Create a global instance
let numberOps = new BasicArrayOperations<number>();

// Call a method that returns a value
let length = numberOps.getLength();
let lengthStr = numberOps.getLengthAsString();