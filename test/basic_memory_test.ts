// Basic Memory Test - Simple ARC and RAII validation

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

function testBasicMemory() {
    console.log("=== Basic Memory Test ===");
    
    let resource1 = new SimpleResource(1);
    let resource2 = new SimpleResource(2);
    
    console.log("Resources created, going out of scope...");
    // Resources should be automatically destroyed when going out of scope
}

// Run the test
testBasicMemory();