// Quick Start Example
// Demonstrates essential TypeScript syntax patterns

// 1. Basic Types
let isDone: boolean = true;
let age: number = 25;
let username: string = "Satish";
let scores: number[] = [95, 80, 70];
let tuple: [string, number] = ["Score", 100];

// 2. Enums
enum Direction {
  Up,
  Down,
  Left,
  Right
}

let move: Direction = Direction.Up;

// 3. Any, Unknown, Void, Never
let data: any = "could be anything";
let value: unknown = 42;

function logMessage(msg: string): void {
  console.log(msg);
}

function throwError(msg: string): never {
  throw new Error(msg);
}

// 4. Functions
function add(x: number, y: number): number {
  return x + y;
}

const multiply = (a: number, b: number): number => a * b;

// 5. Interfaces & Types
interface User {
  id: number;
  name: string;
  isAdmin?: boolean; // optional
}

type Point = {
  x: number;
  y: number;
};

const user: User = { id: 1, name: "Alice" };

// 6. Classes
class Animal {
  constructor(public name: string) {}

  move(distance: number = 0): void {
    console.log(`${this.name} moved ${distance}m`);
  }
}

class Dog extends Animal {
  bark(): void {
    console.log("Woof!");
  }
}

const dog = new Dog("Buddy");
dog.bark();
dog.move(10);

// 7. Generics
function identity<T>(arg: T): T {
  return arg;
}

let num = identity<number>(42);
let str = identity<string>("Hello");

// 8. Union & Intersection Types
let result: string | number;
result = "hello";
result = 100;

type A = { a: string };
type B = { b: number };
type AB = A & B;

const obj: AB = { a: "hi", b: 42 };

// 9. Type Assertions
let someValue: unknown = "TypeScript";
let strLength: number = (someValue as string).length;

// 10. Control Flow
if (age >= 18) {
  console.log("Adult");
} else {
  console.log("Minor");
}

switch (move) {
  case Direction.Up:
    console.log("Going up");
    break;
  case Direction.Down:
    console.log("Going down");
    break;
  default:
    console.log("Unknown direction");
}

for (let i = 0; i < 10; i++) {
  console.log(i);
}

for (let score of scores) {
  console.log(score);
}

// 11. Arrays & Objects
let numbers: number[] = [1, 2, 3, 4, 5];
let matrix: number[][] = [[1, 2], [3, 4]];

let person = {
  name: "Alice",
  age: 30,
  greet() {
    return `Hello, I'm ${this.name}`;
  }
};

// 12. Template Literals
let name = "Alice";
let userAge = 30;
let message = `Hello, ${name}! You are ${userAge} years old.`;

// 13. Destructuring
let [first, second, ...rest] = [1, 2, 3, 4, 5];
let { name: fullName, age: userAge2 } = person;

// 14. Error Handling
try {
  let result = add(5, 3);
  console.log("Result:", result);
} catch (error) {
  console.error("Error:", error.message);
} finally {
  console.log("Cleanup completed");
}

// Usage examples
logMessage("TypeScript Quick Start");
let sum = add(5, 3);
let product = multiply(4, 7);
let identified = identity("TypeScript");
let greeting = person.greet();

console.log("Sum:", sum);
console.log("Product:", product);
console.log("Identified:", identified);
console.log("Greeting:", greeting);
console.log("Message:", message);
console.log("String length:", strLength);
console.log("First score:", first);
console.log("Full name:", fullName);