// Dead code elimination test
function deadCode(): number {
    let x = 42
    if (false) {
        return x
    }
    return 0
}
