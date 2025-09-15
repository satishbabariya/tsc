class TestClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        _print("Constructor: Stored id =", id);
        _print("Constructor: this pointer =", this);
    }
    
    ~TestClass() {
        _print("Destructor: this pointer =", this);
        _print("Destructor: Reading id =", this.id);
    }
}

function testMemoryAddress() {
    _print("Creating TestClass with id = 42");
    let obj = new TestClass(42);
    _print("Object created successfully");
    _print("Object pointer =", obj);
}

testMemoryAddress();