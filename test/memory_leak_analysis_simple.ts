// Simple Memory Leak Analysis Test Suite
// This test suite validates ARC and RAII functionality to prevent memory leaks

class Resource {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        console.log(`Resource ${id} (${name}) created`);
    }
    
    ~Resource() {
        console.log(`Resource ${this.id} (${this.name}) destroyed`);
    }
}

class Container {
    resources: Resource[];
    name: string;
    
    constructor(name: string) {
        this.name = name;
        this.resources = [];
        console.log(`Container ${name} created`);
    }
    
    ~Container() {
        console.log(`Container ${this.name} destroyed with ${this.resources.length} resources`);
        // Resources will be automatically destroyed when container is destroyed
    }
    
    addResource(resource: Resource) {
        this.resources.push(resource);
        console.log(`Added resource ${resource.id} to container ${this.name}`);
    }
}

// Test 1: Basic ARC functionality
function testBasicARC() {
    console.log("=== Test 1: Basic ARC Functionality ===");
    
    let resource1 = new Resource(1, "Test Resource 1");
    let resource2 = new Resource(2, "Test Resource 2");
    
    console.log("Resources created, going out of scope...");
    // Resources should be automatically destroyed when going out of scope
}

// Test 2: RAII with nested objects
function testRAIINested() {
    console.log("=== Test 2: RAII with Nested Objects ===");
    
    let container = new Container("Test Container");
    let resource1 = new Resource(10, "Nested Resource 1");
    let resource2 = new Resource(11, "Nested Resource 2");
    
    container.addResource(resource1);
    container.addResource(resource2);
    
    console.log("Container with resources created, going out of scope...");
    // Container and all its resources should be destroyed
}

// Test 3: Multiple references to same object
function testMultipleReferences() {
    console.log("=== Test 3: Multiple References ===");
    
    let resource = new Resource(100, "Shared Resource");
    let container1 = new Container("Container 1");
    let container2 = new Container("Container 2");
    
    container1.addResource(resource);
    container2.addResource(resource);
    
    console.log("Multiple references created, going out of scope...");
    // Resource should be destroyed only once when all references are gone
}

// Test 4: Early return scenarios
function testEarlyReturn() {
    console.log("=== Test 4: Early Return Scenarios ===");
    
    let resource1 = new Resource(200, "Early Return Resource 1");
    
    if (true) {
        let resource2 = new Resource(201, "Early Return Resource 2");
        console.log("Early return scenario - resources should be cleaned up");
        return; // Early return
    }
    
    let resource3 = new Resource(202, "Early Return Resource 3");
    console.log("This should not be reached");
}

// Test 5: Stress test with many objects
function testStressTest() {
    console.log("=== Test 5: Stress Test ===");
    
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
    
    console.log("Created multiple containers with resources, going out of scope...");
    // All objects should be destroyed
}

// Main test runner
function runMemoryLeakTests() {
    console.log("Starting Simple Memory Leak Analysis Tests");
    console.log("==========================================");
    
    testBasicARC();
    console.log("");
    
    testRAIINested();
    console.log("");
    
    testMultipleReferences();
    console.log("");
    
    testEarlyReturn();
    console.log("");
    
    testStressTest();
    console.log("");
    
    console.log("All memory leak analysis tests completed");
    console.log("========================================");
}

// Run the tests
runMemoryLeakTests();