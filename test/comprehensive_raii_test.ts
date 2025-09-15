// Comprehensive RAII test covering multiple scenarios

class ResourceA {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log("ResourceA constructor: " + name);
    }
    
    ~ResourceA() {
        console.log("ResourceA destructor: " + this.name);
    }
}

class ResourceB {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("ResourceB constructor: " + id.toString());
    }
    
    ~ResourceB() {
        console.log("ResourceB destructor: " + this.id.toString());
    }
}

function testNestedScopes() {
    console.log("=== Testing Nested Scopes ===");
    let outer = new ResourceA("outer");
    
    if (true) {
        let inner = new ResourceB(42);
        console.log("Inside nested scope");
        // inner should be destroyed here
    }
    
    console.log("Back in outer scope");
    // outer should be destroyed here
}

function testMultipleObjects() {
    console.log("=== Testing Multiple Objects ===");
    let res1 = new ResourceA("first");
    let res2 = new ResourceB(100);
    let res3 = new ResourceA("second");
    console.log("All objects created");
    // All objects should be destroyed in reverse order: res3, res2, res1
}

function testEarlyReturn() {
    console.log("=== Testing Early Return ===");
    let resource = new ResourceA("early_return");
    
    if (true) {
        console.log("Taking early return path");
        return; // resource should still be destroyed
    }
    
    console.log("This should not be printed");
}

function runComprehensiveRAIITests() {
    console.log("Starting comprehensive RAII tests...");
    
    testNestedScopes();
    console.log("");
    
    testMultipleObjects();
    console.log("");
    
    testEarlyReturn();
    console.log("");
    
    console.log("Comprehensive RAII tests completed");
}

runComprehensiveRAIITests();