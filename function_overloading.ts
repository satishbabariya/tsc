
// Function overloading testing
function processValue(value: string): string;
function processValue(value: number): number;
function processValue(value: boolean): boolean;
function processValue(value: any): any {
    if (typeof value === "string") {
        return value.toUpperCase();
    } else if (typeof value === "number") {
        return value * 2;
    } else if (typeof value === "boolean") {
        return !value;
    }
    return value;
}

// Test function overloading
console.log("String:", processValue("hello"));
console.log("Number:", processValue(42));
console.log("Boolean:", processValue(true));

// Function overloading with different parameter counts
function createUser(name: string): User;
function createUser(name: string, age: number): User;
function createUser(name: string, age: number, email: string): User;
function createUser(name: string, age?: number, email?: string): User {
    return {
        name,
        age: age || 0,
        email: email || "no-email"
    };
}

interface User {
    name: string;
    age: number;
    email: string;
}

// Test different parameter counts
const user1 = createUser("Alice");
const user2 = createUser("Bob", 30);
const user3 = createUser("Charlie", 25, "charlie@example.com");

console.log("User 1:", user1);
console.log("User 2:", user2);
console.log("User 3:", user3);

// Function overloading with union types
function formatValue(value: string | number): string;
function formatValue(value: boolean): string;
function formatValue(value: any): string {
    if (typeof value === "string") {
        return `"${value}"`;
    } else if (typeof value === "number") {
        return value.toString();
    } else if (typeof value === "boolean") {
        return value ? "true" : "false";
    }
    return String(value);
}

console.log("Formatted string:", formatValue("hello"));
console.log("Formatted number:", formatValue(42));
console.log("Formatted boolean:", formatValue(true));
