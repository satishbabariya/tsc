class TestResource1 {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("TestResource1 constructor");
    }
    
    ~TestResource1() {
        console.log("TestResource1 destructor");
    }
}

class TestResource2 {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log("TestResource2 constructor");
    }
    
    ~TestResource2() {
        console.log("TestResource2 destructor");
    }
}

function testMultipleResources() {
    console.log("=== Testing Multiple Resources ===");
    let resource1 = new TestResource1(1);
    let resource2 = new TestResource2("test");
    console.log("Multiple resources created");
    // Destructors should be called in reverse order: resource2, resource1
}

testMultipleResources();