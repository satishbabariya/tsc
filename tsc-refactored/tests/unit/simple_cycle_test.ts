// Simple Cycle Detection Test
// This test focuses on basic cycle detection without complex decorators

class Parent {
    child: shared_ptr<Child>;

    constructor() {
    }

    setChild(c: shared_ptr<Child>) {
        this.child = c;
        c.parent = new shared_ptr<Parent>(this);  // Creates cycle
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

    parent.get().setChild(child);

    _print("Cycle test completed");
}

testCycleDetection();