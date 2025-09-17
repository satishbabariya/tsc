class TestClass {
    id: number;

    constructor(id: number) {
        this.id = id;
    }

~

    TestClass() {
        _print("Destructor called");
    }
}

function testSimpleDestructor() {
    _print("Creating TestClass instance");
    let obj = new TestClass(42);
    _print("TestClass instance created");
}

testSimpleDestructor();