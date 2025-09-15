class ClassA {
    id: number;
    constructor(id: number) {
        this.id = id;
        console.log("ClassA constructor - id:", this.id);
    }
    ~ClassA() {
        console.log("ClassA destructor - id:", this.id);
    }
}

class ClassB {
    name: string;
    constructor(name: string) {
        this.name = name;
        console.log("ClassB constructor - name:", this.name);
    }
    ~ClassB() {
        console.log("ClassB destructor - name:", this.name);
    }
}

function testDifferentDestructors() {
    console.log("=== Testing Different Destructors ===");
    let objA = new ClassA(1);
    let objB = new ClassB("test");
    console.log("Both objects created");
    // Destructors should be called in reverse order: objB, objA
}

testDifferentDestructors();