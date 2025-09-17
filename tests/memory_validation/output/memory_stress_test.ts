// Simple memory stress test
function processString<T extends string>(value: T): T {
    return value;
}

function main(): void {
    // Stress test with many calls
    for (let i = 0; i < 10000; i++) {
        let result = processString<string>("stress_test_" + i.toString());
    }
}

main();
