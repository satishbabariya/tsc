
// Async for...of loop testing
class AsyncDataProcessor {
    private data: number[];
    
    constructor(data: number[]) {
        this.data = data;
    }
    
    async *processAsync(): AsyncGenerator<number> {
        for (const item of this.data) {
            // Simulate async processing
            await new Promise(resolve => setTimeout(resolve, 100));
            yield item * 2;
        }
    }
}

class AsyncFileReader {
    private files: string[];
    
    constructor(files: string[]) {
        this.files = files;
    }
    
    async *readFiles(): AsyncGenerator<string> {
        for (const file of this.files) {
            // Simulate file reading
            await new Promise(resolve => setTimeout(resolve, 200));
            yield `Content of ${file}`;
        }
    }
}

// Test async for...of loops
async function testAsyncForOf() {
    const processor = new AsyncDataProcessor([1, 2, 3, 4, 5]);
    const processedData: number[] = [];
    
    // Async for...of loop
    for await (const item of processor.processAsync()) {
        processedData.push(item);
    }
    
    console.log("Processed data:", processedData);
}

async function testAsyncFileReading() {
    const reader = new AsyncFileReader(["file1.txt", "file2.txt", "file3.txt"]);
    const fileContents: string[] = [];
    
    // Async for...of loop
    for await (const content of reader.readFiles()) {
        fileContents.push(content);
    }
    
    console.log("File contents:", fileContents);
}

// Test with Promise.all and async iteration
async function testPromiseAllWithAsyncIteration() {
    const urls = [
        "https://api.example.com/users",
        "https://api.example.com/posts",
        "https://api.example.com/comments"
    ];
    
    const results: string[] = [];
    
    // Simulate async API calls
    for (const url of urls) {
        const result = await fetchData(url);
        results.push(result);
    }
    
    console.log("API results:", results);
}

// Mock fetch function
async function fetchData(url: string): Promise<string> {
    // Simulate API call
    await new Promise(resolve => setTimeout(resolve, 300));
    return `Data from ${url}`;
}

// Test with async generator
async function* asyncNumberGenerator(count: number): AsyncGenerator<number> {
    for (let i = 0; i < count; i++) {
        await new Promise(resolve => setTimeout(resolve, 100));
        yield i;
    }
}

async function testAsyncGenerator() {
    const numbers: number[] = [];
    
    for await (const num of asyncNumberGenerator(5)) {
        numbers.push(num);
    }
    
    console.log("Async generated numbers:", numbers);
}

// Run async tests
async function runAsyncTests() {
    await testAsyncForOf();
    await testAsyncFileReading();
    await testPromiseAllWithAsyncIteration();
    await testAsyncGenerator();
}

// runAsyncTests(); // Uncomment for async testing
