// Core Generic Class Integration Test
// Tests the fundamental generic class features that are working

class SimpleContainer<T> {
    data: T;

    constructor(data: T) {
        this.data = data;
    }
}

class Pair<A, B> {
    first: A;
    second: B;

    constructor(first: A, second: B) {
        this.first = first;
        this.second = second;
    }
}

function main(): number {
    // Test explicit type arguments - core functionality
    let stringContainer = new SimpleContainer<string>("hello");
    let numberContainer = new SimpleContainer<number>(42);
    let boolContainer = new SimpleContainer<boolean>(true);

    // Test multiple type parameters
    let stringNumberPair = new Pair<string, number>("count", 100);
    let numberBoolPair = new Pair<number, boolean>(123, false);

    // Test implicit type arguments
    let implicitContainer = new SimpleContainer(999);
    let implicitPair = new Pair("implicit", 456);

    // Basic arithmetic (tests the LLVM bug fix)
    let result = 10 + 20;
    let product = result * 2;

    return 0;
}