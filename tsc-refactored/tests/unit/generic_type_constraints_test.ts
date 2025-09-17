// Test generic type constraints with array methods

// Test with basic type constraints
class ConstrainedContainer<T extends string> {
    private items: T[];

    add(item: T): void {
        this.items.push(item);
    }

    getLength(): number {
        return this.items.length;
    }

    testPush(item: T): number {
        return this.items.push(item);
    }
}

// Test with union type constraints
class UnionContainer<T extends string | number> {
    private data: T[];

    add(value: T): void {
        this.data.push(value);
    }

    getLength(): number {
        return this.data.length;
    }

    testArrayMethods(value: T): number {
        return this.data.push(value);
    }
}

// Test with nested constraints
class NestedContainer<T extends string> {
    private storage: T[];
    private backup: T[];

    add(item: T): void {
        this.storage.push(item);
    }

    backupData(): void {
        this.backup = this.storage;
    }

    restoreData(): void {
        this.storage = this.backup;
    }

    getTotalLength(): number {
        return this.storage.length + this.backup.length;
    }
}

// Test with interface constraints
interface StringLike {
    toString(): string;
}

class InterfaceContainer<T extends StringLike> {
    private items: T[];

    add(item: T): void {
        this.items.push(item);
    }

    getLength(): number {
        return this.items.length;
    }
}