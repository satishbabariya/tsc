// Comprehensive Memory Leak Analysis Test Suite
// This test validates ARC and RAII functionality for memory leak prevention

class MemoryResource {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        console.log(`MemoryResource ${id} (${name}) created`);
    }
    
    ~MemoryResource() {
        console.log(`MemoryResource ${this.id} (${this.name}) destroyed`);
    }
}

class ResourceManager {
    resources: MemoryResource[];
    name: string;
    
    constructor(name: string) {
        this.name = name;
        this.resources = [];
        console.log(`ResourceManager ${name} created`);
    }
    
    ~ResourceManager() {
        console.log(`ResourceManager ${this.name} destroyed with ${this.resources.length} resources`);
    }
    
    addResource(resource: MemoryResource) {
        this.resources.push(resource);
        console.log(`Added resource ${resource.id} to manager ${this.name}`);
    }
}

// Test 1: Basic ARC functionality
function testBasicARC() {
    console.log("=== Test 1: Basic ARC Functionality ===");
    
    let resource1 = new MemoryResource(1, "Basic Resource 1");
    let resource2 = new MemoryResource(2, "Basic Resource 2");
    
    console.log("Basic resources created, going out of scope...");
    // Resources should be automatically destroyed when going out of scope
}

// Test 2: RAII with nested objects
function testRAIINested() {
    console.log("=== Test 2: RAII with Nested Objects ===");
    
    let manager = new ResourceManager("Test Manager");
    let resource1 = new MemoryResource(10, "Nested Resource 1");
    let resource2 = new MemoryResource(11, "Nested Resource 2");
    
    manager.addResource(resource1);
    manager.addResource(resource2);
    
    console.log("Nested objects created, going out of scope...");
    // Manager and all its resources should be destroyed
}

// Test 3: Multiple references to same object
function testMultipleReferences() {
    console.log("=== Test 3: Multiple References ===");
    
    let resource = new MemoryResource(100, "Shared Resource");
    let manager1 = new ResourceManager("Manager 1");
    let manager2 = new ResourceManager("Manager 2");
    
    manager1.addResource(resource);
    manager2.addResource(resource);
    
    console.log("Multiple references created, going out of scope...");
    // Resource should be destroyed only once when all references are gone
}

// Test 4: Early return scenarios
function testEarlyReturn() {
    console.log("=== Test 4: Early Return Scenarios ===");
    
    let resource1 = new MemoryResource(200, "Early Return Resource 1");
    
    if (true) {
        let resource2 = new MemoryResource(201, "Early Return Resource 2");
        console.log("Early return scenario - resources should be cleaned up");
        return; // Early return
    }
    
    let resource3 = new MemoryResource(202, "Early Return Resource 3");
    console.log("This should not be reached");
}

// Test 5: Stress test with many objects
function testStressTest() {
    console.log("=== Test 5: Stress Test ===");
    
    let manager1 = new ResourceManager("Stress Manager 1");
    let resource1 = new MemoryResource(1001, "Stress Resource 1-1");
    let resource2 = new MemoryResource(1002, "Stress Resource 1-2");
    let resource3 = new MemoryResource(1003, "Stress Resource 1-3");
    let resource4 = new MemoryResource(1004, "Stress Resource 1-4");
    let resource5 = new MemoryResource(1005, "Stress Resource 1-5");
    
    manager1.addResource(resource1);
    manager1.addResource(resource2);
    manager1.addResource(resource3);
    manager1.addResource(resource4);
    manager1.addResource(resource5);
    
    let manager2 = new ResourceManager("Stress Manager 2");
    let resource6 = new MemoryResource(2001, "Stress Resource 2-1");
    let resource7 = new MemoryResource(2002, "Stress Resource 2-2");
    let resource8 = new MemoryResource(2003, "Stress Resource 2-3");
    let resource9 = new MemoryResource(2004, "Stress Resource 2-4");
    let resource10 = new MemoryResource(2005, "Stress Resource 2-5");
    
    manager2.addResource(resource6);
    manager2.addResource(resource7);
    manager2.addResource(resource8);
    manager2.addResource(resource9);
    manager2.addResource(resource10);
    
    console.log("Multiple managers with resources created, going out of scope...");
    // All objects should be destroyed
}

// Main test runner
function runComprehensiveMemoryTests() {
    console.log("Starting Comprehensive Memory Leak Analysis Tests");
    console.log("================================================");
    
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
    
    console.log("All comprehensive memory leak analysis tests completed");
    console.log("=====================================================");
}

// Run the tests
runComprehensiveMemoryTests();