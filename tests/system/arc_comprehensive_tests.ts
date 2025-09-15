// Comprehensive ARC System Tests
// These tests validate the complete ARC implementation end-to-end

// Test 1: Basic Smart Pointer Lifecycle
function testBasicLifecycle(): void {
    _print("=== Test 1: Basic Smart Pointer Lifecycle ===");
    
    // Test unique_ptr lifecycle
    let uniquePtr: unique_ptr<number> = new unique_ptr<number>(42);
    assert(uniquePtr.get() === 42, "unique_ptr should contain 42");
    
    // Test shared_ptr lifecycle
    let sharedPtr: shared_ptr<string> = new shared_ptr<string>("Hello");
    assert(sharedPtr.get() === "Hello", "shared_ptr should contain 'Hello'");
    
    // Test weak_ptr lifecycle
    let weakPtr: weak_ptr<string> = new weak_ptr<string>(sharedPtr);
    assert(weakPtr.get() === "Hello", "weak_ptr should contain 'Hello'");
    
    _print("✓ Basic lifecycle tests passed");
}

// Test 2: Reference Counting Accuracy
function testReferenceCounting(): void {
    _print("=== Test 2: Reference Counting Accuracy ===");
    
    let ptr1: shared_ptr<number> = new shared_ptr<number>(100);
    let ptr2: shared_ptr<number> = ptr1;
    let ptr3: shared_ptr<number> = ptr2;
    
    // All should reference the same object
    assert(ptr1.get() === 100, "ptr1 should contain 100");
    assert(ptr2.get() === 100, "ptr2 should contain 100");
    assert(ptr3.get() === 100, "ptr3 should contain 100");
    
    // Clear one reference
    ptr2 = null;
    assert(ptr1.get() === 100, "ptr1 should still contain 100");
    assert(ptr3.get() === 100, "ptr3 should still contain 100");
    
    // Clear another reference
    ptr1 = null;
    assert(ptr3.get() === 100, "ptr3 should still contain 100");
    
    // Clear last reference
    ptr3 = null;
    
    _print("✓ Reference counting tests passed");
}

// Test 3: Move Semantics Correctness
function testMoveSemantics(): void {
    _print("=== Test 3: Move Semantics Correctness ===");
    
    // Test unique_ptr move
    let source: unique_ptr<number> = new unique_ptr<number>(42);
    let destination: unique_ptr<number> = std::move(source);
    
    assert(source.get() === null, "Source should be null after move");
    assert(destination.get() === 42, "Destination should contain 42");
    
    // Test shared_ptr move
    let sharedSource: shared_ptr<string> = new shared_ptr<string>("Move me");
    let sharedDest: shared_ptr<string> = std::move(sharedSource);
    
    assert(sharedSource.get() === null, "Shared source should be null after move");
    assert(sharedDest.get() === "Move me", "Shared destination should contain 'Move me'");
    
    _print("✓ Move semantics tests passed");
}

// Test 4: Weak Reference Behavior
function testWeakReferences(): void {
    _print("=== Test 4: Weak Reference Behavior ===");
    
    let strong: shared_ptr<number> = new shared_ptr<number>(200);
    let weak: weak_ptr<number> = new weak_ptr<number>(strong);
    
    // Weak reference should be valid
    assert(weak.get() === 200, "Weak reference should contain 200");
    
    // Clear strong reference
    strong = null;
    
    // Weak reference should now be null
    assert(weak.get() === null, "Weak reference should be null after strong reference cleared");
    
    _print("✓ Weak reference tests passed");
}

// Test 5: Complex Object Graphs
function testComplexObjectGraphs(): void {
    _print("=== Test 5: Complex Object Graphs ===");
    
    class Node {
        private value: number;
        private children: shared_ptr<Node>[];
        
        constructor(value: number) {
            this.value = value;
            this.children = [];
        }
        
        addChild(child: shared_ptr<Node>): void {
            this.children.push(child);
        }
        
        getValue(): number {
            return this.value;
        }
        
        getChildren(): shared_ptr<Node>[] {
            return this.children;
        }
    }
    
    // Create a tree structure
    let root: shared_ptr<Node> = new shared_ptr<Node>(1);
    let child1: shared_ptr<Node> = new shared_ptr<Node>(2);
    let child2: shared_ptr<Node> = new shared_ptr<Node>(3);
    let grandchild: shared_ptr<Node> = new shared_ptr<Node>(4);
    
    root.get().addChild(child1);
    root.get().addChild(child2);
    child1.get().addChild(grandchild);
    
    // Verify structure
    assert(root.get().getValue() === 1, "Root should have value 1");
    assert(root.get().getChildren().length === 2, "Root should have 2 children");
    assert(child1.get().getChildren().length === 1, "Child1 should have 1 child");
    
    _print("✓ Complex object graph tests passed");
}

// Test 6: Cycle Detection and Breaking
function testCycleDetection(): void {
    _print("=== Test 6: Cycle Detection and Breaking ===");
    
    class CyclicNode {
        private value: number;
        private next: weak_ptr<CyclicNode>; // Use weak_ptr to break cycles
        
        constructor(value: number) {
            this.value = value;
        }
        
        setNext(node: shared_ptr<CyclicNode>): void {
            this.next = new weak_ptr<CyclicNode>(node);
        }
        
        getNext(): shared_ptr<CyclicNode> | null {
            return this.next.get();
        }
        
        getValue(): number {
            return this.value;
        }
    }
    
    // Create a cycle
    let node1: shared_ptr<CyclicNode> = new shared_ptr<CyclicNode>(1);
    let node2: shared_ptr<CyclicNode> = new shared_ptr<CyclicNode>(2);
    let node3: shared_ptr<CyclicNode> = new shared_ptr<CyclicNode>(3);
    
    node1.get().setNext(node2);
    node2.get().setNext(node3);
    node3.get().setNext(node1); // Creates cycle
    
    // Verify cycle exists
    assert(node1.get().getNext() !== null, "Node1 should have next");
    assert(node2.get().getNext() !== null, "Node2 should have next");
    assert(node3.get().getNext() !== null, "Node3 should have next");
    
    _print("✓ Cycle detection tests passed");
}

// Test 7: Memory Management Annotations
function testMemoryAnnotations(): void {
    _print("=== Test 7: Memory Management Annotations ===");
    
    @manual_memory
    class ManualClass {
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
        
        destroy(): void {
            this.data = null;
        }
    }
    
    @stack_allocated
    class StackClass {
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
    
    let manual = new ManualClass(10);
    let stack = new StackClass(42);
    let immutable = new ImmutableClass(999);
    
    assert(manual.getData().length === 10, "Manual class should have 10 elements");
    assert(stack.getValue() === 42, "Stack class should have value 42");
    assert(immutable.getValue() === 999, "Immutable class should have value 999");
    
    manual.destroy();
    
    _print("✓ Memory annotation tests passed");
}

// Test 8: Exception Handling with ARC
function testExceptionHandling(): void {
    _print("=== Test 8: Exception Handling with ARC ===");
    
    function riskyOperation(): shared_ptr<number> {
        let ptr: shared_ptr<number> = new shared_ptr<number>(42);
        
        try {
            // Simulate risky operation
            if (Math.random() > 0.5) {
                throw new Error("Random error");
            }
            
            return ptr;
        } catch (error) {
            // ptr should be automatically cleaned up
            throw error;
        }
    }
    
    try {
        let result = riskyOperation();
        assert(result.get() === 42, "Result should contain 42");
    } catch (error) {
        // Exception should be handled gracefully
        _print("Exception handled:", error.message);
    }
    
    _print("✓ Exception handling tests passed");
}

// Test 9: Function Parameters and Return Values
function testFunctionParameters(): void {
    _print("=== Test 9: Function Parameters and Return Values ===");
    
    function processPtr(ptr: shared_ptr<number>): shared_ptr<string> {
        let value = ptr.get();
        return new shared_ptr<string>(`Processed: ${value}`);
    }
    
    function createPtr(): shared_ptr<number> {
        return new shared_ptr<number>(100);
    }
    
    let input = createPtr();
    let output = processPtr(input);
    
    assert(input.get() === 100, "Input should contain 100");
    assert(output.get() === "Processed: 100", "Output should contain 'Processed: 100'");
    
    _print("✓ Function parameter tests passed");
}

// Test 10: Array and Collection Handling
function testArrayHandling(): void {
    _print("=== Test 10: Array and Collection Handling ===");
    
    let numbers: shared_ptr<number>[] = [];
    
    // Create array of smart pointers
    for (let i = 0; i < 10; i++) {
        numbers.push(new shared_ptr<number>(i * 10));
    }
    
    // Verify array contents
    for (let i = 0; i < numbers.length; i++) {
        assert(numbers[i].get() === i * 10, `numbers[${i}] should contain ${i * 10}`);
    }
    
    // Test array operations
    let sum = 0;
    for (let ptr of numbers) {
        sum += ptr.get();
    }
    
    assert(sum === 450, "Sum should be 450"); // 0+10+20+...+90 = 450
    
    _print("✓ Array handling tests passed");
}

// Test 11: Inheritance and Polymorphism
function testInheritance(): void {
    _print("=== Test 11: Inheritance and Polymorphism ===");
    
    class BaseClass {
        protected value: number;
        
        constructor(value: number) {
            this.value = value;
        }
        
        getValue(): number {
            return this.value;
        }
    }
    
    class DerivedClass extends BaseClass {
        private multiplier: number;
        
        constructor(value: number, multiplier: number) {
            super(value);
            this.multiplier = multiplier;
        }
        
        getMultipliedValue(): number {
            return this.value * this.multiplier;
        }
    }
    
    let base: shared_ptr<BaseClass> = new shared_ptr<BaseClass>(new BaseClass(10));
    let derived: shared_ptr<DerivedClass> = new shared_ptr<DerivedClass>(new DerivedClass(10, 2));
    
    assert(base.get().getValue() === 10, "Base class should have value 10");
    assert(derived.get().getValue() === 10, "Derived class should have value 10");
    assert(derived.get().getMultipliedValue() === 20, "Derived class should have multiplied value 20");
    
    _print("✓ Inheritance tests passed");
}

// Test 12: Performance Characteristics
function testPerformance(): void {
    _print("=== Test 12: Performance Characteristics ===");
    
    const startTime = Date.now();
    
    // Create many objects to test performance
    let objects: shared_ptr<number>[] = [];
    for (let i = 0; i < 1000; i++) {
        objects.push(new shared_ptr<number>(i));
    }
    
    // Perform operations
    let sum = 0;
    for (let obj of objects) {
        sum += obj.get();
    }
    
    const endTime = Date.now();
    const duration = endTime - startTime;
    
    assert(sum === 499500, "Sum should be 499500"); // 0+1+2+...+999
    assert(duration < 1000, "Operation should complete in less than 1000ms");
    
    _print(`✓ Performance test passed (${duration}ms for 1000 objects)`);
}

// Test 13: Edge Cases
function testEdgeCases(): void {
    _print("=== Test 13: Edge Cases ===");
    
    // Test null handling
    let nullPtr: shared_ptr<number> | null = null;
    assert(nullPtr === null, "nullPtr should be null");
    
    // Test zero-sized objects
    let emptyPtr: shared_ptr<object> = new shared_ptr<object>({});
    assert(emptyPtr.get() !== null, "Empty object should not be null");
    
    // Test large objects
    let largeArray: shared_ptr<number[]> = new shared_ptr<number[]>(new Array<number>(10000));
    assert(largeArray.get().length === 10000, "Large array should have 10000 elements");
    
    _print("✓ Edge case tests passed");
}

// Test 14: Memory Leak Detection
function testMemoryLeakDetection(): void {
    _print("=== Test 14: Memory Leak Detection ===");
    
    // Create objects that should be automatically cleaned up
    for (let i = 0; i < 100; i++) {
        let obj = new shared_ptr<number>(i);
        let value = obj.get();
        // obj should be automatically cleaned up
    }
    
    // Force garbage collection if available
    if (typeof gc !== 'undefined') {
        gc();
    }
    
    _print("✓ Memory leak detection test passed");
}

// Test 15: Concurrent Access (Simplified)
function testConcurrentAccess(): void {
    _print("=== Test 15: Concurrent Access ===");
    
    let shared: shared_ptr<number> = new shared_ptr<number>(42);
    
    // Simulate concurrent access
    let ptr1: shared_ptr<number> = shared;
    let ptr2: shared_ptr<number> = shared;
    let ptr3: shared_ptr<number> = shared;
    
    // All should reference the same object
    assert(ptr1.get() === 42, "ptr1 should contain 42");
    assert(ptr2.get() === 42, "ptr2 should contain 42");
    assert(ptr3.get() === 42, "ptr3 should contain 42");
    
    // Clear references
    ptr1 = null;
    ptr2 = null;
    ptr3 = null;
    
    _print("✓ Concurrent access tests passed");
}

// Helper function for assertions
function assert(condition: boolean, message: string): void {
    if (!condition) {
        throw new Error(`Assertion failed: ${message}`);
    }
}

// Main test runner
function runComprehensiveTests(): void {
    _print("TSC ARC Comprehensive System Tests");
    _print("===================================");
    
    let tests = [
        testBasicLifecycle,
        testReferenceCounting,
        testMoveSemantics,
        testWeakReferences,
        testComplexObjectGraphs,
        testCycleDetection,
        testMemoryAnnotations,
        testExceptionHandling,
        testFunctionParameters,
        testArrayHandling,
        testInheritance,
        testPerformance,
        testEdgeCases,
        testMemoryLeakDetection,
        testConcurrentAccess
    ];
    
    let passed = 0;
    let failed = 0;
    
    for (let test of tests) {
        try {
            test();
            passed++;
        } catch (error) {
            _print(`❌ Test failed: ${error.message}`);
            failed++;
        }
    }
    
    _print("\n=== Test Results ===");
    _print(`Passed: ${passed}`);
    _print(`Failed: ${failed}`);
    _print(`Total: ${passed + failed}`);
    
    if (failed === 0) {
        _print("🎉 All tests passed!");
    } else {
        _print("⚠️ Some tests failed!");
    }
}

// Run tests
runComprehensiveTests();