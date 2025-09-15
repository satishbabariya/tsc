// RAII Integration Tests - Testing RAII with other language features
// Tests for RAII interaction with inheritance, polymorphism, generics, and other features

// Test 1: RAII with inheritance and virtual destructors
class BaseRAII {
    protected name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log(`BaseRAII '${name}' created`);
    }
    
    ~BaseRAII() {
        console.log(`BaseRAII '${this.name}' destroyed`);
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
        console.log(`DerivedRAII '${name}' with extra '${extra}' created`);
    }
    
    ~DerivedRAII() {
        console.log(`DerivedRAII '${this.name}' with extra '${this.extra}' destroyed`);
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
        console.log(`PolymorphicRAII '${name}' of type '${type}' created`);
    }
    
    ~PolymorphicRAII() {
        console.log(`PolymorphicRAII '${this.name}' of type '${this.type}' destroyed`);
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
        console.log(`GenericRAII '${name}' created with data: ${data}`);
    }
    
    ~GenericRAII() {
        console.log(`GenericRAII '${this.name}' destroyed`);
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
        console.log(`InterfaceRAII '${name}' created`);
    }
    
    ~InterfaceRAII() {
        this.cleanup();
        console.log(`InterfaceRAII '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
    
    cleanup(): void {
        if (!this.cleaned) {
            console.log(`Cleaning up InterfaceRAII '${this.name}'`);
            this.cleaned = true;
        }
    }
}

// Test 5: RAII with abstract classes
abstract class AbstractRAII {
    protected name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log(`AbstractRAII '${name}' created`);
    }
    
    ~AbstractRAII() {
        console.log(`AbstractRAII '${this.name}' destroyed`);
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
        console.log(`ConcreteRAII '${name}' of type '${type}' created`);
    }
    
    ~ConcreteRAII() {
        console.log(`ConcreteRAII '${this.name}' of type '${this.type}' destroyed`);
    }
    
    getType(): string {
        return this.type;
    }
}

// Test 6: RAII with static members
class StaticRAII {
    private name: string;
    private static instanceCount: number = 0;
    
    constructor(name: string) {
        this.name = name;
        StaticRAII.instanceCount++;
        console.log(`StaticRAII '${name}' created (total instances: ${StaticRAII.instanceCount})`);
    }
    
    ~StaticRAII() {
        StaticRAII.instanceCount--;
        console.log(`StaticRAII '${this.name}' destroyed (remaining instances: ${StaticRAII.instanceCount})`);
    }
    
    getName(): string {
        return this.name;
    }
    
    static getInstanceCount(): number {
        return StaticRAII.instanceCount;
    }
}

// Test 7: RAII with nested classes
class OuterRAII {
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log(`OuterRAII '${name}' created`);
    }
    
    ~OuterRAII() {
        console.log(`OuterRAII '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
    
    class InnerRAII {
        private innerName: string;
        private outer: OuterRAII;
        
        constructor(outer: OuterRAII, innerName: string) {
            this.outer = outer;
            this.innerName = innerName;
            console.log(`InnerRAII '${innerName}' created in '${outer.getName()}'`);
        }
        
        ~InnerRAII() {
            console.log(`InnerRAII '${this.innerName}' destroyed`);
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
        console.log(`ChainedRAII '${name}' created`);
    }
    
    ~ChainedRAII() {
        console.log(`ChainedRAII '${this.name}' destroyed`);
        console.log(`Chain: ${this.chain.join(" -> ")}`);
    }
    
    addToChain(step: string): ChainedRAII {
        this.chain.push(step);
        console.log(`Added '${step}' to chain`);
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
        console.log(`ExceptionRAII '${name}' created`);
    }
    
    ~ExceptionRAII() {
        console.log(`ExceptionRAII '${this.name}' destroyed`);
        if (this.shouldThrow) {
            console.log("ERROR: Destructor throwing exception!");
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
        console.log(`AsyncRAII '${name}' created`);
    }
    
    ~AsyncRAII() {
        console.log(`AsyncRAII '${this.name}' destroyed`);
        if (!this.completed) {
            console.log("WARNING: Async operation not completed");
        }
    }
    
    getName(): string {
        return this.name;
    }
    
    complete(): void {
        this.completed = true;
        console.log(`AsyncRAII '${this.name}' completed`);
    }
}

// Integration test functions
function testInheritanceIntegration(): void {
    console.log("=== Testing RAII with Inheritance ===");
    let base = new BaseRAII("base");
    let derived = new DerivedRAII("derived", "extra");
    let polymorphic = new PolymorphicRAII("poly", "type1");
    
    console.log(`Base name: ${base.getName()}`);
    console.log(`Derived name: ${derived.getName()}, extra: ${derived.getExtra()}`);
    console.log(`Polymorphic name: ${polymorphic.getName()}, type: ${polymorphic.getType()}`);
}

function testGenericIntegration(): void {
    console.log("=== Testing RAII with Generics ===");
    let stringResource = new GenericRAII<string>("string-resource", "hello");
    let numberResource = new GenericRAII<number>("number-resource", 42);
    
    console.log(`String resource: ${stringResource.getName()} = ${stringResource.getData()}`);
    console.log(`Number resource: ${numberResource.getName()} = ${numberResource.getData()}`);
}

function testInterfaceIntegration(): void {
    console.log("=== Testing RAII with Interfaces ===");
    let resource: ResourceInterface = new InterfaceRAII("interface-resource");
    console.log(`Interface resource name: ${resource.getName()}`);
}

function testAbstractIntegration(): void {
    console.log("=== Testing RAII with Abstract Classes ===");
    let concrete = new ConcreteRAII("concrete", "type2");
    console.log(`Concrete resource: ${concrete.getName()}, type: ${concrete.getType()}`);
}

function testStaticIntegration(): void {
    console.log("=== Testing RAII with Static Members ===");
    let resource1 = new StaticRAII("static1");
    let resource2 = new StaticRAII("static2");
    console.log(`Instance count: ${StaticRAII.getInstanceCount()}`);
    console.log(`Resource1 name: ${resource1.getName()}`);
    console.log(`Resource2 name: ${resource2.getName()}`);
}

function testNestedClassIntegration(): void {
    console.log("=== Testing RAII with Nested Classes ===");
    let outer = new OuterRAII("outer");
    let inner = new outer.InnerRAII(outer, "inner");
    console.log(`Outer name: ${outer.getName()}`);
    console.log(`Inner name: ${inner.getInnerName()}, outer: ${inner.getOuterName()}`);
}

function testMethodChainingIntegration(): void {
    console.log("=== Testing RAII with Method Chaining ===");
    let resource = new ChainedRAII("chained");
    resource.addToChain("step1").addToChain("step2").addToChain("step3");
    console.log(`Chained resource name: ${resource.getName()}`);
}

function testExceptionIntegration(): void {
    console.log("=== Testing RAII with Exception Handling ===");
    try {
        let resource = new ExceptionRAII("exception-resource", true);
        resource.throwInConstructor();
    } catch (e) {
        console.log("Caught exception in constructor");
    }
    
    try {
        let resource = new ExceptionRAII("exception-destructor", false);
        // Resource should be destroyed when going out of scope
    } catch (e) {
        console.log("Caught exception in destructor");
    }
}

function testAsyncIntegration(): void {
    console.log("=== Testing RAII with Async Operations ===");
    let resource = new AsyncRAII("async-resource");
    resource.complete();
    console.log(`Async resource name: ${resource.getName()}`);
}

// Run all integration tests
function runAllIntegrationTests(): void {
    console.log("🚀 Starting RAII Integration Tests");
    console.log("=====================================");
    
    testInheritanceIntegration();
    testGenericIntegration();
    testInterfaceIntegration();
    testAbstractIntegration();
    testStaticIntegration();
    testNestedClassIntegration();
    testMethodChainingIntegration();
    testExceptionIntegration();
    testAsyncIntegration();
    
    console.log("=====================================");
    console.log("✅ All RAII Integration Tests Completed");
}

// Export for testing
export { runAllIntegrationTests };