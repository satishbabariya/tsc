// Minimal RAII Test - Basic Destructor Functionality
// This test focuses on core RAII features that should work

class BasicResource {
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log(`Resource '${name}' created`);
    }
    
    ~BasicResource() {
        console.log(`Resource '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
}

// Test function
function testBasicRAII(): void {
    console.log("=== Testing Basic RAII ===");
    
    let resource = new BasicResource("TestResource");
    console.log(`Resource name: ${resource.getName()}`);
    
    // Resource should be automatically destroyed when going out of scope
    console.log("End of test function - resource should be destroyed");
}

// Run the test
testBasicRAII();