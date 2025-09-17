// RAII Integration Tests - Testing RAII with other language features
// Tests for RAII interaction with inheritance, polymorphism, generics, and other features

// Test 1: RAII with inheritance and virtual destructors
class BaseRAII {
    protected name: string;

    constructor(name: string) {
        this.name = name;
        _print(`BaseRAII '${name}' created`);
    }

~

    BaseRAII() {
        _print(`BaseRAII '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }
}

class DerivedRAII extends BaseRAII {
    private extra: string;

    constructor(name: string, extra: string) {
        super(name);
        this.extra = extra;
        _print(`DerivedRAII '${name}' with extra '${extra}' created`);
    }

~

    DerivedRAII() {
        _print(`DerivedRAII '${this.name}' with extra '${this.extra}' destroyed`);
    }

    getExtra(): string {
        return this.extra;
    }
}

// Test 2: RAII with polymorphism
class PolymorphicRAII extends BaseRAII {
    private type: string;

    constructor(name: string, type: string) {
        super(name);
        this.type = type;
        _print(`PolymorphicRAII '${name}' of type '${type}' created`);
    }

~

    PolymorphicRAII() {
        _print(`PolymorphicRAII '${this.name}' of type '${this.type}' destroyed`);
    }

    getType(): string {
        return this.type;
    }
}

// Test 3: RAII with generic types and constraints
class GenericRAII<T extends string | number> {
    private data: T;
    private name: string;

    constructor(name: string, data: T) {
        this.name = name;
        this.data = data;
        _print(`GenericRAII '${name}' created with data: ${data}`);
    }

~

    GenericRAII() {
        _print(`GenericRAII '${this.name}' destroyed`);
    }

    getData(): T {
        return this.data;
    }

    getName(): string {
        return this.name;
    }
}

// Test 4: RAII with interfaces
interface ResourceInterface {
    getName(): string;

    cleanup(): void;
}

class InterfaceRAII implements ResourceInterface {
    private name: string;
    private cleaned: boolean;

    constructor(name: string) {
        this.name = name;
        this.cleaned = false;
        _print(`InterfaceRAII '${name}' created`);
    }

~

    InterfaceRAII() {
        this.cleanup();
        _print(`InterfaceRAII '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    cleanup(): void {
        if (!this.cleaned) {
            _print(`Cleaning up InterfaceRAII '${this.name}'`);
            this.cleaned = true;
        }
    }
}

// Test 5: RAII with abstract classes
abstract class AbstractRAII {
    protected name: string;

    constructor(name: string) {
        this.name = name;
        _print(`AbstractRAII '${name}' created`);
    }

~

    AbstractRAII() {
        _print(`AbstractRAII '${this.name}' destroyed`);
    }

    abstract getType(): string;

    getName(): string {
        return this.name;
    }
}

class ConcreteRAII extends AbstractRAII {
    private type: string;

    constructor(name: string, type: string) {
        super(name);
        this.type = type;
        _print(`ConcreteRAII '${name}' of type '${type}' created`);
    }

~

    ConcreteRAII() {
        _print(`ConcreteRAII '${this.name}' of type '${this.type}' destroyed`);
    }

    getType(): string {
        return this.type;
    }
}

// Test 6: RAII with static members
class StaticRAII {
    private static instanceCount: number = 0;
    private name: string;

    constructor(name: string) {
        this.name = name;
        StaticRAII.instanceCount++;
        _print(`StaticRAII '${name}' created (total instances: ${StaticRAII.instanceCount})`);
    }

~

    static getInstanceCount(): number {
        return StaticRAII.instanceCount;
    }

    StaticRAII() {
        StaticRAII.instanceCount--;
        _print(`StaticRAII '${this.name}' destroyed (remaining instances: ${StaticRAII.instanceCount})`);
    }

    getName(): string {
        return this.name;
    }
}

// Test 7: RAII with nested classes
class OuterRAII {
    class
InnerRAII

~
    private name: string;
    private innerName: string;
    private outer: OuterRAII;

    constructor(name: string) {
        this.name = name;
        _print(`OuterRAII '${name}' created`);
    } {

    constructor(outer: OuterRAII, innerName: string) {
        this.outer = outer;
        this.innerName = innerName;
        _print(`InnerRAII '${innerName}' created in '${outer.getName()}'`);
    }

    OuterRAII() {
        _print(`OuterRAII '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

~

    InnerRAII() {
        _print(`InnerRAII '${this.innerName}' destroyed`);
    }

    getInnerName(): string {
        return this.innerName;
    }

    getOuterName(): string {
        return this.outer.getName();
    }
}
}

// Test 8: RAII with method chaining
class ChainedRAII {
    private name: string;
    private chain: string[];

    constructor(name: string) {
        this.name = name;
        this.chain = [];
        _print(`ChainedRAII '${name}' created`);
    }

~

    ChainedRAII() {
        _print(`ChainedRAII '${this.name}' destroyed`);
        _print(`Chain: ${this.chain.join(" -> ")}`);
    }

    addToChain(step: string): ChainedRAII {
        this.chain.push(step);
        _print(`Added '${step}' to chain`);
        return this;
    }

    getName(): string {
        return this.name;
    }
}

// Test 9: RAII with exception handling
class ExceptionRAII {
    private name: string;
    private shouldThrow: boolean;

    constructor(name: string, shouldThrow: boolean = false) {
        this.name = name;
        this.shouldThrow = shouldThrow;
        _print(`ExceptionRAII '${name}' created`);
    }

~

    ExceptionRAII() {
        _print(`ExceptionRAII '${this.name}' destroyed`);
        if (this.shouldThrow) {
            _print("ERROR: Destructor throwing exception!");
            throw new Error("Destructor exception");
        }
    }

    getName(): string {
        return this.name;
    }

    throwInConstructor(): void {
        if (this.shouldThrow) {
            throw new Error("Constructor exception");
        }
    }
}

// Test 10: RAII with async operations (simulated)
class AsyncRAII {
    private name: string;
    private completed: boolean;

    constructor(name: string) {
        this.name = name;
        this.completed = false;
        _print(`AsyncRAII '${name}' created`);
    }

~

    AsyncRAII() {
        _print(`AsyncRAII '${this.name}' destroyed`);
        if (!this.completed) {
            _print("WARNING: Async operation not completed");
        }
    }

    getName(): string {
        return this.name;
    }

    complete(): void {
        this.completed = true;
        _print(`AsyncRAII '${this.name}' completed`);
    }
}

// Integration test functions
function testInheritanceIntegration(): void {
    _print("=== Testing RAII with Inheritance ===");
    let base = new BaseRAII("base");
    let derived = new DerivedRAII("derived", "extra");
    let polymorphic = new PolymorphicRAII("poly", "type1");

    _print(`Base name: ${base.getName()}`);
    _print(`Derived name: ${derived.getName()}, extra: ${derived.getExtra()}`);
    _print(`Polymorphic name: ${polymorphic.getName()}, type: ${polymorphic.getType()}`);
}

function testGenericIntegration(): void {
    _print("=== Testing RAII with Generics ===");
    let stringResource = new GenericRAII<string>("string-resource", "hello");
    let numberResource = new GenericRAII<number>("number-resource", 42);

    _print(`String resource: ${stringResource.getName()} = ${stringResource.getData()}`);
    _print(`Number resource: ${numberResource.getName()} = ${numberResource.getData()}`);
}

function testInterfaceIntegration(): void {
    _print("=== Testing RAII with Interfaces ===");
    let resource: ResourceInterface = new InterfaceRAII("interface-resource");
    _print(`Interface resource name: ${resource.getName()}`);
}

function testAbstractIntegration(): void {
    _print("=== Testing RAII with Abstract Classes ===");
    let concrete = new ConcreteRAII("concrete", "type2");
    _print(`Concrete resource: ${concrete.getName()}, type: ${concrete.getType()}`);
}

function testStaticIntegration(): void {
    _print("=== Testing RAII with Static Members ===");
    let resource1 = new StaticRAII("static1");
    let resource2 = new StaticRAII("static2");
    _print(`Instance count: ${StaticRAII.getInstanceCount()}`);
    _print(`Resource1 name: ${resource1.getName()}`);
    _print(`Resource2 name: ${resource2.getName()}`);
}

function testNestedClassIntegration(): void {
    _print("=== Testing RAII with Nested Classes ===");
    let outer = new OuterRAII("outer");
    let inner = new outer.InnerRAII(outer, "inner");
    _print(`Outer name: ${outer.getName()}`);
    _print(`Inner name: ${inner.getInnerName()}, outer: ${inner.getOuterName()}`);
}

function testMethodChainingIntegration(): void {
    _print("=== Testing RAII with Method Chaining ===");
    let resource = new ChainedRAII("chained");
    resource.addToChain("step1").addToChain("step2").addToChain("step3");
    _print(`Chained resource name: ${resource.getName()}`);
}

function testExceptionIntegration(): void {
    _print("=== Testing RAII with Exception Handling ===");
    try {
        let resource = new ExceptionRAII("exception-resource", true);
        resource.throwInConstructor();
    } catch (e) {
        _print("Caught exception in constructor");
    }

    try {
        let resource = new ExceptionRAII("exception-destructor", false);
        // Resource should be destroyed when going out of scope
    } catch (e) {
        _print("Caught exception in destructor");
    }
}

function testAsyncIntegration(): void {
    _print("=== Testing RAII with Async Operations ===");
    let resource = new AsyncRAII("async-resource");
    resource.complete();
    _print(`Async resource name: ${resource.getName()}`);
}

// Run all integration tests
function runAllIntegrationTests(): void {
    _print("🚀 Starting RAII Integration Tests");
    _print("=====================================");

    testInheritanceIntegration();
    testGenericIntegration();
    testInterfaceIntegration();
    testAbstractIntegration();
    testStaticIntegration();
    testNestedClassIntegration();
    testMethodChainingIntegration();
    testExceptionIntegration();
    testAsyncIntegration();

    _print("=====================================");
    _print("✅ All RAII Integration Tests Completed");
}

// Export for testing
export {runAllIntegrationTests};