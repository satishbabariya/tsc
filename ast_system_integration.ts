
// AST system integration with generic type inference
interface Repository<T> {
    findById(id: number): T | undefined;
    findAll(): T[];
    create(item: Omit<T, "id">): T;
    update(id: number, updates: Partial<T>): T | undefined;
    delete(id: number): boolean;
}

interface User {
    id: number;
    name: string;
    email: string;
    age: number;
}

interface Product {
    id: number;
    title: string;
    price: number;
    category: string;
}

class GenericRepository<T> implements Repository<T> {
    private items: Map<number, T> = new Map();
    private nextId = 1;
    
    findById(id: number): T | undefined {
        return this.items.get(id);
    }
    
    findAll(): T[] {
        return Array.from(this.items.values());
    }
    
    create(item: Omit<T, "id">): T {
        const newItem = { ...item, id: this.nextId++ } as T;
        this.items.set((newItem as any).id, newItem);
        return newItem;
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
}

// Generic service with type inference
class Service<T> {
    constructor(private repository: Repository<T>) {}
    
    create(item: Omit<T, "id">): T {
        return this.repository.create(item);
    }
    
    getById(id: number): T | undefined {
        return this.repository.findById(id);
    }
    
    getAll(): T[] {
        return this.repository.findAll();
    }
    
    update(id: number, updates: Partial<T>): T | undefined {
        return this.repository.update(id, updates);
    }
    
    delete(id: number): boolean {
        return this.repository.delete(id);
    }
}

// Test AST integration with type inference
const userRepository = new GenericRepository<User>();
const productRepository = new GenericRepository<Product>();

const userService = new Service(userRepository);
const productService = new Service(productRepository);

// Test type inference
const newUser = userService.create({ name: "John Doe", email: "john@example.com", age: 30 });
const newProduct = productService.create({ title: "Laptop", price: 999.99, category: "Electronics" });

const foundUser = userService.getById(newUser.id);
const foundProduct = productService.getById(newProduct.id);

const updatedUser = userService.update(newUser.id, { age: 31 });
const updatedProduct = productService.update(newProduct.id, { price: 899.99 });

console.log("New user:", newUser);
console.log("New product:", newProduct);
console.log("Found user:", foundUser);
console.log("Found product:", foundProduct);
console.log("Updated user:", updatedUser);
console.log("Updated product:", updatedProduct);
