function testTryFinally() {
    try {
        let result = 42;
        return result;
    } finally {
        _print("Always executed");
    }
}