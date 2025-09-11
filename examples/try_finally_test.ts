function testTryFinally() {
    try {
        let result = 42;
        return result;
    } finally {
        console.log("Always executed");
    }
}