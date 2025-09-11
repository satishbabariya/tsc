# TypeScript Syntax Cheat Sheet
*Quick reference for TSC Compiler*

## 1. Basic Types

```typescript
let isDone: boolean = true;
let age: number = 25;
let username: string = "Satish";
let scores: number[] = [95, 80, 70];
let tuple: [string, number] = ["Score", 100];
```

## 2. Enums

```typescript
enum Direction {
  Up,
  Down,
  Left,
  Right
}

let move: Direction = Direction.Up;
```

## 3. Any, Unknown, Void, Never

```typescript
let data: any = "could be anything";
let value: unknown = 42;

function logMessage(msg: string): void {
  console.log(msg);
}

function throwError(msg: string): never {
  throw new Error(msg);
}
```

## 4. Functions

```typescript
function add(x: number, y: number): number {
  return x + y;
}

const multiply = (a: number, b: number): number => a * b;
```

## 5. Interfaces & Types

```typescript
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
```

## 6. Classes

```typescript
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
```

## 7. Generics

```typescript
function identity<T>(arg: T): T {
  return arg;
}

let num = identity<number>(42);
let str = identity<string>("Hello");
```

## 8. Union & Intersection Types

```typescript
let result: string | number;
result = "hello";
result = 100;

type A = { a: string };
type B = { b: number };
type AB = A & B;

const obj: AB = { a: "hi", b: 42 };
```

## 9. Type Assertions

```typescript
let someValue: unknown = "TypeScript";
let strLength: number = (someValue as string).length;
```

## 10. Modules & Imports

```typescript
// math.ts
export function add(x: number, y: number): number {
  return x + y;
}

// main.ts
import { add } from "./math";
console.log(add(2, 3));
```

## 11. Control Flow

```typescript
// If/Else
if (age >= 18) {
  console.log("Adult");
} else {
  console.log("Minor");
}

// Switch
switch (direction) {
  case Direction.Up:
    console.log("Going up");
    break;
  case Direction.Down:
    console.log("Going down");
    break;
  default:
    console.log("Unknown direction");
}

// Loops
for (let i = 0; i < 10; i++) {
  console.log(i);
}

for (let item of scores) {
  console.log(item);
}
```

## 12. Arrays & Objects

```typescript
// Arrays
let numbers: number[] = [1, 2, 3, 4, 5];
let matrix: number[][] = [[1, 2], [3, 4]];

// Objects
let person = {
  name: "Alice",
  age: 30,
  greet() {
    return `Hello, I'm ${this.name}`;
  }
};
```

## 13. Template Literals

```typescript
let name = "Alice";
let age = 30;
let message = `Hello, ${name}! You are ${age} years old.`;
```

## 14. Destructuring

```typescript
// Array destructuring
let [first, second, ...rest] = [1, 2, 3, 4, 5];

// Object destructuring
let { name, age } = person;
let { name: fullName, age: userAge } = person;
```

## 15. Optional Chaining & Nullish Coalescing

```typescript
let user = { profile: { name: "Alice" } };
let userName = user?.profile?.name ?? "Unknown";

let config = { timeout: 5000 };
let timeout = config.timeout ?? 3000;
```

## 16. Error Handling

```typescript
try {
  // risky operation
  let result = riskyFunction();
} catch (error) {
  console.error("Error:", error.message);
} finally {
  // cleanup
  console.log("Cleanup completed");
}
```

## 17. Utility Types

```typescript
type Partial<T> = { [P in keyof T]?: T[P] };
type Required<T> = { [P in keyof T]-?: T[P] };
type Pick<T, K extends keyof T> = { [P in K]: T[P] };
type Omit<T, K extends keyof T> = Pick<T, Exclude<keyof T, K>>;
```

## 18. Advanced Generics

```typescript
// Generic constraints
function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
  return obj[key];
}

// Conditional types
type NonNullable<T> = T extends null | undefined ? never : T;

// Mapped types
type Readonly<T> = { readonly [P in keyof T]: T[P] };
```

## 19. Decorators (Concept)

```typescript
function sealed(constructor: Function) {
  Object.seal(constructor);
  Object.seal(constructor.prototype);
}

@sealed
class BugReport {
  type = "report";
  title: string;
}
```

## 20. Async/Await

```typescript
async function fetchData(): Promise<string> {
  try {
    let response = await fetch("/api/data");
    let data = await response.json();
    return data;
  } catch (error) {
    throw new Error("Failed to fetch data");
  }
}
```

---

## 🎯 TSC Compiler Status

| Feature | Status | Notes |
|---------|--------|-------|
| Basic Types | ✅ | All primitive types supported |
| Variables | ✅ | `let`, `const`, `var` with annotations |
| Functions | ✅ | Declarations, calls, parameters |
| Classes | ⚠️ | Basic structure, method calls have issues |
| Interfaces | ✅ | Declarations and implementation |
| Enums | ✅ | All enum types and operations |
| Generics | ✅ | Functions, classes, constraints |
| Control Flow | ✅ | If/else, loops, switch |
| Arrays/Objects | ✅ | Literals, indexing, operations |
| Error Handling | ⚠️ | Basic structure, full implementation pending |

## 🚀 Quick Compile & Run

```bash
# Compile
./tsc examples/01_basic_types.ts

# Run
./a.out
```

*This cheat sheet covers the essential TypeScript syntax supported by the TSC compiler.*