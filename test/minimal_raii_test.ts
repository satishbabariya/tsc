// Minimal RAII Test - Basic Destructor Functionality
// This test focuses on core RAII features that should work

class BasicResource {
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        _print(`Resource '${name}' created`);
    }
    
    ~BasicResource() {
        _print(`Resource '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
}

// Test function
function testBasicRAII(): void {
    _print("=== Testing Basic RAII ===");
    
    let resource = new BasicResource("TestResource");
    _print(`Resource name: ${resource.getName()}`);
    
    // Resource should be automatically destroyed when going out of scope
    _print("End of test function - resource should be destroyed");
}

// Run the test
testBasicRAII();