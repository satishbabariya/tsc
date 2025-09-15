// Test to verify array length field initialization
class TestArray<T> {
    items: T[];
    
    constructor() {
        this.items = [];
        // Let's try to access the length directly to see if it's initialized
    }
    
    getLength(): number {
        return this.items.length;
    }
}

// Test constructor initialization
let testArray = new TestArray<number>();
_print("Array length after constructor:", testArray.getLength());