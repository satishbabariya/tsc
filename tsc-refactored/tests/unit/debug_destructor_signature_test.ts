// Test to debug destructor signature issues
class DebugClass {
    constructor() {
        _print("DebugClass constructor");
    }

~

    DebugClass() {
        _print("DebugClass destructor - about to return");
    }
}

function testDebugDestructor() {
    _print("=== Testing Debug Destructor ===");

    let obj = new DebugClass();
    _print("Object created");
    // Only one object, should be simpler to debug
}

testDebugDestructor();