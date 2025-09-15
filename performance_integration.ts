
// Performance integration with complex generic scenarios
interface Entity {
    id: number;
    createdAt: Date;
    updatedAt: Date;
}

interface User extends Entity {
    name: string;
    email: string;
    age: number;
    role: "admin" | "user" | "guest";
}

interface Product extends Entity {
    title: string;
    description: string;
    price: number;
    category: string;
    inStock: boolean;
}

interface Order extends Entity {
    userId: number;
    productId: number;
    quantity: number;
    status: "pending" | "processing" | "shipped" | "delivered" | "cancelled";
}

// Generic repository with performance considerations
class Repository<T extends Entity> {
    private items: Map<number, T> = new Map();
    private nextId = 1;
    
    create(item: Omit<T, "id" | "createdAt" | "updatedAt">): T {
        const now = new Date();
        const newItem = {
            ...item,
            id: this.nextId++,
            createdAt: now,
            updatedAt: now
        } as T;
        this.items.set(newItem.id, newItem);
        return newItem;
    }
    
    findById(id: number): T | undefined {
        return this.items.get(id);
    }
    
    findAll(): T[] {
        return Array.from(this.items.values());
    }
    
    update(id: number, updates: Partial<Omit<T, "id" | "createdAt">>): T | undefined {
        const item = this.items.get(id);
        if (item) {
            const updatedItem = {
                ...item,
                ...updates,
                updatedAt: new Date()
            };
            this.items.set(id, updatedItem);
            return updatedItem;
        }
        return undefined;
    }
    
    delete(id: number): boolean {
        return this.items.delete(id);
    }
    
    // Performance-optimized query methods
    findWhere<K extends keyof T>(key: K, value: T[K]): T[] {
        return this.findAll().filter(item => item[key] === value);
    }
    
    findWhereMultiple(predicate: (item: T) => boolean): T[] {
        return this.findAll().filter(predicate);
    }
    
    count(): number {
        return this.items.size;
    }
    
    countWhere<K extends keyof T>(key: K, value: T[K]): number {
        return this.findWhere(key, value).length;
    }
    
    // Performance-optimized transformation methods
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

// Generic service with performance considerations
class Service<T extends Entity> {
    constructor(private repository: Repository<T>) {}
    
    create(item: Omit<T, "id" | "createdAt" | "updatedAt">): T {
        return this.repository.create(item);
    }
    
    getById(id: number): T | undefined {
        return this.repository.findById(id);
    }
    
    getAll(): T[] {
        return this.repository.findAll();
    }
    
    update(id: number, updates: Partial<Omit<T, "id" | "createdAt">>): T | undefined {
        return this.repository.update(id, updates);
    }
    
    delete(id: number): boolean {
        return this.repository.delete(id);
    }
    
    findWhere<K extends keyof T>(key: K, value: T[K]): T[] {
        return this.repository.findWhere(key, value);
    }
    
    findWhereMultiple(predicate: (item: T) => boolean): T[] {
        return this.repository.findWhereMultiple(predicate);
    }
    
    count(): number {
        return this.repository.count();
    }
    
    countWhere<K extends keyof T>(key: K, value: T[K]): number {
        return this.repository.countWhere(key, value);
    }
    
    map<U>(fn: (item: T) => U): U[] {
        return this.repository.map(fn);
    }
    
    groupBy<K extends keyof T>(key: K): Map<T[K], T[]> {
        return this.repository.groupBy(key);
    }
}

// Test performance with large datasets
const userRepository = new Repository<User>();
const productRepository = new Repository<Product>();
const orderRepository = new Repository<Order>();

const userService = new Service(userRepository);
const productService = new Service(productRepository);
const orderService = new Service(orderRepository);

// Create large datasets for performance testing
const startTime = Date.now();

// Create 1000 users
for (let i = 0; i < 1000; i++) {
    userService.create({
        name: `User ${i}`,
        email: `user${i}@example.com`,
        age: 20 + (i % 50),
        role: i % 3 === 0 ? "admin" : i % 3 === 1 ? "user" : "guest"
    });
}

// Create 1000 products
for (let i = 0; i < 1000; i++) {
    productService.create({
        title: `Product ${i}`,
        description: `Description for product ${i}`,
        price: 10 + (i % 1000),
        category: i % 5 === 0 ? "Electronics" : i % 5 === 1 ? "Books" : i % 5 === 2 ? "Clothing" : i % 5 === 3 ? "Home" : "Sports",
        inStock: i % 2 === 0
    });
}

// Create 1000 orders
for (let i = 0; i < 1000; i++) {
    orderService.create({
        userId: (i % 1000) + 1,
        productId: (i % 1000) + 1,
        quantity: 1 + (i % 10),
        status: i % 4 === 0 ? "pending" : i % 4 === 1 ? "processing" : i % 4 === 2 ? "shipped" : "delivered"
    });
}

const creationTime = Date.now() - startTime;

// Test query performance
const queryStartTime = Date.now();

const adminUsers = userService.findWhere("role", "admin");
const electronicsProducts = productService.findWhere("category", "Electronics");
const pendingOrders = orderService.findWhere("status", "pending");

const queryTime = Date.now() - queryStartTime;

// Test aggregation performance
const aggStartTime = Date.now();

const totalUsers = userService.count();
const totalProducts = productService.count();
const totalOrders = orderService.count();

const adminCount = userService.countWhere("role", "admin");
const electronicsCount = productService.countWhere("category", "Electronics");
const pendingCount = orderService.countWhere("status", "pending");

const aggTime = Date.now() - aggStartTime;

// Test transformation performance
const transformStartTime = Date.now();

const userNames = userService.map(user => user.name);
const productTitles = productService.map(product => product.title);
const orderStatuses = orderService.map(order => order.status);

const usersByRole = userService.groupBy("role");
const productsByCategory = productService.groupBy("category");
const ordersByStatus = orderService.groupBy("status");

const transformTime = Date.now() - transformStartTime;

const totalTime = Date.now() - startTime;

console.log("Performance Results:");
console.log("Creation time:", creationTime, "ms");
console.log("Query time:", queryTime, "ms");
console.log("Aggregation time:", aggTime, "ms");
console.log("Transformation time:", transformTime, "ms");
console.log("Total time:", totalTime, "ms");

console.log("Results:");
console.log("Admin users:", adminUsers.length);
console.log("Electronics products:", electronicsProducts.length);
console.log("Pending orders:", pendingOrders.length);
console.log("Total counts:", totalUsers, totalProducts, totalOrders);
console.log("Specific counts:", adminCount, electronicsCount, pendingCount);
console.log("Transformation results:", userNames.length, productTitles.length, orderStatuses.length);
console.log("Grouping results:", usersByRole.size, productsByCategory.size, ordersByStatus.size);
