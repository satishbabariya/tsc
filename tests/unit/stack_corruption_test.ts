class TestClass {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("TestClass constructor - id:", this.id);
    }

~

    TestClass() {
        _print("TestClass destructor - id:", this.id);
    }
}

function testStackCorruption() {
    _print("=== Testing Stack Corruption ===");
    let obj1 = new TestClass(1);
    let obj2 = new TestClass(2);
    _print("Both objects created");
    // Destructors should be called in reverse order: obj2, obj1
}

testStackCorruption();