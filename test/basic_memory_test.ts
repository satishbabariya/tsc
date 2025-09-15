// Basic Memory Test - Simple ARC and RAII validation

class SimpleResource {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        _print(`SimpleResource ${id} created`);
    }
    
    ~SimpleResource() {
        _print(`SimpleResource ${this.id} destroyed`);
    }
}

function testBasicMemory() {
    _print("=== Basic Memory Test ===");
    
    let resource1 = new SimpleResource(1);
    let resource2 = new SimpleResource(2);
    
    _print("Resources created, going out of scope...");
    // Resources should be automatically destroyed when going out of scope
}

// Run the test
testBasicMemory();