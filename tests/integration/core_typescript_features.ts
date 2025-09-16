// Core TypeScript features regression test
function testBasicTypes(): void {
    const stringVar: string = "hello";
    const numberVar: number = 42;
    const booleanVar: boolean = true;
    const arrayVar: number[] = [1, 2, 3];
    const objectVar: { name: string; age: number } = {name: "Alice", age: 30};

    console.log("Basic types test passed");
}

interface User {
    id: string;
    name: string;
    email: string;
    age?: number;
}

function testInterfaces(): void {
    const user: User = {
        id: "1",
        name: "Alice",
        email: "alice@example.com"
    };

    console.log("Interfaces test passed");
}

class Person {
    constructor(public name: string, public age: number) {
    }

    greet(): string {
        return `Hello, I'm ${this.name} and I'm ${this.age} years old`;
    }
}

function testClasses(): void {
    const person = new Person("Alice", 30);
    console.log(person.greet());
    console.log("Classes test passed");
}

function identity<T>(arg: T): T {
    return arg;
}

function testGenerics(): void {
    const stringResult = identity("hello");
    const numberResult = identity(42);
    console.log("Generics test passed");
}

enum Color {
    Red = "red",
    Green = "green",
    Blue = "blue"
}

function testEnums(): void {
    const color: Color = Color.Red;
    console.log("Enums test passed");
}

type StringOrNumber = string | number;

function testUnionTypes(): void {
    const stringOrNumber: StringOrNumber = "hello";
    console.log("Union types test passed");
}

type MathOperation = (a: number, b: number) => number;

function testFunctionTypes(): void {
    const add: MathOperation = (a, b) => a + b;
    const result = add(5, 3);
    console.log("Function types test passed");
}

interface Config {
    readonly apiUrl: string;
    timeout?: number;
}

function testOptionalReadonly(): void {
    const config: Config = {
        apiUrl: "https://api.example.com"
    };
    console.log("Optional and readonly test passed");
}

console.log("=== Core TypeScript Features Regression Test ===");
testBasicTypes();
testInterfaces();
testClasses();
testGenerics();
testEnums();
testUnionTypes();
testFunctionTypes();
testOptionalReadonly();
console.log("All core TypeScript features tests passed!");
