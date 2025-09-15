// Basic Cycle Detection Test
// This test focuses on basic cycle detection without method calls

class Parent {
    child: shared_ptr<Child>;
    
    constructor() {}
}

class Child {
    parent: shared_ptr<Parent>;  // Strong reference creates cycle
    
    constructor() {}
}

// Test function
function testCycleDetection() {
    console.log("Testing cycle detection...");
    
    let parent = new shared_ptr<Parent>(new Parent());
    let child = new shared_ptr<Child>(new Child());
    
    console.log("Cycle test completed");
}

testCycleDetection();