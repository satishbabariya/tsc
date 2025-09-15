
// Complete integration test of enhanced generics
interface Identifiable {
    id: number;
}

interface Timestamped {
    createdAt: Date;
    updatedAt: Date;
}

interface Auditable extends Identifiable, Timestamped {
    createdBy: string;
    updatedBy: string;
}

// Generic repository with constraints
class Repository<T extends Identifiable> {
    private items: Map<number, T> = new Map();
    private nextId = 1;
    
    create(item: Omit<T, "id">): T {
        const newItem = { ...item, id: this.nextId++ } as T;
        this.items.set(newItem.id, newItem);
        return newItem;
    }
    
    findById(id: number): T | undefined {
        return this.items.get(id);
    }
    
    findAll(): T[] {
        return Array.from(this.items.values());
    }
    
    update(id: number, updates: Partial<T>): T | undefined {
        const item = this.items.get(id);
        if (item) {
            const updatedItem = { ...item, ...updates };
            this.items.set(id, updatedItem);
            return updatedItem;
        }
        return undefined;
    }
    
    delete(id: number): boolean {
        return this.items.delete(id);
    }
    
    // Generic query methods
    findWhere<K extends keyof T>(key: K, value: T[K]): T[] {
        return this.findAll().filter(item => item[key] === value);
    }
    
    findWhereMultiple(predicate: (item: T) => boolean): T[] {
        return this.findAll().filter(predicate);
    }
    
    // Generic aggregation methods
    count(): number {
        return this.items.size;
    }
    
    countWhere<K extends keyof T>(key: K, value: T[K]): number {
        return this.findWhere(key, value).length;
    }
    
    // Generic transformation methods
    map<U>(fn: (item: T) => U): U[] {
        return this.findAll().map(fn);
    }
    
    groupBy<K extends keyof T>(key: K): Map<T[K], T[]> {
        const groups = new Map<T[K], T[]>();
        for (const item of this.findAll()) {
            const groupKey = item[key];
            if (!groups.has(groupKey)) {
                groups.set(groupKey, []);
            }
            groups.get(groupKey)!.push(item);
        }
        return groups;
    }
}

// Generic service with constraints and overloading
class Service<T extends Auditable> {
    constructor(private repository: Repository<T>) {}
    
    // Overload 1: Create with user
    create(item: Omit<T, "id" | "createdAt" | "updatedAt" | "createdBy" | "updatedBy">, user: string): T;
    
    // Overload 2: Create with user and timestamp
    create(item: Omit<T, "id" | "createdAt" | "updatedAt" | "createdBy" | "updatedBy">, user: string, timestamp: Date): T;
    
    // Implementation
    create(item: Omit<T, "id" | "createdAt" | "updatedAt" | "createdBy" | "updatedBy">, user: string, timestamp?: Date): T {
        const now = timestamp || new Date();
        const newItem = {
            ...item,
            createdAt: now,
            updatedAt: now,
            createdBy: user,
            updatedBy: user
        } as Omit<T, "id">;
        
        return this.repository.create(newItem);
    }
    
    // Generic update with user tracking
    update(id: number, updates: Partial<Omit<T, "id" | "createdAt" | "createdBy">>, user: string): T | undefined {
        const updateData = {
            ...updates,
            updatedAt: new Date(),
            updatedBy: user
        } as Partial<T>;
        
        return this.repository.update(id, updateData);
    }
    
    // Generic query methods
    findById(id: number): T | undefined {
        return this.repository.findById(id);
    }
    
    findAll(): T[] {
        return this.repository.findAll();
    }
    
    findWhere<K extends keyof T>(key: K, value: T[K]): T[] {
        return this.repository.findWhere(key, value);
    }
    
    findWhereMultiple(predicate: (item: T) => boolean): T[] {
        return this.repository.findWhereMultiple(predicate);
    }
    
    // Generic aggregation methods
    count(): number {
        return this.repository.count();
    }
    
    countWhere<K extends keyof T>(key: K, value: T[K]): number {
        return this.repository.countWhere(key, value);
    }
    
    // Generic transformation methods
    map<U>(fn: (item: T) => U): U[] {
        return this.repository.map(fn);
    }
    
    groupBy<K extends keyof T>(key: K): Map<T[K], T[]> {
        return this.repository.groupBy(key);
    }
}

// Concrete entity types
interface User extends Auditable {
    name: string;
    email: string;
    age: number;
    role: "admin" | "user" | "guest";
}

interface Product extends Auditable {
    title: string;
    description: string;
    price: number;
    category: string;
    inStock: boolean;
}

interface Order extends Auditable {
    userId: number;
    productId: number;
    quantity: number;
    status: "pending" | "processing" | "shipped" | "delivered" | "cancelled";
}

// Test the complete integration
const userRepository = new Repository<User>();
const productRepository = new Repository<Product>();
const orderRepository = new Repository<Order>();

const userService = new Service(userRepository);
const productService = new Service(productRepository);
const orderService = new Service(orderRepository);

// Test user operations
const user1 = userService.create({ name: "Alice", email: "alice@example.com", age: 25, role: "admin" }, "system");
const user2 = userService.create({ name: "Bob", email: "bob@example.com", age: 30, role: "user" }, "system");
const user3 = userService.create({ name: "Charlie", email: "charlie@example.com", age: 35, role: "user" }, "system");

console.log("Created users:", userService.findAll());

// Test product operations
const product1 = productService.create({ title: "Laptop", description: "High-performance laptop", price: 999.99, category: "Electronics", inStock: true }, "system");
const product2 = productService.create({ title: "Mouse", description: "Wireless mouse", price: 29.99, category: "Electronics", inStock: true }, "system");
const product3 = productService.create({ title: "Book", description: "Programming book", price: 49.99, category: "Books", inStock: false }, "system");

console.log("Created products:", productService.findAll());

// Test order operations
const order1 = orderService.create({ userId: user1.id, productId: product1.id, quantity: 1, status: "pending" }, "system");
const order2 = orderService.create({ userId: user2.id, productId: product2.id, quantity: 2, status: "processing" }, "system");
const order3 = orderService.create({ userId: user3.id, productId: product3.id, quantity: 1, status: "shipped" }, "system");

console.log("Created orders:", orderService.findAll());

// Test generic query operations
const adminUsers = userService.findWhere("role", "admin");
const electronicsProducts = productService.findWhere("category", "Electronics");
const pendingOrders = orderService.findWhere("status", "pending");

console.log("Admin users:", adminUsers);
console.log("Electronics products:", electronicsProducts);
console.log("Pending orders:", pendingOrders);

// Test generic aggregation operations
const totalUsers = userService.count();
const totalProducts = productService.count();
const totalOrders = orderService.count();

const adminCount = userService.countWhere("role", "admin");
const electronicsCount = productService.countWhere("category", "Electronics");
const pendingCount = orderService.countWhere("status", "pending");

console.log("Total users:", totalUsers);
console.log("Total products:", totalProducts);
console.log("Total orders:", totalOrders);
console.log("Admin count:", adminCount);
console.log("Electronics count:", electronicsCount);
console.log("Pending count:", pendingCount);

// Test generic transformation operations
const userNames = userService.map(user => user.name);
const productTitles = productService.map(product => product.title);
const orderStatuses = orderService.map(order => order.status);

console.log("User names:", userNames);
console.log("Product titles:", productTitles);
console.log("Order statuses:", orderStatuses);

// Test generic grouping operations
const usersByRole = userService.groupBy("role");
const productsByCategory = productService.groupBy("category");
const ordersByStatus = orderService.groupBy("status");

console.log("Users by role:", usersByRole);
console.log("Products by category:", productsByCategory);
console.log("Orders by status:", ordersByStatus);

// Test update operations
const updatedUser = userService.update(user1.id, { age: 26 }, "admin");
const updatedProduct = productService.update(product1.id, { price: 899.99 }, "admin");
const updatedOrder = orderService.update(order1.id, { status: "processing" }, "admin");

console.log("Updated user:", updatedUser);
console.log("Updated product:", updatedProduct);
console.log("Updated order:", updatedOrder);
