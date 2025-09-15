
// Generic type inference tests
class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    map<U>(fn: (value: T) => U): Container<U> {
        return new Container(fn(this.value));
    }
    
    flatMap<U>(fn: (value: T) => Container<U>): Container<U> {
        return fn(this.value);
    }
    
    filter(predicate: (value: T) => boolean): Container<T> {
        return predicate(this.value) ? this : new Container(null as any);
    }
}

// Generic utility functions
function identity<T>(value: T): T {
    return value;
}

function toArray<T>(value: T): T[] {
    return [value];
}

function toString<T>(value: T): string {
    return String(value);
}

function toNumber<T>(value: T): number {
    return Number(value);
}

// Test type inference
const stringContainer = new Container("hello");
const numberContainer = new Container(42);
const booleanContainer = new Container(true);

// Test map with type inference
const mappedString = stringContainer.map(s => s.length); // Should infer number
const mappedNumber = numberContainer.map(n => n * 2); // Should infer number
const mappedBoolean = booleanContainer.map(b => b ? "true" : "false"); // Should infer string

// Test flatMap with type inference
const flatMappedString = stringContainer.flatMap(s => new Container(s.length)); // Should infer number
const flatMappedNumber = numberContainer.flatMap(n => new Container(n.toString())); // Should infer string

// Test filter with type inference
const filteredString = stringContainer.filter(s => s.length > 3); // Should infer string
const filteredNumber = numberContainer.filter(n => n > 40); // Should infer number

// Test utility functions with type inference
const identityResult = identity("test"); // Should infer string
const arrayResult = toArray(42); // Should infer number[]
const stringResult = toString(true); // Should infer string
const numberResult = toNumber("123"); // Should infer number

console.log("Mapped string:", mappedString.value);
console.log("Mapped number:", mappedNumber.value);
console.log("Mapped boolean:", mappedBoolean.value);
console.log("Flat mapped string:", flatMappedString.value);
console.log("Flat mapped number:", flatMappedNumber.value);
console.log("Filtered string:", filteredString.value);
console.log("Filtered number:", filteredNumber.value);
console.log("Identity result:", identityResult);
console.log("Array result:", arrayResult);
console.log("String result:", stringResult);
console.log("Number result:", numberResult);
