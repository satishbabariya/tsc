class ResourceWithStringField {
    name: string;
    
    constructor(name: string) {
        this.name = name;
        _print("ResourceWithStringField constructor");
    }
    
    ~ResourceWithStringField() {
        _print("ResourceWithStringField destructor");
    }
}

function testStringFieldResource() {
    _print("=== Testing String Field Resource ===");
    let resource = new ResourceWithStringField("test");
    _print("String field resource created");
}

testStringFieldResource();