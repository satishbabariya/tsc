// Simple comprehensive memory leak test
// Focus on core ARC and RAII functionality

class MemoryResource {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        console.log(`MemoryResource ${this.id} (${this.name}) constructed.`);
    }
    
    ~MemoryResource() {
        console.log(`MemoryResource ${this.id} (${this.name}) destructed.`);
    }
}

// Test Case 1: Basic ARC functionality
function testBasicARC() {
    console.log("--- Test Case 1: Basic ARC ---");
    let res1 = new MemoryResource(1, "BasicResource1");
    let res2 = new MemoryResource(2, "BasicResource2");
    // res1 and res2 go out of scope, should be destructed by ARC
    console.log("End of Test Case 1 scope.");
}

// Test Case 2: Early return in a function
function testEarlyReturn() {
    console.log("--- Test Case 2: Early Return ---");
    let res = new MemoryResource(200, "EarlyReturnResource");
    if (true) {
        console.log("Early return path taken.");
        return; // res should be destructed here
    }
    console.log("This line should not be reached.");
}

// Test Case 3: Stress test (many allocations)
function testStressTest() {
    console.log("--- Test Case 3: Stress Test ---");
    // Simulate many allocations without loops
    let s1 = new MemoryResource(1000, "Stress1");
    let s2 = new MemoryResource(1001, "Stress2");
    let s3 = new MemoryResource(1002, "Stress3");
    let s4 = new MemoryResource(1003, "Stress4");
    let s5 = new MemoryResource(1004, "Stress5");
    console.log("Many resources created.");
    // All should be destructed at scope end
    console.log("End of Test Case 3 scope.");
}

function runSimpleMemoryTests() {
    console.log("--- Running Simple Memory Tests ---");
    testBasicARC();
    testEarlyReturn();
    testStressTest();
    console.log("--- All Simple Memory Tests Completed ---");
}

runSimpleMemoryTests();