// Test with destructors that access object properties
// This should work without segmentation fault

class PropertyDestructorClass1 {
    id: number;
    constructor(id: number) {
        this.id = id;
        console.log("PropertyDestructorClass1 constructor - id:", this.id);
    }
    ~PropertyDestructorClass1() {
        console.log("PropertyDestructorClass1 destructor - id:", this.id);
    }
}

class PropertyDestructorClass2 {
    name: string;
    constructor(name: string) {
        this.name = name;
        console.log("PropertyDestructorClass2 constructor - name:", this.name);
    }
    ~PropertyDestructorClass2() {
        console.log("PropertyDestructorClass2 destructor - name:", this.name);
    }
}

function testPropertyAccessDestructors() {
    console.log("=== Testing Property Access Destructors ===");
    let obj1 = new PropertyDestructorClass1(1);
    let obj2 = new PropertyDestructorClass2("test");
    console.log("Both objects created");
    console.log("Expected destruction order: obj2, obj1");
}

testPropertyAccessDestructors();