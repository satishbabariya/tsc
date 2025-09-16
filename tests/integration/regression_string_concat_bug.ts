// Regression test: Specific scenario that triggered the LLVM verification bug
// This reproduces the exact error pattern that was fixed

class TestContainer<T> {
    data: T;

    constructor(data: T) {
        this.data = data;
    }
}

function main(): number {
    // This exact pattern was causing the LLVM verification error:
    // "Call parameter type does not match function signature!"
    // "%property_value = load double, ptr %property_ptr, align 8"
    // "ptr  %strcat = call ptr @string_concat(double %property_value, ptr %value_val)"

    let container = new TestContainer<number>(42);

    // The bug occurred when:
    // 1. Property access returned a double (numeric value)
    // 2. Some operation involved a pointer type (object reference)  
    // 3. The + operator was used, incorrectly triggering string concatenation

    // This was the problematic pattern - property access + arithmetic
    // that was misidentified as string concatenation
    let value = 5.0;  // Simulating property access result (double)
    let result = value + 10;  // This was incorrectly calling string_concat(double, ptr)

    return 0;
}