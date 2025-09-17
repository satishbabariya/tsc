// Constraint Violation Test - This should fail compilation
// This test contains actual constraint violations that should be caught by the compiler

function processString<T extends string>(value: T): T {
    return value;
}

function processNumber<T extends number>(value: T): T {
    return value;
}

function main(): void {
    // These should cause compilation errors:
    let invalidStr = processString<number>(123);        // number not assignable to string
    let invalidNum = processNumber<string>("abc");     // string not assignable to number
}

main();