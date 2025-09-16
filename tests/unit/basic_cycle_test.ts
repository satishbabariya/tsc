// Basic Cycle Detection Test
// This test focuses on basic cycle detection without method calls

class Parent {
    child: shared_ptr<Child>;

    constructor() {
    }
}

class Child {
    parent: shared_ptr<Parent>;  // Strong reference creates cycle

    constructor() {
    }
}

// Test function
function testCycleDetection() {
    _print("Testing cycle detection...");

    let parent = new shared_ptr<Parent>(new Parent());
    let child = new shared_ptr<Child>(new Child());

    _print("Cycle test completed");
}

testCycleDetection();