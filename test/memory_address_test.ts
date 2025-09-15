class TestClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("Constructor: Stored id =", id);
        console.log("Constructor: this pointer =", this);
    }
    
    ~TestClass() {
        console.log("Destructor: this pointer =", this);
        console.log("Destructor: Reading id =", this.id);
    }
}

function testMemoryAddress() {
    console.log("Creating TestClass with id = 42");
    let obj = new TestClass(42);
    console.log("Object created successfully");
    console.log("Object pointer =", obj);
}

testMemoryAddress();