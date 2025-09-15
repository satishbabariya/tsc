// Minimal reproduction case for the Heisenbug
// This test reproduces the segmentation fault when different destructor functions are called

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

function testHeisenbug() {
    console.log("=== Testing Heisenbug Reproduction ===");
    
    // Create objects with different destructor functions
    let objA = new ClassA(1);
    let objB = new ClassB("test");
    
    console.log("Both objects created successfully");
}

testHeisenbug();