_print("Simple test");

class SimpleClass {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("Constructor called");
    }

~

    SimpleClass() {
        _print("Destructor called");
    }
}

function testSimple() {
    _print("Starting test");
    let obj = new SimpleClass(42);
    _print("Object created");
}

testSimple();