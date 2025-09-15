
// Performance stress test with many errors
function performanceStress(): void {
    for (let i = 0; i < 1000; i++) {
        let x: number = "hello" // Type error
        console.log(undefinedVariable) // Semantic error
        let y = x + undefinedVariable // Multiple errors
    }
}
