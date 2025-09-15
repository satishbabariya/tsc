class ResourceWithStringField {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log("ResourceWithStringField constructor");
    }
    
    ~ResourceWithStringField() {
        console.log("ResourceWithStringField destructor");
    }
}

function testStringFieldResource() {
    console.log("=== Testing String Field Resource ===");
    let resource = new ResourceWithStringField("test");
    console.log("String field resource created");
}

testStringFieldResource();