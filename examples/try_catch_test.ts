function testTryCatch() {
    try {
        let x = 10;
        throw "Something went wrong";
        return x;
    } catch (e) {
        _print("Caught error: " + e);
        return -1;
    } finally {
        _print("Cleanup complete");
    }
}