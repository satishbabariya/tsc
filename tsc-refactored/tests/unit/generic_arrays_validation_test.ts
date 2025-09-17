// Validation test for generic arrays after the fix
// This tests the core functionality that was crashing before

// Test 1: Basic generic array property
class ArrayContainer<T> {
    items: T[];

    addItem(item: T): void {
        this.items.push(item);
    }

    getItem(index: number): T {
        return this.items[index];
    }
}

// Test 2: Generic array with different types
class NumberArrayContainer {
    numbers: number[];

    addNumber(num: number): void {
        this.numbers.push(num);
    }
}

class StringArrayContainer {
    strings: string[];

    addString(str: string): void {
        this.strings.push(str);
    }
}

// Test 3: Generic array methods
class GenericMethods<T> {
    data: T[];

    push(item: T): number {
        return this.data.push(item);
    }

    pop(): T {
        return this.data.pop();
    }

    length(): number {
        return this.data.length;
    }
}

// Test 4: Complex generic array scenarios
class ComplexArrayContainer<T> {
    primary: T[];
    secondary: T[];

    constructor() {
        this.primary = [];
        this.secondary = [];
    }

    transferAll(): void {
        while (this.primary.length > 0) {
            const item = this.primary.pop();
            this.secondary.push(item);
        }
    }

    getTotalLength(): number {
        return this.primary.length + this.secondary.length;
    }
}

// Test 5: Array of generic objects
interface GenericItem<T> {
    value: T;
    id: string;
}

class GenericObjectArray<T> {
    items: GenericItem<T>[];

    addGenericItem(item: GenericItem<T>): void {
        this.items.push(item);
    }

    findById(id: string): GenericItem<T> | null {
        return this.items.find(item => item.id === id) || null;
    }
}