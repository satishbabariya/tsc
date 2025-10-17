// Complex generic type resolution tests
class Container<T> {
    private items: T[];
    
    constructor() {
        this.items = [];
    }
    
    add(item: T): void {
        this.items.push(item);
    }
    
    get(index: number): T {
        return this.items[index];
    }
    
    size(): number {
        return this.items.length;
    }
}

// Test: generic type instantiation
let numberContainer: Container<number> = new Container<number>();
let stringContainer: Container<string> = new Container<string>();

// Test: generic method calls
numberContainer.add(42);
numberContainer.add(100);
stringContainer.add("hello");
stringContainer.add("world");

let num: number = numberContainer.get(0);
let str: string = stringContainer.get(1);
let size: number = numberContainer.size();

console.log("Number:", num);
console.log("String:", str);
console.log("Size:", size);