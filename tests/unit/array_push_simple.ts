// Test array push method without global array literals

class Container<T> {
    items: T[];

    add(item: T): void {
        this.items.push(item);
    }

    getLength(): number {
        return this.items.length;
    }
}