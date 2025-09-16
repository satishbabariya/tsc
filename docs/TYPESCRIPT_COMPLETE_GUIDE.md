# TypeScript Complete Guide

*From Beginner to Expert - The Ultimate Reference*

## 📚 Table of Contents

1. [Quick Start](#quick-start)
2. [Beginner Level](#beginner-level) (1-10)
3. [Intermediate Level](#intermediate-level) (11-20)
4. [Advanced Level](#advanced-level) (21-30)
5. [Expert Level](#expert-level) (31-40)
6. [TSC Compiler Status](#tsc-compiler-status)
7. [Real-World Patterns](#real-world-patterns)
8. [Quick Reference](#quick-reference)

---

## Quick Start

### Essential Syntax (5 minutes)

```typescript
// Basic types
let name: string = "Alice";
let age: number = 30;
let isActive: boolean = true;

// Functions
function greet(name: string): string {
  return `Hello, ${name}!`;
}

// Classes
class Person {
  constructor(public name: string, public age: number) {}
  
  greet(): string {
    return `Hi, I'm ${this.name}`;
  }
}

// Interfaces
interface User {
  id: number;
  name: string;
  email?: string;
}

// Generics
function identity<T>(arg: T): T {
  return arg;
}
```

---

## Beginner Level

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

// main.ts
import { add } from "./math";
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
```

### 21. Maps & Sets

```typescript
// Maps - Key-Value Collections
const userRoles = new Map<string, string>();
userRoles.set("alice", "admin");
userRoles.set("bob", "user");

console.log(userRoles.get("alice")); // "admin"
console.log(userRoles.has("bob"));   // true

// Sets - Unique Value Collections
const uniqueNumbers = new Set<number>();
uniqueNumbers.add(1);
uniqueNumbers.add(2);
uniqueNumbers.add(2); // duplicates ignored

console.log(uniqueNumbers.has(1)); // true
console.log(uniqueNumbers.size);   // 2

// Iteration
for (const [user, role] of userRoles) {
  console.log(user, role);
}

for (const num of uniqueNumbers) {
  console.log(num);
}
```

### 22. Async/Await

```typescript
function delay(ms: number): Promise<string> {
  return new Promise(resolve => {
    setTimeout(() => resolve(`Done after ${ms}ms`), ms);
  });
}

async function runTask() {
  console.log("Task started...");
  const result = await delay(1000);
  console.log(result);
}

// Error handling
async function fetchData(): Promise<string> {
  return "Hello World";
}

async function main() {
  try {
    const data = await fetchData();
    console.log(data);
  } catch (err) {
    console.error("Error:", err);
  }
}

// Async iteration
async function* asyncGenerator() {
  yield "First";
  yield "Second";
}

(async () => {
  for await (const val of asyncGenerator()) {
    console.log(val);
  }
})();
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

// Deep readonly type
type DeepReadonly<T> = {
  readonly [P in keyof T]: T[P] extends object ? DeepReadonly<T[P]> : T[P];
};
```

### 24. Advanced Mapped Types

```typescript
// Make all properties optional and nullable
type PartialNullable<T> = {
  [P in keyof T]?: T[P] | null;
};

// Create a type with specific property types
type Stringify<T> = {
  [K in keyof T]: string;
};
```

### 25. Branded Types

```typescript
type Brand<T, B> = T & { __brand: B };

type UserId = Brand<number, 'UserId'>;
type ProductId = Brand<number, 'ProductId'>;

function createUserId(id: number): UserId {
  return id as UserId;
}

// Prevents mixing up different ID types
let userId: UserId = createUserId(123);
let productId: ProductId = createProductId(456);
// let mixed: UserId = productId; // ❌ Error
```

### 26. Abstract Classes

```typescript
abstract class Animal {
  abstract makeSound(): void;
  move() {
    console.log("Moving...");
  }
}

class Dog extends Animal {
  makeSound() {
    console.log("Woof!");
  }
}
```

### 27. Mixins

```typescript
type Constructor<T = {}> = new (...args: any[]) => T;

function CanFly<TBase extends Constructor>(Base: TBase) {
  return class extends Base {
    fly() {
      console.log("Flying...");
    }
  };
}

class Animal {}
class Bird extends CanFly(Animal) {}

const b = new Bird();
b.fly();
```

### 28. Module Augmentation

```typescript
declare module "express" {
  interface Request {
    user?: { id: string; name: string };
  }
}
```

### 29. Declaration Merging

```typescript
interface User {
  id: number;
}
interface User {
  name: string;
}

const u: User = { id: 1, name: "Alice" };
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
```

---

## Expert Level

### 31. Discriminated Unions

```typescript
type Shape =
  | { kind: "circle"; radius: number }
  | { kind: "square"; side: number };

function area(shape: Shape) {
  switch (shape.kind) {
    case "circle": return Math.PI * shape.radius ** 2;
    case "square": return shape.side * shape.side;
  }
}
```

### 32. Template Literal Types (Advanced)

```typescript
type EventType = "click" | "hover";
type EventName = `${EventType}Event`; // "clickEvent" | "hoverEvent"

type Join<K, P> = K extends string ? P extends string ? `${K}${P}` : never : never;
type Paths<T> = T extends object ? {
  [K in keyof T]-?: K extends string ? 
    `${K}` | Join<K, Paths<T[K]>> : never
}[keyof T] : '';
```

### 33. Satisfies Operator

```typescript
type Theme = { primary: string; secondary: string };

const theme = {
  primary: "#000",
  secondary: "#fff",
  extra: "ignored"
} satisfies Theme;
```

### 34. Record & Lookup Types

```typescript
type Roles = "admin" | "user" | "guest";
type Permissions = Record<Roles, string[]>;

const perms: Permissions = {
  admin: ["create", "delete"],
  user: ["read"],
  guest: []
};
```

### 35. Indexed Access Types

```typescript
interface Person {
  name: string;
  age: number;
}

type NameType = Person["name"]; // string
```

### 36. This Parameter Types

```typescript
class Counter {
  count = 0;
  inc(this: Counter) {
    this.count++;
  }
}

const c = new Counter();
const fn = c.inc;
// fn(); ❌ error
fn.call(c); // ✅
```

### 37. Type Guards

```typescript
function isString(value: unknown): value is string {
  return typeof value === "string";
}

function print(v: unknown) {
  if (isString(v)) {
    console.log(v.toUpperCase());
  }
}
```

### 38. Exhaustiveness Checking

```typescript
function check(x: "a" | "b"): number {
  switch (x) {
    case "a": return 1;
    case "b": return 2;
    default: 
      const _exhaustive: never = x; // forces compile error if missed
      return _exhaustive;
  }
}
```

### 39. Utility Types (Complete)

```typescript
// Built-in utility types
Partial<T>     // makes properties optional
Required<T>    // makes all required
Readonly<T>    // all readonly
Pick<T, K>     // pick specific props
Omit<T, K>     // exclude specific props
Record<K, T>   // object with keys K and values T
Extract<T, U>  // keep only common types
Exclude<T, U>  // remove matching types
NonNullable<T> // remove null and undefined
ReturnType<T>  // function return type
InstanceType<T> // instance type of a class
```

### 40. Advanced Inference with Generics

```typescript
function first<T extends any[]>(arr: T): T[0] {
  return arr[0];
}

const result = first([10, "hi", true]); // number
```

---

## TSC Compiler Status

| Level            | Feature                 | Status | Notes                                     |
|------------------|-------------------------|--------|-------------------------------------------|
| **Beginner**     | Basic Types             | ✅      | All primitive types supported             |
|                  | Variables               | ✅      | `let`, `const`, `var` with annotations    |
|                  | Functions               | ✅      | Declarations, calls, parameters           |
|                  | Classes                 | ⚠️     | Basic structure, method calls have issues |
|                  | Interfaces              | ✅      | Declarations and implementation           |
|                  | Enums                   | ✅      | All enum types and operations             |
|                  | Generics (Basic)        | ✅      | Basic generic functions and classes       |
|                  | Control Flow            | ✅      | If/else, loops, switch                    |
|                  | Arrays/Objects          | ✅      | Literals, indexing, operations            |
| **Intermediate** | Optional Parameters     | ✅      | Function parameters                       |
|                  | Readonly                | ✅      | Interface properties                      |
|                  | Utility Types           | ⚠️     | Partial implementation                    |
|                  | Keyof/Index Types       | ⚠️     | Basic support                             |
|                  | Mapped Types            | ⚠️     | Limited support                           |
|                  | Namespaces              | ✅      | Basic namespace support                   |
|                  | Modules                 | ⚠️     | Import/export structure                   |
| **Advanced**     | Conditional Types       | ❌      | Not implemented                           |
|                  | Infer Keyword           | ❌      | Not implemented                           |
|                  | Decorators              | ❌      | Not implemented                           |
|                  | Recursive Types         | ❌      | Not implemented                           |
|                  | Branded Types           | ❌      | Not implemented                           |
| **Expert**       | Discriminated Unions    | ⚠️     | Basic support                             |
|                  | Template Literals       | ⚠️     | Basic string interpolation                |
|                  | Satisfies Operator      | ❌      | Not implemented                           |
|                  | Type Guards             | ⚠️     | Basic support                             |
|                  | Exhaustiveness Checking | ❌      | Not implemented                           |
| **Collections**  | Maps & Sets             | ✅      | Full support                              |
|                  | WeakMap & WeakSet       | ✅      | Full support                              |
| **Async**        | Async/Await             | ✅      | Full support                              |
|                  | Promise Types           | ✅      | Full support                              |
|                  | Async Iteration         | ✅      | Full support                              |

---

## Real-World Patterns

### Framework Patterns

```typescript
// React Component Props
interface ButtonProps {
  children: React.ReactNode;
  onClick: () => void;
  variant?: 'primary' | 'secondary';
  disabled?: boolean;
}

// Angular Service
@Injectable()
export class UserService {
  constructor(private http: HttpClient) {}
  
  getUsers(): Observable<User[]> {
    return this.http.get<User[]>('/api/users');
  }
}

// NestJS Controller
@Controller('users')
export class UsersController {
  @Get()
  findAll(): User[] {
    return this.userService.findAll();
  }
}
```

### Library Patterns

```typescript
// Generic API Client
class ApiClient<T> {
  constructor(private baseUrl: string) {}
  
  async get<K extends keyof T>(endpoint: K): Promise<T[K]> {
    const response = await fetch(`${this.baseUrl}/${endpoint}`);
    return response.json();
  }
}

// Event System
type EventMap = {
  'user.created': { id: number; name: string };
  'user.updated': { id: number; changes: Partial<User> };
};

class EventEmitter {
  on<K extends keyof EventMap>(event: K, handler: (data: EventMap[K]) => void): void {
    // Implementation
  }
  
  emit<K extends keyof EventMap>(event: K, data: EventMap[K]): void {
    // Implementation
  }
}
```

---

## Quick Reference

### Compile & Run

```bash
# Compile
./tsc examples/00_quick_start.ts

# Run
./a.out
```

### Learning Path

1. **Beginner** (1-10) - Master fundamentals
2. **Intermediate** (11-20) - Learn advanced patterns
3. **Advanced** (21-30) - Understand type system deeply
4. **Expert** (31-40) - Framework/library level patterns

### Feature Support Summary

- ✅ **Fully Working**: Basic types, functions, interfaces, enums
- ⚠️ **Partial**: Classes, utility types, modules, some advanced features
- ❌ **Not Implemented**: Advanced type manipulation, decorators, conditional types

### Common Patterns

- **Type Safety**: Use strict types, avoid `any`
- **Generic Constraints**: Use `extends` for type bounds
- **Utility Types**: Leverage `Partial`, `Pick`, `Omit` for transformations
- **Discriminated Unions**: For type-safe state management
- **Type Guards**: For runtime type checking
- **Mapped Types**: For dynamic type generation

---

*This TypeScript Complete Guide covers the full spectrum from beginner to expert, with clear status indicators for the
TSC compiler and real-world patterns used in modern frameworks and libraries.*