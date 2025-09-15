// RAII Performance Tests - Benchmarking RAII performance impact
// Tests to measure allocation, deallocation, and overall performance overhead

// Performance Test 1: Basic RAII Performance
class BasicRAIIPerf {
    private name: string;
    private data: number[];
    
    constructor(name: string, size: number) {
        this.name = name;
        this.data = new Array(size);
        for (let i = 0; i < size; i++) {
            this.data[i] = i;
        }
    }
    
    ~BasicRAIIPerf() {
        // Basic cleanup
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[] {
        return this.data;
    }
}

// Performance Test 2: Complex RAII Performance
class ComplexRAIIPerf {
    private name: string;
    private data: number[][];
    private metadata: Map<string, any>;
    
    constructor(name: string, rows: number, cols: number) {
        this.name = name;
        this.data = new Array(rows);
        this.metadata = new Map();
        
        for (let i = 0; i < rows; i++) {
            this.data[i] = new Array(cols);
            for (let j = 0; j < cols; j++) {
                this.data[i][j] = i * j;
            }
        }
        
        this.metadata.set("rows", rows);
        this.metadata.set("cols", cols);
        this.metadata.set("total", rows * cols);
    }
    
    ~ComplexRAIIPerf() {
        // Complex cleanup
        this.data = [];
        this.metadata.clear();
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[][] {
        return this.data;
    }
    
    getMetadata(): Map<string, any> {
        return this.metadata;
    }
}

// Performance Test 3: RAII with Smart Pointers
class SmartPointerRAIIPerf {
    private name: string;
    private ptr: shared_ptr<number[]>;
    private refCount: number;
    
    constructor(name: string, size: number) {
        this.name = name;
        this.ptr = new shared_ptr<number[]>(new Array(size));
        this.refCount = 1;
        
        for (let i = 0; i < size; i++) {
            this.ptr.get()[i] = i;
        }
    }
    
    ~SmartPointerRAIIPerf() {
        this.refCount--;
        if (this.refCount === 0) {
            this.ptr = null;
        }
    }
    
    getName(): string {
        return this.name;
    }
    
    getPtr(): shared_ptr<number[]> {
        return this.ptr;
    }
    
    getRefCount(): number {
        return this.refCount;
    }
}

// Performance Test 4: RAII with Inheritance
class BaseRAIIPerf {
    protected name: string;
    protected data: number[];
    
    constructor(name: string, size: number) {
        this.name = name;
        this.data = new Array(size);
        for (let i = 0; i < size; i++) {
            this.data[i] = i;
        }
    }
    
    ~BaseRAIIPerf() {
        // Base cleanup
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[] {
        return this.data;
    }
}

class DerivedRAIIPerf extends BaseRAIIPerf {
    private extra: number[];
    
    constructor(name: string, size: number) {
        super(name, size);
        this.extra = new Array(size);
        for (let i = 0; i < size; i++) {
            this.extra[i] = i * 2;
        }
    }
    
    ~DerivedRAIIPerf() {
        // Derived cleanup
        this.extra = [];
    }
    
    getExtra(): number[] {
        return this.extra;
    }
}

// Performance Test 5: RAII with Generic Types
class GenericRAIIPerf<T> {
    private name: string;
    private data: T[];
    
    constructor(name: string, size: number, defaultValue: T) {
        this.name = name;
        this.data = new Array(size);
        for (let i = 0; i < size; i++) {
            this.data[i] = defaultValue;
        }
    }
    
    ~GenericRAIIPerf() {
        // Generic cleanup
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): T[] {
        return this.data;
    }
}

// Performance Test 6: RAII with Nested Resources
class NestedRAIIPerf {
    private name: string;
    private inner: BasicRAIIPerf;
    private counter: number;
    
    constructor(name: string, size: number) {
        this.name = name;
        this.inner = new BasicRAIIPerf("inner-" + name, size);
        this.counter = 0;
    }
    
    ~NestedRAIIPerf() {
        // Nested cleanup
        this.counter = -1;
    }
    
    getName(): string {
        return this.name;
    }
    
    getCounter(): number {
        return this.counter;
    }
    
    getInner(): BasicRAIIPerf {
        return this.inner;
    }
}

// Performance Test 7: RAII with Resource Pool
class ResourcePoolRAIIPerf {
    private name: string;
    private pool: BasicRAIIPerf[];
    private maxSize: number;
    
    constructor(name: string, maxSize: number) {
        this.name = name;
        this.maxSize = maxSize;
        this.pool = [];
    }
    
    ~ResourcePoolRAIIPerf() {
        // Pool cleanup
        this.pool = [];
    }
    
    getName(): string {
        return this.name;
    }
    
    getMaxSize(): number {
        return this.maxSize;
    }
    
    getPoolSize(): number {
        return this.pool.length;
    }
    
    addResource(resource: BasicRAIIPerf): boolean {
        if (this.pool.length < this.maxSize) {
            this.pool.push(resource);
            return true;
        }
        return false;
    }
    
    removeResource(): BasicRAIIPerf | null {
        if (this.pool.length > 0) {
            return this.pool.pop() || null;
        }
        return null;
    }
}

// Performance Test 8: RAII with Move Semantics
class MoveableRAIIPerf {
    private name: string;
    private data: number[];
    private moved: boolean;
    
    constructor(name: string, size: number) {
        this.name = name;
        this.data = new Array(size);
        this.moved = false;
        for (let i = 0; i < size; i++) {
            this.data[i] = i;
        }
    }
    
    ~MoveableRAIIPerf() {
        if (!this.moved) {
            // Only cleanup if not moved
        }
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[] {
        return this.data;
    }
    
    move(): MoveableRAIIPerf {
        this.moved = true;
        return this;
    }
    
    isMoved(): boolean {
        return this.moved;
    }
}

// Performance Test 9: RAII with Exception Handling
class ExceptionRAIIPerf {
    private name: string;
    private data: number[];
    private shouldThrow: boolean;
    
    constructor(name: string, size: number, shouldThrow: boolean = false) {
        this.name = name;
        this.data = new Array(size);
        this.shouldThrow = shouldThrow;
        for (let i = 0; i < size; i++) {
            this.data[i] = i;
        }
    }
    
    ~ExceptionRAIIPerf() {
        if (this.shouldThrow) {
            throw new Error("Destructor exception");
        }
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[] {
        return this.data;
    }
}

// Performance Test 10: RAII with Thread Safety
class ThreadSafeRAIIPerf {
    private name: string;
    private data: number[];
    private counter: number;
    
    constructor(name: string, size: number) {
        this.name = name;
        this.data = new Array(size);
        this.counter = 0;
        for (let i = 0; i < size; i++) {
            this.data[i] = i;
        }
    }
    
    ~ThreadSafeRAIIPerf() {
        // Thread-safe cleanup
        this.counter = -1;
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[] {
        return this.data;
    }
    
    getCounter(): number {
        return this.counter;
    }
    
    incrementCounter(): void {
        this.counter++;
    }
}

// Performance benchmark functions
function benchmarkBasicRAII(iterations: number): void {
    _print(`=== Benchmarking Basic RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new BasicRAIIPerf(`test-${i}`, 1000);
        let name = resource.getName();
        let data = resource.getData();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Basic RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkComplexRAII(iterations: number): void {
    _print(`=== Benchmarking Complex RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new ComplexRAIIPerf(`test-${i}`, 100, 100);
        let name = resource.getName();
        let data = resource.getData();
        let metadata = resource.getMetadata();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Complex RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkSmartPointerRAII(iterations: number): void {
    _print(`=== Benchmarking Smart Pointer RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new SmartPointerRAIIPerf(`test-${i}`, 1000);
        let name = resource.getName();
        let ptr = resource.getPtr();
        let refCount = resource.getRefCount();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Smart Pointer RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkInheritanceRAII(iterations: number): void {
    _print(`=== Benchmarking Inheritance RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new DerivedRAIIPerf(`test-${i}`, 1000);
        let name = resource.getName();
        let data = resource.getData();
        let extra = resource.getExtra();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Inheritance RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkGenericRAII(iterations: number): void {
    _print(`=== Benchmarking Generic RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new GenericRAIIPerf<number>(`test-${i}`, 1000, 42);
        let name = resource.getName();
        let data = resource.getData();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Generic RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkNestedRAII(iterations: number): void {
    _print(`=== Benchmarking Nested RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new NestedRAIIPerf(`test-${i}`, 1000);
        let name = resource.getName();
        let counter = resource.getCounter();
        let inner = resource.getInner();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Nested RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkResourcePoolRAII(iterations: number): void {
    _print(`=== Benchmarking Resource Pool RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let pool = new ResourcePoolRAIIPerf(`test-${i}`, 10);
        let name = pool.getName();
        let maxSize = pool.getMaxSize();
        let poolSize = pool.getPoolSize();
        
        // Add some resources
        for (let j = 0; j < 5; j++) {
            let resource = new BasicRAIIPerf(`resource-${j}`, 100);
            pool.addResource(resource);
        }
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Resource Pool RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkMoveableRAII(iterations: number): void {
    _print(`=== Benchmarking Moveable RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new MoveableRAIIPerf(`test-${i}`, 1000);
        let name = resource.getName();
        let data = resource.getData();
        let moved = resource.move();
        let isMoved = moved.isMoved();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Moveable RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkExceptionRAII(iterations: number): void {
    _print(`=== Benchmarking Exception RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        try {
            let resource = new ExceptionRAIIPerf(`test-${i}`, 1000, false);
            let name = resource.getName();
            let data = resource.getData();
        } catch (e) {
            // Handle exception
        }
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Exception RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

function benchmarkThreadSafeRAII(iterations: number): void {
    _print(`=== Benchmarking Thread Safe RAII (${iterations} iterations) ===`);
    let startTime = Date.now();
    
    for (let i = 0; i < iterations; i++) {
        let resource = new ThreadSafeRAIIPerf(`test-${i}`, 1000);
        let name = resource.getName();
        let data = resource.getData();
        let counter = resource.getCounter();
        resource.incrementCounter();
    }
    
    let endTime = Date.now();
    let duration = endTime - startTime;
    _print(`Thread Safe RAII: ${duration}ms for ${iterations} iterations`);
    _print(`Average: ${duration / iterations}ms per iteration`);
}

// Run all performance benchmarks
function runAllPerformanceTests(): void {
    _print("🚀 Starting RAII Performance Tests");
    _print("====================================");
    
    let iterations = 1000;
    
    benchmarkBasicRAII(iterations);
    benchmarkComplexRAII(iterations);
    benchmarkSmartPointerRAII(iterations);
    benchmarkInheritanceRAII(iterations);
    benchmarkGenericRAII(iterations);
    benchmarkNestedRAII(iterations);
    benchmarkResourcePoolRAII(iterations);
    benchmarkMoveableRAII(iterations);
    benchmarkExceptionRAII(iterations);
    benchmarkThreadSafeRAII(iterations);
    
    _print("====================================");
    _print("✅ All RAII Performance Tests Completed");
}

// Export for testing
export { runAllPerformanceTests };