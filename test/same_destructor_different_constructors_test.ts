class TestClass {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        console.log("TestClass constructor - id:", this.id, "name:", this.name);
    }
    
    ~TestClass() {
        console.log("TestClass destructor - id:", this.id, "name:", this.name);
    }
}

function testSameDestructorDifferentConstructors() {
    console.log("Creating objects with same destructor");
    let obj1 = new TestClass(1, "first");
    let obj2 = new TestClass(2, "second");
    console.log("Both objects created");
}

testSameDestructorDifferentConstructors();