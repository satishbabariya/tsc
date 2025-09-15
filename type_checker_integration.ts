
// Type checker integration with generic constraints
interface Comparable<T> {
    compareTo(other: T): number;
}

interface Serializable {
    serialize(): string;
}

class NumberWrapper implements Comparable<NumberWrapper>, Serializable {
    value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    compareTo(other: NumberWrapper): number {
        return this.value - other.value;
    }
    
    serialize(): string {
        return this.value.toString();
    }
}

// Generic function with constraints
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

// Test type checking integration
const numberWrapper = new NumberWrapper(42);

// These should pass type checking
const processed1 = processComparable(numberWrapper);
const processed2 = processSerializable(numberWrapper);
const processed3 = processBoth(numberWrapper);

console.log("Processed comparable:", processed1);
console.log("Processed serializable:", processed2);
console.log("Processed both:", processed3);

// Test type checking with invalid types
class InvalidType {
    value: string;
    
    constructor(value: string) {
        this.value = value;
    }
}

const invalidType = new InvalidType("test");

// These should fail type checking
// const invalid1 = processComparable(invalidType); // Should fail
// const invalid2 = processSerializable(invalidType); // Should fail
// const invalid3 = processBoth(invalidType); // Should fail
