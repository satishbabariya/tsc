class TestClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("Constructor: Stored id =", id);
    }
    
    ~TestClass() {
        console.log("Destructor: Reading id =", this.id);
    }
}

function testMemoryLayout() {
    console.log("Creating TestClass with id = 42");
    let obj = new TestClass(42);
    console.log("Object created successfully");
}

testMemoryLayout();