// Expert TypeScript Patterns
// Demonstrates advanced patterns used in real-world frameworks and libraries

// 21. Discriminated Unions
// Used for type-safe switch cases and state management

type Shape = 
  | { kind: "circle"; radius: number }
  | { kind: "square"; side: number }
  | { kind: "rectangle"; width: number; height: number };

function area(shape: Shape): number {
  switch (shape.kind) {
    case "circle": 
      return Math.PI * shape.radius ** 2;
    case "square": 
      return shape.side * shape.side;
    case "rectangle": 
      return shape.width * shape.height;
  }
}

// API Response discriminated union
type ApiResponse<T> = 
  | { status: "success"; data: T; timestamp: string }
  | { status: "error"; error: string; code: number }
  | { status: "loading" };

function handleResponse<T>(response: ApiResponse<T>): string {
  switch (response.status) {
    case "success":
      return `Data: ${JSON.stringify(response.data)}`;
    case "error":
      return `Error ${response.code}: ${response.error}`;
    case "loading":
      return "Loading...";
  }
}

// 22. Template Literal Types
type EventType = "click" | "hover" | "focus" | "blur";
type EventName = `${EventType}Event`; // "clickEvent" | "hoverEvent" | "focusEvent" | "blurEvent"

type CSSProperty = `--${string}`;
type CSSValue = `${number}px` | `${number}%` | `${number}em` | `${number}rem`;

type CSSRule = `${CSSProperty}: ${CSSValue}`;

// 23. Satisfies Operator (Concept - may not be supported in TSC yet)
// Ensures a value fits a type but keeps literal inference

type Theme = { primary: string; secondary: string };

// This would work in full TypeScript with satisfies operator
const theme = {
  primary: "#000",
  secondary: "#fff",
  extra: "ignored" // This would be allowed with satisfies
} as Theme; // Using type assertion as workaround

// 24. Record & Lookup Types
type Roles = "admin" | "user" | "guest";
type Permissions = Record<Roles, string[]>;

const perms: Permissions = {
  admin: ["create", "delete", "read", "update"],
  user: ["read", "update"],
  guest: ["read"]
};

// Dynamic record creation
type DynamicRecord<K extends string, V> = Record<K, V>;

type UserRoles = DynamicRecord<"admin" | "user", { level: number; permissions: string[] }>;

// 25. Indexed Access Types
interface Person {
  name: string;
  age: number;
  address: {
    street: string;
    city: string;
    country: string;
  };
}

type NameType = Person["name"]; // string
type AgeType = Person["age"]; // number
type AddressType = Person["address"]; // { street: string; city: string; country: string }
type StreetType = Person["address"]["street"]; // string

// 26. Abstract Classes
abstract class Animal {
  protected name: string;
  
  constructor(name: string) {
    this.name = name;
  }
  
  abstract makeSound(): void;
  
  move(): void {
    _print(`${this.name} is moving...`);
  }
  
  // Concrete method
  sleep(): void {
    _print(`${this.name} is sleeping...`);
  }
}

class Dog extends Animal {
  makeSound(): void {
    _print(`${this.name} says Woof!`);
  }
  
  wagTail(): void {
    _print(`${this.name} is wagging tail`);
  }
}

class Cat extends Animal {
  makeSound(): void {
    _print(`${this.name} says Meow!`);
  }
  
  purr(): void {
    _print(`${this.name} is purring`);
  }
}

// 27. Mixins
type Constructor<T = {}> = new (...args: any[]) => T;

function CanFly<TBase extends Constructor>(Base: TBase) {
  return class extends Base {
    fly(): void {
      _print("Flying...");
    }
  };
}

function CanSwim<TBase extends Constructor>(Base: TBase) {
  return class extends Base {
    swim(): void {
      _print("Swimming...");
    }
  };
}

class Animal {
  name: string;
  
  constructor(name: string) {
    this.name = name;
  }
  
  move(): void {
    _print(`${this.name} is moving`);
  }
}

class Bird extends CanFly(Animal) {
  chirp(): void {
    _print(`${this.name} is chirping`);
  }
}

class Duck extends CanFly(CanSwim(Animal)) {
  quack(): void {
    _print(`${this.name} is quacking`);
  }
}

// 28. Module Augmentation (Concept)
// Extend existing modules/types

// This would work in full TypeScript with module declaration
declare global {
  interface Window {
    myCustomProperty: string;
  }
}

// Simulated module augmentation
interface ExpressRequest {
  body: any;
  params: any;
  query: any;
}

interface ExtendedRequest extends ExpressRequest {
  user?: { id: string; name: string; role: string };
}

// 29. Declaration Merging
interface User {
  id: number;
}

interface User {
  name: string;
}

interface User {
  email: string;
}

// All properties are merged
const user: User = { id: 1, name: "Alice", email: "alice@example.com" };

// Namespace merging
namespace Database {
  export interface Connection {
    host: string;
    port: number;
  }
}

namespace Database {
  export interface Query {
    sql: string;
    params: any[];
  }
}

// 30. Conditional + Mapped Types (Pro Level)
type Nullable<T> = {
  [K in keyof T]: T[K] | null;
};

type Optional<T> = {
  [K in keyof T]?: T[K];
};

type Required<T> = {
  [K in keyof T]-?: T[K];
};

interface Profile {
  id: number;
  name: string;
  email: string;
}

type NullableProfile = Nullable<Profile>;
// { id: number | null; name: string | null; email: string | null }

type OptionalProfile = Optional<Profile>;
// { id?: number; name?: string; email?: string }

// 31. This Parameter Types
class Counter {
  count: number = 0;
  
  inc(this: Counter): void {
    this.count++;
  }
  
  getCount(this: Counter): number {
    return this.count;
  }
}

class Timer {
  private interval: number = 0;
  
  start(this: Timer, callback: () => void): void {
    this.interval = setInterval(callback, 1000);
  }
  
  stop(this: Timer): void {
    clearInterval(this.interval);
  }
}

// 32. Type Guards
function isString(value: unknown): value is string {
  return typeof value === "string";
}

function isNumber(value: unknown): value is number {
  return typeof value === "number";
}

function isArray<T>(value: unknown): value is T[] {
  return Array.isArray(value);
}

function isObject(value: unknown): value is object {
  return typeof value === "object" && value !== null;
}

function print(value: unknown): void {
  if (isString(value)) {
    _print(value.toUpperCase());
  } else if (isNumber(value)) {
    _print(value.toFixed(2));
  } else if (isArray(value)) {
    _print(`Array with ${value.length} items`);
  } else if (isObject(value)) {
    _print("Object:", Object.keys(value));
  } else {
    _print("Unknown type");
  }
}

// 33. Exhaustiveness Checking
function check(x: "a" | "b" | "c"): number {
  switch (x) {
    case "a": return 1;
    case "b": return 2;
    case "c": return 3;
    default: 
      const _exhaustive: never = x; // forces compile error if missed
      return _exhaustive;
  }
}

// 34. Utility Types (Full Implementation)
type Partial<T> = {
  [P in keyof T]?: T[P];
};

type Required<T> = {
  [P in keyof T]-?: T[P];
};

type Readonly<T> = {
  readonly [P in keyof T]: T[P];
};

type Pick<T, K extends keyof T> = {
  [P in K]: T[P];
};

type Omit<T, K extends keyof T> = {
  [P in Exclude<keyof T, K>]: T[P];
};

type Record<K extends keyof any, T> = {
  [P in K]: T;
};

type Extract<T, U> = T extends U ? T : never;

type Exclude<T, U> = T extends U ? never : T;

type NonNullable<T> = T extends null | undefined ? never : T;

type ReturnType<T extends (...args: any) => any> = T extends (...args: any) => infer R ? R : any;

type Parameters<T extends (...args: any) => any> = T extends (...args: infer P) => any ? P : never;

type InstanceType<T extends new (...args: any) => any> = T extends new (...args: any) => infer R ? R : any;

// 35. Advanced Inference with Generics
function first<T extends any[]>(arr: T): T[0] {
  return arr[0];
}

function last<T extends any[]>(arr: T): T[number] {
  return arr[arr.length - 1];
}

function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
  return obj[key];
}

// Usage examples
let circle: Shape = { kind: "circle", radius: 5 };
let square: Shape = { kind: "square", side: 4 };
let rectangle: Shape = { kind: "rectangle", width: 6, height: 8 };

let circleArea = area(circle);
let squareArea = area(square);
let rectangleArea = area(rectangle);

// API Response usage
let successResponse: ApiResponse<{ id: number; name: string }> = {
  status: "success",
  data: { id: 1, name: "Alice" },
  timestamp: new Date().toISOString()
};

let errorResponse: ApiResponse<never> = {
  status: "error",
  error: "Not found",
  code: 404
};

let loadingResponse: ApiResponse<never> = {
  status: "loading"
};

_print(handleResponse(successResponse));
_print(handleResponse(errorResponse));
_print(handleResponse(loadingResponse));

// Abstract class usage
let dog = new Dog("Buddy");
let cat = new Cat("Whiskers");

dog.makeSound();
dog.move();
dog.sleep();
dog.wagTail();

cat.makeSound();
cat.move();
cat.sleep();
cat.purr();

// Mixin usage
let bird = new Bird("Tweety");
let duck = new Duck("Donald");

bird.move();
bird.fly();
bird.chirp();

duck.move();
duck.fly();
duck.swim();
duck.quack();

// Type guard usage
print("Hello World");
print(42);
print([1, 2, 3]);
print({ name: "Alice", age: 30 });
print(null);

// Utility types usage
let profile: Profile = { id: 1, name: "Alice", email: "alice@example.com" };
let partialProfile: Partial<Profile> = { name: "Alice" };
let requiredProfile: Required<Profile> = { id: 1, name: "Alice", email: "alice@example.com" };
let readonlyProfile: Readonly<Profile> = { id: 1, name: "Alice", email: "alice@example.com" };
let pickedProfile: Pick<Profile, "id" | "name"> = { id: 1, name: "Alice" };
let omittedProfile: Omit<Profile, "email"> = { id: 1, name: "Alice" };

// Advanced inference usage
let firstNumber = first([10, 20, 30]); // number
let firstString = first(["hello", "world"]); // string
let lastItem = last([1, 2, 3, 4, 5]); // number
let property = getProperty(profile, "name"); // string

// This example demonstrates expert-level TypeScript patterns