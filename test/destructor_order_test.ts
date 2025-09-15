// Test to isolate destructor calling order issue
class FirstClass {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("FirstClass constructor - id:", this.id);
    }
    
    ~FirstClass() {
        console.log("FirstClass destructor - id:", this.id);
    }
}

class SecondClass {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log("SecondClass constructor - name:", this.name);
    }
    
    ~SecondClass() {
        console.log("SecondClass destructor - name:", this.name);
    }
}

function testDestructorOrder() {
    console.log("=== Testing Destructor Order ===");
    
    // Create objects in specific order
    let first = new FirstClass(1);
    let second = new SecondClass("test");
    
    console.log("Both objects created");
    // Destructors should be called in reverse order: second, first
}

testDestructorOrder();