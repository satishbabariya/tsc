
// Custom iterable testing
class NumberRange implements Iterable<number> {
    private start: number;
    private end: number;
    private step: number;
    
    constructor(start: number, end: number, step: number = 1) {
        this.start = start;
        this.end = end;
        this.step = step;
    }
    
    *[Symbol.iterator](): Iterator<number> {
        for (let i = this.start; i < this.end; i += this.step) {
            yield i;
        }
    }
}

class FibonacciSequence implements Iterable<number> {
    private count: number;
    
    constructor(count: number) {
        this.count = count;
    }
    
    *[Symbol.iterator](): Iterator<number> {
        let a = 0, b = 1;
        for (let i = 0; i < this.count; i++) {
            yield a;
            [a, b] = [b, a + b];
        }
    }
}

class StringWords implements Iterable<string> {
    private text: string;
    
    constructor(text: string) {
        this.text = text;
    }
    
    *[Symbol.iterator](): Iterator<string> {
        const words = this.text.split(/\s+/);
        for (const word of words) {
            yield word;
        }
    }
}

// Test custom iterables
function testCustomIterables() {
    // Test NumberRange
    const range = new NumberRange(1, 10, 2);
    const oddNumbers: number[] = [];
    
    for (const num of range) {
        oddNumbers.push(num);
    }
    
    console.log("Odd numbers:", oddNumbers);
    
    // Test FibonacciSequence
    const fibonacci = new FibonacciSequence(10);
    const fibNumbers: number[] = [];
    
    for (const num of fibonacci) {
        fibNumbers.push(num);
    }
    
    console.log("Fibonacci numbers:", fibNumbers);
    
    // Test StringWords
    const words = new StringWords("Hello world TypeScript programming");
    const wordList: string[] = [];
    
    for (const word of words) {
        wordList.push(word);
    }
    
    console.log("Words:", wordList);
}

// Test with async iterable
class AsyncNumberGenerator implements AsyncIterable<number> {
    private count: number;
    private delay: number;
    
    constructor(count: number, delay: number = 100) {
        this.count = count;
        this.delay = delay;
    }
    
    async *[Symbol.asyncIterator](): AsyncIterator<number> {
        for (let i = 0; i < this.count; i++) {
            await new Promise(resolve => setTimeout(resolve, this.delay));
            yield i;
        }
    }
}

// Test async iterable
async function testAsyncIterable() {
    const asyncGen = new AsyncNumberGenerator(5, 50);
    const asyncNumbers: number[] = [];
    
    for await (const num of asyncGen) {
        asyncNumbers.push(num);
    }
    
    console.log("Async numbers:", asyncNumbers);
}

// Test with generator functions
function* numberGenerator(start: number, end: number): Generator<number> {
    for (let i = start; i < end; i++) {
        yield i;
    }
}

function* fibonacciGenerator(count: number): Generator<number> {
    let a = 0, b = 1;
    for (let i = 0; i < count; i++) {
        yield a;
        [a, b] = [b, a + b];
    }
}

function testGeneratorFunctions() {
    // Test number generator
    const numbers: number[] = [];
    for (const num of numberGenerator(1, 6)) {
        numbers.push(num);
    }
    
    console.log("Generated numbers:", numbers);
    
    // Test fibonacci generator
    const fibNumbers: number[] = [];
    for (const num of fibonacciGenerator(8)) {
        fibNumbers.push(num);
    }
    
    console.log("Generated fibonacci:", fibNumbers);
}

// Run tests
testCustomIterables();
testGeneratorFunctions();
// testAsyncIterable(); // Uncomment for async testing
