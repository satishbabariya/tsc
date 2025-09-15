// ARC Memory Management Test Suite
// This file tests the ARC memory management features of TSC

// Test 1: Basic Smart Pointer Types
function testSmartPointerTypes() {
    // unique_ptr test
    let uniquePtr: unique_ptr<number> = new unique_ptr<number>(42);
    console.log("unique_ptr created with value:", uniquePtr.get());
    
    // shared_ptr test
    let sharedPtr: shared_ptr<string> = new shared_ptr<string>("Hello ARC");
    console.log("shared_ptr created with value:", sharedPtr.get());
    
    // weak_ptr test
    let weakPtr: weak_ptr<string> = new weak_ptr<string>(sharedPtr);
    console.log("weak_ptr created from shared_ptr");
}

// Test 2: Move Semantics
function testMoveSemantics() {
    let source: unique_ptr<number> = new unique_ptr<number>(100);
    let destination: unique_ptr<number> = std::move(source);
    
    console.log("Move semantics test:");
    console.log("Source after move:", source.get()); // Should be null
    console.log("Destination after move:", destination.get()); // Should be 100
}

// Test 3: ARC Assignment
function testARCAssignment() {
    let ptr1: shared_ptr<string> = new shared_ptr<string>("First");
    let ptr2: shared_ptr<string> = new shared_ptr<string>("Second");
    
    console.log("Before assignment:");
    console.log("ptr1:", ptr1.get());
    console.log("ptr2:", ptr2.get());
    
    ptr1 = ptr2; // ARC should handle reference counting
    
    console.log("After assignment:");
    console.log("ptr1:", ptr1.get());
    console.log("ptr2:", ptr2.get());
}

// Test 4: Memory Management Annotations
@manual_memory
class ManualMemoryClass {
    private data: number[];
    
    constructor(size: number) {
        this.data = new Array<number>(size);
    }
    
    getData(): number[] {
        return this.data;
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

// Test 5: Cycle Detection
class Node {
    private value: number;
    private next: shared_ptr<Node> | null = null;
    
    constructor(value: number) {
        this.value = value;
    }
    
    setNext(node: shared_ptr<Node>): void {
        this.next = node;
    }
    
    getNext(): shared_ptr<Node> | null {
        return this.next;
    }
    
    getValue(): number {
        return this.value;
    }
}

function testCycleDetection() {
    let node1: shared_ptr<Node> = new shared_ptr<Node>(1);
    let node2: shared_ptr<Node> = new shared_ptr<Node>(2);
    let node3: shared_ptr<Node> = new shared_ptr<Node>(3);
    
    // Create a cycle
    node1.setNext(node2);
    node2.setNext(node3);
    node3.setNext(node1);
    
    console.log("Cycle created between nodes");
    // ARC should detect and break the cycle
}

// Test 6: Weak References
function testWeakReferences() {
    let strongRef: shared_ptr<string> = new shared_ptr<string>("Strong reference");
    let weakRef: weak_ptr<string> = new weak_ptr<string>(strongRef);
    
    console.log("Strong reference:", strongRef.get());
    console.log("Weak reference:", weakRef.get());
    
    // Clear strong reference
    strongRef = null;
    
    console.log("After clearing strong reference:");
    console.log("Weak reference:", weakRef.get()); // Should be null
}

// Test 7: Function Parameters and Return Values
function testFunctionParameters(ptr: shared_ptr<number>): shared_ptr<string> {
    let value = ptr.get();
    let result = new shared_ptr<string>(`Value: ${value}`);
    return result;
}

function testFunctionCalls() {
    let input: shared_ptr<number> = new shared_ptr<number>(42);
    let output: shared_ptr<string> = testFunctionParameters(input);
    
    console.log("Function call test:");
    console.log("Input:", input.get());
    console.log("Output:", output.get());
}

// Test 8: Array of Smart Pointers
function testArrayOfSmartPointers() {
    let numbers: shared_ptr<number>[] = [];
    
    for (let i = 0; i < 5; i++) {
        numbers.push(new shared_ptr<number>(i * 10));
    }
    
    console.log("Array of smart pointers:");
    for (let i = 0; i < numbers.length; i++) {
        console.log(`numbers[${i}]:`, numbers[i].get());
    }
}

// Test 9: Class with Smart Pointer Members
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
}

function testSmartPointerContainer() {
    let container = new SmartPointerContainer();
    
    console.log("Smart pointer container test:");
    console.log("Unique data:", container.getUniqueData());
    console.log("Shared data:", container.getSharedData());
    console.log("Weak data:", container.getWeakData());
}

// Test 10: Memory Leak Detection
function testMemoryLeakDetection() {
    console.log("Testing memory leak detection...");
    
    // Create many objects
    for (let i = 0; i < 1000; i++) {
        let obj = new shared_ptr<number>(i);
        // Objects should be automatically cleaned up
    }
    
    console.log("Memory leak test completed");
}

// Main test runner
function runARCTests() {
    console.log("=== ARC Memory Management Test Suite ===");
    
    try {
        testSmartPointerTypes();
        testMoveSemantics();
        testARCAssignment();
        testCycleDetection();
        testWeakReferences();
        testFunctionCalls();
        testArrayOfSmartPointers();
        testSmartPointerContainer();
        testMemoryLeakDetection();
        
        console.log("=== All ARC tests completed successfully ===");
    } catch (error) {
        console.error("ARC test failed:", error);
    }
}

// Run tests
runARCTests();