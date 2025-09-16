// Panic/Abort Performance Benchmark
class PanicAbortBenchmark {
    private iterations: number;
    private panicRate: number;

    constructor(iterations: number = 1000000, panicRate: number = 0.001) {
        this.iterations = iterations;
        this.panicRate = panicRate;
    }

    benchmarkPanicPerformance(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;

        let successCount = 0;
        let panicCount = 0;

        for (let i = 0; i < this.iterations; i++) {
            try {
                if (Math.random() < this.panicRate) {
                    panic(`Panic at iteration ${i}`);
                }
                successCount++;
            } catch (error: Error) {
                panicCount++;
            }
        }

        const endTime = performance.now();
        const endMemory = process.memoryUsage().heapUsed;

        return {
            executionTime: endTime - startTime,
            memoryUsage: endMemory - startMemory,
            successCount,
            errorCount: panicCount,
            iterations: this.iterations
        };
    }

    benchmarkAbortPerformance(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;

        let successCount = 0;
        let abortCount = 0;

        for (let i = 0; i < this.iterations; i++) {
            try {
                if (Math.random() < this.panicRate) {
                    abort(`Abort at iteration ${i}`);
                }
                successCount++;
            } catch (error: Error) {
                abortCount++;
            }
        }

        const endTime = performance.now();
        const endMemory = process.memoryUsage().heapUsed;

        return {
            executionTime: endTime - startTime,
            memoryUsage: endMemory - startMemory,
            successCount,
            errorCount: abortCount,
            iterations: this.iterations
        };
    }

    benchmarkAssertionPerformance(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;

        let successCount = 0;
        let assertionCount = 0;

        for (let i = 0; i < this.iterations; i++) {
            try {
                assert(i >= 0, `Assertion failed at iteration ${i}`);
                successCount++;
            } catch (error: Error) {
                assertionCount++;
            }
        }

        const endTime = performance.now();
        const endMemory = process.memoryUsage().heapUsed;

        return {
            executionTime: endTime - startTime,
            memoryUsage: endMemory - startMemory,
            successCount,
            errorCount: assertionCount,
            iterations: this.iterations
        };
    }
}

// Run panic/abort benchmarks
console.log("=== Panic/Abort Performance Benchmark ===");

const benchmark = new PanicAbortBenchmark(1000000, 0.001);

const panicResult = benchmark.benchmarkPanicPerformance();
console.log("Panic Performance:");
console.log(`  Execution time: ${panicResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${panicResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(panicResult.successCount / panicResult.iterations * 100).toFixed(2)}%`);
console.log(`  Panic rate: ${(panicResult.errorCount / panicResult.iterations * 100).toFixed(2)}%`);

const abortResult = benchmark.benchmarkAbortPerformance();
console.log("Abort Performance:");
console.log(`  Execution time: ${abortResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${abortResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(abortResult.successCount / abortResult.iterations * 100).toFixed(2)}%`);
console.log(`  Abort rate: ${(abortResult.errorCount / abortResult.iterations * 100).toFixed(2)}%`);

const assertionResult = benchmark.benchmarkAssertionPerformance();
console.log("Assertion Performance:");
console.log(`  Execution time: ${assertionResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${assertionResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(assertionResult.successCount / assertionResult.iterations * 100).toFixed(2)}%`);
console.log(`  Assertion rate: ${(assertionResult.errorCount / assertionResult.iterations * 100).toFixed(2)}%`);
