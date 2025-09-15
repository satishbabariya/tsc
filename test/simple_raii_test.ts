// Simple RAII Test - Focus on core functionality

class SimpleResource {
    private id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("Resource created with ID: " + id);
    }
    
    ~SimpleResource() {
        console.log("Resource destroyed with ID: " + this.id);
    }
    
    getId(): number {
        return this.id;
    }
}

function testSimpleRAII(): void {
    console.log("=== Starting RAII Test ===");
    
    let resource = new SimpleResource(42);
    console.log("Resource ID: " + resource.getId());
    
    console.log("=== End of RAII Test ===");
}

testSimpleRAII();