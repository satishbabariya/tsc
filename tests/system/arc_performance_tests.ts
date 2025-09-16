// ARC Performance Benchmark Tests
// These tests measure the performance characteristics of ARC implementation

// Performance measurement utilities
class PerformanceTimer {
    private startTime: number = 0;

    start(): void {
        this.startTime = Date.now();
    }

    stop(): number {
        return Date.now() - this.startTime;
    }
}

class PerformanceStats {
    private measurements: number[] = [];

    addMeasurement(duration: number): void {
        this.measurements.push(duration);
    }

    getAverage(): number {
        if (this.measurements.length === 0) return 0;
        return this.measurements.reduce((sum, val) => sum + val, 0) / this.measurements.length;
    }

    getMin(): number {
        return Math.min(...this.measurements);
    }

    getMax(): number {
        return Math.max(...this.measurements);
    }

    getCount(): number {
        return this.measurements.length;
    }

    getTotal(): number {
        return this.measurements.reduce((sum, val) => sum + val, 0);
    }
}

// Test 1: Object Creation Performance
function benchmarkObjectCreation(): void {
    _print("=== Benchmark 1: Object Creation Performance ===");

    const iterations = 10000;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

        let objects: shared_ptr<number>[] = [];
        for (let i = 0; i < iterations; i++) {
            objects.push(new shared_ptr<number>(i));
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);

        // Clean up
        objects = [];
    }

    _print(`Object Creation (${iterations} objects):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per object: ${(stats.getAverage() / iterations).toFixed(4)}ms`);
}

// Test 2: Reference Counting Performance
function benchmarkReferenceCounting(): void {
    _print("=== Benchmark 2: Reference Counting Performance ===");

    const iterations = 10000;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

        let ptr: shared_ptr<number> = new shared_ptr<number>(42);

        for (let i = 0; i < iterations; i++) {
            let temp1: shared_ptr<number> = ptr;
            let temp2: shared_ptr<number> = temp1;
            let temp3: shared_ptr<number> = temp2;

            // Clear references
            temp1 = null;
            temp2 = null;
            temp3 = null;
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);
    }

    _print(`Reference Counting (${iterations} operations):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per operation: ${(stats.getAverage() / iterations).toFixed(4)}ms`);
}

// Test 3: Move Semantics Performance
function benchmarkMoveSemantics(): void {
    _print("=== Benchmark 3: Move Semantics Performance ===");

    const iterations = 10000;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

        let current: unique_ptr<number> = new unique_ptr<number>(0);

        for (let i = 1; i < iterations; i++) {
            let next: unique_ptr<number> = new unique_ptr<number>(i);
            current = std::move(next);
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);
    }

    _print(`Move Semantics (${iterations} moves):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per move: ${(stats.getAverage() / iterations).toFixed(4)}ms`);
}

// Test 4: Weak Reference Performance
function benchmarkWeakReferences(): void {
    _print("=== Benchmark 4: Weak Reference Performance ===");

    const iterations = 10000;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

        for (let i = 0; i < iterations; i++) {
            let strong: shared_ptr<number> = new shared_ptr<number>(i);
            let weak: weak_ptr<number> = new weak_ptr<number>(strong);

            // Access weak reference
            let value = weak.get();
            assert(value === i, `Weak reference should contain ${i}`);

            // Clear strong reference
            strong = null;

            // Check weak reference again
            value = weak.get();
            assert(value === null, "Weak reference should be null");
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);
    }

    _print(`Weak References (${iterations} operations):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per operation: ${(stats.getAverage() / iterations).toFixed(4)}ms`);
}

// Test 5: Array Operations Performance
function benchmarkArrayOperations(): void {
    _print("=== Benchmark 5: Array Operations Performance ===");

    const arraySize = 1000;
    const iterations = 100;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

        for (let iter = 0; iter < iterations; iter++) {
            let numbers: shared_ptr<number>[] = [];

            // Create array
            for (let i = 0; i < arraySize; i++) {
                numbers.push(new shared_ptr<number>(i));
            }

            // Process array
            let sum = 0;
            for (let ptr of numbers) {
                sum += ptr.get();
            }

            // Clear array
            numbers = [];
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);
    }

    _print(`Array Operations (${arraySize} elements, ${iterations} iterations):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per iteration: ${(stats.getAverage() / iterations).toFixed(4)}ms`);
}

// Test 6: Function Call Performance
function benchmarkFunctionCalls(): void {
    _print("=== Benchmark 6: Function Call Performance ===");

    function processPtr(ptr: shared_ptr<number>): shared_ptr<string> {
        let value = ptr.get();
        return new shared_ptr<string>(`Processed: ${value}`);
    }

    const iterations = 10000;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

        for (let i = 0; i < iterations; i++) {
            let input: shared_ptr<number> = new shared_ptr<number>(i);
            let output: shared_ptr<string> = processPtr(input);

            // Verify result
            assert(output.get() === `Processed: ${i}`, `Output should be 'Processed: ${i}'`);
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);
    }

    _print(`Function Calls (${iterations} calls):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per call: ${(stats.getAverage() / iterations).toFixed(4)}ms`);
}

// Test 7: Memory Allocation Patterns
function benchmarkMemoryAllocation(): void {
    _print("=== Benchmark 7: Memory Allocation Patterns ===");

    const patterns = [
        {name: "Small Objects", count: 10000, size: 1},
        {name: "Medium Objects", count: 1000, size: 100},
        {name: "Large Objects", count: 100, size: 1000}
    ];

    for (let pattern of patterns) {
        const stats = new PerformanceStats();

        for (let run = 0; run < 10; run++) {
            const timer = new PerformanceTimer();
            timer.start();

            let objects: shared_ptr<number[]>[] = [];
            for (let i = 0; i < pattern.count; i++) {
                let array = new Array<number>(pattern.size);
                for (let j = 0; j < pattern.size; j++) {
                    array[j] = j;
                }
                objects.push(new shared_ptr<number[]>(array));
            }

            const duration = timer.stop();
            stats.addMeasurement(duration);

            // Clean up
            objects = [];
        }

        _print(`${pattern.name} (${pattern.count} objects, ${pattern.size} elements each):`);
        _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
        _print(`  Min: ${stats.getMin()}ms`);
        _print(`  Max: ${stats.getMax()}ms`);
        _print(`  Per object: ${(stats.getAverage() / pattern.count).toFixed(4)}ms`);
    }
}

// Test 8: Cycle Detection Performance
function benchmarkCycleDetection(): void {
    _print("=== Benchmark 8: Cycle Detection Performance ===");

    class CycleNode {
        private value: number;
        private next: weak_ptr<CycleNode>;

        constructor(value: number) {
            this.value = value;
        }

        setNext(node: shared_ptr<CycleNode>): void {
            this.next = new weak_ptr<CycleNode>(node);
        }

        getNext(): shared_ptr<CycleNode> | null {
            return this.next.get();
        }

        getValue(): number {
            return this.value;
        }
    }

    const nodeCounts = [10, 50, 100, 500];

    for (let nodeCount of nodeCounts) {
        const stats = new PerformanceStats();

        for (let run = 0; run < 10; run++) {
            const timer = new PerformanceTimer();
            timer.start();

            // Create cycle
            let nodes: shared_ptr<CycleNode>[] = [];
            for (let i = 0; i < nodeCount; i++) {
                nodes.push(new shared_ptr<CycleNode>(new CycleNode(i)));
            }

            // Create cycle
            for (let i = 0; i < nodeCount; i++) {
                let nextIndex = (i + 1) % nodeCount;
                nodes[i].get().setNext(nodes[nextIndex]);
            }

            // Traverse cycle
            let node: shared_ptr<CycleNode> | null = nodes[0];
            let count = 0;
            while (node !== null && count < nodeCount) {
                node = node.get().getNext();
                count++;
            }

            const duration = timer.stop();
            stats.addMeasurement(duration);
        }

        _print(`Cycle Detection (${nodeCount} nodes):`);
        _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
        _print(`  Min: ${stats.getMin()}ms`);
        _print(`  Max: ${stats.getMax()}ms`);
        _print(`  Per node: ${(stats.getAverage() / nodeCount).toFixed(4)}ms`);
    }
}

// Test 9: Memory Pressure Performance
function benchmarkMemoryPressure(): void {
    _print("=== Benchmark 9: Memory Pressure Performance ===");

    const cycles = 100;
    const objectsPerCycle = 1000;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

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

            // Clear objects
            objects = [];
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);
    }

    _print(`Memory Pressure (${cycles} cycles, ${objectsPerCycle} objects each):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per cycle: ${(stats.getAverage() / cycles).toFixed(4)}ms`);
}

// Test 10: Overall System Performance
function benchmarkOverallSystem(): void {
    _print("=== Benchmark 10: Overall System Performance ===");

    const iterations = 1000;
    const stats = new PerformanceStats();

    for (let run = 0; run < 10; run++) {
        const timer = new PerformanceTimer();
        timer.start();

        // Mixed operations
        let objects: shared_ptr<number>[] = [];
        let uniqueObjects: unique_ptr<string>[] = [];
        let weakRefs: weak_ptr<number>[] = [];

        for (let i = 0; i < iterations; i++) {
            // Create shared pointer
            let shared: shared_ptr<number> = new shared_ptr<number>(i);
            objects.push(shared);

            // Create unique pointer
            let unique: unique_ptr<string> = new unique_ptr<string>(`Unique ${i}`);
            uniqueObjects.push(std::move(unique));

            // Create weak reference
            let weak: weak_ptr<number> = new weak_ptr<number>(shared);
            weakRefs.push(weak);

            // Perform operations
            let value = shared.get();
            let uniqueValue = uniqueObjects[i].get();
            let weakValue = weak.get();

            assert(value === i, `Shared value should be ${i}`);
            assert(uniqueValue === `Unique ${i}`, `Unique value should be 'Unique ${i}'`);
            assert(weakValue === i, `Weak value should be ${i}`);
        }

        const duration = timer.stop();
        stats.addMeasurement(duration);
    }

    _print(`Overall System (${iterations} mixed operations):`);
    _print(`  Average: ${stats.getAverage().toFixed(2)}ms`);
    _print(`  Min: ${stats.getMin()}ms`);
    _print(`  Max: ${stats.getMax()}ms`);
    _print(`  Per operation: ${(stats.getAverage() / iterations).toFixed(4)}ms`);
}

// Helper function for assertions
function assert(condition: boolean, message: string): void {
    if (!condition) {
        throw new Error(`Assertion failed: ${message}`);
    }
}

// Main benchmark runner
function runPerformanceBenchmarks(): void {
    _print("TSC ARC Performance Benchmarks");
    _print("===============================");
    _print();

    let benchmarks = [
        benchmarkObjectCreation,
        benchmarkReferenceCounting,
        benchmarkMoveSemantics,
        benchmarkWeakReferences,
        benchmarkArrayOperations,
        benchmarkFunctionCalls,
        benchmarkMemoryAllocation,
        benchmarkCycleDetection,
        benchmarkMemoryPressure,
        benchmarkOverallSystem
    ];

    let passed = 0;
    let failed = 0;
    let totalStartTime = Date.now();

    for (let benchmark of benchmarks) {
        try {
            benchmark();
            passed++;
        } catch (error) {
            _print(`❌ Benchmark failed: ${error.message}`);
            failed++;
        }
        _print();
    }

    let totalTime = Date.now() - totalStartTime;

    _print("=== Benchmark Results ===");
    _print(`Passed: ${passed}`);
    _print(`Failed: ${failed}`);
    _print(`Total: ${passed + failed}`);
    _print(`Total time: ${totalTime}ms`);

    if (failed === 0) {
        _print("🎉 All benchmarks completed successfully!");
    } else {
        _print("⚠️ Some benchmarks failed!");
    }
}

// Run benchmarks
runPerformanceBenchmarks();