// Test with array operations to isolate the crash
class ArrayClass {
    items: number[];
    
    constructor() {
        this.items = [];
    }
    
    addItem(item: number): void {
        this.items.push(item);
    }
}

// Global objects
let arrayObj: ArrayClass = new ArrayClass();

// Add one item
arrayObj.addItem(42);