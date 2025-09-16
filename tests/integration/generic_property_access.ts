// Integration test: Generic classes with property access patterns
// This targets the specific property access + arithmetic that was causing issues

class NumberBox<T> {
    value: T;
    multiplier: number;

    constructor(value: T, multiplier: number) {
        this.value = value;
        this.multiplier = multiplier;
    }

    // Property access with arithmetic operations
    getScaledValue(): number {
        // This pattern was causing LLVM verification errors:
        // property access (double) + arithmetic was incorrectly treated as string concatenation
        return this.multiplier * 2;
    }

    // Multiple property access with mixed operations
    compute(): number {
        let base = this.multiplier;
        let scaled = base + 10;  // This was the problematic pattern
        return scaled;
    }
}

class Calculator<T, U> {
    first: T;
    second: U;

    constructor(first: T, second: U) {
        this.first = first;
        this.second = second;
    }

    // Mixed property access with numeric operations
    performCalculation(): number {
        // These property accesses were being misidentified as string concatenation
        let val1 = 5.0;  // Simulating property access result
        let val2 = 3.0;  // Simulating another property access result

        // Arithmetic operations that were incorrectly calling string_concat
        let sum = val1 + val2;
        let product = val1 * val2;
        let difference = val1 - val2;

        return sum + product + difference;
    }
}

function main(): number {
    // Test the problematic patterns
    let numberBox = new NumberBox<string>("test", 5);
    let result1 = numberBox.getScaledValue();
    let result2 = numberBox.compute();

    let calc = new Calculator<number, string>(42, "hello");
    let result3 = calc.performCalculation();

    return result1 + result2 + result3;
}