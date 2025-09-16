// Test to isolate destructor calling order issue
class FirstClass {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("FirstClass constructor - id:", this.id);
    }

~

    FirstClass() {
        _print("FirstClass destructor - id:", this.id);
    }
}

class SecondClass {
    name: string;

    constructor(name: string) {
        this.name = name;
        _print("SecondClass constructor - name:", this.name);
    }

~

    SecondClass() {
        _print("SecondClass destructor - name:", this.name);
    }
}

function testDestructorOrder() {
    _print("=== Testing Destructor Order ===");

    // Create objects in specific order
    let first = new FirstClass(1);
    let second = new SecondClass("test");

    _print("Both objects created");
    // Destructors should be called in reverse order: second, first
}

testDestructorOrder();