// Test to isolate constructor parameter issues

class SimpleStringResource {
    name: string;
    
    constructor(name: string) {
        console.log("SimpleStringResource constructor");
        this.name = name;
    }
    
    ~SimpleStringResource() {
        console.log("SimpleStringResource destructor");
    }
}

function testStringConstructor() {
    console.log("Creating string resource");
    let resource = new SimpleStringResource("test");
    console.log("String resource created");
}

testStringConstructor();