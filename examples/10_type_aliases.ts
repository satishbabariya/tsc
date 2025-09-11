// Type Aliases
// Demonstrates type alias declarations and usage

// Basic type aliases
type UserId = number;
type UserName = string;
type IsActive = boolean;

// Type alias for object types
type Point = {
    x: number;
    y: number;
};

type Person = {
    id: UserId;
    name: UserName;
    email: string;
    isActive: IsActive;
};

// Type alias for function types
type MathOperation = (a: number, b: number) => number;
type StringProcessor = (input: string) => string;
type VoidFunction = () => void;

// Type alias for array types
type NumberArray = number[];
type StringArray = string[];
type PersonArray = Person[];

// Type alias for union types
type Status = "pending" | "approved" | "rejected" | "cancelled";
type Size = "small" | "medium" | "large" | "extra-large";
type Theme = "light" | "dark" | "auto";

// Type alias for intersection types
type Timestamped = {
    createdAt: Date;
    updatedAt: Date;
};

type Identifiable = {
    id: number;
};

type TimestampedPerson = Person & Timestamped;

// Type alias for generic types
type Container<T> = {
    value: T;
    isEmpty: boolean;
};

type Optional<T> = T | null | undefined;

type Result<T, E> = {
    success: true;
    data: T;
} | {
    success: false;
    error: E;
};

// Type alias for complex object types
type UserProfile = {
    id: UserId;
    personalInfo: {
        firstName: string;
        lastName: string;
        dateOfBirth: Date;
    };
    contactInfo: {
        email: string;
        phone?: string;
        address?: {
            street: string;
            city: string;
            country: string;
        };
    };
    preferences: {
        theme: Theme;
        notifications: boolean;
        language: string;
    };
};

// Type alias for function signatures
type EventHandler<T> = (event: T) => void;
type AsyncFunction<T, R> = (input: T) => Promise<R>;
type Predicate<T> = (item: T) => boolean;

// Type alias for utility types
type Partial<T> = {
    [P in keyof T]?: T[P];
};

type Required<T> = {
    [P in keyof T]-?: T[P];
};

type Readonly<T> = {
    readonly [P in keyof T]: T[P];
};

// Type alias for mapped types
type Stringify<T> = {
    [K in keyof T]: string;
};

type Optionalize<T> = {
    [K in keyof T]?: T[K];
};

// Type alias for conditional types
type NonNullable<T> = T extends null | undefined ? never : T;
type Flatten<T> = T extends (infer U)[] ? U : T;

// Type alias for recursive types
type JSONValue = 
    | string
    | number
    | boolean
    | null
    | JSONValue[]
    | { [key: string]: JSONValue };

// Type alias for template literal types
type EventName<T extends string> = `on${Capitalize<T>}`;
type CSSProperty = `--${string}`;

// Usage examples
let userId: UserId = 123;
let userName: UserName = "Alice";
let isUserActive: IsActive = true;

let point: Point = { x: 10, y: 20 };
let person: Person = {
    id: userId,
    name: userName,
    email: "alice@example.com",
    isActive: isUserActive
};

// Function type usage
let add: MathOperation = (a: number, b: number): number => a + b;
let multiply: MathOperation = (a: number, b: number): number => a * b;
let toUpperCase: StringProcessor = (input: string): string => input.toUpperCase();
let logMessage: VoidFunction = (): void => { /* logging logic */ };

let result1: number = add(5, 3);
let result2: number = multiply(4, 7);
let upperString: string = toUpperCase("hello");
logMessage();

// Array type usage
let numbers: NumberArray = [1, 2, 3, 4, 5];
let names: StringArray = ["Alice", "Bob", "Charlie"];
let people: PersonArray = [person];

// Union type usage
let currentStatus: Status = "pending";
let currentSize: Size = "medium";
let currentTheme: Theme = "dark";

function handleStatus(status: Status): string {
    switch (status) {
        case "pending":
            return "Processing...";
        case "approved":
            return "Approved!";
        case "rejected":
            return "Rejected";
        case "cancelled":
            return "Cancelled";
        default:
            return "Unknown status";
    }
}

let statusMessage: string = handleStatus(currentStatus);

// Intersection type usage
let timestampedPerson: TimestampedPerson = {
    id: 1,
    name: "Bob",
    email: "bob@example.com",
    isActive: true,
    createdAt: new Date(),
    updatedAt: new Date()
};

// Generic type usage
let numberContainer: Container<number> = {
    value: 42,
    isEmpty: false
};

let stringContainer: Container<string> = {
    value: "Hello",
    isEmpty: false
};

let optionalNumber: Optional<number> = 42;
let optionalString: Optional<string> = null;

let successResult: Result<number, string> = {
    success: true,
    data: 42
};

let errorResult: Result<number, string> = {
    success: false,
    error: "Something went wrong"
};

// Complex object type usage
let userProfile: UserProfile = {
    id: 1,
    personalInfo: {
        firstName: "Alice",
        lastName: "Smith",
        dateOfBirth: new Date("1990-01-01")
    },
    contactInfo: {
        email: "alice@example.com",
        phone: "+1234567890",
        address: {
            street: "123 Main St",
            city: "New York",
            country: "USA"
        }
    },
    preferences: {
        theme: "dark",
        notifications: true,
        language: "en"
    }
};

// Function signature usage
let clickHandler: EventHandler<string> = (event: string): void => {
    console.log("Clicked: " + event);
};

let asyncProcessor: AsyncFunction<number, string> = async (input: number): Promise<string> => {
    return "Processed: " + input;
};

let isEven: Predicate<number> = (item: number): boolean => item % 2 === 0;

// Utility type usage
let partialPerson: Partial<Person> = {
    name: "Charlie"
    // Other properties are optional
};

let requiredPerson: Required<Person> = {
    id: 1,
    name: "David",
    email: "david@example.com",
    isActive: true
};

let readonlyPerson: Readonly<Person> = {
    id: 1,
    name: "Eve",
    email: "eve@example.com",
    isActive: true
};

// Mapped type usage
let stringifiedPerson: Stringify<Person> = {
    id: "1",
    name: "Frank",
    email: "frank@example.com",
    isActive: "true"
};

// Conditional type usage
let nonNullString: NonNullable<string | null> = "Hello";
let flattenedArray: Flatten<number[]> = 42;

// Recursive type usage
let jsonData: JSONValue = {
    name: "Alice",
    age: 30,
    hobbies: ["reading", "coding"],
    address: {
        street: "123 Main St",
        city: "New York"
    }
};

// Template literal type usage
let clickEventName: EventName<"click"> = "onClick";
let hoverEventName: EventName<"hover"> = "onHover";
let customCSSProperty: CSSProperty = "--primary-color";

// This example demonstrates comprehensive type alias functionality