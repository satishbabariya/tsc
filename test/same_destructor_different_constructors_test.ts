class TestClass {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        _print("TestClass constructor - id:", this.id, "name:", this.name);
    }
    
    ~TestClass() {
        _print("TestClass destructor - id:", this.id, "name:", this.name);
    }
}

function testSameDestructorDifferentConstructors() {
    _print("Creating objects with same destructor");
    let obj1 = new TestClass(1, "first");
    let obj2 = new TestClass(2, "second");
    _print("Both objects created");
}

testSameDestructorDifferentConstructors();