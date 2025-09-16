class TestClass {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("Constructor: Stored id =", id);
    }

~

    TestClass() {
        _print("Destructor: Reading id =", this.id);
    }
}

function testMemoryLayout() {
    _print("Creating TestClass with id = 42");
    let obj = new TestClass(42);
    _print("Object created successfully");
}

testMemoryLayout();