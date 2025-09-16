class ClassA {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("ClassA constructor - id:", this.id);
    }

~

    ClassA() {
        _print("ClassA destructor - id:", this.id);
    }
}

class ClassB {
    name: string;

    constructor(name: string) {
        this.name = name;
        _print("ClassB constructor - name:", this.name);
    }

~

    ClassB() {
        _print("ClassB destructor - name:", this.name);
    }
}

function testDifferentDestructors() {
    _print("=== Testing Different Destructors ===");
    let objA = new ClassA(1);
    let objB = new ClassB("test");
    _print("Both objects created");
    // Destructors should be called in reverse order: objB, objA
}

testDifferentDestructors();