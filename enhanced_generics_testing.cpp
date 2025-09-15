#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

// Enhanced Generics Testing Suite
class EnhancedGenericsTestingSuite {
public:
    void runComprehensiveTesting() {
        std::cout << "=== Enhanced Generics Testing Suite ===" << std::endl;
        
        // Test 1: Generic Method Constraints
        testGenericMethodConstraints();
        
        // Test 2: Generic Method Overloading
        testGenericMethodOverloading();
        
        // Test 3: Generic Type Inference
        testGenericTypeInference();
        
        // Test 4: Generic Array Types
        testGenericArrayTypes();
        
        // Test 5: Integration Tests
        testIntegrationScenarios();
        
        std::cout << "=== Enhanced Generics Testing Complete ===" << std::endl;
    }
    
private:
    void testGenericMethodConstraints() {
        std::cout << "\n--- Testing Generic Method Constraints ---" << std::endl;
        
        // Test 1: T extends SomeInterface constraints
        std::string constraintTest1 = R"(
// Generic method with interface constraint
interface Comparable {
    compareTo(other: Comparable): number;
}

interface Serializable {
    serialize(): string;
}

class Person implements Comparable, Serializable {
    name: string;
    age: number;
    
    constructor(name: string, age: number) {
        this.name = name;
        this.age = age;
    }
    
    compareTo(other: Person): number {
        return this.age - other.age;
    }
    
    serialize(): string {
        return JSON.stringify({ name: this.name, age: this.age });
    }
}

class Product implements Comparable {
    id: number;
    price: number;
    
    constructor(id: number, price: number) {
        this.id = id;
        this.price = price;
    }
    
    compareTo(other: Product): number {
        return this.price - other.price;
    }
}

// Generic method with T extends Comparable constraint
function sortItems<T extends Comparable>(items: T[]): T[] {
    return items.sort((a, b) => a.compareTo(b));
}

// Generic method with multiple constraints
function processItem<T extends Comparable & Serializable>(item: T): string {
    const comparison = item.compareTo(item);
    const serialized = item.serialize();
    return `Comparison: ${comparison}, Serialized: ${serialized}`;
}

// Test the constraints
const people: Person[] = [
    new Person("Alice", 30),
    new Person("Bob", 25),
    new Person("Charlie", 35)
];

const products: Product[] = [
    new Product(1, 100),
    new Product(2, 50),
    new Product(3, 200)
];

const sortedPeople = sortItems(people);
const sortedProducts = sortItems(products);

const processedPerson = processItem(people[0]);
console.log("Sorted people:", sortedPeople);
console.log("Sorted products:", sortedProducts);
console.log("Processed person:", processedPerson);
)";
        
        saveTestFile("generic_constraints_1.ts", constraintTest1);
        
        // Test 2: Constructor constraints
        std::string constraintTest2 = R"(
// Generic method with constructor constraint
class DatabaseConnection {
    connected: boolean;
    
    constructor() {
        this.connected = false;
    }
    
    connect(): void {
        this.connected = true;
    }
    
    disconnect(): void {
        this.connected = false;
    }
}

class CacheManager {
    cache: Map<string, any>;
    
    constructor() {
        this.cache = new Map();
    }
    
    set(key: string, value: any): void {
        this.cache.set(key, value);
    }
    
    get(key: string): any {
        return this.cache.get(key);
    }
}

// Generic factory method with constructor constraint
function createInstance<T extends new () => any>(constructor: T): InstanceType<T> {
    return new constructor();
}

// Generic method with constructor constraint
function initializeService<T extends new () => { connect(): void }>(ServiceClass: T): InstanceType<T> {
    const instance = new ServiceClass();
    instance.connect();
    return instance;
}

// Test the constructor constraints
const dbConnection = createInstance(DatabaseConnection);
const cacheManager = createInstance(CacheManager);

const initializedDB = initializeService(DatabaseConnection);
const initializedCache = initializeService(CacheManager);

console.log("Database connection:", dbConnection);
console.log("Cache manager:", cacheManager);
console.log("Initialized DB:", initializedDB);
console.log("Initialized cache:", initializedCache);
)";
        
        saveTestFile("generic_constraints_2.ts", constraintTest2);
        
        std::cout << "Generated generic constraints test files" << std::endl;
    }
    
    void testGenericMethodOverloading() {
        std::cout << "\n--- Testing Generic Method Overloading ---" << std::endl;
        
        // Test 1: Multiple generic method signatures
        std::string overloadingTest1 = R"(
// Generic method overloading
class DataProcessor<T> {
    data: T;
    
    constructor(data: T) {
        this.data = data;
    }
    
    // Overload 1: Process with single function
    process<U>(processor: (input: T) => U): DataProcessor<U>;
    
    // Overload 2: Process with array of functions
    process<U>(processors: ((input: T) => U)[]): DataProcessor<U[]>;
    
    // Overload 3: Process with conditional function
    process<U>(condition: (input: T) => boolean, processor: (input: T) => U): DataProcessor<U | null>;
    
    // Implementation
    process<U>(processorOrProcessors: ((input: T) => U) | ((input: T) => U)[] | ((input: T) => boolean), 
               processor?: (input: T) => U): DataProcessor<U | U[] | U | null> {
        if (Array.isArray(processorOrProcessors)) {
            // Array of processors
            const results = processorOrProcessors.map(p => p(this.data));
            return new DataProcessor<U[]>(results as U[]);
        } else if (processor) {
            // Conditional processor
            if (processorOrProcessors(this.data)) {
                return new DataProcessor<U>(processor(this.data));
            } else {
                return new DataProcessor<U | null>(null);
            }
        } else {
            // Single processor
            return new DataProcessor<U>(processorOrProcessors(this.data));
        }
    }
}

// Generic function overloading
function transform<T, U>(input: T, transformer: (input: T) => U): U;
function transform<T, U>(input: T[], transformer: (input: T) => U): U[];
function transform<T, U>(input: T | T[], transformer: (input: T) => U): U | U[] {
    if (Array.isArray(input)) {
        return input.map(transformer);
    } else {
        return transformer(input);
    }
}

// Test the overloading
const processor = new DataProcessor<number>(42);

// Test single processor
const result1 = processor.process(x => x * 2);
console.log("Single processor result:", result1.data);

// Test array of processors
const result2 = processor.process([x => x * 2, x => x + 10, x => x - 5]);
console.log("Array processor result:", result2.data);

// Test conditional processor
const result3 = processor.process(x => x > 40, x => x * 3);
console.log("Conditional processor result:", result3.data);

// Test function overloading
const singleResult = transform(42, x => x * 2);
const arrayResult = transform([1, 2, 3], x => x * 2);

console.log("Single transform result:", singleResult);
console.log("Array transform result:", arrayResult);
)";
        
        saveTestFile("generic_overloading_1.ts", overloadingTest1);
        
        // Test 2: Complex generic overloading scenarios
        std::string overloadingTest2 = R"(
// Complex generic overloading scenarios
interface ApiResponse<T> {
    data: T;
    status: number;
    message: string;
}

interface ErrorResponse {
    error: string;
    status: number;
}

class ApiClient {
    // Overload 1: GET request
    request<T>(url: string): Promise<ApiResponse<T>>;
    
    // Overload 2: POST request with data
    request<T, U>(url: string, method: "POST", data: T): Promise<ApiResponse<U>>;
    
    // Overload 3: PUT request with data
    request<T, U>(url: string, method: "PUT", data: T): Promise<ApiResponse<U>>;
    
    // Overload 4: DELETE request
    request<T>(url: string, method: "DELETE"): Promise<ApiResponse<T>>;
    
    // Implementation
    async request<T, U = T>(url: string, method?: string, data?: T): Promise<ApiResponse<U>> {
        // Simulated API call
        return {
            data: {} as U,
            status: 200,
            message: "Success"
        };
    }
}

// Generic utility functions with overloading
function parse<T>(input: string): T;
function parse<T>(input: string, parser: (input: string) => T): T;
function parse<T>(input: string, parser?: (input: string) => T): T {
    if (parser) {
        return parser(input);
    } else {
        // Default JSON parsing
        return JSON.parse(input) as T;
    }
}

// Test the complex overloading
const apiClient = new ApiClient();

// Test different request types
const getResponse = await apiClient.request<User>("/api/users/1");
const postResponse = await apiClient.request<CreateUserRequest, User>("/api/users", "POST", { name: "John", email: "john@example.com" });
const putResponse = await apiClient.request<UpdateUserRequest, User>("/api/users/1", "PUT", { name: "Jane" });
const deleteResponse = await apiClient.request<void>("/api/users/1", "DELETE");

console.log("GET response:", getResponse);
console.log("POST response:", postResponse);
console.log("PUT response:", putResponse);
console.log("DELETE response:", deleteResponse);

// Test parsing overloading
const parsedJson = parse<User>('{"id": 1, "name": "John"}');
const parsedCustom = parse<number>("42", input => parseInt(input));

console.log("Parsed JSON:", parsedJson);
console.log("Parsed custom:", parsedCustom);
)";
        
        saveTestFile("generic_overloading_2.ts", overloadingTest2);
        
        std::cout << "Generated generic overloading test files" << std::endl;
    }
    
    void testGenericTypeInference() {
        std::cout << "\n--- Testing Generic Type Inference ---" << std::endl;
        
        // Test 1: Automatic type inference for generic method calls
        std::string inferenceTest1 = R"(
// Generic type inference tests
class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    map<U>(fn: (value: T) => U): Container<U> {
        return new Container(fn(this.value));
    }
    
    flatMap<U>(fn: (value: T) => Container<U>): Container<U> {
        return fn(this.value);
    }
    
    filter(predicate: (value: T) => boolean): Container<T> {
        return predicate(this.value) ? this : new Container(null as any);
    }
}

// Generic utility functions
function identity<T>(value: T): T {
    return value;
}

function toArray<T>(value: T): T[] {
    return [value];
}

function toString<T>(value: T): string {
    return String(value);
}

function toNumber<T>(value: T): number {
    return Number(value);
}

// Test type inference
const stringContainer = new Container("hello");
const numberContainer = new Container(42);
const booleanContainer = new Container(true);

// Test map with type inference
const mappedString = stringContainer.map(s => s.length); // Should infer number
const mappedNumber = numberContainer.map(n => n * 2); // Should infer number
const mappedBoolean = booleanContainer.map(b => b ? "true" : "false"); // Should infer string

// Test flatMap with type inference
const flatMappedString = stringContainer.flatMap(s => new Container(s.length)); // Should infer number
const flatMappedNumber = numberContainer.flatMap(n => new Container(n.toString())); // Should infer string

// Test filter with type inference
const filteredString = stringContainer.filter(s => s.length > 3); // Should infer string
const filteredNumber = numberContainer.filter(n => n > 40); // Should infer number

// Test utility functions with type inference
const identityResult = identity("test"); // Should infer string
const arrayResult = toArray(42); // Should infer number[]
const stringResult = toString(true); // Should infer string
const numberResult = toNumber("123"); // Should infer number

console.log("Mapped string:", mappedString.value);
console.log("Mapped number:", mappedNumber.value);
console.log("Mapped boolean:", mappedBoolean.value);
console.log("Flat mapped string:", flatMappedString.value);
console.log("Flat mapped number:", flatMappedNumber.value);
console.log("Filtered string:", filteredString.value);
console.log("Filtered number:", filteredNumber.value);
console.log("Identity result:", identityResult);
console.log("Array result:", arrayResult);
console.log("String result:", stringResult);
console.log("Number result:", numberResult);
)";
        
        saveTestFile("generic_inference_1.ts", inferenceTest1);
        
        // Test 2: Complex type inference scenarios
        std::string inferenceTest2 = R"(
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
)";
        
        saveTestFile("generic_inference_2.ts", inferenceTest2);
        
        std::cout << "Generated generic type inference test files" << std::endl;
    }
    
    void testGenericArrayTypes() {
        std::cout << "\n--- Testing Generic Array Types ---" << std::endl;
        
        // Test 1: Array vs T[] support and conversion
        std::string arrayTest1 = R"(
// Generic array types tests
interface Comparable<T> {
    compareTo(other: T): number;
}

class NumberArray<T extends number> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    remove(item: T): boolean {
        const index = this.items.indexOf(item);
        if (index !== -1) {
            this.items.splice(index, 1);
            return true;
        }
        return false;
    }
    
    get(index: number): T | undefined {
        return this.items[index];
    }
    
    length(): number {
        return this.items.length;
    }
    
    toArray(): T[] {
        return [...this.items];
    }
    
    fromArray(array: T[]): void {
        this.items = [...array];
    }
    
    map<U>(fn: (item: T) => U): U[] {
        return this.items.map(fn);
    }
    
    filter(predicate: (item: T) => boolean): T[] {
        return this.items.filter(predicate);
    }
    
    reduce<U>(fn: (acc: U, item: T) => U, initial: U): U {
        return this.items.reduce(fn, initial);
    }
}

// Generic array utilities
function createArray<T>(...items: T[]): T[] {
    return items;
}

function arrayToSet<T>(array: T[]): Set<T> {
    return new Set(array);
}

function setToArray<T>(set: Set<T>): T[] {
    return Array.from(set);
}

function arrayToMap<T, K, V>(array: T[], keyFn: (item: T) => K, valueFn: (item: T) => V): Map<K, V> {
    const map = new Map<K, V>();
    for (const item of array) {
        map.set(keyFn(item), valueFn(item));
    }
    return map;
}

function mapToArray<K, V>(map: Map<K, V>): [K, V][] {
    return Array.from(map.entries());
}

// Test generic array types
const numberArray = new NumberArray<number>();

numberArray.add(1);
numberArray.add(2);
numberArray.add(3);
numberArray.add(4);
numberArray.add(5);

console.log("Array length:", numberArray.length());
console.log("Array items:", numberArray.toArray());

// Test array operations
const doubled = numberArray.map(x => x * 2);
const evens = numberArray.filter(x => x % 2 === 0);
const sum = numberArray.reduce((acc, x) => acc + x, 0);

console.log("Doubled:", doubled);
console.log("Evens:", evens);
console.log("Sum:", sum);

// Test array conversions
const array = createArray(1, 2, 3, 4, 5);
const set = arrayToSet(array);
const backToArray = setToArray(set);

console.log("Original array:", array);
console.log("Set:", set);
console.log("Back to array:", backToArray);

// Test array to map conversion
const users = [
    { id: 1, name: "Alice" },
    { id: 2, name: "Bob" },
    { id: 3, name: "Charlie" }
];

const userMap = arrayToMap(users, user => user.id, user => user.name);
const mapArray = mapToArray(userMap);

console.log("User map:", userMap);
console.log("Map to array:", mapArray);
)";
        
        saveTestFile("generic_arrays_1.ts", arrayTest1);
        
        // Test 2: Complex array operations with generics
        std::string arrayTest2 = R"(
// Complex generic array operations
interface Entity {
    id: number;
    createdAt: Date;
    updatedAt: Date;
}

interface User extends Entity {
    name: string;
    email: string;
    age: number;
}

interface Product extends Entity {
    title: string;
    description: string;
    price: number;
    category: string;
}

// Generic array operations
class GenericArray<T> {
    private items: T[] = [];
    
    constructor(items: T[] = []) {
        this.items = [...items];
    }
    
    // Basic operations
    add(item: T): void {
        this.items.push(item);
    }
    
    addAll(items: T[]): void {
        this.items.push(...items);
    }
    
    remove(item: T): boolean {
        const index = this.items.indexOf(item);
        if (index !== -1) {
            this.items.splice(index, 1);
            return true;
        }
        return false;
    }
    
    removeAt(index: number): T | undefined {
        if (index >= 0 && index < this.items.length) {
            return this.items.splice(index, 1)[0];
        }
        return undefined;
    }
    
    get(index: number): T | undefined {
        return this.items[index];
    }
    
    set(index: number, item: T): boolean {
        if (index >= 0 && index < this.items.length) {
            this.items[index] = item;
            return true;
        }
        return false;
    }
    
    length(): number {
        return this.items.length;
    }
    
    isEmpty(): boolean {
        return this.items.length === 0;
    }
    
    clear(): void {
        this.items = [];
    }
    
    // Functional operations
    map<U>(fn: (item: T, index: number) => U): U[] {
        return this.items.map(fn);
    }
    
    filter(predicate: (item: T, index: number) => boolean): T[] {
        return this.items.filter(predicate);
    }
    
    reduce<U>(fn: (acc: U, item: T, index: number) => U, initial: U): U {
        return this.items.reduce(fn, initial);
    }
    
    forEach(fn: (item: T, index: number) => void): void {
        this.items.forEach(fn);
    }
    
    find(predicate: (item: T, index: number) => boolean): T | undefined {
        return this.items.find(predicate);
    }
    
    findIndex(predicate: (item: T, index: number) => boolean): number {
        return this.items.findIndex(predicate);
    }
    
    some(predicate: (item: T, index: number) => boolean): boolean {
        return this.items.some(predicate);
    }
    
    every(predicate: (item: T, index: number) => boolean): boolean {
        return this.items.every(predicate);
    }
    
    // Array operations
    slice(start?: number, end?: number): T[] {
        return this.items.slice(start, end);
    }
    
    splice(start: number, deleteCount?: number, ...items: T[]): T[] {
        return this.items.splice(start, deleteCount, ...items);
    }
    
    concat(other: GenericArray<T>): GenericArray<T> {
        return new GenericArray([...this.items, ...other.items]);
    }
    
    // Sorting
    sort(compareFn?: (a: T, b: T) => number): GenericArray<T> {
        const sorted = [...this.items].sort(compareFn);
        return new GenericArray(sorted);
    }
    
    reverse(): GenericArray<T> {
        const reversed = [...this.items].reverse();
        return new GenericArray(reversed);
    }
    
    // Conversion
    toArray(): T[] {
        return [...this.items];
    }
    
    toSet(): Set<T> {
        return new Set(this.items);
    }
    
    toString(): string {
        return this.items.toString();
    }
}

// Test complex generic array operations
const users = new GenericArray<User>([
    { id: 1, name: "Alice", email: "alice@example.com", age: 25, createdAt: new Date(), updatedAt: new Date() },
    { id: 2, name: "Bob", email: "bob@example.com", age: 30, createdAt: new Date(), updatedAt: new Date() },
    { id: 3, name: "Charlie", email: "charlie@example.com", age: 35, createdAt: new Date(), updatedAt: new Date() }
]);

const products = new GenericArray<Product>([
    { id: 1, title: "Laptop", description: "High-performance laptop", price: 999.99, category: "Electronics", createdAt: new Date(), updatedAt: new Date() },
    { id: 2, title: "Mouse", description: "Wireless mouse", price: 29.99, category: "Electronics", createdAt: new Date(), updatedAt: new Date() },
    { id: 3, title: "Keyboard", description: "Mechanical keyboard", price: 79.99, category: "Electronics", createdAt: new Date(), updatedAt: new Date() }
]);

// Test user operations
const userNames = users.map(user => user.name);
const youngUsers = users.filter(user => user.age < 30);
const totalAge = users.reduce((sum, user) => sum + user.age, 0);
const hasAlice = users.some(user => user.name === "Alice");
const allAdults = users.every(user => user.age >= 18);

console.log("User names:", userNames);
console.log("Young users:", youngUsers);
console.log("Total age:", totalAge);
console.log("Has Alice:", hasAlice);
console.log("All adults:", allAdults);

// Test product operations
const productTitles = products.map(product => product.title);
const expensiveProducts = products.filter(product => product.price > 50);
const totalPrice = products.reduce((sum, product) => sum + product.price, 0);
const hasLaptop = products.some(product => product.title === "Laptop");
const allElectronics = products.every(product => product.category === "Electronics");

console.log("Product titles:", productTitles);
console.log("Expensive products:", expensiveProducts);
console.log("Total price:", totalPrice);
console.log("Has laptop:", hasLaptop);
console.log("All electronics:", allElectronics);

// Test array operations
const sortedUsers = users.sort((a, b) => a.age - b.age);
const reversedProducts = products.reverse();
const combined = users.concat(products as any);

console.log("Sorted users:", sortedUsers.toArray());
console.log("Reversed products:", reversedProducts.toArray());
console.log("Combined length:", combined.length());
)";
        
        saveTestFile("generic_arrays_2.ts", arrayTest2);
        
        std::cout << "Generated generic array types test files" << std::endl;
    }
    
    void testIntegrationScenarios() {
        std::cout << "\n--- Testing Integration Scenarios ---" << std::endl;
        
        // Test: Complete integration of all generic features
        std::string integrationTest = R"(
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
)";
        
        saveTestFile("generic_integration.ts", integrationTest);
        
        std::cout << "Generated integration test file" << std::endl;
    }
    
    void saveTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Generated: " << filename << std::endl;
        }
    }
};

int main() {
    try {
        EnhancedGenericsTestingSuite suite;
        suite.runComprehensiveTesting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}