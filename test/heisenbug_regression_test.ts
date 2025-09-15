// Regression test for the Heisenbug: simple destructors calling console.log
// This test should fail WITHOUT the delay loop workaround and pass WITH it

class SimpleDestructorClass1 {
    constructor() {
        console.log("SimpleDestructorClass1 constructor");
    }
    ~SimpleDestructorClass1() {
        console.log("SimpleDestructorClass1 destructor");
    }
}

class SimpleDestructorClass2 {
    constructor() {
        console.log("SimpleDestructorClass2 constructor");
    }
    ~SimpleDestructorClass2() {
        console.log("SimpleDestructorClass2 destructor");
    }
}

class SimpleDestructorClass3 {
    constructor() {
        console.log("SimpleDestructorClass3 constructor");
    }
    ~SimpleDestructorClass3() {
        console.log("SimpleDestructorClass3 destructor");
    }
}

function testHeisenbugRegression() {
    console.log("=== Heisenbug Regression Test ===");
    console.log("Creating multiple objects with simple destructors...");
    
    let obj1 = new SimpleDestructorClass1();
    let obj2 = new SimpleDestructorClass2();
    let obj3 = new SimpleDestructorClass3();
    
    console.log("All objects created successfully");
    console.log("Expected destruction order: obj3, obj2, obj1");
    console.log("Test completed - destructors should be called in reverse order");
}

testHeisenbugRegression();