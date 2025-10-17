// Minimal test to reproduce generic type segfault
class SimpleGeneric<T> {
    value: T;
    
    constructor(val: T) {
        this.value = val;
    }
    
    getValue(): T {
        return this.value;
    }
}

let instance = new SimpleGeneric<number>(42);
let result = instance.getValue();
_print("Generic test result:", result);