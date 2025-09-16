// Potential memory leak
function memoryLeak(): void {
    let arr = new Array(1000000)
    // No cleanup
}
