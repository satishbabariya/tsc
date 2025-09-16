class TestClass1 {
    id: number;

    constructor(id: number) {
        this.id = id;
        _print("TestClass1 constructor");
    }

~

    TestClass1() {
        _print("TestClass1 destructor");
    }
}

class TestClass2 {
    name: string;

    constructor(name: string) {
        this.name = name;
        _print("TestClass2 constructor");
    }

~

    TestClass2() {
        _print("TestClass2 destructor");
    }
}

function testDifferentDestructors() {
    _print("Creating objects with different destructors");
    let obj1 = new TestClass1(1);
    _print("First object created");
    let obj2 = new TestClass2("test");
    _print("Second object created");
    _print("Both objects created");
}

testDifferentDestructors();