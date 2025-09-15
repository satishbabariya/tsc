// Simple Memory Leak Analysis Test Suite
// This test suite validates ARC and RAII functionality to prevent memory leaks

class Resource {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        _print(`Resource ${id} (${name}) created`);
    }
    
    ~Resource() {
        _print(`Resource ${this.id} (${this.name}) destroyed`);
    }
}

class Container {
    resources: Resource[];
    name: string;
    
    constructor(name: string) {
        this.name = name;
        this.resources = [];
        _print(`Container ${name} created`);
    }
    
    ~Container() {
        _print(`Container ${this.name} destroyed with ${this.resources.length} resources`);
        // Resources will be automatically destroyed when container is destroyed
    }
    
    addResource(resource: Resource) {
        this.resources.push(resource);
        _print(`Added resource ${resource.id} to container ${this.name}`);
    }
}

// Test 1: Basic ARC functionality
function testBasicARC() {
    _print("=== Test 1: Basic ARC Functionality ===");
    
    let resource1 = new Resource(1, "Test Resource 1");
    let resource2 = new Resource(2, "Test Resource 2");
    
    _print("Resources created, going out of scope...");
    // Resources should be automatically destroyed when going out of scope
}

// Test 2: RAII with nested objects
function testRAIINested() {
    _print("=== Test 2: RAII with Nested Objects ===");
    
    let container = new Container("Test Container");
    let resource1 = new Resource(10, "Nested Resource 1");
    let resource2 = new Resource(11, "Nested Resource 2");
    
    container.addResource(resource1);
    container.addResource(resource2);
    
    _print("Container with resources created, going out of scope...");
    // Container and all its resources should be destroyed
}

// Test 3: Multiple references to same object
function testMultipleReferences() {
    _print("=== Test 3: Multiple References ===");
    
    let resource = new Resource(100, "Shared Resource");
    let container1 = new Container("Container 1");
    let container2 = new Container("Container 2");
    
    container1.addResource(resource);
    container2.addResource(resource);
    
    _print("Multiple references created, going out of scope...");
    // Resource should be destroyed only once when all references are gone
}

// Test 4: Early return scenarios
function testEarlyReturn() {
    _print("=== Test 4: Early Return Scenarios ===");
    
    let resource1 = new Resource(200, "Early Return Resource 1");
    
    if (true) {
        let resource2 = new Resource(201, "Early Return Resource 2");
        _print("Early return scenario - resources should be cleaned up");
        return; // Early return
    }
    
    let resource3 = new Resource(202, "Early Return Resource 3");
    _print("This should not be reached");
}

// Test 5: Stress test with many objects
function testStressTest() {
    _print("=== Test 5: Stress Test ===");
    
    let container1 = new Container("Stress Container 1");
    let resource1 = new Resource(1001, "Stress Resource 1-1");
    let resource2 = new Resource(1002, "Stress Resource 1-2");
    let resource3 = new Resource(1003, "Stress Resource 1-3");
    let resource4 = new Resource(1004, "Stress Resource 1-4");
    let resource5 = new Resource(1005, "Stress Resource 1-5");
    
    container1.addResource(resource1);
    container1.addResource(resource2);
    container1.addResource(resource3);
    container1.addResource(resource4);
    container1.addResource(resource5);
    
    let container2 = new Container("Stress Container 2");
    let resource6 = new Resource(2001, "Stress Resource 2-1");
    let resource7 = new Resource(2002, "Stress Resource 2-2");
    let resource8 = new Resource(2003, "Stress Resource 2-3");
    let resource9 = new Resource(2004, "Stress Resource 2-4");
    let resource10 = new Resource(2005, "Stress Resource 2-5");
    
    container2.addResource(resource6);
    container2.addResource(resource7);
    container2.addResource(resource8);
    container2.addResource(resource9);
    container2.addResource(resource10);
    
    _print("Created multiple containers with resources, going out of scope...");
    // All objects should be destroyed
}

// Main test runner
function runMemoryLeakTests() {
    _print("Starting Simple Memory Leak Analysis Tests");
    _print("==========================================");
    
    testBasicARC();
    _print("");
    
    testRAIINested();
    _print("");
    
    testMultipleReferences();
    _print("");
    
    testEarlyReturn();
    _print("");
    
    testStressTest();
    _print("");
    
    _print("All memory leak analysis tests completed");
    _print("========================================");
}

// Run the tests
runMemoryLeakTests();