// Intermediate TypeScript Features
// Demonstrates optional parameters, readonly, utility types, and more

// 11. Optional & Default Parameters
function greet(name: string, age?: number): string {
    return age ? `Hello ${name}, age ${age}` : `Hello ${name}`;
}

function multiply(a: number, b: number = 2): number {
    return a * b;
}

function createUser(name: string, options?: { age?: number; email?: string }): object {
    return {
        name,
        age: options?.age || 0,
        email: options?.email || "unknown@example.com"
    };
}

// 12. Readonly & Const Assertions
interface Car {
    readonly brand: string;
    model: string;
    year: number;
}

const myCar: Car = {brand: "Tesla", model: "Model 3", year: 2023};
// myCar.brand = "Ford"; // This would be an error in full TypeScript

const arr = [10, 20] as const; // tuple & readonly
const config = {
    apiUrl: "https://api.example.com",
    timeout: 5000
} as const;

// 13. Type Aliases with Utility Types
type Person = {
    id: number;
    name: string;
    age?: number;
    email: string;
};

// Manual implementation of utility types
type Partial<T> = {
    [P in keyof T]?: T[P];
};

type Required<T> = {
    [P in keyof T]-?: T[P];
};

type Readonly<T> = {
    readonly [P in keyof T]: T[P];
};

type Pick<T, K extends keyof T> = {
    [P in K]: T[P];
};

type Omit<T, K extends keyof T> = {
    [P in Exclude<keyof T, K>]: T[P];
};

// Using utility types
type PartialPerson = Partial<Person>; // all props optional
type ReadonlyPerson = Readonly<Person>; // all props readonly
type RequiredPerson = Required<Person>; // all props required
type PickPerson = Pick<Person, 'id' | 'name'>; // pick specific props
type OmitPerson = Omit<Person, 'age'>; // omit specific props

// 14. Keyof & Index Types
interface User {
    id: number;
    name: string;
    email: string;
    isActive: boolean;
}

type UserKeys = keyof User; // "id" | "name" | "email" | "isActive"

function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
    return obj[key];
}

function setProperty<T, K extends keyof T>(obj: T, key: K, value: T[K]): void {
    obj[key] = value;
}

const user: User = {id: 1, name: "Alice", email: "alice@example.com", isActive: true};
let nameValue: string = getProperty(user, "name");
let idValue: number = getProperty(user, "id");
setProperty(user, "isActive", false);

// 15. Mapped Types
type OptionsFlags<T> = {
    [Property in keyof T]: boolean;
};

type Features = {
    darkMode: () => void;
    newUserProfile: () => void;
    notifications: () => void;
};

type FeatureOptions = OptionsFlags<Features>;
// { darkMode: boolean; newUserProfile: boolean; notifications: boolean }

// More complex mapped types
type Stringify<T> = {
    [K in keyof T]: string;
};

type Optionalize<T> = {
    [K in keyof T]?: T[K];
};

type MakeReadonly<T> = {
    readonly [K in keyof T]: T[K];
};

// 16. Template Literal Types (Basic)
type EventName<T extends string> = `on${Capitalize<T>}`;
type CSSProperty = `--${string}`;

type ClickEvent = EventName<"click">; // "onClick"
type HoverEvent = EventName<"hover">; // "onHover"
type CustomCSS = CSSProperty; // "--anything"

// 17. Advanced Generics
function merge<T extends object, U extends object>(obj1: T, obj2: U): T & U {
    return {...obj1, ...obj2};
}

const combined = merge({name: "Alice"}, {age: 25});

// Generic constraints
interface Lengthwise {
    length: number;
}

function loggingIdentity<T extends Lengthwise>(arg: T): T {
    _print(arg.length);
    return arg;
}

// Multiple constraints
interface HasId {
    id: number;
}

interface HasName {
    name: string;
}

function processEntity<T extends HasId & HasName>(entity: T): T {
    _print(`Processing ${entity.name} with ID ${entity.id}`);
    return entity;
}

// 18. Namespaces
namespace Utils {
    export function log(msg: string): void {
        _print("Log:", msg);
    }

    export function formatDate(date: Date): string {
        return date.toISOString();
    }

    export namespace Math {
        export function add(a: number, b: number): number {
            return a + b;
        }

        export function multiply(a: number, b: number): number {
            return a * b;
        }

        export const PI = 3.14159;
    }

    export namespace String {
        export function capitalize(str: string): string {
            return str.charAt(0).toUpperCase() + str.slice(1);
        }

        export function truncate(str: string, length: number): string {
            return str.length > length ? str.slice(0, length) + "..." : str;
        }
    }
}

// Using namespaces
Utils.log("Hello from namespace");
let sum = Utils.Math.add(5, 3);
let product = Utils.Math.multiply(4, 7);
let capitalized = Utils.String.capitalize("hello");
let truncated = Utils.String.truncate("This is a long string", 10);

// 19. Modules & Imports (Concept)
// Note: TSC compiler may not fully support ES modules yet
// This shows the pattern that would work in full TypeScript

// Simulated module exports
let MathModule = {
    add: (x: number, y: number): number => x + y,
    subtract: (x: number, y: number): number => x - y,
    multiply: (x: number, y: number): number => x * y,
    divide: (x: number, y: number): number => x / y,
    PI: 3.14159
};

let StringModule = {
    capitalize: (str: string): string => str.charAt(0).toUpperCase() + str.slice(1),
    lowercase: (str: string): string => str.toLowerCase(),
    uppercase: (str: string): string => str.toUpperCase()
};

// 20. Destructuring & Spread
let numbers: number[] = [1, 2, 3, 4, 5];

// Array destructuring
let [first, second, ...rest] = numbers;
let [head, , third, ...tail] = numbers;

// Object destructuring
let person = {
    name: "Alice",
    age: 30,
    city: "New York",
    country: "USA"
};

let {name, age, ...otherInfo} = person;
let {name: fullName, age: userAge} = person;

// Spread operator
let arr1: number[] = [1, 2, 3];
let arr2: number[] = [4, 5, 6];
let combined: number[] = [...arr1, ...arr2];

let obj1 = {a: 1, b: 2};
let obj2 = {c: 3, d: 4};
let merged = {...obj1, ...obj2};

// Destructuring in function parameters
function processUser({name, age, ...rest}: { name: string; age: number; [key: string]: any }): string {
    return `Processing ${name} (${age}) with additional data: ${Object.keys(rest).length} properties`;
}

let userInfo = processUser({name: "Bob", age: 25, city: "LA", country: "USA"});

// Usage examples
let greeting1 = greet("Alice");
let greeting2 = greet("Bob", 30);
let product1 = multiply(5);
let product2 = multiply(5, 3);

let newUser = createUser("Charlie", {age: 28, email: "charlie@example.com"});

let featureFlags: FeatureOptions = {
    darkMode: true,
    newUserProfile: false,
    notifications: true
};

let stringifiedPerson: Stringify<Person> = {
    id: "1",
    name: "Alice",
    age: "30",
    email: "alice@example.com"
};

let partialUser: PartialPerson = {
    name: "Alice"
    // Other properties are optional
};

let readonlyUser: ReadonlyPerson = {
    id: 1,
    name: "Alice",
    age: 30,
    email: "alice@example.com"
};

let pickedUser: PickPerson = {
    id: 1,
    name: "Alice"
};

let omittedUser: OmitPerson = {
    id: 1,
    name: "Alice",
    email: "alice@example.com"
    // age is omitted
};

// This example demonstrates comprehensive intermediate TypeScript features