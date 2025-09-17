// Edge Cases and Error Handling System Tests
// Tests boundary conditions, error scenarios, and unusual constraint patterns

// Test 1: Empty string constraints
function processEmptyString<T extends string>(value: T): T {
    return value;
}

// Test 2: Zero and negative number constraints
function processNumber<T extends number>(value: T): T {
    return value;
}

// Test 3: Unicode and special character constraints
function processUnicodeString<T extends string>(value: T): T {
    return value;
}

// Test 4: Large number constraints
function processLargeNumber<T extends number>(value: T): T {
    return value;
}

// Test 5: Constraint with optional properties
interface OptionalConstraint {
    id?: number;
    name?: string;
}

function processOptional<T extends OptionalConstraint>(item: T): T {
    return item;
}

// Test 6: Constraint with array types
function processArray<T extends string[]>(arr: T): T {
    return arr;
}

// Test 7: Constraint with function types
type StringFunction = (value: string) => string;

function processFunction<T extends StringFunction>(fn: T): T {
    return fn;
}

// Test 8: Constraint with union types (if supported)
interface UnionConstraint {
    value: string | number;
}

function processUnion<T extends UnionConstraint>(item: T): T {
    return item;
}

// Test 9: Recursive constraint (self-referencing)
interface RecursiveConstraint {
    data: string;
    children?: RecursiveConstraint[];
}

function processRecursive<T extends RecursiveConstraint>(item: T): T {
    return item;
}

// Test 10: Constraint with method signatures
interface MethodConstraint {
    getId(): number;
    getName(): string;
}

function processMethod<T extends MethodConstraint>(item: T): string {
    return `${item.getId()}: ${item.getName()}`;
}

// Test data
const emptyString = "";
const unicodeString = "Hello 世界 🌍";
const largeNumber = 999999999;
const zeroNumber = 0;
const negativeNumber = -42;

const optionalItem: OptionalConstraint = {
    id: 1,
    name: "test"
};

const stringArray: string[] = ["a", "b", "c"];

const stringFunction: StringFunction = (s: string) => s.toUpperCase();

const unionItem: UnionConstraint = {
    value: "test"
};

const recursiveItem: RecursiveConstraint = {
    data: "root",
    children: [
        { data: "child1" },
        { data: "child2" }
    ]
};

const methodItem: MethodConstraint = {
    getId: () => 123,
    getName: () => "Method Item"
};

// Test functions
function testEdgeCases(): void {
    console.log("=== Edge Case Tests ===");
    
    // Empty string
    let emptyResult = processEmptyString<string>(emptyString);
    console.log(`Empty string result: "${emptyResult}"`);
    
    // Unicode string
    let unicodeResult = processUnicodeString<string>(unicodeString);
    console.log(`Unicode string result: ${unicodeResult}`);
    
    // Large number
    let largeResult = processLargeNumber<number>(largeNumber);
    console.log(`Large number result: ${largeResult}`);
    
    // Zero number
    let zeroResult = processNumber<number>(zeroNumber);
    console.log(`Zero number result: ${zeroResult}`);
    
    // Negative number
    let negativeResult = processNumber<number>(negativeNumber);
    console.log(`Negative number result: ${negativeResult}`);
    
    // Optional constraint
    let optionalResult = processOptional<OptionalConstraint>(optionalItem);
    console.log(`Optional result: ${optionalResult.id}, ${optionalResult.name}`);
    
    // Array constraint
    let arrayResult = processArray<string[]>(stringArray);
    console.log(`Array result: ${arrayResult.join(", ")}`);
    
    // Function constraint
    let functionResult = processFunction<StringFunction>(stringFunction);
    let functionOutput = functionResult("hello");
    console.log(`Function result: ${functionOutput}`);
    
    // Union constraint
    let unionResult = processUnion<UnionConstraint>(unionItem);
    console.log(`Union result: ${unionResult.value}`);
    
    // Recursive constraint
    let recursiveResult = processRecursive<RecursiveConstraint>(recursiveItem);
    console.log(`Recursive result: ${recursiveResult.data}, children: ${recursiveResult.children?.length || 0}`);
    
    // Method constraint
    let methodResult = processMethod<MethodConstraint>(methodItem);
    console.log(`Method result: ${methodResult}`);
}

// Test constraint violations (should fail compilation)
function testEdgeCaseViolations(): void {
    console.log("=== Edge Case Constraint Violation Tests (Expected Failures) ===");
    
    // These should cause compilation errors:
    // let invalidEmpty = processEmptyString<number>(0);                    // number not assignable to string
    // let invalidUnicode = processUnicodeString<number>(123);              // number not assignable to string
    // let invalidLarge = processLargeNumber<string>("large");              // string not assignable to number
    // let invalidZero = processNumber<string>("zero");                    // string not assignable to number
    // let invalidNegative = processNumber<string>("negative");            // string not assignable to number
    // let invalidOptional = processOptional<string>("test");              // string doesn't extend OptionalConstraint
    // let invalidArray = processArray<number[]>([]);                      // number[] not assignable to string[]
    // let invalidFunction = processFunction<number>(123);                // number not assignable to StringFunction
    // let invalidUnion = processUnion<string>("test");                   // string doesn't extend UnionConstraint
    // let invalidRecursive = processRecursive<string>("test");            // string doesn't extend RecursiveConstraint
    // let invalidMethod = processMethod<string>("test");                  // string doesn't extend MethodConstraint
    
    console.log("Edge case constraint violation tests are commented out as they should fail compilation");
}

// Test error handling scenarios
function testErrorHandling(): void {
    console.log("=== Error Handling Tests ===");
    
    // Test with valid constraints
    try {
        let validResult = processNumber<number>(42);
        console.log(`Valid constraint test passed: ${validResult}`);
    } catch (error) {
        console.log(`Unexpected error in valid constraint test: ${error}`);
    }
    
    // Test with edge values
    try {
        let edgeResult = processEmptyString<string>("");
        console.log(`Edge value test passed: "${edgeResult}"`);
    } catch (error) {
        console.log(`Unexpected error in edge value test: ${error}`);
    }
    
    console.log("Error handling tests completed");
}

function main(): void {
    testEdgeCases();
    testEdgeCaseViolations();
    testErrorHandling();
}

main();