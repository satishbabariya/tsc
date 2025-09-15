
// Error reporting integration with generic constraints
interface Comparable<T> {
    compareTo(other: T): number;
}

interface Serializable {
    serialize(): string;
}

class ValidType implements Comparable<ValidType>, Serializable {
    value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    compareTo(other: ValidType): number {
        return this.value - other.value;
    }
    
    serialize(): string {
        return this.value.toString();
    }
}

class InvalidType {
    value: string;
    
    constructor(value: string) {
        this.value = value;
    }
}

// Generic functions with constraints
function processComparable<T extends Comparable<T>>(item: T): T {
    return item;
}

function processSerializable<T extends Serializable>(item: T): string {
    return item.serialize();
}

function processBoth<T extends Comparable<T> & Serializable>(item: T): string {
    const comparison = item.compareTo(item);
    const serialized = item.serialize();
    return `Comparison: ${comparison}, Serialized: ${serialized}`;
}

// Test error reporting
const validType = new ValidType(42);
const invalidType = new InvalidType("test");

// These should work
const result1 = processComparable(validType);
const result2 = processSerializable(validType);
const result3 = processBoth(validType);

console.log("Valid results:", result1, result2, result3);

// These should generate errors
// const error1 = processComparable(invalidType); // Error: Type 'InvalidType' does not satisfy constraint 'Comparable<InvalidType>'
// const error2 = processSerializable(invalidType); // Error: Type 'InvalidType' does not satisfy constraint 'Serializable'
// const error3 = processBoth(invalidType); // Error: Type 'InvalidType' does not satisfy constraint 'Comparable<InvalidType> & Serializable'

// Test generic overloading errors
function overloadedFunction<T>(input: T): T;
function overloadedFunction<T>(input: T[]): T[];
function overloadedFunction<T>(input: T | T[]): T | T[] {
    if (Array.isArray(input)) {
        return input;
    } else {
        return input;
    }
}

// These should work
const singleResult = overloadedFunction(42);
const arrayResult = overloadedFunction([1, 2, 3]);

console.log("Overload results:", singleResult, arrayResult);

// Test type inference errors
function inferType<T>(input: T): T {
    return input;
}

// These should work with type inference
const inferred1 = inferType(42); // T inferred as number
const inferred2 = inferType("hello"); // T inferred as string
const inferred3 = inferType(true); // T inferred as boolean

console.log("Inferred types:", inferred1, inferred2, inferred3);
