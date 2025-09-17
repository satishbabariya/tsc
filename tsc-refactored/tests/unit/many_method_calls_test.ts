// Test many method calls to see if that causes the crash
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
        return "3";
    }

    getLengthPlusOne(): number {
        return this.getLength() + 1;
    }

    getLengthMinusOne(): number {
        return this.getLength() - 1;
    }
}

// Create global instances
let numberOps = new BasicArrayOperations<number>();
let stringOps = new BasicArrayOperations<string>();

// Many method calls
numberOps.addItem(10);
numberOps.addItem(20);
stringOps.addItem("hello");
stringOps.addItem("world");

let length1 = numberOps.getLength();
let length2 = stringOps.getLength();
let lengthStr1 = numberOps.getLengthAsString();
let lengthStr2 = stringOps.getLengthAsString();
let plusOne1 = numberOps.getLengthPlusOne();
let plusOne2 = stringOps.getLengthPlusOne();
let minusOne1 = numberOps.getLengthMinusOne();
let minusOne2 = stringOps.getLengthMinusOne();