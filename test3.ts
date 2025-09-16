// Simple test with local variable to avoid deferred initialization issue
function add(a: number, b: number): number {
    return a + b;
}

function main(): void {
    let result: number = add(5, 3);
    console.log("Result:", result);
}

main();