
// Code generation integration with generic array types
class GenericArray<T> {
    private items: T[] = [];
    
    constructor(items: T[] = []) {
        this.items = [...items];
    }
    
    add(item: T): void {
        this.items.push(item);
    }
    
    addAll(items: T[]): void {
        this.items.push(...items);
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
    
    set(index: number, item: T): boolean {
        if (index >= 0 && index < this.items.length) {
            this.items[index] = item;
            return true;
        }
        return false;
    }
    
    length(): number {
        return this.items.length;
    }
    
    isEmpty(): boolean {
        return this.items.length === 0;
    }
    
    clear(): void {
        this.items = [];
    }
    
    // Functional operations
    map<U>(fn: (item: T, index: number) => U): U[] {
        return this.items.map(fn);
    }
    
    filter(predicate: (item: T, index: number) => boolean): T[] {
        return this.items.filter(predicate);
    }
    
    reduce<U>(fn: (acc: U, item: T, index: number) => U, initial: U): U {
        return this.items.reduce(fn, initial);
    }
    
    forEach(fn: (item: T, index: number) => void): void {
        this.items.forEach(fn);
    }
    
    find(predicate: (item: T, index: number) => boolean): T | undefined {
        return this.items.find(predicate);
    }
    
    findIndex(predicate: (item: T, index: number) => boolean): number {
        return this.items.findIndex(predicate);
    }
    
    some(predicate: (item: T, index: number) => boolean): boolean {
        return this.items.some(predicate);
    }
    
    every(predicate: (item: T, index: number) => boolean): boolean {
        return this.items.every(predicate);
    }
    
    // Array operations
    slice(start?: number, end?: number): T[] {
        return this.items.slice(start, end);
    }
    
    splice(start: number, deleteCount?: number, ...items: T[]): T[] {
        return this.items.splice(start, deleteCount, ...items);
    }
    
    concat(other: GenericArray<T>): GenericArray<T> {
        return new GenericArray([...this.items, ...other.items]);
    }
    
    // Sorting
    sort(compareFn?: (a: T, b: T) => number): GenericArray<T> {
        const sorted = [...this.items].sort(compareFn);
        return new GenericArray(sorted);
    }
    
    reverse(): GenericArray<T> {
        const reversed = [...this.items].reverse();
        return new GenericArray(reversed);
    }
    
    // Conversion
    toArray(): T[] {
        return [...this.items];
    }
    
    toSet(): Set<T> {
        return new Set(this.items);
    }
    
    toString(): string {
        return this.items.toString();
    }
}

// Test code generation integration
const numberArray = new GenericArray<number>([1, 2, 3, 4, 5]);
const stringArray = new GenericArray<string>(["a", "b", "c", "d", "e"]);

// Test array operations
numberArray.add(6);
stringArray.add("f");

const doubled = numberArray.map(x => x * 2);
const uppercased = stringArray.map(s => s.toUpperCase());

const evens = numberArray.filter(x => x % 2 === 0);
const longStrings = stringArray.filter(s => s.length > 1);

const sum = numberArray.reduce((acc, x) => acc + x, 0);
const concatenated = stringArray.reduce((acc, s) => acc + s, "");

const hasEven = numberArray.some(x => x % 2 === 0);
const allShort = stringArray.every(s => s.length <= 1);

console.log("Number array:", numberArray.toArray());
console.log("String array:", stringArray.toArray());
console.log("Doubled:", doubled);
console.log("Uppercased:", uppercased);
console.log("Evens:", evens);
console.log("Long strings:", longStrings);
console.log("Sum:", sum);
console.log("Concatenated:", concatenated);
console.log("Has even:", hasEven);
console.log("All short:", allShort);
