// Simple destructor test without complex property access

class SimpleResource {
    constructor() {
        console.log("SimpleResource constructor");
    }
    
    ~SimpleResource() {
        console.log("SimpleResource destructor");
    }
}

function testSimpleDestructor() {
    console.log("Creating resource");
    let resource = new SimpleResource();
    console.log("Resource created, exiting function");
}

testSimpleDestructor();