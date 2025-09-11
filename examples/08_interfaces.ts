// Interfaces
// Demonstrates interface declarations, implementation, and usage

// Basic interface
interface User {
    id: number;
    name: string;
    email: string;
    isActive: boolean;
}

// Interface with optional properties
interface Product {
    id: number;
    name: string;
    price: number;
    description?: string;  // Optional property
    category?: string;     // Optional property
}

// Interface with readonly properties
interface Point {
    readonly x: number;
    readonly y: number;
}

// Interface with method signatures
interface Drawable {
    draw(): void;
    getArea(): number;
}

// Interface extending another interface
interface Employee extends User {
    employeeId: string;
    department: string;
    salary: number;
}

// Interface with multiple inheritance
interface Manager extends Employee {
    teamSize: number;
    manageTeam(): void;
}

// Interface with function signatures
interface MathOperation {
    (a: number, b: number): number;
}

// Interface with index signatures
interface Dictionary {
    [key: string]: string;
}

// Interface with hybrid types (object and function)
interface Counter {
    (): void;
    count: number;
    reset(): void;
}

// Interface with generic types
interface Repository<T> {
    findById(id: number): T | null;
    save(entity: T): void;
    delete(id: number): boolean;
}

// Interface with optional method signatures
interface Logger {
    log(message: string): void;
    error?(message: string): void;  // Optional method
    warn?(message: string): void;   // Optional method
}

// Interface with call signatures
interface EventHandler {
    (event: string, data: any): void;
}

// Interface with constructor signatures
interface UserConstructor {
    new (name: string, email: string): User;
}

// Class implementing interface
class BasicUser implements User {
    id: number;
    name: string;
    email: string;
    isActive: boolean;

    constructor(id: number, name: string, email: string) {
        this.id = id;
        this.name = name;
        this.email = email;
        this.isActive = true;
    }
}

// Class implementing multiple interfaces
class Rectangle implements Drawable {
    width: number;
    height: number;

    constructor(width: number, height: number) {
        this.width = width;
        this.height = height;
    }

    draw(): void {
        // Drawing logic
    }

    getArea(): number {
        return this.width * height;
    }
}

// Class implementing interface with optional methods
class ConsoleLogger implements Logger {
    log(message: string): void {
        console.log(message);
    }

    error(message: string): void {
        console.error(message);
    }

    warn(message: string): void {
        console.warn(message);
    }
}

// Function implementing interface
let addOperation: MathOperation = function(a: number, b: number): number {
    return a + b;
};

let multiplyOperation: MathOperation = function(a: number, b: number): number {
    return a * b;
};

// Object implementing interface
let userDict: Dictionary = {
    "admin": "Administrator",
    "user": "Regular User",
    "guest": "Guest User"
};

// Function with interface parameter
function processUser(user: User): string {
    return "Processing user: " + user.name + " (" + user.email + ")";
}

// Function with interface return type
function createUser(name: string, email: string): User {
    return {
        id: Math.floor(Math.random() * 1000),
        name: name,
        email: email,
        isActive: true
    };
}

// Generic function with interface constraint
function processRepository<T>(repo: Repository<T>, id: number): T | null {
    return repo.findById(id);
}

// Interface with method overloading
interface StringProcessor {
    process(value: string): string;
    process(value: number): string;
}

class MyStringProcessor implements StringProcessor {
    process(value: string | number): string {
        if (typeof value === "string") {
            return value.toUpperCase();
        } else {
            return value.toString();
        }
    }
}

// Interface with nested interfaces
interface Company {
    name: string;
    address: Address;
    employees: Employee[];
}

interface Address {
    street: string;
    city: string;
    state: string;
    zipCode: string;
}

// Interface with union types
interface FlexibleConfig {
    timeout: number | string;
    retries: number | boolean;
    debug: boolean | string;
}

// Interface with intersection types (concept)
interface Timestamped {
    createdAt: Date;
    updatedAt: Date;
}

interface Identifiable {
    id: number;
}

// Type that combines interfaces
type TimestampedUser = User & Timestamped;

// Usage examples
let user1: User = {
    id: 1,
    name: "Alice",
    email: "alice@example.com",
    isActive: true
};

let product1: Product = {
    id: 1,
    name: "Laptop",
    price: 999.99,
    description: "High-performance laptop"
    // category is optional, so we can omit it
};

let point1: Point = { x: 10, y: 20 };
// point1.x = 30; // Error: Cannot assign to readonly property

let employee1: Employee = {
    id: 1,
    name: "Bob",
    email: "bob@example.com",
    isActive: true,
    employeeId: "EMP001",
    department: "Engineering",
    salary: 75000
};

let manager1: Manager = {
    id: 2,
    name: "Carol",
    email: "carol@example.com",
    isActive: true,
    employeeId: "EMP002",
    department: "Engineering",
    salary: 95000,
    teamSize: 5,
    manageTeam: function(): void {
        // Management logic
    }
};

// Using interfaces with functions
let result1: number = addOperation(5, 3);
let result2: number = multiplyOperation(4, 7);

let userInfo: string = processUser(user1);
let newUser: User = createUser("David", "david@example.com");

// Using classes that implement interfaces
let basicUser: BasicUser = new BasicUser(1, "Eve", "eve@example.com");
let rectangle: Rectangle = new Rectangle(10, 5);
let logger: ConsoleLogger = new ConsoleLogger();

rectangle.draw();
let area: number = rectangle.getArea();

logger.log("Application started");
logger.error("Something went wrong");
logger.warn("Warning message");

// Using generic interfaces
class UserRepository implements Repository<User> {
    private users: User[] = [];

    findById(id: number): User | null {
        for (let user of this.users) {
            if (user.id === id) {
                return user;
            }
        }
        return null;
    }

    save(entity: User): void {
        this.users.push(entity);
    }

    delete(id: number): boolean {
        for (let i = 0; i < this.users.length; i++) {
            if (this.users[i].id === id) {
                this.users.splice(i, 1);
                return true;
            }
        }
        return false;
    }
}

let userRepo: Repository<User> = new UserRepository();
userRepo.save(user1);
let foundUser: User | null = userRepo.findById(1);

// This example demonstrates comprehensive interface functionality