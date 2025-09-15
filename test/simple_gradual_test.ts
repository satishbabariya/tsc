// Simple test to isolate the gradual complexity issue

class SimpleResource {
    constructor() {
        console.log("SimpleResource constructor");
    }
    
    ~SimpleResource() {
        console.log("SimpleResource destructor");
    }
}

function testSimpleResource() {
    console.log("=== Testing Simple Resource ===");
    let resource = new SimpleResource();
    console.log("Simple resource created");
}

function runSimpleTest() {
    console.log("Starting simple test...");
    testSimpleResource();
    console.log("Simple test completed");
}

runSimpleTest();