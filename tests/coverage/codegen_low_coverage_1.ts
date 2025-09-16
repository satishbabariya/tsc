// Complex memory operations for code generation coverage
function complexMemoryOperations(): void {
    // Large array allocations
    let largeArray = new Array(10000);
    for (let i = 0; i < 10000; i++) {
        largeArray[i] = i * i;
    }

    // Complex object allocations
    let complexObject = {};
    for (let i = 0; i < 1000; i++) {
        complexObject[`key${i}`] = {
            value: i,
            nested: {
                deep: {
                    value: i * 2
                }
            }
        };
    }

    // Complex string operations
    let complexString = "";
    for (let i = 0; i < 1000; i++) {
        complexString += `item${i}`;
    }

    // Complex function calls
    function recursiveFunction(n: number): number {
        if (n <= 0) return 1;
        return n * recursiveFunction(n - 1);
    }

    let factorial = recursiveFunction(10);

    // Complex mathematical operations
    let complexMath = 0;
    for (let i = 0; i < 1000; i++) {
        complexMath += Math.sqrt(i) * Math.sin(i) + Math.cos(i) * Math.tan(i);
    }

    console.log(largeArray.length, Object.keys(complexObject).length, complexString.length, factorial, complexMath);
}
