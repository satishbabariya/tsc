// MRE: Minimal Reproducible Example for global initialization timing issue
class SimpleClass {
    value: number;

    constructor() {
        this.value = 42; // This initialization might be skipped in global context
    }

    getValue(): number {
        return this.value; // This might access uninitialized memory
    }
}

// Create global instance - constructor might be deferred/skipped
let globalObj = new SimpleClass();

// Call method on global instance - might access uninitialized memory
let result = globalObj.getValue();