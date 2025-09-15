class DebugResource {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        _print("DebugResource constructor - id:", this.id);
    }
    
    ~DebugResource() {
        _print("DebugResource destructor - id:", this.id);
    }
}

function testTwoResources() {
    _print("Creating first resource");
    let resource1 = new DebugResource(1);
    _print("Creating second resource");
    let resource2 = new DebugResource(2);
    _print("Both resources created");
}

testTwoResources();