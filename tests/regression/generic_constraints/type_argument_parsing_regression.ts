// Type Argument Parsing Regression Tests
// These tests ensure that type argument parsing continues to work correctly
// and prevent regressions in generic method calls with explicit type arguments

// Test 1: Function with type arguments (CRITICAL - must not regress)
function processString<T extends string>(value: T): T {
    return value;
}

function processNumber<T extends number>(value: T): T {
    return value;
}

function processMultiple<T extends string, U extends number>(str: T, num: U): string {
    return str + "123";
}

// Test 2: Generic class with type arguments (CRITICAL - must not regress)
class GenericProcessor<T extends string> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
    
    process<U extends number>(num: U): string {
        return this.value + "123";
    }
}

// Test 3: Interface with type arguments (CRITICAL - must not regress)
interface GenericInterface<T extends string> {
    value: T;
    process(): T;
}

class GenericInterfaceImpl<T extends string> implements GenericInterface<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    process(): T {
        return this.value;
    }
}

// Test functions that validate type argument parsing
function testTypeArgumentParsingRegression(): void {
    // These tests MUST pass - they represent core type argument parsing functionality
    
    // Function calls with explicit type arguments
    let strResult = processString<string>("hello");
    let numResult = processNumber<number>(42);
    let multiResult = processMultiple<string, number>("test", 123);
    
    // Generic class instantiation with type arguments
    let processor = new GenericProcessor<string>("processor");
    let processorValue = processor.getValue();
    let processorResult = processor.process<number>(42);
    
    // Generic interface implementation with type arguments
    let interfaceImpl = new GenericInterfaceImpl<string>("interface");
    let interfaceValue = interfaceImpl.value;
    let interfaceResult = interfaceImpl.process();
    
    // All tests completed successfully
    let allTestsPassed = true;
}

function main(): void {
    testTypeArgumentParsingRegression();
}

main();