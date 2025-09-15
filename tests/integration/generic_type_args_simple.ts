// Simple test for generic type arguments in properties and methods

// Interface with generic type arguments in properties
interface Container<T> {
    data: T;
    items: T[];
}

// Interface with generic type arguments in methods
interface Processor<T> {
    process(value: T): T;
    transform(input: T[]): T[];
}

// Class with generic type arguments
class DataStore<T> {
    private storage: T[];
    private config: Container<T>;
    
    constructor(initialData: T[], settings: Container<T>) {
        this.storage = initialData;
        this.config = settings;
    }
    
    add(item: T): void {
        this.storage.push(item);
    }
    
    getAll(): T[] {
        return this.storage;
    }
}

// Function with generic type arguments
function createContainer<T>(data: T): Container<T> {
    return {
        data: data,
        items: [data]
    };
}

// Test the functionality
let stringContainer: Container<string> = createContainer("hello");
let numberContainer: Container<number> = createContainer(42);

let stringStore: DataStore<string> = new DataStore(["a", "b", "c"], stringContainer);
let numberStore: DataStore<number> = new DataStore([1, 2, 3], numberContainer);

// Test method calls
stringStore.add("d");
numberStore.add(4);