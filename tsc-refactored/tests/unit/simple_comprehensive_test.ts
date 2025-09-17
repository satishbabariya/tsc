// Simple comprehensive memory leak test
// Focus on core ARC and RAII functionality

class MemoryResource {
    id: number;
    name: string;

    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        _print(`MemoryResource ${this.id} (${this.name}) constructed.`);
    }

~

    MemoryResource() {
        _print(`MemoryResource ${this.id} (${this.name}) destructed.`);
    }
}

// Test Case 1: Basic ARC functionality
function testBasicARC() {
    _print("--- Test Case 1: Basic ARC ---");
    let res1 = new MemoryResource(1, "BasicResource1");
    let res2 = new MemoryResource(2, "BasicResource2");
    // res1 and res2 go out of scope, should be destructed by ARC
    _print("End of Test Case 1 scope.");
}

// Test Case 2: Early return in a function
function testEarlyReturn() {
    _print("--- Test Case 2: Early Return ---");
    let res = new MemoryResource(200, "EarlyReturnResource");
    if (true) {
        _print("Early return path taken.");
        return; // res should be destructed here
    }
    _print("This line should not be reached.");
}

// Test Case 3: Stress test (many allocations)
function testStressTest() {
    _print("--- Test Case 3: Stress Test ---");
    // Simulate many allocations without loops
    let s1 = new MemoryResource(1000, "Stress1");
    let s2 = new MemoryResource(1001, "Stress2");
    let s3 = new MemoryResource(1002, "Stress3");
    let s4 = new MemoryResource(1003, "Stress4");
    let s5 = new MemoryResource(1004, "Stress5");
    _print("Many resources created.");
    // All should be destructed at scope end
    _print("End of Test Case 3 scope.");
}

function runSimpleMemoryTests() {
    _print("--- Running Simple Memory Tests ---");
    testBasicARC();
    testEarlyReturn();
    testStressTest();
    _print("--- All Simple Memory Tests Completed ---");
}

runSimpleMemoryTests();