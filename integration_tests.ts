
// Integration tests for generics with other compiler features
interface User {
    id: number;
    name: string;
    email: string;
}

interface Product {
    id: number;
    name: string;
    price: number;
}

// Generic repository pattern
class Repository<T> {
    private items: Map<number, T> = new Map();
    
    save(item: T & { id: number }): void {
        this.items.set(item.id, item);
    }
    
    findById(id: number): T | undefined {
        return this.items.get(id);
    }
    
    findAll(): T[] {
        return Array.from(this.items.values());
    }
    
    delete(id: number): boolean {
        return this.items.delete(id);
    }
}

// Generic service layer
class Service<T> {
    constructor(private repository: Repository<T>) {}
    
    create(item: T & { id: number }): T {
        this.repository.save(item);
        return item;
    }
    
    getById(id: number): T | undefined {
        return this.repository.findById(id);
    }
    
    getAll(): T[] {
        return this.repository.findAll();
    }
    
    remove(id: number): boolean {
        return this.repository.delete(id);
    }
}

// Generic factory pattern
class Factory<T> {
    constructor(private createFn: () => T) {}
    
    create(): T {
        return this.createFn();
    }
    
    createMany(count: number): T[] {
        return Array.from({ length: count }, () => this.create());
    }
}

// Generic decorator pattern
function Loggable<T extends { new(...args: any[]): {} }>(constructor: T) {
    return class extends constructor {
        log(message: string): void {
            console.log(`[${constructor.name}] ${message}`);
        }
    };
}

@Loggable
class UserService extends Service<User> {
    constructor() {
        super(new Repository<User>());
    }
    
    findByEmail(email: string): User | undefined {
        return this.getAll().find(user => user.email === email);
    }
}

@Loggable
class ProductService extends Service<Product> {
    constructor() {
        super(new Repository<Product>());
    }
    
    findByPriceRange(min: number, max: number): Product[] {
        return this.getAll().filter(product => 
            product.price >= min && product.price <= max
        );
    }
}

// Test integration
const userService = new UserService();
const productService = new ProductService();

const user = userService.create({
    id: 1,
    name: "Alice",
    email: "alice@example.com"
});

const product = productService.create({
    id: 1,
    name: "Laptop",
    price: 999.99
});

console.log("User:", user);
console.log("Product:", product);

// Test factory integration
const userFactory = new Factory<User>(() => ({
    id: Math.floor(Math.random() * 1000),
    name: "Generated User",
    email: "generated@example.com"
}));

const generatedUsers = userFactory.createMany(3);
console.log("Generated users:", generatedUsers);
