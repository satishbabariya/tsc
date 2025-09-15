
// Complex generic array operations
interface Entity {
    id: number;
    createdAt: Date;
    updatedAt: Date;
}

interface User extends Entity {
    name: string;
    email: string;
    age: number;
}

interface Product extends Entity {
    title: string;
    description: string;
    price: number;
    category: string;
}

// Generic array operations
class GenericArray<T> {
    private items: T[] = [];
    
    constructor(items: T[] = []) {
        this.items = [...items];
    }
    
    // Basic operations
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
    
    removeAt(index: number): T | undefined {
        if (index >= 0 && index < this.items.length) {
            return this.items.splice(index, 1)[0];
        }
        return undefined;
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

// Test complex generic array operations
const users = new GenericArray<User>([
    { id: 1, name: "Alice", email: "alice@example.com", age: 25, createdAt: new Date(), updatedAt: new Date() },
    { id: 2, name: "Bob", email: "bob@example.com", age: 30, createdAt: new Date(), updatedAt: new Date() },
    { id: 3, name: "Charlie", email: "charlie@example.com", age: 35, createdAt: new Date(), updatedAt: new Date() }
]);

const products = new GenericArray<Product>([
    { id: 1, title: "Laptop", description: "High-performance laptop", price: 999.99, category: "Electronics", createdAt: new Date(), updatedAt: new Date() },
    { id: 2, title: "Mouse", description: "Wireless mouse", price: 29.99, category: "Electronics", createdAt: new Date(), updatedAt: new Date() },
    { id: 3, title: "Keyboard", description: "Mechanical keyboard", price: 79.99, category: "Electronics", createdAt: new Date(), updatedAt: new Date() }
]);

// Test user operations
const userNames = users.map(user => user.name);
const youngUsers = users.filter(user => user.age < 30);
const totalAge = users.reduce((sum, user) => sum + user.age, 0);
const hasAlice = users.some(user => user.name === "Alice");
const allAdults = users.every(user => user.age >= 18);

console.log("User names:", userNames);
console.log("Young users:", youngUsers);
console.log("Total age:", totalAge);
console.log("Has Alice:", hasAlice);
console.log("All adults:", allAdults);

// Test product operations
const productTitles = products.map(product => product.title);
const expensiveProducts = products.filter(product => product.price > 50);
const totalPrice = products.reduce((sum, product) => sum + product.price, 0);
const hasLaptop = products.some(product => product.title === "Laptop");
const allElectronics = products.every(product => product.category === "Electronics");

console.log("Product titles:", productTitles);
console.log("Expensive products:", expensiveProducts);
console.log("Total price:", totalPrice);
console.log("Has laptop:", hasLaptop);
console.log("All electronics:", allElectronics);

// Test array operations
const sortedUsers = users.sort((a, b) => a.age - b.age);
const reversedProducts = products.reverse();
const combined = users.concat(products as any);

console.log("Sorted users:", sortedUsers.toArray());
console.log("Reversed products:", reversedProducts.toArray());
console.log("Combined length:", combined.length());
