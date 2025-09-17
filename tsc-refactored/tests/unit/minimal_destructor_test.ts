// Minimal Destructor Test - Isolate the core issue
class TestClass {
    constructor() {
        _print("TestClass constructor");
    }

~

    TestClass() {
        _print("TestClass destructor");
    }
}

function testMinimalDestructor() {
    _print("Entering testMinimalDestructor");
    let obj = new TestClass();
    _print("Exiting testMinimalDestructor");
}

testMinimalDestructor();