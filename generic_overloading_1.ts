
// Generic method overloading
class DataProcessor<T> {
    data: T;
    
    constructor(data: T) {
        this.data = data;
    }
    
    // Overload 1: Process with single function
    process<U>(processor: (input: T) => U): DataProcessor<U>;
    
    // Overload 2: Process with array of functions
    process<U>(processors: ((input: T) => U)[]): DataProcessor<U[]>;
    
    // Overload 3: Process with conditional function
    process<U>(condition: (input: T) => boolean, processor: (input: T) => U): DataProcessor<U | null>;
    
    // Implementation
    process<U>(processorOrProcessors: ((input: T) => U) | ((input: T) => U)[] | ((input: T) => boolean), 
               processor?: (input: T) => U): DataProcessor<U | U[] | U | null> {
        if (Array.isArray(processorOrProcessors)) {
            // Array of processors
            const results = processorOrProcessors.map(p => p(this.data));
            return new DataProcessor<U[]>(results as U[]);
        } else if (processor) {
            // Conditional processor
            if (processorOrProcessors(this.data)) {
                return new DataProcessor<U>(processor(this.data));
            } else {
                return new DataProcessor<U | null>(null);
            }
        } else {
            // Single processor
            return new DataProcessor<U>(processorOrProcessors(this.data));
        }
    }
}

// Generic function overloading
function transform<T, U>(input: T, transformer: (input: T) => U): U;
function transform<T, U>(input: T[], transformer: (input: T) => U): U[];
function transform<T, U>(input: T | T[], transformer: (input: T) => U): U | U[] {
    if (Array.isArray(input)) {
        return input.map(transformer);
    } else {
        return transformer(input);
    }
}

// Test the overloading
const processor = new DataProcessor<number>(42);

// Test single processor
const result1 = processor.process(x => x * 2);
console.log("Single processor result:", result1.data);

// Test array of processors
const result2 = processor.process([x => x * 2, x => x + 10, x => x - 5]);
console.log("Array processor result:", result2.data);

// Test conditional processor
const result3 = processor.process(x => x > 40, x => x * 3);
console.log("Conditional processor result:", result3.data);

// Test function overloading
const singleResult = transform(42, x => x * 2);
const arrayResult = transform([1, 2, 3], x => x * 2);

console.log("Single transform result:", singleResult);
console.log("Array transform result:", arrayResult);
