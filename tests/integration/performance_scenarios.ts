// Performance scenarios for for...of loops
function testPerformanceScenarios() {
    // Test with large arrays
    const largeArray = Array.from({length: 100000}, (_, i) => i);

    console.time("for...of loop");
    let sum = 0;
    for (const num of largeArray) {
        sum += num;
    }
    console.timeEnd("for...of loop");
    console.log("Sum:", sum);

    // Test with nested loops
    const matrix = Array.from({length: 1000}, () =>
        Array.from({length: 1000}, (_, i) => Math.random())
    );

    console.time("nested for...of loops");
    let total = 0;
    for (const row of matrix) {
        for (const cell of row) {
            total += cell;
        }
    }
    console.timeEnd("nested for...of loops");
    console.log("Total:", total);

    // Test with object iteration
    const largeObject = Object.fromEntries(
        Array.from({length: 10000}, (_, i) => [`key${i}`, i])
    );

    console.time("object iteration");
    const values: number[] = [];
    for (const value of Object.values(largeObject)) {
        values.push(value);
    }
    console.timeEnd("object iteration");
    console.log("Values count:", values.length);
}

// Test with different iteration methods
function testIterationMethods() {
    const numbers = Array.from({length: 100000}, (_, i) => i);

    // for...of loop
    console.time("for...of");
    let sum1 = 0;
    for (const num of numbers) {
        sum1 += num;
    }
    console.timeEnd("for...of");

    // traditional for loop
    console.time("for loop");
    let sum2 = 0;
    for (let i = 0; i < numbers.length; i++) {
        sum2 += numbers[i];
    }
    console.timeEnd("for loop");

    // forEach
    console.time("forEach");
    let sum3 = 0;
    numbers.forEach(num => {
        sum3 += num;
    });
    console.timeEnd("forEach");

    // map
    console.time("map");
    const doubled = numbers.map(num => num * 2);
    console.timeEnd("map");

    console.log("Sums:", sum1, sum2, sum3);
    console.log("Doubled length:", doubled.length);
}

// Test with memory usage
function testMemoryUsage() {
    const createLargeArray = (size: number) =>
        Array.from({length: size}, (_, i) => ({id: i, data: `data${i}`}));

    // Test with different array sizes
    const sizes = [1000, 10000, 100000];

    for (const size of sizes) {
        console.log(`Testing with array size: ${size}`);

        const array = createLargeArray(size);

        console.time(`iteration-${size}`);
        const results: string[] = [];
        for (const item of array) {
            results.push(item.data);
        }
        console.timeEnd(`iteration-${size}`);

        console.log(`Results count: ${results.length}`);

        // Clear array to free memory
        array.length = 0;
    }
}

// Test with async performance
async function testAsyncPerformance() {
    const createAsyncGenerator = (count: number, delay: number) =>
        async function* () {
            for (let i = 0; i < count; i++) {
                await new Promise(resolve => setTimeout(resolve, delay));
                yield i;
            }
        };

    const asyncGen = createAsyncGenerator(100, 10);

    console.time("async for...of");
    const asyncResults: number[] = [];
    for await (const num of asyncGen()) {
        asyncResults.push(num);
    }
    console.timeEnd("async for...of");

    console.log("Async results count:", asyncResults.length);
}

// Test with break and continue
function testBreakAndContinue() {
    const numbers = Array.from({length: 1000000}, (_, i) => i);

    console.time("for...of with break");
    let sum = 0;
    for (const num of numbers) {
        sum += num;
        if (num > 100000) {
            break;
        }
    }
    console.timeEnd("for...of with break");
    console.log("Sum with break:", sum);

    console.time("for...of with continue");
    sum = 0;
    for (const num of numbers) {
        if (num % 2 === 0) {
            continue;
        }
        sum += num;
        if (num > 100000) {
            break;
        }
    }
    console.timeEnd("for...of with continue");
    console.log("Sum with continue:", sum);
}

// Run performance tests
testPerformanceScenarios();
testIterationMethods();
testMemoryUsage();
testBreakAndContinue();
// testAsyncPerformance(); // Uncomment for async testing
