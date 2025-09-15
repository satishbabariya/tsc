#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

// Generics Integration Testing Suite
class GenericsIntegrationTestingSuite {
public:
    void runComprehensiveIntegrationTesting() {
        std::cout << "=== Generics Integration Testing Suite ===" << std::endl;
        
        // Test 1: Type Checker Integration
        testTypeCheckerIntegration();
        
        // Test 2: Semantic Analyzer Integration
        testSemanticAnalyzerIntegration();
        
        // Test 3: AST System Integration
        testASTSystemIntegration();
        
        // Test 4: Code Generation Integration
        testCodeGenerationIntegration();
        
        // Test 5: Error Reporting Integration
        testErrorReportingIntegration();
        
        // Test 6: Performance Integration
        testPerformanceIntegration();
        
        std::cout << "=== Generics Integration Testing Complete ===" << std::endl;
    }
    
private:
    void testTypeCheckerIntegration() {
        std::cout << "\n--- Testing Type Checker Integration ---" << std::endl;
        
        // Test: Generic constraints with type checking
        std::string typeCheckerTest = R"(
// Type checker integration with generic constraints
interface Comparable<T> {
    compareTo(other: T): number;
}

interface Serializable {
    serialize(): string;
}

class NumberWrapper implements Comparable<NumberWrapper>, Serializable {
    value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    compareTo(other: NumberWrapper): number {
        return this.value - other.value;
    }
    
    serialize(): string {
        return this.value.toString();
    }
}

// Generic function with constraints
function processComparable<T extends Comparable<T>>(item: T): T {
    return item;
}

function processSerializable<T extends Serializable>(item: T): string {
    return item.serialize();
}

function processBoth<T extends Comparable<T> & Serializable>(item: T): string {
    const comparison = item.compareTo(item);
    const serialized = item.serialize();
    return `Comparison: ${comparison}, Serialized: ${serialized}`;
}

// Test type checking integration
const numberWrapper = new NumberWrapper(42);

// These should pass type checking
const processed1 = processComparable(numberWrapper);
const processed2 = processSerializable(numberWrapper);
const processed3 = processBoth(numberWrapper);

console.log("Processed comparable:", processed1);
console.log("Processed serializable:", processed2);
console.log("Processed both:", processed3);

// Test type checking with invalid types
class InvalidType {
    value: string;
    
    constructor(value: string) {
        this.value = value;
    }
}

const invalidType = new InvalidType("test");

// These should fail type checking
// const invalid1 = processComparable(invalidType); // Should fail
// const invalid2 = processSerializable(invalidType); // Should fail
// const invalid3 = processBoth(invalidType); // Should fail
)";
        
        saveTestFile("type_checker_integration.ts", typeCheckerTest);
        
        std::cout << "Generated type checker integration test" << std::endl;
    }
    
    void testSemanticAnalyzerIntegration() {
        std::cout << "\n--- Testing Semantic Analyzer Integration ---" << std::endl;
        
        // Test: Generic overloading with semantic analysis
        std::string semanticTest = R"(
// Semantic analyzer integration with generic overloading
class DataProcessor<T> {
    private data: T;
    
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
            const results = processorOrProcessors.map(p => p(this.data));
            return new DataProcessor<U[]>(results as U[]);
        } else if (processor) {
            if (processorOrProcessors(this.data)) {
                return new DataProcessor<U>(processor(this.data));
            } else {
                return new DataProcessor<U | null>(null);
            }
        } else {
            return new DataProcessor<U>(processorOrProcessors(this.data));
        }
    }
}

// Generic utility functions with overloading
function transform<T, U>(input: T, transformer: (input: T) => U): U;
function transform<T, U>(input: T[], transformer: (input: T) => U): U[];
function transform<T, U>(input: T | T[], transformer: (input: T) => U): U | U[] {
    if (Array.isArray(input)) {
        return input.map(transformer);
    } else {
        return transformer(input);
    }
}

// Test semantic analysis integration
const processor = new DataProcessor<number>(42);

// Test overload resolution
const result1 = processor.process(x => x * 2);
const result2 = processor.process([x => x * 2, x => x + 10, x => x - 5]);
const result3 = processor.process(x => x > 40, x => x * 3);

console.log("Single processor result:", result1);
console.log("Array processor result:", result2);
console.log("Conditional processor result:", result3);

// Test function overloading
const singleResult = transform(42, x => x * 2);
const arrayResult = transform([1, 2, 3], x => x * 2);

console.log("Single transform result:", singleResult);
console.log("Array transform result:", arrayResult);
)";
        
        saveTestFile("semantic_analyzer_integration.ts", semanticTest);
        
        std::cout << "Generated semantic analyzer integration test" << std::endl;
    }
    
    void testASTSystemIntegration() {
        std::cout << "\n--- Testing AST System Integration ---" << std::endl;
        
        // Test: Generic type inference with AST
        std::string astTest = R"(
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
)";
        
        saveTestFile("ast_system_integration.ts", astTest);
        
        std::cout << "Generated AST system integration test" << std::endl;
    }
    
    void testCodeGenerationIntegration() {
        std::cout << "\n--- Testing Code Generation Integration ---" << std::endl;
        
        // Test: Generic array types with code generation
        std::string codeGenTest = R"(
// Code generation integration with generic array types
class GenericArray<T> {
    private items: T[] = [];
    
    constructor(items: T[] = []) {
        this.items = [...items];
    }
    
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

// Test code generation integration
const numberArray = new GenericArray<number>([1, 2, 3, 4, 5]);
const stringArray = new GenericArray<string>(["a", "b", "c", "d", "e"]);

// Test array operations
numberArray.add(6);
stringArray.add("f");

const doubled = numberArray.map(x => x * 2);
const uppercased = stringArray.map(s => s.toUpperCase());

const evens = numberArray.filter(x => x % 2 === 0);
const longStrings = stringArray.filter(s => s.length > 1);

const sum = numberArray.reduce((acc, x) => acc + x, 0);
const concatenated = stringArray.reduce((acc, s) => acc + s, "");

const hasEven = numberArray.some(x => x % 2 === 0);
const allShort = stringArray.every(s => s.length <= 1);

console.log("Number array:", numberArray.toArray());
console.log("String array:", stringArray.toArray());
console.log("Doubled:", doubled);
console.log("Uppercased:", uppercased);
console.log("Evens:", evens);
console.log("Long strings:", longStrings);
console.log("Sum:", sum);
console.log("Concatenated:", concatenated);
console.log("Has even:", hasEven);
console.log("All short:", allShort);
)";
        
        saveTestFile("code_generation_integration.ts", codeGenTest);
        
        std::cout << "Generated code generation integration test" << std::endl;
    }
    
    void testErrorReportingIntegration() {
        std::cout << "\n--- Testing Error Reporting Integration ---" << std::endl;
        
        // Test: Error reporting with generic constraints
        std::string errorTest = R"(
// Error reporting integration with generic constraints
interface Comparable<T> {
    compareTo(other: T): number;
}

interface Serializable {
    serialize(): string;
}

class ValidType implements Comparable<ValidType>, Serializable {
    value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    compareTo(other: ValidType): number {
        return this.value - other.value;
    }
    
    serialize(): string {
        return this.value.toString();
    }
}

class InvalidType {
    value: string;
    
    constructor(value: string) {
        this.value = value;
    }
}

// Generic functions with constraints
function processComparable<T extends Comparable<T>>(item: T): T {
    return item;
}

function processSerializable<T extends Serializable>(item: T): string {
    return item.serialize();
}

function processBoth<T extends Comparable<T> & Serializable>(item: T): string {
    const comparison = item.compareTo(item);
    const serialized = item.serialize();
    return `Comparison: ${comparison}, Serialized: ${serialized}`;
}

// Test error reporting
const validType = new ValidType(42);
const invalidType = new InvalidType("test");

// These should work
const result1 = processComparable(validType);
const result2 = processSerializable(validType);
const result3 = processBoth(validType);

console.log("Valid results:", result1, result2, result3);

// These should generate errors
// const error1 = processComparable(invalidType); // Error: Type 'InvalidType' does not satisfy constraint 'Comparable<InvalidType>'
// const error2 = processSerializable(invalidType); // Error: Type 'InvalidType' does not satisfy constraint 'Serializable'
// const error3 = processBoth(invalidType); // Error: Type 'InvalidType' does not satisfy constraint 'Comparable<InvalidType> & Serializable'

// Test generic overloading errors
function overloadedFunction<T>(input: T): T;
function overloadedFunction<T>(input: T[]): T[];
function overloadedFunction<T>(input: T | T[]): T | T[] {
    if (Array.isArray(input)) {
        return input;
    } else {
        return input;
    }
}

// These should work
const singleResult = overloadedFunction(42);
const arrayResult = overloadedFunction([1, 2, 3]);

console.log("Overload results:", singleResult, arrayResult);

// Test type inference errors
function inferType<T>(input: T): T {
    return input;
}

// These should work with type inference
const inferred1 = inferType(42); // T inferred as number
const inferred2 = inferType("hello"); // T inferred as string
const inferred3 = inferType(true); // T inferred as boolean

console.log("Inferred types:", inferred1, inferred2, inferred3);
)";
        
        saveTestFile("error_reporting_integration.ts", errorTest);
        
        std::cout << "Generated error reporting integration test" << std::endl;
    }
    
    void testPerformanceIntegration() {
        std::cout << "\n--- Testing Performance Integration ---" << std::endl;
        
        // Test: Performance with complex generic scenarios
        std::string performanceTest = R"(
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
)";
        
        saveTestFile("performance_integration.ts", performanceTest);
        
        std::cout << "Generated performance integration test" << std::endl;
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
        GenericsIntegrationTestingSuite suite;
        suite.runComprehensiveIntegrationTesting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}