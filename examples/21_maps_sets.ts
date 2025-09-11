// Maps & Sets
// Demonstrates Map and Set data structures with TypeScript type safety

// Maps - Key-Value Collections
// Map<K, V> where K is key type, V is value type

// Basic Map operations
const userRoles = new Map<string, string>();

userRoles.set("alice", "admin");
userRoles.set("bob", "user");
userRoles.set("charlie", "moderator");

console.log(userRoles.get("alice")); // "admin"
console.log(userRoles.has("bob"));   // true
console.log(userRoles.size);         // 3

// Map with different key/value types
const userAges = new Map<string, number>();
userAges.set("alice", 25);
userAges.set("bob", 30);
userAges.set("charlie", 35);

// Map with object keys
interface User {
  id: number;
  name: string;
}

const userSessions = new Map<User, string>();
const alice: User = { id: 1, name: "Alice" };
const bob: User = { id: 2, name: "Bob" };

userSessions.set(alice, "session_123");
userSessions.set(bob, "session_456");

// Map with complex value types
interface UserProfile {
  name: string;
  email: string;
  lastLogin: Date;
}

const userProfiles = new Map<string, UserProfile>();
userProfiles.set("alice", {
  name: "Alice Smith",
  email: "alice@example.com",
  lastLogin: new Date()
});

// Map iteration
console.log("User Roles:");
for (const [user, role] of userRoles) {
  console.log(`${user}: ${role}`);
}

// Map with forEach
userRoles.forEach((role, user) => {
  console.log(`${user} has role: ${role}`);
});

// Map methods
console.log("Map entries:", Array.from(userRoles.entries()));
console.log("Map keys:", Array.from(userRoles.keys()));
console.log("Map values:", Array.from(userRoles.values()));

// Map deletion
userRoles.delete("charlie");
console.log("After deletion:", userRoles.size); // 2

// Map clearing
userRoles.clear();
console.log("After clear:", userRoles.size); // 0

// Sets - Unique Value Collections
// Set<T> where T is the element type

// Basic Set operations
const uniqueNumbers = new Set<number>();

uniqueNumbers.add(1);
uniqueNumbers.add(2);
uniqueNumbers.add(3);
uniqueNumbers.add(2); // duplicates ignored

console.log(uniqueNumbers.has(1)); // true
console.log(uniqueNumbers.size);   // 3

// Set with string values
const uniqueNames = new Set<string>();
uniqueNames.add("Alice");
uniqueNames.add("Bob");
uniqueNames.add("Charlie");
uniqueNames.add("Alice"); // duplicate ignored

// Set with object values
const uniqueUsers = new Set<User>();
uniqueUsers.add({ id: 1, name: "Alice" });
uniqueUsers.add({ id: 2, name: "Bob" });
uniqueUsers.add({ id: 1, name: "Alice" }); // different object, so added

// Set iteration
console.log("Unique numbers:");
for (const num of uniqueNumbers) {
  console.log(num);
}

// Set with forEach
uniqueNames.forEach(name => {
  console.log(`Name: ${name}`);
});

// Set methods
console.log("Set values:", Array.from(uniqueNumbers));
console.log("Set has 1:", uniqueNumbers.has(1));
console.log("Set has 5:", uniqueNumbers.has(5));

// Set operations
const set1 = new Set<number>([1, 2, 3, 4]);
const set2 = new Set<number>([3, 4, 5, 6]);

// Union
const union = new Set<number>([...set1, ...set2]);
console.log("Union:", Array.from(union)); // [1, 2, 3, 4, 5, 6]

// Intersection
const intersection = new Set<number>([...set1].filter(x => set2.has(x)));
console.log("Intersection:", Array.from(intersection)); // [3, 4]

// Difference
const difference = new Set<number>([...set1].filter(x => !set2.has(x)));
console.log("Difference:", Array.from(difference)); // [1, 2]

// Set deletion
uniqueNumbers.delete(1);
console.log("After deletion:", uniqueNumbers.size); // 2

// Set clearing
uniqueNumbers.clear();
console.log("After clear:", uniqueNumbers.size); // 0

// WeakMap - Garbage collected keys
// WeakMap<K, V> where K must be object type

const weakUserData = new WeakMap<User, string>();
const user1: User = { id: 1, name: "Alice" };
const user2: User = { id: 2, name: "Bob" };

weakUserData.set(user1, "private_data_1");
weakUserData.set(user2, "private_data_2");

console.log(weakUserData.get(user1)); // "private_data_1"
console.log(weakUserData.has(user2)); // true

// WeakSet - Garbage collected values
// WeakSet<T> where T must be object type

const weakUserSet = new WeakSet<User>();
weakUserSet.add(user1);
weakUserSet.add(user2);

console.log(weakUserSet.has(user1)); // true
console.log(weakUserSet.has(user2)); // true

// Practical examples

// 1. Cache implementation
class Cache<K, V> {
  private cache = new Map<K, V>();
  private maxSize: number;

  constructor(maxSize: number = 100) {
    this.maxSize = maxSize;
  }

  set(key: K, value: V): void {
    if (this.cache.size >= this.maxSize) {
      const firstKey = this.cache.keys().next().value;
      this.cache.delete(firstKey);
    }
    this.cache.set(key, value);
  }

  get(key: K): V | undefined {
    return this.cache.get(key);
  }

  has(key: K): boolean {
    return this.cache.has(key);
  }

  clear(): void {
    this.cache.clear();
  }

  size(): number {
    return this.cache.size;
  }
}

const stringCache = new Cache<string, string>(5);
stringCache.set("key1", "value1");
stringCache.set("key2", "value2");
console.log("Cache size:", stringCache.size());

// 2. Unique collection manager
class UniqueCollection<T> {
  private items = new Set<T>();

  add(item: T): boolean {
    if (this.items.has(item)) {
      return false; // already exists
    }
    this.items.add(item);
    return true; // added successfully
  }

  remove(item: T): boolean {
    return this.items.delete(item);
  }

  has(item: T): boolean {
    return this.items.has(item);
  }

  getAll(): T[] {
    return Array.from(this.items);
  }

  size(): number {
    return this.items.size;
  }

  clear(): void {
    this.items.clear();
  }
}

const uniqueStrings = new UniqueCollection<string>();
uniqueStrings.add("hello");
uniqueStrings.add("world");
uniqueStrings.add("hello"); // duplicate, returns false
console.log("Unique strings:", uniqueStrings.getAll());

// 3. Event system with Map
class EventEmitter {
  private listeners = new Map<string, Set<Function>>();

  on(event: string, listener: Function): void {
    if (!this.listeners.has(event)) {
      this.listeners.set(event, new Set<Function>());
    }
    this.listeners.get(event)!.add(listener);
  }

  off(event: string, listener: Function): void {
    const eventListeners = this.listeners.get(event);
    if (eventListeners) {
      eventListeners.delete(listener);
    }
  }

  emit(event: string, ...args: any[]): void {
    const eventListeners = this.listeners.get(event);
    if (eventListeners) {
      eventListeners.forEach(listener => {
        try {
          listener(...args);
        } catch (error) {
          console.error("Error in event listener:", error);
        }
      });
    }
  }

  removeAllListeners(event?: string): void {
    if (event) {
      this.listeners.delete(event);
    } else {
      this.listeners.clear();
    }
  }
}

const emitter = new EventEmitter();
emitter.on("user.created", (user: User) => {
  console.log("User created:", user.name);
});

emitter.on("user.updated", (user: User) => {
  console.log("User updated:", user.name);
});

emitter.emit("user.created", { id: 1, name: "Alice" });
emitter.emit("user.updated", { id: 1, name: "Alice Smith" });

// 4. Configuration manager with Map
class ConfigManager {
  private config = new Map<string, any>();

  set(key: string, value: any): void {
    this.config.set(key, value);
  }

  get<T>(key: string): T | undefined {
    return this.config.get(key) as T;
  }

  has(key: string): boolean {
    return this.config.has(key);
  }

  delete(key: string): boolean {
    return this.config.delete(key);
  }

  getAll(): Record<string, any> {
    const result: Record<string, any> = {};
    this.config.forEach((value, key) => {
      result[key] = value;
    });
    return result;
  }

  clear(): void {
    this.config.clear();
  }
}

const config = new ConfigManager();
config.set("database.host", "localhost");
config.set("database.port", 5432);
config.set("api.timeout", 5000);

console.log("Database host:", config.get<string>("database.host"));
console.log("Database port:", config.get<number>("database.port"));
console.log("All config:", config.getAll());

// This example demonstrates comprehensive Map and Set functionality