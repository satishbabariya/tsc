// Simple test without generics
class SimpleClass {
    value: number;
    
    constructor(val: number) {
        this.value = val;
    }
    
    getValue(): number {
        return this.value;
    }
}

let instance = new SimpleClass(42);
let result = instance.getValue();
_print("Simple class test result:", result);