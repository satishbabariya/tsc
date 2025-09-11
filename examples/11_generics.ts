// Generics
// Demonstrates generic functions, classes, and type parameters

// Basic generic function
function identity<T>(arg: T): T {
    return arg;
}

// Generic function with multiple type parameters
function pair<T, U>(first: T, second: U): [T, U] {
    return [first, second];
}

// Generic function with constraints
function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
    return obj[key];
}

// Generic function with default type parameter
function createArray<T = string>(length: number, value: T): T[] {
    let result: T[] = [];
    for (let i = 0; i < length; i++) {
        result.push(value);
    }
    return result;
}

// Generic class
class Container<T> {
    private value: T;

    constructor(value: T) {
        this.value = value;
    }

    getValue(): T {
        return this.value;
    }

    setValue(value: T): void {
        this.value = value;
    }
}

// Generic class with multiple type parameters
class Pair<T, U> {
    private first: T;
    private second: U;

    constructor(first: T, second: U) {
        this.first = first;
        this.second = second;
    }

    getFirst(): T {
        return this.first;
    }

    getSecond(): U {
        return this.second;
    }

    setFirst(value: T): void {
        this.first = value;
    }

    setSecond(value: U): void {
        this.second = value;
    }
}

// Generic class with constraints
class Repository<T extends { id: number }> {
    private items: T[] = [];

    add(item: T): void {
        this.items.push(item);
    }

    findById(id: number): T | undefined {
        return this.items.find(item => item.id === id);
    }

    getAll(): T[] {
        return [...this.items];
    }

    remove(id: number): boolean {
        let index = this.items.findIndex(item => item.id === id);
        if (index !== -1) {
            this.items.splice(index, 1);
            return true;
        }
        return false;
    }
}

// Generic interface
interface Comparable<T> {
    compareTo(other: T): number;
}

// Generic interface with multiple type parameters
interface KeyValuePair<K, V> {
    key: K;
    value: V;
}

// Generic interface with constraints
interface Processor<T extends string | number> {
    process(input: T): T;
}

// Generic type alias
type Optional<T> = T | null | undefined;
type Result<T, E> = {
    success: true;
    data: T;
} | {
    success: false;
    error: E;
};

// Generic type alias with constraints
type NonNullable<T> = T extends null | undefined ? never : T;
type Flatten<T> = T extends (infer U)[] ? U : T;

// Generic function with conditional types
function processValue<T>(value: T): T extends string ? string : T extends number ? number : unknown {
    if (typeof value === "string") {
        return value.toUpperCase() as any;
    } else if (typeof value === "number") {
        return (value * 2) as any;
    } else {
        return value as any;
    }
}

// Generic function with mapped types
function createReadonly<T>(obj: T): Readonly<T> {
    return Object.freeze({ ...obj });
}

// Generic function with utility types
function makePartial<T>(obj: T): Partial<T> {
    return { ...obj };
}

function makeRequired<T>(obj: Partial<T>): Required<T> {
    return obj as Required<T>;
}

// Generic class with static methods
class MathUtils {
    static add<T extends number>(a: T, b: T): T {
        return (a + b) as T;
    }

    static multiply<T extends number>(a: T, b: T): T {
        return (a * b) as T;
    }

    static max<T extends number>(a: T, b: T): T {
        return a > b ? a : b;
    }
}

// Generic class with inheritance
class BaseEntity {
    id: number;
    createdAt: Date;

    constructor(id: number) {
        this.id = id;
        this.createdAt = new Date();
    }
}

class GenericRepository<T extends BaseEntity> {
    private items: T[] = [];

    add(item: T): void {
        this.items.push(item);
    }

    findById(id: number): T | undefined {
        return this.items.find(item => item.id === id);
    }

    getAll(): T[] {
        return [...this.items];
    }
}

// Generic class with method overloading
class DataProcessor<T> {
    process(data: T): T;
    process(data: T[]): T[];
    process(data: T | T[]): T | T[] {
        if (Array.isArray(data)) {
            return data.map(item => this.processItem(item));
        } else {
            return this.processItem(data);
        }
    }

    private processItem(item: T): T {
        // Process individual item
        return item;
    }
}

// Generic function with rest parameters
function combine<T>(...items: T[]): T[] {
    return items;
}

// Generic function with spread operator
function spread<T>(items: T[]): T[] {
    return [...items];
}

// Generic function with async/await
async function fetchData<T>(url: string): Promise<T> {
    // Simulate async operation
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve({} as T);
        }, 100);
    });
}

// Usage examples
let stringIdentity: string = identity("Hello");
let numberIdentity: number = identity(42);
let booleanIdentity: boolean = identity(true);

let stringPair: [string, number] = pair("Alice", 25);
let mixedPair: [number, string] = pair(100, "points");

let person = { name: "Bob", age: 30, city: "New York" };
let name: string = getProperty(person, "name");
let age: number = getProperty(person, "age");

let stringArray: string[] = createArray(3, "Hello");
let numberArray: number[] = createArray(5, 42);
let booleanArray: boolean[] = createArray(2, true);

// Generic class usage
let stringContainer: Container<string> = new Container("Hello");
let numberContainer: Container<number> = new Container(42);

let stringValue: string = stringContainer.getValue();
let numberValue: number = numberContainer.getValue();

stringContainer.setValue("World");
numberContainer.setValue(100);

// Generic pair usage
let nameAgePair: Pair<string, number> = new Pair("Alice", 25);
let idNamePair: Pair<number, string> = new Pair(1, "Bob");

let firstName: string = nameAgePair.getFirst();
let firstAge: number = nameAgePair.getSecond();

// Generic repository usage
let userRepo: Repository<{ id: number; name: string }> = new Repository();
userRepo.add({ id: 1, name: "Alice" });
userRepo.add({ id: 2, name: "Bob" });

let user: { id: number; name: string } | undefined = userRepo.findById(1);
let allUsers: { id: number; name: string }[] = userRepo.getAll();

// Generic interface usage
let stringProcessor: Processor<string> = {
    process: (input: string) => input.toUpperCase()
};

let numberProcessor: Processor<number> = {
    process: (input: number) => input * 2
};

let processedString: string = stringProcessor.process("hello");
let processedNumber: number = numberProcessor.process(21);

// Generic type alias usage
let optionalString: Optional<string> = "Hello";
let optionalNumber: Optional<number> = null;

let successResult: Result<number, string> = {
    success: true,
    data: 42
};

let errorResult: Result<number, string> = {
    success: false,
    error: "Something went wrong"
};

// Generic function with conditional types usage
let processedValue1: string = processValue("hello");
let processedValue2: number = processValue(21);
let processedValue3: unknown = processValue(true);

// Generic utility functions usage
let readonlyPerson = createReadonly({ name: "Alice", age: 30 });
let partialPerson = makePartial({ name: "Bob", age: 25, city: "NYC" });
let requiredPerson = makeRequired({ name: "Charlie", age: 35 });

// Generic static methods usage
let sum: number = MathUtils.add(5, 3);
let product: number = MathUtils.multiply(4, 7);
let maximum: number = MathUtils.max(10, 20);

// Generic inheritance usage
class User extends BaseEntity {
    name: string;
    email: string;

    constructor(id: number, name: string, email: string) {
        super(id);
        this.name = name;
        this.email = email;
    }
}

let userRepository: GenericRepository<User> = new GenericRepository();
userRepository.add(new User(1, "Alice", "alice@example.com"));
userRepository.add(new User(2, "Bob", "bob@example.com"));

let foundUser: User | undefined = userRepository.findById(1);
let allUsers2: User[] = userRepository.getAll();

// Generic method overloading usage
let processor: DataProcessor<string> = new DataProcessor();
let singleResult: string = processor.process("Hello");
let arrayResult: string[] = processor.process(["Hello", "World"]);

// Generic rest parameters usage
let combined: number[] = combine(1, 2, 3, 4, 5);
let combinedStrings: string[] = combine("a", "b", "c");

// Generic spread usage
let original: number[] = [1, 2, 3];
let spreaded: number[] = spread(original);

// Generic async usage
async function example() {
    let data: { name: string; age: number } = await fetchData<{ name: string; age: number }>("https://api.example.com/user");
    // Use the fetched data
}

// This example demonstrates comprehensive generic functionality