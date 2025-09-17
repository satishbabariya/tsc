// Test array push method specifically

// Simple array test
let numbers: number[] = [1, 2, 3];
numbers.push(4);

// Generic array test
class Container<T> {
    items: T[];

    add(item: T): void {
        this.items.push(item);
    }
}