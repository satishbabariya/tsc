// ARC Stress Tests
// These tests validate ARC behavior under heavy load and stress conditions

// Test 1: Massive Object Creation
function testMassiveObjectCreation(): void {
    _print("=== Test 1: Massive Object Creation ===");
    
    const objectCount = 10000;
    let objects: shared_ptr<number>[] = [];
    
    const startTime = Date.now();
    
    // Create many objects
    for (let i = 0; i < objectCount; i++) {
        objects.push(new shared_ptr<number>(i));
    }
    
    const creationTime = Date.now() - startTime;
    
    // Verify all objects
    let sum = 0;
    for (let i = 0; i < objects.length; i++) {
        sum += objects[i].get();
    }
    
    const expectedSum = (objectCount - 1) * objectCount / 2; // Sum of 0 to 9999
    assert(sum === expectedSum, `Sum should be ${expectedSum}, got ${sum}`);
    
    _print(`✓ Created ${objectCount} objects in ${creationTime}ms`);
    _print(`✓ Average creation time: ${creationTime / objectCount}ms per object`);
}

// Test 2: Rapid Reference Counting
function testRapidReferenceCounting(): void {
    _print("=== Test 2: Rapid Reference Counting ===");
    
    const iterations = 1000;
    let ptr: shared_ptr<number> = new shared_ptr<number>(42);
    
    const startTime = Date.now();
    
    // Rapidly create and destroy references
    for (let i = 0; i < iterations; i++) {
        let temp1: shared_ptr<number> = ptr;
        let temp2: shared_ptr<number> = temp1;
        let temp3: shared_ptr<number> = temp2;
        
        // Clear references
        temp1 = null;
        temp2 = null;
        temp3 = null;
    }
    
    const endTime = Date.now();
    const duration = endTime - startTime;
    
    // Original pointer should still be valid
    assert(ptr.get() === 42, "Original pointer should still contain 42");
    
    _print(`✓ Performed ${iterations} rapid reference operations in ${duration}ms`);
    _print(`✓ Average time per operation: ${duration / iterations}ms`);
}

// Test 3: Deep Object Graphs
function testDeepObjectGraphs(): void {
    _print("=== Test 3: Deep Object Graphs ===");
    
    class DeepNode {
        private value: number;
        private child: shared_ptr<DeepNode> | null = null;
        
        constructor(value: number) {
            this.value = value;
        }
        
        setChild(child: shared_ptr<DeepNode>): void {
            this.child = child;
        }
        
        getChild(): shared_ptr<DeepNode> | null {
            return this.child;
        }
        
        getValue(): number {
            return this.value;
        }
    }
    
    const depth = 1000;
    let root: shared_ptr<DeepNode> = new shared_ptr<DeepNode>(0);
    let current: shared_ptr<DeepNode> = root;
    
    // Create deep chain
    for (let i = 1; i < depth; i++) {
        let newNode: shared_ptr<DeepNode> = new shared_ptr<DeepNode>(i);
        current.get().setChild(newNode);
        current = newNode;
    }
    
    // Traverse the chain
    let node: shared_ptr<DeepNode> | null = root;
    let count = 0;
    while (node !== null) {
        assert(node.get().getValue() === count, `Node ${count} should have value ${count}`);
        node = node.get().getChild();
        count++;
    }
    
    assert(count === depth, `Should have traversed ${depth} nodes, got ${count}`);
    
    _print(`✓ Created and traversed deep graph of ${depth} nodes`);
}

// Test 4: Complex Cycle Detection
function testComplexCycleDetection(): void {
    _print("=== Test 4: Complex Cycle Detection ===");
    
    class CycleNode {
        private value: number;
        private connections: weak_ptr<CycleNode>[];
        
        constructor(value: number) {
            this.value = value;
            this.connections = [];
        }
        
        addConnection(node: shared_ptr<CycleNode>): void {
            this.connections.push(new weak_ptr<CycleNode>(node));
        }
        
        getConnections(): shared_ptr<CycleNode>[] {
            let validConnections: shared_ptr<CycleNode>[] = [];
            for (let weak of this.connections) {
                let strong = weak.get();
                if (strong !== null) {
                    validConnections.push(strong);
                }
            }
            return validConnections;
        }
        
        getValue(): number {
            return this.value;
        }
    }
    
    const nodeCount = 100;
    let nodes: shared_ptr<CycleNode>[] = [];
    
    // Create nodes
    for (let i = 0; i < nodeCount; i++) {
        nodes.push(new shared_ptr<CycleNode>(new CycleNode(i)));
    }
    
    // Create complex cycle pattern
    for (let i = 0; i < nodeCount; i++) {
        let node = nodes[i];
        // Connect to next 3 nodes (with wraparound)
        for (let j = 1; j <= 3; j++) {
            let targetIndex = (i + j) % nodeCount;
            node.get().addConnection(nodes[targetIndex]);
        }
    }
    
    // Verify connections
    let totalConnections = 0;
    for (let node of nodes) {
        let connections = node.get().getConnections();
        totalConnections += connections.length;
    }
    
    assert(totalConnections === nodeCount * 3, `Should have ${nodeCount * 3} total connections, got ${totalConnections}`);
    
    _print(`✓ Created complex cycle with ${nodeCount} nodes and ${totalConnections} connections`);
}

// Test 5: Memory Pressure Test
function testMemoryPressure(): void {
    _print("=== Test 5: Memory Pressure Test ===");
    
    const cycles = 10;
    const objectsPerCycle = 1000;
    
    for (let cycle = 0; cycle < cycles; cycle++) {
        let objects: shared_ptr<number>[] = [];
        
        // Create objects
        for (let i = 0; i < objectsPerCycle; i++) {
            objects.push(new shared_ptr<number>(cycle * objectsPerCycle + i));
        }
        
        // Use objects
        let sum = 0;
        for (let obj of objects) {
            sum += obj.get();
        }
        
        // Clear objects (should trigger cleanup)
        objects = [];
        
        _print(`Cycle ${cycle + 1}/${cycles} completed`);
    }
    
    _print(`✓ Completed ${cycles} memory pressure cycles`);
}

// Test 6: Move Semantics Stress Test
function testMoveSemanticsStress(): void {
    _print("=== Test 6: Move Semantics Stress Test ===");
    
    const iterations = 1000;
    let current: unique_ptr<number> = new unique_ptr<number>(0);
    
    const startTime = Date.now();
    
    // Chain moves
    for (let i = 1; i < iterations; i++) {
        let next: unique_ptr<number> = new unique_ptr<number>(i);
        current = std::move(next);
    }
    
    const endTime = Date.now();
    const duration = endTime - startTime;
    
    assert(current.get() === iterations - 1, `Final value should be ${iterations - 1}, got ${current.get()}`);
    
    _print(`✓ Performed ${iterations} move operations in ${duration}ms`);
    _print(`✓ Average time per move: ${duration / iterations}ms`);
}

// Test 7: Weak Reference Stress Test
function testWeakReferenceStress(): void {
    _print("=== Test 7: Weak Reference Stress Test ===");
    
    const iterations = 1000;
    let weakRefs: weak_ptr<number>[] = [];
    
    const startTime = Date.now();
    
    // Create and destroy strong references rapidly
    for (let i = 0; i < iterations; i++) {
        let strong: shared_ptr<number> = new shared_ptr<number>(i);
        let weak: weak_ptr<number> = new weak_ptr<number>(strong);
        weakRefs.push(weak);
        
        // Clear strong reference
        strong = null;
    }
    
    const endTime = Date.now();
    const duration = endTime - startTime;
    
    // Check weak references
    let validCount = 0;
    for (let weak of weakRefs) {
        if (weak.get() !== null) {
            validCount++;
        }
    }
    
    assert(validCount === 0, `All weak references should be null, ${validCount} are still valid`);
    
    _print(`✓ Created and destroyed ${iterations} weak references in ${duration}ms`);
}

// Test 8: Exception Handling Stress Test
function testExceptionHandlingStress(): void {
    _print("=== Test 8: Exception Handling Stress Test ===");
    
    const iterations = 100;
    let successCount = 0;
    let exceptionCount = 0;
    
    for (let i = 0; i < iterations; i++) {
        try {
            let ptr: shared_ptr<number> = new shared_ptr<number>(i);
            
            // Simulate random failure
            if (Math.random() < 0.3) {
                throw new Error(`Random error ${i}`);
            }
            
            assert(ptr.get() === i, `Pointer should contain ${i}`);
            successCount++;
            
        } catch (error) {
            exceptionCount++;
            // ARC should handle cleanup automatically
        }
    }
    
    _print(`✓ Handled ${iterations} operations: ${successCount} success, ${exceptionCount} exceptions`);
}

// Test 9: Concurrent Access Simulation
function testConcurrentAccessSimulation(): void {
    _print("=== Test 9: Concurrent Access Simulation ===");
    
    const iterations = 1000;
    let shared: shared_ptr<number> = new shared_ptr<number>(42);
    
    const startTime = Date.now();
    
    // Simulate concurrent access
    for (let i = 0; i < iterations; i++) {
        let ptr1: shared_ptr<number> = shared;
        let ptr2: shared_ptr<number> = shared;
        let ptr3: shared_ptr<number> = shared;
        
        // Verify all point to same object
        assert(ptr1.get() === 42, "ptr1 should contain 42");
        assert(ptr2.get() === 42, "ptr2 should contain 42");
        assert(ptr3.get() === 42, "ptr3 should contain 42");
        
        // Clear references
        ptr1 = null;
        ptr2 = null;
        ptr3 = null;
    }
    
    const endTime = Date.now();
    const duration = endTime - startTime;
    
    // Original should still be valid
    assert(shared.get() === 42, "Original shared pointer should still contain 42");
    
    _print(`✓ Simulated ${iterations} concurrent access operations in ${duration}ms`);
}

// Test 10: Memory Fragmentation Test
function testMemoryFragmentation(): void {
    _print("=== Test 10: Memory Fragmentation Test ===");
    
    const cycles = 50;
    const smallObjects = 100;
    const largeObjects = 10;
    
    for (let cycle = 0; cycle < cycles; cycle++) {
        // Create small objects
        let small: shared_ptr<number>[] = [];
        for (let i = 0; i < smallObjects; i++) {
            small.push(new shared_ptr<number>(i));
        }
        
        // Create large objects
        let large: shared_ptr<number[]>[] = [];
        for (let i = 0; i < largeObjects; i++) {
            large.push(new shared_ptr<number[]>(new Array<number>(1000)));
        }
        
        // Use objects
        let smallSum = 0;
        for (let obj of small) {
            smallSum += obj.get();
        }
        
        let largeSum = 0;
        for (let obj of large) {
            largeSum += obj.get().length;
        }
        
        // Clear objects
        small = [];
        large = [];
        
        if (cycle % 10 === 0) {
            _print(`Fragmentation cycle ${cycle + 1}/${cycles} completed`);
        }
    }
    
    _print(`✓ Completed ${cycles} memory fragmentation cycles`);
}

// Helper function for assertions
function assert(condition: boolean, message: string): void {
    if (!condition) {
        throw new Error(`Assertion failed: ${message}`);
    }
}

// Main stress test runner
function runStressTests(): void {
    _print("TSC ARC Stress Tests");
    _print("===================");
    
    let tests = [
        testMassiveObjectCreation,
        testRapidReferenceCounting,
        testDeepObjectGraphs,
        testComplexCycleDetection,
        testMemoryPressure,
        testMoveSemanticsStress,
        testWeakReferenceStress,
        testExceptionHandlingStress,
        testConcurrentAccessSimulation,
        testMemoryFragmentation
    ];
    
    let passed = 0;
    let failed = 0;
    let startTime = Date.now();
    
    for (let test of tests) {
        try {
            test();
            passed++;
        } catch (error) {
            _print(`❌ Stress test failed: ${error.message}`);
            failed++;
        }
    }
    
    let totalTime = Date.now() - startTime;
    
    _print("\n=== Stress Test Results ===");
    _print(`Passed: ${passed}`);
    _print(`Failed: ${failed}`);
    _print(`Total: ${passed + failed}`);
    _print(`Total time: ${totalTime}ms`);
    
    if (failed === 0) {
        _print("🎉 All stress tests passed!");
    } else {
        _print("⚠️ Some stress tests failed!");
    }
}

// Run stress tests
runStressTests();