// Memory Usage Benchmark
class MemoryUsageBenchmark {
    private iterations: number;

    constructor(iterations: number = 100000) {
        this.iterations = iterations;
    }

    benchmarkMemoryUsage(): MemoryBenchmarkResult {
        const startMemory = process.memoryUsage();

        // Test try-catch memory usage
        const tryCatchMemory = this.testTryCatchMemory();

        // Test error propagation memory usage
        const errorPropagationMemory = this.testErrorPropagationMemory();

        // Test custom exception memory usage
        const customExceptionMemory = this.testCustomExceptionMemory();

        const endMemory = process.memoryUsage();

        return {
            startMemory: startMemory.heapUsed,
            endMemory: endMemory.heapUsed,
            totalMemoryDelta: endMemory.heapUsed - startMemory.heapUsed,
            tryCatchMemory,
            errorPropagationMemory,
            customExceptionMemory
        };
    }

    private testTryCatchMemory(): number {
        const startMemory = process.memoryUsage().heapUsed;

        for (let i = 0; i < this.iterations; i++) {
            try {
                if (i % 100 === 0) {
                    throw new Error(`Error at iteration ${i}`);
                }
            } catch (error: Error) {
                // Handle error
            }
        }

        const endMemory = process.memoryUsage().heapUsed;
        return endMemory - startMemory;
    }

    private testErrorPropagationMemory(): number {
        const startMemory = process.memoryUsage().heapUsed;

        for (let i = 0; i < this.iterations; i++) {
            const result = this.simulateOperation(i);
            if (result.isErr()) {
                // Handle error
            }
        }

        const endMemory = process.memoryUsage().heapUsed;
        return endMemory - startMemory;
    }

    private testCustomExceptionMemory(): number {
        const startMemory = process.memoryUsage().heapUsed;

        for (let i = 0; i < this.iterations; i++) {
            try {
                if (i % 100 === 0) {
                    throw new CustomMemoryError(`Memory error at iteration ${i}`, i);
                }
            } catch (error: CustomMemoryError) {
                // Handle error
            }
        }

        const endMemory = process.memoryUsage().heapUsed;
        return endMemory - startMemory;
    }

    private simulateOperation(value: number): Result<number, string> {
        if (value % 100 === 0) {
            return Result.err(`Error at value ${value}`);
        }
        return Result.ok(value * 2);
    }
}

class CustomMemoryError extends Error {
    constructor(message: string, public code: number) {
        super(message);
        this.name = "CustomMemoryError";
    }
}

interface MemoryBenchmarkResult {
    startMemory: number;
    endMemory: number;
    totalMemoryDelta: number;
    tryCatchMemory: number;
    errorPropagationMemory: number;
    customExceptionMemory: number;
}

// Run memory usage benchmark
console.log("=== Memory Usage Benchmark ===");

const benchmark = new MemoryUsageBenchmark(100000);
const result = benchmark.benchmarkMemoryUsage();

console.log("Memory Usage Results:");
console.log(`  Start memory: ${result.startMemory} bytes`);
console.log(`  End memory: ${result.endMemory} bytes`);
console.log(`  Total memory delta: ${result.totalMemoryDelta} bytes`);
console.log(`  Try-Catch memory: ${result.tryCatchMemory} bytes`);
console.log(`  Error Propagation memory: ${result.errorPropagationMemory} bytes`);
console.log(`  Custom Exception memory: ${result.customExceptionMemory} bytes`);

console.log("\nMemory Usage Ratios:");
console.log(`  Try-Catch: ${(result.tryCatchMemory / result.totalMemoryDelta * 100).toFixed(2)}%`);
console.log(`  Error Propagation: ${(result.errorPropagationMemory / result.totalMemoryDelta * 100).toFixed(2)}%`);
console.log(`  Custom Exception: ${(result.customExceptionMemory / result.totalMemoryDelta * 100).toFixed(2)}%`);
