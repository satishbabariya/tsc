// Comprehensive Monomorphization Test Suite
// Tests various aspects of generic type monomorphization

// =============================================================================
// 1. Basic Generic Functions
// =============================================================================

function identity<T>(value: T): T {
    return value;
}

function swap<T, U>(a: T, b: U): [U, T] {
    return [b, a];
}

function first<T>(items: T[]): T | undefined {
    return items.length > 0 ? items[0] : undefined;
}

// Test basic monomorphization
let numIdentity = identity<number>(42);
let strIdentity = identity<string>("hello");
let boolIdentity = identity<boolean>(true);

let swapped = swap<number, string>(123, "test");
let firstItem = first<number>([1, 2, 3]);

// =============================================================================
// 2. Generic Classes
// =============================================================================

class Container<T> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    get(index: number): T | undefined {
        return this.items[index];
    }
    
    size(): number {
        return this.items.length;
    }
    
    map<U>(fn: (item: T) => U): Container<U> {
        const result = new Container<U>();
        for (const item of this.items) {
            result.add(fn(item));
        }
        return result;
    }
}

// Test class monomorphization
let numContainer = new Container<number>();
numContainer.add(1);
numContainer.add(2);
numContainer.add(3);

let strContainer = new Container<string>();
strContainer.add("a");
strContainer.add("b");

let mappedContainer = numContainer.map<number>(x => x * 2);

// =============================================================================
// 3. Generic Interfaces
// =============================================================================

interface Repository<T> {
    findById(id: number): T | null;
    save(entity: T): void;
    delete(id: number): boolean;
}

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

class UserRepository implements Repository<User> {
    private users: User[] = [];
    
    findById(id: number): User | null {
        return this.users.find(u => u.id === id) || null;
    }
    
    save(user: User): void {
        const existingIndex = this.users.findIndex(u => u.id === user.id);
        if (existingIndex >= 0) {
            this.users[existingIndex] = user;
        } else {
            this.users.push(user);
        }
    }
    
    delete(id: number): boolean {
        const index = this.users.findIndex(u => u.id === id);
        if (index >= 0) {
            this.users.splice(index, 1);
            return true;
        }
        return false;
    }
}

class ProductRepository implements Repository<Product> {
    private products: Product[] = [];
    
    findById(id: number): Product | null {
        return this.products.find(p => p.id === id) || null;
    }
    
    save(product: Product): void {
        const existingIndex = this.products.findIndex(p => p.id === product.id);
        if (existingIndex >= 0) {
            this.products[existingIndex] = product;
        } else {
            this.products.push(product);
        }
    }
    
    delete(id: number): boolean {
        const index = this.products.findIndex(p => p.id === id);
        if (index >= 0) {
            this.products.splice(index, 1);
            return true;
        }
        return false;
    }
}

// Test interface monomorphization
let userRepo = new UserRepository();
let productRepo = new ProductRepository();

userRepo.save({ id: 1, name: "John", email: "john@example.com" });
productRepo.save({ id: 1, name: "Widget", price: 29.99 });

// =============================================================================
// 4. Nested Generics
// =============================================================================

class NestedContainer<T> {
    private inner: Container<T> = new Container<T>();
    
    add(item: T): void {
        this.inner.add(item);
    }
    
    getContainer(): Container<T> {
        return this.inner;
    }
    
    transform<U>(fn: (container: Container<T>) => Container<U>): NestedContainer<U> {
        const result = new NestedContainer<U>();
        const transformed = fn(this.inner);
        // Copy items from transformed container
        for (let i = 0; i < transformed.size(); i++) {
            const item = transformed.get(i);
            if (item !== undefined) {
                result.add(item);
            }
        }
        return result;
    }
}

// Test nested generic monomorphization
let nestedNum = new NestedContainer<number>();
nestedNum.add(10);
nestedNum.add(20);

let nestedStr = new NestedContainer<string>();
nestedStr.add("hello");
nestedStr.add("world");

let transformed = nestedNum.transform<number>(container => 
    container.map<number>(x => x * 3)
);

// =============================================================================
// 5. Generic Constraints
// =============================================================================

interface Comparable<T> {
    compareTo(other: T): number;
}

class SortedContainer<T extends Comparable<T>> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
        this.items.sort((a, b) => a.compareTo(b));
    }
    
    find(item: T): T | null {
        for (const existing of this.items) {
            if (existing.compareTo(item) === 0) {
                return existing;
            }
        }
        return null;
    }
}

class NumberWrapper implements Comparable<NumberWrapper> {
    constructor(public value: number) {}
    
    compareTo(other: NumberWrapper): number {
        return this.value - other.value;
    }
}

// Test constraint monomorphization
let sortedContainer = new SortedContainer<NumberWrapper>();
sortedContainer.add(new NumberWrapper(5));
sortedContainer.add(new NumberWrapper(1));
sortedContainer.add(new NumberWrapper(3));

// =============================================================================
// 6. Generic Methods in Classes
// =============================================================================

class UtilityClass {
    static createArray<T>(size: number, defaultValue: T): T[] {
        const result: T[] = [];
        for (let i = 0; i < size; i++) {
            result.push(defaultValue);
        }
        return result;
    }
    
    static zip<T, U>(arr1: T[], arr2: U[]): [T, U][] {
        const result: [T, U][] = [];
        const minLength = Math.min(arr1.length, arr2.length);
        for (let i = 0; i < minLength; i++) {
            result.push([arr1[i], arr2[i]]);
        }
        return result;
    }
    
    static groupBy<T, K>(items: T[], keyFn: (item: T) => K): Map<K, T[]> {
        const result = new Map<K, T[]>();
        for (const item of items) {
            const key = keyFn(item);
            if (!result.has(key)) {
                result.set(key, []);
            }
            result.get(key)!.push(item);
        }
        return result;
    }
}

// Test generic method monomorphization
let numArray = UtilityClass.createArray<number>(5, 0);
let strArray = UtilityClass.createArray<string>(3, "default");

let zipped = UtilityClass.zip<number, string>([1, 2, 3], ["a", "b", "c"]);

let numbers = [1, 2, 3, 4, 5];
let grouped = UtilityClass.groupBy<number, string>(numbers, x => x % 2 === 0 ? "even" : "odd");

// =============================================================================
// 7. Complex Generic Scenarios
// =============================================================================

interface Transformer<T, U> {
    transform(input: T): U;
}

class Pipeline<T> {
    private transformers: Transformer<any, any>[] = [];
    
    add<U>(transformer: Transformer<T, U>): Pipeline<U> {
        this.transformers.push(transformer);
        return this as any;
    }
    
    execute(input: T): any {
        let result: any = input;
        for (const transformer of this.transformers) {
            result = transformer.transform(result);
        }
        return result;
    }
}

class NumberToStringTransformer implements Transformer<number, string> {
    transform(input: number): string {
        return input.toString();
    }
}

class StringLengthTransformer implements Transformer<string, number> {
    transform(input: string): number {
        return input.length;
    }
}

// Test complex pipeline monomorphization
let pipeline = new Pipeline<number>();
pipeline.add(new NumberToStringTransformer());
pipeline.add(new StringLengthTransformer());

let result = pipeline.execute(12345);

// =============================================================================
// 8. Generic Type Aliases
// =============================================================================

type Optional<T> = T | null | undefined;
type Result<T, E> = { success: true; data: T } | { success: false; error: E };
type AsyncResult<T> = Promise<Result<T, string>>;

function processOptional<T>(value: Optional<T>): T | null {
    return value ?? null;
}

function createResult<T, E>(data: T): Result<T, E> {
    return { success: true, data };
}

function createError<T, E>(error: E): Result<T, E> {
    return { success: false, error };
}

// Test type alias monomorphization
let optionalNum: Optional<number> = 42;
let optionalStr: Optional<string> = null;

let successResult = createResult<number, string>(100);
let errorResult = createError<number, string>("Something went wrong");

// =============================================================================
// 9. Generic Recursive Types
// =============================================================================

interface TreeNode<T> {
    value: T;
    left?: TreeNode<T>;
    right?: TreeNode<T>;
}

class Tree<T> {
    private root?: TreeNode<T>;
    
    insert(value: T): void {
        this.root = this.insertNode(this.root, value);
    }
    
    private insertNode(node: TreeNode<T> | undefined, value: T): TreeNode<T> {
        if (!node) {
            return { value };
        }
        
        if (value < node.value) {
            node.left = this.insertNode(node.left, value);
        } else if (value > node.value) {
            node.right = this.insertNode(node.right, value);
        }
        
        return node;
    }
    
    search(value: T): boolean {
        return this.searchNode(this.root, value);
    }
    
    private searchNode(node: TreeNode<T> | undefined, value: T): boolean {
        if (!node) return false;
        if (value === node.value) return true;
        if (value < node.value) return this.searchNode(node.left, value);
        return this.searchNode(node.right, value);
    }
}

// Test recursive generic monomorphization
let numberTree = new Tree<number>();
numberTree.insert(5);
numberTree.insert(3);
numberTree.insert(7);

let stringTree = new Tree<string>();
stringTree.insert("apple");
stringTree.insert("banana");
stringTree.insert("cherry");

// =============================================================================
// 10. Generic Union and Intersection Types
// =============================================================================

type StringOrNumber<T extends string | number> = T extends string ? string : number;
type ArrayOrObject<T> = T extends any[] ? T : T extends object ? T : never;

function processStringOrNumber<T extends string | number>(value: T): StringOrNumber<T> {
    return value as StringOrNumber<T>;
}

function processArrayOrObject<T>(value: T): ArrayOrObject<T> {
    return value as ArrayOrObject<T>;
}

// Test union/intersection generic monomorphization
let strResult = processStringOrNumber<string>("hello");
let numResult = processStringOrNumber<number>(42);

let arrayResult = processArrayOrObject<number[]>([1, 2, 3]);
let objectResult = processArrayOrObject<{ name: string }>({ name: "test" });

// =============================================================================
// Main execution
// =============================================================================

function main(): void {
    console.log("Monomorphization comprehensive test suite");
    console.log("All tests completed successfully!");
}

main();