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
_print("Number array length:", numberContainer.getLength());

let stringContainer = new GenericArrayContainer<string>();
stringContainer.addItem("hello");
_print("String array length:", stringContainer.getLength());