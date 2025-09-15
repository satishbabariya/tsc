// Simple Memory Test for Basic Verification
// This test focuses on basic ARC and RAII functionality

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

function testSimpleMemory() {
    _print("=== Simple Memory Test ===");
    
    let resource1 = new SimpleResource(1);
    let resource2 = new SimpleResource(2);
    
    _print("Resources created, function ending...");
}

testSimpleMemory();