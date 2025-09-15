// RAII Error Handling Tests - Testing error conditions and edge cases
// Tests designed to trigger potential errors during destruction or resource release

// Error Test 1: Destructor throwing exceptions
class ExceptionDestructor {
    private name: string;
    private shouldThrow: boolean;
    
    constructor(name: string, shouldThrow: boolean = false) {
        this.name = name;
        this.shouldThrow = shouldThrow;
        console.log(`ExceptionDestructor '${name}' created`);
    }
    
    ~ExceptionDestructor() {
        console.log(`ExceptionDestructor '${this.name}' being destroyed`);
        if (this.shouldThrow) {
            console.log("ERROR: Destructor throwing exception!");
            throw new Error(`Destructor exception for ${this.name}`);
        }
        console.log(`ExceptionDestructor '${this.name}' destroyed successfully`);
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
        console.log(`CircularRAIIA '${name}' created`);
    }
    
    ~CircularRAIIA() {
        console.log(`CircularRAIIA '${this.name}' being destroyed`);
        this.b = null;
        console.log(`CircularRAIIA '${this.name}' destroyed`);
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
        console.log(`CircularRAIIB '${name}' created`);
    }
    
    ~CircularRAIIB() {
        console.log(`CircularRAIIB '${this.name}' being destroyed`);
        this.a = null;
        console.log(`CircularRAIIB '${this.name}' destroyed`);
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
        console.log(`DoubleDestruction '${name}' created`);
    }
    
    ~DoubleDestruction() {
        if (this.destroyed) {
            console.log("ERROR: Attempting to destroy already destroyed object!");
            throw new Error("Double destruction attempt");
        }
        this.destroyed = true;
        console.log(`DoubleDestruction '${this.name}' destroyed`);
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
        console.log(`ResourceLeak '${name}' created with ${this.resources.length} resources`);
    }
    
    ~ResourceLeak() {
        console.log(`ResourceLeak '${this.name}' being destroyed`);
        if (!this.cleaned) {
            console.log("WARNING: Resources not properly cleaned up!");
            console.log(`Leaked resources: ${this.resources.join(", ")}`);
        }
        this.cleaned = true;
        console.log(`ResourceLeak '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
    
    getResources(): string[] {
        return this.resources;
    }
    
    cleanup(): void {
        if (!this.cleaned) {
            console.log(`Cleaning up resources for '${this.name}'`);
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
        console.log(`InvalidAccess '${name}' created`);
    }
    
    ~InvalidAccess() {
        console.log(`InvalidAccess '${this.name}' being destroyed`);
        this.destroyed = true;
        console.log(`InvalidAccess '${this.name}' destroyed`);
    }
    
    getName(): string {
        if (this.destroyed) {
            console.log("ERROR: Accessing destroyed object!");
            throw new Error("Access to destroyed object");
        }
        return this.name;
    }
    
    getData(): string {
        if (this.destroyed) {
            console.log("ERROR: Accessing destroyed object!");
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
        console.log(`MemoryCorruption '${name}' created with buffer size ${size}`);
    }
    
    ~MemoryCorruption() {
        console.log(`MemoryCorruption '${this.name}' being destroyed`);
        if (this.corrupted) {
            console.log("ERROR: Memory corruption detected!");
            throw new Error("Memory corruption detected");
        }
        console.log(`MemoryCorruption '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
    
    getBuffer(): number[] {
        return this.buffer;
    }
    
    corruptMemory(): void {
        this.corrupted = true;
        console.log("WARNING: Memory corruption simulated!");
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
        console.log(`ResourceExhaustion '${name}' created with max ${maxResources} resources`);
    }
    
    ~ResourceExhaustion() {
        console.log(`ResourceExhaustion '${this.name}' being destroyed`);
        console.log(`Final resource count: ${this.resources.length}/${this.maxResources}`);
        if (this.resources.length > this.maxResources) {
            console.log("ERROR: Resource exhaustion detected!");
            throw new Error("Resource exhaustion");
        }
        console.log(`ResourceExhaustion '${this.name}' destroyed`);
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
            console.log("ERROR: Cannot add resource, limit exceeded!");
            return false;
        }
        this.resources.push(resource);
        console.log(`Resource '${resource}' added (${this.resources.length}/${this.maxResources})`);
        return true;
    }
    
    removeResource(resource: string): boolean {
        let index = this.resources.indexOf(resource);
        if (index > -1) {
            this.resources.splice(index, 1);
            console.log(`Resource '${resource}' removed (${this.resources.length}/${this.maxResources})`);
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
        console.log(`ThreadSafety '${name}' created`);
    }
    
    ~ThreadSafety() {
        console.log(`ThreadSafety '${this.name}' being destroyed`);
        this.destroyed = true;
        console.log(`ThreadSafety '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
    
    getCounter(): number {
        if (this.destroyed) {
            console.log("ERROR: Accessing destroyed object from thread!");
            throw new Error("Thread safety violation");
        }
        return this.counter;
    }
    
    incrementCounter(): void {
        if (this.destroyed) {
            console.log("ERROR: Modifying destroyed object from thread!");
            throw new Error("Thread safety violation");
        }
        this.counter++;
        console.log(`Counter incremented to ${this.counter}`);
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
        console.log(`InvalidState '${name}' created in state '${this.state}'`);
    }
    
    ~InvalidState() {
        console.log(`InvalidState '${this.name}' being destroyed`);
        if (this.state === "destroyed") {
            console.log("ERROR: Invalid state transition to destroyed!");
            throw new Error("Invalid state transition");
        }
        this.state = "destroyed";
        console.log(`InvalidState '${this.name}' destroyed`);
    }
    
    getName(): string {
        return this.name;
    }
    
    getState(): string {
        return this.state;
    }
    
    setState(newState: string): boolean {
        if (this.state === "destroyed") {
            console.log("ERROR: Cannot change state of destroyed object!");
            return false;
        }
        if (this.validStates.indexOf(newState) === -1) {
            console.log(`ERROR: Invalid state '${newState}'!`);
            return false;
        }
        this.state = newState;
        console.log(`State changed to '${newState}'`);
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
        console.log(`ResourceDependency '${name}' created with dependencies: ${dependencies.join(", ")}`);
    }
    
    ~ResourceDependency() {
        console.log(`ResourceDependency '${this.name}' being destroyed`);
        if (!this.satisfied) {
            console.log("ERROR: Resource dependencies not satisfied!");
            console.log(`Unsatisfied dependencies: ${this.dependencies.join(", ")}`);
            throw new Error("Resource dependency failure");
        }
        console.log(`ResourceDependency '${this.name}' destroyed`);
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
            console.log(`Dependency '${dependency}' satisfied`);
            if (this.dependencies.length === 0) {
                this.satisfied = true;
                console.log("All dependencies satisfied");
            }
            return true;
        }
        return false;
    }
}

// Error handling test functions
function testExceptionDestructor(): void {
    console.log("=== Testing Exception Destructor ===");
    try {
        let resource = new ExceptionDestructor("exception-test", true);
        console.log(`Resource name: ${resource.getName()}`);
    } catch (e) {
        console.log("Caught exception in destructor");
    }
}

function testCircularDependencies(): void {
    console.log("=== Testing Circular Dependencies ===");
    let a = new CircularRAIIA("A");
    let b = new CircularRAIIB("B");
    a.setB(b);
    b.setA(a);
    console.log(`CircularA: ${a.getName()}`);
    console.log(`CircularB: ${b.getName()}`);
}

function testDoubleDestruction(): void {
    console.log("=== Testing Double Destruction ===");
    try {
        let resource = new DoubleDestruction("double-test");
        console.log(`Resource name: ${resource.getName()}`);
        // Attempt to destroy again (should fail)
        resource = null;
    } catch (e) {
        console.log("Caught double destruction error");
    }
}

function testResourceLeak(): void {
    console.log("=== Testing Resource Leak Detection ===");
    let resource = new ResourceLeak("leak-test");
    console.log(`Resource name: ${resource.getName()}`);
    console.log(`Resources: ${resource.getResources().join(", ")}`);
    // Don't call cleanup() to trigger leak detection
}

function testInvalidAccess(): void {
    console.log("=== Testing Invalid Access ===");
    try {
        let resource = new InvalidAccess("access-test", "test-data");
        console.log(`Resource name: ${resource.getName()}`);
        console.log(`Resource data: ${resource.getData()}`);
    } catch (e) {
        console.log("Caught invalid access error");
    }
}

function testMemoryCorruption(): void {
    console.log("=== Testing Memory Corruption ===");
    try {
        let resource = new MemoryCorruption("corruption-test", 100);
        console.log(`Resource name: ${resource.getName()}`);
        resource.corruptMemory();
        console.log(`Corrupted: ${resource.isCorrupted()}`);
    } catch (e) {
        console.log("Caught memory corruption error");
    }
}

function testResourceExhaustion(): void {
    console.log("=== Testing Resource Exhaustion ===");
    try {
        let resource = new ResourceExhaustion("exhaustion-test", 3);
        console.log(`Resource name: ${resource.getName()}`);
        resource.addResource("res1");
        resource.addResource("res2");
        resource.addResource("res3");
        resource.addResource("res4"); // Should fail
    } catch (e) {
        console.log("Caught resource exhaustion error");
    }
}

function testThreadSafety(): void {
    console.log("=== Testing Thread Safety ===");
    try {
        let resource = new ThreadSafety("thread-test");
        console.log(`Resource name: ${resource.getName()}`);
        resource.incrementCounter();
        console.log(`Counter: ${resource.getCounter()}`);
    } catch (e) {
        console.log("Caught thread safety error");
    }
}

function testInvalidState(): void {
    console.log("=== Testing Invalid State ===");
    try {
        let resource = new InvalidState("state-test");
        console.log(`Resource name: ${resource.getName()}`);
        resource.setState("active");
        resource.setState("invalid-state"); // Should fail
    } catch (e) {
        console.log("Caught invalid state error");
    }
}

function testResourceDependency(): void {
    console.log("=== Testing Resource Dependency ===");
    try {
        let resource = new ResourceDependency("dependency-test", ["dep1", "dep2"]);
        console.log(`Resource name: ${resource.getName()}`);
        resource.satisfyDependency("dep1");
        // Don't satisfy dep2 to trigger dependency failure
    } catch (e) {
        console.log("Caught resource dependency error");
    }
}

// Run all error handling tests
function runAllErrorHandlingTests(): void {
    console.log("🚀 Starting RAII Error Handling Tests");
    console.log("=======================================");
    
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
    
    console.log("=======================================");
    console.log("✅ All RAII Error Handling Tests Completed");
}

// Export for testing
export { runAllErrorHandlingTests };