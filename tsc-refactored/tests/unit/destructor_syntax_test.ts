class TestClass {
    constructor() {
        _print("Constructor called");
    }

~

    TestClass() {
        _print("Destructor called");
    }
}

let obj = new TestClass();
_print("Object created");