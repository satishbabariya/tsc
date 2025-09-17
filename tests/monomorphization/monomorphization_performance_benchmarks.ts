// Performance Benchmarks for Monomorphization
// Tests compile time and runtime performance of generic code

// =============================================================================
// Benchmark 1: Generic Function Performance
// =============================================================================

function genericIdentity<T>(value: T): T {
    return value;
}

function genericTransform<T, U>(value: T, fn: (t: T) => U): U {
    return fn(value);
}

function genericArrayMap<T, U>(array: T[], fn: (t: T) => U): U[] {
    const result: U[] = [];
    for (let i = 0; i < array.length; i++) {
        result.push(fn(array[i]));
    }
    return result;
}

function genericArrayFilter<T>(array: T[], predicate: (t: T) => boolean): T[] {
    const result: T[] = [];
    for (let i = 0; i < array.length; i++) {
        if (predicate(array[i])) {
            result.push(array[i]);
        }
    }
    return result;
}

function genericArrayReduce<T, U>(array: T[], fn: (acc: U, t: T) => U, initial: U): U {
    let result = initial;
    for (let i = 0; i < array.length; i++) {
        result = fn(result, array[i]);
    }
    return result;
}

// Benchmark data
const largeArray = Array.from({ length: 10000 }, (_, i) => i);
const stringArray = Array.from({ length: 10000 }, (_, i) => `item_${i}`);

// =============================================================================
// Benchmark 2: Generic Class Performance
// =============================================================================

class GenericContainer<T> {
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
    
    map<U>(fn: (item: T) => U): GenericContainer<U> {
        const result = new GenericContainer<U>();
        for (const item of this.items) {
            result.add(fn(item));
        }
        return result;
    }
    
    filter(predicate: (item: T) => boolean): GenericContainer<T> {
        const result = new GenericContainer<T>();
        for (const item of this.items) {
            if (predicate(item)) {
                result.add(item);
            }
        }
        return result;
    }
    
    reduce<U>(fn: (acc: U, item: T) => U, initial: U): U {
        let result = initial;
        for (const item of this.items) {
            result = fn(result, item);
        }
        return result;
    }
}

// =============================================================================
// Benchmark 3: Generic Interface Performance
// =============================================================================

interface GenericProcessor<T> {
    process(value: T): T;
    validate(value: T): boolean;
}

class NumberProcessor implements GenericProcessor<number> {
    process(value: number): number {
        return value * 2;
    }
    
    validate(value: number): boolean {
        return value > 0;
    }
}

class StringProcessor implements GenericProcessor<string> {
    process(value: string): string {
        return value.toUpperCase();
    }
    
    validate(value: string): boolean {
        return value.length > 0;
    }
}

// =============================================================================
// Benchmark 4: Nested Generic Performance
// =============================================================================

class NestedGeneric<T> {
    private inner: GenericContainer<T>;
    
    constructor() {
        this.inner = new GenericContainer<T>();
    }
    
    add(item: T): void {
        this.inner.add(item);
    }
    
    transform<U>(fn: (item: T) => U): NestedGeneric<U> {
        const result = new NestedGeneric<U>();
        const transformed = this.inner.map(fn);
        for (let i = 0; i < transformed.size(); i++) {
            const item = transformed.get(i);
            if (item !== undefined) {
                result.add(item);
            }
        }
        return result;
    }
    
    getSize(): number {
        return this.inner.size();
    }
}

// =============================================================================
// Benchmark 5: Generic Recursive Performance
// =============================================================================

interface GenericTreeNode<T> {
    value: T;
    left?: GenericTreeNode<T>;
    right?: GenericTreeNode<T>;
}

class GenericTree<T> {
    private root?: GenericTreeNode<T>;
    
    insert(value: T): void {
        this.root = this.insertNode(this.root, value);
    }
    
    private insertNode(node: GenericTreeNode<T> | undefined, value: T): GenericTreeNode<T> {
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
    
    private searchNode(node: GenericTreeNode<T> | undefined, value: T): boolean {
        if (!node) return false;
        if (value === node.value) return true;
        if (value < node.value) return this.searchNode(node.left, value);
        return this.searchNode(node.right, value);
    }
    
    toArray(): T[] {
        const result: T[] = [];
        this.collectValues(this.root, result);
        return result;
    }
    
    private collectValues(node: GenericTreeNode<T> | undefined, result: T[]): void {
        if (!node) return;
        this.collectValues(node.left, result);
        result.push(node.value);
        this.collectValues(node.right, result);
    }
}

// =============================================================================
// Benchmark 6: Generic Union Performance
// =============================================================================

type GenericUnion<T, U> = T | U;

class UnionProcessor<T, U> {
    static process<T, U>(value: GenericUnion<T, U>): string {
        if (typeof value === "string") {
            return `String: ${value}`;
        } else if (typeof value === "number") {
            return `Number: ${value}`;
        } else if (typeof value === "boolean") {
            return `Boolean: ${value}`;
        } else {
            return `Object: ${JSON.stringify(value)}`;
        }
    }
    
    static isFirstType<T, U>(value: GenericUnion<T, U>): value is T {
        return typeof value === "string";
    }
}

// =============================================================================
// Benchmark 7: Generic Constraint Performance
// =============================================================================

interface ConstraintInterface<T extends string | number> {
    value: T;
    process(): T;
}

class ConstraintProcessor<T extends string | number> implements ConstraintInterface<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    process(): T {
        if (typeof this.value === "string") {
            return this.value.toUpperCase() as T;
        } else {
            return (this.value * 2) as T;
        }
    }
}

// =============================================================================
// Benchmark 8: Generic Method Overloading Performance
// =============================================================================

class OverloadProcessor {
    static process<T>(value: T): string {
        return `Generic: ${value}`;
    }
    
    static process(value: number): string {
        return `Number: ${value}`;
    }
    
    static process(value: string): string {
        return `String: ${value}`;
    }
    
    static process(value: boolean): string {
        return `Boolean: ${value}`;
    }
    
    static processArray<T>(values: T[]): string[] {
        return values.map(v => OverloadProcessor.process(v));
    }
}

// =============================================================================
// Benchmark 9: Generic Error Handling Performance
// =============================================================================

type GenericResult<T, E = string> = 
    | { success: true; data: T }
    | { success: false; error: E };

class ErrorProcessor<T, E = string> {
    static success<T>(data: T): GenericResult<T> {
        return { success: true, data };
    }
    
    static error<T, E = string>(error: E): GenericResult<T, E> {
        return { success: false, error };
    }
    
    static tryProcess<T, E = string>(
        fn: () => T,
        errorHandler: (error: E) => GenericResult<T, E>
    ): GenericResult<T, E> {
        try {
            const result = fn();
            return { success: true, data: result };
        } catch (error) {
            return errorHandler(error as E);
        }
    }
}

// =============================================================================
// Benchmark 10: Generic Memory Management Performance
// =============================================================================

class MemoryIntensiveGeneric<T> {
    private data: T[][];
    
    constructor(size: number) {
        this.data = [];
        for (let i = 0; i < size; i++) {
            this.data.push([]);
        }
    }
    
    addToRow<T>(row: number, item: T): void {
        if (row >= 0 && row < this.data.length) {
            this.data[row].push(item as any);
        }
    }
    
    getRow(row: number): T[] {
        if (row >= 0 && row < this.data.length) {
            return this.data[row];
        }
        return [];
    }
    
    processAll<U>(fn: (item: T) => U): U[][] {
        const result: U[][] = [];
        for (const row of this.data) {
            const processedRow: U[] = [];
            for (const item of row) {
                processedRow.push(fn(item));
            }
            result.push(processedRow);
        }
        return result;
    }
    
    clear(): void {
        for (const row of this.data) {
            row.length = 0;
        }
    }
}

// =============================================================================
// Performance Test Functions
// =============================================================================

function runGenericFunctionBenchmark(): void {
    console.log("Running generic function benchmark...");
    
    // Test generic identity
    for (let i = 0; i < 1000; i++) {
        genericIdentity<number>(i);
        genericIdentity<string>(`item_${i}`);
        genericIdentity<boolean>(i % 2 === 0);
    }
    
    // Test generic transform
    for (let i = 0; i < 1000; i++) {
        genericTransform<number, string>(i, x => x.toString());
        genericTransform<string, number>(`item_${i}`, x => x.length);
    }
    
    // Test generic array operations
    genericArrayMap<number, string>(largeArray, x => x.toString());
    genericArrayFilter<number>(largeArray, x => x % 2 === 0);
    genericArrayReduce<number, number>(largeArray, (acc, x) => acc + x, 0);
    
    console.log("Generic function benchmark completed");
}

function runGenericClassBenchmark(): void {
    console.log("Running generic class benchmark...");
    
    const numContainer = new GenericContainer<number>();
    const strContainer = new GenericContainer<string>();
    
    // Add items
    for (let i = 0; i < 1000; i++) {
        numContainer.add(i);
        strContainer.add(`item_${i}`);
    }
    
    // Test operations
    const mappedNumContainer = numContainer.map<string>(x => x.toString());
    const filteredNumContainer = numContainer.filter(x => x % 2 === 0);
    const reducedNum = numContainer.reduce<number>((acc, x) => acc + x, 0);
    
    const mappedStrContainer = strContainer.map<number>(x => x.length);
    const filteredStrContainer = strContainer.filter(x => x.length > 5);
    const reducedStr = strContainer.reduce<string>((acc, x) => acc + x, "");
    
    console.log("Generic class benchmark completed");
}

function runGenericInterfaceBenchmark(): void {
    console.log("Running generic interface benchmark...");
    
    const numProcessor = new NumberProcessor();
    const strProcessor = new StringProcessor();
    
    // Test processing
    for (let i = 0; i < 1000; i++) {
        numProcessor.process(i);
        numProcessor.validate(i);
        strProcessor.process(`item_${i}`);
        strProcessor.validate(`item_${i}`);
    }
    
    console.log("Generic interface benchmark completed");
}

function runNestedGenericBenchmark(): void {
    console.log("Running nested generic benchmark...");
    
    const nestedNum = new NestedGeneric<number>();
    const nestedStr = new NestedGeneric<string>();
    
    // Add items
    for (let i = 0; i < 500; i++) {
        nestedNum.add(i);
        nestedStr.add(`item_${i}`);
    }
    
    // Test transformation
    const transformedNum = nestedNum.transform<string>(x => x.toString());
    const transformedStr = nestedStr.transform<number>(x => x.length);
    
    console.log("Nested generic benchmark completed");
}

function runGenericTreeBenchmark(): void {
    console.log("Running generic tree benchmark...");
    
    const numTree = new GenericTree<number>();
    const strTree = new GenericTree<string>();
    
    // Insert items
    for (let i = 0; i < 1000; i++) {
        numTree.insert(i);
        strTree.insert(`item_${i}`);
    }
    
    // Test operations
    for (let i = 0; i < 100; i++) {
        numTree.search(i);
        strTree.search(`item_${i}`);
    }
    
    const numArray = numTree.toArray();
    const strArray = strTree.toArray();
    
    console.log("Generic tree benchmark completed");
}

function runGenericUnionBenchmark(): void {
    console.log("Running generic union benchmark...");
    
    const unionValues: GenericUnion<string, number>[] = [];
    
    // Create mixed array
    for (let i = 0; i < 1000; i++) {
        if (i % 2 === 0) {
            unionValues.push(i);
        } else {
            unionValues.push(`item_${i}`);
        }
    }
    
    // Process union values
    for (const value of unionValues) {
        UnionProcessor.process(value);
        UnionProcessor.isFirstType(value);
    }
    
    console.log("Generic union benchmark completed");
}

function runConstraintBenchmark(): void {
    console.log("Running constraint benchmark...");
    
    const numProcessor = new ConstraintProcessor<number>(42);
    const strProcessor = new ConstraintProcessor<string>("hello");
    
    // Test processing
    for (let i = 0; i < 1000; i++) {
        numProcessor.process();
        strProcessor.process();
    }
    
    console.log("Constraint benchmark completed");
}

function runOverloadBenchmark(): void {
    console.log("Running overload benchmark...");
    
    const testValues = [42, "hello", true, [1, 2, 3]];
    
    // Test overloading
    for (let i = 0; i < 1000; i++) {
        for (const value of testValues) {
            OverloadProcessor.process(value);
        }
    }
    
    // Test array processing
    OverloadProcessor.processArray<number>([1, 2, 3, 4, 5]);
    OverloadProcessor.processArray<string>(["a", "b", "c", "d", "e"]);
    
    console.log("Overload benchmark completed");
}

function runErrorHandlingBenchmark(): void {
    console.log("Running error handling benchmark...");
    
    // Test success cases
    for (let i = 0; i < 1000; i++) {
        ErrorProcessor.success<number>(i);
        ErrorProcessor.success<string>(`item_${i}`);
    }
    
    // Test error cases
    for (let i = 0; i < 1000; i++) {
        ErrorProcessor.error<number, string>(`Error ${i}`);
    }
    
    // Test try-catch
    for (let i = 0; i < 1000; i++) {
        ErrorProcessor.tryProcess<number, string>(
            () => i,
            (error) => ErrorProcessor.error<number, string>(error)
        );
    }
    
    console.log("Error handling benchmark completed");
}

function runMemoryBenchmark(): void {
    console.log("Running memory benchmark...");
    
    const memoryTest = new MemoryIntensiveGeneric<number>(100);
    
    // Add items
    for (let i = 0; i < 1000; i++) {
        memoryTest.addToRow(i % 100, i);
    }
    
    // Process all
    const processed = memoryTest.processAll<string>(x => x.toString());
    
    // Clear
    memoryTest.clear();
    
    console.log("Memory benchmark completed");
}

// =============================================================================
// Main Benchmark Runner
// =============================================================================

function main(): void {
    console.log("Starting monomorphization performance benchmarks...");
    
    const startTime = Date.now();
    
    runGenericFunctionBenchmark();
    runGenericClassBenchmark();
    runGenericInterfaceBenchmark();
    runNestedGenericBenchmark();
    runGenericTreeBenchmark();
    runGenericUnionBenchmark();
    runConstraintBenchmark();
    runOverloadBenchmark();
    runErrorHandlingBenchmark();
    runMemoryBenchmark();
    
    const endTime = Date.now();
    const totalTime = endTime - startTime;
    
    console.log(`All benchmarks completed in ${totalTime}ms`);
    console.log("Monomorphization performance benchmarks finished!");
}

main();