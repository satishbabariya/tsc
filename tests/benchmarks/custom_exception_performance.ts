// Custom Exception Performance Benchmark
class CustomExceptionBenchmark {
    private iterations: number;
    private errorRate: number;

    constructor(iterations: number = 1000000, errorRate: number = 0.01) {
        this.iterations = iterations;
        this.errorRate = errorRate;
    }

    benchmarkCustomExceptionCreation(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;

        let successCount = 0;
        let errorCount = 0;

        for (let i = 0; i < this.iterations; i++) {
            try {
                if (Math.random() < this.errorRate) {
                    throw new CustomBenchmarkError(`Custom error at iteration ${i}`, i);
                }
                successCount++;
            } catch (error: CustomBenchmarkError) {
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

    benchmarkExceptionInheritance(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;

        let successCount = 0;
        let errorCount = 0;

        for (let i = 0; i < this.iterations; i++) {
            try {
                if (Math.random() < this.errorRate) {
                    throw new InheritedBenchmarkError(`Inherited error at iteration ${i}`, i, "test");
                }
                successCount++;
            } catch (error: InheritedBenchmarkError) {
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

    benchmarkExceptionFactory(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;

        let successCount = 0;
        let errorCount = 0;

        for (let i = 0; i < this.iterations; i++) {
            try {
                if (Math.random() < this.errorRate) {
                    const error = ExceptionFactory.createBenchmarkError(`Factory error at iteration ${i}`, i);
                    throw error;
                }
                successCount++;
            } catch (error: CustomBenchmarkError) {
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
}

class CustomBenchmarkError extends Error {
    constructor(message: string, public code: number) {
        super(message);
        this.name = "CustomBenchmarkError";
    }
}

class InheritedBenchmarkError extends CustomBenchmarkError {
    constructor(message: string, code: number, public category: string) {
        super(message, code);
        this.name = "InheritedBenchmarkError";
    }
}

class ExceptionFactory {
    static createBenchmarkError(message: string, code: number): CustomBenchmarkError {
        return new CustomBenchmarkError(message, code);
    }
}

// Run custom exception benchmarks
console.log("=== Custom Exception Performance Benchmark ===");

const benchmark = new CustomExceptionBenchmark(1000000, 0.01);

const customExceptionResult = benchmark.benchmarkCustomExceptionCreation();
console.log("Custom Exception Creation:");
console.log(`  Execution time: ${customExceptionResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${customExceptionResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(customExceptionResult.successCount / customExceptionResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(customExceptionResult.errorCount / customExceptionResult.iterations * 100).toFixed(2)}%`);

const inheritanceResult = benchmark.benchmarkExceptionInheritance();
console.log("Exception Inheritance:");
console.log(`  Execution time: ${inheritanceResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${inheritanceResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(inheritanceResult.successCount / inheritanceResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(inheritanceResult.errorCount / inheritanceResult.iterations * 100).toFixed(2)}%`);

const factoryResult = benchmark.benchmarkExceptionFactory();
console.log("Exception Factory:");
console.log(`  Execution time: ${factoryResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${factoryResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(factoryResult.successCount / factoryResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(factoryResult.errorCount / factoryResult.iterations * 100).toFixed(2)}%`);
