class TestClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("TestClass constructor - id:", id);
    }
    
    ~TestClass() {
        console.log("TestClass destructor");
        console.log("ID value:", this.id);
        let idStr = this.id.toString();
        console.log("ID:", idStr);
    }
}

function testDebugIdValue() {
    console.log("Creating TestClass instance");
    let obj = new TestClass(42);
    console.log("TestClass instance created");
}

testDebugIdValue();