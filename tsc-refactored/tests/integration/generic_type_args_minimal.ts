// Minimal test suite for generic type arguments in properties and methods

// ===== INTERFACE TESTS =====

// Basic generic interface with type arguments in properties
interface BasicContainer<T> {
    value: T;
    items: T[];
}

// Generic interface with type arguments in methods (void return only)
interface Processor<T> {
    process(input: T): void;
}

// ===== CLASS TESTS =====

// Basic generic class with array methods
class DataStore<T> {
    private storage: T[];

    add(item: T): void {
        this.storage.push(item);
    }

    getLength(): number {
        return this.storage.length;
    }

    // Test array methods
    testPush(item: T): number {
        return this.storage.push(item);
    }

    testLength(): number {
        return this.storage.length;
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