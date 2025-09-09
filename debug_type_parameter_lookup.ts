// Test to debug type parameter lookup in different scopes

class TestClass<T> {
    // This should work - property declaration in class scope
    property: T;
    
    constructor(value: T) {
        // This should work - parameter in constructor scope, T should be found from class scope
        this.property = value;
    }
    
    method(param: T): T {
        // This should work - parameter and return type in method scope, T should be found from class scope
        return param;
    }
}

function main(): number {
    let test = new TestClass<number>(42);
    return 0;
}