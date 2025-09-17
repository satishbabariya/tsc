// Memory Leak Detection Tests for Generic Type Constraints
// These tests specifically target potential memory leaks in generic constraint handling

// Test 1: Generic function with large data (test for memory leaks in large objects)
function processLargeString<T extends string>(value: T): T {
    return value;
}

function processLargeNumber<T extends number>(value: T): T {
    return value;
}

// Test 2: Generic class with memory-intensive operations
class MemoryIntensiveContainer<T extends string> {
    private data: T[];
    
    constructor() {
        this.data = [];
    }
    
    addData(value: T): void {
        this.data.push(value);
    }
    
    getData(): T[] {
        return this.data;
    }
    
    clearData(): void {
        this.data = [];
    }
}

// Test 3: Generic function with recursive calls (test for stack overflow and memory leaks)
function processRecursive<T extends number>(value: T, depth: number): T {
    if (depth <= 0) {
        return value;
    }
    return processRecursive<T>(value, depth - 1);
}

// Test 4: Generic constraint with complex object (test for object lifecycle management)
interface ComplexObject {
    id: number;
    data: string;
    metadata: { [key: string]: string };
}

function processComplexObject<T extends ComplexObject>(obj: T): T {
    return obj;
}

// Test functions that target memory leak scenarios
function testMemoryLeaks(): void {
    // These tests target specific memory leak scenarios
    
    // Large string processing
    let largeString = "x".repeat(10000);  // 10KB string
    for (let i = 0; i < 100; i++) {
        let result = processLargeString<string>(largeString);
    }
    
    // Large number processing
    for (let i = 0; i < 100; i++) {
        let result = processLargeNumber<number>(999999999);
    }
    
    // Memory-intensive container operations
    let container = new MemoryIntensiveContainer<string>();
    for (let i = 0; i < 1000; i++) {
        container.addData("data_" + i.toString());
    }
    let data = container.getData();
    container.clearData();
    
    // Recursive calls (test for stack management)
    for (let i = 0; i < 100; i++) {
        let result = processRecursive<number>(42, 50);
    }
    
    // Complex object processing
    for (let i = 0; i < 100; i++) {
        let complexObj: ComplexObject = {
            id: i,
            data: "complex_data_" + i.toString(),
            metadata: {
                key1: "value1_" + i.toString(),
                key2: "value2_" + i.toString(),
                key3: "value3_" + i.toString()
            }
        };
        let result = processComplexObject<ComplexObject>(complexObj);
    }
    
    // All leak tests completed
    let allLeakTestsCompleted = true;
}

function main(): void {
    testMemoryLeaks();
}

main();