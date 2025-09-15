// Test to debug destructor signature issues
class DebugClass {
    constructor() {
        console.log("DebugClass constructor");
    }
    
    ~DebugClass() {
        console.log("DebugClass destructor - about to return");
    }
}

function testDebugDestructor() {
    console.log("=== Testing Debug Destructor ===");
    
    let obj = new DebugClass();
    console.log("Object created");
    // Only one object, should be simpler to debug
}

testDebugDestructor();