class TestClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("TestClass constructor");
    }
    
    ~TestClass() {
        console.log("TestClass destructor");
        // This should trigger the GEP error
        let idStr = this.id.toString();
        console.log("ID:", idStr);
    }
}

function testMinimalGEPError() {
    console.log("Creating TestClass instance");
    let obj = new TestClass(42);
    console.log("TestClass instance created");
}

testMinimalGEPError();