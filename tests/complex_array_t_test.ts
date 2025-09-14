// Complex test for Array<T> functionality
class GenericArrayContainer<T> {
    items: Array<T>;
    
    constructor() {
        this.items = [];
    }
    
    addItem(item: T): void {
        this.items.push(item);
    }
    
    getLength(): number {
        return this.items.length;
    }
}

let numberContainer = new GenericArrayContainer<number>();
numberContainer.addItem(42);
console.log("Number array length:", numberContainer.getLength());

let stringContainer = new GenericArrayContainer<string>();
stringContainer.addItem("hello");
console.log("String array length:", stringContainer.getLength());