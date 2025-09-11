// Minimal test to debug type creation and identity

class SimpleContainer<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
}

class Wrapper<T> {
    container: SimpleContainer<T>;
    
    constructor(value: T) {
        // This should work but might have type identity issues
        this.container = new SimpleContainer<T>(value);
    }
}

function main(): number {
    let wrapper = new Wrapper<number>(42);
    return 0;
}