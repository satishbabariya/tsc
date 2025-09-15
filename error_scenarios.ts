
// Error scenarios testing
// Function with invalid parameter types
function processValue(value: string): string {
    return value.toUpperCase();
}

// These should cause type errors
// processValue(42); // Error: Argument of type 'number' is not assignable to parameter of type 'string'
// processValue(true); // Error: Argument of type 'boolean' is not assignable to parameter of type 'string'

// Function with missing required parameters
function createUser(name: string, age: number, email: string): User {
    return { name, age, email };
}

// These should cause errors
// createUser("Alice"); // Error: Expected 3 arguments, but got 1
// createUser("Alice", 30); // Error: Expected 3 arguments, but got 2

interface User {
    name: string;
    age: number;
    email: string;
}

// Function with invalid return type
function getStringValue(): string {
    return 42; // Error: Type 'number' is not assignable to type 'string'
}

// Function with invalid default parameter type
function processData(data: string, count: number = "invalid"): void {
    console.log(data, count);
}

// Function with invalid rest parameter type
function processItems(...items: string[]): void {
    items.forEach(item => console.log(item));
}

// These should cause errors
// processItems("hello", 42, "world"); // Error: Argument of type 'number' is not assignable to parameter of type 'string'

// Function with conflicting overloads
function conflict(value: string): string;
function conflict(value: string): number; // Error: Duplicate function implementation
function conflict(value: string): string | number {
    return value;
}

// Function with invalid arrow function syntax
// const invalidArrow = (a, b) => { return a + b; }; // Error: Parameter 'a' implicitly has an 'any' type

// Function with invalid async usage
async function asyncFunction(): Promise<string> {
    return "Hello";
}

// This should cause an error
// const result = asyncFunction(); // Error: 'await' expression is only allowed within an async function

// Function with invalid generic constraints
function processGeneric<T extends string>(value: T): T {
    return value;
}

// This should cause an error
// processGeneric(42); // Error: Argument of type 'number' is not assignable to parameter of type 'string'

// Function with invalid destructuring
function processObject({ name, age }: { name: string; age: number }): void {
    console.log(name, age);
}

// This should cause an error
// processObject({ name: "Alice" }); // Error: Property 'age' is missing

// Function with invalid method chaining
class InvalidBuilder {
    method1(): InvalidBuilder {
        return this;
    }
    
    method2(): string { // Error: Method 'method2' of type 'InvalidBuilder' is not assignable to the same method in base type 'InvalidBuilder'
        return "invalid";
    }
}

// Function with invalid callback types
function withCallback(callback: (value: string) => void): void {
    callback("hello");
}

// This should cause an error
// withCallback((value: number) => console.log(value)); // Error: Argument of type '(value: number) => void' is not assignable to parameter of type '(value: string) => void'
