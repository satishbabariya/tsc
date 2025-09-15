class TestClass {
    id: number;
    constructor(id: number) {
        this.id = id;
        console.log("TestClass constructor - id:", this.id);
    }
    ~TestClass() {
        console.log("TestClass destructor - id:", this.id);
    }
}

function testStackCorruption() {
    console.log("=== Testing Stack Corruption ===");
    let obj1 = new TestClass(1);
    let obj2 = new TestClass(2);
    console.log("Both objects created");
    // Destructors should be called in reverse order: obj2, obj1
}

testStackCorruption();