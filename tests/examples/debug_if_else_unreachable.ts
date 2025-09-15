// Debug if-else with unreachable code after
function main(): number {
    let x: number = 3;
    if (x > 5) {
        return 1;
    } else {
        return 0;
    }
    // This line should be unreachable
}