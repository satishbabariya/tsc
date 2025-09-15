// Comprehensive RAII test covering multiple scenarios

class ResourceA {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        _print("ResourceA constructor: " + name);
    }
    
    ~ResourceA() {
        _print("ResourceA destructor: " + this.name);
    }
}

class ResourceB {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        _print("ResourceB constructor: " + id.toString());
    }
    
    ~ResourceB() {
        _print("ResourceB destructor: " + this.id.toString());
    }
}

function testNestedScopes() {
    _print("=== Testing Nested Scopes ===");
    let outer = new ResourceA("outer");
    
    if (true) {
        let inner = new ResourceB(42);
        _print("Inside nested scope");
        // inner should be destroyed here
    }
    
    _print("Back in outer scope");
    // outer should be destroyed here
}

function testMultipleObjects() {
    _print("=== Testing Multiple Objects ===");
    let res1 = new ResourceA("first");
    let res2 = new ResourceB(100);
    let res3 = new ResourceA("second");
    _print("All objects created");
    // All objects should be destroyed in reverse order: res3, res2, res1
}

function testEarlyReturn() {
    _print("=== Testing Early Return ===");
    let resource = new ResourceA("early_return");
    
    if (true) {
        _print("Taking early return path");
        return; // resource should still be destroyed
    }
    
    _print("This should not be printed");
}

function runComprehensiveRAIITests() {
    _print("Starting comprehensive RAII tests...");
    
    testNestedScopes();
    _print("");
    
    testMultipleObjects();
    _print("");
    
    testEarlyReturn();
    _print("");
    
    _print("Comprehensive RAII tests completed");
}

runComprehensiveRAIITests();