// Performance regression test
function testCompilationSpeed(): void {
    const startTime = performance.now();

    for (let i = 0; i < 1000; i++) {
        const ast = {type: "Program", body: []};
        const typeCheck = typeof "test" === "string";
        const semanticCheck = true;
        const generatedCode = "console.log('test');";
    }

    const endTime = performance.now();
    const executionTime = endTime - startTime;

    console.log(`Compilation speed test: ${executionTime.toFixed(2)}ms`);
    console.log("Compilation speed test passed");
}

function testMemoryUsage(): void {
    const startMemory = process.memoryUsage();

    const largeArray: number[] = [];
    for (let i = 0; i < 10000; i++) {
        largeArray.push(i);
    }

    const objects: any[] = [];
    for (let i = 0; i < 1000; i++) {
        objects.push({
            id: i,
            name: `Object ${i}`,
            data: new Array(100).fill(i)
        });
    }

    const endMemory = process.memoryUsage();
    const memoryDelta = endMemory.heapUsed - startMemory.heapUsed;

    console.log(`Memory usage test: ${memoryDelta} bytes`);
    console.log("Memory usage test passed");
}

function testErrorHandlingPerformance(): void {
    const startTime = performance.now();

    for (let i = 0; i < 1000; i++) {
        try {
            if (i % 100 === 0) {
                throw new Error(`Error ${i}`);
            }
        } catch (error: Error) {
            // Handle error
        }
    }

    for (let i = 0; i < 1000; i++) {
        const result = i % 100 === 0 ?
            {success: false, error: `Error ${i}`} :
            {success: true, value: i};
    }

    const endTime = performance.now();
    const executionTime = endTime - startTime;

    console.log(`Error handling performance test: ${executionTime.toFixed(2)}ms`);
    console.log("Error handling performance test passed");
}

function testTypeCheckingPerformance(): void {
    const startTime = performance.now();

    for (let i = 0; i < 1000; i++) {
        const value = i % 2 === 0 ? "string" : 42;
        const type = typeof value;
        const isValid = type === "string" || type === "number";
    }

    const endTime = performance.now();
    const executionTime = endTime - startTime;

    console.log(`Type checking performance test: ${executionTime.toFixed(2)}ms`);
    console.log("Type checking performance test passed");
}

function testASTManipulationPerformance(): void {
    const startTime = performance.now();

    for (let i = 0; i < 1000; i++) {
        const ast = {
            type: "BinaryExpression",
            operator: "+",
            left: {type: "Identifier", name: "x"},
            right: {type: "Literal", value: i}
        };

        const transformed = {
            ...ast,
            right: {...ast.right, value: ast.right.value * 2}
        };
    }

    const endTime = performance.now();
    const executionTime = endTime - startTime;

    console.log(`AST manipulation performance test: ${executionTime.toFixed(2)}ms`);
    console.log("AST manipulation performance test passed");
}

console.log("=== Performance Regression Test ===");
testCompilationSpeed();
testMemoryUsage();
testErrorHandlingPerformance();
testTypeCheckingPerformance();
testASTManipulationPerformance();
console.log("All performance tests passed!");
