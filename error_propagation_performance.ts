
// Error Propagation Performance Benchmark
class ErrorPropagationBenchmark {
    private iterations: number;
    private errorRate: number;
    
    constructor(iterations: number = 1000000, errorRate: number = 0.01) {
        this.iterations = iterations;
        this.errorRate = errorRate;
    }
    
    benchmarkResultType(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;
        
        let successCount = 0;
        let errorCount = 0;
        
        for (let i = 0; i < this.iterations; i++) {
            const result = this.simulateOperation(i);
            if (result.isOk()) {
                successCount++;
            } else {
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
    
    benchmarkOptionalType(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;
        
        let successCount = 0;
        let errorCount = 0;
        
        for (let i = 0; i < this.iterations; i++) {
            const optional = this.simulateOptionalOperation(i);
            if (optional.hasValue()) {
                successCount++;
            } else {
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
    
    benchmarkErrorChaining(): BenchmarkResult {
        const startTime = performance.now();
        const startMemory = process.memoryUsage().heapUsed;
        
        let successCount = 0;
        let errorCount = 0;
        
        for (let i = 0; i < this.iterations; i++) {
            const result = this.simulateChainedOperation(i);
            if (result.isOk()) {
                successCount++;
            } else {
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
    
    private simulateOperation(value: number): Result<number, string> {
        if (Math.random() < this.errorRate) {
            return Result.err(`Error at value ${value}`);
        }
        return Result.ok(value * 2);
    }
    
    private simulateOptionalOperation(value: number): Optional<number> {
        if (Math.random() < this.errorRate) {
            return Optional.none();
        }
        return Optional.some(value * 2);
    }
    
    private simulateChainedOperation(value: number): Result<number, string> {
        const step1 = this.simulateOperation(value);
        if (step1.isErr()) {
            return step1;
        }
        
        const step2 = this.simulateOperation(step1.unwrap());
        if (step2.isErr()) {
            return step2;
        }
        
        const step3 = this.simulateOperation(step2.unwrap());
        return step3;
    }
}

// Run error propagation benchmarks
console.log("=== Error Propagation Performance Benchmark ===");

const benchmark = new ErrorPropagationBenchmark(1000000, 0.01);

const resultTypeResult = benchmark.benchmarkResultType();
console.log("Result Type:");
console.log(`  Execution time: ${resultTypeResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${resultTypeResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(resultTypeResult.successCount / resultTypeResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(resultTypeResult.errorCount / resultTypeResult.iterations * 100).toFixed(2)}%`);

const optionalTypeResult = benchmark.benchmarkOptionalType();
console.log("Optional Type:");
console.log(`  Execution time: ${optionalTypeResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${optionalTypeResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(optionalTypeResult.successCount / optionalTypeResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(optionalTypeResult.errorCount / optionalTypeResult.iterations * 100).toFixed(2)}%`);

const errorChainingResult = benchmark.benchmarkErrorChaining();
console.log("Error Chaining:");
console.log(`  Execution time: ${errorChainingResult.executionTime.toFixed(2)}ms`);
console.log(`  Memory usage: ${errorChainingResult.memoryUsage} bytes`);
console.log(`  Success rate: ${(errorChainingResult.successCount / errorChainingResult.iterations * 100).toFixed(2)}%`);
console.log(`  Error rate: ${(errorChainingResult.errorCount / errorChainingResult.iterations * 100).toFixed(2)}%`);
