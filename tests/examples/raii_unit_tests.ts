// RAII Unit Tests - Comprehensive Testing Suite
// Tests for RAII functionality, edge cases, and error conditions

// Test 1: Basic RAII with simple resource
class SimpleResource {
    private name: string;

    constructor(name: string) {
        this.name = name;
        _print(`Resource '${name}' acquired`);
    }

~

    SimpleResource() {
        _print(`Resource '${this.name}' released`);
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
        _print(`Complex resource opened with ${size} elements`);
    }

~

    ComplexResource() {
        if (this.isOpen) {
            _print("Closing complex resource...");
            this.data = [];
            this.isOpen = false;
            _print("Complex resource closed");
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
        _print("Nested resource created");
    }

~

    NestedResource() {
        _print("Cleaning up nested resource...");
        this.counter = -1;
        _print("Nested resource cleaned up");
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
        _print(`Error-prone resource '${name}' created`);
    }

~

    ErrorProneResource() {
        _print(`Cleaning up error-prone resource '${this.name}'`);
        if (this.shouldThrow) {
            _print("ERROR: Destructor throwing exception!");
            throw new Error("Destructor error");
        }
        _print(`Error-prone resource '${this.name}' cleaned up`);
    }
}

// Test 5: RAII with shared ownership
class SharedResource {
    private name: string;
    private refCount: number;

    constructor(name: string) {
        this.name = name;
        this.refCount = 1;
        _print(`Shared resource '${name}' created (refCount: ${this.refCount})`);
    }

~

    SharedResource() {
        this.refCount--;
        _print(`Shared resource '${this.name}' cleaned up (refCount: ${this.refCount})`);
    }

    addRef(): void {
        this.refCount++;
        _print(`Shared resource '${this.name}' refCount increased to ${this.refCount}`);
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
        _print(`CircularA '${name}' created`);
    }

~

    CircularA() {
        _print(`CircularA '${this.name}' destroyed`);
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
        _print(`CircularB '${name}' created`);
    }

~

    CircularB() {
        _print(`CircularB '${this.name}' destroyed`);
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
        _print(`Base resource '${name}' created`);
    }

~

    BaseResource() {
        _print(`Base resource '${this.name}' destroyed`);
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
        _print(`Derived resource '${name}' with extra '${extra}' created`);
    }

~

    DerivedResource() {
        _print(`Derived resource '${this.name}' with extra '${this.extra}' destroyed`);
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
        _print(`Generic resource '${name}' created with data`);
    }

~

    GenericResource() {
        _print(`Generic resource '${this.name}' destroyed`);
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
        _print(`Smart pointer resource '${name}' created`);
    }

~

    SmartPointerResource() {
        _print(`Smart pointer resource '${this.name}' destroyed`);
        this.ptr = null;
    }

    getValue(): string {
        if (this.ptr) {
            return this.ptr.get();
        } else {
            return "";
        }
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
        _print(`Moveable resource '${name}' created`);
    }

~

    MoveableResource() {
        if (!this.moved) {
            _print(`Moveable resource '${this.name}' destroyed`);
        } else {
            _print(`Moveable resource '${this.name}' was moved, not destroyed`);
        }
    }

    move(): MoveableResource {
        this.moved = true;
        _print(`Moveable resource '${this.name}' moved`);
        return this;
    }

    getName(): string {
        return this.name;
    }
}

// Test functions
function testBasicRAII(): void {
    _print("=== Testing Basic RAII ===");
    let resource = new SimpleResource("test");
    _print(`Resource name: ${resource.getName()}`);
    // Resource should be destroyed when going out of scope
}

function testComplexRAII(): void {
    _print("=== Testing Complex RAII ===");
    let resource = new ComplexResource(100);
    _print(`Resource data length: ${resource.getData().length}`);
    // Resource should be destroyed when going out of scope
}

function testNestedRAII(): void {
    _print("=== Testing Nested RAII ===");
    let resource = new NestedResource("nested");
    _print(`Resource counter: ${resource.getCounter()}`);
    // Resource should be destroyed when going out of scope
}

function testErrorHandling(): void {
    _print("=== Testing Error Handling ===");
    try {
        let resource = new ErrorProneResource("error-test", true);
        // Resource should be destroyed when going out of scope
    } catch (e) {
        _print("Caught error in destructor");
    }
}

function testSharedOwnership(): void {
    _print("=== Testing Shared Ownership ===");
    let resource = new SharedResource("shared");
    resource.addRef();
    _print(`Resource ref count: ${resource.getRefCount()}`);
    // Resource should be destroyed when going out of scope
}

function testCircularDependencies(): void {
    _print("=== Testing Circular Dependencies ===");
    let a = new CircularA("A");
    let b = new CircularB("B");
    a.setB(b);
    b.setA(a);
    _print(`CircularA name: ${a.getName()}`);
    _print(`CircularB name: ${b.getName()}`);
    // Resources should be destroyed when going out of scope
}

function testInheritance(): void {
    _print("=== Testing Inheritance ===");
    let resource = new DerivedResource("derived", "extra");
    _print(`Resource name: ${resource.getName()}`);
    _print(`Resource extra: ${resource.getExtra()}`);
    // Resource should be destroyed when going out of scope
}

function testGenericTypes(): void {
    _print("=== Testing Generic Types ===");
    let resource = new GenericResource<string>("generic", "test-value");
    _print(`Resource name: ${resource.getName()}`);
    _print(`Resource data: ${resource.getData()}`);
    // Resource should be destroyed when going out of scope
}

function testSmartPointers(): void {
    _print("=== Testing Smart Pointers ===");
    let resource = new SmartPointerResource("smart", "smart-value");
    _print(`Resource value: ${resource.getValue()}`);
    // Resource should be destroyed when going out of scope
}

function testMoveSemantics(): void {
    _print("=== Testing Move Semantics ===");
    let resource = new MoveableResource("moveable", 50);
    _print(`Resource name: ${resource.getName()}`);
    let moved = resource.move();
    _print(`Moved resource name: ${moved.getName()}`);
    // Resources should be destroyed when going out of scope
}

// Run all tests
function runAllTests(): void {
    _print("🚀 Starting RAII Unit Tests");
    _print("================================");

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

    _print("================================");
    _print("✅ All RAII Unit Tests Completed");
}

// Export for testing - removed export statement for compatibility