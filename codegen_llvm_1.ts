
// Complex expression that might cause LLVM generation issues
function complexExpression(): number {
    let result = 0
    for (let i = 0; i < 1000000; i++) {
        result += Math.sqrt(i) * Math.sin(i) + Math.cos(i)
    }
    return result
}
