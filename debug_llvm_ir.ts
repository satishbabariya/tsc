// Minimal test to examine LLVM IR generation
class Container<T> {
    value: T;
    
    test(): T {
        let temp = this.value;  // This should create an alloca and store
        return temp;            // This should load the value
    }
}

function main(): number {
    return 0;
}