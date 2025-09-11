// Advanced TypeScript Features
// Demonstrates conditional types, infer, recursive types, and advanced patterns

// Note: Many of these features may not be fully supported by the TSC compiler yet
// This example shows the patterns and concepts for reference

// 21. Conditional Types (Concept)
// These would work in full TypeScript but may not compile with TSC yet

// Basic conditional type
type IsString<T> = T extends string ? true : false;

// More complex conditional types
type NonNullable<T> = T extends null | undefined ? never : T;
type Flatten<T> = T extends (infer U)[] ? U : T;

// Conditional type with multiple branches
type TypeName<T> = 
  T extends string ? "string" :
  T extends number ? "number" :
  T extends boolean ? "boolean" :
  T extends undefined ? "undefined" :
  T extends null ? "null" :
  "object";

// 22. Infer Keyword (Concept)
// These patterns show how infer would work in full TypeScript

// Extract return type
type ReturnType<T> = T extends (...args: any[]) => infer R ? R : never;

// Extract parameter types
type Parameters<T> = T extends (...args: infer P) => any ? P : never;

// Extract constructor parameter types
type ConstructorParameters<T> = T extends new (...args: infer P) => any ? P : never;

// 23. Recursive Types (Concept)
// These show recursive type patterns

// JSON value type
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

// 24. Advanced Mapped Types (Concept)
// These show advanced mapped type patterns

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

// 25. Branded Types (Concept)
// These show how to create branded types for type safety

type Brand<T, B> = T & { __brand: B };

type UserId = Brand<number, 'UserId'>;
type ProductId = Brand<number, 'ProductId'>;
type OrderId = Brand<number, 'OrderId'>;

function createUserId(id: number): UserId {
  return id as UserId;
}

function createProductId(id: number): ProductId {
  return id as ProductId;
}

function createOrderId(id: number): OrderId {
  return id as OrderId;
}

// 26. Advanced Utility Types (Concept)
// These show advanced utility type patterns

// Extract specific types from union
type Extract<T, U> = T extends U ? T : never;

// Exclude specific types from union
type Exclude<T, U> = T extends U ? never : T;

// Get all possible values of a union
type UnionToIntersection<U> = 
  (U extends any ? (k: U) => void : never) extends (k: infer I) => void ? I : never;

// 27. Complex Generic Constraints (Concept)
// These show complex generic constraint patterns

// Constrain to object with specific keys
type HasName<T> = T extends { name: any } ? T : never;

// Constrain to array-like
type ArrayLike<T> = T extends { length: number; [n: number]: any } ? T : never;

// Constrain to function
type FunctionLike<T> = T extends (...args: any[]) => any ? T : never;

// Multiple constraints
function processEntity<T extends object & { id: number } & { name: string }>(item: T): T {
  console.log(`Processing ${item.name} with ID ${item.id}`);
  return item;
}

// 28. Template Literal Types (Advanced Concept)
// These show advanced template literal type patterns

type Join<K, P> = K extends string ? P extends string ? `${K}${P}` : never : never;

type Paths<T> = T extends object ? {
  [K in keyof T]-?: K extends string ? 
    `${K}` | Join<K, Paths<T[K]>> : never
}[keyof T] : '';

// 29. Advanced Type Manipulation (Concept)
// These show advanced type manipulation patterns

// Get all keys of a type as a union
type Keys<T> = keyof T;

// Get all values of a type as a union
type Values<T> = T[keyof T];

// Create a type that makes all properties optional except specified ones
type PartialExcept<T, K extends keyof T> = Partial<T> & Pick<T, K>;

// 30. Practical Advanced Patterns
// These show how advanced types can be used in practice

// API Response type
type ApiResponse<T> = {
  data: T;
  status: number;
  message: string;
  timestamp: string;
};

// Paginated response
type PaginatedResponse<T> = ApiResponse<T[]> & {
  pagination: {
    page: number;
    limit: number;
    total: number;
    totalPages: number;
  };
};

// Error response
type ErrorResponse = {
  error: {
    code: string;
    message: string;
    details?: any;
  };
  status: number;
  timestamp: string;
};

// Union of all possible responses
type ApiResult<T> = ApiResponse<T> | PaginatedResponse<T> | ErrorResponse;

// Event system with typed events
type EventMap = {
  'user.created': { id: number; name: string; email: string };
  'user.updated': { id: number; changes: Partial<{ name: string; email: string }> };
  'user.deleted': { id: number };
  'order.created': { id: number; userId: number; total: number };
  'order.completed': { id: number; completedAt: string };
};

type EventHandler<T> = (event: T) => void;

// Generic event emitter
class TypedEventEmitter {
  private listeners: Map<string, Function[]> = new Map();
  
  on<K extends keyof EventMap>(event: K, handler: EventHandler<EventMap[K]>): void {
    if (!this.listeners.has(event)) {
      this.listeners.set(event, []);
    }
    this.listeners.get(event)!.push(handler);
  }
  
  emit<K extends keyof EventMap>(event: K, data: EventMap[K]): void {
    let handlers = this.listeners.get(event);
    if (handlers) {
      handlers.forEach(handler => {
        try {
          handler(data);
        } catch (error) {
          console.error("Error in event handler:", error);
        }
      });
    }
  }
}

// Usage examples (these would work in full TypeScript)
let eventEmitter = new TypedEventEmitter();

// Type-safe event handling
eventEmitter.on('user.created', (user) => {
  console.log(`User created: ${user.name} (${user.email})`);
});

eventEmitter.on('order.created', (order) => {
  console.log(`Order created: ${order.id} for user ${order.userId}`);
});

// Type-safe event emission
eventEmitter.emit('user.created', {
  id: 1,
  name: "Alice",
  email: "alice@example.com"
});

eventEmitter.emit('order.created', {
  id: 100,
  userId: 1,
  total: 99.99
});

// Branded type usage
let userId: UserId = createUserId(123);
let productId: ProductId = createProductId(456);
let orderId: OrderId = createOrderId(789);

// These would prevent mixing up different ID types in full TypeScript
// let mixed: UserId = productId; // This would be an error

// Advanced generic usage
let entity = processEntity({
  id: 1,
  name: "Test Entity",
  description: "A test entity"
});

// JSON value usage
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

// This example demonstrates advanced TypeScript concepts and patterns
// Note: Many features may not be fully supported by the TSC compiler yet