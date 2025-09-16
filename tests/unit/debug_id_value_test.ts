class TestClass {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("TestClass constructor - id:", id);
    }

~

    TestClass() {
        _print("TestClass destructor");
        _print("ID value:", this.id);
        let idStr = this.id.toString();
        _print("ID:", idStr);
    }
}

function testDebugIdValue() {
    _print("Creating TestClass instance");
    let obj = new TestClass(42);
    _print("TestClass instance created");
}

testDebugIdValue();