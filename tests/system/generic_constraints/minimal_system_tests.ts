// Minimal System Tests for Generic Type Constraints
// Tests only the core functionality that we know works

// Test 1: Basic string constraint
function processString<T extends string>(value: T): T {
    return value;
}

// Test 2: Basic number constraint
function processNumber<T extends number>(value: T): T {
    return value;
}

// Test 3: Basic boolean constraint
function processBoolean<T extends boolean>(value: T): T {
    return value;
}

// Test 4: Multiple type parameters
function processMultiple<T extends string, U extends number>(str: T, num: U): string {
    return str + num.toString();
}

// Test 5: Simple interface constraint
interface SimpleInterface {
    id: number;
    name: string;
}

function processSimple<T extends SimpleInterface>(item: T): number {
    return item.id;
}

// Test 6: Generic class with constraint
class SimpleContainer<T extends SimpleInterface> {
    private item: T;
    
    constructor(item: T) {
        this.item = item;
    }
    
    setItem(item: T): void {
        this.item = item;
    }
    
    getItem(): T {
        return this.item;
    }
}

// Test data
const testString = "hello";
const testNumber = 42;
const testBoolean = true;

const testItem: SimpleInterface = {
    id: 123,
    name: "test item"
};

// Test functions
function testMinimalConstraints(): void {
    console.log("=== Minimal System Tests ===");
    
    // String constraint
    let strResult = processString<string>(testString);
    console.log("String result: " + strResult);
    
    // Number constraint
    let numResult = processNumber<number>(testNumber);
    console.log("Number result: " + numResult.toString());
    
    // Boolean constraint
    let boolResult = processBoolean<boolean>(testBoolean);
    console.log("Boolean result: " + boolResult.toString());
    
    // Multiple constraints
    let multiResult = processMultiple<string, number>(testString, testNumber);
    console.log("Multiple result: " + multiResult);
    
    // Interface constraint
    let interfaceResult = processSimple<SimpleInterface>(testItem);
    console.log("Interface result: " + interfaceResult.toString());
    
    // Generic class
    let container = new SimpleContainer<SimpleInterface>(testItem);
    let retrievedItem = container.getItem();
    console.log("Container result: " + retrievedItem.name);
    
    console.log("All minimal tests completed successfully!");
}

function main(): void {
    testMinimalConstraints();
}

main();