// Test the complex methods that might be causing the crash
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

    getFirstItem(): T {
        // This might be causing issues
        return this.items[0];
    }

    getLastItem(): T {
        // This might be causing issues
        return this.items[this.items.length - 1];
    }

    getSecondToLast(): T {
        // This might be causing issues
        return this.items[this.items.length - 2];
    }
}

// Create global instances
let numberOps = new BasicArrayOperations<number>();
let stringOps = new BasicArrayOperations<string>();

// Add items
numberOps.addItem(10);
numberOps.addItem(20);
numberOps.addItem(30);

stringOps.addItem("hello");
stringOps.addItem("world");
stringOps.addItem("test");

// Test the complex methods
let firstItem = numberOps.getFirstItem();
let lastItem = numberOps.getLastItem();
let secondToLast = numberOps.getSecondToLast();