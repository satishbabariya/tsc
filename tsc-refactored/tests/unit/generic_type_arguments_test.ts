// Test generic type arguments in properties and methods

// Interface with generic type arguments in properties
interface Container<T> {
    data: T;
    items: T[];
    metadata: Container<string>;
}

// Interface with generic type arguments in methods
interface Processor<T, U> {
    process(value: T): U;

    transform(input: T[], config: Container<U>): U[];
}

// Class with generic type arguments
class DataStore<T> {
    private storage: T[] = [];
    private config: Container<T>;

    constructor(initialData: T[], settings: Container<T>) {
        this.storage = initialData;
        this.config = settings;
    }

    add(item: T): void {
        this.storage.push(item);
    }

    get(index: number): T | null {
        if (index >= 0 && index < this.storage.length) {
            return this.storage[index];
        }
        return null;
    }

    getAll(): T[] {
        return this.storage;
    }

    processWith(processor: Processor<T, string>): string[] {
        let results: string[] = [];
        for (let item of this.storage) {
            results.push(processor.process(item));
        }
        return results;
    }
}

// Function with generic type arguments
function createContainer<T>(data: T): Container<T> {
    return {
        data: data,
        items: [data],
        metadata: {data: "test", items: ["test"], metadata: null as any}
    };
}

// Nested generic types
interface NestedContainer<T, U> {
    primary: Container<T>;
    secondary: Container<U>;
    combined: Container<T | U>;
}

// Complex generic constraints
interface AdvancedProcessor<T extends string | number, U extends Container<T>> {
    input: T;
    processor: Processor<T, U>;
    result: Container<U>;
}

// Test the functionality
let stringContainer: Container<string> = createContainer("hello");
let numberContainer: Container<number> = createContainer(42);

let stringStore: DataStore<string> = new DataStore(["a", "b", "c"], stringContainer);
let numberStore: DataStore<number> = new DataStore([1, 2, 3], numberContainer);

// Test method calls
stringStore.add("d");
numberStore.add(4);

let firstString: string | null = stringStore.get(0);
let firstNumber: number | null = numberStore.get(0);