class SimpleClass1 {
    constructor() {
        console.log("SimpleClass1 constructor");
    }
    ~SimpleClass1() {
        console.log("SimpleClass1 destructor");
    }
}

class SimpleClass2 {
    constructor() {
        console.log("SimpleClass2 constructor");
    }
    ~SimpleClass2() {
        console.log("SimpleClass2 destructor");
    }
}

function testSimpleDestructorsNoProperties() {
    console.log("=== Testing Simple Destructors (No Properties) ===");
    let obj1 = new SimpleClass1();
    let obj2 = new SimpleClass2();
    console.log("Both objects created");
}

testSimpleDestructorsNoProperties();