// Test to isolate destructor calling mechanism
class TestClass1 {
    constructor() {
        _print("TestClass1 constructor");
    }
    
    ~TestClass1() {
        _print("TestClass1 destructor");
    }
}

class TestClass2 {
    constructor() {
        _print("TestClass2 constructor");
    }
    
    ~TestClass2() {
        _print("TestClass2 destructor");
    }
}

function testDestructorCalling() {
    _print("=== Testing Destructor Calling ===");
    
    // Create objects with different destructors
    let obj1 = new TestClass1();
    let obj2 = new TestClass2();
    
    _print("Both objects created");
}

testDestructorCalling();