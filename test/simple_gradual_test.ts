// Simple test to isolate the gradual complexity issue

class SimpleResource {
    constructor() {
        _print("SimpleResource constructor");
    }
    
    ~SimpleResource() {
        _print("SimpleResource destructor");
    }
}

function testSimpleResource() {
    _print("=== Testing Simple Resource ===");
    let resource = new SimpleResource();
    _print("Simple resource created");
}

function runSimpleTest() {
    _print("Starting simple test...");
    testSimpleResource();
    _print("Simple test completed");
}

runSimpleTest();