// TypeScript Meta-Programming & Cutting-Edge Features
// Demonstrates advanced type-level programming and meta-patterns

// 36. Recursive Types
type JSONValue =
  | string
  | number
  | boolean
  | null
  | JSONValue[]
  | { [key: string]: JSONValue };

const obj: JSONValue = {
  name: "Alice",
  scores: [10, 20],
  nested: { valid: true }
};

// Recursive utility types
type DeepReadonly<T> = {
  readonly [P in keyof T]: T[P] extends object ? DeepReadonly<T[P]> : T[P];
};

type DeepPartial<T> = {
  [P in keyof T]?: T[P] extends object ? DeepPartial<T[P]> : T[P];
};

type DeepRequired<T> = {
  [P in keyof T]-?: T[P] extends object ? DeepRequired<T[P]> : T[P];
};

// 37. Variadic Tuple Types
type Prepend<T, U extends any[]> = [T, ...U];
type Append<T, U extends any[]> = [...U, T];

type Result1 = Prepend<number, [string, boolean]>; // [number, string, boolean]
type Result2 = Append<boolean, [number, string]>; // [number, string, boolean]

function concat<T extends unknown[], U extends unknown[]>(a: [...T], b: [...U]): [...T, ...U] {
  return [...a, ...b];
}

const x = concat([1, 2], ["a", "b"]); // [number, number, string, string]

// Variadic tuple utilities
type Head<T extends any[]> = T extends [infer H, ...any[]] ? H : never;
type Tail<T extends any[]> = T extends [any, ...infer T] ? T : [];
type Last<T extends any[]> = T extends [...any[], infer L] ? L : never;

type H = Head<[1, 2, 3]>; // 1
type T = Tail<[1, 2, 3]>; // [2, 3]
type L = Last<[1, 2, 3]>; // 3

// 38. Higher-Kinded Types (Simulated)
// Not natively supported, but can be simulated with type parameters

type Functor<T> = {
  map<U>(fn: (x: T) => U): Functor<U>;
};

type Monad<T> = {
  flatMap<U>(fn: (x: T) => Monad<U>): Monad<U>;
  pure<U>(value: U): Monad<U>;
};

// Simulated higher-kinded type
type HKT<F, T> = F extends { map: any } ? F : never;

// 39. Branded (Nominal) Types
type UserId = string & { __brand: "UserId" };
type OrderId = string & { __brand: "OrderId" };
type ProductId = string & { __brand: "ProductId" };

function createUserId(id: string): UserId {
  return id as UserId;
}

function createOrderId(id: string): OrderId {
  return id as OrderId;
}

function createProductId(id: string): ProductId {
  return id as ProductId;
}

function getUser(id: UserId): { id: UserId; name: string } {
  return { id, name: "User" };
}

function getOrder(id: OrderId): { id: OrderId; total: number } {
  return { id, total: 100 };
}

// Type-safe ID usage
let userId = createUserId("u123");
let orderId = createOrderId("o456");
let productId = createProductId("p789");

let user = getUser(userId);   // ✅
let order = getOrder(orderId); // ✅
// let user2 = getUser(orderId); // ❌ type error

// 40. Type-Level Computation
type Length<T extends any[]> = T["length"];
type A = Length<[1, 2, 3]>; // 3
type B = Length<[]>; // 0

// Type-level arithmetic (simplified)
type Add<A extends number, B extends number> = A extends number ? B extends number ? number : never : never;
type Subtract<A extends number, B extends number> = A extends number ? B extends number ? number : never : never;

// Type-level comparison
type IsEqual<A, B> = A extends B ? B extends A ? true : false : false;
type IsGreater<A extends number, B extends number> = A extends B ? false : true; // Simplified

// 41. Distributive Conditional Types
type ExcludeNull<T> = T extends null | undefined ? never : T;
type Clean = ExcludeNull<string | null | number>; // string | number

type NonNullable<T> = T extends null | undefined ? never : T;
type ExtractString<T> = T extends string ? T : never;

// Distributive mapping
type ToArray<T> = T extends any ? T[] : never;
type StringArray = ToArray<string | number>; // string[] | number[]

// 42. Template Type Inference
type ExtractId<T> = T extends `${infer Prefix}:${infer Id}` ? Id : never;
type ExtractPrefix<T> = T extends `${infer Prefix}:${infer Id}` ? Prefix : never;

type A = ExtractId<"user:123">; // "123"
type B = ExtractId<"order:456">; // "456"
type C = ExtractPrefix<"user:123">; // "user"

// More complex template inference
type ParseRoute<T> = T extends `/${infer Path}` ? Path : never;
type RouteParams<T> = T extends `${infer Start}:${infer Param}/${infer Rest}` 
  ? Param | RouteParams<`/${Rest}`>
  : T extends `${infer Start}:${infer Param}`
  ? Param
  : never;

type UserRoute = ParseRoute<"/users/:id">; // "users/:id"
type UserParams = RouteParams<"/users/:id">; // "id"

// 43. TypeScript with as const + Narrowing
const roles = ["admin", "user", "guest"] as const;
type Role = typeof roles[number]; // "admin" | "user" | "guest"

const config = {
  api: {
    baseUrl: "https://api.example.com",
    timeout: 5000
  },
  features: {
    enableLogging: true,
    enableCaching: false
  }
} as const;

type Config = typeof config;
type ApiConfig = Config["api"];
type FeaturesConfig = Config["features"];

// Narrowing with const assertions
function processRole(role: Role): string {
  switch (role) {
    case "admin":
      return "Administrator";
    case "user":
      return "Regular User";
    case "guest":
      return "Guest User";
  }
}

// 44. Index Signatures with Modifiers
interface Config {
  [key: string]: string | number;
  readonly [key: number]: string; // numeric keys readonly
}

interface FlexibleObject {
  [key: string]: any;
  readonly id: number; // specific key readonly
  name: string;
}

// 45. Conditional Inference with infer Chains
type Flatten<T> = T extends (infer U)[] ? U : T;
type A = Flatten<string[]>; // string
type B = Flatten<number>;   // number

// Complex inference chains
type UnwrapPromise<T> = T extends Promise<infer U> ? U : T;
type UnwrapArray<T> = T extends (infer U)[] ? U : T;
type UnwrapAll<T> = UnwrapPromise<UnwrapArray<T>>;

type PromiseArray = Promise<string[]>;
type Unwrapped = UnwrapAll<PromiseArray>; // string

// 46. Awaited Type (Promise Unwrapping)
type Awaited<T> = T extends Promise<infer U> ? Awaited<U> : T;

type A = Awaited<Promise<string>>; // string
type B = Awaited<Promise<Promise<number>>>; // number
type C = Awaited<string>; // string

// 47. Self-Referential Types
type Tree<T> = {
  value: T;
  children?: Tree<T>[];
};

const root: Tree<number> = {
  value: 1,
  children: [
    { value: 2 },
    { value: 3, children: [{ value: 4 }] }
  ]
};

// More complex self-referential types
type LinkedList<T> = {
  value: T;
  next?: LinkedList<T>;
};

type BinaryTree<T> = {
  value: T;
  left?: BinaryTree<T>;
  right?: BinaryTree<T>;
};

// 48. Function Overloads
function reverse(str: string): string;
function reverse(arr: number[]): number[];
function reverse(val: string | number[]): string | number[] {
  if (typeof val === "string") {
    return val.split("").reverse().join("");
  } else {
    return val.slice().reverse();
  }
}

let strResult = reverse("hello"); // "olleh"
let arrResult = reverse([1, 2, 3]); // [3, 2, 1]

// Complex overloads
function createElement(tag: "div"): HTMLDivElement;
function createElement(tag: "span"): HTMLSpanElement;
function createElement(tag: "button"): HTMLButtonElement;
function createElement(tag: string): HTMLElement {
  return document.createElement(tag) as HTMLElement;
}

// 49. Const Enum (Performance Optimization)
const enum Colors {
  Red,
  Green,
  Blue
}

let c: Colors = Colors.Green;

// Const enum with string values
const enum Status {
  Pending = "pending",
  Approved = "approved",
  Rejected = "rejected"
}

let status: Status = Status.Pending;

// 50. TypeScript + JSDoc for JS Files (Concept)
// This shows how JSDoc can provide TypeScript-like type checking in JS files

/**
 * @param {number} a
 * @param {number} b
 * @returns {number}
 */
function add(a, b) {
  return a + b;
}

/**
 * @param {string} name
 * @param {number} age
 * @returns {Object}
 */
function createPerson(name, age) {
  return { name, age };
}

// Advanced Meta-Programming Patterns

// Type-level validation
type IsValidEmail<T extends string> = T extends `${string}@${string}.${string}` ? true : false;
type ValidEmail = IsValidEmail<"user@example.com">; // true
type InvalidEmail = IsValidEmail<"not-an-email">; // false

// Type-level mapping
type StringToNumber<T extends string> = T extends "one" ? 1 : T extends "two" ? 2 : T extends "three" ? 3 : never;
type One = StringToNumber<"one">; // 1
type Two = StringToNumber<"two">; // 2

// Type-level filtering
type FilterStrings<T> = T extends string ? T : never;
type OnlyStrings = FilterStrings<string | number | boolean>; // string

// Type-level transformation
type CapitalizeWords<T extends string> = T extends `${infer First} ${infer Rest}` 
  ? `${Capitalize<First>} ${CapitalizeWords<Rest>}`
  : Capitalize<T>;

type TitleCase = CapitalizeWords<"hello world">; // "Hello World"

// Usage examples
let jsonData: JSONValue = {
  name: "Alice",
  age: 30,
  hobbies: ["reading", "coding"],
  address: {
    street: "123 Main St",
    city: "New York",
    coordinates: [40.7128, -74.0060]
  }
};

// Variadic tuple usage
let concatenated = concat([1, 2, 3], ["a", "b", "c"]);

// Branded type usage
let user1 = getUser(createUserId("user1"));
let order1 = getOrder(createOrderId("order1"));

// Template inference usage
let userId = "user:123";
let extractedId = userId as ExtractId<typeof userId>; // "123"

// Const assertion usage
let role: Role = "admin";
let roleDescription = processRole(role);

// Function overload usage
let reversedString = reverse("TypeScript");
let reversedArray = reverse([1, 2, 3, 4, 5]);

// Tree structure usage
let tree: Tree<string> = {
  value: "root",
  children: [
    { value: "child1" },
    { value: "child2", children: [{ value: "grandchild" }] }
  ]
};

// This example demonstrates meta-programming and cutting-edge TypeScript features