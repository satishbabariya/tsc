class TestClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
    }
    
    ~TestClass() {
        console.log("Destructor called");
    }
}

function testSimpleDestructor() {
    console.log("Creating TestClass instance");
    let obj = new TestClass(42);
    console.log("TestClass instance created");
}

testSimpleDestructor();