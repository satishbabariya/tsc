// Comprehensive Memory Leak Analysis Test Suite
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

class Node {
    id: number;
    data: string;
    next: Node | null;
    
    constructor(id: number, data: string) {
        this.id = id;
        this.data = data;
        this.next = null;
        _print(`Node ${id} created with data: ${data}`);
    }
    
    ~Node() {
        _print(`Node ${this.id} destroyed`);
    }
    
    setNext(node: Node | null) {
        this.next = node;
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

// Test 3: Linked list (potential cycle)
function testLinkedList() {
    _print("=== Test 3: Linked List ===");
    
    let node1 = new Node(1, "First");
    let node2 = new Node(2, "Second");
    let node3 = new Node(3, "Third");
    
    node1.setNext(node2);
    node2.setNext(node3);
    
    _print("Linked list created, going out of scope...");
    // All nodes should be destroyed (no cycles in this case)
}

// Test 4: Multiple references to same object
function testMultipleReferences() {
    _print("=== Test 4: Multiple References ===");
    
    let resource = new Resource(100, "Shared Resource");
    let container1 = new Container("Container 1");
    let container2 = new Container("Container 2");
    
    container1.addResource(resource);
    container2.addResource(resource);
    
    _print("Multiple references created, going out of scope...");
    // Resource should be destroyed only once when all references are gone
}

// Test 5: Complex object graph
function testComplexGraph() {
    _print("=== Test 5: Complex Object Graph ===");
    
    let root = new Container("Root Container");
    
    // Create multiple levels of nesting
    for (let i = 0; i < 3; i++) {
        let subContainer = new Container(`Sub Container ${i}`);
        for (let j = 0; j < 2; j++) {
            let resource = new Resource(i * 10 + j, `Resource ${i}-${j}`);
            subContainer.addResource(resource);
        }
        root.addResource(subContainer);
    }
    
    _print("Complex object graph created, going out of scope...");
    // All objects should be destroyed in correct order
}

// Test 6: Stress test with many objects
function testStressTest() {
    _print("=== Test 6: Stress Test ===");
    
    let containers: Container[] = [];
    
    // Create many containers with resources
    for (let i = 0; i < 10; i++) {
        let container = new Container(`Stress Container ${i}`);
        for (let j = 0; j < 5; j++) {
            let resource = new Resource(i * 100 + j, `Stress Resource ${i}-${j}`);
            container.addResource(resource);
        }
        containers.push(container);
    }
    
    _print(`Created ${containers.length} containers with resources, going out of scope...`);
    // All objects should be destroyed
}

// Test 7: Early return scenarios
function testEarlyReturn() {
    _print("=== Test 7: Early Return Scenarios ===");
    
    let resource1 = new Resource(200, "Early Return Resource 1");
    
    if (true) {
        let resource2 = new Resource(201, "Early Return Resource 2");
        _print("Early return scenario - resources should be cleaned up");
        return; // Early return
    }
    
    let resource3 = new Resource(202, "Early Return Resource 3");
    _print("This should not be reached");
}

// Test 8: Exception-like scenarios (simulated with early returns)
function testExceptionScenarios() {
    _print("=== Test 8: Exception Scenarios ===");
    
    let resource1 = new Resource(300, "Exception Resource 1");
    
    try {
        let resource2 = new Resource(301, "Exception Resource 2");
        throw new Error("Simulated exception");
    } catch (error) {
        let resource3 = new Resource(302, "Exception Resource 3");
        _print("Exception caught, resources should be cleaned up");
    }
    
    _print("Exception scenario completed");
}

// Main test runner
function runMemoryLeakTests() {
    _print("Starting Comprehensive Memory Leak Analysis Tests");
    _print("================================================");
    
    testBasicARC();
    _print("");
    
    testRAIINested();
    _print("");
    
    testLinkedList();
    _print("");
    
    testMultipleReferences();
    _print("");
    
    testComplexGraph();
    _print("");
    
    testStressTest();
    _print("");
    
    testEarlyReturn();
    _print("");
    
    testExceptionScenarios();
    _print("");
    
    _print("All memory leak analysis tests completed");
    _print("========================================");
}

// Run the tests
runMemoryLeakTests();