// Complex nested errors
function complexErrors(): void {
    if (true) {
        let x: number = "hello" // Type error
        console.log(undefinedVariable) // Semantic error
        let y = x + undefinedVariable // Multiple errors
    }
}
