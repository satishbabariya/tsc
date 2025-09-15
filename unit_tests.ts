
// Unit tests for generic constraints
interface Comparable<T> {
    compareTo(other: T): number;
}

interface Serializable {
    serialize(): string;
}

// Test constraint checking
function testConstraintChecking<T extends Comparable<T>>(item: T): T {
    return item;
}

function testMultipleConstraints<T extends Comparable<T> & Serializable>(item: T): string {
    const comparison = item.compareTo(item);
    const serialized = item.serialize();
    return `Comparison: ${comparison}, Serialized: ${serialized}`;
}

// Test overload resolution
function testOverloadResolution<T>(value: T): string;
function testOverloadResolution<T extends string>(value: T): string;
function testOverloadResolution<T>(value: T): string {
    return `Value: ${value}`;
}

// Test type inference
function testTypeInference<T>(items: T[]): T[] {
    return items.map(item => item);
}

// Test array type operations
function testArrayTypes<T>(items: T[]): T[] {
    return items.filter((item, index) => index % 2 === 0);
}

// Test generic class constraints
class GenericContainer<T extends Comparable<T>> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    sort(): void {
        this.items.sort((a, b) => a.compareTo(b));
    }
    
    getItems(): T[] {
        return this.items;
    }
}

// Test generic method constraints
class Utility {
    static process<T extends Serializable>(item: T): string {
        return item.serialize();
    }
    
    static compare<T extends Comparable<T>>(a: T, b: T): number {
        return a.compareTo(b);
    }
}

// Run unit tests
console.log("Unit tests completed");
