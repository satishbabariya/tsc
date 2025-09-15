// Test to isolate number constructor parameter issues

class SimpleNumberResource {
    id: number;
    
    constructor(id: number) {
        console.log("SimpleNumberResource constructor");
        this.id = id;
    }
    
    ~SimpleNumberResource() {
        console.log("SimpleNumberResource destructor");
    }
}

function testNumberConstructor() {
    console.log("Creating number resource");
    let resource = new SimpleNumberResource(42);
    console.log("Number resource created");
}

testNumberConstructor();