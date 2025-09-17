// Test to gradually increase complexity and identify property access issues

class ResourceWithStringField {
    name: string;

    constructor(name: string) {
        this.name = name;
        _print("ResourceWithStringField constructor");
    }

~

    ResourceWithStringField() {
        _print("ResourceWithStringField destructor");
        // Note: Not accessing this.name to avoid property access issues for now
    }
}

class ResourceWithNumberField {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("ResourceWithNumberField constructor");
    }

~

    ResourceWithNumberField() {
        _print("ResourceWithNumberField destructor");
        // Note: Not accessing this.id to avoid property access issues for now
    }
}

function testStringFieldResource() {
    _print("=== Testing String Field Resource ===");
    let resource = new ResourceWithStringField("test");
    _print("String field resource created");
}

function testNumberFieldResource() {
    _print("=== Testing Number Field Resource ===");
    let resource = new ResourceWithNumberField(42);
    _print("Number field resource created");
}

function testMultipleResources() {
    _print("=== Testing Multiple Resources ===");
    let resource1 = new ResourceWithStringField("first");
    let resource2 = new ResourceWithNumberField(100);
    _print("Multiple resources created");
    // Destructors should be called in reverse order: resource2, resource1
}

function runGradualTests() {
    _print("Starting gradual complexity tests...");

    testStringFieldResource();
    _print("");

    testNumberFieldResource();
    _print("");

    testMultipleResources();
    _print("");

    _print("Gradual complexity tests completed");
}

runGradualTests();