// Performance tests for generics
interface PerformanceTest {
    name: string;
    value: number;
}

// Generic performance test class
class PerformanceTester<T> {
    private results: Array<{ operation: string; time: number; data: T[] }> = [];

    measureOperation(operation: string, data: T[], operationFn: (data: T[]) => T[]): void {
        const start = performance.now();
        const result = operationFn(data);
        const end = performance.now();

        this.results.push({
            operation,
            time: end - start,
            data: result
        });
    }

    getResults(): Array<{ operation: string; time: number }> {
        return this.results.map(r => ({operation: r.operation, time: r.time}));
    }

    getAverageTime(): number {
        const total = this.results.reduce((sum, r) => sum + r.time, 0);
        return total / this.results.length;
    }
}

// Generic sorting performance test
function testSortingPerformance<T extends { value: number }>(items: T[]): T[] {
    return items.sort((a, b) => a.value - b.value);
}

// Generic filtering performance test
function testFilteringPerformance<T extends { value: number }>(items: T[], threshold: number): T[] {
    return items.filter(item => item.value > threshold);
}

// Generic mapping performance test
function testMappingPerformance<T extends { value: number }>(items: T[]): T[] {
    return items.map(item => ({...item, value: item.value * 2}));
}

// Generic reduction performance test
function testReductionPerformance<T extends { value: number }>(items: T[]): number {
    return items.reduce((sum, item) => sum + item.value, 0);
}

// Generic search performance test
function testSearchPerformance<T extends { name: string }>(items: T[], searchTerm: string): T[] {
    return items.filter(item => item.name.includes(searchTerm));
}

// Create test data
const createTestData = (count: number): PerformanceTest[] => {
    return Array.from({length: count}, (_, i) => ({
        name: `Test${i}`,
        value: Math.floor(Math.random() * 1000)
    }));
};

// Run performance tests
const tester = new PerformanceTester<PerformanceTest>();
const testData = createTestData(10000);

console.log('Running performance tests with', testData.length, 'items...');

// Test sorting
tester.measureOperation('Sorting', testData, testSortingPerformance);

// Test filtering
tester.measureOperation('Filtering', testData, (data) => testFilteringPerformance(data, 500));

// Test mapping
tester.measureOperation('Mapping', testData, testMappingPerformance);

// Test reduction
tester.measureOperation('Reduction', testData, (data) => {
    const result = testReductionPerformance(data);
    return [{name: 'sum', value: result}];
});

// Test search
tester.measureOperation('Search', testData, (data) => testSearchPerformance(data, 'Test'));

// Display results
const results = tester.getResults();
console.log('Performance Test Results:');
results.forEach(result => {
    console.log(`${result.operation}: ${result.time.toFixed(2)}ms`);
});

console.log(`Average time: ${tester.getAverageTime().toFixed(2)}ms`);

// Memory usage test
function testMemoryUsage<T>(createFn: () => T, count: number): void {
    const startMemory = (performance as any).memory?.usedJSHeapSize || 0;

    const items: T[] = [];
    for (let i = 0; i < count; i++) {
        items.push(createFn());
    }

    const endMemory = (performance as any).memory?.usedJSHeapSize || 0;
    const memoryUsed = endMemory - startMemory;

    console.log(`Memory test: ${count} items created, ${memoryUsed} bytes used`);
}

// Test memory usage
testMemoryUsage(() => ({name: 'Test', value: Math.random()}), 1000);
testMemoryUsage(() => ({name: 'Test', value: Math.random()}), 10000);
testMemoryUsage(() => ({name: 'Test', value: Math.random()}), 100000);
