// Advanced Features
// Demonstrates complex combinations of TypeScript features

// Complex type definitions
type UserRole = "admin" | "user" | "guest";
type Permission = "read" | "write" | "delete" | "execute";

interface User {
    id: number;
    name: string;
    email: string;
    role: UserRole;
    permissions: Permission[];
    createdAt: Date;
    lastLogin?: Date;
}

interface Product {
    id: number;
    name: string;
    price: number;
    category: string;
    inStock: boolean;
    tags: string[];
}

interface Order {
    id: number;
    userId: number;
    products: Product[];
    total: number;
    status: "pending" | "processing" | "shipped" | "delivered" | "cancelled";
    createdAt: Date;
}

// Generic repository with complex constraints
interface Entity {
    id: number;
    createdAt: Date;
    updatedAt: Date;
}

interface Repository<T extends Entity> {
    findById(id: number): T | null;
    findAll(): T[];
    save(entity: T): T;
    delete(id: number): boolean;
    findBy<K extends keyof T>(key: K, value: T[K]): T[];
}

// Generic service with dependency injection
interface Service<T extends Entity> {
    repository: Repository<T>;
    validate(entity: T): boolean;
    process(entity: T): T;
}

// Complex class hierarchy
abstract class BaseService<T extends Entity> implements Service<T> {
    constructor(public repository: Repository<T>) {}
    
    abstract validate(entity: T): boolean;
    
    process(entity: T): T {
        if (!this.validate(entity)) {
            throw new Error("Validation failed");
        }
        
        entity.updatedAt = new Date();
        return this.repository.save(entity);
    }
    
    findById(id: number): T | null {
        return this.repository.findById(id);
    }
    
    findAll(): T[] {
        return this.repository.findAll();
    }
}

// Concrete implementations
class UserService extends BaseService<User> {
    validate(user: User): boolean {
        return user.name.length > 0 && 
               user.email.includes("@") && 
               user.role !== undefined;
    }
    
    hasPermission(user: User, permission: Permission): boolean {
        return user.permissions.includes(permission);
    }
    
    canAccess(user: User, resource: string): boolean {
        if (user.role === "admin") return true;
        if (user.role === "user" && resource !== "admin-panel") return true;
        if (user.role === "guest" && resource === "public") return true;
        return false;
    }
}

class ProductService extends BaseService<Product> {
    validate(product: Product): boolean {
        return product.name.length > 0 && 
               product.price > 0 && 
               product.category.length > 0;
    }
    
    searchByCategory(category: string): Product[] {
        return this.repository.findBy("category", category);
    }
    
    searchByPriceRange(minPrice: number, maxPrice: number): Product[] {
        return this.findAll().filter(p => p.price >= minPrice && p.price <= maxPrice);
    }
}

// Generic repository implementation
class InMemoryRepository<T extends Entity> implements Repository<T> {
    private entities: T[] = [];
    private nextId: number = 1;
    
    findById(id: number): T | null {
        return this.entities.find(e => e.id === id) || null;
    }
    
    findAll(): T[] {
        return [...this.entities];
    }
    
    save(entity: T): T {
        if (entity.id === 0) {
            entity.id = this.nextId++;
            entity.createdAt = new Date();
        }
        entity.updatedAt = new Date();
        
        let index = this.entities.findIndex(e => e.id === entity.id);
        if (index >= 0) {
            this.entities[index] = entity;
        } else {
            this.entities.push(entity);
        }
        
        return entity;
    }
    
    delete(id: number): boolean {
        let index = this.entities.findIndex(e => e.id === id);
        if (index >= 0) {
            this.entities.splice(index, 1);
            return true;
        }
        return false;
    }
    
    findBy<K extends keyof T>(key: K, value: T[K]): T[] {
        return this.entities.filter(e => e[key] === value);
    }
}

// Complex business logic
class OrderService {
    private userService: UserService;
    private productService: ProductService;
    private orderRepository: Repository<Order>;
    
    constructor(
        userService: UserService,
        productService: ProductService,
        orderRepository: Repository<Order>
    ) {
        this.userService = userService;
        this.productService = productService;
        this.orderRepository = orderRepository;
    }
    
    createOrder(userId: number, productIds: number[]): Order | null {
        let user = this.userService.findById(userId);
        if (!user) {
            throw new Error("User not found");
        }
        
        if (!this.userService.hasPermission(user, "write")) {
            throw new Error("User does not have write permission");
        }
        
        let products: Product[] = [];
        let total: number = 0;
        
        for (let productId of productIds) {
            let product = this.productService.findById(productId);
            if (!product) {
                throw new Error("Product not found: " + productId);
            }
            if (!product.inStock) {
                throw new Error("Product out of stock: " + product.name);
            }
            products.push(product);
            total += product.price;
        }
        
        let order: Order = {
            id: 0,
            userId: userId,
            products: products,
            total: total,
            status: "pending",
            createdAt: new Date()
        };
        
        return this.orderRepository.save(order);
    }
    
    processOrder(orderId: number): boolean {
        let order = this.orderRepository.findById(orderId);
        if (!order) {
            return false;
        }
        
        if (order.status !== "pending") {
            return false;
        }
        
        order.status = "processing";
        this.orderRepository.save(order);
        
        // Simulate processing
        setTimeout(() => {
            order.status = "shipped";
            this.orderRepository.save(order);
        }, 1000);
        
        return true;
    }
    
    getOrdersByUser(userId: number): Order[] {
        return this.orderRepository.findBy("userId", userId);
    }
    
    getOrdersByStatus(status: Order["status"]): Order[] {
        return this.orderRepository.findBy("status", status);
    }
}

// Complex data processing
class DataProcessor<T> {
    private transformers: Array<(data: T) => T> = [];
    private validators: Array<(data: T) => boolean> = [];
    
    addTransformer(transformer: (data: T) => T): DataProcessor<T> {
        this.transformers.push(transformer);
        return this;
    }
    
    addValidator(validator: (data: T) => boolean): DataProcessor<T> {
        this.validators.push(validator);
        return this;
    }
    
    process(data: T): T | null {
        // Apply validators
        for (let validator of this.validators) {
            if (!validator(data)) {
                return null;
            }
        }
        
        // Apply transformers
        let result = data;
        for (let transformer of this.transformers) {
            result = transformer(result);
        }
        
        return result;
    }
    
    processBatch(data: T[]): T[] {
        let results: T[] = [];
        for (let item of data) {
            let processed = this.process(item);
            if (processed !== null) {
                results.push(processed);
            }
        }
        return results;
    }
}

// Complex event system
type EventHandler<T> = (event: T) => void;
type EventMap = {
    "user.created": User;
    "user.updated": User;
    "user.deleted": number;
    "order.created": Order;
    "order.processed": Order;
    "product.updated": Product;
};

class EventEmitter {
    private listeners: Map<string, EventHandler<any>[]> = new Map();
    
    on<K extends keyof EventMap>(event: K, handler: EventHandler<EventMap[K]>): void {
        if (!this.listeners.has(event)) {
            this.listeners.set(event, []);
        }
        this.listeners.get(event)!.push(handler);
    }
    
    emit<K extends keyof EventMap>(event: K, data: EventMap[K]): void {
        let handlers = this.listeners.get(event);
        if (handlers) {
            for (let handler of handlers) {
                try {
                    handler(data);
                } catch (error) {
                    _print("Error in event handler:", error);
                }
            }
        }
    }
    
    off<K extends keyof EventMap>(event: K, handler: EventHandler<EventMap[K]>): void {
        let handlers = this.listeners.get(event);
        if (handlers) {
            let index = handlers.indexOf(handler);
            if (index >= 0) {
                handlers.splice(index, 1);
            }
        }
    }
}

// Complex configuration system
interface Config {
    database: {
        host: string;
        port: number;
        name: string;
        username: string;
        password: string;
    };
    api: {
        baseUrl: string;
        timeout: number;
        retries: number;
    };
    features: {
        enableLogging: boolean;
        enableCaching: boolean;
        enableMetrics: boolean;
    };
}

class ConfigManager {
    private config: Config;
    
    constructor(config: Config) {
        this.config = config;
    }
    
    get<K extends keyof Config>(key: K): Config[K] {
        return this.config[key];
    }
    
    getNested<K extends keyof Config, N extends keyof Config[K]>(
        key: K, 
        nestedKey: N
    ): Config[K][N] {
        return this.config[key][nestedKey];
    }
    
    update<K extends keyof Config>(key: K, value: Config[K]): void {
        this.config[key] = value;
    }
    
    validate(): boolean {
        try {
            // Validate database config
            if (!this.config.database.host || this.config.database.port <= 0) {
                return false;
            }
            
            // Validate API config
            if (!this.config.api.baseUrl || this.config.api.timeout <= 0) {
                return false;
            }
            
            return true;
        } catch {
            return false;
        }
    }
}

// Usage examples
function createApplication(): void {
    // Create repositories
    let userRepository = new InMemoryRepository<User>();
    let productRepository = new InMemoryRepository<Product>();
    let orderRepository = new InMemoryRepository<Order>();
    
    // Create services
    let userService = new UserService(userRepository);
    let productService = new ProductService(productRepository);
    let orderService = new OrderService(userService, productService, orderRepository);
    
    // Create event emitter
    let eventEmitter = new EventEmitter();
    
    // Set up event handlers
    eventEmitter.on("user.created", (user: User) => {
        _print("User created:", user.name);
    });
    
    eventEmitter.on("order.created", (order: Order) => {
        _print("Order created:", order.id, "Total:", order.total);
    });
    
    // Create configuration
    let config: Config = {
        database: {
            host: "localhost",
            port: 5432,
            name: "myapp",
            username: "admin",
            password: "secret"
        },
        api: {
            baseUrl: "https://api.example.com",
            timeout: 5000,
            retries: 3
        },
        features: {
            enableLogging: true,
            enableCaching: false,
            enableMetrics: true
        }
    };
    
    let configManager = new ConfigManager(config);
    
    // Create data processor
    let userProcessor = new DataProcessor<User>()
        .addValidator((user: User) => user.name.length > 0)
        .addTransformer((user: User) => ({ ...user, name: user.name.toUpperCase() }));
    
    // Create sample data
    let user: User = {
        id: 0,
        name: "Alice",
        email: "alice@example.com",
        role: "admin",
        permissions: ["read", "write", "delete"],
        createdAt: new Date()
    };
    
    let product: Product = {
        id: 0,
        name: "Laptop",
        price: 999.99,
        category: "Electronics",
        inStock: true,
        tags: ["computer", "portable"]
    };
    
    // Process data
    let savedUser = userService.process(user);
    let savedProduct = productService.process(product);
    
    // Emit events
    eventEmitter.emit("user.created", savedUser);
    
    // Create order
    try {
        let order = orderService.createOrder(savedUser.id, [savedProduct.id]);
        if (order) {
            eventEmitter.emit("order.created", order);
            orderService.processOrder(order.id);
        }
    } catch (error) {
        _print("Order creation failed:", error.message);
    }
    
    // Process data with processor
    let processedUser = userProcessor.process(savedUser);
    if (processedUser) {
        _print("Processed user:", processedUser.name);
    }
    
    // Use configuration
    if (configManager.validate()) {
        _print("Configuration is valid");
        _print("Database host:", configManager.getNested("database", "host"));
        _print("API timeout:", configManager.getNested("api", "timeout"));
    }
}

// This example demonstrates comprehensive advanced functionality