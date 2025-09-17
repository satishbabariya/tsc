// Test to isolate number constructor parameter issues

class SimpleNumberResource {
    id: number;

    constructor(id: number) {
        _print("SimpleNumberResource constructor");
        this.id = id;
    }

~

    SimpleNumberResource() {
        _print("SimpleNumberResource destructor");
    }
}

function testNumberConstructor() {
    _print("Creating number resource");
    let resource = new SimpleNumberResource(42);
    _print("Number resource created");
}

testNumberConstructor();