class TestClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        _print("TestClass constructor");
    }
    
    ~TestClass() {
        _print("TestClass destructor");
        // This should trigger the GEP error
        let idStr = this.id.toString();
        _print("ID:", idStr);
    }
}

function testMinimalGEPError() {
    _print("Creating TestClass instance");
    let obj = new TestClass(42);
    _print("TestClass instance created");
}

testMinimalGEPError();