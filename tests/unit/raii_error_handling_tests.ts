// RAII Error Handling Tests - Testing error conditions and edge cases
// Tests designed to trigger potential errors during destruction or resource release

// Error Test 1: Destructor throwing exceptions
class ExceptionDestructor {
    private name: string;
    private shouldThrow: boolean;

    constructor(name: string, shouldThrow: boolean = false) {
        this.name = name;
        this.shouldThrow = shouldThrow;
        _print(`ExceptionDestructor '${name}' created`);
    }

~

    ExceptionDestructor() {
        _print(`ExceptionDestructor '${this.name}' being destroyed`);
        if (this.shouldThrow) {
            _print("ERROR: Destructor throwing exception!");
            throw new Error(`Destructor exception for ${this.name}`);
        }
        _print(`ExceptionDestructor '${this.name}' destroyed successfully`);
    }

    getName(): string {
        return this.name;
    }
}

// Error Test 2: Circular dependencies with RAII
class CircularRAIIA {
    private name: string;
    private b: CircularRAIIB | null;

    constructor(name: string) {
        this.name = name;
        this.b = null;
        _print(`CircularRAIIA '${name}' created`);
    }

~

    CircularRAIIA() {
        _print(`CircularRAIIA '${this.name}' being destroyed`);
        this.b = null;
        _print(`CircularRAIIA '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    setB(b: CircularRAIIB): void {
        this.b = b;
    }
}

class CircularRAIIB {
    private name: string;
    private a: CircularRAIIA | null;

    constructor(name: string) {
        this.name = name;
        this.a = null;
        _print(`CircularRAIIB '${name}' created`);
    }

~

    CircularRAIIB() {
        _print(`CircularRAIIB '${this.name}' being destroyed`);
        this.a = null;
        _print(`CircularRAIIB '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    setA(a: CircularRAIIA): void {
        this.a = a;
    }
}

// Error Test 3: Double destruction attempt
class DoubleDestruction {
    private name: string;
    private destroyed: boolean;

    constructor(name: string) {
        this.name = name;
        this.destroyed = false;
        _print(`DoubleDestruction '${name}' created`);
    }

~

    DoubleDestruction() {
        if (this.destroyed) {
            _print("ERROR: Attempting to destroy already destroyed object!");
            throw new Error("Double destruction attempt");
        }
        this.destroyed = true;
        _print(`DoubleDestruction '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    isDestroyed(): boolean {
        return this.destroyed;
    }
}

// Error Test 4: Resource leak detection
class ResourceLeak {
    private name: string;
    private resources: string[];
    private cleaned: boolean;

    constructor(name: string) {
        this.name = name;
        this.resources = ["resource1", "resource2", "resource3"];
        this.cleaned = false;
        _print(`ResourceLeak '${name}' created with ${this.resources.length} resources`);
    }

~

    ResourceLeak() {
        _print(`ResourceLeak '${this.name}' being destroyed`);
        if (!this.cleaned) {
            _print("WARNING: Resources not properly cleaned up!");
            _print(`Leaked resources: ${this.resources.join(", ")}`);
        }
        this.cleaned = true;
        _print(`ResourceLeak '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    getResources(): string[] {
        return this.resources;
    }

    cleanup(): void {
        if (!this.cleaned) {
            _print(`Cleaning up resources for '${this.name}'`);
            this.resources = [];
            this.cleaned = true;
        }
    }
}

// Error Test 5: Invalid resource access after destruction
class InvalidAccess {
    private name: string;
    private data: string;
    private destroyed: boolean;

    constructor(name: string, data: string) {
        this.name = name;
        this.data = data;
        this.destroyed = false;
        _print(`InvalidAccess '${name}' created`);
    }

~

    InvalidAccess() {
        _print(`InvalidAccess '${this.name}' being destroyed`);
        this.destroyed = true;
        _print(`InvalidAccess '${this.name}' destroyed`);
    }

    getName(): string {
        if (this.destroyed) {
            _print("ERROR: Accessing destroyed object!");
            throw new Error("Access to destroyed object");
        }
        return this.name;
    }

    getData(): string {
        if (this.destroyed) {
            _print("ERROR: Accessing destroyed object!");
            throw new Error("Access to destroyed object");
        }
        return this.data;
    }

    isDestroyed(): boolean {
        return this.destroyed;
    }
}

// Error Test 6: Memory corruption simulation
class MemoryCorruption {
    private name: string;
    private buffer: number[];
    private corrupted: boolean;

    constructor(name: string, size: number) {
        this.name = name;
        this.buffer = new Array(size);
        this.corrupted = false;
        _print(`MemoryCorruption '${name}' created with buffer size ${size}`);
    }

~

    MemoryCorruption() {
        _print(`MemoryCorruption '${this.name}' being destroyed`);
        if (this.corrupted) {
            _print("ERROR: Memory corruption detected!");
            throw new Error("Memory corruption detected");
        }
        _print(`MemoryCorruption '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    getBuffer(): number[] {
        return this.buffer;
    }

    corruptMemory(): void {
        this.corrupted = true;
        _print("WARNING: Memory corruption simulated!");
    }

    isCorrupted(): boolean {
        return this.corrupted;
    }
}

// Error Test 7: Resource exhaustion
class ResourceExhaustion {
    private name: string;
    private resources: string[];
    private maxResources: number;

    constructor(name: string, maxResources: number) {
        this.name = name;
        this.maxResources = maxResources;
        this.resources = [];
        _print(`ResourceExhaustion '${name}' created with max ${maxResources} resources`);
    }

~

    ResourceExhaustion() {
        _print(`ResourceExhaustion '${this.name}' being destroyed`);
        _print(`Final resource count: ${this.resources.length}/${this.maxResources}`);
        if (this.resources.length > this.maxResources) {
            _print("ERROR: Resource exhaustion detected!");
            throw new Error("Resource exhaustion");
        }
        _print(`ResourceExhaustion '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    getResourceCount(): number {
        return this.resources.length;
    }

    getMaxResources(): number {
        return this.maxResources;
    }

    addResource(resource: string): boolean {
        if (this.resources.length >= this.maxResources) {
            _print("ERROR: Cannot add resource, limit exceeded!");
            return false;
        }
        this.resources.push(resource);
        _print(`Resource '${resource}' added (${this.resources.length}/${this.maxResources})`);
        return true;
    }

    removeResource(resource: string): boolean {
        let index = this.resources.indexOf(resource);
        if (index > -1) {
            this.resources.splice(index, 1);
            _print(`Resource '${resource}' removed (${this.resources.length}/${this.maxResources})`);
            return true;
        }
        return false;
    }
}

// Error Test 8: Thread safety issues
class ThreadSafety {
    private name: string;
    private counter: number;
    private destroyed: boolean;

    constructor(name: string) {
        this.name = name;
        this.counter = 0;
        this.destroyed = false;
        _print(`ThreadSafety '${name}' created`);
    }

~

    ThreadSafety() {
        _print(`ThreadSafety '${this.name}' being destroyed`);
        this.destroyed = true;
        _print(`ThreadSafety '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    getCounter(): number {
        if (this.destroyed) {
            _print("ERROR: Accessing destroyed object from thread!");
            throw new Error("Thread safety violation");
        }
        return this.counter;
    }

    incrementCounter(): void {
        if (this.destroyed) {
            _print("ERROR: Modifying destroyed object from thread!");
            throw new Error("Thread safety violation");
        }
        this.counter++;
        _print(`Counter incremented to ${this.counter}`);
    }

    isDestroyed(): boolean {
        return this.destroyed;
    }
}

// Error Test 9: Invalid state transitions
class InvalidState {
    private name: string;
    private state: string;
    private validStates: string[];

    constructor(name: string) {
        this.name = name;
        this.state = "initialized";
        this.validStates = ["initialized", "active", "inactive", "destroyed"];
        _print(`InvalidState '${name}' created in state '${this.state}'`);
    }

~

    InvalidState() {
        _print(`InvalidState '${this.name}' being destroyed`);
        if (this.state === "destroyed") {
            _print("ERROR: Invalid state transition to destroyed!");
            throw new Error("Invalid state transition");
        }
        this.state = "destroyed";
        _print(`InvalidState '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    getState(): string {
        return this.state;
    }

    setState(newState: string): boolean {
        if (this.state === "destroyed") {
            _print("ERROR: Cannot change state of destroyed object!");
            return false;
        }
        if (this.validStates.indexOf(newState) === -1) {
            _print(`ERROR: Invalid state '${newState}'!`);
            return false;
        }
        this.state = newState;
        _print(`State changed to '${newState}'`);
        return true;
    }
}

// Error Test 10: Resource dependency chain failure
class ResourceDependency {
    private name: string;
    private dependencies: string[];
    private satisfied: boolean;

    constructor(name: string, dependencies: string[]) {
        this.name = name;
        this.dependencies = dependencies;
        this.satisfied = false;
        _print(`ResourceDependency '${name}' created with dependencies: ${dependencies.join(", ")}`);
    }

~

    ResourceDependency() {
        _print(`ResourceDependency '${this.name}' being destroyed`);
        if (!this.satisfied) {
            _print("ERROR: Resource dependencies not satisfied!");
            _print(`Unsatisfied dependencies: ${this.dependencies.join(", ")}`);
            throw new Error("Resource dependency failure");
        }
        _print(`ResourceDependency '${this.name}' destroyed`);
    }

    getName(): string {
        return this.name;
    }

    getDependencies(): string[] {
        return this.dependencies;
    }

    isSatisfied(): boolean {
        return this.satisfied;
    }

    satisfyDependency(dependency: string): boolean {
        let index = this.dependencies.indexOf(dependency);
        if (index > -1) {
            this.dependencies.splice(index, 1);
            _print(`Dependency '${dependency}' satisfied`);
            if (this.dependencies.length === 0) {
                this.satisfied = true;
                _print("All dependencies satisfied");
            }
            return true;
        }
        return false;
    }
}

// Error handling test functions
function testExceptionDestructor(): void {
    _print("=== Testing Exception Destructor ===");
    try {
        let resource = new ExceptionDestructor("exception-test", true);
        _print(`Resource name: ${resource.getName()}`);
    } catch (e) {
        _print("Caught exception in destructor");
    }
}

function testCircularDependencies(): void {
    _print("=== Testing Circular Dependencies ===");
    let a = new CircularRAIIA("A");
    let b = new CircularRAIIB("B");
    a.setB(b);
    b.setA(a);
    _print(`CircularA: ${a.getName()}`);
    _print(`CircularB: ${b.getName()}`);
}

function testDoubleDestruction(): void {
    _print("=== Testing Double Destruction ===");
    try {
        let resource = new DoubleDestruction("double-test");
        _print(`Resource name: ${resource.getName()}`);
        // Attempt to destroy again (should fail)
        resource = null;
    } catch (e) {
        _print("Caught double destruction error");
    }
}

function testResourceLeak(): void {
    _print("=== Testing Resource Leak Detection ===");
    let resource = new ResourceLeak("leak-test");
    _print(`Resource name: ${resource.getName()}`);
    _print(`Resources: ${resource.getResources().join(", ")}`);
    // Don't call cleanup() to trigger leak detection
}

function testInvalidAccess(): void {
    _print("=== Testing Invalid Access ===");
    try {
        let resource = new InvalidAccess("access-test", "test-data");
        _print(`Resource name: ${resource.getName()}`);
        _print(`Resource data: ${resource.getData()}`);
    } catch (e) {
        _print("Caught invalid access error");
    }
}

function testMemoryCorruption(): void {
    _print("=== Testing Memory Corruption ===");
    try {
        let resource = new MemoryCorruption("corruption-test", 100);
        _print(`Resource name: ${resource.getName()}`);
        resource.corruptMemory();
        _print(`Corrupted: ${resource.isCorrupted()}`);
    } catch (e) {
        _print("Caught memory corruption error");
    }
}

function testResourceExhaustion(): void {
    _print("=== Testing Resource Exhaustion ===");
    try {
        let resource = new ResourceExhaustion("exhaustion-test", 3);
        _print(`Resource name: ${resource.getName()}`);
        resource.addResource("res1");
        resource.addResource("res2");
        resource.addResource("res3");
        resource.addResource("res4"); // Should fail
    } catch (e) {
        _print("Caught resource exhaustion error");
    }
}

function testThreadSafety(): void {
    _print("=== Testing Thread Safety ===");
    try {
        let resource = new ThreadSafety("thread-test");
        _print(`Resource name: ${resource.getName()}`);
        resource.incrementCounter();
        _print(`Counter: ${resource.getCounter()}`);
    } catch (e) {
        _print("Caught thread safety error");
    }
}

function testInvalidState(): void {
    _print("=== Testing Invalid State ===");
    try {
        let resource = new InvalidState("state-test");
        _print(`Resource name: ${resource.getName()}`);
        resource.setState("active");
        resource.setState("invalid-state"); // Should fail
    } catch (e) {
        _print("Caught invalid state error");
    }
}

function testResourceDependency(): void {
    _print("=== Testing Resource Dependency ===");
    try {
        let resource = new ResourceDependency("dependency-test", ["dep1", "dep2"]);
        _print(`Resource name: ${resource.getName()}`);
        resource.satisfyDependency("dep1");
        // Don't satisfy dep2 to trigger dependency failure
    } catch (e) {
        _print("Caught resource dependency error");
    }
}

// Run all error handling tests
function runAllErrorHandlingTests(): void {
    _print("🚀 Starting RAII Error Handling Tests");
    _print("=======================================");

    testExceptionDestructor();
    testCircularDependencies();
    testDoubleDestruction();
    testResourceLeak();
    testInvalidAccess();
    testMemoryCorruption();
    testResourceExhaustion();
    testThreadSafety();
    testInvalidState();
    testResourceDependency();

    _print("=======================================");
    _print("✅ All RAII Error Handling Tests Completed");
}

// Export for testing
export {runAllErrorHandlingTests};