// Minimal Destructor Test - Isolate the core issue
class TestClass {
    constructor() {
        console.log("TestClass constructor");
    }

    ~TestClass() {
        console.log("TestClass destructor");
    }
}

function testMinimalDestructor() {
    console.log("Entering testMinimalDestructor");
    let obj = new TestClass();
    console.log("Exiting testMinimalDestructor");
}

testMinimalDestructor();