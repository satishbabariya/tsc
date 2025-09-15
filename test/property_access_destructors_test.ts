// Test with destructors that access object properties
// This should work without segmentation fault

class PropertyDestructorClass1 {
    id: number;
    constructor(id: number) {
        this.id = id;
        _print("PropertyDestructorClass1 constructor - id:", this.id);
    }
    ~PropertyDestructorClass1() {
        _print("PropertyDestructorClass1 destructor - id:", this.id);
    }
}

class PropertyDestructorClass2 {
    name: string;
    constructor(name: string) {
        this.name = name;
        _print("PropertyDestructorClass2 constructor - name:", this.name);
    }
    ~PropertyDestructorClass2() {
        _print("PropertyDestructorClass2 destructor - name:", this.name);
    }
}

function testPropertyAccessDestructors() {
    _print("=== Testing Property Access Destructors ===");
    let obj1 = new PropertyDestructorClass1(1);
    let obj2 = new PropertyDestructorClass2("test");
    _print("Both objects created");
    _print("Expected destruction order: obj2, obj1");
}

testPropertyAccessDestructors();