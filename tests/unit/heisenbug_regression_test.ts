// Regression test for the Heisenbug: simple destructors calling _print
// This test should fail WITHOUT the delay loop workaround and pass WITH it

class SimpleDestructorClass1 {
    constructor() {
        _print("SimpleDestructorClass1 constructor");
    }

~

    SimpleDestructorClass1() {
        _print("SimpleDestructorClass1 destructor");
    }
}

class SimpleDestructorClass2 {
    constructor() {
        _print("SimpleDestructorClass2 constructor");
    }

~

    SimpleDestructorClass2() {
        _print("SimpleDestructorClass2 destructor");
    }
}

class SimpleDestructorClass3 {
    constructor() {
        _print("SimpleDestructorClass3 constructor");
    }

~

    SimpleDestructorClass3() {
        _print("SimpleDestructorClass3 destructor");
    }
}

function testHeisenbugRegression() {
    _print("=== Heisenbug Regression Test ===");
    _print("Creating multiple objects with simple destructors...");

    let obj1 = new SimpleDestructorClass1();
    let obj2 = new SimpleDestructorClass2();
    let obj3 = new SimpleDestructorClass3();

    _print("All objects created successfully");
    _print("Expected destruction order: obj3, obj2, obj1");
    _print("Test completed - destructors should be called in reverse order");
}

testHeisenbugRegression();