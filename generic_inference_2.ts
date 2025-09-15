
// Complex type inference scenarios
interface User {
    id: number;
    name: string;
    email: string;
}

interface Product {
    id: number;
    title: string;
    price: number;
}

interface Order {
    id: number;
    userId: number;
    productId: number;
    quantity: number;
}

// Generic repository with type inference
class Repository<T> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    findById(id: number): T | undefined {
        return this.items.find((item: any) => item.id === id);
    }
    
    findAll(): T[] {
        return [...this.items];
    }
    
    update(id: number, updates: Partial<T>): T | undefined {
        const index = this.items.findIndex((item: any) => item.id === id);
        if (index !== -1) {
            this.items[index] = { ...this.items[index], ...updates };
            return this.items[index];
        }
        return undefined;
    }
    
    delete(id: number): boolean {
        const index = this.items.findIndex((item: any) => item.id === id);
        if (index !== -1) {
            this.items.splice(index, 1);
            return true;
        }
        return false;
    }
}

// Generic service with type inference
class Service<T> {
    constructor(private repository: Repository<T>) {}
    
    create(item: Omit<T, "id">): T {
        const newItem = { ...item, id: Date.now() } as T;
        this.repository.add(newItem);
        return newItem;
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

// Test complex type inference
const userRepository = new Repository<User>();
const productRepository = new Repository<Product>();
const orderRepository = new Repository<Order>();

const userService = new Service(userRepository);
const productService = new Service(productRepository);
const orderService = new Service(orderRepository);

// Test user operations with type inference
const newUser = userService.create({ name: "John Doe", email: "john@example.com" });
const foundUser = userService.getById(newUser.id);
const updatedUser = userService.update(newUser.id, { name: "Jane Doe" });
const allUsers = userService.getAll();

// Test product operations with type inference
const newProduct = productService.create({ title: "Laptop", price: 999.99 });
const foundProduct = productService.getById(newProduct.id);
const updatedProduct = productService.update(newProduct.id, { price: 899.99 });
const allProducts = productService.getAll();

// Test order operations with type inference
const newOrder = orderService.create({ userId: newUser.id, productId: newProduct.id, quantity: 2 });
const foundOrder = orderService.getById(newOrder.id);
const updatedOrder = orderService.update(newOrder.id, { quantity: 3 });
const allOrders = orderService.getAll();

console.log("New user:", newUser);
console.log("Found user:", foundUser);
console.log("Updated user:", updatedUser);
console.log("All users:", allUsers);
console.log("New product:", newProduct);
console.log("Found product:", foundProduct);
console.log("Updated product:", updatedProduct);
console.log("All products:", allProducts);
console.log("New order:", newOrder);
console.log("Found order:", foundOrder);
console.log("Updated order:", updatedOrder);
console.log("All orders:", allOrders);
