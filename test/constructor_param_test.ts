// Test to isolate constructor parameter issues

class SimpleStringResource {
    name: string;
    
    constructor(name: string) {
        _print("SimpleStringResource constructor");
        this.name = name;
    }
    
    ~SimpleStringResource() {
        _print("SimpleStringResource destructor");
    }
}

function testStringConstructor() {
    _print("Creating string resource");
    let resource = new SimpleStringResource("test");
    _print("String resource created");
}

testStringConstructor();