// Test to isolate destructor calling mechanism
class TestClass1 {
    constructor() {
        console.log("TestClass1 constructor");
    }
    
    ~TestClass1() {
        console.log("TestClass1 destructor");
    }
}

class TestClass2 {
    constructor() {
        console.log("TestClass2 constructor");
    }
    
    ~TestClass2() {
        console.log("TestClass2 destructor");
    }
}

function testDestructorCalling() {
    console.log("=== Testing Destructor Calling ===");
    
    // Create objects with different destructors
    let obj1 = new TestClass1();
    let obj2 = new TestClass2();
    
    console.log("Both objects created");
}

testDestructorCalling();