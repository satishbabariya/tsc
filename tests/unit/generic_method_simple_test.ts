// Simple test for generic method resolution
class SimpleGeneric<T> {
    value: T;

    constructor(val: T) {
        this.value = val;
    }

    getValue(): T {
        return this.value;
    }
}

// Test basic generic method calls
let instance = new SimpleGeneric<number>(42);
let result = instance.getValue();
_print("Generic test result:", result);

// Test with different type
let stringInstance = new SimpleGeneric<string>("Hello");
let stringResult = stringInstance.getValue();
_print("String result:", stringResult);