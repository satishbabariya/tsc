// Simple RAII Test - Focus on core functionality

class SimpleResource {
    private id: number;

    constructor(id: number) {
        this.id = id;
        _print("Resource created with ID: " + id);
    }

~

    SimpleResource() {
        _print("Resource destroyed with ID: " + this.id);
    }

    getId(): number {
        return this.id;
    }
}

function testSimpleRAII(): void {
    _print("=== Starting RAII Test ===");

    let resource = new SimpleResource(42);
    _print("Resource ID: " + resource.getId());

    _print("=== End of RAII Test ===");
}

testSimpleRAII();