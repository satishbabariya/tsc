class TestClass1 {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("TestClass1 constructor");
    }
    
    ~TestClass1() {
        console.log("TestClass1 destructor");
    }
}

class TestClass2 {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log("TestClass2 constructor");
    }
    
    ~TestClass2() {
        console.log("TestClass2 destructor");
    }
}

function testSameDestructor() {
    console.log("Creating objects with different destructors");
    let obj1 = new TestClass1(1);
    let obj2 = new TestClass2("test");
    console.log("Both objects created");
}

testSameDestructor();