// Destructuring
// Demonstrates object and array destructuring patterns

// Array destructuring
let numbers: number[] = [1, 2, 3, 4, 5];

// Basic array destructuring
let [first, second, third] = numbers;
let [a, b, c, d, e] = numbers;

// Array destructuring with rest operator
let [head, ...tail] = numbers;
let [firstTwo, ...remaining] = numbers;

// Array destructuring with default values
let [x = 0, y = 0, z = 0] = [10, 20];
let [p, q, r = 100] = [1, 2];

// Array destructuring with skipping elements
let [firstElement, , thirdElement] = numbers;
let [start, , , end] = numbers;

// Array destructuring in function parameters
function processArray([first, second, ...rest]: number[]): number {
    return first + second + rest.length;
}

let result: number = processArray([1, 2, 3, 4, 5]);

// Object destructuring
let person: object = {
    name: "Alice",
    age: 30,
    city: "New York",
    country: "USA",
    email: "alice@example.com"
};

// Basic object destructuring
let { name, age, city } = person;
let { email, country } = person;

// Object destructuring with different variable names
let { name: personName, age: personAge } = person;
let { city: location, country: nation } = person;

// Object destructuring with default values
let { name: userName = "Unknown", age: userAge = 0, phone = "N/A" } = person;

// Object destructuring with rest operator
let { name: fullName, ...otherInfo } = person;

// Object destructuring in function parameters
function processPerson({ name, age, city }: { name: string; age: number; city: string }): string {
    return `${name} (${age}) from ${city}`;
}

let personInfo: string = processPerson(person);

// Nested object destructuring
let complexObject: object = {
    user: {
        personal: {
            firstName: "John",
            lastName: "Doe"
        },
        contact: {
            email: "john@example.com",
            phone: "123-456-7890"
        }
    },
    settings: {
        theme: "dark",
        notifications: true
    }
};

let { 
    user: { 
        personal: { firstName, lastName },
        contact: { email: userEmail }
    },
    settings: { theme, notifications }
} = complexObject;

// Nested array destructuring
let matrix: number[][] = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
];

let [firstRow, secondRow, thirdRow] = matrix;
let [[a1, a2, a3], [b1, b2, b3], [c1, c2, c3]] = matrix;

// Destructuring with computed property names
let propertyName: string = "name";
let { [propertyName]: computedName } = person;

// Destructuring with function return values
function getCoordinates(): [number, number] {
    return [10, 20];
}

let [x, y] = getCoordinates();

function getUserInfo(): { id: number; name: string; email: string } {
    return {
        id: 1,
        name: "Bob",
        email: "bob@example.com"
    };
}

let { id, name: userName2, email: userEmail2 } = getUserInfo();

// Destructuring in loops
let users: object[] = [
    { name: "Alice", age: 25 },
    { name: "Bob", age: 30 },
    { name: "Charlie", age: 35 }
];

for (let { name, age } of users) {
    _print(`${name} is ${age} years old`);
}

// Destructuring with type annotations
let [num1, num2]: [number, number] = [10, 20];
let { title, author }: { title: string; author: string } = { title: "Book", author: "Author" };

// Destructuring with optional chaining (concept)
let optionalObject: object | null = {
    data: {
        value: 42
    }
};

// Manual optional chaining with destructuring
let dataValue: number | undefined = optionalObject ? optionalObject.data ? optionalObject.data.value : undefined : undefined;

// Destructuring with validation
function validateAndDestructure(data: any): { name: string; age: number } | null {
    if (data && typeof data.name === "string" && typeof data.age === "number") {
        let { name, age } = data;
        return { name, age };
    }
    return null;
}

let validData: { name: string; age: number } | null = validateAndDestructure(person);

// Destructuring with error handling
function safeDestructure(data: any): { success: boolean; result?: any; error?: string } {
    try {
        let { name, age, city } = data;
        return { success: true, result: { name, age, city } };
    } catch (error) {
        return { success: false, error: "Destructuring failed" };
    }
}

let destructureResult: { success: boolean; result?: any; error?: string } = safeDestructure(person);

// Destructuring with mapping
let items: object[] = [
    { id: 1, name: "Item 1", price: 10.99 },
    { id: 2, name: "Item 2", price: 20.99 },
    { id: 3, name: "Item 3", price: 30.99 }
];

let itemNames: string[] = items.map(({ name }) => name);
let itemPrices: number[] = items.map(({ price }) => price);

// Destructuring with filtering
let expensiveItems: object[] = items.filter(({ price }) => price > 20);

// Destructuring with sorting
let sortedItems: object[] = items.sort(({ price: a }, { price: b }) => a - b);

// Destructuring with reduce
let totalPrice: number = items.reduce((sum, { price }) => sum + price, 0);

// Destructuring with spread operator
let [firstItem, ...otherItems] = items;
let { name: firstName, ...otherProperties } = person;

// Destructuring with nested arrays
let nestedArray: number[][][] = [
    [[1, 2], [3, 4]],
    [[5, 6], [7, 8]]
];

let [[[a1, a2], [a3, a4]], [[b1, b2], [b3, b4]]] = nestedArray;

// Destructuring with mixed types
let mixedArray: (string | number)[] = ["hello", 42, "world", 100];
let [str1, num1, str2, num2] = mixedArray;

// Destructuring with function overloading
function processData(data: [string, number]): string;
function processData(data: { name: string; age: number }): string;
function processData(data: [string, number] | { name: string; age: number }): string {
    if (Array.isArray(data)) {
        let [name, age] = data;
        return `${name} is ${age} years old`;
    } else {
        let { name, age } = data;
        return `${name} is ${age} years old`;
    }
}

let result1: string = processData(["Alice", 25]);
let result2: string = processData({ name: "Bob", age: 30 });

// Destructuring with class properties
class Point {
    constructor(public x: number, public y: number) {}
    
    destructure(): [number, number] {
        return [this.x, this.y];
    }
}

let point: Point = new Point(10, 20);
let [px, py] = point.destructure();

// This example demonstrates comprehensive destructuring functionality