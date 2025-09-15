console.log("Simple test");

class SimpleClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("Constructor called");
    }
    
    ~SimpleClass() {
        console.log("Destructor called");
    }
}

function testSimple() {
    console.log("Starting test");
    let obj = new SimpleClass(42);
    console.log("Object created");
}

testSimple();