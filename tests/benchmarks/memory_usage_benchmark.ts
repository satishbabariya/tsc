// Memory usage benchmark for error handling
class MemoryUsageBenchmark {
    private errorHandlers: BaseError[] = [];
    private results: Result<any, BaseError>[] = [];

    async benchmarkMemoryUsage(iterations: number): Promise<MemoryBenchmarkResult> {
        const startMemory = process.memoryUsage();

        // Test error object creation
        for (let i = 0; i < iterations; i++) {
            try {
                if (i % 100 === 0) {
                    throw new NetworkError(`Network error ${i}`, `https://api.example.com/test/${i}`, 500);
                } else if (i % 200 === 0) {
                    throw new DatabaseError(`Database error ${i}`, `SELECT *
                                                                    FROM test
                                                                    WHERE id = ${i}`, "test");
                } else if (i % 300 === 0) {
                    throw new ValidationError(`Validation error ${i}`, "field", i);
                }

                // Create result objects
                const result = Result.ok({iteration: i, data: `test-${i}`});
                this.results.push(result);

            } catch (error: BaseError) {
                this.errorHandlers.push(error);
                const errorResult = Result.err(error);
                this.results.push(errorResult);
            }
        }

        const endMemory = process.memoryUsage();

        return {
            startMemory: startMemory.heapUsed,
            endMemory: endMemory.heapUsed,
            memoryDelta: endMemory.heapUsed - startMemory.heapUsed,
            errorCount: this.errorHandlers.length,
            resultCount: this.results.length,
            iterations
        };
    }

    async benchmarkErrorPropagationMemory(iterations: number): Promise<MemoryBenchmarkResult> {
        const startMemory = process.memoryUsage();

        for (let i = 0; i < iterations; i++) {
            const result = this.simulateErrorPropagation(i);
            this.results.push(result);
        }

        const endMemory = process.memoryUsage();

        return {
            startMemory: startMemory.heapUsed,
            endMemory: endMemory.heapUsed,
            memoryDelta: endMemory.heapUsed - startMemory.heapUsed,
            errorCount: this.results.filter(r => r.isErr()).length,
            resultCount: this.results.length,
            iterations
        };
    }

    private simulateErrorPropagation(value: number): Result<number, BaseError> {
        if (value % 100 === 0) {
            return Result.err(new ValidationError(`Error at value ${value}`, "value", value));
        }
        return Result.ok(value * 2);
    }
}

interface MemoryBenchmarkResult {
    startMemory: number;
    endMemory: number;
    memoryDelta: number;
    errorCount: number;
    resultCount: number;
    iterations: number;
}

// Test memory usage
console.log("=== Memory Usage Benchmark ===");

const memoryBenchmark = new MemoryUsageBenchmark();

memoryBenchmark.benchmarkMemoryUsage(10000).then(result => {
    console.log("Memory Usage Results:");
    console.log(`  Start memory: ${result.startMemory} bytes`);
    console.log(`  End memory: ${result.endMemory} bytes`);
    console.log(`  Memory delta: ${result.memoryDelta} bytes`);
    console.log(`  Error count: ${result.errorCount}`);
    console.log(`  Result count: ${result.resultCount}`);
    console.log(`  Iterations: ${result.iterations}`);
});

memoryBenchmark.benchmarkErrorPropagationMemory(10000).then(result => {
    console.log("Error Propagation Memory Results:");
    console.log(`  Start memory: ${result.startMemory} bytes`);
    console.log(`  End memory: ${result.endMemory} bytes`);
    console.log(`  Memory delta: ${result.memoryDelta} bytes`);
    console.log(`  Error count: ${result.errorCount}`);
    console.log(`  Result count: ${result.resultCount}`);
    console.log(`  Iterations: ${result.iterations}`);
});
