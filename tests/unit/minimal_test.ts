// Minimal test to isolate the crash issue
class MinimalClass {
    value: number;
    
    constructor() {
        this.value = 42;
    }
    
    getValue(): number {
        return this.value;
    }
}

// Global objects
let obj1: MinimalClass = new MinimalClass();
let obj2: MinimalClass = new MinimalClass();

// Call methods
let val1: number = obj1.getValue();
let val2: number = obj2.getValue();