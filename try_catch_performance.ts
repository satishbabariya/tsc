
// Try-Catch Performance Benchmark
class TryCatchBenchmark {
    private iterations: number;
    private errorRate: number;
    
    constructor(iterations: number = 1000000, errorRate: number = 0.01) {
        this.iterations = iterations;
        this.errorRate = errorRate;
    }
    
    benchmarkBasicTryCatch(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;
        
        let successCount = 0;
        let errorCount = 0;
        
        for (let i = 0; i < this.iterations; i++) {
            try {
                if (Math.random() < this.errorRate) {
                    throw new Error(`Error at iteration ${i}`);
                }
                successCount++;
            } catch (error: Error) {
                errorCount++;
            }
        }
        
        const endTime = performance.now();
        const endMemory = process.memoryUsage().heapUsed;
        
        return {
            executionTime: endTime - startTime,
            memoryUsage: endMemory - startMemory,
            successCount,
            errorCount,
            iterations: this.iterations
        };
    }
    
    benchmarkNestedTryCatch(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;
        
        let successCount = 0;
        let errorCount = 0;
        
        for (let i = 0; i < this.iterations; i++) {
            try {
                try {
                    if (Math.random() < this.errorRate) {
                        throw new Error(`Inner error at iteration ${i}`);
                    }
                    successCount++;
                } catch (innerError: Error) {
                    throw new Error(`Outer error: ${innerError.message}`);
                }
            } catch (outerError: Error) {
                errorCount++;
            }
        }
        
        const endTime = performance.now();
        const endMemory = process.memoryUsage().heapUsed;
        
        return {
            executionTime: endTime - startTime,
            memoryUsage: endMemory - startMemory,
            successCount,
            errorCount,
            iterations: this.iterations
        };
    }
    
    benchmarkTryCatchFinally(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;
        
        let successCount = 0;
        let errorCount = 0;
        let finallyCount = 0;
        
        for (let i = 0; i < this.iterations; i++) {
            try {
                if (Math.random() < this.errorRate) {
                    throw new Error(`Error at iteration ${i}`);
                }
                successCount++;
            } catch (error: Error) {
                errorCount++;
            } finally {
                finallyCount++;
            }
        }
        
        const endTime = performance.now();
        const endMemory = process.memoryUsage().heapUsed;
        
        return {
            executionTime: endTime - startTime,
            memoryUsage: endMemory - startMemory,
            successCount,
            errorCount,
            iterations: this.iterations,
            finallyCount
        };
    }
}

interface BenchmarkResult {
    executionTime: number;
    memoryUsage: number;
    successCount: number;
    errorCount: number;
    iterations: number;
    finallyCount?: number;
}

// Run try-catch benchmarks
console.log("=== Try-Catch Performance Benchmark ===");

const benchmark = new TryCatchBenchmark(1000000, 0.01);

const basicResult = benchmark.benchmarkBasicTryCatch();
console.log("Basic Try-Catch:");
console.log(`  Execution time: ${basicResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${basicResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(basicResult.successCount / basicResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(basicResult.errorCount / basicResult.iterations * 100).toFixed(2)}%`);

const nestedResult = benchmark.benchmarkNestedTryCatch();
console.log("Nested Try-Catch:");
console.log(`  Execution time: ${nestedResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${nestedResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(nestedResult.successCount / nestedResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(nestedResult.errorCount / nestedResult.iterations * 100).toFixed(2)}%`);

const finallyResult = benchmark.benchmarkTryCatchFinally();
console.log("Try-Catch-Finally:");
console.log(`  Execution time: ${finallyResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${finallyResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(finallyResult.successCount / finallyResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(finallyResult.errorCount / finallyResult.iterations * 100).toFixed(2)}%`);
console.log(`  Finally executions: ${finallyResult.finallyCount}`);
