# ARC Examples

## Basic Examples

### 1. Simple Smart Pointer Usage

```typescript
// unique_ptr example
function createUniqueNumber(): unique_ptr<number> {
    return new unique_ptr<number>(42);
}

function useUniqueNumber(): number {
    let ptr = createUniqueNumber();
    return ptr.get() * 2; // Returns 84
    // ptr is automatically cleaned up
}

// shared_ptr example
function createSharedString(): shared_ptr<string> {
    return new shared_ptr<string>("Hello World");
}

function useSharedString(): string {
    let ptr1 = createSharedString();
    let ptr2 = shared_ptr<string> = ptr1; // Both reference the same object
    
    return ptr1.get() + " " + ptr2.get(); // Returns "Hello World Hello World"
    // Object is cleaned up when both pointers go out of scope
}
```

### 2. Move Semantics

```typescript
function demonstrateMoveSemantics(): void {
    // Create a unique pointer
    let source: unique_ptr<number> = new unique_ptr<number>(100);
    
    // Move ownership
    let destination: unique_ptr<number> = std::move(source);
    
    console.log("Source after move:", source.get()); // null
    console.log("Destination after move:", destination.get()); // 100
    
    // source is now null and cannot be used
    // destination owns the object
}
```

### 3. Weak References

```typescript
function demonstrateWeakReferences(): void {
    // Create a strong reference
    let strong: shared_ptr<number> = new shared_ptr<number>(42);
    
    // Create a weak reference
    let weak: weak_ptr<number> = new weak_ptr<number>(strong);
    
    console.log("Strong reference:", strong.get()); // 42
    console.log("Weak reference:", weak.get()); // 42
    
    // Clear the strong reference
    strong = null;
    
    console.log("Weak reference after strong cleared:", weak.get()); // null
}
```

## Intermediate Examples

### 4. Class with Smart Pointer Members

```typescript
class SmartPointerContainer {
    private uniqueData: unique_ptr<number>;
    private sharedData: shared_ptr<string>;
    private weakData: weak_ptr<string>;
    
    constructor() {
        this.uniqueData = new unique_ptr<number>(999);
        this.sharedData = new shared_ptr<string>("Container data");
        this.weakData = new weak_ptr<string>(this.sharedData);
    }
    
    getUniqueData(): number {
        return this.uniqueData.get();
    }
    
    getSharedData(): string {
        return this.sharedData.get();
    }
    
    getWeakData(): string | null {
        return this.weakData.get();
    }
    
    // Move constructor
    moveFrom(other: SmartPointerContainer): void {
        this.uniqueData = std::move(other.uniqueData);
        this.sharedData = other.sharedData;
        this.weakData = other.weakData;
    }
}

function useContainer(): void {
    let container = new SmartPointerContainer();
    
    console.log("Unique data:", container.getUniqueData());
    console.log("Shared data:", container.getSharedData());
    console.log("Weak data:", container.getWeakData());
    
    // Container is automatically cleaned up
}
```

### 5. Array of Smart Pointers

```typescript
function createNumberArray(): shared_ptr<number>[] {
    let numbers: shared_ptr<number>[] = [];
    
    for (let i = 0; i < 10; i++) {
        numbers.push(new shared_ptr<number>(i * 10));
    }
    
    return numbers;
}

function processNumberArray(): void {
    let numbers = createNumberArray();
    
    console.log("Number array:");
    for (let i = 0; i < numbers.length; i++) {
        console.log(`numbers[${i}]:`, numbers[i].get());
    }
    
    // All numbers are automatically cleaned up
}
```

### 6. Function Parameters and Return Values

```typescript
function processSmartPointer(ptr: shared_ptr<number>): shared_ptr<string> {
    let value = ptr.get();
    let result = new shared_ptr<string>(`Processed: ${value}`);
    return result;
}

function demonstrateFunctionCalls(): void {
    let input: shared_ptr<number> = new shared_ptr<number>(42);
    let output: shared_ptr<string> = processSmartPointer(input);
    
    console.log("Input:", input.get());
    console.log("Output:", output.get());
    
    // Both input and output are automatically cleaned up
}
```

## Advanced Examples

### 7. Reference Cycle Prevention

```typescript
class Node {
    private value: number;
    private next: weak_ptr<Node>; // Use weak_ptr to break cycles
    
    constructor(value: number) {
        this.value = value;
    }
    
    setNext(node: shared_ptr<Node>): void {
        this.next = new weak_ptr<Node>(node);
    }
    
    getNext(): shared_ptr<Node> | null {
        return this.next.get();
    }
    
    getValue(): number {
        return this.value;
    }
}

function createLinkedList(): void {
    let node1: shared_ptr<Node> = new shared_ptr<Node>(1);
    let node2: shared_ptr<Node> = new shared_ptr<Node>(2);
    let node3: shared_ptr<Node> = new shared_ptr<Node>(3);
    
    // Create a cycle using weak references
    node1.setNext(node2);
    node2.setNext(node3);
    node3.setNext(node1);
    
    console.log("Linked list created with cycle prevention");
    
    // All nodes are automatically cleaned up
}
```

### 8. Memory Management Annotations

```typescript
@manual_memory
class ManualMemoryClass {
    private data: number[];
    
    constructor(size: number) {
        this.data = new Array<number>(size);
        for (let i = 0; i < size; i++) {
            this.data[i] = i;
        }
    }
    
    getData(): number[] {
        return this.data;
    }
    
    // Manual cleanup required
    destroy(): void {
        this.data = null;
    }
}

@stack_allocated
class StackAllocatedClass {
    private value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    getValue(): number {
        return this.value;
    }
}

@immutable
class ImmutableClass {
    private readonly value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    getValue(): number {
        return this.value;
    }
}

function demonstrateAnnotations(): void {
    let manual = new ManualMemoryClass(100);
    let stack = new StackAllocatedClass(42);
    let immutable = new ImmutableClass(999);
    
    console.log("Manual memory class data length:", manual.getData().length);
    console.log("Stack allocated class value:", stack.getValue());
    console.log("Immutable class value:", immutable.getValue());
    
    // Manual cleanup required for manual memory class
    manual.destroy();
    
    // Stack and immutable classes are automatically cleaned up
}
```

### 9. Database Connection Example

```typescript
class DatabaseConnection {
    private connection: shared_ptr<object>;
    private isConnected: boolean;
    
    constructor() {
        this.connection = new shared_ptr<object>({ connected: true });
        this.isConnected = true;
    }
    
    query(sql: string): shared_ptr<object[]> {
        if (!this.isConnected) {
            throw new Error("Database not connected");
        }
        
        let results: shared_ptr<object>[] = [];
        for (let i = 0; i < 5; i++) {
            results.push(new shared_ptr<object>({ 
                id: i, 
                data: `Result ${i}` 
            }));
        }
        
        return new shared_ptr<object[]>(results);
    }
    
    close(): void {
        this.connection = null;
        this.isConnected = false;
    }
}

function demonstrateDatabase(): void {
    let db = new DatabaseConnection();
    
    try {
        let results = db.query("SELECT * FROM users");
        let resultArray = results.get();
        
        console.log("Query returned", resultArray.length, "results");
        
        for (let i = 0; i < resultArray.length; i++) {
            console.log("Result", i, ":", resultArray[i].get());
        }
        
    } finally {
        db.close();
    }
}
```

### 10. Performance-Critical Example

```typescript
class PerformanceCriticalClass {
    private data: unique_ptr<number[]>;
    private size: number;
    
    constructor(size: number) {
        this.size = size;
        this.data = new unique_ptr<number[]>(new Array<number>(size));
        
        // Initialize data
        for (let i = 0; i < size; i++) {
            this.data.get()[i] = i;
        }
    }
    
    // Move constructor for performance
    moveFrom(other: PerformanceCriticalClass): void {
        this.data = std::move(other.data);
        this.size = other.size;
    }
    
    // Efficient data access
    getData(): number[] {
        return this.data.get();
    }
    
    // Process data efficiently
    processData(): number {
        let sum = 0;
        let data = this.data.get();
        
        for (let i = 0; i < this.size; i++) {
            sum += data[i];
        }
        
        return sum;
    }
}

function demonstratePerformance(): void {
    let obj = new PerformanceCriticalClass(1000);
    
    console.log("Data sum:", obj.processData());
    
    // Move to another object efficiently
    let obj2 = new PerformanceCriticalClass(0);
    obj2.moveFrom(obj);
    
    console.log("Moved data sum:", obj2.processData());
}
```

## Error Handling Examples

### 11. Safe Smart Pointer Usage

```typescript
function safeSmartPointerUsage(): void {
    let ptr: shared_ptr<number> | null = null;
    
    try {
        ptr = new shared_ptr<number>(42);
        let value = ptr.get();
        
        if (value !== null) {
            console.log("Value:", value);
        }
        
    } catch (error) {
        console.error("Error:", error);
    } finally {
        // ptr is automatically cleaned up
    }
}
```

### 12. Weak Reference Safety

```typescript
function safeWeakReferenceUsage(): void {
    let strong: shared_ptr<number> = new shared_ptr<number>(100);
    let weak: weak_ptr<number> = new weak_ptr<number>(strong);
    
    // Safe access to weak reference
    let value = weak.get();
    if (value !== null) {
        console.log("Weak reference value:", value);
    } else {
        console.log("Object has been deallocated");
    }
    
    // Clear strong reference
    strong = null;
    
    // Check weak reference again
    value = weak.get();
    if (value !== null) {
        console.log("Weak reference still valid:", value);
    } else {
        console.log("Object has been deallocated");
    }
}
```

## Testing Examples

### 13. Unit Test Example

```typescript
function testSmartPointerBasic(): void {
    // Test unique_ptr
    let unique: unique_ptr<number> = new unique_ptr<number>(42);
    assert(unique.get() === 42, "unique_ptr should contain 42");
    
    // Test shared_ptr
    let shared1: shared_ptr<string> = new shared_ptr<string>("Hello");
    let shared2: shared_ptr<string> = shared1;
    assert(shared1.get() === "Hello", "shared_ptr should contain 'Hello'");
    assert(shared2.get() === "Hello", "shared_ptr should contain 'Hello'");
    
    // Test weak_ptr
    let strong: shared_ptr<number> = new shared_ptr<number>(100);
    let weak: weak_ptr<number> = new weak_ptr<number>(strong);
    assert(weak.get() === 100, "weak_ptr should contain 100");
    
    strong = null;
    assert(weak.get() === null, "weak_ptr should be null after strong reference cleared");
    
    console.log("All basic smart pointer tests passed");
}
```

### 14. Memory Leak Test

```typescript
function testMemoryLeaks(): void {
    console.log("Testing memory leaks...");
    
    // Create many objects
    for (let i = 0; i < 1000; i++) {
        let obj = new shared_ptr<number>(i);
        // Objects should be automatically cleaned up
    }
    
    console.log("Memory leak test completed");
}
```

## Conclusion

These examples demonstrate the power and flexibility of ARC memory management in TSC. By following these patterns and
best practices, you can write efficient, safe, and maintainable code with automatic memory management.