class DebugResource {
    id: number;
    
    constructor(id: number) {
        this.id = id;
        console.log("DebugResource constructor - id:", this.id);
    }
    
    ~DebugResource() {
        console.log("DebugResource destructor - id:", this.id);
    }
}

function testTwoResources() {
    console.log("Creating first resource");
    let resource1 = new DebugResource(1);
    console.log("Creating second resource");
    let resource2 = new DebugResource(2);
    console.log("Both resources created");
}

testTwoResources();