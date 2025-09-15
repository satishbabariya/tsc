class TestClass {
    constructor() {
        console.log("Constructor called");
    }
    
    ~TestClass() {
        console.log("Destructor called");
    }
}

let obj = new TestClass();
console.log("Object created");