// Simple test to isolate the crash issue
class SimpleClass {
    items: number[];
    
    constructor() {
        this.items = [] as number[];
    }
    
    addItem(item: number): void {
        this.items.push(item);
    }
}

// Global objects
let simpleObj: SimpleClass = new SimpleClass();
let simpleObj2: SimpleClass = new SimpleClass();

// Add some items
simpleObj.addItem(1);
simpleObj.addItem(2);
simpleObj2.addItem(3);