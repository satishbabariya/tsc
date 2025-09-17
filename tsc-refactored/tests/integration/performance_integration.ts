// Performance integration with error handling
class PerformanceMonitor {
    private startTime: number = 0;
    private endTime: number = 0;
    private memoryUsage: NodeJS.MemoryUsage | null = null;

    start(): void {
        this.startTime = performance.now();
        this.memoryUsage = process.memoryUsage();
    }

    end(): PerformanceMetrics {
        this.endTime = performance.now();
        const endMemoryUsage = process.memoryUsage();

        return {
            executionTime: this.endTime - this.startTime,
            memoryDelta: endMemoryUsage.heapUsed - (this.memoryUsage?.heapUsed || 0),
            peakMemory: endMemoryUsage.heapUsed,
            startMemory: this.memoryUsage?.heapUsed || 0
        };
    }
}

interface PerformanceMetrics {
    executionTime: number;
    memoryDelta: number;
    peakMemory: number;
    startMemory: number;
}

// Performance testing with error handling
class ErrorHandlingPerformanceTest {
    private monitor: PerformanceMonitor;

    constructor() {
        this.monitor = new PerformanceMonitor();
    }

    testTryCatchPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();

        for (let i = 0; i < iterations; i++) {
            try {
                if (i % 100 === 0) {
                    throw new Error(`Error at iteration ${i}`);
                }
                // Simulate work
                Math.random();
            } catch (error: Error) {
                // Handle error
                console.log("Caught error:", error.message);
            }
        }

        return this.monitor.end();
    }

    testErrorPropagationPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();

        for (let i = 0; i < iterations; i++) {
            const result = this.simulateErrorPropagation(i);
            if (result.isErr()) {
                // Handle error
                console.log("Error propagated:", result.unwrapOr(""));
            }
        }

        return this.monitor.end();
    }

    testPanicAbortPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();

        for (let i = 0; i < iterations; i++) {
            try {
                if (i % 1000 === 0) {
                    this.simulatePanic(`Panic at iteration ${i}`);
                }
                // Simulate work
                Math.random();
            } catch (error: Error) {
                // Handle panic
                console.log("Panic handled:", error.message);
            }
        }

        return this.monitor.end();
    }

    testCustomExceptionPerformance(iterations: number): PerformanceMetrics {
        this.monitor.start();

        for (let i = 0; i < iterations; i++) {
            try {
                if (i % 50 === 0) {
                    throw new CustomPerformanceError(`Custom error at iteration ${i}`, i);
                }
                // Simulate work
                Math.random();
            } catch (error: CustomPerformanceError) {
                // Handle custom error
                console.log("Custom error handled:", error.message, error.code);
            }
        }

        return this.monitor.end();
    }

    private simulateErrorPropagation(value: number): Result<number, string> {
        if (value % 100 === 0) {
            return Result.err(`Error at value ${value}`);
        }
        return Result.ok(value * 2);
    }

    private simulatePanic(message: string): never {
        throw new Error(`PANIC: ${message}`);
    }
}

class CustomPerformanceError extends Error {
    constructor(message: string, public code: number) {
        super(message);
        this.name = "CustomPerformanceError";
    }
}

// Performance comparison
class PerformanceComparison {
    static compareErrorHandlingMethods(iterations: number): void {
        const test = new ErrorHandlingPerformanceTest();

        console.log("=== Performance Comparison ===");
        console.log(`Testing with ${iterations} iterations`);

        // Test try-catch performance
        const tryCatchMetrics = test.testTryCatchPerformance(iterations);
        console.log("Try-Catch Performance:");
        console.log(`  Execution time: ${tryCatchMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${tryCatchMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${tryCatchMetrics.peakMemory} bytes`);

        // Test error propagation performance
        const errorPropagationMetrics = test.testErrorPropagationPerformance(iterations);
        console.log("Error Propagation Performance:");
        console.log(`  Execution time: ${errorPropagationMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${errorPropagationMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${errorPropagationMetrics.peakMemory} bytes`);

        // Test panic/abort performance
        const panicAbortMetrics = test.testPanicAbortPerformance(iterations);
        console.log("Panic/Abort Performance:");
        console.log(`  Execution time: ${panicAbortMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${panicAbortMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${panicAbortMetrics.peakMemory} bytes`);

        // Test custom exception performance
        const customExceptionMetrics = test.testCustomExceptionPerformance(iterations);
        console.log("Custom Exception Performance:");
        console.log(`  Execution time: ${customExceptionMetrics.executionTime.toFixed(2)}ms`);
        console.log(`  Memory delta: ${customExceptionMetrics.memoryDelta} bytes`);
        console.log(`  Peak memory: ${customExceptionMetrics.peakMemory} bytes`);

        // Calculate performance ratios
        const tryCatchTime = tryCatchMetrics.executionTime;
        const errorPropagationTime = errorPropagationMetrics.executionTime;
        const panicAbortTime = panicAbortMetrics.executionTime;
        const customExceptionTime = customExceptionMetrics.executionTime;

        console.log("\nPerformance Ratios (relative to try-catch):");
        console.log(`  Error Propagation: ${(errorPropagationTime / tryCatchTime).toFixed(2)}x`);
        console.log(`  Panic/Abort: ${(panicAbortTime / tryCatchTime).toFixed(2)}x`);
        console.log(`  Custom Exception: ${(customExceptionTime / tryCatchTime).toFixed(2)}x`);
    }
}

// Run performance tests
console.log("=== Testing Performance Integration ===");

// Test with different iteration counts
PerformanceComparison.compareErrorHandlingMethods(1000);
PerformanceComparison.compareErrorHandlingMethods(10000);
PerformanceComparison.compareErrorHandlingMethods(100000);
