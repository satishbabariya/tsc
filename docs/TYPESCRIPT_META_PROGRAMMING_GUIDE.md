# TypeScript Meta-Programming Guide
*Beyond Expert: Type-Level Programming & Cutting-Edge Features*

## 🧠 Table of Contents

1. [Meta-Programming Overview](#meta-programming-overview)
2. [Recursive Types](#recursive-types) (36)
3. [Variadic Tuple Types](#variadic-tuple-types) (37)
4. [Higher-Kinded Types](#higher-kinded-types) (38)
5. [Branded Types](#branded-types) (39)
6. [Type-Level Computation](#type-level-computation) (40)
7. [Distributive Conditional Types](#distributive-conditional-types) (41)
8. [Template Type Inference](#template-type-inference) (42)
9. [Const Assertions & Narrowing](#const-assertions--narrowing) (43)
10. [Index Signatures with Modifiers](#index-signatures-with-modifiers) (44)
11. [Conditional Inference Chains](#conditional-inference-chains) (45)
12. [Awaited Type](#awaited-type) (46)
13. [Self-Referential Types](#self-referential-types) (47)
14. [Function Overloads](#function-overloads) (48)
15. [Const Enums](#const-enums) (49)
16. [JSDoc Integration](#jsdoc-integration) (50)
17. [Real-World Meta-Patterns](#real-world-meta-patterns)
18. [TSC Compiler Status](#tsc-compiler-status)

---

## Meta-Programming Overview

At this level, TypeScript becomes a **compile-time programming language**. You can:
- Model complex data structures at the type level
- Validate data shapes and enforce contracts
- Perform computations during compilation
- Create type-safe APIs with zero runtime cost

---

## Recursive Types

### Basic Recursive Types
```typescript
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
```

### Recursive Utility Types
```typescript
type DeepReadonly<T> = {
  readonly [P in keyof T]: T[P] extends object ? DeepReadonly<T[P]> : T[P];
};

type DeepPartial<T> = {
  [P in keyof T]?: T[P] extends object ? DeepPartial<T[P]> : T[P];
};

type DeepRequired<T> = {
  [P in keyof T]-?: T[P] extends object ? DeepRequired<T[P]> : T[P];
};
```

### Self-Referential Data Structures
```typescript
type Tree<T> = {
  value: T;
  children?: Tree<T>[];
};

type LinkedList<T> = {
  value: T;
  next?: LinkedList<T>;
};

type BinaryTree<T> = {
  value: T;
  left?: BinaryTree<T>;
  right?: BinaryTree<T>;
};
```

---

## Variadic Tuple Types

### Basic Variadic Tuples
```typescript
type Prepend<T, U extends any[]> = [T, ...U];
type Append<T, U extends any[]> = [...U, T];

type Result1 = Prepend<number, [string, boolean]>; // [number, string, boolean]
type Result2 = Append<boolean, [number, string]>; // [number, string, boolean]
```

### Variadic Functions
```typescript
function concat<T extends unknown[], U extends unknown[]>(
  a: [...T], 
  b: [...U]
): [...T, ...U] {
  return [...a, ...b];
}

const x = concat([1, 2], ["a", "b"]); // [number, number, string, string]
```

### Tuple Utilities
```typescript
type Head<T extends any[]> = T extends [infer H, ...any[]] ? H : never;
type Tail<T extends any[]> = T extends [any, ...infer T] ? T : [];
type Last<T extends any[]> = T extends [...any[], infer L] ? L : never;

type H = Head<[1, 2, 3]>; // 1
type T = Tail<[1, 2, 3]>; // [2, 3]
type L = Last<[1, 2, 3]>; // 3
```

---

## Higher-Kinded Types

### Simulated Higher-Kinded Types
```typescript
type Functor<T> = {
  map<U>(fn: (x: T) => U): Functor<U>;
};

type Monad<T> = {
  flatMap<U>(fn: (x: T) => Monad<U>): Monad<U>;
  pure<U>(value: U): Monad<U>;
};

// Simulated higher-kinded type
type HKT<F, T> = F extends { map: any } ? F : never;
```

### Generic Type Constructors
```typescript
type ArrayConstructor = <T>(value: T) => T[];
type PromiseConstructor = <T>(value: T) => Promise<T>;
type OptionConstructor = <T>(value: T) => T | null;
```

---

## Branded Types

### Basic Branded Types
```typescript
type UserId = string & { __brand: "UserId" };
type OrderId = string & { __brand: "OrderId" };
type ProductId = string & { __brand: "ProductId" };

function createUserId(id: string): UserId {
  return id as UserId;
}

function getUser(id: UserId): { id: UserId; name: string } {
  return { id, name: "User" };
}

// Type-safe ID usage
let userId = createUserId("u123");
let user = getUser(userId);   // ✅
// let user2 = getUser("o456"); // ❌ type error
```

### Branded Type Utilities
```typescript
type Brand<T, B> = T & { __brand: B };

type BrandedString<B> = Brand<string, B>;
type BrandedNumber<B> = Brand<number, B>;

type UserId = BrandedString<"UserId">;
type OrderId = BrandedString<"OrderId">;
```

---

## Type-Level Computation

### Basic Type-Level Operations
```typescript
type Length<T extends any[]> = T["length"];
type A = Length<[1, 2, 3]>; // 3
type B = Length<[]>; // 0

// Type-level comparison
type IsEqual<A, B> = A extends B ? B extends A ? true : false : false;
type IsGreater<A extends number, B extends number> = A extends B ? false : true;
```

### Type-Level Arithmetic
```typescript
type Add<A extends number, B extends number> = A extends number ? B extends number ? number : never : never;
type Subtract<A extends number, B extends number> = A extends number ? B extends number ? number : never : never;
type Multiply<A extends number, B extends number> = A extends number ? B extends number ? number : never : never;
```

### Type-Level Validation
```typescript
type IsValidEmail<T extends string> = T extends `${string}@${string}.${string}` ? true : false;
type ValidEmail = IsValidEmail<"user@example.com">; // true
type InvalidEmail = IsValidEmail<"not-an-email">; // false
```

---

## Distributive Conditional Types

### Basic Distributive Types
```typescript
type ExcludeNull<T> = T extends null | undefined ? never : T;
type Clean = ExcludeNull<string | null | number>; // string | number

type NonNullable<T> = T extends null | undefined ? never : T;
type ExtractString<T> = T extends string ? T : never;
```

### Distributive Mapping
```typescript
type ToArray<T> = T extends any ? T[] : never;
type StringArray = ToArray<string | number>; // string[] | number[]

type ToPromise<T> = T extends any ? Promise<T> : never;
type Promises = ToPromise<string | number>; // Promise<string> | Promise<number>
```

---

## Template Type Inference

### Basic Template Inference
```typescript
type ExtractId<T> = T extends `${infer Prefix}:${infer Id}` ? Id : never;
type ExtractPrefix<T> = T extends `${infer Prefix}:${infer Id}` ? Prefix : never;

type A = ExtractId<"user:123">; // "123"
type B = ExtractId<"order:456">; // "456"
type C = ExtractPrefix<"user:123">; // "user"
```

### Complex Template Patterns
```typescript
type ParseRoute<T> = T extends `/${infer Path}` ? Path : never;
type RouteParams<T> = T extends `${infer Start}:${infer Param}/${infer Rest}` 
  ? Param | RouteParams<`/${Rest}`>
  : T extends `${infer Start}:${infer Param}`
  ? Param
  : never;

type UserRoute = ParseRoute<"/users/:id">; // "users/:id"
type UserParams = RouteParams<"/users/:id">; // "id"
```

### String Manipulation
```typescript
type CapitalizeWords<T extends string> = T extends `${infer First} ${infer Rest}` 
  ? `${Capitalize<First>} ${CapitalizeWords<Rest>}`
  : Capitalize<T>;

type TitleCase = CapitalizeWords<"hello world">; // "Hello World"
```

---

## Const Assertions & Narrowing

### Basic Const Assertions
```typescript
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
```

### Narrowing with Const Assertions
```typescript
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
```

---

## Index Signatures with Modifiers

### Modifier Index Signatures
```typescript
interface Config {
  [key: string]: string | number;
  readonly [key: number]: string; // numeric keys readonly
}

interface FlexibleObject {
  [key: string]: any;
  readonly id: number; // specific key readonly
  name: string;
}
```

### Conditional Index Signatures
```typescript
interface ConditionalObject<T> {
  [K in keyof T]: T[K];
  readonly [K in keyof T as `readonly_${string & K}`]: T[K];
}
```

---

## Conditional Inference Chains

### Basic Inference Chains
```typescript
type Flatten<T> = T extends (infer U)[] ? U : T;
type A = Flatten<string[]>; // string
type B = Flatten<number>;   // number
```

### Complex Inference Chains
```typescript
type UnwrapPromise<T> = T extends Promise<infer U> ? U : T;
type UnwrapArray<T> = T extends (infer U)[] ? U : T;
type UnwrapAll<T> = UnwrapPromise<UnwrapArray<T>>;

type PromiseArray = Promise<string[]>;
type Unwrapped = UnwrapAll<PromiseArray>; // string
```

---

## Awaited Type

### Promise Unwrapping
```typescript
type Awaited<T> = T extends Promise<infer U> ? Awaited<U> : T;

type A = Awaited<Promise<string>>; // string
type B = Awaited<Promise<Promise<number>>>; // number
type C = Awaited<string>; // string
```

### Recursive Promise Unwrapping
```typescript
type DeepAwaited<T> = T extends Promise<infer U> ? DeepAwaited<U> : T;
type DeepAwaitedResult = DeepAwaited<Promise<Promise<Promise<string>>>>; // string
```

---

## Self-Referential Types

### Tree Structures
```typescript
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
```

### Linked Lists
```typescript
type LinkedList<T> = {
  value: T;
  next?: LinkedList<T>;
};

type BinaryTree<T> = {
  value: T;
  left?: BinaryTree<T>;
  right?: BinaryTree<T>;
};
```

---

## Function Overloads

### Basic Overloads
```typescript
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
```

### Complex Overloads
```typescript
function createElement(tag: "div"): HTMLDivElement;
function createElement(tag: "span"): HTMLSpanElement;
function createElement(tag: "button"): HTMLButtonElement;
function createElement(tag: string): HTMLElement {
  return document.createElement(tag) as HTMLElement;
}
```

---

## Const Enums

### Basic Const Enums
```typescript
const enum Colors {
  Red,
  Green,
  Blue
}

let c: Colors = Colors.Green;
```

### String Const Enums
```typescript
const enum Status {
  Pending = "pending",
  Approved = "approved",
  Rejected = "rejected"
}

let status: Status = Status.Pending;
```

---

## JSDoc Integration

### Type Annotations in JS
```typescript
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
```

---

## Real-World Meta-Patterns

### API Route Types
```typescript
type ApiRoute<T extends string> = T extends `/${infer Path}` ? Path : never;
type ApiParams<T extends string> = T extends `${infer Start}:${infer Param}/${infer Rest}` 
  ? Param | ApiParams<`/${Rest}`>
  : T extends `${infer Start}:${infer Param}`
  ? Param
  : never;

type UserRoute = ApiRoute<"/users/:id">; // "users/:id"
type UserParams = ApiParams<"/users/:id">; // "id"
```

### Form Validation Types
```typescript
type FormField<T> = {
  value: T;
  error?: string;
  touched: boolean;
};

type FormData<T> = {
  [K in keyof T]: FormField<T[K]>;
};

type UserForm = FormData<{
  name: string;
  email: string;
  age: number;
}>;
```

### State Management Types
```typescript
type State<T> = {
  data: T | null;
  loading: boolean;
  error: string | null;
};

type Action<T> = 
  | { type: "LOADING" }
  | { type: "SUCCESS"; payload: T }
  | { type: "ERROR"; payload: string };

type Reducer<T> = (state: State<T>, action: Action<T>) => State<T>;
```

---

## TSC Compiler Status

| Feature | Status | Notes |
|---------|--------|-------|
| **Recursive Types** | ⚠️ | Basic support, complex patterns may not work |
| **Variadic Tuples** | ❌ | Not implemented |
| **Higher-Kinded Types** | ❌ | Not implemented |
| **Branded Types** | ⚠️ | Basic support with type assertions |
| **Type-Level Computation** | ❌ | Not implemented |
| **Distributive Types** | ❌ | Not implemented |
| **Template Inference** | ❌ | Not implemented |
| **Const Assertions** | ⚠️ | Basic support |
| **Index Signatures** | ✅ | Basic support |
| **Inference Chains** | ❌ | Not implemented |
| **Awaited Type** | ❌ | Not implemented |
| **Self-Referential** | ⚠️ | Basic support |
| **Function Overloads** | ✅ | Basic support |
| **Const Enums** | ✅ | Basic support |
| **JSDoc Integration** | ❌ | Not implemented |

---

## Quick Reference

### Meta-Programming Patterns
- **Recursive Types**: For complex data structures
- **Variadic Tuples**: For flexible function signatures
- **Branded Types**: For type safety and preventing mixing
- **Template Inference**: For string manipulation and parsing
- **Const Assertions**: For literal type narrowing
- **Function Overloads**: For type-safe APIs

### When to Use Meta-Programming
- **Library Development**: Creating type-safe APIs
- **Framework Internals**: Complex type transformations
- **Data Validation**: Type-level validation
- **API Design**: Type-safe route definitions
- **State Management**: Type-safe state patterns

---

*This guide covers the cutting-edge of TypeScript meta-programming, where the type system becomes a compile-time programming language. These patterns are used in production libraries and frameworks to create incredibly type-safe and powerful APIs.*