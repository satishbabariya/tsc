// Test suite for generic type arguments in properties and methods (working features only)

// ===== INTERFACE TESTS =====

// Basic generic interface with type arguments in properties
interface BasicContainer<T> {
    value: T;
    items: T[];
}

// Generic interface with type arguments in methods
interface Processor<T> {
    process(input: T): T;
    transform(items: T[]): T[];
}

// ===== CLASS TESTS =====

// Basic generic class with array methods
class DataStore<T> {
    private storage: T[];
    
    add(item: T): void {
        this.storage.push(item);
    }
    
    get(index: number): T {
        return this.storage[index];
    }
    
    getAll(): T[] {
        return this.storage;
    }
    
    getLength(): number {
        return this.storage.length;
    }
    
    // Test array methods
    testPush(item: T): number {
        return this.storage.push(item);
    }
    
    testPop(): T {
        return this.storage.pop();
    }
    
    testLength(): number {
        return this.storage.length;
    }
}

// Generic class with multiple type parameters
class KeyValueStore<K, V> {
    private data: V[];
    private keys: K[];
    
    set(key: K, value: V): void {
        this.keys.push(key);
        this.data.push(value);
    }
    
    get(key: K): V {
        return this.data[0];
    }
    
    getKeys(): K[] {
        return this.keys;
    }
}

// ===== FUNCTION TESTS =====

// Generic function with type arguments
function createContainer<T>(data: T): BasicContainer<T> {
    return {
        value: data,
        items: []
    };
}

// Generic function with array type arguments
function processArray<T>(items: T[]): T[] {
    return items;
}