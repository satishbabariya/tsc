// Simple Memory Test for Basic Verification
// This test focuses on basic ARC and RAII functionality

class SimpleResource {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log(`SimpleResource ${id} created`);
    }
    
    ~SimpleResource() {
        console.log(`SimpleResource ${this.id} destroyed`);
    }
}

function testSimpleMemory() {
    console.log("=== Simple Memory Test ===");
    
    let resource1 = new SimpleResource(1);
    let resource2 = new SimpleResource(2);
    
    console.log("Resources created, function ending...");
}

testSimpleMemory();