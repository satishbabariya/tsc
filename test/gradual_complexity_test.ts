// Test to gradually increase complexity and identify property access issues

class ResourceWithStringField {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log("ResourceWithStringField constructor");
    }
    
    ~ResourceWithStringField() {
        console.log("ResourceWithStringField destructor");
        // Note: Not accessing this.name to avoid property access issues for now
    }
}

class ResourceWithNumberField {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("ResourceWithNumberField constructor");
    }
    
    ~ResourceWithNumberField() {
        console.log("ResourceWithNumberField destructor");
        // Note: Not accessing this.id to avoid property access issues for now
    }
}

function testStringFieldResource() {
    console.log("=== Testing String Field Resource ===");
    let resource = new ResourceWithStringField("test");
    console.log("String field resource created");
}

function testNumberFieldResource() {
    console.log("=== Testing Number Field Resource ===");
    let resource = new ResourceWithNumberField(42);
    console.log("Number field resource created");
}

function testMultipleResources() {
    console.log("=== Testing Multiple Resources ===");
    let resource1 = new ResourceWithStringField("first");
    let resource2 = new ResourceWithNumberField(100);
    console.log("Multiple resources created");
    // Destructors should be called in reverse order: resource2, resource1
}

function runGradualTests() {
    console.log("Starting gradual complexity tests...");
    
    testStringFieldResource();
    console.log("");
    
    testNumberFieldResource();
    console.log("");
    
    testMultipleResources();
    console.log("");
    
    console.log("Gradual complexity tests completed");
}

runGradualTests();