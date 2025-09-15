// RAII Unit Tests - Comprehensive Testing Suite
// Tests for RAII functionality, edge cases, and error conditions

// Test 1: Basic RAII with simple resource
class SimpleResource {
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log(`Resource '${name}' acquired`);
    }
    
    ~SimpleResource() {
        console.log(`Resource '${name}' released`);
    }
    
    getName(): string {
        return this.name;
    }
}

// Test 2: RAII with complex logic in destructor
class ComplexResource {
    private data: number[];
    private isOpen: boolean;
    
    constructor(size: number) {
        this.data = new Array(size);
        this.isOpen = true;
        console.log(`Complex resource opened with ${size} elements`);
    }
    
    ~ComplexResource() {
        if (this.isOpen) {
            console.log("Closing complex resource...");
            this.data = [];
            this.isOpen = false;
            console.log("Complex resource closed");
        }
    }
    
    getData(): number[] {
        return this.data;
    }
}

// Test 3: RAII with nested resource management
class NestedResource {
    private inner: SimpleResource;
    private counter: number;
    
    constructor(name: string) {
        this.inner = new SimpleResource(name);
        this.counter = 0;
        console.log("Nested resource created");
    }
    
    ~NestedResource() {
        console.log("Cleaning up nested resource...");
        this.counter = -1;
        console.log("Nested resource cleaned up");
    }
    
    getCounter(): number {
        return this.counter;
    }
}

// Test 4: RAII with error handling
class ErrorProneResource {
    private shouldThrow: boolean;
    private name: string;
    
    constructor(name: string, shouldThrow: boolean = false) {
        this.name = name;
        this.shouldThrow = shouldThrow;
        console.log(`Error-prone resource '${name}' created`);
    }
    
    ~ErrorProneResource() {
        console.log(`Cleaning up error-prone resource '${this.name}'`);
        if (this.shouldThrow) {
            console.log("ERROR: Destructor throwing exception!");
            throw new Error("Destructor error");
        }
        console.log(`Error-prone resource '${this.name}' cleaned up`);
    }
}

// Test 5: RAII with shared ownership
class SharedResource {
    private name: string;
    private refCount: number;
    
    constructor(name: string) {
        this.name = name;
        this.refCount = 1;
        console.log(`Shared resource '${name}' created (refCount: ${this.refCount})`);
    }
    
    ~SharedResource() {
        this.refCount--;
        console.log(`Shared resource '${this.name}' cleaned up (refCount: ${this.refCount})`);
    }
    
    addRef(): void {
        this.refCount++;
        console.log(`Shared resource '${this.name}' refCount increased to ${this.refCount}`);
    }
    
    getRefCount(): number {
        return this.refCount;
    }
}

// Test 6: RAII with circular dependencies (should be handled safely)
class CircularA {
    private b: CircularB | null;
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        this.b = null;
        console.log(`CircularA '${name}' created`);
    }
    
    ~CircularA() {
        console.log(`CircularA '${this.name}' destroyed`);
        this.b = null;
    }
    
    setB(b: CircularB): void {
        this.b = b;
    }
    
    getName(): string {
        return this.name;
    }
}

class CircularB {
    private a: CircularA | null;
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        this.a = null;
        console.log(`CircularB '${name}' created`);
    }
    
    ~CircularB() {
        console.log(`CircularB '${this.name}' destroyed`);
        this.a = null;
    }
    
    setA(a: CircularA): void {
        this.a = a;
    }
    
    getName(): string {
        return this.name;
    }
}

// Test 7: RAII with inheritance
class BaseResource {
    protected name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log(`Base resource '${name}' created`);
    }
    
    ~BaseResource() {
        console.log(`Base resource '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
}

class DerivedResource extends BaseResource {
    private extra: string;
    
    constructor(name: string, extra: string) {
        super(name);
        this.extra = extra;
        console.log(`Derived resource '${name}' with extra '${extra}' created`);
    }
    
    ~DerivedResource() {
        console.log(`Derived resource '${this.name}' with extra '${this.extra}' destroyed`);
    }
    
    getExtra(): string {
        return this.extra;
    }
}

// Test 8: RAII with generic types
class GenericResource<T> {
    private data: T;
    private name: string;
    
    constructor(name: string, data: T) {
        this.name = name;
        this.data = data;
        console.log(`Generic resource '${name}' created with data`);
    }
    
    ~GenericResource() {
        console.log(`Generic resource '${this.name}' destroyed`);
    }
    
    getData(): T {
        return this.data;
    }
    
    getName(): string {
        return this.name;
    }
}

// Test 9: RAII with smart pointers
class SmartPointerResource {
    private ptr: shared_ptr<string>;
    private name: string;
    
    constructor(name: string, value: string) {
        this.name = name;
        this.ptr = new shared_ptr<string>(value);
        console.log(`Smart pointer resource '${name}' created`);
    }
    
    ~SmartPointerResource() {
        console.log(`Smart pointer resource '${this.name}' destroyed`);
        this.ptr = null;
    }
    
    getValue(): string {
        return this.ptr ? this.ptr.get() : "";
    }
}

// Test 10: RAII with move semantics
class MoveableResource {
    private data: number[];
    private name: string;
    private moved: boolean;
    
    constructor(name: string, size: number) {
        this.name = name;
        this.data = new Array(size);
        this.moved = false;
        console.log(`Moveable resource '${name}' created`);
    }
    
    ~MoveableResource() {
        if (!this.moved) {
            console.log(`Moveable resource '${this.name}' destroyed`);
        } else {
            console.log(`Moveable resource '${this.name}' was moved, not destroyed`);
        }
    }
    
    move(): MoveableResource {
        this.moved = true;
        console.log(`Moveable resource '${this.name}' moved`);
        return this;
    }
    
    getName(): string {
        return this.name;
    }
}

// Test functions
function testBasicRAII(): void {
    console.log("=== Testing Basic RAII ===");
    let resource = new SimpleResource("test");
    console.log(`Resource name: ${resource.getName()}`);
    // Resource should be destroyed when going out of scope
}

function testComplexRAII(): void {
    console.log("=== Testing Complex RAII ===");
    let resource = new ComplexResource(100);
    console.log(`Resource data length: ${resource.getData().length}`);
    // Resource should be destroyed when going out of scope
}

function testNestedRAII(): void {
    console.log("=== Testing Nested RAII ===");
    let resource = new NestedResource("nested");
    console.log(`Resource counter: ${resource.getCounter()}`);
    // Resource should be destroyed when going out of scope
}

function testErrorHandling(): void {
    console.log("=== Testing Error Handling ===");
    try {
        let resource = new ErrorProneResource("error-test", true);
        // Resource should be destroyed when going out of scope
    } catch (e) {
        console.log("Caught error in destructor");
    }
}

function testSharedOwnership(): void {
    console.log("=== Testing Shared Ownership ===");
    let resource = new SharedResource("shared");
    resource.addRef();
    console.log(`Resource ref count: ${resource.getRefCount()}`);
    // Resource should be destroyed when going out of scope
}

function testCircularDependencies(): void {
    console.log("=== Testing Circular Dependencies ===");
    let a = new CircularA("A");
    let b = new CircularB("B");
    a.setB(b);
    b.setA(a);
    console.log(`CircularA name: ${a.getName()}`);
    console.log(`CircularB name: ${b.getName()}`);
    // Resources should be destroyed when going out of scope
}

function testInheritance(): void {
    console.log("=== Testing Inheritance ===");
    let resource = new DerivedResource("derived", "extra");
    console.log(`Resource name: ${resource.getName()}`);
    console.log(`Resource extra: ${resource.getExtra()}`);
    // Resource should be destroyed when going out of scope
}

function testGenericTypes(): void {
    console.log("=== Testing Generic Types ===");
    let resource = new GenericResource<string>("generic", "test-value");
    console.log(`Resource name: ${resource.getName()}`);
    console.log(`Resource data: ${resource.getData()}`);
    // Resource should be destroyed when going out of scope
}

function testSmartPointers(): void {
    console.log("=== Testing Smart Pointers ===");
    let resource = new SmartPointerResource("smart", "smart-value");
    console.log(`Resource value: ${resource.getValue()}`);
    // Resource should be destroyed when going out of scope
}

function testMoveSemantics(): void {
    console.log("=== Testing Move Semantics ===");
    let resource = new MoveableResource("moveable", 50);
    console.log(`Resource name: ${resource.getName()}`);
    let moved = resource.move();
    console.log(`Moved resource name: ${moved.getName()}`);
    // Resources should be destroyed when going out of scope
}

// Run all tests
function runAllTests(): void {
    console.log("🚀 Starting RAII Unit Tests");
    console.log("================================");
    
    testBasicRAII();
    testComplexRAII();
    testNestedRAII();
    testErrorHandling();
    testSharedOwnership();
    testCircularDependencies();
    testInheritance();
    testGenericTypes();
    testSmartPointers();
    testMoveSemantics();
    
    console.log("================================");
    console.log("✅ All RAII Unit Tests Completed");
}

// Export for testing
export { runAllTests };