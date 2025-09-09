// Minimal test for constructor resolution
class SimpleContainer<T> {
    data: T;
    
    constructor(data: T) {
        this.data = data;  // This should work now
    }
}

function main(): number {
    let container = new SimpleContainer<string>("hello");  // This fails
    return 0;
}