
// Generic array types tests
interface Comparable<T> {
    compareTo(other: T): number;
}

class NumberArray<T extends number> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    remove(item: T): boolean {
        const index = this.items.indexOf(item);
        if (index !== -1) {
            this.items.splice(index, 1);
            return true;
        }
        return false;
    }
    
    get(index: number): T | undefined {
        return this.items[index];
    }
    
    length(): number {
        return this.items.length;
    }
    
    toArray(): T[] {
        return [...this.items];
    }
    
    fromArray(array: T[]): void {
        this.items = [...array];
    }
    
    map<U>(fn: (item: T) => U): U[] {
        return this.items.map(fn);
    }
    
    filter(predicate: (item: T) => boolean): T[] {
        return this.items.filter(predicate);
    }
    
    reduce<U>(fn: (acc: U, item: T) => U, initial: U): U {
        return this.items.reduce(fn, initial);
    }
}

// Generic array utilities
function createArray<T>(...items: T[]): T[] {
    return items;
}

function arrayToSet<T>(array: T[]): Set<T> {
    return new Set(array);
}

function setToArray<T>(set: Set<T>): T[] {
    return Array.from(set);
}

function arrayToMap<T, K, V>(array: T[], keyFn: (item: T) => K, valueFn: (item: T) => V): Map<K, V> {
    const map = new Map<K, V>();
    for (const item of array) {
        map.set(keyFn(item), valueFn(item));
    }
    return map;
}

function mapToArray<K, V>(map: Map<K, V>): [K, V][] {
    return Array.from(map.entries());
}

// Test generic array types
const numberArray = new NumberArray<number>();

numberArray.add(1);
numberArray.add(2);
numberArray.add(3);
numberArray.add(4);
numberArray.add(5);

console.log("Array length:", numberArray.length());
console.log("Array items:", numberArray.toArray());

// Test array operations
const doubled = numberArray.map(x => x * 2);
const evens = numberArray.filter(x => x % 2 === 0);
const sum = numberArray.reduce((acc, x) => acc + x, 0);

console.log("Doubled:", doubled);
console.log("Evens:", evens);
console.log("Sum:", sum);

// Test array conversions
const array = createArray(1, 2, 3, 4, 5);
const set = arrayToSet(array);
const backToArray = setToArray(set);

console.log("Original array:", array);
console.log("Set:", set);
console.log("Back to array:", backToArray);

// Test array to map conversion
const users = [
    { id: 1, name: "Alice" },
    { id: 2, name: "Bob" },
    { id: 3, name: "Charlie" }
];

const userMap = arrayToMap(users, user => user.id, user => user.name);
const mapArray = mapToArray(userMap);

console.log("User map:", userMap);
console.log("Map to array:", mapArray);
