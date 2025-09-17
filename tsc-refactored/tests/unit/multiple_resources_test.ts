class TestResource1 {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("TestResource1 constructor");
    }

~

    TestResource1() {
        _print("TestResource1 destructor");
    }
}

class TestResource2 {
    name: string;

    constructor(name: string) {
        this.name = name;
        _print("TestResource2 constructor");
    }

~

    TestResource2() {
        _print("TestResource2 destructor");
    }
}

function testMultipleResources() {
    _print("=== Testing Multiple Resources ===");
    let resource1 = new TestResource1(1);
    let resource2 = new TestResource2("test");
    _print("Multiple resources created");
    // Destructors should be called in reverse order: resource2, resource1
}

testMultipleResources();