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

function testSimpleDestructorsNoProperties() {
    _print("=== Testing Simple Destructors (No Properties) ===");
    let obj1 = new SimpleClass1();
    let obj2 = new SimpleClass2();
    _print("Both objects created");
}

testSimpleDestructorsNoProperties();