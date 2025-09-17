// Performance and Stress Tests for Generic Type Constraints
// Tests compilation performance and memory usage with complex generic scenarios

// Test 1: Deeply nested generic constraints
function processDeepNested<T extends string>(value: T): T {
    return value;
}

function processNestedLevel1<T extends string>(value: T): T {
    return processDeepNested<T>(value);
}

function processNestedLevel2<T extends string>(value: T): T {
    return processNestedLevel1<T>(value);
}

function processNestedLevel3<T extends string>(value: T): T {
    return processNestedLevel2<T>(value);
}

// Test 2: Multiple type parameters with complex constraints
interface ComplexConstraint1 {
    id: number;
    name: string;
}

interface ComplexConstraint2 {
    value: number;
    description: string;
}

interface ComplexConstraint3 {
    data: string;
    metadata: ComplexConstraint1;
}

function processMultipleComplex<T extends ComplexConstraint1, U extends ComplexConstraint2, V extends ComplexConstraint3>(
    item1: T, item2: U, item3: V
): string {
    return `${item1.name}: ${item2.description}: ${item3.data}`;
}

// Test 3: Generic class with multiple constraints
class ComplexGenericClass<T extends ComplexConstraint1, U extends ComplexConstraint2> {
    private items1: T[] = [];
    private items2: U[] = [];
    
    addItem1(item: T): void {
        this.items1.push(item);
    }
    
    addItem2(item: U): void {
        this.items2.push(item);
    }
    
    processItems(): string[] {
        const results: string[] = [];
        for (const item1 of this.items1) {
            for (const item2 of this.items2) {
                results.push(`${item1.name}: ${item2.description}`);
            }
        }
        return results;
    }
}

// Test 4: Recursive generic constraints
interface RecursiveConstraint {
    id: number;
    children: RecursiveConstraint[];
}

function processRecursive<T extends RecursiveConstraint>(item: T): number {
    let count = 1;
    for (const child of item.children) {
        count += processRecursive<RecursiveConstraint>(child);
    }
    return count;
}

// Test 5: Generic constraints with large data structures
interface LargeConstraint {
    id: number;
    data: string[];
    metadata: { [key: string]: string };
}

function processLarge<T extends LargeConstraint>(item: T): T {
    return item;
}

// Test 6: Generic constraints with function types
type ComplexFunction<T extends string> = (value: T) => T;

function processFunction<T extends string>(fn: ComplexFunction<T>, value: T): T {
    return fn(value);
}

// Test 7: Generic constraints with array operations
function processArray<T extends string[]>(arr: T): T {
    return arr.map(item => item.toUpperCase()) as T;
}

// Test 8: Generic constraints with object operations
function processObject<T extends { [key: string]: string }>(obj: T): T {
    const result = {} as T;
    for (const key in obj) {
        result[key] = obj[key].toUpperCase();
    }
    return result;
}

// Test data
const testString = "performance test";
const complexItem1: ComplexConstraint1 = {
    id: 1,
    name: "Item 1"
};

const complexItem2: ComplexConstraint2 = {
    value: 42,
    description: "Test item"
};

const complexItem3: ComplexConstraint3 = {
    data: "test data",
    metadata: complexItem1
};

const recursiveItem: RecursiveConstraint = {
    id: 1,
    children: [
        { id: 2, children: [] },
        { id: 3, children: [
            { id: 4, children: [] }
        ]}
    ]
};

const largeItem: LargeConstraint = {
    id: 1,
    data: Array.from({ length: 100 }, (_, i) => `item_${i}`),
    metadata: Object.fromEntries(
        Array.from({ length: 50 }, (_, i) => [`key_${i}`, `value_${i}`])
    )
};

const stringArray: string[] = ["a", "b", "c", "d", "e"];
const stringObject = {
    prop1: "value1",
    prop2: "value2",
    prop3: "value3"
};

// Performance test functions
function testPerformance(): void {
    console.log("=== Performance Tests ===");
    
    const startTime = Date.now();
    
    // Test nested function calls
    for (let i = 0; i < 1000; i++) {
        let result = processNestedLevel3<string>(testString);
    }
    
    const nestedTime = Date.now() - startTime;
    console.log(`Nested function calls (1000 iterations): ${nestedTime}ms`);
    
    // Test multiple complex constraints
    const startTime2 = Date.now();
    for (let i = 0; i < 1000; i++) {
        let result = processMultipleComplex<ComplexConstraint1, ComplexConstraint2, ComplexConstraint3>(
            complexItem1, complexItem2, complexItem3
        );
    }
    
    const complexTime = Date.now() - startTime2;
    console.log(`Complex constraints (1000 iterations): ${complexTime}ms`);
    
    // Test generic class operations
    const startTime3 = Date.now();
    let complexClass = new ComplexGenericClass<ComplexConstraint1, ComplexConstraint2>();
    for (let i = 0; i < 100; i++) {
        complexClass.addItem1({ id: i, name: `Item ${i}` });
        complexClass.addItem2({ value: i, description: `Description ${i}` });
    }
    let classResults = complexClass.processItems();
    
    const classTime = Date.now() - startTime3;
    console.log(`Generic class operations: ${classTime}ms, results: ${classResults.length}`);
    
    // Test recursive processing
    const startTime4 = Date.now();
    let recursiveCount = processRecursive<RecursiveConstraint>(recursiveItem);
    const recursiveTime = Date.now() - startTime4;
    console.log(`Recursive processing: ${recursiveTime}ms, count: ${recursiveCount}`);
    
    // Test large data processing
    const startTime5 = Date.now();
    let largeResult = processLarge<LargeConstraint>(largeItem);
    const largeTime = Date.now() - startTime5;
    console.log(`Large data processing: ${largeTime}ms, items: ${largeResult.data.length}`);
    
    // Test function processing
    const startTime6 = Date.now();
    let functionResult = processFunction<string>((s: string) => s.toUpperCase(), "test");
    const functionTime = Date.now() - startTime6;
    console.log(`Function processing: ${functionTime}ms, result: ${functionResult}`);
    
    // Test array processing
    const startTime7 = Date.now();
    let arrayResult = processArray<string[]>(stringArray);
    const arrayTime = Date.now() - startTime7;
    console.log(`Array processing: ${arrayTime}ms, result: ${arrayResult.join(", ")}`);
    
    // Test object processing
    const startTime8 = Date.now();
    let objectResult = processObject<{ [key: string]: string }>(stringObject);
    const objectTime = Date.now() - startTime8;
    console.log(`Object processing: ${objectTime}ms, keys: ${Object.keys(objectResult).length}`);
    
    const totalTime = Date.now() - startTime;
    console.log(`Total performance test time: ${totalTime}ms`);
}

// Memory stress test
function testMemoryStress(): void {
    console.log("=== Memory Stress Tests ===");
    
    // Create many generic instances
    const instances: ComplexGenericClass<ComplexConstraint1, ComplexConstraint2>[] = [];
    
    for (let i = 0; i < 1000; i++) {
        let instance = new ComplexGenericClass<ComplexConstraint1, ComplexConstraint2>();
        instance.addItem1({ id: i, name: `Stress Item ${i}` });
        instance.addItem2({ value: i, description: `Stress Description ${i}` });
        instances.push(instance);
    }
    
    console.log(`Created ${instances.length} generic class instances`);
    
    // Process all instances
    let totalResults = 0;
    for (const instance of instances) {
        let results = instance.processItems();
        totalResults += results.length;
    }
    
    console.log(`Processed ${totalResults} total results from all instances`);
    
    // Clear instances to test memory cleanup
    instances.length = 0;
    console.log("Cleared all instances for memory cleanup test");
}

// Compilation stress test
function testCompilationStress(): void {
    console.log("=== Compilation Stress Tests ===");
    
    // Test many generic function calls in a single function
    function stressTest(): void {
        let result1 = processNestedLevel3<string>("test1");
        let result2 = processNestedLevel3<string>("test2");
        let result3 = processNestedLevel3<string>("test3");
        let result4 = processNestedLevel3<string>("test4");
        let result5 = processNestedLevel3<string>("test5");
        
        let complex1 = processMultipleComplex<ComplexConstraint1, ComplexConstraint2, ComplexConstraint3>(
            complexItem1, complexItem2, complexItem3
        );
        let complex2 = processMultipleComplex<ComplexConstraint1, ComplexConstraint2, ComplexConstraint3>(
            complexItem1, complexItem2, complexItem3
        );
        let complex3 = processMultipleComplex<ComplexConstraint1, ComplexConstraint2, ComplexConstraint3>(
            complexItem1, complexItem2, complexItem3
        );
        
        console.log(`Stress test results: ${result1}, ${result2}, ${result3}, ${result4}, ${result5}`);
        console.log(`Complex results: ${complex1}, ${complex2}, ${complex3}`);
    }
    
    stressTest();
    console.log("Compilation stress test completed");
}

function main(): void {
    testPerformance();
    testMemoryStress();
    testCompilationStress();
}

main();