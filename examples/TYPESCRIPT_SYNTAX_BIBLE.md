# TypeScript Syntax Bible
*Complete reference from Beginner to Pro*

## 📚 Table of Contents

1. [Basic Level](#basic-level)
2. [Intermediate Level](#intermediate-level)
3. [Advanced Level](#advanced-level)
4. [TSC Compiler Status](#tsc-compiler-status)
5. [Quick Reference](#quick-reference)

---

## Basic Level

### 1. Basic Types

```typescript
let isDone: boolean = true;
let age: number = 25;
let username: string = "Satish";
let scores: number[] = [95, 80, 70];
let tuple: [string, number] = ["Score", 100];
```

### 2. Enums

```typescript
enum Direction {
  Up,
  Down,
  Left,
  Right
}

let move: Direction = Direction.Up;
```

### 3. Any, Unknown, Void, Never

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

### 4. Functions

```typescript
function add(x: number, y: number): number {
  return x + y;
}

const multiply = (a: number, b: number): number => a * b;
```

### 5. Interfaces & Types

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

### 6. Classes

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

### 7. Generics (Basic)

```typescript
function identity<T>(arg: T): T {
  return arg;
}

let num = identity<number>(42);
let str = identity<string>("Hello");
```

### 8. Union & Intersection Types

```typescript
let result: string | number;
result = "hello";
result = 100;

type A = { a: string };
type B = { b: number };
type AB = A & B;

const obj: AB = { a: "hi", b: 42 };
```

### 9. Type Assertions

```typescript
let someValue: unknown = "TypeScript";
let strLength: number = (someValue as string).length;
```

### 10. Control Flow

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

---

## Intermediate Level

### 11. Optional & Default Parameters

```typescript
function greet(name: string, age?: number) {
  return age ? `Hello ${name}, age ${age}` : `Hello ${name}`;
}

function multiply(a: number, b: number = 2) {
  return a * b;
}
```

### 12. Readonly & Const Assertions

```typescript
interface Car {
  readonly brand: string;
  model: string;
}

const myCar: Car = { brand: "Tesla", model: "Model 3" };
// myCar.brand = "Ford"; ❌ Error

const arr = [10, 20] as const; // tuple & readonly
```

### 13. Type Aliases with Utility Types

```typescript
type Person = {
  id: number;
  name: string;
  age?: number;
};

type PartialPerson = Partial<Person>; // all props optional
type ReadonlyPerson = Readonly<Person>; // all props readonly
type RequiredPerson = Required<Person>; // all props required
type PickPerson = Pick<Person, 'id' | 'name'>; // pick specific props
type OmitPerson = Omit<Person, 'age'>; // omit specific props
```

### 14. Keyof & Index Types

```typescript
interface User {
  id: number;
  name: string;
}

type UserKeys = keyof User; // "id" | "name"

function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
  return obj[key];
}

const u: User = { id: 1, name: "Alice" };
let nameValue = getProperty(u, "name");
```

### 15. Mapped Types

```typescript
type OptionsFlags<T> = {
  [Property in keyof T]: boolean;
};

type Features = {
  darkMode: () => void;
  newUserProfile: () => void;
};

type FeatureOptions = OptionsFlags<Features>;
// { darkMode: boolean; newUserProfile: boolean }

// Built-in mapped types
type Partial<T> = { [P in keyof T]?: T[P] };
type Required<T> = { [P in keyof T]-?: T[P] };
type Readonly<T> = { readonly [P in keyof T]: T[P] };
```

### 16. Template Literal Types

```typescript
type EventName<T extends string> = `on${Capitalize<T>}`;
type CSSProperty = `--${string}`;

type ClickEvent = EventName<"click">; // "onClick"
type HoverEvent = EventName<"hover">; // "onHover"
```

### 17. Advanced Generics

```typescript
function merge<T extends object, U extends object>(obj1: T, obj2: U): T & U {
  return { ...obj1, ...obj2 };
}

const combined = merge({ name: "Alice" }, { age: 25 });

// Generic constraints
interface Lengthwise {
  length: number;
}

function loggingIdentity<T extends Lengthwise>(arg: T): T {
  console.log(arg.length);
  return arg;
}
```

### 18. Namespaces

```typescript
namespace Utils {
  export function log(msg: string) {
    console.log("Log:", msg);
  }
  
  export namespace Math {
    export function add(a: number, b: number): number {
      return a + b;
    }
  }
}

Utils.log("Hello");
Utils.Math.add(5, 3);
```

### 19. Modules & Imports

```typescript
// math.ts
export function add(x: number, y: number): number {
  return x + y;
}

export const PI = 3.14159;

// main.ts
import { add, PI } from "./math";
import * as MathUtils from "./math";
console.log(add(2, 3));
```

### 20. Destructuring & Spread

```typescript
// Array destructuring
let [first, second, ...rest] = [1, 2, 3, 4, 5];

// Object destructuring
let { name, age } = person;
let { name: fullName, age: userAge } = person;

// Spread operator
let arr1 = [1, 2, 3];
let arr2 = [4, 5, 6];
let combined = [...arr1, ...arr2];

let obj1 = { a: 1, b: 2 };
let obj2 = { c: 3, d: 4 };
let merged = { ...obj1, ...obj2 };
```

---

## Advanced Level

### 21. Conditional Types

```typescript
type IsString<T> = T extends string ? true : false;

type A = IsString<string>; // true
type B = IsString<number>; // false

// More complex conditional types
type NonNullable<T> = T extends null | undefined ? never : T;
type Flatten<T> = T extends (infer U)[] ? U : T;
```

### 22. Infer Keyword

```typescript
type ReturnType<T> = T extends (...args: any[]) => infer R ? R : never;

function add(a: number, b: number) {
  return a + b;
}

type Result = ReturnType<typeof add>; // number

// More infer examples
type Parameters<T> = T extends (...args: infer P) => any ? P : never;
type ConstructorParameters<T> = T extends new (...args: infer P) => any ? P : never;
```

### 23. Recursive Types

```typescript
type JSONValue = 
  | string
  | number
  | boolean
  | null
  | JSONValue[]
  | { [key: string]: JSONValue };

// Recursive utility types
type DeepReadonly<T> = {
  readonly [P in keyof T]: T[P] extends object ? DeepReadonly<T[P]> : T[P];
};
```

### 24. Decorators (Experimental)

```typescript
function Logger(constructor: Function) {
  console.log(`Class created: ${constructor.name}`);
}

function sealed(constructor: Function) {
  Object.seal(constructor);
  Object.seal(constructor.prototype);
}

@Logger
@sealed
class MyService {
  @readonly
  name: string = "MyService";
}

function readonly(target: any, propertyKey: string) {
  // Property decorator implementation
}
```

### 25. Advanced Mapped Types

```typescript
// Make all properties optional and nullable
type PartialNullable<T> = {
  [P in keyof T]?: T[P] | null;
};

// Make all properties required and non-nullable
type RequiredNonNullable<T> = {
  [P in keyof T]-?: NonNullable<T[P]>;
};

// Create a type with specific property types
type Stringify<T> = {
  [K in keyof T]: string;
};
```

### 26. Template Literal Types (Advanced)

```typescript
type Join<K, P> = K extends string ? P extends string ? `${K}${P}` : never : never;

type Paths<T> = T extends object ? {
  [K in keyof T]-?: K extends string ? 
    `${K}` | Join<K, Paths<T[K]>> : never
}[keyof T] : '';

type ApiEndpoints = {
  users: {
    list: string;
    create: string;
    update: { id: string };
  };
  posts: {
    list: string;
    create: string;
  };
};

type AllPaths = Paths<ApiEndpoints>;
// "users" | "users.list" | "users.create" | "users.update" | "posts" | "posts.list" | "posts.create"
```

### 27. Branded Types

```typescript
type Brand<T, B> = T & { __brand: B };

type UserId = Brand<number, 'UserId'>;
type ProductId = Brand<number, 'ProductId'>;

function createUserId(id: number): UserId {
  return id as UserId;
}

function createProductId(id: number): ProductId {
  return id as ProductId;
}

// Prevents mixing up different ID types
let userId: UserId = createUserId(123);
let productId: ProductId = createProductId(456);
// let mixed: UserId = productId; // ❌ Error
```

### 28. Advanced Utility Types

```typescript
// Extract specific types from union
type Extract<T, U> = T extends U ? T : never;

// Exclude specific types from union
type Exclude<T, U> = T extends U ? never : T;

// Get function parameter types
type Parameters<T extends (...args: any) => any> = 
  T extends (...args: infer P) => any ? P : never;

// Get constructor parameter types
type ConstructorParameters<T extends new (...args: any) => any> = 
  T extends new (...args: infer P) => any ? P : never;

// Get return type of function
type ReturnType<T extends (...args: any) => any> = 
  T extends (...args: any) => infer R ? R : any;
```

### 29. Complex Generic Constraints

```typescript
// Constrain to object with specific keys
type HasName<T> = T extends { name: any } ? T : never;

// Constrain to array-like
type ArrayLike<T> = T extends { length: number; [n: number]: any } ? T : never;

// Constrain to function
type FunctionLike<T> = T extends (...args: any[]) => any ? T : never;

// Multiple constraints
function process<T extends object & { id: number }>(item: T): T {
  return item;
}
```

### 30. Advanced Type Manipulation

```typescript
// Get all possible values of a union
type UnionToIntersection<U> = 
  (U extends any ? (k: U) => void : never) extends (k: infer I) => void ? I : never;

// Convert union to tuple
type UnionToTuple<T> = UnionToIntersection<
  T extends any ? () => T : never
> extends () => infer R ? R : never;

// Get all keys of a type as a union
type Keys<T> = keyof T;

// Get all values of a type as a union
type Values<T> = T[keyof T];
```

---

## TSC Compiler Status

| Level | Feature | Status | Notes |
|-------|---------|--------|-------|
| **Basic** | Basic Types | ✅ | All primitive types supported |
| | Variables | ✅ | `let`, `const`, `var` with annotations |
| | Functions | ✅ | Declarations, calls, parameters |
| | Classes | ⚠️ | Basic structure, method calls have issues |
| | Interfaces | ✅ | Declarations and implementation |
| | Enums | ✅ | All enum types and operations |
| | Generics (Basic) | ✅ | Basic generic functions and classes |
| | Control Flow | ✅ | If/else, loops, switch |
| | Arrays/Objects | ✅ | Literals, indexing, operations |
| **Intermediate** | Optional Parameters | ✅ | Function parameters |
| | Readonly | ✅ | Interface properties |
| | Utility Types | ⚠️ | Partial implementation |
| | Keyof/Index Types | ⚠️ | Basic support |
| | Mapped Types | ⚠️ | Limited support |
| | Namespaces | ✅ | Basic namespace support |
| | Modules | ⚠️ | Import/export structure |
| **Advanced** | Conditional Types | ❌ | Not implemented |
| | Infer Keyword | ❌ | Not implemented |
| | Decorators | ❌ | Not implemented |
| | Template Literals | ⚠️ | Basic string interpolation |
| | Recursive Types | ❌ | Not implemented |

---

## Quick Reference

### Compile & Run
```bash
# Compile
./tsc examples/00_quick_start.ts

# Run
./a.out
```

### Feature Support Summary
- ✅ **Fully Working**: Basic types, functions, classes (structure), interfaces, enums
- ⚠️ **Partial**: Classes (method calls), utility types, modules
- ❌ **Not Implemented**: Advanced type manipulation, decorators, conditional types

### Learning Path
1. **Start with Basic Level** (1-10) - Master fundamentals
2. **Progress to Intermediate** (11-20) - Learn advanced patterns
3. **Explore Advanced** (21-30) - Understand type system deeply

---

*This TypeScript Syntax Bible covers the complete spectrum from beginner to pro, with clear status indicators for the TSC compiler.*