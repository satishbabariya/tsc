// Simple test for monomorphization
console.log("🧪 Running Simple Monomorphization Test");
console.log("=====================================");

// Test basic generic function
function identity<T>(value: T): T {
    return value;
}

// Test generic class
class GenericClass<T> {
    private value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    getValue(): T {
        return this.value;
    }
}

// Run tests
console.log("Testing generic function...");
let stringResult = identity<string>("hello");
let numberResult = identity<number>(42);
console.log(`String result: ${stringResult}`);
console.log(`Number result: ${numberResult}`);

console.log("Testing generic class...");
let stringClass = new GenericClass<string>("test");
let numberClass = new GenericClass<number>(100);
console.log(`String class value: ${stringClass.getValue()}`);
console.log(`Number class value: ${numberClass.getValue()}`);

console.log("✅ Simple monomorphization test completed successfully!");