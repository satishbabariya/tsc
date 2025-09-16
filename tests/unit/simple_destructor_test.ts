// Test with simple destructors (no property access)
class SimpleClass1 {
    constructor() {
        _print("SimpleClass1 constructor");
    }

~

    SimpleClass1() {
        _print("SimpleClass1 destructor");
    }
}

class SimpleClass2 {
    constructor() {
        _print("SimpleClass2 constructor");
    }

~

    SimpleClass2() {
        _print("SimpleClass2 destructor");
    }
}

function testSimpleDestructors() {
    _print("=== Testing Simple Destructors ===");

    let obj1 = new SimpleClass1();
    let obj2 = new SimpleClass2();

    _print("Both objects created");
}

testSimpleDestructors();